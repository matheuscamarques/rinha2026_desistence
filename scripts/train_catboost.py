#!/usr/bin/env python3
"""Train a CatBoost classifier on the synthetic references dataset.

The script samples from `priv/references.json.gz`, trains CatBoost on the raw
14-feature vectors, evaluates a holdout split, and stores a joblib artifact
compatible with the ONNX exporter.

Example:
  .venv/bin/python3 scripts/train_catboost.py \
      --refs priv/references.json.gz \
      --sample-size 100000 \
      --model-out models/catboost_v1.joblib
"""

from __future__ import annotations

import argparse
import gzip
import random
from collections import Counter
from pathlib import Path

import ijson
import joblib
import numpy as np
from catboost import CatBoostClassifier
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


def reservoir_sample_refs(gz_path: str | Path, sample_size: int, seed: int = 42):
    random.seed(seed)
    sample = []
    total = 0
    with gzip.open(gz_path, 'rt', encoding='utf-8') as handle:
        for obj in ijson.items(handle, 'item'):
            total += 1
            if len(sample) < sample_size:
                sample.append(obj)
            else:
                index = random.randrange(total)
                if index < sample_size:
                    sample[index] = obj
    print(f'Streamed {total} records, sampled {len(sample)}')
    return sample


def load_sample_vectors(sample):
    features = []
    labels = []
    for item in sample:
        vector = item.get('vector', [])
        if len(vector) < 14:
            continue
        row = [float(value) for value in vector[:14]]
        row = [np.nan if value == -1.0 else value for value in row]
        features.append(row)
        labels.append(1 if item.get('label') == 'fraud' else 0)
    return np.asarray(features, dtype=np.float32), np.asarray(labels, dtype=np.int32)


def evaluate_model(model, X, y, split_name: str):
    proba = model.predict_proba(X)[:, 1]
    pred = (proba >= 0.5).astype(np.int32)
    tn, fp, fn, tp = confusion_matrix(y, pred).ravel()
    metrics = {
        'split': split_name,
        'accuracy': accuracy_score(y, pred),
        'precision': precision_score(y, pred, zero_division=0),
        'recall': recall_score(y, pred, zero_division=0),
        'f1': f1_score(y, pred, zero_division=0),
        'roc_auc': roc_auc_score(y, proba),
        'pr_auc': average_precision_score(y, proba),
        'tp': int(tp),
        'tn': int(tn),
        'fp': int(fp),
        'fn': int(fn),
    }
    print(f"{split_name}: ROC AUC={metrics['roc_auc']:.6f} PR AUC={metrics['pr_auc']:.6f}")
    print(f"{split_name} classification report:\n{classification_report(y, pred, zero_division=0)}")
    print(f"{split_name} confusion matrix:\n{confusion_matrix(y, pred)}")
    return metrics


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument('--refs', default='priv/references.json.gz')
    parser.add_argument('--sample-size', type=int, default=100000)
    parser.add_argument('--val-size', type=int, default=25000)
    parser.add_argument('--test-size', type=int, default=25000)
    parser.add_argument('--seed', type=int, default=42)
    parser.add_argument('--model-out', default='models/catboost_v1.joblib')
    parser.add_argument('--iterations', type=int, default=500)
    parser.add_argument('--depth', type=int, default=8)
    parser.add_argument('--learning-rate', type=float, default=0.05)
    parser.add_argument('--l2-leaf-reg', type=float, default=3.0)
    args = parser.parse_args()

    print('Reservoir sampling from', args.refs)
    sample = reservoir_sample_refs(args.refs, args.sample_size, seed=args.seed)

    X, y = load_sample_vectors(sample)
    if X.size == 0:
        raise SystemExit('No valid vectors were loaded from the sample')

    print('Loaded sample shapes X=', X.shape, 'y=', y.shape)
    print('Label distribution:', Counter(y))

    if args.sample_size <= args.val_size + args.test_size:
        raise SystemExit('sample-size must be larger than val-size + test-size')

    X_train_val, X_test, y_train_val, y_test = train_test_split(
        X,
        y,
        test_size=args.test_size,
        stratify=y,
        random_state=args.seed,
    )
    val_fraction = args.val_size / (args.sample_size - args.test_size)
    X_train, X_val, y_train, y_val = train_test_split(
        X_train_val,
        y_train_val,
        test_size=val_fraction,
        stratify=y_train_val,
        random_state=args.seed,
    )

    model = CatBoostClassifier(
        iterations=args.iterations,
        depth=args.depth,
        learning_rate=args.learning_rate,
        l2_leaf_reg=args.l2_leaf_reg,
        loss_function='Logloss',
        eval_metric='AUC',
        auto_class_weights='Balanced',
        random_seed=args.seed,
        verbose=False,
        allow_writing_files=False,
        task_type='CPU',
    )

    print('Training CatBoost...')
    model.fit(
        X_train,
        y_train,
        eval_set=(X_val, y_val),
        use_best_model=True,
        early_stopping_rounds=50,
    )
    print('Best iteration:', model.get_best_iteration())

    metrics = {
        'train': evaluate_model(model, X_train, y_train, 'train'),
        'val': evaluate_model(model, X_val, y_val, 'val'),
        'test': evaluate_model(model, X_test, y_test, 'test'),
    }

    artifact = {
        'model': model,
        'metrics': metrics,
        'feature_dim': 14,
        'model_type': 'catboost',
        'best_iteration': model.get_best_iteration(),
    }
    Path(args.model_out).parent.mkdir(parents=True, exist_ok=True)
    joblib.dump(artifact, args.model_out)
    print('Saved model to', args.model_out)
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
