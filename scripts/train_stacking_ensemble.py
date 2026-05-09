#!/usr/bin/env python3
"""
Train Stacking Ensemble combining LGB, CatBoost, and KNN with LogReg as meta-learner.
This is a Level-0 + Level-1 stacking approach for maximum ROC-AUC and F1-Score.
"""

import argparse
import gzip
import sys
from collections import Counter
from pathlib import Path

import ijson
import joblib
import lightgbm as lgb
import numpy as np
from catboost import CatBoostClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import (
    auc,
    confusion_matrix,
    f1_score,
    precision_recall_curve,
    roc_auc_score,
)
from sklearn.neighbors import KNeighborsClassifier
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler
from sklearn.impute import SimpleImputer

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


def train_level0_models(X_train, y_train, X_val, y_val):
    """Train Level-0 base models (LGB, CatBoost, KNN)."""
    print("\n=== Training Level-0 Base Models ===")

    # 1. LightGBM
    print("Training LGB...")
    lgb_model = lgb.LGBMClassifier(
        n_estimators=200,
        max_depth=8,
        learning_rate=0.05,
        num_leaves=31,
        random_state=42,
        n_jobs=-1,
        verbose=-1,
    )
    lgb_model.fit(
        X_train, y_train, eval_set=[(X_val, y_val)], callbacks=[
            lgb.early_stopping(50),
            lgb.log_evaluation(0),
        ]
    )
    lgb_train_proba = lgb_model.predict_proba(X_train)[:, 1]
    lgb_val_proba = lgb_model.predict_proba(X_val)[:, 1]
    print(f"  LGB val ROC-AUC: {roc_auc_score(y_val, lgb_val_proba):.6f}")

    # 2. CatBoost
    print("Training CatBoost...")
    cb_model = CatBoostClassifier(
        iterations=500,
        depth=8,
        learning_rate=0.05,
        l2_leaf_reg=3.0,
        auto_class_weights="Balanced",
        early_stopping_rounds=50,
        random_state=42,
        verbose=0,
    )
    cb_model.fit(X_train, y_train, eval_set=[(X_val, y_val)])
    cb_train_proba = cb_model.predict_proba(X_train)[:, 1]
    cb_val_proba = cb_model.predict_proba(X_val)[:, 1]
    print(f"  CatBoost val ROC-AUC: {roc_auc_score(y_val, cb_val_proba):.6f}")

    # 3. KNN
    print("Training KNN...")
    knn_model = KNeighborsClassifier(
        n_neighbors=11, weights="uniform", p=1, leaf_size=20, n_jobs=-1
    )
    knn_model.fit(X_train, y_train)
    knn_train_proba = knn_model.predict_proba(X_train)[:, 1]
    knn_val_proba = knn_model.predict_proba(X_val)[:, 1]
    print(f"  KNN val ROC-AUC: {roc_auc_score(y_val, knn_val_proba):.6f}")

    return {
        "lgb": lgb_model,
        "catboost": cb_model,
        "knn": knn_model,
    }, {
        "lgb_train": lgb_train_proba,
        "lgb_val": lgb_val_proba,
        "catboost_train": cb_train_proba,
        "catboost_val": cb_val_proba,
        "knn_train": knn_train_proba,
        "knn_val": knn_val_proba,
    }


def create_meta_features(base_probas: dict) -> tuple:
    """Create meta-features (stack predictions)."""
    # Shape: (n_samples, 3) - one column per base model
    X_meta_train = np.column_stack([
        base_probas["lgb_train"],
        base_probas["catboost_train"],
        base_probas["knn_train"],
    ])
    X_meta_val = np.column_stack([
        base_probas["lgb_val"],
        base_probas["catboost_val"],
        base_probas["knn_val"],
    ])
    return X_meta_train, X_meta_val


