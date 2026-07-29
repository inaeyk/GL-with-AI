#!/usr/bin/env python3
"""Fixture-only Stage 4AO-D12 signed-mode and plateau analysis."""

from __future__ import annotations

import argparse
import cmath
import math
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Iterable, List, Mapping, Sequence, Tuple


FIELD_PAIRS = {
    "q": ("qC", "qS"),
    "ratio": ("ratioC", "ratioS"),
    "K": ("KC", "KS"),
    "Aww": ("AwwC", "AwwS"),
    "GammaX": ("GammaXC", "GammaXS"),
}
CONSTRAINT_PAIRS = {
    "H": ("HC", "HS"),
    "Mx": ("MxC", "MxS"),
    "Mz": ("MzC", "MzS"),
}
ROLLING_WIDTHS = (0.5, 1.0)
ROLLING_STEP = 0.25
CONTAMINATION_LIMIT = 1.0e-3
SLOPE_VARIATION_LIMIT = 0.20
FIELD_SLOPE_TOLERANCE = 0.25
PROFILE_OVERLAP_LIMIT = 0.99
LEAKAGE_RATIO_MINIMUM = 100.0
CONSTRAINT_RAPID_SLOPE = 0.5


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
    background: Dict[float, Dict[str, str]]
    invalid_metric: bool
    counts: Dict[str, str]
    case: Dict[str, str]


def read_log(path: Path) -> LogData:
    radial: Dict[int, Dict[float, Dict[int, Dict[str, float]]]] = {}
    background: Dict[float, Dict[str, str]] = {}
    invalid_metric = False
    counts: Dict[str, str] = {}
    case: Dict[str, str] = {}
    with path.open(encoding="utf-8") as source:
        for line in source:
            if line.startswith("FOURIER_RADIAL_SAMPLE "):
                values = parse_fields(line)
                mode = int(values["mode_number"])
                time = float(values["t"])
                index = int(values["radial_index"])
                radial.setdefault(mode, {}).setdefault(time, {})[index] = {
                    name: float(value)
                    for name, value in values.items()
                    if name
                    not in {
                        "mode",
                        "kind",
                        "mode_number",
                        "epsilon",
                        "t",
                        "radial_index",
                    }
                }
            elif line.startswith("D9_SAMPLE "):
                values = parse_fields(line)
                background[float(values["t"])] = values
            elif line.startswith("D9_INVALID_METRIC "):
                invalid_metric = True
            elif line.startswith("FOURIER_COUNTS "):
                counts = parse_fields(line)
            elif line.startswith("FOURIER_CASE "):
                case = parse_fields(line)
    if not radial or not background or not counts or not case:
        raise RuntimeError(f"incomplete D12 fixture log: {path}")
    return LogData(radial, background, invalid_metric, counts, case)


def common_times(*datasets: Mapping[float, object]) -> List[float]:
    shared = set(datasets[0])
    for dataset in datasets[1:]:
        shared.intersection_update(dataset)
    return sorted(shared)


def rms(values: Sequence[complex]) -> float:
    if not values:
        raise RuntimeError("empty radial response")
    return math.sqrt(sum(abs(value) ** 2 for value in values) / len(values))


def normalized_profile(values: Sequence[complex]) -> Tuple[complex, ...]:
    norm = math.sqrt(sum(abs(value) ** 2 for value in values))
    if not math.isfinite(norm) or norm == 0.0:
        return tuple(0.0j for _ in values)
    normalized = [value / norm for value in values]
    anchor = max(range(len(normalized)), key=lambda index: abs(normalized[index]))
    phase = cmath.phase(normalized[anchor])
    rotation = cmath.exp(-1.0j * phase)
    return tuple(value * rotation for value in normalized)


def profile_overlap(left: Sequence[complex], right: Sequence[complex]) -> float:
    left_norm = math.sqrt(sum(abs(value) ** 2 for value in left))
    right_norm = math.sqrt(sum(abs(value) ** 2 for value in right))
    if left_norm == 0.0 or right_norm == 0.0:
        return 0.0
    product = sum(
        first.conjugate() * second for first, second in zip(left, right)
    )
    return abs(product) / (left_norm * right_norm)


