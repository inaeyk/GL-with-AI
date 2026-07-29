#!/usr/bin/env python3
"""Fixture-only signed Fourier response and rolling-slope analysis."""

from __future__ import annotations

import argparse
import math
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Iterable, List, Mapping, Sequence, Tuple


FIELDS = ("qC", "qS", "HC", "HS", "MxC", "MxS", "MzC", "MzS")
ROLLING_WIDTHS = (0.5, 1.0)
ROLLING_STEP = 0.25
CONTAMINATION_LIMIT = 1.0e-2
SIGNAL_TO_NOISE_MINIMUM = 100.0
SLOPE_VARIATION_LIMIT = 0.20
LINEARITY_TOLERANCE = 5.0e-4


def parse_fields(line: str) -> Dict[str, str]:
    parsed: Dict[str, str] = {}
    for token in line.split()[1:]:
        name, separator, value = token.partition("=")
        if separator:
            parsed[name] = value
    return parsed


@dataclass
class LogData:
    radial: Dict[int, Dict[float, Dict[int, Dict[str, float]]]]
    drift: Dict[float, float]


def read_log(path: Path) -> LogData:
    radial: Dict[int, Dict[float, Dict[int, Dict[str, float]]]] = {}
    drift: Dict[float, float] = {}
    with path.open(encoding="utf-8") as source:
        for line in source:
            if line.startswith("FOURIER_RADIAL_SAMPLE "):
                values = parse_fields(line)
                mode = int(values["mode_number"])
                time = float(values["t"])
                index = int(values["radial_index"])
                radial.setdefault(mode, {}).setdefault(time, {})[index] = {
                    name: float(values[name]) for name in FIELDS
                }
            elif line.startswith("FOURIER_SAMPLE "):
                values = parse_fields(line)
                drift[float(values["t"])] = float(values["state_drift"])
    return LogData(radial=radial, drift=drift)


def signed_rms(
    radial_values: Sequence[Tuple[float, float]],
) -> Tuple[float, float, float]:
    count = len(radial_values)
    if count == 0:
        raise RuntimeError("response contains no radial Fourier values")
    cosine_square = sum(value[0] * value[0] for value in radial_values)
    sine_square = sum(value[1] * value[1] for value in radial_values)
    cosine_sum = sum(value[0] for value in radial_values)
    sine_sum = sum(value[1] for value in radial_values)
    cosine = math.copysign(math.sqrt(cosine_square / count), cosine_sum)
    sine = math.copysign(math.sqrt(sine_square / count), sine_sum)
    return cosine, sine, math.hypot(cosine, sine)


@dataclass
class Response:
    time: float
    odd_amplitude: float
    even_amplitude: float
    contamination: float
    ordinary_amplitude: float
    phase: float
    constraint_odd: Tuple[float, float, float]
    control_amplitude: float
    signal_to_noise: float
    control_drift: float


def common_times(*datasets: Mapping[float, object]) -> List[float]:
    shared = set(datasets[0])
    for dataset in datasets[1:]:
        shared.intersection_update(dataset)
    return sorted(shared)