def train_meta_learner(X_meta_train, y_train, X_meta_val, y_val):
    """Train Level-1 meta-learner (LogisticRegression)."""
    print("\n=== Training Level-1 Meta-Learner ===")
    print("Training LogReg as meta-learner...")

    # Preprocess meta-features
    scaler = StandardScaler()
    imputer = SimpleImputer(strategy="constant", fill_value=-1)
    
    X_meta_train_proc = imputer.fit_transform(X_meta_train)
    X_meta_train_proc = scaler.fit_transform(X_meta_train_proc)
    
    X_meta_val_proc = imputer.transform(X_meta_val)
    X_meta_val_proc = scaler.transform(X_meta_val_proc)

    # Train meta-learner
    meta_learner = LogisticRegression(
        max_iter=1000, random_state=42, n_jobs=-1, solver="lbfgs"
    )
    meta_learner.fit(X_meta_train_proc, y_train)

    meta_proba_val = meta_learner.predict_proba(X_meta_val_proc)[:, 1]
    meta_roc = roc_auc_score(y_val, meta_proba_val)
    print(f"  Meta-Learner val ROC-AUC: {meta_roc:.6f}")

    return meta_learner, scaler, imputer


def main():
    parser = argparse.ArgumentParser(description="Train Stacking Ensemble")
    parser.add_argument("--refs", required=True, help="Path to references.json.gz")
    parser.add_argument("--sample-size", type=int, default=20000)
    parser.add_argument("--val-size", type=int, default=5000)
    parser.add_argument("--test-size", type=int, default=5000)
    parser.add_argument("--model-out", required=True, help="Output joblib path")

    args = parser.parse_args()

    # Load data
    X, y = load_sample(args.refs, args.sample_size)

    # Split: train / val / test
    split_1 = args.sample_size - args.val_size - args.test_size
    X_train = X[:split_1]
    X_val = X[split_1 : split_1 + args.val_size]
    X_test = X[split_1 + args.val_size :]
    y_train = y[:split_1]
    y_val = y[split_1 : split_1 + args.val_size]
    y_test = y[split_1 + args.val_size :]

    print(f"Train: {X_train.shape}, Val: {X_val.shape}, Test: {X_test.shape}")

    # Level-0: Train base models
    base_models, base_probas = train_level0_models(X_train, y_train, X_val, y_val)

    # Create meta-features
    X_meta_train, X_meta_val = create_meta_features(base_probas)

    # Level-1: Train meta-learner
    meta_learner, scaler, imputer = train_meta_learner(
        X_meta_train, y_train, X_meta_val, y_val
    )

    # Evaluate on test set
    print("\n=== Evaluating on Test Set ===")
    
    # Generate base predictions on test
    lgb_test_proba = base_models["lgb"].predict_proba(X_test)[:, 1]
    cb_test_proba = base_models["catboost"].predict_proba(X_test)[:, 1]
    knn_test_proba = base_models["knn"].predict_proba(X_test)[:, 1]

    # Create meta-features for test
    X_meta_test = np.column_stack([lgb_test_proba, cb_test_proba, knn_test_proba])
    X_meta_test_proc = imputer.transform(X_meta_test)
    X_meta_test_proc = scaler.transform(X_meta_test_proc)

    # Meta-learner final prediction
    stacking_proba = meta_learner.predict_proba(X_meta_test_proc)[:, 1]
    stacking_pred = (stacking_proba > 0.5).astype(int)

    test_roc = roc_auc_score(y_test, stacking_proba)
    precision_test, recall_test, _ = precision_recall_curve(y_test, stacking_proba)
    test_pr_auc = auc(recall_test, precision_test)
    test_f1 = f1_score(y_test, stacking_pred)
    cm = confusion_matrix(y_test, stacking_pred)
    tn, fp, fn, tp = cm.ravel()

    print(f"Stacking ROC AUC: {test_roc:.6f}")
    print(f"Stacking PR AUC: {test_pr_auc:.6f}")
    print(f"Stacking F1: {test_f1:.6f}")
    print(f"Confusion Matrix: TP={tp}, TN={tn}, FP={fp}, FN={fn}")

    # Save complete ensemble
    artifact = {
        "base_models": base_models,
        "meta_learner": meta_learner,
        "scaler": scaler,
        "imputer": imputer,
        "metrics": {
            "test_roc_auc": float(test_roc),
            "test_pr_auc": float(test_pr_auc),
            "test_f1": float(test_f1),
            "tp": int(tp),
            "tn": int(tn),
            "fp": int(fp),
            "fn": int(fn),
        },
        "feature_dim": 14,
        "model_type": "StackingEnsemble",
        "base_models_names": ["lgb", "catboost", "knn"],
    }

    joblib.dump(artifact, args.model_out)
    print(f"\nSaved Stacking Ensemble to {args.model_out}")


if __name__ == "__main__":
    main()
