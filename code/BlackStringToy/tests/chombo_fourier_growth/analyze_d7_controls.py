#!/usr/bin/env python3
"""Fixture-only Stage 4AO-D7 control diagnostics."""

from __future__ import annotations

import argparse
import math
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Iterable, List, Mapping, Sequence, Tuple


THRESHOLDS = (1.0e-3, 1.0e-2, 1.0e-1, 1.0)
ROLLING_WIDTH = 1.0
ROLLING_STEP = 0.25
SERIES_STEP = 0.25
VARIABLE_SERIES_STEP = 1.0


def fields(line: str) -> Dict[str, str]:
    result: Dict[str, str] = {}
    for token in line.split()[1:]:
        name, separator, value = token.partition("=")
        if separator:
            result[name] = value
    return result


@dataclass
class ControlData:
    samples: Dict[float, Dict[str, str]]
    variables: Dict[float, Dict[str, Dict[str, str]]]


def read_control(path: Path) -> ControlData:
    samples: Dict[float, Dict[str, str]] = {}
    variables: Dict[float, Dict[str, Dict[str, str]]] = {}
    with path.open(encoding="utf-8") as source:
        for line in source:
            if line.startswith(("D7_SAMPLE ", "D8_SAMPLE ")):
                parsed = fields(line)
                samples[float(parsed["t"])] = parsed
            elif line.startswith(("D7_VARIABLE ", "D8_VARIABLE ")):
                parsed = fields(line)
                time = float(parsed["t"])
                variables.setdefault(time, {})[parsed["name"]] = parsed
    if not samples:
        raise RuntimeError(f"{path} contains no D7 samples")
    return ControlData(samples=samples, variables=variables)


def read_baseline(path: Path) -> Dict[float, float]:
    result: Dict[float, float] = {}
    with path.open(encoding="utf-8") as source:
        for line in source:
            if line.startswith("#") or not line.strip():
                continue
            time, drift = line.split()
            result[float(time)] = float(drift)
    return result


def closest_times(times: Iterable[float], step: float) -> Iterable[float]:
    available = sorted(times)
    target = 0.0
    used = set()
    while target <= available[-1] + 1.0e-12:
        selected = min(available, key=lambda value: abs(value - target))
        if selected not in used:
            used.add(selected)
            yield selected
        target += step


@dataclass
class Fit:
    start: float
    end: float
    points: int
    slope: float
    standard_error: float
    r_squared: float


def fit_log_series(
    series: Mapping[float, float], start: float, end: float
) -> Fit | None:
    selected = sorted(
        (time, value)
        for time, value in series.items()
        if start - 1.0e-12 <= time <= end + 1.0e-12 and value > 0.0
    )
    if len(selected) < 4:
        return None
    times = [item[0] for item in selected]
    values = [math.log(item[1]) for item in selected]
    mean_time = sum(times) / len(times)
    mean_value = sum(values) / len(values)
    centered_time = sum((value - mean_time) ** 2 for value in times)
    if centered_time <= 0.0:
        return None
    slope = sum(
        (time - mean_time) * (value - mean_value)
        for time, value in zip(times, values)
    ) / centered_time
    intercept = mean_value - slope * mean_time
    residual = sum(
        (value - (intercept + slope * time)) ** 2
        for time, value in zip(times, values)
    )
    total = sum((value - mean_value) ** 2 for value in values)
    standard_error = math.sqrt(
        residual / ((len(selected) - 2) * centered_time)
    )
    return Fit(
        start=start,
        end=end,
        points=len(selected),
        slope=slope,
        standard_error=standard_error,
        r_squared=1.0 - residual / total if total > 0.0 else 1.0,
    )


def rolling_fits(series: Mapping[float, float]) -> List[Fit]:
    final_time = max(series)
    result: List[Fit] = []
    end = ROLLING_WIDTH
    while end <= final_time + 1.0e-12:
        fit = fit_log_series(series, end - ROLLING_WIDTH, end)
        if fit is not None:
            result.append(fit)
        end += ROLLING_STEP
    return result


