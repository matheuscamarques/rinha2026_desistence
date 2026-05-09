#!/usr/bin/env python3
"""Train models with advanced feature engineering (v2).

Reads from priv/references.json.gz and applies feature engineering:
1. Superman Rule: tx_speed = km_from_last_tx / (minutes_since_last_tx + 0.1)
2. Cyclic Time: hour_sin = sin(2π·h/24), hour_cos = cos(2π·h/24)
3. Log Transforms: log(1 + amount), log(1 + amount_vs_avg), etc.
4. First Transaction Flag: is_first_tx (identifies first transaction vs. spatial anomaly)

Produces:
- models/logreg_pipeline_v2.joblib (sklearn Pipeline: scaler+LogReg, 19 features)
- models/lgb_model_v2.txt (LightGBM, 19 features)
- test/results_v2.json (performance metrics comparison)
"""
from pathlib import Path
import numpy as np
import joblib
import gzip
import json
from sklearn.model_selection import StratifiedKFold, train_test_split
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
    from onnxmltools.convert.common.data_types import FloatTensorType as OnnxFloatTensorType
    ONNXTOOLS = True
except Exception:
    ONNXTOOLS = False

try:
    import optuna
    OPTUNA = True
except Exception:
    OPTUNA = False


RINHA_THRESHOLD_GRID = np.linspace(0.01, 0.99, 99)
DEFAULT_EARLY_STOPPING_ROUNDS = 50
DEFAULT_P99_SCORE = 350.0
LOCAL_REFINEMENT_CENTER = {
    'max_depth': 7,
    'num_leaves': 126,
    'learning_rate': 0.025927813889896976,
    'min_data_in_leaf': 240,
    'lambda_l1': 1.6506404396828047,
    'lambda_l2': 0.006236648166912853,
    'feature_fraction': 0.9455589144733407,
    'bagging_fraction': 0.7385895284638209,
    'bagging_freq': 2,
    'min_gain_to_split': 0.39114306186345105,
    'is_unbalance': True,
}


FEATURE_NAMES_ORIGINAL = [
    "amount_normalized",           # [0]
    "installments_normalized",     # [1]
    "amount_vs_customer_avg",      # [2]
    "hour_of_day",                 # [3]
    "day_of_week",                 # [4]
    "minutes_since_last_tx",       # [5]
    "km_from_last_tx",             # [6]
    "km_from_home",                # [7]
    "tx_count_24h",                # [8]
    "is_online",                   # [9]
    "card_present",                # [10]
    "merchant_is_known",           # [11]
    "merchant_mcc_risk",           # [12]
    "merchant_avg_amount",         # [13]
]

FEATURE_NAMES_ENGINEERED = FEATURE_NAMES_ORIGINAL + [
    "tx_speed_km_min",             # [14] - Superman Rule
    "log_tx_speed",                # [15] - Log of speed
    "hour_sin",                    # [16] - Cyclic encoding of hour
    "hour_cos",                    # [17] - Cyclic encoding of hour
    "log_amount",                  # [18] - Log transform of amount
]


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