def build_response(
    mode: int,
    epsilon: float,
    control: LogData,
    plus: LogData,
    minus: LogData,
) -> List[Response]:
    control_mode = control.radial[mode]
    plus_mode = plus.radial[mode]
    minus_mode = minus.radial[mode]
    responses: List[Response] = []
    for time in common_times(control_mode, plus_mode, minus_mode):
        control_radial = control_mode[time]
        plus_radial = plus_mode[time]
        minus_radial = minus_mode[time]
        indices = sorted(
            set(control_radial) & set(plus_radial) & set(minus_radial)
        )
        if not indices:
            raise RuntimeError("signed response has no common radial cells")

        def combine(
            cosine_name: str, sine_name: str, operation: str
        ) -> Tuple[float, float, float]:
            values: List[Tuple[float, float]] = []
            for index in indices:
                zero = control_radial[index]
                positive = plus_radial[index]
                negative = minus_radial[index]
                if operation == "odd":
                    cosine = (
                        positive[cosine_name] - negative[cosine_name]
                    ) / (2.0 * epsilon)
                    sine = (
                        positive[sine_name] - negative[sine_name]
                    ) / (2.0 * epsilon)
                elif operation == "even":
                    cosine = (
                        positive[cosine_name]
                        + negative[cosine_name]
                        - 2.0 * zero[cosine_name]
                    ) / (2.0 * epsilon)
                    sine = (
                        positive[sine_name]
                        + negative[sine_name]
                        - 2.0 * zero[sine_name]
                    ) / (2.0 * epsilon)
                elif operation == "ordinary":
                    cosine = (
                        positive[cosine_name] - zero[cosine_name]
                    ) / epsilon
                    sine = (
                        positive[sine_name] - zero[sine_name]
                    ) / epsilon
                elif operation == "control":
                    cosine = zero[cosine_name]
                    sine = zero[sine_name]
                else:
                    raise RuntimeError(f"unknown response operation {operation}")
                values.append((cosine, sine))
            return signed_rms(values)

        odd_cosine, odd_sine, odd_amplitude = combine("qC", "qS", "odd")
        _, _, even_amplitude = combine("qC", "qS", "even")
        _, _, ordinary_amplitude = combine("qC", "qS", "ordinary")
        _, _, control_amplitude = combine("qC", "qS", "control")
        constraint_odd = tuple(
            combine(cosine_name, sine_name, "odd")[2]
            for cosine_name, sine_name in (
                ("HC", "HS"),
                ("MxC", "MxS"),
                ("MzC", "MzS"),
            )
        )
        noise_floor = max(control_amplitude / epsilon, 1.0e-12)
        contamination = (
            even_amplitude / odd_amplitude
            if odd_amplitude > 0.0
            else math.inf
        )
        signal_to_noise = (
            odd_amplitude / noise_floor if noise_floor > 0.0 else math.inf
        )
        nearest_drift_time = min(
            control.drift, key=lambda candidate: abs(candidate - time)
        )
        responses.append(
            Response(
                time=time,
                odd_amplitude=odd_amplitude,
                even_amplitude=even_amplitude,
                contamination=contamination,
                ordinary_amplitude=ordinary_amplitude,
                phase=math.atan2(odd_sine, odd_cosine),
                constraint_odd=constraint_odd,
                control_amplitude=control_amplitude,
                signal_to_noise=signal_to_noise,
                control_drift=control.drift[nearest_drift_time],
            )
        )
    return responses


@dataclass
class RollingFit:
    width: float
    start: float
    end: float
    points: int
    slope: float
    standard_error: float
    r_squared: float
    maximum_contamination: float
    minimum_signal_to_noise: float


def linear_fit(samples: Sequence[Response], start: float, end: float) -> RollingFit:
    selected = [
        sample
        for sample in samples
        if start - 1.0e-12 <= sample.time <= end + 1.0e-12
        and sample.odd_amplitude > 0.0
    ]
    if len(selected) < 4:
        raise RuntimeError("rolling fit has fewer than four positive samples")
    times = [sample.time for sample in selected]
    values = [math.log(sample.odd_amplitude) for sample in selected]
    mean_time = sum(times) / len(times)
    mean_value = sum(values) / len(values)
    centered_time = sum((time - mean_time) ** 2 for time in times)
    if centered_time <= 0.0:
        raise RuntimeError("rolling fit has singular sample times")
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
    r_squared = 1.0 - residual / total if total > 0.0 else 1.0
    return RollingFit(
        width=end - start,
        start=start,
        end=end,
        points=len(selected),
        slope=slope,
        standard_error=standard_error,
        r_squared=r_squared,
        maximum_contamination=max(
            sample.contamination for sample in selected
        ),
        minimum_signal_to_noise=min(
            sample.signal_to_noise for sample in selected
        ),
    )


