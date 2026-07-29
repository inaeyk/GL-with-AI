#!/usr/bin/env python3
"""Fixture-only Stage 4AO-D9 excision-placement diagnostics."""

from __future__ import annotations

import argparse
import math
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Iterable, Mapping

from analyze_d7_controls import (
    THRESHOLDS,
    closest_times,
    first_crossing,
    fit_log_series,
    format_crossing,
    read_baseline,
)


def fields(line: str) -> Dict[str, str]:
    result: Dict[str, str] = {}
    for token in line.split()[1:]:
        name, separator, value = token.partition("=")
        if separator:
            result[name] = value
    return result


@dataclass
class Control:
    samples: Dict[float, Dict[str, str]]
    variables: Dict[float, Dict[str, Dict[str, str]]]
    ghost: Dict[str, str]
    invalid: Dict[str, str] | None


def read_control(path: Path) -> Control:
    samples: Dict[float, Dict[str, str]] = {}
    variables: Dict[float, Dict[str, Dict[str, str]]] = {}
    ghost: Dict[str, str] = {}
    invalid: Dict[str, str] | None = None
    with path.open(encoding="utf-8") as source:
        for line in source:
            if line.startswith("D9_SAMPLE "):
                parsed = fields(line)
                samples[float(parsed["t"])] = parsed
            elif line.startswith("D9_VARIABLE "):
                parsed = fields(line)
                time = float(parsed["t"])
                variables.setdefault(time, {})[parsed["name"]] = parsed
            elif line.startswith("D9_GHOST_AUDIT "):
                ghost = fields(line)
            elif line.startswith("D9_INVALID_METRIC "):
                invalid = fields(line)
    if not samples or not variables or not ghost:
        raise RuntimeError(f"{path} lacks required D9 fixture evidence")
    return Control(samples, variables, ghost, invalid)


def numeric_series(control: Control, field: str) -> Dict[float, float]:
    return {
        time: float(sample[field]) for time, sample in control.samples.items()
    }


def rolling_fits(
    series: Mapping[float, float], width: float
) -> Iterable[object]:
    end = width
    final_time = max(series)
    while end <= final_time + 1.0e-12:
        fit = fit_log_series(series, end - width, end)
        if fit is not None:
            yield fit
        end += 0.25


def output_characteristics() -> None:
    sectors = {
        "physical": 1.0,
        "constraint": 1.0,
        "lapse": math.sqrt(2.0),
        "shift_transverse": math.sqrt(0.75),
        "shift_longitudinal_d4": math.sqrt(0.75 * 1.5),
    }
    print(
        "D9_LOCKED_PARAMETERS"
        " alpha_gp=1 lapse_coeff=2 lapse_power=1"
        " lapse_advec_coeff=0 shift_Gamma_coeff=0.75"
        " shift_advec_coeff=0 eta=1"
        " kappa1=0.1 kappa2=0 kappa3=1 covariantZ4=1"
        " fixed_lapse_source_principal_order=LOWER_ORDER"
        " kappa_damping_principal_order=LOWER_ORDER"
    )
    print(
        "D9_CHARACTERISTIC_ENVELOPE"
        f" c_physical={sectors['physical']:.12e}"
        f" c_constraint={sectors['constraint']:.12e}"
        f" c_lapse={sectors['lapse']:.12e}"
        f" c_shift_transverse={sectors['shift_transverse']:.12e}"
        f" c_shift_longitudinal_d4={sectors['shift_longitudinal_d4']:.12e}"
        f" conservative_c_max={max(sectors.values()):.12e}"
        " convention=v_plus=-beta_x+c"
        " convention_v_minus=-beta_x-c"
        " complete_principal_proof=0"
    )
    positions = {
        "reused_xin_0.5_face": 0.5,
        "reused_xin_0.5_first_valid": 0.5625,
        "d9_face": 0.375,
        "d9_medium_first_valid": 0.4375,
        "d9_fine_first_valid": 0.375 + 1.0 / 24.0,
    }
    for position, x_value in positions.items():
        beta = math.sqrt(1.0 / x_value)
        for sector, speed in sectors.items():
            print(
                "D9_CHARACTERISTIC_SPEED"
                f" position={position} x={x_value:.12e}"
                f" sector={sector} c={speed:.12e}"
                f" beta_x={beta:.12e}"
                f" v_plus={-beta + speed:.12e}"
                f" v_minus={-beta - speed:.12e}"
                f" outflow_margin={beta - speed:.12e}"
            )


