#!/usr/bin/env python3
"""
Train Local Outlier Factor (LOF) model for fraud detection.
LOF is an unsupervised anomaly detection algorithm.
"""

import argparse
import gzip
import json
import sys
from collections import Counter
from pathlib import Path

import ijson
import joblib
import numpy as np
from sklearn.metrics import (
    confusion_matrix,
    f1_score,
    precision_score,
    recall_score,
    roc_auc_score,
    auc,
    precision_recall_curve,
)
from sklearn.neighbors import LocalOutlierFactor
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler
from sklearn.impute import SimpleImputer

# Add parent directory to path for imports
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))


def load_sample(ref_path: str, max_samples: int) -> tuple:
    """Load sample from gzip-compressed JSON using ijson."""
    print(f"Loading sample from {ref_path}")
    X_list, y_list = [], []
    
    with gzip.open(ref_path, "rt", encoding="utf-8") as handle:
        for index, item in enumerate(ijson.items(handle, "item"), 1):
            if max_samples is not None and index > max_samples:
                break
            vector = item.get("vector", [])
            if len(vector) < 14:
                continue
            X_list.append([float(v) for v in vector[:14]])
            y_list.append(1 if item.get("label") == "fraud" else 0)
    
    print(f"Loaded {len(X_list)} samples")
    X = np.array(X_list, dtype=np.float32)
    y = np.array(y_list, dtype=np.int32)
    print(f"Sample shapes X= {X.shape} y= {y.shape}")
    print(f"Label distribution: {Counter(y)}")
    return X, y


def train_lof(
    X_train: np.ndarray,
    X_val: np.ndarray,
    y_train: np.ndarray,
    y_val: np.ndarray,
    n_neighbors: int = 20,
    contamination: float = 0.33,
) -> tuple:
    """Train LOF with given parameters and return (pipeline, metrics_dict)."""
    print(f"Training LOF (n_neighbors={n_neighbors}, contamination={contamination})...")

    # LOF with novelty=True allows decision_function on new data
    pipeline = Pipeline(
        [
            ("imputer", SimpleImputer(strategy="constant", fill_value=-1)),
            ("scaler", StandardScaler()),
            (
                "lof",
                LocalOutlierFactor(
                    n_neighbors=n_neighbors,
                    contamination=contamination,
                    novelty=True,  # Required for decision_function on new data
                    n_jobs=-1,
                ),
            ),
        ]
    )

    # Fit on training data
    pipeline.fit(X_train)
    
    # Get decision function scores (negative outlier factor)
    train_scores = pipeline.decision_function(X_train)
    val_scores = pipeline.decision_function(X_val)
    
    # Convert outlier factor to probability-like scores using logistic sigmoid
    # negative_outlier_factor ranges from -inf to 0 typically
    # We use logistic sigmoid to convert to [0,1]
    train_proba = 1 / (1 + np.exp(train_scores))
    val_proba = 1 / (1 + np.exp(val_scores))

    # Use threshold=0.5 for binary classification
    train_pred = (train_proba > 0.5).astype(int)
    val_pred = (val_proba > 0.5).astype(int)

    train_roc = roc_auc_score(y_train, train_proba)
    val_roc = roc_auc_score(y_val, val_proba)

    precision_val, recall_val, _ = precision_recall_curve(y_val, val_proba)
    val_pr_auc = auc(recall_val, precision_val)

    val_f1 = f1_score(y_val, val_pred)

    metrics = {
        "train_roc_auc": float(train_roc),
        "train_pr_auc": float(0.0),
        "val_roc_auc": float(val_roc),
        "val_pr_auc": float(val_pr_auc),
        "val_f1": float(val_f1),
    }

    return pipeline, metrics


def main():
    parser = argparse.ArgumentParser(description="Train LOF model")
    parser.add_argument("--refs", required=True, help="Path to references.json.gz")
    parser.add_argument("--sample-size", type=int, default=20000)
    parser.add_argument("--val-size", type=int, default=5000)
    parser.add_argument("--test-size", type=int, default=5000)
    parser.add_argument("--model-out", required=True, help="Output joblib path")
    parser.add_argument("--n-neighbors", type=int, default=20)
    parser.add_argument("--contamination", type=float, default=0.33)

    args = parser.parse_args()

    # Load data
    X, y = load_sample(args.refs, args.sample_size)

    # Split
    split_1 = args.sample_size - args.val_size - args.test_size
    X_train, X_val, X_test = X[:split_1], X[split_1 : split_1 + args.val_size], X[split_1 + args.val_size :]
    y_train, y_val, y_test = y[:split_1], y[split_1 : split_1 + args.val_size], y[split_1 + args.val_size :]

    # Train
    pipeline, metrics = train_lof(
        X_train,
        X_val,
        y_train,
        y_val,
        n_neighbors=args.n_neighbors,
        contamination=args.contamination,
    )

    # Evaluate on test
    test_scores = pipeline.decision_function(X_test)
    test_proba = 1 / (1 + np.exp(test_scores))
    test_pred = (test_proba > 0.5).astype(int)

    test_roc = roc_auc_score(y_test, test_proba)
    precision_test, recall_test, _ = precision_recall_curve(y_test, test_proba)
    test_pr_auc = auc(recall_test, precision_test)
    test_f1 = f1_score(y_test, test_pred)

    print(f"test: ROC AUC={test_roc:.6f} PR AUC={test_pr_auc:.6f} F1={test_f1:.6f}")
    print(f"test confusion matrix: {confusion_matrix(y_test, test_pred)}")

    # Save model (store pipeline + metadata)
    artifact = {
        "model": pipeline,
        "metrics": {
            "train_roc_auc": float(metrics["train_roc_auc"]),
            "val_roc_auc": float(metrics["val_roc_auc"]),
            "test_roc_auc": float(test_roc),
            "test_pr_auc": float(test_pr_auc),
            "test_f1": float(test_f1),
        },
        "feature_dim": 14,
        "model_type": "LOF",
        "threshold": 0.5,
    }

    joblib.dump(artifact, args.model_out)
    print(f"Saved model to {args.model_out}")


if __name__ == "__main__":
    main()
