# rinha2026_desistence
# all the code genartes with LLM's  I wont exped why some people understand theese things.

# Training and inference helper

This workspace includes helper scripts to train classical ML models from the internal binary dataset and run inference.

Files added:

- `scripts/train.py` — trains a LogisticRegression pipeline and a LightGBM model from `priv/references.bin`. Saves artifacts to `models/` and attempts ONNX export when conversion packages are installed.
- `scripts/infer.py` — loads joblib/ONNX artifacts and runs a sample inference from `priv/references.bin`.
- `requirements.txt` — Python dependencies for training and ONNX export/inference.

Quick start

1. Create virtualenv and install dependencies:

```bash
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

2. Train models:

```bash
python scripts/train.py --bin priv/references.bin --out models
```

3. Run inference (joblib pipeline):

```bash
python scripts/infer.py --model models/logreg_pipeline.joblib --bin priv/references.bin --index 0
```

4. Run ONNX inference (if ONNX exported):

```bash
python scripts/infer.py --onnx models/logreg.onnx --bin priv/references.bin --index 0
```

Notes

- ONNX export requires `skl2onnx` (for sklearn pipeline) and `onnxmltools` (for LightGBM). If those packages are not available, the export step is skipped but models are saved in `joblib` / LightGBM text format.
- For production C integration consider exporting an ONNX model and using the ONNX Runtime C API; ask me and I can add a minimal C example.

