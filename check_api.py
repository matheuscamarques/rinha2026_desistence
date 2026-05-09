#!/usr/bin/env python3
from treelite.frontend import from_lightgbm
import lightgbm as lgb

bst = lgb.Booster(model_file="models/lgb_model.txt")
model_tl = from_lightgbm(bst)

print("Available methods on Model:")
print([m for m in dir(model_tl) if not m.startswith('_')][:20])