@dataclass
class FieldResponse:
    odd: float
    even: float
    ordinary: float
    control: float
    phase: float
    contamination: float
    seeded_to_leakage: float
    profile: Tuple[complex, ...]


@dataclass
class ModeResponse:
    time: float
    x: Tuple[float, ...]
    fields: Dict[str, FieldResponse]
    constraints: Dict[str, float]
    radial_nyquist_fraction: float
    spurious_harmonic_fraction: float = math.nan


def vectors(
    control: Mapping[int, Mapping[str, float]],
    plus: Mapping[int, Mapping[str, float]],
    minus: Mapping[int, Mapping[str, float]],
    pair: Tuple[str, str],
    epsilon: float,
) -> Tuple[Tuple[int, ...], Dict[str, List[complex]]]:
    indices = tuple(sorted(set(control) & set(plus) & set(minus)))
    if not indices:
        raise RuntimeError("signed response has no shared radial cells")
    result = {"odd": [], "even": [], "ordinary": [], "control": []}
    cosine_name, sine_name = pair
    for index in indices:
        zero = complex(control[index][cosine_name], control[index][sine_name])
        positive = complex(plus[index][cosine_name], plus[index][sine_name])
        negative = complex(minus[index][cosine_name], minus[index][sine_name])
        result["odd"].append((positive - negative) / (2.0 * epsilon))
        result["even"].append(
            (positive + negative - 2.0 * zero) / (2.0 * epsilon)
        )
        result["ordinary"].append((positive - zero) / epsilon)
        result["control"].append(zero)
    return indices, result


def build_mode_response(
    mode: int,
    secondary_mode: int,
    epsilon: float,
    control: LogData,
    plus: LogData,
    minus: LogData,
) -> List[ModeResponse]:
    primary_times = common_times(
        control.radial[mode], plus.radial[mode], minus.radial[mode]
    )
    secondary_times = set(
        common_times(
            control.radial[secondary_mode],
            plus.radial[secondary_mode],
            minus.radial[secondary_mode],
        )
    )
    responses: List[ModeResponse] = []
    for time in primary_times:
        zero = control.radial[mode][time]
        positive = plus.radial[mode][time]
        negative = minus.radial[mode][time]
        field_responses: Dict[str, FieldResponse] = {}
        x_values: Tuple[float, ...] = ()
        q_odd: Sequence[complex] = ()
        for field, pair in FIELD_PAIRS.items():
            indices, combined = vectors(
                zero, positive, negative, pair, epsilon
            )
            if not x_values:
                x_values = tuple(zero[index]["x"] for index in indices)
            odd = rms(combined["odd"])
            even = rms(combined["even"])
            ordinary = rms(combined["ordinary"])
            control_amplitude = rms(combined["control"])
            anchor = max(
                range(len(combined["odd"])),
                key=lambda index: abs(combined["odd"][index]),
            )
            field_responses[field] = FieldResponse(
                odd=odd,
                even=even,
                ordinary=ordinary,
                control=control_amplitude,
                phase=cmath.phase(combined["odd"][anchor]),
                contamination=even / odd if odd > 0.0 else math.inf,
                seeded_to_leakage=(
                    epsilon * ordinary / max(control_amplitude, 1.0e-300)
                ),
                profile=normalized_profile(combined["odd"]),
            )
            if field == "q":
                q_odd = combined["odd"]

        constraint_response: Dict[str, float] = {}
        for name, pair in CONSTRAINT_PAIRS.items():
            _, combined = vectors(zero, positive, negative, pair, epsilon)
            constraint_response[name] = rms(combined["odd"])

        q_amplitude = field_responses["q"].odd
        alternating = abs(
            sum(
                (1.0 if index % 2 == 0 else -1.0) * value
                for index, value in enumerate(q_odd)
            )
        ) / len(q_odd)
        nyquist_fraction = (
            alternating / q_amplitude if q_amplitude > 0.0 else math.inf
        )

        spurious_fraction = math.nan
        if time in secondary_times:
            secondary_zero = control.radial[secondary_mode][time]
            secondary_plus = plus.radial[secondary_mode][time]
            secondary_minus = minus.radial[secondary_mode][time]
            _, secondary = vectors(
                secondary_zero,
                secondary_plus,
                secondary_minus,
                FIELD_PAIRS["q"],
                epsilon,
            )
            secondary_amplitude = rms(secondary["odd"])
            spurious_fraction = (
                secondary_amplitude / q_amplitude
                if q_amplitude > 0.0
                else math.inf
            )

        responses.append(
            ModeResponse(
                time=time,
                x=x_values,
                fields=field_responses,
                constraints=constraint_response,
                radial_nyquist_fraction=nyquist_fraction,
                spurious_harmonic_fraction=spurious_fraction,
            )
        )
    return responses


