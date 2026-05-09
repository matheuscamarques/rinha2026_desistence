#!/usr/bin/env python3
"""Extract prompt-ready statistics for selecting 2D index dimensions.

This tool summarizes a representative sample of the reference dataset so an
LLM can reason about spread, correlation, and bucket entropy without loading
the full 3,000,000-record corpus into its context.

The script understands the repository's gzipped JSON reference file and can
optionally read Parquet samples when pandas is installed.
"""

from __future__ import annotations

import argparse
import gzip
import json
import math
import os
import pathlib
import random
import sys
from dataclasses import dataclass
from typing import Iterable, Iterator, Sequence

FEATURE_NAMES = [
    "amount",
    "installments",
    "amount_vs_avg",
    "hour_of_day",
    "day_of_week",
    "minutes_since_last_tx",
    "km_from_last_tx",
    "km_from_home",
    "tx_count_24h",
    "is_online",
    "card_present",
    "unknown_merchant",
    "mcc_risk",
    "merchant_avg_amount",
]


@dataclass
class FeatureSummary:
    name: str
    mean: float
    stddev: float
    minimum: float
    maximum: float
    zero_fraction: float
    neg1_fraction: float


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Summarize feature statistics for 2D bucket dimension selection"
    )
    parser.add_argument(
        "--input",
        default="priv/references.json.gz",
        help="Reference dataset or sample file (.json, .json.gz, or .parquet)",
    )
    parser.add_argument(
        "--sample-size",
        type=int,
        default=100000,
        help="Maximum number of records to keep in the representative sample",
    )
    parser.add_argument(
        "--seed",
        type=int,
        default=42,
        help="Random seed used for reservoir sampling",
    )
    parser.add_argument(
        "--top-correlations",
        type=int,
        default=20,
        help="Number of strongest correlation pairs to print",
    )
    return parser.parse_args()


def _normalize_vector(raw_record: object) -> list[float]:
    if isinstance(raw_record, dict):
        if "vector" in raw_record:
            vector = raw_record["vector"]
        elif "features" in raw_record:
            vector = raw_record["features"]
        else:
            raise ValueError("record does not contain 'vector' or 'features'")
    else:
        vector = raw_record

    if not isinstance(vector, Sequence):
        raise ValueError("record payload is not a vector-like sequence")

    if len(vector) != len(FEATURE_NAMES):
        raise ValueError(f"expected {len(FEATURE_NAMES)} features, got {len(vector)}")

    return [float(value) for value in vector]


def iter_vectors_from_json(path: str) -> Iterator[list[float]]:
    open_fn = gzip.open if path.endswith(".gz") else open
    mode = "rb" if path.endswith(".gz") else "rt"

    try:
        import ijson  # type: ignore

        with open_fn(path, mode) as file_handle:
            for item in ijson.items(file_handle, "item"):
                yield _normalize_vector(item)
        return
    except Exception:
        pass

    if mode == "rb":
        with open_fn(path, mode) as file_handle:
            data = json.load(file_handle)
    else:
        with open_fn(path, mode, encoding="utf-8") as file_handle:
            data = json.load(file_handle)

    for item in data:
        yield _normalize_vector(item)


def iter_vectors_from_parquet(path: str) -> Iterator[list[float]]:
    try:
        import pandas as pd  # type: ignore
    except Exception as exc:
        raise RuntimeError(
            "Parquet input requires pandas; use a JSON sample or install pandas"
        ) from exc

    frame = pd.read_parquet(path)
    if "vector" in frame.columns:
        for value in frame["vector"]:
            yield _normalize_vector(value)
        return

    feature_columns = [column for column in frame.columns if column != "label"]
    if len(feature_columns) != len(FEATURE_NAMES):
        raise ValueError(
            f"expected {len(FEATURE_NAMES)} feature columns, got {len(feature_columns)}"
        )

    for row in frame[feature_columns].itertuples(index=False, name=None):
        yield _normalize_vector(row)


def iter_vectors(path: str) -> Iterator[list[float]]:
    suffixes = pathlib.Path(path).suffixes
    if suffixes and suffixes[-1] == ".parquet":
        yield from iter_vectors_from_parquet(path)
    else:
        yield from iter_vectors_from_json(path)


def reservoir_sample(vectors: Iterable[list[float]], sample_size: int, seed: int) -> tuple[list[list[float]], int]:
    rng = random.Random(seed)
    sample: list[list[float]] = []
    total = 0

    for vector in vectors:
        total += 1
        if len(sample) < sample_size:
            sample.append(vector)
            continue

        replacement_index = rng.randrange(total)
        if replacement_index < sample_size:
            sample[replacement_index] = vector

    return sample, total


