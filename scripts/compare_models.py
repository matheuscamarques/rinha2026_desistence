#!/usr/bin/env python3
"""
Compare baseline model (14 features) vs v2 model (19 engineered features).
Evaluates both Logistic Regression models on test set.
"""

import joblib
import json
import numpy as np
from pathlib import Path
from sklearn.metrics import (
    classification_report, confusion_matrix, roc_auc_score, 
    average_precision_score, f1_score, precision_score, recall_score
)

def load_model(model_path):
    """Load a scikit-learn pipeline model."""
    if not Path(model_path).exists():
        return None
    return joblib.load(model_path)

def evaluate_model(model, X_test, y_test, model_name):
    """Evaluate model and return metrics."""
    
    y_proba = model.predict_proba(X_test)[:, 1]
    y_pred = model.predict(X_test)
    
    tn, fp, fn, tp = confusion_matrix(y_test, y_pred).ravel()
    
    metrics = {
        'name': model_name,
        'accuracy': np.mean(y_pred == y_test),
        'precision': precision_score(y_test, y_pred),
        'recall': recall_score(y_test, y_pred),
        'f1': f1_score(y_test, y_pred),
        'roc_auc': roc_auc_score(y_test, y_proba),
        'pr_auc': average_precision_score(y_test, y_proba),
        'true_positives': int(tp),
        'true_negatives': int(tn),
        'false_positives': int(fp),
        'false_negatives': int(fn),
        'specificity': tn / (tn + fp) if (tn + fp) > 0 else 0,
    }
    
    return metrics, y_proba, y_pred

def print_comparison(baseline_metrics, v2_metrics):
    """Print formatted comparison of models."""
    
    print("\n" + "="*90)
    print("MODEL COMPARISON: BASELINE (14 FEATURES) vs V2 (19 ENGINEERED FEATURES)")
    print("="*90)
    
    print(f"\n{'Metric':<25} {'Baseline':<20} {'V2':<20} {'Improvement':<20}")
    print("-" * 90)
    
    metrics_to_compare = [
        ('accuracy', 'Accuracy'),
        ('precision', 'Precision'),
        ('recall', 'Recall (TPR)'),
        ('specificity', 'Specificity (TNR)'),
        ('f1', 'F1-Score'),
        ('roc_auc', 'ROC AUC'),
        ('pr_auc', 'PR AUC'),
    ]
    
    improvements = {}
    
    for key, name in metrics_to_compare:
        baseline_val = baseline_metrics.get(key, 0)
        v2_val = v2_metrics.get(key, 0)
        
        if isinstance(baseline_val, (int, float)):
            improvement = v2_val - baseline_val
            improvement_pct = (improvement / baseline_val * 100) if baseline_val != 0 else 0
            
            baseline_str = f"{baseline_val:.6f}"
            v2_str = f"{v2_val:.6f}"
            
            if improvement >= 0:
                improvement_str = f"+{improvement:.6f} (+{improvement_pct:.2f}%)"
                symbol = "↑"
            else:
                improvement_str = f"{improvement:.6f} ({improvement_pct:.2f}%)"
                symbol = "↓"
            
            improvements[key] = improvement
            
            print(f"{name:<25} {baseline_str:<20} {v2_str:<20} {symbol} {improvement_str:<20}")
    
    print("\n" + "-" * 90)
    print(f"{'Confusion Matrix':<25} {'Baseline':<20} {'V2':<20}")
    print("-" * 90)
    
    for label, key in [('True Positives', 'true_positives'), 
                       ('True Negatives', 'true_negatives'),
                       ('False Positives', 'false_positives'),
                       ('False Negatives', 'false_negatives')]:
        b_val = baseline_metrics.get(key, 0)
        v2_val = v2_metrics.get(key, 0)
        diff = v2_val - b_val
        
        print(f"{label:<25} {b_val:<20,} {v2_val:<20,} ({diff:+,})")
    
    # Calculate error reduction
    baseline_errors = baseline_metrics['false_positives'] + baseline_metrics['false_negatives']
    v2_errors = v2_metrics['false_positives'] + v2_metrics['false_negatives']
    error_reduction = baseline_errors - v2_errors
    error_reduction_pct = (error_reduction / baseline_errors * 100) if baseline_errors > 0 else 0
    
    print("\n" + "="*90)
    print("ERROR ANALYSIS")
    print("="*90)
    print(f"\nBaseline Total Errors: {baseline_errors:,}")
    print(f"V2 Total Errors:       {v2_errors:,}")
    print(f"Error Reduction:       {error_reduction:,} ({error_reduction_pct:.2f}%)")
    
    print("\n" + "="*90)
    print("FEATURE ENGINEERING IMPACT SUMMARY")
    print("="*90)
    
    print("\n✓ NEW FEATURES IN V2:")
    print("  [14] tx_speed_km_min        - Superman Rule: detects impossible velocities")
    print("  [15] log_tx_speed           - Log transform of speed: handles outliers")
    print("  [16] hour_sin               - Cyclic encoding: preserves time continuity")
    print("  [17] hour_cos               - Cyclic encoding: preserves time continuity")
    print("  [18] log_amount             - Log transform: normalizes financial data distribution")
    
    print("\n📊 RESULTS:")
    if improvements['roc_auc'] > 0:
        print(f"  ✓ ROC AUC improved by {improvements['roc_auc']:.6f}")
    if improvements['f1'] > 0:
        print(f"  ✓ F1-Score improved by {improvements['f1']:.6f}")
    if error_reduction > 0:
        print(f"  ✓ Reduced {error_reduction:,} total errors ({error_reduction_pct:.1f}%)")
    
    print("\n" + "="*90)

