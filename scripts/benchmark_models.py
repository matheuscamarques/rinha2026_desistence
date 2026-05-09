#!/usr/bin/env python3
"""Benchmark KNN, EIF, CatBoost, SVM, Logistic Regression and LightGBM on the same sample.

The script evaluates:
  - models/knn_v1.onnx
    - models/eif_v1.joblib
    - models/catboost_v1.onnx
  - models/svm_rbf_sample.onnx
  - models/logreg.onnx
  - models/lgb_model.txt

All models are run on the raw 14-feature representation so the comparison is
apples-to-apples.
"""

from __future__ import annotations

import argparse
import gzip
import sys
from pathlib import Path

import ijson
import lightgbm as lgb
import joblib
import numpy as np
import onnxruntime as ort
from sklearn.metrics import (
    accuracy_score,
    average_precision_score,
    confusion_matrix,
    f1_score,
    precision_score,
    recall_score,
    roc_auc_score,
)
from sklearn.model_selection import train_test_split

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))


def load_references(gz_path: Path, max_samples: int | None = None):
    x_rows = []
    y_rows = []
    with gzip.open(gz_path, 'rt', encoding='utf-8') as handle:
        for index, item in enumerate(ijson.items(handle, 'item'), 1):
            if max_samples is not None and index > max_samples:
                break
            vector = item.get('vector', [])
            if len(vector) < 14:
                continue
            x_rows.append([float(value) for value in vector[:14]])
            y_rows.append(1 if item.get('label') == 'fraud' else 0)
    return np.asarray(x_rows, dtype=np.float32), np.asarray(y_rows, dtype=np.int32)


def onnx_predict_proba(model_path: Path, X: np.ndarray) -> np.ndarray:
    session = ort.InferenceSession(str(model_path), providers=['CPUExecutionProvider'])
    input_name = session.get_inputs()[0].name
    outputs = session.run(None, {input_name: X.astype(np.float32)})

    for output in outputs:
        array = np.asarray(output)
        if array.ndim == 2 and array.shape[1] >= 2:
            return array[:, 1].astype(np.float64)

    for output in outputs:
        if isinstance(output, list) and output and isinstance(output[0], dict):
            probs = []
            for item in output:
                if 1 in item:
                    probs.append(float(item[1]))
                elif '1' in item:
                    probs.append(float(item['1']))
                else:
                    probs.append(float(item[max(item.keys())]))
            return np.asarray(probs, dtype=np.float64)

    raise RuntimeError(f'Could not extract probabilities from {model_path}')


def lgb_predict_proba(model_path: Path, X: np.ndarray) -> np.ndarray:
    booster = lgb.Booster(model_file=str(model_path))
    return booster.predict(X).astype(np.float64)


def joblib_predict_proba(model_path: Path, X: np.ndarray) -> np.ndarray:
    obj = joblib.load(model_path)
    model = obj['model'] if isinstance(obj, dict) and 'model' in obj else obj
    return model.predict_proba(X)[:, 1].astype(np.float64)


def lof_predict_proba(model_path: Path, X: np.ndarray) -> np.ndarray:
    """Extract probability scores from LOF model."""
    obj = joblib.load(model_path)
    model = obj['model'] if isinstance(obj, dict) and 'model' in obj else obj
    
    # Apply preprocessing (imputer + scaler)
    X_processed = model.named_steps['imputer'].transform(X)
    X_processed = model.named_steps['scaler'].transform(X_processed)
    
    # Get LOF decision function (negative outlier factor)
    lof_model = model.named_steps['lof']
    scores = lof_model.decision_function(X_processed)
    
    # Convert to probability-like scores using logistic sigmoid
    proba = 1 / (1 + np.exp(scores))
    return proba.astype(np.float64)


def stacking_predict_proba(model_path: Path, X: np.ndarray) -> np.ndarray:
    """Extract probability scores from Stacking Ensemble."""
    artifact = joblib.load(model_path)
    base_models = artifact['base_models']
    meta_learner = artifact['meta_learner']
    scaler = artifact['scaler']
    imputer = artifact['imputer']
    
    # Generate base model predictions
    lgb_proba = base_models['lgb'].predict_proba(X)[:, 1]
    cb_proba = base_models['catboost'].predict_proba(X)[:, 1]
    knn_proba = base_models['knn'].predict_proba(X)[:, 1]
    
    # Stack predictions into meta-features
    X_meta = np.column_stack([lgb_proba, cb_proba, knn_proba])
    
    # Preprocess meta-features
    X_meta = imputer.transform(X_meta)
    X_meta = scaler.transform(X_meta)
    
    # Meta-learner final prediction
    proba = meta_learner.predict_proba(X_meta)[:, 1]
    return proba.astype(np.float64)


