#!/usr/bin/env python3
"""Inference helpers: load saved models (joblib or ONNX) and run predictions.

Usage examples:
  python scripts/infer.py --model models/logreg_pipeline.joblib
  python scripts/infer.py --onnx models/logreg.onnx
"""
from pathlib import Path
import numpy as np
import joblib
import argparse

try:
    import onnxruntime as ort
    ORT = True
except Exception:
    ORT = False


def read_sample_from_bin(bin_path: Path, idx: int = 0):
    record_size = 16
    data = np.fromfile(str(bin_path), dtype=np.uint8)
    n = data.size // record_size
    if idx < 0 or idx >= n:
        raise IndexError('sample index out of range')
    data = data.reshape((n, record_size))
    X = data[:, :14].astype(np.float32)
    return X[idx]


def main():
    p = argparse.ArgumentParser()
    p.add_argument('--model', help='joblib sklearn pipeline or scaler+model')
    p.add_argument('--lgb', help='LightGBM model path (txt)')
    p.add_argument('--onnx', help='ONNX model path')
    p.add_argument('--bin', default='priv/references.bin')
    p.add_argument('--index', type=int, default=0)
    args = p.parse_args()

    sample = read_sample_from_bin(Path(args.bin), args.index)
    print('Sample features:', sample.tolist())

    if args.model:
        clf = joblib.load(args.model)
        if hasattr(clf, 'predict_proba'):
            print('joblib proba:', clf.predict_proba(sample.reshape(1, -1))[:, 1])
        else:
            print('joblib predict:', clf.predict(sample.reshape(1, -1)))

    if args.onnx:
        if not ORT:
            print('onnxruntime not installed; cannot run ONNX model')
        else:
            sess = ort.InferenceSession(str(args.onnx))
            inp_name = sess.get_inputs()[0].name
            out = sess.run(None, {inp_name: sample.reshape(1, -1).astype(np.float32)})
            print('onnx outputs:', out)


if __name__ == '__main__':
    main()
