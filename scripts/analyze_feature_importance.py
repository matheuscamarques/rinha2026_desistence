#!/usr/bin/env python3
"""
Feature Importance Analysis for Fraud Detection Model
Extracts weights from trained Logistic Regression and visualizes importance.
"""

import json
import numpy as np
from pathlib import Path

# Try to import visualization libraries (optional)
try:
    import matplotlib.pyplot as plt
    import seaborn as sns
    HAS_MATPLOTLIB = True
except (ImportError, AttributeError) as e:
    HAS_MATPLOTLIB = False
    print(f"⚠ Warning: Visualization libraries unavailable ({type(e).__name__})")
    print("  Continuing with text analysis only...\n")

# Weights extracted from lib/model_infer_logreg.c
FEATURE_NAMES = [
    "amount_normalized",
    "installments_normalized", 
    "amount_vs_customer_avg",
    "hour_of_day",
    "day_of_week",
    "minutes_since_last_tx",
    "km_from_last_tx",
    "km_from_home",
    "tx_count_24h",
    "is_online",
    "card_present",
    "merchant_is_known",
    "merchant_mcc_risk",
    "merchant_avg_amount",
]

# Logistic Regression weights from model_infer_logreg.c
WEIGHTS = np.array([
    1.395724177360535e+00,      # [0] amount_normalized
    1.344075083732605e+00,      # [1] installments_normalized
    4.067320823669434e-01,      # [2] amount_vs_customer_avg
    -6.080102548003197e-02,     # [3] hour_of_day
    -1.381704001687467e-03,     # [4] day_of_week
    -3.225100517272949e+00,     # [5] minutes_since_last_tx
    3.472514152526855e+00,      # [6] km_from_last_tx
    1.362916707992554e+00,      # [7] km_from_home
    1.211409807205200e+00,      # [8] tx_count_24h
    6.824566423892975e-02,      # [9] is_online
    -2.328358404338360e-02,     # [10] card_present
    2.735289335250854e-01,      # [11] merchant_is_known
    3.037384748458862e-01,      # [12] merchant_mcc_risk
    -3.758953511714935e-01,     # [13] merchant_avg_amount
])