def evaluate(name: str, y_true: np.ndarray, y_proba: np.ndarray):
    y_pred = (y_proba >= 0.5).astype(np.int32)
    tn, fp, fn, tp = confusion_matrix(y_true, y_pred).ravel()
    return {
        'name': name,
        'accuracy': accuracy_score(y_true, y_pred),
        'precision': precision_score(y_true, y_pred, zero_division=0),
        'recall': recall_score(y_true, y_pred, zero_division=0),
        'f1': f1_score(y_true, y_pred, zero_division=0),
        'roc_auc': roc_auc_score(y_true, y_proba),
        'pr_auc': average_precision_score(y_true, y_proba),
        'tp': int(tp),
        'tn': int(tn),
        'fp': int(fp),
        'fn': int(fn),
    }


def print_table(rows):
    print('\n' + '=' * 108)
    print(f"{'Model':<20} {'ROC AUC':>10} {'PR AUC':>10} {'Acc':>10} {'Prec':>10} {'Rec':>10} {'F1':>10}")
    print('-' * 108)
    for row in rows:
        print(
            f"{row['name']:<20} {row['roc_auc']:10.6f} {row['pr_auc']:10.6f} {row['accuracy']:10.6f} "
            f"{row['precision']:10.6f} {row['recall']:10.6f} {row['f1']:10.6f}"
        )
    print('=' * 108)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument('--bin', default='priv/references.json.gz')
    parser.add_argument('--max-samples', type=int, default=200000)
    parser.add_argument('--knn-onnx', default='models/knn_v1.onnx')
    parser.add_argument('--eif-joblib', default='models/eif_v1.joblib')
    parser.add_argument('--lof-joblib', default='models/lof_v1.joblib')
    parser.add_argument('--stacking-joblib', default='models/stacking_ensemble_v1.joblib')
    parser.add_argument('--catboost-onnx', default='models/catboost_v1.onnx')
    parser.add_argument('--svm-onnx', default='models/svm_rbf_sample.onnx')
    parser.add_argument('--logreg-onnx', default='models/logreg.onnx')
    parser.add_argument('--lgb-model', default='models/lgb_model.txt')
    args = parser.parse_args()

    X, y = load_references(Path(args.bin), max_samples=args.max_samples)
    _, X_test, _, y_test = train_test_split(X, y, test_size=0.2, stratify=y, random_state=42)

    rows = [
        evaluate('stacking_ensemble', y_test, stacking_predict_proba(Path(args.stacking_joblib), X_test)),
        evaluate('lgb_model', y_test, lgb_predict_proba(Path(args.lgb_model), X_test)),
        evaluate('svm_onnx', y_test, onnx_predict_proba(Path(args.svm_onnx), X_test)),
        evaluate('catboost_onnx', y_test, onnx_predict_proba(Path(args.catboost_onnx), X_test)),
        evaluate('logreg_onnx', y_test, onnx_predict_proba(Path(args.logreg_onnx), X_test)),
        evaluate('knn_onnx', y_test, onnx_predict_proba(Path(args.knn_onnx), X_test)),
        evaluate('eif_joblib', y_test, joblib_predict_proba(Path(args.eif_joblib), X_test)),
        evaluate('lof_joblib', y_test, lof_predict_proba(Path(args.lof_joblib), X_test)),
    ]

    rows = sorted(rows, key=lambda row: (row['roc_auc'], row['pr_auc'], row['f1']), reverse=True)
    print_table(rows)

    print('\nRanking:')
    for index, row in enumerate(rows, 1):
        print(f"{index}. {row['name']}  (ROC AUC={row['roc_auc']:.6f}, PR AUC={row['pr_auc']:.6f}, F1={row['f1']:.6f})")

    print('\nConfusion matrices:')
    for row in rows:
        print(f"{row['name']}: TP={row['tp']} TN={row['tn']} FP={row['fp']} FN={row['fn']}")

    return 0


if __name__ == '__main__':
    raise SystemExit(main())
