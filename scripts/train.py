#!/usr/bin/env python3
"""Train models from raw float features (no quantization).

Reads from priv/references.json.gz (original float data) instead of quantized references.bin.

Produces:
- models/logreg_pipeline.joblib (sklearn Pipeline: scaler+LogReg)
- models/lgb_model.txt (LightGBM)
- models/logreg.onnx (optional)
- models/lgb.onnx (optional)
"""
from pathlib import Path
import numpy as np
import joblib
import gzip
import json
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LogisticRegression
from sklearn.pipeline import Pipeline
from sklearn.metrics import roc_auc_score, average_precision_score, classification_report
import lightgbm as lgb
import argparse

try:
    from skl2onnx import convert_sklearn
    from skl2onnx.common.data_types import FloatTensorType
    SKL2ONNX = True
except Exception:
    SKL2ONNX = False

try:
    from onnxmltools import convert_lightgbm
    from onnxmltools.utils import save_model as save_onnx
    ONNXTOOLS = True
except Exception:
    ONNXTOOLS = False


def read_references_from_json(json_path: Path):
    """Read references from original JSON format (float precision, no quantization)."""
    X_list = []
    y_list = []
    
    print(f"Reading references from {json_path}...")
    
    # Try ijson first for memory-efficient streaming
    try:
        import ijson
        with gzip.open(json_path, 'rb') as f:
            for line_num, entry in enumerate(ijson.items(f, 'item'), 1):
                if line_num % 100000 == 0:
                    print(f"  Loaded {line_num} records...")
                
                # Extract 14 features from the JSON
                features = entry.get('vector', [])
                if len(features) < 14:
                    print(f"Warning: Line {line_num} has {len(features)} features, expected 14")
                    continue
                
                features = [float(f) for f in features[:14]]
                label = 1 if entry.get('label') == 'fraud' else 0
                
                X_list.append(features)
                y_list.append(label)
        
        print(f"Total records loaded: {len(X_list)}")
        return np.array(X_list, dtype=np.float32), np.array(y_list, dtype=np.int32)
    except Exception as ijson_err:
        print(f"ijson failed: {ijson_err}, falling back to json.load()...")
    
    # Fallback: load entire JSON array into memory
    try:
        with gzip.open(json_path, 'rt', encoding='utf-8') as f:
            data = json.load(f)
        
        if not isinstance(data, list):
            raise ValueError(f"Expected JSON array at root, got {type(data)}")
        
        for line_num, entry in enumerate(data, 1):
            if line_num % 100000 == 0:
                print(f"  Loaded {line_num} records...")
            
            # Extract 14 features from the JSON
            features = entry.get('vector', [])
            if len(features) < 14:
                print(f"Warning: Record {line_num} has {len(features)} features, expected 14")
                continue
            
            features = [float(f) for f in features[:14]]
            label = 1 if entry.get('label') == 'fraud' else 0
            
            X_list.append(features)
            y_list.append(label)
        
        print(f"Total records loaded: {len(X_list)}")
        return np.array(X_list, dtype=np.float32), np.array(y_list, dtype=np.int32)
    
    except Exception as e:
        print(f"Fatal: Failed to load JSON: {e}")
        raise


def read_references(bin_path: Path):
    """Fallback: Read from quantized references.bin (legacy)."""
    record_size = 16  # 14 features + 1 label + 1 padding
    data = np.fromfile(str(bin_path), dtype=np.uint8)
    if data.size % record_size != 0:
        raise ValueError("references.bin has unexpected size")
    n = data.size // record_size
    data = data.reshape((n, record_size))
    X = data[:, :14].astype(np.float32)
    y = data[:, 14].astype(np.int32)
    return X, y


def main():
    p = argparse.ArgumentParser()
    p.add_argument('--bin', default='priv/references.bin')
    p.add_argument('--out', default='models')
    p.add_argument('--seed', type=int, default=42)
    args = p.parse_args()

    bin_path = Path(args.bin)
    out_dir = Path(args.out)
    out_dir.mkdir(parents=True, exist_ok=True)

    # Try to read from original JSON (float precision) first
    json_path = Path("priv/references.json.gz")
    if json_path.exists():
        print("Reading from original float data (references.json.gz)...")
        X, y = read_references_from_json(json_path)
    else:
        print("Falling back to quantized data (references.bin)...")
        X, y = read_references(bin_path)
    
    print(f"Loaded {X.shape[0]} records, positive ratio={y.mean():.4f}")

    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=0.2, stratify=y, random_state=args.seed
    )

    # Logistic regression pipeline (scaled)
    scaler = StandardScaler()
    lr = LogisticRegression(max_iter=2000, class_weight='balanced', solver='saga')
    pipe = Pipeline([('scaler', scaler), ('lr', lr)])
    pipe.fit(X_train, y_train)

    y_proba_lr = pipe.predict_proba(X_test)[:, 1]
    print("LogReg ROC AUC:", roc_auc_score(y_test, y_proba_lr))
    print("LogReg PR AUC:", average_precision_score(y_test, y_proba_lr))
    print(classification_report(y_test, pipe.predict(X_test)))

    joblib.dump(pipe, out_dir / 'logreg_pipeline.joblib')

    # Try export to ONNX (skl2onnx)
    if SKL2ONNX:
        try:
            initial_type = [('input', FloatTensorType([None, X.shape[1]]))]
            onx = convert_sklearn(pipe, initial_types=initial_type)
            with open(out_dir / 'logreg.onnx', 'wb') as f:
                f.write(onx.SerializeToString())
            print('Exported logreg.onnx')
        except Exception as e:
            print('logreg ONNX export failed:', e)
    else:
        print('skl2onnx not installed; skipping logreg ONNX export')

    # LightGBM
    dtrain = lgb.Dataset(X_train, label=y_train)
    params = {
        'objective': 'binary',
        'metric': 'auc',
        'verbosity': -1,
        'learning_rate': 0.05,
        'num_leaves': 64,
    }
    bst = lgb.train(params, dtrain, num_boost_round=500)
    bst.save_model(str(out_dir / 'lgb_model.txt'))
    print('Saved LightGBM model')

    y_proba_lgb = bst.predict(X_test)
    print('LGBM ROC AUC:', roc_auc_score(y_test, y_proba_lgb))
    print('LGBM PR AUC:', average_precision_score(y_test, y_proba_lgb))
    print(classification_report(y_test, (y_proba_lgb > 0.5).astype(int)))

    # Try export LightGBM to ONNX via onnxmltools
    if ONNXTOOLS:
        try:
            onx_lgb = convert_lightgbm(bst, name='lgbm_model')
            save_onnx(onx_lgb, str(out_dir / 'lgb.onnx'))
            print('Exported lgb.onnx')
        except Exception as e:
            print('LightGBM ONNX export failed:', e)
    else:
        print('onnxmltools not installed; skipping LightGBM ONNX export')


if __name__ == '__main__':
    main()
