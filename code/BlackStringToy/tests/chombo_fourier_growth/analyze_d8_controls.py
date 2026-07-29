#!/usr/bin/env python3
"""Fixture-only Stage 4AO-D8 frozen-control analysis."""

from __future__ import annotations

import argparse
from pathlib import Path

from analyze_d7_controls import (
    THRESHOLDS,
    closest_times,
    first_crossing,
    format_crossing,
    numeric_series,
    read_baseline,
    read_control,
    rolling_fits,
)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--baseline", type=Path, required=True)
    parser.add_argument("--frozen-gauge", type=Path, required=True)
    arguments = parser.parse_args()

    baseline = read_baseline(arguments.baseline)
    frozen = read_control(arguments.frozen_gauge)

    for time in closest_times(frozen.samples, 0.25):
        sample = frozen.samples[time]
        print(
            "D8_SERIES"
            f" t={time:.12e}"
            f" state_max={float(sample['state_max']):.12e}"
            f" state_l2={float(sample['state_l2']):.12e}"
            f" state_variable={sample['state_max_variable']}"
            f" state_location={sample['state_max_location']}"
            f" H_max={float(sample['H_max']):.12e}"
            f" H_l2={float(sample['H_l2']):.12e}"
            f" H_location={sample['H_location']}"
            f" Mx_max={float(sample['Mx_max']):.12e}"
            f" Mx_l2={float(sample['Mx_l2']):.12e}"
            f" Mx_location={sample['Mx_location']}"
            f" Mz_max={float(sample['Mz_max']):.12e}"
            f" Mz_l2={float(sample['Mz_l2']):.12e}"
            f" Mz_location={sample['Mz_location']}"
            f" determinant_max={float(sample['determinant_max']):.12e}"
            f" weighted_trace_max="
            f"{float(sample['weighted_trace_max']):.12e}"
            f" maximum_z_span={float(sample['maximum_z_span']):.12e}"
            f" maximum_fourier={float(sample['maximum_fourier']):.12e}"
        )

    final_time = max(frozen.variables)
    for name, variable in sorted(
        frozen.variables[final_time].items(),
        key=lambda item: int(item[1]["slot"]),
    ):
        print(
            "D8_VARIABLE_FINAL"
            f" slot={variable['slot']} name={name}"
            f" maximum={float(variable['maximum']):.12e}"
            f" l2={float(variable['l2']):.12e}"
            f" location={variable['location']}"
            f" z_span={float(variable['z_span']):.12e}"
            f" fourier={float(variable['fourier']):.12e}"
        )

    metrics = {
        "state_max": numeric_series(frozen, "state_max"),
        "H_max": numeric_series(frozen, "H_max"),
        "Mx_max": numeric_series(frozen, "Mx_max"),
        "Mz_max": numeric_series(frozen, "Mz_max"),
    }
    for metric, series in metrics.items():
        for fit in rolling_fits(series):
            if fit.end + 1.0e-12 < 5.0:
                continue
            print(
                "D8_ROLLING"
                f" metric={metric}"
                f" start={fit.start:.12e} end={fit.end:.12e}"
                f" slope={fit.slope:.12e}"
                f" standard_error={fit.standard_error:.12e}"
                f" R2={fit.r_squared:.12e}"
            )

    state = metrics["state_max"]
    for threshold in THRESHOLDS:
        print(
            "D8_CROSSING"
            f" threshold={threshold:.12e}"
            f" first_time={format_crossing(first_crossing(state, threshold))}"
        )

    baseline_final = baseline[max(baseline)]
    baseline_crossing = first_crossing(baseline, 1.0e-1)
    final_sample = frozen.samples[max(frozen.samples)]
    print(
        "D8_FINAL"
        f" t={max(frozen.samples):.12e}"
        f" state_max={float(final_sample['state_max']):.12e}"
        f" state_l2={float(final_sample['state_l2']):.12e}"
        f" final_ratio_to_baseline="
        f"{float(final_sample['state_max']) / baseline_final:.12e}"
        f" threshold_1e-1_delay="
        f"{first_crossing(state, 1.0e-1) - baseline_crossing:.12e}"
        f" H_max={float(final_sample['H_max']):.12e}"
        f" Mx_max={float(final_sample['Mx_max']):.12e}"
        f" Mz_max={float(final_sample['Mz_max']):.12e}"
        f" determinant_max="
        f"{float(final_sample['determinant_max']):.12e}"
        f" weighted_trace_max="
        f"{float(final_sample['weighted_trace_max']):.12e}"
        f" maximum_z_span="
        f"{float(final_sample['maximum_z_span']):.12e}"
        f" maximum_fourier="
        f"{float(final_sample['maximum_fourier']):.12e}"
    )


if __name__ == "__main__":
    main()
