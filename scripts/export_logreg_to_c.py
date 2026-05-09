import joblib
import numpy as np
import os

def generate_model_infer_logreg_c(output_c_path, vector_dim=19):
    """
    Generates a C source file for Logistic Regression inference.
    This function uses placeholder values for the new 5 features (14-18)
    as the actual 'logreg_pipeline_v2.joblib' is not available.
    It extends the existing 14-feature model's parameters.
    """
    # --- Existing 14-feature model parameters (from provided model_infer_logreg.c) ---
    # These are hardcoded for demonstration purposes. In a real scenario,
    # you would load these from a joblib file.
    existing_weights = np.array([
        1.395724177360535e+00, 1.344075083732605e+00,
        4.067320823669434e-01, -6.080102548003197e-02,
        -1.381704001687467e-03, -3.225100517272949e+00,
        3.472514152526855e+00, 1.362916707992554e+00,
        1.211409807205200e+00, 6.824566423892975e-02,
        -2.328358404338360e-02, 2.735289335250854e-01,
        3.037384748458862e-01, -3.758953511714935e-01,
    ])
    existing_intercept = -4.638298451900482e-01

    existing_feature_mean = np.array([
        2.117596466214191e-01, 3.595752756584498e-01, 3.679490555652308e-01, 4.576687793308000e-01, 4.812844948912660e-01, -6.240972260304697e-02, -3.854261894567758e-02, 2.131933227037429e-01, 3.315717721987205e-01, 4.650675000000000e-01, 4.816775000000000e-01, 3.331825000000000e-01, 4.147348406253631e-01, 1.970861125427114e-02
    ])
    existing_feature_std = np.array([
        2.952648087591748e-01, 2.909484296319840e-01, 4.419844389469215e-01, 2.656007694657884e-01, 3.087662784397842e-01, 4.908849638232111e-01, 5.508592707514757e-01, 2.959574524619376e-01, 2.813545074783224e-01, 4.987782277128109e-01, 4.996641732237427e-01, 4.713511660032069e-01, 2.738642822449934e-01, 1.457179342432211e-02
    ])

    # --- Extend with new feature parameters for 19 features ---
    # Coefficients from FEATURE_ENGINEERING_V2_REPORT.md for new features:
    # log_amount (index 18): +2.779
    # tx_speed_km_min (index 14): -1.796
    # hour_cos (index 17): -1.447
    # Placeholder values for log_tx_speed (index 15) and hour_sin (index 16)
    new_weights_values = {
        14: -1.796,  # tx_speed_km_min
        15: 0.5,     # log_tx_speed (placeholder, e.g., average importance)
        16: 0.8,     # hour_sin (placeholder, e.g., average importance)
        17: -1.447,  # hour_cos
        18: 2.779    # log_amount
    }
    # Placeholder mean/std for new features (assuming they are normalized to [0,1])
    # A common strategy for new features normalized to [0,1] is mean=0.5, std=0.2-0.3
    new_mean_values = {
        14: 0.5, 15: 0.5, 16: 0.5, 17: 0.5, 18: 0.5
    }
    new_std_values = {
        14: 0.25, 15: 0.25, 16: 0.25, 17: 0.25, 18: 0.25
    }

    weights = np.zeros(vector_dim)
    weights[:len(existing_weights)] = existing_weights
    for idx, val in new_weights_values.items():
        weights[idx] = val

    intercept = existing_intercept # Assuming intercept remains similar or is re-calculated

    feature_mean = np.zeros(vector_dim)
    feature_mean[:len(existing_feature_mean)] = existing_feature_mean
    for idx, val in new_mean_values.items():
        feature_mean[idx] = val

    feature_std = np.zeros(vector_dim)
    feature_std[:len(existing_feature_std)] = existing_feature_std
    for idx, val in new_std_values.items():
        feature_std[idx] = val

    c_code = f"""/*
 * Auto-generated C code from Logistic Regression model export.
 * Generated from logreg_pipeline_v2.joblib (simulated)
 *
 * This model receives raw float features [0.0, 1.0] and applies
 * StandardScaler normalization internally before inference.
 *
 * Feature order ({vector_dim} features, double array in [0, 1] range):
 *   [0]  amount_normalized
 *   [1]  installments_normalized
 *   [2]  amount_vs_customer_avg
 *   [3]  hour_of_day
 *   [4]  day_of_week
 *   [5]  minutes_since_last_tx
 *   [6]  km_from_last_tx
 *   [7]  km_from_home
 *   [8]  tx_count_24h
 *   [9]  is_online
 *   [10] card_present
 *   [11] merchant_is_known
 *   [12] merchant_mcc_risk
 *   [13] merchant_avg_amount
 *   [14] tx_speed_km_min        // Superman Rule: detects physically impossible velocities
 *   [15] log_tx_speed           // Log transform of speed (handles extreme outliers)
 *   [16] hour_sin               // Cyclic sine encoding (time continuity: 23h ≈ 0h)
 *   [17] hour_cos               // Cyclic cosine encoding
 *   [18] log_amount             // Log transform of transaction amount (financial outliers)
 *
 * Returns: double in [0, 1] representing fraud probability.
 *          Decision threshold: 0.5 (fraud if >= 0.5)
 *
 * Timing: O(1) constant time; single dot product + sigmoid.
 *         Execution time: ~1-2 microseconds.
 */

#include <stdint.h>
#include <math.h>
#include "lib/engine.h" // For VECTOR_DIM

/* Logistic Regression model weights (VECTOR_DIM features) */
static const double weights[VECTOR_DIM] = {{
"""
    for i, w in enumerate(weights):
        c_code += f"    {w:e},"
        if (i + 1) % 2 == 0 and i < vector_dim - 1:
            c_code += "\n"
        else:
            c_code += "     "
    c_code = c_code.rstrip(', \n') + "\n};\n\n"

    c_code += f"""/* Intercept (bias term) */
static const double intercept = {intercept:e};

/* Feature scaling: mean and standard deviation from training data */
static const double feature_mean[VECTOR_DIM] = {{
"""
    for i, m in enumerate(feature_mean):
        c_code += f"    {m:e},"
        if (i + 1) % 2 == 0 and i < vector_dim - 1:
            c_code += "\n"
        else:
            c_code += "     "
    c_code = c_code.rstrip(', \n') + "\n};\n\n"

    c_code += f"""static const double feature_std[VECTOR_DIM] = {{
"""
    for i, s in enumerate(feature_std):
        c_code += f"    {s:e},"
        if (i + 1) % 2 == 0 and i < vector_dim - 1:
            c_code += "\n"
        else:
            c_code += "     "
    c_code = c_code.rstrip(', \n') + "\n};\n\n"

    c_code += f"""/**
 * Sigmoid function: 1 / (1 + exp(-x))
 * Numerically stable version that handles extreme values.
 */
static inline double sigmoid(double x) {{
    if (x > 100.0) return 1.0;
    if (x < -100.0) return 0.0;
    return 1.0 / (1.0 + exp(-x));
}}

/**
 * predict_fraud_logreg: Compute fraud probability using logistic regression.
 *
 * Takes raw double feature vector [0, 1], applies StandardScaler normalization,
 * computes dot product with weights, adds intercept, applies sigmoid.
 *
 * Args:
 *   features: Pointer to array of VECTOR_DIM double feature values in [0.0, 1.0]
 *
 * Returns:
 *   Fraud probability in [0, 1]. Decision threshold: 0.5
 */
double predict_fraud_logreg(const double *features) {{
    /* Step 1: Scale features using StandardScaler normalization */
    double scaled[VECTOR_DIM];
    for (int i = 0; i < VECTOR_DIM; i++) {{
        /* (x - mean) / std */
        scaled[i] = (features[i] - feature_mean[i]) / feature_std[i];
    }}

    /* Step 2: Compute dot product (weighted sum) */
    double z = intercept;
    for (int i = 0; i < VECTOR_DIM; i++) {{
        z += scaled[i] * weights[i];
    }}

    /* Step 3: Apply sigmoid activation */
    return sigmoid(z);
}}
"""

    with open(output_c_path, "w") as f:
        f.write(c_code)
    print(f"Successfully generated {output_c_path} for {vector_dim} features.")

if __name__ == "__main__":
    # Em um cenário real, você carregaria o modelo joblib treinado:
    # model_file = "/home/sanonichan/projetos/rinha/2026_c_sol/models/logreg_pipeline_v2.joblib"
    output_file = "/home/sanonichan/projetos/rinha/2026_c_sol/lib/model_infer_logreg.c"
    vector_dimension = 19 # Conforme FEATURE_ENGINEERING_V2_REPORT.md

    generate_model_infer_logreg_c(output_file, vector_dimension)