def compute_summaries(sample: Sequence[Sequence[float]]) -> tuple[list[FeatureSummary], list[tuple[int, int, float]]]:
    if not sample:
        raise ValueError("sample is empty")

    sample_count = len(sample)
    dimension_count = len(sample[0])
    sum_values = [0.0] * dimension_count
    sum_squares = [0.0] * dimension_count
    min_values = [float("inf")] * dimension_count
    max_values = [float("-inf")] * dimension_count
    zero_counts = [0] * dimension_count
    neg1_counts = [0] * dimension_count
    sum_products = [[0.0] * dimension_count for _ in range(dimension_count)]

    for vector in sample:
        if len(vector) != dimension_count:
            raise ValueError("sample vectors must all have the same dimension")

        for index, value in enumerate(vector):
            sum_values[index] += value
            sum_squares[index] += value * value
            min_values[index] = value if value < min_values[index] else min_values[index]
            max_values[index] = value if value > max_values[index] else max_values[index]
            if value == 0.0:
                zero_counts[index] += 1
            if value == -1.0:
                neg1_counts[index] += 1

        for left in range(dimension_count):
            left_value = vector[left]
            row = sum_products[left]
            for right in range(left, dimension_count):
                row[right] += left_value * vector[right]

    summaries: list[FeatureSummary] = []
    means = [value / sample_count for value in sum_values]
    stddevs = []
    for index in range(dimension_count):
        mean = means[index]
        if sample_count > 1:
            variance = (sum_squares[index] - sample_count * mean * mean) / (sample_count - 1)
            variance = max(variance, 0.0)
            stddev = math.sqrt(variance)
        else:
            stddev = 0.0
        stddevs.append(stddev)
        summaries.append(
            FeatureSummary(
                name=FEATURE_NAMES[index],
                mean=mean,
                stddev=stddev,
                minimum=min_values[index],
                maximum=max_values[index],
                zero_fraction=zero_counts[index] / sample_count,
                neg1_fraction=neg1_counts[index] / sample_count,
            )
        )

    correlations: list[tuple[int, int, float]] = []
    if sample_count > 1:
        covariance_scale = sample_count - 1
        for left in range(dimension_count):
            for right in range(left + 1, dimension_count):
                numerator = sum_products[left][right] - sample_count * means[left] * means[right]
                covariance = numerator / covariance_scale
                denominator = stddevs[left] * stddevs[right]
                correlation = covariance / denominator if denominator else 0.0
                correlations.append((left, right, correlation))

    correlations.sort(key=lambda item: (-abs(item[2]), item[0], item[1]))
    return summaries, correlations


def format_summary(summaries: Sequence[FeatureSummary], correlations: Sequence[tuple[int, int, float]], sample_count: int, total_count: int, top_n: int) -> str:
    lines: list[str] = []
    lines.append("Feature stats:")
    lines.append(f"sampled_records={sample_count}")
    if total_count:
        lines.append(f"source_records={total_count}")
    lines.append("")
    lines.append("| idx | feature | mean | stddev | min | max | zero_frac | neg1_frac |")
    lines.append("|---:|---|---:|---:|---:|---:|---:|---:|")
    for index, summary in enumerate(summaries):
        lines.append(
            "| {idx} | {name} | {mean:.6f} | {stddev:.6f} | {minimum:.6f} | {maximum:.6f} | {zero:.4f} | {neg1:.4f} |".format(
                idx=index,
                name=summary.name,
                mean=summary.mean,
                stddev=summary.stddev,
                minimum=summary.minimum,
                maximum=summary.maximum,
                zero=summary.zero_fraction,
                neg1=summary.neg1_fraction,
            )
        )

    lines.append("")
    lines.append("Top correlations:")
    if not correlations:
        lines.append("- not enough data")
    else:
        for left, right, correlation in correlations[:top_n]:
            lines.append(
                f"- {left} ({FEATURE_NAMES[left]}) <-> {right} ({FEATURE_NAMES[right]}) = {correlation:.6f}"
            )

    return "\n".join(lines)


def main() -> int:
    args = parse_args()
    if args.sample_size <= 0:
        print("sample-size must be greater than zero", file=sys.stderr)
        return 1

    input_path = os.path.abspath(args.input)
    try:
        sample, total_count = reservoir_sample(iter_vectors(input_path), args.sample_size, args.seed)
        summaries, correlations = compute_summaries(sample)
        print(format_summary(summaries, correlations, len(sample), total_count, args.top_correlations))
    except Exception as exc:
        print(f"failed to extract feature metadata: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())