#!/usr/bin/env python3
"""Export the trained KNN pipeline to ONNX.

This expects the joblib artifact produced by `scripts/train_knn.py` and exports
the fitted sklearn pipeline stored under the `model` key.
"""

from __future__ import annotations

import argparse
from pathlib import Path

import joblib
import numpy as np
from skl2onnx import convert_sklearn
from skl2onnx.common.data_types import FloatTensorType


def load_model(model_path: Path):
    obj = joblib.load(model_path)
    if isinstance(obj, dict) and 'model' in obj:
        return obj['model']
    return obj


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument('--model', default='models/knn_v1.joblib')
    parser.add_argument('--output', default='models/knn_v1.onnx')
    parser.add_argument('--n-features', type=int, default=14)
    parser.add_argument('--verify', action='store_true')
    args = parser.parse_args()

    model = load_model(Path(args.model))
    initial_types = [('input', FloatTensorType([None, args.n_features]))]

    onnx_model = convert_sklearn(
        model,
        initial_types=initial_types,
        target_opset=17,
        options={id(model): {'zipmap': False}},
    )

    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_bytes(onnx_model.SerializeToString())
    print('Exported ONNX model to', output_path)

    if args.verify:
        import onnxruntime as ort

        sess = ort.InferenceSession(str(output_path), providers=['CPUExecutionProvider'])
        input_name = sess.get_inputs()[0].name
        sample = np.zeros((1, args.n_features), dtype=np.float32)
        outputs = sess.run(None, {input_name: sample})
        print('ONNX outputs shapes:', [getattr(o, 'shape', None) for o in outputs])

    return 0


if __name__ == '__main__':
    raise SystemExit(main())