def main():
    print("Loading models and test data...")
    
    # We don't have the test data directly, so we'll need to reconstruct it
    # For now, let's just load the models and show their coefficients
    
    baseline_model = load_model('models/logreg_pipeline.joblib')
    v2_model = load_model('models/logreg_pipeline_v2.joblib')
    
    if not baseline_model or not v2_model:
        print("❌ Error: Could not load both models")
        print("  Baseline: models/logreg_pipeline.joblib")
        print("  V2:       models/logreg_pipeline_v2.joblib")
        return
    
    print("✓ Models loaded successfully\n")
    
    # Extract feature coefficients
    baseline_coeffs = baseline_model.named_steps['lr'].coef_[0]
    v2_coeffs = v2_model.named_steps['lr'].coef_[0]
    
    print("="*90)
    print("FEATURE IMPORTANCE COMPARISON (Logistic Regression Coefficients)")
    print("="*90)
    
    print(f"\n{'Baseline Features (14)':^90}")
    print("-" * 90)
    
    baseline_names = [
        "amount_normalized", "installments_normalized", "amount_vs_customer_avg",
        "hour_of_day", "day_of_week", "minutes_since_last_tx", "km_from_last_tx",
        "km_from_home", "tx_count_24h", "is_online", "card_present",
        "merchant_is_known", "merchant_mcc_risk", "merchant_avg_amount"
    ]
    
    for i, (name, coeff) in enumerate(zip(baseline_names, baseline_coeffs)):
        abs_coeff = np.abs(coeff)
        stars = "⭐" * min(5, max(1, int(abs_coeff * 2)))
        print(f"[{i:2}] {name:30} {coeff:+.6f}  {stars}")
    
    print(f"\n{'V2 Features (19)':^90}")
    print("-" * 90)
    
    v2_names = baseline_names + [
        "tx_speed_km_min", "log_tx_speed", "hour_sin", "hour_cos", "log_amount"
    ]
    
    for i, (name, coeff) in enumerate(zip(v2_names, v2_coeffs)):
        abs_coeff = np.abs(coeff)
        stars = "⭐" * min(5, max(1, int(abs_coeff * 2)))
        marker = " ← NEW" if i >= 14 else ""
        print(f"[{i:2}] {name:30} {coeff:+.6f}  {stars}{marker}")
    
    print("\n" + "="*90)
    print("KEY OBSERVATIONS")
    print("="*90)
    
    # Find top features in each
    baseline_top_idx = np.argsort(np.abs(baseline_coeffs))[::-1][:5]
    v2_top_idx = np.argsort(np.abs(v2_coeffs))[::-1][:5]
    
    print("\nTop 5 Features in Baseline:")
    for rank, idx in enumerate(baseline_top_idx, 1):
        print(f"  {rank}. [{idx:2}] {baseline_names[idx]:30} {baseline_coeffs[idx]:+.6f}")
    
    print("\nTop 5 Features in V2:")
    for rank, idx in enumerate(v2_top_idx, 1):
        name = v2_names[idx]
        print(f"  {rank}. [{idx:2}] {name:30} {v2_coeffs[idx]:+.6f}")
    
    # Check if new features made it into top 5
    new_feature_ranks = [i for i, idx in enumerate(v2_top_idx, 1) if idx >= 14]
    if new_feature_ranks:
        print(f"\n✓ NEW FEATURES in Top 5: {new_feature_ranks[0]} position(s)")
        for idx in v2_top_idx[np.array(new_feature_ranks) - 1]:
            print(f"    - {v2_names[idx]} (ranked #{list(v2_top_idx).index(idx) + 1})")
    
    print("\n" + "="*90)
    print("✓ FEATURE ENGINEERING ANALYSIS COMPLETE")
    print("="*90)

if __name__ == '__main__':
    main()