@dataclass
class RollingFit:
    field: str
    width: float
    start: float
    end: float
    points: int
    slope: float
    standard_error: float
    r_squared: float
    maximum_contamination: float
    minimum_leakage_ratio: float


def fit_window(
    responses: Sequence[ModeResponse],
    field: str,
    start: float,
    end: float,
) -> RollingFit:
    selected = [
        response
        for response in responses
        if start - 1.0e-12 <= response.time <= end + 1.0e-12
        and response.fields[field].odd > 0.0
    ]
    if len(selected) < 4:
        raise RuntimeError("D12 rolling fit has fewer than four samples")
    times = [sample.time for sample in selected]
    values = [math.log(sample.fields[field].odd) for sample in selected]
    mean_time = sum(times) / len(times)
    mean_value = sum(values) / len(values)
    centered = sum((time - mean_time) ** 2 for time in times)
    if centered <= 0.0:
        raise RuntimeError("D12 rolling fit has singular times")
    slope = sum(
        (time - mean_time) * (value - mean_value)
        for time, value in zip(times, values)
    ) / centered
    intercept = mean_value - slope * mean_time
    residual = sum(
        (value - intercept - slope * time) ** 2
        for time, value in zip(times, values)
    )
    total = sum((value - mean_value) ** 2 for value in values)
    standard_error = math.sqrt(
        residual / ((len(selected) - 2) * centered)
    )
    return RollingFit(
        field=field,
        width=end - start,
        start=start,
        end=end,
        points=len(selected),
        slope=slope,
        standard_error=standard_error,
        r_squared=1.0 - residual / total if total > 0.0 else 1.0,
        maximum_contamination=max(
            sample.fields[field].contamination for sample in selected
        ),
        minimum_leakage_ratio=min(
            sample.fields[field].seeded_to_leakage for sample in selected
        ),
    )


def rolling_fits(
    responses: Sequence[ModeResponse], field: str, width: float
) -> List[RollingFit]:
    fits: List[RollingFit] = []
    end = width
    while end <= responses[-1].time + 1.0e-12:
        fits.append(fit_window(responses, field, end - width, end))
        end += ROLLING_STEP
    return fits


def response_near(
    responses: Sequence[ModeResponse], time: float
) -> ModeResponse:
    return min(responses, key=lambda response: abs(response.time - time))


@dataclass
class Plateau:
    sign: int
    width: float
    fits: Tuple[RollingFit, RollingFit, RollingFit]
    minimum_profile_overlap: float
    field_slope_compatible: bool
    constraints_controlled: bool