def first_crossing(series: Mapping[float, float], threshold: float) -> float | None:
    for time, value in sorted(series.items()):
        if value >= threshold:
            return time
    return None


def format_crossing(value: float | None) -> str:
    return "NONE" if value is None else f"{value:.12e}"


def numeric_series(data: ControlData, name: str) -> Dict[float, float]:
    return {
        time: float(sample[name]) for time, sample in data.samples.items()
    }


def output_control(variant: str, data: ControlData) -> None:
    for time in closest_times(data.samples, SERIES_STEP):
        sample = data.samples[time]
        print(
            "D7_SERIES"
            f" variant={variant} t={time:.12e}"
            f" state_max={float(sample['state_max']):.12e}"
            f" state_l2={float(sample['state_l2']):.12e}"
            f" state_max_variable={sample['state_max_variable']}"
            f" state_max_location={sample['state_max_location']}"
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
            f" determinant_l2={float(sample['determinant_l2']):.12e}"
            f" determinant_location={sample['determinant_location']}"
            f" weighted_trace_max={float(sample['weighted_trace_max']):.12e}"
            f" weighted_trace_l2={float(sample['weighted_trace_l2']):.12e}"
            f" weighted_trace_location={sample['weighted_trace_location']}"
            f" maximum_z_span={float(sample['maximum_z_span']):.12e}"
            f" maximum_z_span_variable={sample['maximum_z_span_variable']}"
            f" maximum_fourier={float(sample['maximum_fourier']):.12e}"
            f" maximum_fourier_variable={sample['maximum_fourier_variable']}"
        )

    for time in closest_times(data.variables, VARIABLE_SERIES_STEP):
        for name, variable in sorted(
            data.variables[time].items(), key=lambda item: int(item[1]["slot"])
        ):
            print(
                "D7_VARIABLE_SERIES"
                f" variant={variant} t={time:.12e}"
                f" slot={variable['slot']} name={name}"
                f" maximum={float(variable['maximum']):.12e}"
                f" l2={float(variable['l2']):.12e}"
                f" location={variable['location']}"
                f" z_span={float(variable['z_span']):.12e}"
                f" fourier={float(variable['fourier']):.12e}"
            )

    metrics = {
        "state_max": numeric_series(data, "state_max"),
        "H_max": numeric_series(data, "H_max"),
        "Mx_max": numeric_series(data, "Mx_max"),
        "Mz_max": numeric_series(data, "Mz_max"),
    }
    for metric, series in metrics.items():
        for fit in rolling_fits(series):
            print(
                "D7_ROLLING"
                f" variant={variant} metric={metric}"
                f" width={ROLLING_WIDTH:.2f}"
                f" start={fit.start:.12e} end={fit.end:.12e}"
                f" points={fit.points} slope={fit.slope:.12e}"
                f" standard_error={fit.standard_error:.12e}"
                f" R2={fit.r_squared:.12e}"
            )

    state_series = metrics["state_max"]
    for threshold in THRESHOLDS:
        print(
            "D7_CROSSING"
            f" variant={variant} threshold={threshold:.12e}"
            f" first_time={format_crossing(first_crossing(state_series, threshold))}"
        )

    final_time = max(data.samples)
    final = data.samples[final_time]
    late_fit = rolling_fits(state_series)[-1]
    print(
        "D7_FINAL"
        f" variant={variant} t={final_time:.12e}"
        f" state_max={float(final['state_max']):.12e}"
        f" state_l2={float(final['state_l2']):.12e}"
        f" state_location={final['state_max_location']}"
        f" H_max={float(final['H_max']):.12e}"
        f" Mx_max={float(final['Mx_max']):.12e}"
        f" Mz_max={float(final['Mz_max']):.12e}"
        f" determinant_max={float(final['determinant_max']):.12e}"
        f" weighted_trace_max={float(final['weighted_trace_max']):.12e}"
        f" maximum_z_span={float(final['maximum_z_span']):.12e}"
        f" maximum_fourier={float(final['maximum_fourier']):.12e}"
        f" late_state_slope={late_fit.slope:.12e}"
        f" late_state_slope_se={late_fit.standard_error:.12e}"
        f" late_state_R2={late_fit.r_squared:.12e}"
    )


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--baseline", type=Path, required=True)
    parser.add_argument("--exact-gp", type=Path)
    parser.add_argument("--exact-gp-failure")
    parser.add_argument("--frozen-gauge", type=Path, required=True)
    parser.add_argument("--half-cfl", type=Path, required=True)
    parser.add_argument("--fine", type=Path, required=True)
    arguments = parser.parse_args()

    baseline = read_baseline(arguments.baseline)
    for time, drift in sorted(baseline.items()):
        print(
            "D7_BASELINE_REUSED"
            f" t={time:.12e} state_max={drift:.12e}"
            " source=D6_TRANSIENT_LOG"
        )
    for threshold in THRESHOLDS:
        print(
            "D7_BASELINE_CROSSING"
            f" threshold={threshold:.12e}"
            f" first_time={format_crossing(first_crossing(baseline, threshold))}"
        )
    baseline_fit = rolling_fits(baseline)[-1]
    print(
        "D7_BASELINE_FINAL"
        f" t={max(baseline):.12e} state_max={baseline[max(baseline)]:.12e}"
        f" late_state_slope={baseline_fit.slope:.12e}"
        f" late_state_slope_se={baseline_fit.standard_error:.12e}"
        f" late_state_R2={baseline_fit.r_squared:.12e}"
        " detailed_variables=UNAVAILABLE_NOT_REGENERATED"
        " constraints=UNAVAILABLE_NOT_REGENERATED"
    )

    controls = {
        "frozen_gauge": read_control(arguments.frozen_gauge),
        "half_cfl": read_control(arguments.half_cfl),
        "fine": read_control(arguments.fine),
    }
    if arguments.exact_gp is not None:
        controls = {"exact_gp": read_control(arguments.exact_gp), **controls}
    elif arguments.exact_gp_failure is not None:
        print(
            "D7_CONTROL_FAILURE variant=exact_gp"
            f" result={arguments.exact_gp_failure}"
            " completed_time=UNAVAILABLE_BUFFERED_ABORT"
            " detailed_metrics=UNAVAILABLE_AFTER_EARLY_TERMINATION"
        )
    else:
        raise RuntimeError(
            "exact-GP control log or explicit failure result is required"
        )
    for variant, data in controls.items():
        output_control(variant, data)

    frozen = controls["frozen_gauge"]
    frozen_final_time = max(frozen.variables)
    frozen_final = frozen.variables[frozen_final_time]
    frozen_lapse_shift = max(
        float(frozen_final[name]["maximum"])
        for name in ("lapse", "shiftX", "shiftZ")
    )
    frozen_b = max(
        float(frozen_final[name]["maximum"]) for name in ("Bx", "Bz")
    )
    print(
        "D7_GAUGE_FREEZE"
        " variant=frozen_gauge"
        f" lapse_shift_max={frozen_lapse_shift:.12e}"
        f" B_max={frozen_b:.12e}"
        " result="
        + ("COMPLETE" if frozen_b <= 1.0e-12 else "INCOMPLETE_B_EVOLVES")
    )

    baseline_final = baseline[max(baseline)]
    baseline_onset = first_crossing(baseline, 1.0e-1)
    for variant, data in controls.items():
        state = numeric_series(data, "state_max")
        final_ratio = state[max(state)] / baseline_final
        onset = first_crossing(state, 1.0e-1)
        onset_delay = (
            math.inf
            if onset is None
            else onset - (baseline_onset if baseline_onset is not None else 0.0)
        )
        print(
            "D7_INDICATOR"
            f" variant={variant}"
            f" final_state_ratio_to_baseline={final_ratio:.12e}"
            f" threshold_1e-1_time={format_crossing(onset)}"
            f" threshold_1e-1_delay={format_crossing(onset_delay)}"
        )


if __name__ == "__main__":
    main()
