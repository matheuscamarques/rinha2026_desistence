#!/usr/bin/env python3
"""Train an SVM with RBF kernel (or RBF approximation) on references.json.gz.

Usage examples:
  python scripts/train_svm.py --refs priv/references.json.gz --sample-size 100000 \
      --model-out models/svm_rbf_v1.joblib --n-jobs 4

Options include an `--approx` mode which uses `RBFSampler` + `SGDClassifier`
to scale to larger datasets.
"""
import argparse
import gzip
import json
import random
import sys
from collections import Counter

import ijson
import joblib
import numpy as np
from sklearn.impute import SimpleImputer
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.svm import SVC
from sklearn.linear_model import SGDClassifier
from sklearn.kernel_approximation import RBFSampler
from sklearn.metrics import roc_auc_score, average_precision_score, classification_report, confusion_matrix


def reservoir_sample_refs(gz_path, sample_size, seed=42):
    random.seed(seed)
    sample = []
    total = 0
    with gzip.open(gz_path, 'rt', encoding='utf-8') as f:
        for obj in ijson.items(f, 'item'):
            total += 1
            if len(sample) < sample_size:
                sample.append(obj)
            else:
                j = random.randrange(total)
                if j < sample_size:
                    sample[j] = obj
    print(f"Streamed {total} records, sampled {len(sample)}")
    return sample


def load_sample_vectors(sample):
    X = []
    y = []
    for it in sample:
        vec = it.get('vector')
        label = it.get('label', 'legit')
        X.append(vec)
        y.append(1 if label == 'fraud' else 0)
    return np.array(X, dtype=float), np.array(y, dtype=int)


def impute_negative_ones(X):
    # Replace sentinel -1 with column-wise mean computed ignoring -1
    X = X.copy()
    for j in range(X.shape[1]):
        col = X[:, j]
        mask = col != -1.0
        if np.any(mask):
            mean = np.mean(col[mask])
            col[~mask] = mean
        else:
            col[:] = 0.0
        X[:, j] = col
    return X


def main():
    p = argparse.ArgumentParser()
    p.add_argument('--refs', default='priv/references.json.gz')
    p.add_argument('--sample-size', type=int, default=100000)
    p.add_argument('--val-size', type=int, default=25000)
    p.add_argument('--test-size', type=int, default=25000)
    p.add_argument('--seed', type=int, default=42)
    p.add_argument('--model-out', default='models/svm_rbf_v1.joblib')
    p.add_argument('--approx', action='store_true', help='Use RBFSampler + SGDClassifier')
    p.add_argument('--n-jobs', type=int, default=1)
    args = p.parse_args()

    print('Reservoir sampling from', args.refs)
    sample = reservoir_sample_refs(args.refs, args.sample_size, seed=args.seed)

    X, y = load_sample_vectors(sample)
    print('Loaded sample shapes X=', X.shape, 'y=', y.shape)
    print('Label distribution:', Counter(y))

    # Split into train/val/test by counts
    remaining = args.sample_size - args.test_size - args.val_size
    if remaining <= 0:
        raise SystemExit('sample-size must be larger than val+test sizes')

    X_train_val, X_test, y_train_val, y_test = train_test_split(X, y, test_size=args.test_size, stratify=y, random_state=args.seed)
    val_fraction = args.val_size / (args.sample_size - args.test_size)
    X_train, X_val, y_train, y_val = train_test_split(X_train_val, y_train_val, test_size=val_fraction, stratify=y_train_val, random_state=args.seed)

    # Impute sentinel -1 values
    X_train = impute_negative_ones(X_train)
    X_val = impute_negative_ones(X_val)
    X_test = impute_negative_ones(X_test)

    if args.approx:
        print('Using RBFSampler + SGDClassifier approximation (scalable)')
        pipeline = Pipeline([
            ('scaler', StandardScaler()),
            ('rbf_sampler', RBFSampler(gamma=0.1, random_state=args.seed, n_components=1024)),
            ('clf', SGDClassifier(loss='hinge', max_iter=2000, tol=1e-3, random_state=args.seed))
        ])
        param_grid = {
            'rbf_sampler__gamma': [0.01, 0.1, 1.0],
            'clf__alpha': [1e-4, 1e-3, 1e-2]
        }
    else:
        print('Using full SVC (RBF) -- may be slow for large samples')
        pipeline = Pipeline([
            ('scaler', StandardScaler()),
            ('clf', SVC(kernel='rbf', probability=True))
        ])
        param_grid = {
            'clf__C': [0.1, 1, 10],
            'clf__gamma': ['scale', 0.01, 0.001]
        }

    gs = GridSearchCV(pipeline, param_grid, cv=3, scoring='roc_auc', n_jobs=args.n_jobs, verbose=2)
    print('Starting GridSearchCV...')
    gs.fit(X_train, y_train)
    print('Best params:', gs.best_params_)

    best = gs.best_estimator_
    # Evaluate
    for name, (Xa, ya) in [('train', (X_train, y_train)), ('val', (X_val, y_val)), ('test', (X_test, y_test))]:
        probs = best.predict_proba(Xa)[:, 1]
        preds = (probs >= 0.5).astype(int)
        roc = roc_auc_score(ya, probs)
        pr = average_precision_score(ya, probs)
        print(f"{name}: ROC AUC={roc:.6f} PR AUC={pr:.6f}")
        print(f"{name} classification report:\n", classification_report(ya, preds))
        print(f"{name} confusion matrix:\n", confusion_matrix(ya, preds))

    # Save model
    print('Saving model to', args.model_out)
    joblib.dump({'model': best, 'params': gs.best_params_}, args.model_out)
    print('Done.')


if __name__ == '__main__':
    main()