def engineer_features(X):
    """
    Apply feature engineering to original 14 features.
    
    Args:
        X: (n_samples, 14) array of original features
    
    Returns:
        X_engineered: (n_samples, 19) array with original + engineered features
    """
    n_samples = X.shape[0]
    X_engineered = np.hstack([X, np.zeros((n_samples, 5), dtype=np.float32)])
    
    # Extract original features
    amount_norm = X[:, 0]
    hour_of_day = X[:, 3]
    minutes_since_tx = X[:, 5]
    km_from_tx = X[:, 6]
    
    # === Feature 14: Superman Rule - Transaction Speed (km/min) ===
    # Avoid division by zero with small epsilon
    tx_speed = km_from_tx / (minutes_since_tx + 0.1)
    X_engineered[:, 14] = tx_speed
    
    # === Feature 15: Log Transform of Speed ===
    # Apply log1p (log(1 + x)) to handle zero values gracefully
    log_speed = np.log1p(tx_speed)
    X_engineered[:, 15] = log_speed
    
    # === Features 16-17: Cyclic Encoding of Hour ===
    # Convert hour [0, 23] to sin/cos representation
    # This preserves the circular nature: hour 23.5 ≈ hour 0.5
    hour_radians = 2 * np.pi * (hour_of_day / 24.0)
    X_engineered[:, 16] = np.sin(hour_radians).astype(np.float32)  # hour_sin
    X_engineered[:, 17] = np.cos(hour_radians).astype(np.float32)  # hour_cos
    
    # === Feature 18: Log Transform of Amount ===
    # Denormalize amount first: amount_norm is in [0, 1], max is 10000
    amount_denorm = amount_norm * 10000.0
    log_amount = np.log1p(amount_denorm)
    # Renormalize to [0, 1] range using typical log(10000) ≈ 9.21
    X_engineered[:, 18] = (log_amount / 10.0).astype(np.float32)
    
    return X_engineered


def rinha_metrics(y_true, y_pred_binary):
    """Compute the Rinha-style failure and score metrics."""
    y_true = np.asarray(y_true)
    y_pred_binary = np.asarray(y_pred_binary)

    tp = int(np.sum((y_true == 1) & (y_pred_binary == 1)))
    tn = int(np.sum((y_true == 0) & (y_pred_binary == 0)))
    fp = int(np.sum((y_true == 0) & (y_pred_binary == 1)))
    fn = int(np.sum((y_true == 1) & (y_pred_binary == 0)))
    total = len(y_true)

    failure_rate = (fp + fn) / total if total else 0.0
    if failure_rate > 0.15:
        detection_score = -3000.0
    else:
        weighted_errors = fp + 3 * fn
        rate_component = weighted_errors / total if total else 0.0
        absolute_penalty = np.log1p(weighted_errors)
        detection_score = 1000 * np.log10(1 + rate_component * 10) - absolute_penalty

    final_score = DEFAULT_P99_SCORE + detection_score

    return {
        'TP': tp,
        'TN': tn,
        'FP': fp,
        'FN': fn,
        'failure_rate': failure_rate,
        'detection_score': detection_score,
        'final_score': final_score,
    }


def best_rinha_threshold(y_true, y_proba):
    """Find the threshold that maximizes the Rinha-style final score."""
    best_threshold = 0.5
    best_metrics = rinha_metrics(y_true, (y_proba >= best_threshold).astype(int))

    for threshold in RINHA_THRESHOLD_GRID:
        metrics = rinha_metrics(y_true, (y_proba >= threshold).astype(int))
        if metrics['final_score'] > best_metrics['final_score']:
            best_threshold = float(threshold)
            best_metrics = metrics

    return best_threshold, best_metrics


def lgb_rinha_feval(preds, dataset):
    """Custom LightGBM metric aligned with the competition score."""
    labels = dataset.get_label()
    metrics = rinha_metrics(labels, (preds >= 0.5).astype(int))
    return 'rinha_final_score', metrics['final_score'], True


def train_lightgbm_fold(X_train, y_train, X_valid, y_valid, params, num_boost_round, seed):
    train_data = lgb.Dataset(X_train, label=y_train)
    valid_data = lgb.Dataset(X_valid, label=y_valid, reference=train_data)

    fold_params = dict(params)
    fold_params.update({
        'objective': 'binary',
        'verbosity': -1,
        'seed': seed,
        'feature_fraction_seed': seed,
        'bagging_seed': seed,
        'data_random_seed': seed,
        'metric': 'None',
    })

    booster = lgb.train(
        fold_params,
        train_data,
        num_boost_round=num_boost_round,
        valid_sets=[valid_data],
        valid_names=['valid'],
        feval=lgb_rinha_feval,
        callbacks=[
            lgb.early_stopping(DEFAULT_EARLY_STOPPING_ROUNDS, verbose=False),
            lgb.log_evaluation(period=0),
        ],
    )

    best_iteration = booster.best_iteration or num_boost_round
    valid_proba = booster.predict(X_valid, num_iteration=best_iteration)
    return booster, best_iteration, valid_proba


