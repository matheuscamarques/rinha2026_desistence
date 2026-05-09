from __future__ import annotations

from dataclasses import dataclass
import math
from typing import Optional

import numpy as np


def _c_factor(size: int) -> float:
    if size <= 1:
        return 0.0
    if size == 2:
        return 1.0
    return 2.0 * (math.log(size - 1.0) + 0.5772156649015329) - (2.0 * (size - 1.0) / size)


@dataclass
class _TreeNode:
    normal: Optional[np.ndarray] = None
    offset: Optional[float] = None
    left: Optional["_TreeNode"] = None
    right: Optional["_TreeNode"] = None
    size: int = 0


class ExtendedIsolationTree:
    def __init__(self, max_depth: int, extension_level: int, random_state: np.random.Generator):
        self.max_depth = max_depth
        self.extension_level = extension_level
        self.random_state = random_state
        self.root_: Optional[_TreeNode] = None

    def fit(self, X: np.ndarray) -> "ExtendedIsolationTree":
        self.root_ = self._build(X, depth=0)
        return self

    def _random_normal(self, n_features: int) -> np.ndarray:
        normal = np.zeros(n_features, dtype=np.float64)
        support = n_features if self.extension_level >= n_features - 1 else max(1, self.extension_level + 1)
        idx = self.random_state.choice(n_features, size=support, replace=False)
        normal[idx] = self.random_state.normal(size=support)
        norm = np.linalg.norm(normal)
        if norm == 0.0:
            normal[idx[0]] = 1.0
            norm = 1.0
        return normal / norm

    def _build(self, X: np.ndarray, depth: int) -> _TreeNode:
        node = _TreeNode(size=X.shape[0])
        if depth >= self.max_depth or X.shape[0] <= 1:
            return node

        n_features = X.shape[1]
        normal = self._random_normal(n_features)
        projections = X @ normal
        lo = float(np.min(projections))
        hi = float(np.max(projections))
        if lo == hi:
            return node

        offset = float(self.random_state.uniform(lo, hi))
        mask = projections < offset
        if mask.all() or (~mask).all():
            return node

        node.normal = normal
        node.offset = offset
        node.left = self._build(X[mask], depth + 1)
        node.right = self._build(X[~mask], depth + 1)
        return node

    def path_length(self, x: np.ndarray) -> float:
        return self._path_length(self.root_, x, depth=0)

    def _path_length(self, node: Optional[_TreeNode], x: np.ndarray, depth: int) -> float:
        if node is None or node.normal is None or node.offset is None:
            return depth + _c_factor(node.size if node is not None else 0)
        if x @ node.normal < node.offset:
            return self._path_length(node.left, x, depth + 1)
        return self._path_length(node.right, x, depth + 1)


class ExtendedIsolationForest:
    def __init__(
        self,
        n_estimators: int = 100,
        max_samples: int | str = 256,
        extension_level: int = 13,
        contamination: float = 0.3,
        random_state: int | None = None,
    ):
        self.n_estimators = n_estimators
        self.max_samples = max_samples
        self.extension_level = extension_level
        self.contamination = contamination
        self.random_state = random_state
        self.trees_: list[ExtendedIsolationTree] = []
        self.threshold_: float = 0.5
        self.scale_: float = 1.0
        self.max_samples_: int = 0

    def fit(self, X: np.ndarray, y=None):
        X = np.asarray(X, dtype=np.float64)
        n_samples = X.shape[0]
        if isinstance(self.max_samples, str) and self.max_samples == 'auto':
            max_samples = min(256, n_samples)
        else:
            max_samples = min(int(self.max_samples), n_samples)
        self.max_samples_ = max_samples
        max_depth = int(math.ceil(math.log2(max_samples))) if max_samples > 1 else 1

        rng = np.random.default_rng(self.random_state)
        self.trees_ = []
        sample_scores = []
        for _ in range(self.n_estimators):
            indices = rng.choice(n_samples, size=max_samples, replace=False)
            Xi = X[indices]
            tree = ExtendedIsolationTree(max_depth=max_depth, extension_level=self.extension_level, random_state=rng)
            tree.fit(Xi)
            self.trees_.append(tree)
            sample_scores.append(self._anomaly_score(Xi))

        train_scores = np.concatenate(sample_scores)
        self.threshold_ = float(np.quantile(train_scores, 1.0 - self.contamination))
        self.scale_ = float(np.std(train_scores)) if float(np.std(train_scores)) > 1e-9 else 1.0
        return self

    def _average_path_length(self, X: np.ndarray) -> np.ndarray:
        lengths = np.zeros(X.shape[0], dtype=np.float64)
        for tree in self.trees_:
            lengths += np.array([tree.path_length(row) for row in X], dtype=np.float64)
        return lengths / max(1, len(self.trees_))

    def _anomaly_score(self, X: np.ndarray) -> np.ndarray:
        X = np.asarray(X, dtype=np.float64)
        avg_path = self._average_path_length(X)
        c = _c_factor(self.max_samples_)
        if c <= 0.0:
            return np.ones_like(avg_path)
        return np.power(2.0, -avg_path / c)

    def score_samples(self, X: np.ndarray) -> np.ndarray:
        return self._anomaly_score(X)

    def decision_function(self, X: np.ndarray) -> np.ndarray:
        return self.score_samples(X) - self.threshold_

    def predict_proba(self, X: np.ndarray) -> np.ndarray:
        scores = self.score_samples(X)
        logits = (scores - self.threshold_) / self.scale_
        probs = 1.0 / (1.0 + np.exp(-logits))
        probs = np.clip(probs, 0.0, 1.0)
        return np.column_stack([1.0 - probs, probs])

    def predict(self, X: np.ndarray) -> np.ndarray:
        return (self.score_samples(X) >= self.threshold_).astype(np.int32)