def analyze_importance():
    """Analyze and visualize feature importance."""
    
    # Calculate absolute importance (magnitude of coefficient)
    abs_weights = np.abs(WEIGHTS)
    
    # Sort by importance
    sorted_indices = np.argsort(abs_weights)[::-1]
    sorted_names = [FEATURE_NAMES[i] for i in sorted_indices]
    sorted_weights = WEIGHTS[sorted_indices]
    sorted_abs = abs_weights[sorted_indices]
    
    # Create analysis report
    print("=" * 80)
    print("FEATURE IMPORTANCE ANALYSIS - LOGISTIC REGRESSION MODEL")
    print("=" * 80)
    print("\nFeature Importance Ranking (by absolute weight magnitude):\n")
    
    for rank, (idx, name, weight, abs_w) in enumerate(
        zip(sorted_indices, sorted_names, sorted_weights, sorted_abs), 1
    ):
        stars = "⭐" * min(5, max(1, int(abs_w * 2)))
        direction = "↑ FRAUD" if weight > 0 else "↓ LEGIT"
        print(f"{rank:2}. [{idx:2}] {name:25} | Weight: {weight:+.4f} | {stars:5} {direction}")
    
    print("\n" + "=" * 80)
    print("KEY INSIGHTS")
    print("=" * 80)
    
    # Find strong, weak, and negligible features
    strong = sorted_abs >= 1.0
    medium = (sorted_abs >= 0.1) & (sorted_abs < 1.0)
    weak = sorted_abs < 0.1
    
    print(f"\n✓ STRONG features (|w| >= 1.0): {np.sum(strong)}")
    strong_names = [sorted_names[i] for i in range(len(sorted_names)) if strong[i]]
    for name in strong_names:
        print(f"  - {name}")
    
    print(f"\n◇ MEDIUM features (0.1 <= |w| < 1.0): {np.sum(medium)}")
    medium_names = [sorted_names[i] for i in range(len(sorted_names)) if medium[i]]
    for name in medium_names:
        print(f"  - {name}")
    
    print(f"\n✗ WEAK features (|w| < 0.1): {np.sum(weak)}")
    weak_names = [sorted_names[i] for i in range(len(sorted_names)) if weak[i]]
    for name in weak_names:
        print(f"  - {name}")
    
    # Calculate feature contribution statistics
    total_weight = np.sum(abs_weights)
    contributions = (abs_weights / total_weight) * 100
    
    print(f"\n" + "=" * 80)
    print("FEATURE CONTRIBUTION TO MODEL DECISION")
    print("=" * 80)
    print("\nTop 5 contributors:\n")
    top_5_idx = np.argsort(contributions)[::-1][:5]
    cumsum = 0
    for rank, idx in enumerate(top_5_idx, 1):
        contrib = contributions[idx]
        cumsum += contrib
        print(f"{rank}. {FEATURE_NAMES[idx]:25} | {contrib:6.2f}% | Cumulative: {cumsum:6.2f}%")
    
    print("\n" + "=" * 80)
    print("RECOMMENDATIONS FOR FEATURE ENGINEERING")
    print("=" * 80)
    
    print("\n1. CANDIDATES FOR NEW DERIVED FEATURES (High-Impact Source Variables):")
    high_impact = [FEATURE_NAMES[i] for i in np.argsort(abs_weights)[::-1][:6]]
    for feat in high_impact:
        print(f"   - {feat}")
    print("\n   → Create superman rule from km_from_last_tx + minutes_since_last_tx")
    print("   → Create cyclic encoding from hour_of_day (sin/cos)")
    print("   → Create log transforms from amount variables")
    
    print("\n2. CANDIDATES FOR REMOVAL (Very Weak Contribution):")
    remove_candidates = [FEATURE_NAMES[i] for i in range(len(FEATURE_NAMES)) 
                        if contributions[i] < 2.0]
    for feat in remove_candidates:
        contrib = contributions[FEATURE_NAMES.index(feat)]
        print(f"   - {feat} ({contrib:.2f}% contribution)")
    
    print("\n3. FEATURE INTERACTION OPPORTUNITIES:")
    print("   - amount_vs_customer_avg × merchant_mcc_risk (risky merchant + high amount)")
    print("   - km_from_last_tx × tx_speed (velocity + distance)")
    print("   - tx_count_24h × minutes_since_last_tx (burst activity)")
    
    return {
        'feature_names': FEATURE_NAMES,
        'weights': WEIGHTS,
        'abs_weights': abs_weights,
        'contributions': contributions,
        'strong': strong_names,
        'medium': medium_names,
        'weak': weak_names,
        'sorted_indices': sorted_indices,
    }

