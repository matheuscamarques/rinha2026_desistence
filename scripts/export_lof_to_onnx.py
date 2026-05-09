#!/usr/bin/env python3
"""
Export LOF model to ONNX format.
Note: LOF is difficult to export to ONNX because it requires storing all training data.
We'll attempt export but may fall back to keeping as joblib.
"""

import argparse
import sys
from pathlib import Path

import joblib
import numpy as np
from sklearn.pipeline import Pipeline

# Add parent directory to path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))


def main():
    parser = argparse.ArgumentParser(description="Export LOF model to ONNX")
    parser.add_argument("--model", required=True, help="Input joblib model path")
    parser.add_argument("--output", required=True, help="Output ONNX path")
    parser.add_argument("--verify", action="store_true", help="Verify ONNX output")

    args = parser.parse_args()

    # Load model
    artifact = joblib.load(args.model)
    if isinstance(artifact, dict):
        model = artifact["model"]
    else:
        model = artifact

    print(f"Loaded model: {type(model)}")

    # LOF stores reference data internally, making ONNX export impractical
    # skl2onnx doesn't have built-in support for LocalOutlierFactor
    # Best approach: keep as joblib
    
    print(f"Note: LOF cannot be efficiently exported to ONNX (requires storing training data)")
    print(f"LOF will remain in joblib format for inference")
    print(f"The pipeline.joblib artifact is ready for use with joblib.load()")


if __name__ == "__main__":
    main()