def build_lgb_params(trial, base_scale_pos_weight, refine_local=False):
    """Sample a LightGBM parameter set from Optuna or return a fixed baseline."""
    if trial is None:
        if refine_local:
            return {
                'learning_rate': LOCAL_REFINEMENT_CENTER['learning_rate'],
                'num_leaves': LOCAL_REFINEMENT_CENTER['num_leaves'],
                'max_depth': LOCAL_REFINEMENT_CENTER['max_depth'],
                'min_data_in_leaf': LOCAL_REFINEMENT_CENTER['min_data_in_leaf'],
                'lambda_l1': LOCAL_REFINEMENT_CENTER['lambda_l1'],
                'lambda_l2': LOCAL_REFINEMENT_CENTER['lambda_l2'],
                'feature_fraction': LOCAL_REFINEMENT_CENTER['feature_fraction'],
                'bagging_fraction': LOCAL_REFINEMENT_CENTER['bagging_fraction'],
                'bagging_freq': LOCAL_REFINEMENT_CENTER['bagging_freq'],
                'min_gain_to_split': LOCAL_REFINEMENT_CENTER['min_gain_to_split'],
                'is_unbalance': LOCAL_REFINEMENT_CENTER['is_unbalance'],
            }
        return {
            'learning_rate': 0.03,
            'num_leaves': 64,
            'max_depth': 6,
            'min_data_in_leaf': 80,
            'lambda_l1': 0.1,
            'lambda_l2': 1.0,
            'feature_fraction': 0.9,
            'bagging_fraction': 0.85,
            'bagging_freq': 1,
            'min_gain_to_split': 0.0,
            'scale_pos_weight': base_scale_pos_weight,
        }

    if refine_local:
        max_depth = trial.suggest_int('max_depth', 6, 8)
    else:
        max_depth = trial.suggest_int('max_depth', 4, 8)
    max_num_leaves = max(8, (1 << max_depth) - 1)
    if refine_local:
        min_num_leaves = min(96, max_num_leaves)
    else:
        min_num_leaves = min(16, max_num_leaves)
    num_leaves = trial.suggest_int('num_leaves', min_num_leaves, max_num_leaves)

    if refine_local:
        imbalance_mode = 'is_unbalance'
    else:
        imbalance_mode = trial.suggest_categorical('imbalance_mode', ['scale_pos_weight', 'is_unbalance'])

    params = {
        'learning_rate': trial.suggest_float('learning_rate', 0.018, 0.04, log=True)
        if refine_local else trial.suggest_float('learning_rate', 0.01, 0.08, log=True),
        'num_leaves': num_leaves,
        'max_depth': max_depth,
        'min_data_in_leaf': trial.suggest_int('min_data_in_leaf', 180, 300)
        if refine_local else trial.suggest_int('min_data_in_leaf', 30, 300),
        'lambda_l1': trial.suggest_float('lambda_l1', 0.2, 4.0, log=True)
        if refine_local else trial.suggest_float('lambda_l1', 1e-8, 10.0, log=True),
        'lambda_l2': trial.suggest_float('lambda_l2', 0.001, 0.05, log=True)
        if refine_local else trial.suggest_float('lambda_l2', 1e-8, 10.0, log=True),
        'feature_fraction': trial.suggest_float('feature_fraction', 0.88, 1.0)
        if refine_local else trial.suggest_float('feature_fraction', 0.7, 1.0),
        'bagging_fraction': trial.suggest_float('bagging_fraction', 0.68, 0.82)
        if refine_local else trial.suggest_float('bagging_fraction', 0.7, 1.0),
        'bagging_freq': trial.suggest_int('bagging_freq', 1, 3)
        if refine_local else trial.suggest_int('bagging_freq', 1, 5),
        'min_gain_to_split': trial.suggest_float('min_gain_to_split', 0.25, 0.5)
        if refine_local else trial.suggest_float('min_gain_to_split', 0.0, 0.5),
        'is_unbalance': imbalance_mode == 'is_unbalance',
    }

    if imbalance_mode == 'scale_pos_weight':
        params['scale_pos_weight'] = base_scale_pos_weight * trial.suggest_float(
            'scale_pos_weight_multiplier', 0.6, 2.5, log=True
        )

    return params