def find_plateau(
    responses: Sequence[ModeResponse],
    fits_by_field: Mapping[str, Mapping[float, Sequence[RollingFit]]],
) -> Plateau | None:
    candidates: Dict[float, Plateau] = {}
    for width in ROLLING_WIDTHS:
        q_fits = fits_by_field["q"][width]
        for first in range(len(q_fits) - 3, -1, -1):
            group = tuple(q_fits[first : first + 3])
            if len(group) != 3 or group[0].end < 0.5 * responses[-1].time:
                continue
            signs = tuple(1 if fit.slope > 0.0 else -1 for fit in group)
            if len(set(signs)) != 1:
                continue
            mean_slope = sum(fit.slope for fit in group) / 3.0
            variation = (
                max(fit.slope for fit in group)
                - min(fit.slope for fit in group)
            ) / max(abs(mean_slope), 1.0e-300)
            if variation >= SLOPE_VARIATION_LIMIT:
                continue
            if any(
                fit.standard_error >= abs(fit.slope)
                or fit.maximum_contamination >= CONTAMINATION_LIMIT
                or fit.minimum_leakage_ratio <= LEAKAGE_RATIO_MINIMUM
                for fit in group
            ):
                continue

            selected_responses = [
                response_near(responses, fit.end) for fit in group
            ]
            overlaps = [
                profile_overlap(
                    selected_responses[index].fields["q"].profile,
                    selected_responses[index + 1].fields["q"].profile,
                )
                for index in range(2)
            ]
            minimum_overlap = min(overlaps)
            if minimum_overlap <= PROFILE_OVERLAP_LIMIT:
                continue

            field_compatible = True
            for field in ("ratio", "K", "Aww", "GammaX"):
                field_fits = fits_by_field[field][width]
                for offset, q_fit in enumerate(group):
                    field_fit = field_fits[first + offset]
                    scale = max(abs(q_fit.slope), abs(field_fit.slope), 1.0e-12)
                    if (
                        field_fit.slope * q_fit.slope <= 0.0
                        or field_fit.standard_error >= abs(field_fit.slope)
                        or abs(field_fit.slope - q_fit.slope)
                        > FIELD_SLOPE_TOLERANCE * scale
                    ):
                        field_compatible = False
            if not field_compatible:
                continue

            constraint_fits: List[float] = []
            for name in CONSTRAINT_PAIRS:
                synthetic = [
                    ModeResponse(
                        time=response.time,
                        x=response.x,
                        fields={
                            name: FieldResponse(
                                odd=response.constraints[name],
                                even=0.0,
                                ordinary=0.0,
                                control=0.0,
                                phase=0.0,
                                contamination=0.0,
                                seeded_to_leakage=math.inf,
                                profile=(),
                            )
                        },
                        constraints={},
                        radial_nyquist_fraction=0.0,
                    )
                    for response in responses
                ]
                constraint_fits.append(
                    fit_window(
                        synthetic, name, group[-1].start, group[-1].end
                    ).slope
                )
            constraints_controlled = not any(
                slope > max(CONSTRAINT_RAPID_SLOPE,
                            group[-1].slope + CONSTRAINT_RAPID_SLOPE)
                for slope in constraint_fits
            )
            if not constraints_controlled:
                continue
            candidates[width] = Plateau(
                signs[0],
                width,
                group,
                minimum_overlap,
                field_compatible,
                constraints_controlled,
            )
            break
    if set(candidates) != set(ROLLING_WIDTHS):
        return None
    if candidates[0.5].sign != candidates[1.0].sign:
        return None
    return candidates[1.0]


def background_gate(label: str, control: LogData) -> bool:
    times = sorted(control.background)
    final = control.background[times[-1]]
    crossing = next(
        (
            time
            for time in times
            if float(control.background[time]["state_max"]) >= 0.1
        ),
        None,
    )
    nyquist_fraction = float(final["maximum_radial_nyquist"]) / max(
        float(final["state_l2"]), 1.0e-300
    )
    passed = (
        not control.invalid_metric
        and crossing is None
        and times[-1] >= 8.0 - 1.0e-12
        and float(final["determinant_max"]) < 1.0e-12
        and float(final["weighted_trace_max"]) < 1.0e-12
        and float(final["maximum_radial_nyquist"]) < 1.0e-2
    )
    print(
        "D12_BACKGROUND"
        f" resolution={label} final_time={times[-1]:.12e}"
        f" state_max={float(final['state_max']):.12e}"
        f" state_l2={float(final['state_l2']):.12e}"
        f" drift_0.1_crossing={'NONE' if crossing is None else f'{crossing:.12e}'}"
        f" radial_nyquist={float(final['maximum_radial_nyquist']):.12e}"
        f" radial_nyquist_fraction={nyquist_fraction:.12e}"
        f" determinant_max={float(final['determinant_max']):.12e}"
        f" weighted_trace_max={float(final['weighted_trace_max']):.12e}"
        f" invalid_metric={int(control.invalid_metric)}"
        f" result={'PASS' if passed else 'FAIL'}"
    )
    return passed


def selected_history(
    responses: Sequence[ModeResponse], interval: float = 0.25
) -> Iterable[ModeResponse]:
    target = 0.0
    while target <= responses[-1].time + 1.0e-12:
        yield response_near(responses, target)
        target += interval


@dataclass
class ModeResult:
    interpretable: bool
    plateau: Plateau | None
    responses: Sequence[ModeResponse]


