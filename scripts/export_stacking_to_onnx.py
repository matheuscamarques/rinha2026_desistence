#!/usr/bin/env python3
"""
Export Stacking Ensemble to ONNX.
Note: Stacking Ensemble is complex and ONNX export is challenging.
We'll keep it in joblib format for maximum compatibility.
"""

import argparse
import sys
from pathlib import Path

import joblib

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))


def main():
    parser = argparse.ArgumentParser(description="Export Stacking Ensemble to ONNX")
    parser.add_argument("--model", required=True, help="Input joblib model path")
    parser.add_argument("--output", required=True, help="Output ONNX path (optional)")

    args = parser.parse_args()

    # Load model
    artifact = joblib.load(args.model)
    
    print("Stacking Ensemble loaded successfully")
    print(f"Base models: {artifact.get('base_models_names', [])}")
    print(f"Model type: {artifact.get('model_type', 'unknown')}")
    
    print("\nNote: Stacking Ensemble is best kept in joblib format.")
    print("ONNX export is not practical for multi-level ensembles.")
    print("Use joblib.load() for production inference.")


if __name__ == "__main__":
    main()