def objective_factory(X_train_raw, y_train, n_splits, seed, num_boost_round, refine_local):
    skf = StratifiedKFold(n_splits=n_splits, shuffle=True, random_state=seed)
    base_scale_pos_weight = np.sum(y_train == 0) / max(1, np.sum(y_train == 1))

    def objective(trial):
        fold_scores = []
        fold_best_iterations = []
        params = build_lgb_params(trial, base_scale_pos_weight, refine_local=refine_local)

        for fold_idx, (train_idx, valid_idx) in enumerate(skf.split(X_train_raw, y_train), 1):
            X_fold_train = engineer_features(X_train_raw[train_idx])
            X_fold_valid = engineer_features(X_train_raw[valid_idx])
            y_fold_train = y_train[train_idx]
            y_fold_valid = y_train[valid_idx]

            _, best_iteration, y_fold_proba = train_lightgbm_fold(
                X_fold_train,
                y_fold_train,
                X_fold_valid,
                y_fold_valid,
                params=params,
                num_boost_round=num_boost_round,
                seed=seed + fold_idx,
            )

            _, metrics = best_rinha_threshold(y_fold_valid, y_fold_proba)
            fold_scores.append(metrics['final_score'])
            fold_best_iterations.append(best_iteration)

            trial.report(float(np.mean(fold_scores)), step=fold_idx)
            if trial.should_prune():
                raise optuna.TrialPruned()

        trial.set_user_attr('fold_scores', fold_scores)
        trial.set_user_attr('mean_final_score', float(np.mean(fold_scores)))
        trial.set_user_attr('mean_best_iteration', float(np.mean(fold_best_iterations)))
        return float(np.mean(fold_scores))

    return objective