def analyze_mode(
    resolution: str,
    label: str,
    mode: int,
    control: LogData,
    plus: LogData,
    minus: LogData,
) -> ModeResult:
    secondary_mode = 2 if mode == 1 else 1
    responses = build_mode_response(
        mode, secondary_mode, 1.0e-8, control, plus, minus
    )
    fits_by_field = {
        field: {
            width: rolling_fits(responses, field, width)
            for width in ROLLING_WIDTHS
        }
        for field in FIELD_PAIRS
    }
    plateau = find_plateau(responses, fits_by_field)
    late = [response for response in responses if response.time >= 6.0]
    interpretable = bool(late) and all(
        math.isfinite(response.fields["q"].odd)
        and response.fields["q"].odd > 0.0
        and response.fields["q"].contamination < CONTAMINATION_LIMIT
        and response.fields["q"].seeded_to_leakage > LEAKAGE_RATIO_MINIMUM
        for response in late
    )

    for response in selected_history(responses):
        q = response.fields["q"]
        print(
            "D12_RESPONSE"
            f" resolution={resolution} mode={label} t={response.time:.12e}"
            f" q_odd={q.odd:.12e} q_even={q.even:.12e}"
            f" even_to_odd={q.contamination:.12e}"
            f" ordinary={q.ordinary:.12e} phase={q.phase:.12e}"
            f" seeded_to_control_leakage={q.seeded_to_leakage:.12e}"
            f" control_subtracted_drift={1.0e-8 * q.ordinary:.12e}"
            f" H_odd={response.constraints['H']:.12e}"
            f" Mx_odd={response.constraints['Mx']:.12e}"
            f" Mz_odd={response.constraints['Mz']:.12e}"
            f" radial_nyquist_fraction="
            f"{response.radial_nyquist_fraction:.12e}"
            f" spurious_harmonic_fraction="
            f"{response.spurious_harmonic_fraction:.12e}"
        )
        for field in ("ratio", "K", "Aww", "GammaX"):
            measured = response.fields[field]
            print(
                "D12_FIELD"
                f" resolution={resolution} mode={label}"
                f" t={response.time:.12e} field={field}"
                f" odd_amplitude={measured.odd:.12e}"
                f" even_to_odd={measured.contamination:.12e}"
                f" phase={measured.phase:.12e}"
                f" seeded_to_control_leakage="
                f"{measured.seeded_to_leakage:.12e}"
            )

    for field in FIELD_PAIRS:
        for width in ROLLING_WIDTHS:
            for fit in fits_by_field[field][width]:
                print(
                    "D12_ROLLING"
                    f" resolution={resolution} mode={label}"
                    f" field={field} width={width:.2f}"
                    f" start={fit.start:.12e} end={fit.end:.12e}"
                    f" points={fit.points} slope={fit.slope:.12e}"
                    f" standard_error={fit.standard_error:.12e}"
                    f" R2={fit.r_squared:.12e}"
                    f" maximum_even_to_odd="
                    f"{fit.maximum_contamination:.12e}"
                    f" minimum_seeded_to_leakage="
                    f"{fit.minimum_leakage_ratio:.12e}"
                )

    if plateau is not None:
        accepted = [
            response_near(responses, fit.end) for fit in plateau.fits
        ]
        final_profile = accepted[-1].fields["q"].profile
        maximum_index = max(
            range(len(final_profile)),
            key=lambda index: abs(final_profile[index]),
        )
        print(
            "D12_PLATEAU"
            f" resolution={resolution} mode={label}"
            f" sign={plateau.sign} width={plateau.width:.2f}"
            f" start={plateau.fits[0].start:.12e}"
            f" end={plateau.fits[-1].end:.12e}"
            f" minimum_profile_overlap="
            f"{plateau.minimum_profile_overlap:.12e}"
            f" field_slopes_compatible={int(plateau.field_slope_compatible)}"
            f" constraints_controlled={int(plateau.constraints_controlled)}"
            f" maximum_x={accepted[-1].x[maximum_index]:.12e}"
        )
        for response in accepted:
            for x, value in zip(
                response.x, response.fields["q"].profile
            ):
                print(
                    "D12_EIGENFUNCTION"
                    f" resolution={resolution} mode={label}"
                    f" t={response.time:.12e} x={x:.12e}"
                    f" normalized_real={value.real:.12e}"
                    f" normalized_imag={value.imag:.12e}"
                    f" normalized_magnitude={abs(value):.12e}"
                )
    print(
        "D12_MODE_RESULT"
        f" resolution={resolution} mode={label}"
        f" interpretable={int(interpretable)}"
        f" plateau_sign={0 if plateau is None else plateau.sign}"
        f" final_q_odd={responses[-1].fields['q'].odd:.12e}"
        f" final_even_to_odd="
        f"{responses[-1].fields['q'].contamination:.12e}"
        f" final_seeded_to_leakage="
        f"{responses[-1].fields['q'].seeded_to_leakage:.12e}"
        f" final_radial_nyquist_fraction="
        f"{responses[-1].radial_nyquist_fraction:.12e}"
        f" final_spurious_harmonic_fraction="
        f"{responses[-1].spurious_harmonic_fraction:.12e}"
    )
    return ModeResult(interpretable, plateau, responses)