def rolling_fits(samples: Sequence[Response], width: float) -> List[RollingFit]:
    final_time = samples[-1].time
    fits: List[RollingFit] = []
    end = width
    while end <= final_time + 1.0e-12:
        fits.append(linear_fit(samples, end - width, end))
        end += ROLLING_STEP
    return fits


def qualifying_triplet(fits: Sequence[RollingFit], final_time: float) -> int:
    for start in range(len(fits) - 3, -1, -1):
        group = fits[start : start + 3]
        if len(group) != 3:
            continue
        if group[0].end < 0.5 * final_time:
            continue
        signs = [1 if fit.slope > 0.0 else -1 for fit in group]
        if signs.count(signs[0]) != len(signs):
            continue
        if any(
            abs(fit.slope) <= fit.standard_error
            or fit.maximum_contamination > CONTAMINATION_LIMIT
            or fit.minimum_signal_to_noise < SIGNAL_TO_NOISE_MINIMUM
            for fit in group
        ):
            continue
        mean_slope = sum(fit.slope for fit in group) / len(group)
        variation = (
            max(fit.slope for fit in group)
            - min(fit.slope for fit in group)
        ) / abs(mean_slope)
        if variation <= SLOPE_VARIATION_LIMIT:
            return signs[0]
    return 0


def classify(
    samples: Sequence[Response],
    fits_by_width: Mapping[float, Sequence[RollingFit]],
) -> str:
    signs = [
        qualifying_triplet(fits_by_width[width], samples[-1].time)
        for width in ROLLING_WIDTHS
    ]
    if signs == [1, 1]:
        return (
            "LATE_TIME_LINEAR_INSTABILITY_DETECTED"
            "_PHYSICAL_IDENTITY_UNRESOLVED"
        )
    if signs == [-1, -1]:
        return "LATE_TIME_NEGATIVE_PLATEAU"
    return "NO_MODE_PLATEAU_WITHIN_TESTED_TIME"


def closest_samples(
    samples: Sequence[Response], interval: float = 0.25
) -> Iterable[Response]:
    target = 0.0
    while target <= samples[-1].time + 1.0e-12:
        yield min(samples, key=lambda sample: abs(sample.time - target))
        target += interval


def history_linearity(
    reference: Sequence[Response], candidate: Sequence[Response]
) -> float:
    candidate_by_time = {sample.time: sample for sample in candidate}
    maximum = 0.0
    for sample in reference:
        if sample.time not in candidate_by_time:
            continue
        other = candidate_by_time[sample.time]
        denominator = max(abs(sample.odd_amplitude), 1.0e-30)
        maximum = max(
            maximum,
            abs(other.odd_amplitude - sample.odd_amplitude) / denominator,
        )
    return maximum