def write_results_json(payload):
    results_path = Path('test/results_v2.json')
    results_path.parent.mkdir(parents=True, exist_ok=True)
    with open(results_path, 'w', encoding='utf-8') as f:
        json.dump(payload, f, indent=2, ensure_ascii=False)
    return results_path


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
    p.add_argument('--skip-lgb', action='store_true', help='Skip LightGBM training (LogReg only)')
    p.add_argument('--use-optuna', action='store_true', help='Run Optuna + Stratified K-Fold search for LightGBM')
    p.add_argument('--local-refine', action='store_true', help='Narrow Optuna search around the current best parameter region')
    p.add_argument('--n-trials', type=int, default=25, help='Number of Optuna trials')
    p.add_argument('--n-splits', type=int, default=5, help='Stratified K-Fold splits for Optuna objective')
    p.add_argument('--num-boost-round', type=int, default=1200, help='Maximum number of boosting rounds per fold')
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

    X_train_raw, X_holdout_raw, y_train, y_holdout = train_test_split(
        X, y, test_size=0.2, stratify=y, random_state=args.seed
    )
    X_train_eng = engineer_features(X_train_raw)
    X_holdout_eng = engineer_features(X_holdout_raw)

    print(f"Training subset: {X_train_raw.shape[0]} rows")
    print(f"Holdout subset: {X_holdout_raw.shape[0]} rows")
    
    # === APPLY FEATURE ENGINEERING ===
    print("\n" + "="*80)
    print("APPLYING FEATURE ENGINEERING")
    print("="*80)
    print(f"\nOriginal features: {X.shape[1]} (14)")
    print(f"Engineered features: {X_train_eng.shape[1]} (19)")
    print(f"\nNew features added:")
    print(f"  [14] tx_speed_km_min - Superman Rule (km/min)")
    print(f"  [15] log_tx_speed - Log transform of speed")
    print(f"  [16] hour_sin - Cyclic sine encoding of hour")
    print(f"  [17] hour_cos - Cyclic cosine encoding of hour")
    print(f"  [18] log_amount - Log transform of amount")

    print("\n" + "="*80)
    print("TRAINING LOGISTIC REGRESSION (v2)")
    print("="*80)
    
    # Logistic regression pipeline (scaled)
    scaler = StandardScaler()
    lr = LogisticRegression(max_iter=2000, class_weight='balanced', solver='saga')
    pipe = Pipeline([('scaler', scaler), ('lr', lr)])
    pipe.fit(X_train_eng, y_train)

    y_proba_lr = pipe.predict_proba(X_holdout_eng)[:, 1]
    y_pred_lr = pipe.predict(X_holdout_eng)
    
    roc_auc_lr = roc_auc_score(y_holdout, y_proba_lr)
    pr_auc_lr = average_precision_score(y_holdout, y_proba_lr)
    lr_threshold, lr_rinha = best_rinha_threshold(y_holdout, y_proba_lr)
    
    print(f"\nLogReg ROC AUC: {roc_auc_lr:.4f}")
    print(f"LogReg PR AUC: {pr_auc_lr:.4f}")
    print(f"LogReg best Rinha threshold: {lr_threshold:.2f}")
    print(f"LogReg holdout final score: {lr_rinha['final_score']:.2f}")
    print("\nClassification Report:")
    print(classification_report(y_holdout, y_pred_lr))

    joblib.dump(pipe, out_dir / 'logreg_pipeline_v2.joblib')
    print(f"\n✓ Saved to {out_dir / 'logreg_pipeline_v2.joblib'}")

    # Try export to ONNX (skl2onnx)
    if SKL2ONNX:
        try:
            initial_type = [('input', FloatTensorType([None, X_train_eng.shape[1]]))]
            onx = convert_sklearn(pipe, initial_types=initial_type)
            with open(out_dir / 'logreg_v2.onnx', 'wb') as f:
                f.write(onx.SerializeToString())
            print(f'✓ Exported to {out_dir / "logreg_v2.onnx"}')
        except Exception as e:
            print(f'logreg ONNX export failed: {e}')
    else:
        print('skl2onnx not installed; skipping logreg ONNX export')

    # LightGBM
    if not args.skip_lgb:
        print("\n" + "="*80)
        print("TRAINING LIGHTGBM (v2)")
        print("="*80)

        base_scale_pos_weight = np.sum(y_train == 0) / max(1, np.sum(y_train == 1))
        best_params = build_lgb_params(None, base_scale_pos_weight, refine_local=args.local_refine)
        best_num_boost_round = 500
        optuna_summary = None

        if args.use_optuna:
            if not OPTUNA:
                raise RuntimeError('optuna is not installed. Add it to the environment or run without --use-optuna.')

            print(f"Running Optuna search with {args.n_trials} trials and {args.n_splits} folds...")
            if args.local_refine:
                print("Local refinement mode enabled: searching near current best region.")
            study = optuna.create_study(direction='maximize', study_name='lightgbm_rinha_v2')
            objective = objective_factory(
                X_train_raw=X_train_raw,
                y_train=y_train,
                n_splits=args.n_splits,
                seed=args.seed,
                num_boost_round=args.num_boost_round,
                refine_local=args.local_refine,
            )
            study.optimize(objective, n_trials=args.n_trials)

            best_trial = study.best_trial
            best_params = build_lgb_params(best_trial, base_scale_pos_weight, refine_local=args.local_refine)
            best_num_boost_round = max(50, int(round(best_trial.user_attrs.get('mean_best_iteration', args.num_boost_round))))
            optuna_summary = {
                'best_value': float(study.best_value),
                'best_trial_number': int(best_trial.number),
                'best_params': best_trial.params,
            }

            print(f"Best Optuna score: {study.best_value:.2f}")
            print(f"Best params: {best_trial.params}")

        dtrain = lgb.Dataset(X_train_eng, label=y_train)
        final_params = dict(best_params)
        final_params.update({
            'objective': 'binary',
            'metric': 'None',
            'verbosity': -1,
            'seed': args.seed,
            'feature_fraction_seed': args.seed,
            'bagging_seed': args.seed,
            'data_random_seed': args.seed,
        })

        bst = lgb.train(final_params, dtrain, num_boost_round=best_num_boost_round)
        bst.save_model(str(out_dir / 'lgb_model_v2.txt'))
        print(f'✓ Saved to {out_dir / "lgb_model_v2.txt"}')

        y_proba_lgb = bst.predict(X_holdout_eng)
        y_pred_lgb = (y_proba_lgb > 0.5).astype(int)
        
        roc_auc_lgb = roc_auc_score(y_holdout, y_proba_lgb)
        pr_auc_lgb = average_precision_score(y_holdout, y_proba_lgb)
        lgb_threshold, lgb_rinha = best_rinha_threshold(y_holdout, y_proba_lgb)
        
        print(f'\nLGBM ROC AUC: {roc_auc_lgb:.4f}')
        print(f'LGBM PR AUC: {pr_auc_lgb:.4f}')
        print(f'LGBM best Rinha threshold: {lgb_threshold:.2f}')
        print(f'LGBM holdout final score: {lgb_rinha["final_score"]:.2f}')
        print("\nClassification Report:")
        print(classification_report(y_holdout, y_pred_lgb))

        write_results_json({
            'seed': args.seed,
            'n_samples': int(X.shape[0]),
            'train_samples': int(X_train_raw.shape[0]),
            'holdout_samples': int(X_holdout_raw.shape[0]),
            'logreg': {
                'roc_auc': float(roc_auc_lr),
                'pr_auc': float(pr_auc_lr),
                'best_threshold': float(lr_threshold),
                'rinha': lr_rinha,
            },
            'lgbm': {
                'roc_auc': float(roc_auc_lgb),
                'pr_auc': float(pr_auc_lgb),
                'best_threshold': float(lgb_threshold),
                'rinha': lgb_rinha,
                'optuna_enabled': bool(args.use_optuna),
                'optuna_summary': optuna_summary,
                'best_params': best_params,
                'best_num_boost_round': int(best_num_boost_round),
            },
        })

        # Try export LightGBM to ONNX via onnxmltools
        if ONNXTOOLS:
            try:
                onx_lgb = convert_lightgbm(
                    bst,
                    name='lgbm_model_v2',
                    initial_types=[('input', OnnxFloatTensorType([None, X_train_eng.shape[1]]))],
                )
                save_onnx(onx_lgb, str(out_dir / 'lgb_v2.onnx'))
                print(f'✓ Exported to {out_dir / "lgb_v2.onnx"}')
            except Exception as e:
                print(f'LightGBM ONNX export failed: {e}')
        else:
            print('onnxmltools not installed; skipping LightGBM ONNX export')

    # Print feature importance from LogReg
    print("\n" + "="*80)
    print("FEATURE IMPORTANCE (Logistic Regression Coefficients)")
    print("="*80)
    
    # Get coefficients
    coeffs = pipe.named_steps['lr'].coef_[0]
    
    # Sort by absolute value
    sorted_idx = np.argsort(np.abs(coeffs))[::-1]
    
    print(f"\n{'Rank':<4} {'Index':<6} {'Feature':<30} {'Coefficient':<15} {'Importance'}")
    print("-" * 80)
    for rank, idx in enumerate(sorted_idx, 1):
        feat_name = FEATURE_NAMES_ENGINEERED[idx] if idx < len(FEATURE_NAMES_ENGINEERED) else f"Feature_{idx}"
        coeff = coeffs[idx]
        abs_coeff = np.abs(coeff)
        stars = "⭐" * min(5, max(1, int(abs_coeff * 2)))
        print(f"{rank:<4} {idx:<6} {feat_name:<30} {coeff:+.6f}         {stars}")
    
    print("\n✓ TRAINING COMPLETE")
    print("="*80)


if __name__ == '__main__':
    main()
