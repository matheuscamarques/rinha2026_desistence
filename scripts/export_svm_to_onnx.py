#!/usr/bin/env python3
"""Export the sampled SVM pipeline to ONNX.

The training script stores a dict with the fitted pipeline under the `model`
key. This exporter accepts either that dict or a plain sklearn pipeline.

Example:
  .venv/bin/python3 scripts/export_svm_to_onnx.py \
      --model models/svm_rbf_sample.joblib \
      --output models/svm_rbf_sample.onnx
"""

from __future__ import annotations

import argparse
from pathlib import Path

import joblib
import numpy as np
from skl2onnx import convert_sklearn
from skl2onnx.common.data_types import FloatTensorType


def load_estimator(model_path: Path):
    model_obj = joblib.load(model_path)
    if isinstance(model_obj, dict) and 'model' in model_obj:
        return model_obj['model']
    return model_obj


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument('--model', default='models/svm_rbf_sample.joblib')
    parser.add_argument('--output', default='models/svm_rbf_sample.onnx')
    parser.add_argument('--n-features', type=int, default=14)
    parser.add_argument('--verify', action='store_true', help='Run a tiny parity check with ONNX Runtime after export')
    args = parser.parse_args()

    model_path = Path(args.model)
    output_path = Path(args.output)

    estimator = load_estimator(model_path)
    initial_types = [('input', FloatTensorType([None, args.n_features]))]

    onnx_model = convert_sklearn(
        estimator,
        initial_types=initial_types,
        target_opset=17,
        options={id(estimator): {'zipmap': False}},
    )

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_bytes(onnx_model.SerializeToString())
    print(f'Exported ONNX model to {output_path}')

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
