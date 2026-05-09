#!/usr/bin/env python3
"""Wrapper to run threshold sweep with models that expect 14- or 19-dim inputs.

This imports helper functions from `scripts/threshold_sweep.py`, loads the model,
and slices engineered features when necessary to match the model's expected input.
"""
from pathlib import Path
import argparse
import joblib
import numpy as np
import importlib.machinery
import importlib.util


def load_threshold_module(path):
    loader = importlib.machinery.SourceFileLoader('threshold_sweep', str(path))
    spec = importlib.util.spec_from_loader(loader.name, loader)
    mod = importlib.util.module_from_spec(spec)
    loader.exec_module(mod)
    return mod


def main():
    p = argparse.ArgumentParser()
    p.add_argument('--bin', default='priv/references.json.gz')
    p.add_argument('--model', default='models/svm_rbf_sample.joblib')
    p.add_argument('--max-samples', type=int, default=50000)
    p.add_argument('--output', default='threshold_sweep_wrapper_results.json')
    args = p.parse_args()

    ts_path = Path(__file__).parent / 'threshold_sweep.py'
    ts = load_threshold_module(ts_path)

    X, y = ts.read_references(Path(args.bin), max_samples=args.max_samples)
    X_eng = ts.engineer_features(X)

    # split
    from sklearn.model_selection import train_test_split
    X_train, X_test, y_train, y_test = train_test_split(X_eng, y, test_size=0.2, stratify=y, random_state=42)

    model_obj = joblib.load(args.model)
    # If joblib file is dict wrapper, extract estimator
    if isinstance(model_obj, dict) and 'model' in model_obj:
        model = model_obj['model']
    else:
        model = model_obj

    # Determine expected n_features for model by probing with a dummy
    try:
        exp = model.named_steps['scaler'].n_features_in_
    except Exception:
        try:
            exp = model.n_features_in_
        except Exception:
            exp = None

    if exp is None:
        # fallback: try predict on one sample and catch
        try:
            model.predict_proba(X_test[:1])
            exp = X_test.shape[1]
        except Exception:
            # final fallback assume 14
            exp = 14

    print(f'Model expects {exp} features; test set has {X_test.shape[1]} engineered features')

    if exp < X_test.shape[1]:
        X_for_model = X_test[:, :exp]
    else:
        X_for_model = X_test

    y_proba = model.predict_proba(X_for_model)[:, 1]

    # Sweep thresholds using rinha_score from module
    results = []
    best_threshold = 0.5
    best_score = -1e9
    for threshold in np.arange(0.01, 1.00, 0.01):
        y_pred = (y_proba >= threshold).astype(int)
        metrics = ts.rinha_score(y_test, y_pred)
        entry = {'threshold': round(float(threshold),2), **metrics}
        results.append(entry)
        if metrics['final_score'] > best_score:
            best_score = metrics['final_score']
            best_threshold = threshold

    out = {
        'best_threshold': round(best_threshold,2),
        'best_score': float(best_score),
        'all_results': results
    }

    import json
    with open(args.output, 'w') as f:
        json.dump(out, f, indent=2)

    print(f'Done. Best threshold {best_threshold:.2f} score={best_score:.2f}. Results -> {args.output}')


if __name__ == '__main__':
    main()