def analyze_resolution(
    label: str,
    paths: Sequence[Path],
) -> Tuple[bool, Dict[str, ModeResult]]:
    control, low_plus, low_minus, high_plus, high_minus = map(
        read_log, paths
    )
    background = background_gate(label, control)
    results = {
        "k_pi_over_4": analyze_mode(
            label,
            "k_pi_over_4",
            1,
            control,
            low_plus,
            low_minus,
        ),
        "k_pi_over_2": analyze_mode(
            label,
            "k_pi_over_2",
            2,
            control,
            high_plus,
            high_minus,
        ),
    }
    both_interpretable = background and all(
        result.interpretable for result in results.values()
    )
    print(
        "D12_RESOLUTION_INTERPRETABLE"
        f" resolution={label} value={int(both_interpretable)}"
    )
    return background, results


def main() -> None:
    parser = argparse.ArgumentParser()
    for prefix in ("medium", "fine"):
        required = prefix == "medium"
        parser.add_argument(f"--{prefix}-control", type=Path, required=required)
        parser.add_argument(f"--{prefix}-low-plus", type=Path, required=required)
        parser.add_argument(f"--{prefix}-low-minus", type=Path, required=required)
        parser.add_argument(f"--{prefix}-high-plus", type=Path, required=required)
        parser.add_argument(f"--{prefix}-high-minus", type=Path, required=required)
    arguments = parser.parse_args()

    medium_paths = [
        arguments.medium_control,
        arguments.medium_low_plus,
        arguments.medium_low_minus,
        arguments.medium_high_plus,
        arguments.medium_high_minus,
    ]
    medium_background, medium = analyze_resolution("medium", medium_paths)
    backgrounds = [medium_background]
    resolutions = [medium]

    fine_paths = [
        arguments.fine_control,
        arguments.fine_low_plus,
        arguments.fine_low_minus,
        arguments.fine_high_plus,
        arguments.fine_high_minus,
    ]
    if all(path is not None for path in fine_paths):
        fine_background, fine = analyze_resolution(
            "fine", [path for path in fine_paths if path is not None]
        )
        backgrounds.append(fine_background)
        resolutions.append(fine)

    if not all(backgrounds):
        classification = "BACKGROUND_STABILITY_REGRESSED"
    else:
        low_signs = [
            0 if result["k_pi_over_4"].plateau is None
            else result["k_pi_over_4"].plateau.sign
            for result in resolutions
        ]
        high_signs = [
            0 if result["k_pi_over_2"].plateau is None
            else result["k_pi_over_2"].plateau.sign
            for result in resolutions
        ]
        if all(sign == 1 for sign in low_signs) and all(
            sign == -1 for sign in high_signs
        ):
            classification = (
                "KO_STABILIZED_GROWING_AND_DECAYING_MODES_IDENTIFIED"
            )
        elif all(sign == 1 for sign in low_signs) and all(
            sign in (0, -1) for sign in high_signs
        ):
            classification = (
                "KO_STABILIZED_GROWING_MODE_IDENTIFIED"
                " — STABLE ENDPOINT UNRESOLVED"
            )
        else:
            classification = "NO_CREDIBLE_MODE_PLATEAU"
    print(f"D12_CLASSIFICATION {classification}")


if __name__ == "__main__":
    main()