def analyze_mode(
    label: str,
    mode: int,
    epsilon: float,
    control: LogData,
    plus: LogData,
    minus: LogData,
    half_plus: LogData | None,
    half_minus: LogData | None,
) -> str:
    samples = build_response(mode, epsilon, control, plus, minus)
    fits_by_width = {
        width: rolling_fits(samples, width) for width in ROLLING_WIDTHS
    }
    conclusion = classify(samples, fits_by_width)

    for sample in closest_samples(samples):
        print(
            "TRANSIENT_RESPONSE"
            f" label={label} mode_number={mode} t={sample.time:.12e}"
            f" odd_amplitude={sample.odd_amplitude:.12e}"
            f" even_amplitude={sample.even_amplitude:.12e}"
            f" even_to_odd={sample.contamination:.12e}"
            f" ordinary_amplitude={sample.ordinary_amplitude:.12e}"
            f" phase={sample.phase:.12e}"
            f" H_odd={sample.constraint_odd[0]:.12e}"
            f" Mx_odd={sample.constraint_odd[1]:.12e}"
            f" Mz_odd={sample.constraint_odd[2]:.12e}"
            f" control_fourier={sample.control_amplitude:.12e}"
            f" signal_to_noise={sample.signal_to_noise:.12e}"
            f" control_drift={sample.control_drift:.12e}"
        )
    for width in ROLLING_WIDTHS:
        for fit in fits_by_width[width]:
            print(
                "TRANSIENT_ROLLING"
                f" label={label} width={width:.2f}"
                f" start={fit.start:.12e} end={fit.end:.12e}"
                f" points={fit.points} slope={fit.slope:.12e}"
                f" standard_error={fit.standard_error:.12e}"
                f" R2={fit.r_squared:.12e}"
                f" maximum_even_to_odd={fit.maximum_contamination:.12e}"
                f" minimum_signal_to_noise={fit.minimum_signal_to_noise:.12e}"
            )

    if half_plus is not None and half_minus is not None:
        half_samples = build_response(
            mode, 0.5 * epsilon, control, half_plus, half_minus
        )
        linearity_error = history_linearity(samples, half_samples)
        linearity_pass = linearity_error <= LINEARITY_TOLERANCE
        print(
            "TRANSIENT_LINEARITY"
            f" label={label} epsilon={epsilon:.12e}"
            f" second_epsilon={0.5 * epsilon:.12e}"
            f" maximum_relative_difference={linearity_error:.12e}"
            f" tolerance={LINEARITY_TOLERANCE:.12e}"
            f" result={'PASS' if linearity_pass else 'FAIL'}"
        )
        if not linearity_pass:
            conclusion = "NO_MODE_PLATEAU_WITHIN_TESTED_TIME"

    maximum_contamination = max(sample.contamination for sample in samples)
    minimum_signal_to_noise = min(
        sample.signal_to_noise for sample in samples
    )
    maximum_control_drift = max(sample.control_drift for sample in samples)
    print(
        "TRANSIENT_MODE_RESULT"
        f" label={label} mode_number={mode}"
        f" final_time={samples[-1].time:.12e}"
        f" maximum_even_to_odd={maximum_contamination:.12e}"
        f" minimum_signal_to_noise={minimum_signal_to_noise:.12e}"
        f" maximum_control_drift={maximum_control_drift:.12e}"
        f" conclusion={conclusion}"
    )
    return conclusion


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--control-low", type=Path, required=True)
    parser.add_argument("--low-plus", type=Path, required=True)
    parser.add_argument("--low-minus", type=Path, required=True)
    parser.add_argument("--control-high", type=Path, required=True)
    parser.add_argument("--high-plus", type=Path, required=True)
    parser.add_argument("--high-minus", type=Path, required=True)
    parser.add_argument("--low-plus-half", type=Path)
    parser.add_argument("--low-minus-half", type=Path)
    arguments = parser.parse_args()

    control_low = read_log(arguments.control_low)
    low_plus = read_log(arguments.low_plus)
    low_minus = read_log(arguments.low_minus)
    control_high = read_log(arguments.control_high)
    high_plus = read_log(arguments.high_plus)
    high_minus = read_log(arguments.high_minus)
    low_plus_half = (
        read_log(arguments.low_plus_half)
        if arguments.low_plus_half is not None
        else None
    )
    low_minus_half = (
        read_log(arguments.low_minus_half)
        if arguments.low_minus_half is not None
        else None
    )

    low_conclusion = analyze_mode(
        "k_pi_over_4",
        1,
        1.0e-8,
        control_low,
        low_plus,
        low_minus,
        low_plus_half,
        low_minus_half,
    )
    high_conclusion = analyze_mode(
        "k_pi_over_2",
        2,
        1.0e-8,
        control_high,
        high_plus,
        high_minus,
        None,
        None,
    )
    print(
        "TRANSIENT_DIAGNOSTIC_RESULT"
        f" low={low_conclusion} high={high_conclusion}"
    )


if __name__ == "__main__":
    main()