def create_visualizations(analysis):
    """Create visualization plots."""
    
    if not HAS_MATPLOTLIB:
        print("⊘ Skipping visualization (matplotlib unavailable)")
        return
    
    if not HAS_MATPLOTLIB:
        print("⊘ Skipping visualization (matplotlib unavailable)")
        return
    
    sorted_idx = analysis['sorted_indices']
    sorted_names = [analysis['feature_names'][i] for i in sorted_idx]
    sorted_weights = analysis['weights'][sorted_idx]
    sorted_abs = analysis['abs_weights'][sorted_idx]
    
    # Set up style
    sns.set_style("whitegrid")
    plt.rcParams['figure.figsize'] = (14, 8)
    
    # Create figure with subplots
    fig, axes = plt.subplots(2, 2, figsize=(16, 12))
    
    # Plot 1: Weights (with sign)
    ax1 = axes[0, 0]
    colors = ['#d62728' if w > 0 else '#1f77b4' for w in sorted_weights]
    ax1.barh(range(len(sorted_names)), sorted_weights, color=colors)
    ax1.set_yticks(range(len(sorted_names)))
    ax1.set_yticklabels(sorted_names)
    ax1.set_xlabel('Weight (Coefficient)', fontsize=11, fontweight='bold')
    ax1.set_title('Feature Weights in Logistic Regression\n(Red: Increases Fraud Risk | Blue: Decreases Fraud Risk)', 
                  fontsize=12, fontweight='bold')
    ax1.axvline(x=0, color='black', linestyle='-', linewidth=0.8)
    ax1.grid(True, alpha=0.3)
    
    # Plot 2: Absolute weights (importance magnitude)
    ax2 = axes[0, 1]
    ax2.barh(range(len(sorted_names)), sorted_abs, color='#2ca02c')
    ax2.set_yticks(range(len(sorted_names)))
    ax2.set_yticklabels(sorted_names)
    ax2.set_xlabel('|Weight| (Importance)', fontsize=11, fontweight='bold')
    ax2.set_title('Feature Importance (Absolute Magnitude)', fontsize=12, fontweight='bold')
    ax2.grid(True, alpha=0.3, axis='x')
    
    # Plot 3: Contribution percentage
    ax3 = axes[1, 0]
    contributions = analysis['contributions'][sorted_idx]
    ax3.bar(range(len(sorted_names)), contributions, color='#ff7f0e')
    ax3.set_xticks(range(len(sorted_names)))
    ax3.set_xticklabels(sorted_names, rotation=45, ha='right')
    ax3.set_ylabel('Contribution (%)', fontsize=11, fontweight='bold')
    ax3.set_title('Feature Contribution to Model Decision', fontsize=12, fontweight='bold')
    ax3.grid(True, alpha=0.3, axis='y')
    
    # Plot 4: Cumulative contribution
    ax4 = axes[1, 1]
    cumsum = np.cumsum(contributions)
    ax4.plot(range(len(sorted_names)), cumsum, marker='o', linewidth=2, markersize=6, color='#9467bd')
    ax4.fill_between(range(len(sorted_names)), cumsum, alpha=0.3, color='#9467bd')
    ax4.set_xticks(range(len(sorted_names)))
    ax4.set_xticklabels(sorted_names, rotation=45, ha='right')
    ax4.set_ylabel('Cumulative Contribution (%)', fontsize=11, fontweight='bold')
    ax4.set_ylim([0, 105])
    ax4.axhline(y=80, color='red', linestyle='--', linewidth=1, label='80% threshold')
    ax4.set_title('Cumulative Feature Importance', fontsize=12, fontweight='bold')
    ax4.legend()
    ax4.grid(True, alpha=0.3)
    
    plt.tight_layout()
    
    # Save figure
    output_path = Path(__file__).parent.parent / 'analysis_output' / 'feature_importance.png'
    output_path.parent.mkdir(exist_ok=True)
    plt.savefig(output_path, dpi=150, bbox_inches='tight')
    print(f"\n✓ Visualization saved to: {output_path}")
    
    plt.close()

def load_baseline_metrics():
    """Load baseline test metrics."""
    
    metrics_path = Path(__file__).parent.parent / 'test' / 'results.json'
    
    if not metrics_path.exists():
        print(f"⚠ Baseline metrics file not found: {metrics_path}")
        return None
    
    with open(metrics_path, 'r') as f:
        results = json.load(f)
    
    return results

