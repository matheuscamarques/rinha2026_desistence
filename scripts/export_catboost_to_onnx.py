#!/usr/bin/env python3
"""Export a trained CatBoost classifier to ONNX.

The script expects the joblib artifact created by `scripts/train_catboost.py`
and exports the embedded CatBoost model.
"""

from __future__ import annotations

import argparse
from pathlib import Path

import joblib
import numpy as np


def load_model(model_path: Path):
    obj = joblib.load(model_path)
    if isinstance(obj, dict) and 'model' in obj:
        return obj['model']
    return obj


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument('--model', default='models/catboost_v1.joblib')
    parser.add_argument('--output', default='models/catboost_v1.onnx')
    parser.add_argument('--n-features', type=int, default=14)
    parser.add_argument('--verify', action='store_true')
    args = parser.parse_args()

    model = load_model(Path(args.model))
    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)

    # CatBoost supports native ONNX export in recent versions.
    model.save_model(str(output_path), format='onnx')
    print('Exported ONNX model to', output_path)

    if args.verify:
        import onnxruntime as ort

        session = ort.InferenceSession(str(output_path), providers=['CPUExecutionProvider'])
        input_name = session.get_inputs()[0].name
        sample = np.zeros((1, args.n_features), dtype=np.float32)
        outputs = session.run(None, {input_name: sample})
        print('ONNX outputs shapes:', [getattr(output, 'shape', None) for output in outputs])

    return 0


if __name__ == '__main__':
    raise SystemExit(main())