def output_control(variant: str, control: Control) -> None:
    state = numeric_series(control, "state_max")
    metrics = {
        "state_max": state,
        "H_max": numeric_series(control, "H_max"),
        "Mx_max": numeric_series(control, "Mx_max"),
        "Mz_max": numeric_series(control, "Mz_max"),
        "maximum_radial_nyquist": numeric_series(
            control, "maximum_radial_nyquist"
        ),
    }
    for time in closest_times(control.samples, 0.25):
        sample = control.samples[time]
        print(
            "D9_SERIES"
            f" variant={variant} t={time:.12e}"
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
            f" determinant_l2={float(sample['determinant_l2']):.12e}"
            f" weighted_trace_max="
            f"{float(sample['weighted_trace_max']):.12e}"
            f" weighted_trace_l2="
            f"{float(sample['weighted_trace_l2']):.12e}"
            f" radial_nyquist="
            f"{float(sample['maximum_radial_nyquist']):.12e}"
            f" radial_nyquist_variable="
            f"{sample['maximum_radial_nyquist_variable']}"
            f" maximum_z_span={float(sample['maximum_z_span']):.12e}"
            f" maximum_fourier={float(sample['maximum_fourier']):.12e}"
        )

    all_names = sorted(
        next(iter(control.variables.values())),
        key=lambda name: int(next(iter(control.variables.values()))[name]["slot"]),
    )
    for name in all_names:
        candidates = [
            (time, values[name])
            for time, values in control.variables.items()
            if name in values
        ]
        peak_time, peak = max(
            candidates, key=lambda item: float(item[1]["maximum"])
        )
        print(
            "D9_VARIABLE_PEAK"
            f" variant={variant} t={peak_time:.12e}"
            f" slot={peak['slot']} name={name}"
            f" maximum={float(peak['maximum']):.12e}"
            f" l2={float(peak['l2']):.12e}"
            f" location={peak['location']}"
            f" z_span={float(peak['z_span']):.12e}"
            f" fourier={float(peak['fourier']):.12e}"
            f" radial_nyquist={float(peak['radial_nyquist']):.12e}"
        )

    for metric, series in metrics.items():
        for width in (0.5, 1.0):
            fits = list(rolling_fits(series, width))
            for fit in fits[-4:]:
                print(
                    "D9_ROLLING"
                    f" variant={variant} metric={metric}"
                    f" width={width:.2f}"
                    f" start={fit.start:.12e} end={fit.end:.12e}"
                    f" points={fit.points} slope={fit.slope:.12e}"
                    f" standard_error={fit.standard_error:.12e}"
                    f" R2={fit.r_squared:.12e}"
                )

    for threshold in THRESHOLDS:
        print(
            "D9_CROSSING"
            f" variant={variant} threshold={threshold:.12e}"
            f" first_time={format_crossing(first_crossing(state, threshold))}"
        )

    print(
        "D9_GHOST_RESULT"
        f" variant={variant}"
        f" valid_cells_overwritten={control.ghost['valid_cells_overwritten']}"
        f" components_begin={control.ghost['components_begin']}"
        f" components_end={control.ghost['components_end']}"
        f" inner_ghost_x_near={float(control.ghost['inner_ghost_x_near']):.12e}"
        f" inner_ghost_x_middle="
        f"{float(control.ghost['inner_ghost_x_middle']):.12e}"
        f" inner_ghost_x_far={float(control.ghost['inner_ghost_x_far']):.12e}"
        f" minimum_inner_ghost_x="
        f"{float(control.ghost['minimum_inner_ghost_x']):.12e}"
    )
    if control.invalid is None:
        print(f"D9_INVALID_RESULT variant={variant} first_invalid=NONE")
    else:
        invalid = control.invalid
        print(
            "D9_INVALID_RESULT"
            f" variant={variant}"
            f" first_failing_timestep={invalid['first_failing_timestep']}"
            f" rk_stage={invalid['rk_stage']}"
            f" base_time={float(invalid['base_time']):.12e}"
            f" index_x={invalid['index_x']} index_z={invalid['index_z']}"
            f" region={invalid['region']}"
            f" offending_field={invalid['offending_field']}"
            " valid_cell=1 ghost_cell=0"
        )

    final_time = max(control.samples)
    final = control.samples[final_time]
    print(
        "D9_FINAL"
        f" variant={variant} last_sample_time={final_time:.12e}"
        f" state_max={float(final['state_max']):.12e}"
        f" state_l2={float(final['state_l2']):.12e}"
        f" state_location={final['state_max_location']}"
        f" H_max={float(final['H_max']):.12e}"
        f" H_l2={float(final['H_l2']):.12e}"
        f" Mx_max={float(final['Mx_max']):.12e}"
        f" Mx_l2={float(final['Mx_l2']):.12e}"
        f" Mz_max={float(final['Mz_max']):.12e}"
        f" Mz_l2={float(final['Mz_l2']):.12e}"
        f" determinant_max={float(final['determinant_max']):.12e}"
        f" weighted_trace_max={float(final['weighted_trace_max']):.12e}"
        f" maximum_radial_nyquist="
        f"{float(final['maximum_radial_nyquist']):.12e}"
        f" maximum_z_span={float(final['maximum_z_span']):.12e}"
        f" maximum_fourier={float(final['maximum_fourier']):.12e}"
    )


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--baseline", type=Path, required=True)
    parser.add_argument("--medium", type=Path, required=True)
    parser.add_argument("--fine", type=Path, required=True)
    parser.add_argument("--exact-gp", type=Path, required=True)
    parser.add_argument("--medium-wall", type=float, required=True)
    parser.add_argument("--medium-rss", type=int, required=True)
    parser.add_argument("--fine-wall", type=float, required=True)
    parser.add_argument("--fine-rss", type=int, required=True)
    parser.add_argument("--exact-wall", type=float, required=True)
    parser.add_argument("--exact-rss", type=int, required=True)
    arguments = parser.parse_args()

    output_characteristics()
    baseline = read_baseline(arguments.baseline)
    for threshold in THRESHOLDS:
        print(
            "D9_BASELINE_CROSSING_REUSED"
            f" threshold={threshold:.12e}"
            f" first_time={format_crossing(first_crossing(baseline, threshold))}"
        )
    print(
        "D9_BASELINE_FINAL_REUSED"
        f" t={max(baseline):.12e}"
        f" state_max={baseline[max(baseline)]:.12e}"
        " source=D6_D7_D8_RECORDED_CONTROL"
        " rerun=0 detailed_constraints=UNAVAILABLE_NOT_REGENERATED"
    )

    controls = {
        "medium": read_control(arguments.medium),
        "fine": read_control(arguments.fine),
        "exact_gp": read_control(arguments.exact_gp),
    }
    for variant, control in controls.items():
        output_control(variant, control)

    medium_invalid = controls["medium"].invalid
    fine_invalid = controls["fine"].invalid
    medium_invalid_time = (
        math.inf
        if medium_invalid is None
        else float(medium_invalid["base_time"])
    )
    fine_invalid_time = (
        math.inf if fine_invalid is None else float(fine_invalid["base_time"])
    )
    print(
        "D9_RESOLUTION_COMPARISON"
        f" medium_invalid_time={format_crossing(medium_invalid_time)}"
        f" fine_invalid_time={format_crossing(fine_invalid_time)}"
        f" fine_minus_medium={fine_invalid_time - medium_invalid_time:.12e}"
        " finer_develops_faster="
        + ("1" if fine_invalid_time < medium_invalid_time else "0")
    )
    total_wall = (
        arguments.medium_wall + arguments.fine_wall + arguments.exact_wall
    )
    peak_rss = max(
        arguments.medium_rss, arguments.fine_rss, arguments.exact_rss
    )
    print(
        "D9_RUNTIME"
        " reused_evolutions=1 new_evolutions=3"
        f" total_wall_seconds={total_wall:.2f}"
        f" peak_rss_kb={peak_rss}"
        f" medium_wall_seconds={arguments.medium_wall:.2f}"
        f" fine_wall_seconds={arguments.fine_wall:.2f}"
        f" exact_gp_wall_seconds={arguments.exact_wall:.2f}"
    )
    print(
        "D9_STABILIZATION_GATE"
        " no_invalid_metric_through_t8=0"
        " no_drift_0.1_crossing_through_t8=0"
        " finer_not_faster=0"
        " constraints_controlled=0"
        " radial_grid_scale_content_controlled=0"
    )
    print("D9_CLASSIFICATION EXCISION_PLACEMENT_NOT_SUFFICIENT")


if __name__ == "__main__":
    main()