def print_baseline_summary():
    """Print baseline performance summary."""
    
    results = load_baseline_metrics()
    if not results:
        return
    
    print("\n" + "=" * 80)
    print("BASELINE MODEL PERFORMANCE (Current Production Model)")
    print("=" * 80)
    
    # Calculate metrics
    tp = results['results']['true_positives']
    tn = results['results']['true_negatives']
    fp = results['results']['false_positives']
    fn = results['results']['false_negatives']
    
    tpr = tp / (tp + fn) if (tp + fn) > 0 else 0
    tnr = tn / (tn + fp) if (tn + fp) > 0 else 0
    precision = tp / (tp + fp) if (tp + fp) > 0 else 0
    f1 = 2 * (precision * tpr) / (precision + tpr) if (precision + tpr) > 0 else 0
    accuracy = (tp + tn) / (tp + tn + fp + fn) if (tp + tn + fp + fn) > 0 else 0
    
    print(f"\nDataset Statistics:")
    print(f"  • Total Cases: {tp + tn + fp + fn:,}")
    print(f"  • True Positives (TP):  {tp:,}")
    print(f"  • True Negatives (TN):  {tn:,}")
    print(f"  • False Positives (FP): {fp:,}")
    print(f"  • False Negatives (FN): {fn:,}")
    
    print(f"\nPerformance Metrics:")
    print(f"  • TPR (Recall):    {tpr:.4f} ({tpr*100:.2f}%) - Fraud Detection Rate")
    print(f"  • TNR (Specificity): {tnr:.4f} ({tnr*100:.2f}%) - Legitimate Detection Rate")
    print(f"  • Precision:       {precision:.4f} ({precision*100:.2f}%) - Fraud Confidence")
    print(f"  • F1-Score:        {f1:.4f} - Balanced Metric")
    print(f"  • Accuracy:        {accuracy:.4f} ({accuracy*100:.2f}%)")
    print(f"  • Error Rate:      {results['results']['error_rate']:.4f} ({results['results']['error_rate']*100:.2f}%)")
    
    print(f"\nLatency:")
    print(f"  • P99 Latency: {results['results']['p99_latency']:.2f}ms")
    print(f"  • Composite Score: {results['composite_score']:.2f}")
    
    print(f"\n{'🎯 TARGET FOR V2:':30} Improve TPR & TNR while maintaining P99 latency < 1ms")
    print(f"{'📊 BASELINE FOR COMPARISON':30} Use these metrics as reference for feature engineering impact")

def save_analysis_report(analysis):
    """Save analysis report to JSON."""
    
    report = {
        'timestamp': str(np.datetime64('today')),
        'model': 'Logistic Regression',
        'features': {
            name: {
                'index': i,
                'weight': float(analysis['weights'][i]),
                'abs_weight': float(analysis['abs_weights'][i]),
                'contribution_pct': float(analysis['contributions'][i]),
                'category': 'STRONG' if analysis['abs_weights'][i] >= 1.0 
                          else 'MEDIUM' if analysis['abs_weights'][i] >= 0.1
                          else 'WEAK'
            }
            for i, name in enumerate(analysis['feature_names'])
        },
        'recommendations': {
            'strong_features': analysis['strong'],
            'medium_features': analysis['medium'],
            'weak_features': analysis['weak'],
            'remove_candidates': [name for name in analysis['feature_names'] 
                                 if analysis['contributions'][analysis['feature_names'].index(name)] < 2.0],
            'engineering_candidates': analysis['strong'][:6],
        }
    }
    
    output_path = Path(__file__).parent.parent / 'analysis_output' / 'feature_analysis_report.json'
    output_path.parent.mkdir(exist_ok=True)
    
    with open(output_path, 'w') as f:
        json.dump(report, f, indent=2)
    
    print(f"✓ Analysis report saved to: {output_path}")

if __name__ == '__main__':
    print("\n📊 Starting Feature Importance Analysis...\n")
    
    # Run analysis
    analysis = analyze_importance()
    
    # Create visualizations
    create_visualizations(analysis)
    
    # Save report
    save_analysis_report(analysis)
    
    # Print baseline metrics
    print_baseline_summary()
    
    print("\n" + "=" * 80)
    print("✓ ANALYSIS COMPLETE")
    print("=" * 80)
    print("\nNext Steps:")
    print("1. Review feature importance rankings above")
    print("2. Focus feature engineering on STRONG features (|weight| >= 1.0)")
    print("3. Implement Superman Rule, Cyclic Time, Log Transforms, First Tx Flag")
    print("4. Retrain models with 19 features and compare performance")
    print("=" * 80 + "\n")
