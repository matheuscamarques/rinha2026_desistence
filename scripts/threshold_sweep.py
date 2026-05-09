#!/usr/bin/env python3
"""
Threshold Sweep for Fraud Detection

Finds the optimal fraud_score decision threshold by:
1. Loading the trained model and test data
2. Generating fraud probabilities
3. Testing thresholds from 0.01 to 0.99
4. Computing Rinha scoring formula for each threshold
5. Recommending the best FRAUD_THRESHOLD value
"""

from pathlib import Path
import numpy as np
import joblib
import gzip
import json
from sklearn.model_selection import train_test_split
import argparse

def read_references(json_path: Path, max_samples=None):
    """Read references from original JSON (float precision)."""
    X_list = []
    y_list = []
    
    print(f"Reading references from {json_path}...")
    
    try:
        import ijson
        with gzip.open(json_path, 'rb') as f:
            for line_num, entry in enumerate(ijson.items(f, 'item'), 1):
                if max_samples and line_num > max_samples:
                    break
                if line_num % 100000 == 0:
                    print(f"  Loaded {line_num} records...")
                
                features = entry.get('vector', [])
                if len(features) < 14:
                    continue
                
                features = [float(f) for f in features[:14]]
                label = 1 if entry.get('label') == 'fraud' else 0
                
                X_list.append(features)
                y_list.append(label)
        
        print(f"Total records loaded: {len(X_list)}")
        return np.array(X_list, dtype=np.float32), np.array(y_list, dtype=np.int32)
    except Exception as e:
        print(f"ijson failed: {e}, falling back to json.load()...")
    
    try:
        with gzip.open(json_path, 'rt', encoding='utf-8') as f:
            data = json.load(f)
        
        if not isinstance(data, list):
            raise ValueError(f"Expected JSON array at root, got {type(data)}")
        
        for line_num, entry in enumerate(data, 1):
            if max_samples and line_num > max_samples:
                break
            if line_num % 100000 == 0:
                print(f"  Loaded {line_num} records...")
            
            features = entry.get('vector', [])
            if len(features) < 14:
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


def engineer_features(X):
    """Apply v2 feature engineering (14 → 19 features)."""
    n_samples = X.shape[0]
    X_engineered = np.hstack([X, np.zeros((n_samples, 5), dtype=np.float32)])
    
    amount_norm = X[:, 0]
    hour_of_day = X[:, 3]
    minutes_since_tx = X[:, 5]
    km_from_tx = X[:, 6]
    
    tx_speed = km_from_tx / (minutes_since_tx + 0.1)
    X_engineered[:, 14] = tx_speed
    
    log_speed = np.log1p(tx_speed)
    X_engineered[:, 15] = log_speed
    
    hour_radians = 2 * np.pi * (hour_of_day / 24.0)
    X_engineered[:, 16] = np.sin(hour_radians).astype(np.float32)
    X_engineered[:, 17] = np.cos(hour_radians).astype(np.float32)
    
    amount_denorm = amount_norm * 10000.0
    log_amount = np.log1p(amount_denorm)
    X_engineered[:, 18] = (log_amount / 10.0).astype(np.float32)
    
    return X_engineered


def rinha_score(y_true, y_pred_binary):
    """
    Calculate the Rinha competition score.
    
    Rinha formula:
    - failure_rate = (FP + FN + Err) / N
    - If failure_rate > 15%, detection_score = -3000
    - Otherwise, detection_score = 1000 * log10(1 + rate_component) - absolute_penalty
    where:
      rate_component = (FP + 3*FN) / N
      absolute_penalty = log(1 + weighted_errors_E)
      weighted_errors_E = FP + 3*FN (simplified, no HTTP errors)
    """
    TP = np.sum((y_true == 1) & (y_pred_binary == 1))
    TN = np.sum((y_true == 0) & (y_pred_binary == 0))
    FP = np.sum((y_true == 0) & (y_pred_binary == 1))
    FN = np.sum((y_true == 1) & (y_pred_binary == 0))
    
    N = len(y_true)
    
    # Failure rate
    failure_rate = (FP + FN) / N
    
    # Detection score
    if failure_rate > 0.15:
        detection_score = -3000
    else:
        weighted_errors_E = FP + 3 * FN
        rate_component = (FP + 3 * FN) / N
        absolute_penalty = np.log1p(weighted_errors_E)
        detection_score = 1000 * np.log10(1 + rate_component * 10) - absolute_penalty
    
    # P99 score (assume fixed for now, typically ~350-400)
    p99_score = 350.0
    
    final_score = p99_score + detection_score
    
    return {
        'TP': int(TP),
        'TN': int(TN),
        'FP': int(FP),
        'FN': int(FN),
        'failure_rate': failure_rate,
        'detection_score': detection_score,
        'final_score': final_score
    }


def main():
    p = argparse.ArgumentParser()
    p.add_argument('--bin', default='priv/references.json.gz')
    p.add_argument('--model', default='models/logreg_pipeline_v2.joblib')
    p.add_argument('--max-samples', type=int, default=50000, help='Max samples for sweep (for speed)')
    p.add_argument('--output', default='threshold_sweep_results.json')
    args = p.parse_args()

    bin_path = Path(args.bin)
    model_path = Path(args.model)
    
    if not model_path.exists():
        print(f"Error: {model_path} not found.")
        return 1
    
    # Load data
    X, y = read_references(bin_path, max_samples=args.max_samples)
    print(f"\nLoaded {X.shape[0]} samples, fraud_rate={y.mean():.4f}")
    
    # Apply feature engineering
    print("Applying feature engineering...")
    X_engineered = engineer_features(X)
    
    # Split into train/test
    X_train, X_test, y_train, y_test = train_test_split(
        X_engineered, y, test_size=0.2, stratify=y, random_state=42
    )
    
    # Load model
    print(f"Loading model from {model_path}...")
    pipe = joblib.load(model_path)
    
    # Generate predictions
    print("Generating fraud probabilities on test set...")
    y_proba = pipe.predict_proba(X_test)[:, 1]
    
    # Sweep thresholds
    print("\nThreshold Sweep:")
    print("Threshold | TP     | TN     | FP    | FN    | Failure Rate | Detection Score | Final Score")
    print("-" * 100)
    
    results = []
    best_threshold = 0.5
    best_score = -10000
    
    for threshold in np.arange(0.01, 1.00, 0.01):
        y_pred_binary = (y_proba >= threshold).astype(int)
        metrics = rinha_score(y_test, y_pred_binary)
        
        score = {
            'threshold': round(threshold, 2),
            **metrics
        }
        results.append(score)
        
        print(f"{threshold:.2f}       | {metrics['TP']:6d} | {metrics['TN']:6d} | {metrics['FP']:5d} | {metrics['FN']:5d} | {metrics['failure_rate']:12.4f} | {metrics['detection_score']:15.2f} | {metrics['final_score']:11.2f}")
        
        if metrics['final_score'] > best_score:
            best_score = metrics['final_score']
            best_threshold = threshold
    
    # Save results
    output_path = Path(args.output)
    with open(output_path, 'w') as f:
        json.dump({
            'best_threshold': round(best_threshold, 2),
            'best_score': best_score,
            'all_results': results
        }, f, indent=2)
    
    print("\n" + "=" * 100)
    print(f"✓ Best threshold: {best_threshold:.2f}")
    print(f"✓ Best final_score: {best_score:.2f}")
    print(f"✓ Results saved to {output_path}")
    print("\nTo use this threshold in main.c:")
    print(f"  #define FRAUD_THRESHOLD {best_threshold:.2f}")
    
    return 0


if __name__ == '__main__':
    exit(main())
