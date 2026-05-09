#!/usr/bin/env python3
"""Compare SVM ONNX against logreg ONNX and LightGBM baseline on the same data.

The comparison uses the raw 14-feature representation so all three models can
be evaluated on the same inputs:
  - models/svm_rbf_sample.onnx
  - models/logreg.onnx
  - models/lgb_model.txt

Outputs ROC AUC, PR AUC, threshold-0.5 classification metrics, and a compact
summary table.
"""

from __future__ import annotations

import argparse
import gzip
import json
from pathlib import Path

import ijson
import lightgbm as lgb
import numpy as np
import onnxruntime as ort
from sklearn.metrics import (
    accuracy_score,
    average_precision_score,
    classification_report,
    confusion_matrix,
    f1_score,
    precision_score,
    recall_score,
    roc_auc_score,
)
from sklearn.model_selection import train_test_split


def load_references(gz_path: Path, max_samples: int | None = None):
    X = []
    y = []
    with gzip.open(gz_path, 'rt', encoding='utf-8') as f:
        for idx, item in enumerate(ijson.items(f, 'item'), 1):
            if max_samples is not None and idx > max_samples:
                break
            vec = item.get('vector', [])
            if len(vec) < 14:
                continue
            X.append([float(v) for v in vec[:14]])
            y.append(1 if item.get('label') == 'fraud' else 0)
    return np.asarray(X, dtype=np.float32), np.asarray(y, dtype=np.int32)


def onnx_predict_proba(model_path: Path, X: np.ndarray) -> np.ndarray:
    session = ort.InferenceSession(str(model_path), providers=['CPUExecutionProvider'])
    input_name = session.get_inputs()[0].name
    outputs = session.run(None, {input_name: X.astype(np.float32)})

    # Try to find a 2D probability output first.
    for out in outputs:
        arr = np.asarray(out)
        if arr.ndim == 2 and arr.shape[1] >= 2:
            return arr[:, 1].astype(np.float64)

    # Fallback: some converters return seq(map(class_id -> probability)).
    for out in outputs:
        if isinstance(out, list) and out and isinstance(out[0], dict):
            probs = []
            for item in out:
                if 1 in item:
                    probs.append(float(item[1]))
                elif '1' in item:
                    probs.append(float(item['1']))
                else:
                    # If the positive class is not labeled as 1, use the max key.
                    key = max(item.keys())
                    probs.append(float(item[key]))
            return np.asarray(probs, dtype=np.float64)

    if len(outputs) >= 2:
        out = outputs[1]
        if isinstance(out, list) and out and isinstance(out[0], dict):
            probs = []
            for item in out:
                if 1 in item:
                    probs.append(float(item[1]))
                elif '1' in item:
                    probs.append(float(item['1']))
                else:
                    key = max(item.keys())
                    probs.append(float(item[key]))
            return np.asarray(probs, dtype=np.float64)

    raise RuntimeError(f'Could not infer probability output from {model_path}')


def lgb_predict_proba(model_path: Path, X: np.ndarray) -> np.ndarray:
    booster = lgb.Booster(model_file=str(model_path))
    return booster.predict(X).astype(np.float64)


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
        'report': classification_report(y_true, y_pred, zero_division=0),
    }


def print_table(rows):
    print('\n' + '=' * 100)
    print(f"{'Model':<20} {'ROC AUC':>10} {'PR AUC':>10} {'Acc':>10} {'Prec':>10} {'Rec':>10} {'F1':>10}")
    print('-' * 100)
    for row in rows:
        print(
            f"{row['name']:<20} {row['roc_auc']:10.6f} {row['pr_auc']:10.6f} {row['accuracy']:10.6f} "
            f"{row['precision']:10.6f} {row['recall']:10.6f} {row['f1']:10.6f}"
        )
    print('=' * 100)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument('--bin', default='priv/references.json.gz')
    parser.add_argument('--max-samples', type=int, default=50000)
    parser.add_argument('--svm-onnx', default='models/svm_rbf_sample.onnx')
    parser.add_argument('--logreg-onnx', default='models/logreg.onnx')
    parser.add_argument('--lgb-model', default='models/lgb_model.txt')
    args = parser.parse_args()

    X, y = load_references(Path(args.bin), max_samples=args.max_samples)
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, stratify=y, random_state=42)

    svm_path = Path(args.svm_onnx)
    logreg_path = Path(args.logreg_onnx)
    lgb_path = Path(args.lgb_model)

    results = []
    results.append(evaluate('svm_onnx', y_test, onnx_predict_proba(svm_path, X_test)))
    results.append(evaluate('logreg_onnx', y_test, onnx_predict_proba(logreg_path, X_test)))
    results.append(evaluate('lgb_model', y_test, lgb_predict_proba(lgb_path, X_test)))

    print_table(results)

    for row in results:
        print(f"\n--- {row['name']} confusion matrix ---")
        print(f"TP={row['tp']} TN={row['tn']} FP={row['fp']} FN={row['fn']}")

    return 0


if __name__ == '__main__':
    raise SystemExit(main())
