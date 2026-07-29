#!/usr/bin/env python3
"""Fixture-only Stage 4AO-D11 KO background-control diagnostics."""

from __future__ import annotations

import argparse
import math
from pathlib import Path
from typing import Dict, Mapping

from analyze_d7_controls import (
    THRESHOLDS,
    first_crossing,
    fit_log_series,
    format_crossing,
    read_baseline,
)
from analyze_d9_controls import Control, numeric_series, read_control


def rolling_fits(series: Mapping[float, float], width: float) -> list[object]:
    result: list[object] = []
    end = width
    final_time = max(series)
    while end <= final_time + 1.0e-12:
        fit = fit_log_series(series, end - width, end)
        if fit is not None:
            result.append(fit)
        end += 0.25
    return result


def output_control(variant: str, sigma: float, control: Control) -> Dict[str, object]:
    state = numeric_series(control, "state_max")
    nyquist = numeric_series(control, "maximum_radial_nyquist")
    final_time = max(control.samples)
    final = control.samples[final_time]

    for threshold in THRESHOLDS:
        print(
            "D11_CROSSING"
            f" variant={variant} sigma={sigma:.12e}"
            f" threshold={threshold:.12e}"
            f" first_time={format_crossing(first_crossing(state, threshold))}"
        )

    names = sorted(
        next(iter(control.variables.values())),
        key=lambda name: int(next(iter(control.variables.values()))[name]["slot"]),
    )
    for name in names:
        peak_time, peak = max(
            (
                (time, values[name])
                for time, values in control.variables.items()
                if name in values
            ),
            key=lambda item: float(item[1]["maximum"]),
        )
        print(
            "D11_VARIABLE_PEAK"
            f" variant={variant} sigma={sigma:.12e}"
            f" t={peak_time:.12e} slot={peak['slot']} name={name}"
            f" maximum={float(peak['maximum']):.12e}"
            f" l2={float(peak['l2']):.12e}"
            f" location={peak['location']}"
            f" z_span={float(peak['z_span']):.12e}"
            f" fourier={float(peak['fourier']):.12e}"
            f" radial_nyquist={float(peak['radial_nyquist']):.12e}"
        )

    state_fits = rolling_fits(state, 1.0)
    nyquist_fits = rolling_fits(nyquist, 1.0)
    for metric, fits in (("state", state_fits), ("radial_nyquist", nyquist_fits)):
        for fit in fits[-4:]:
            print(
                "D11_ROLLING"
                f" variant={variant} metric={metric}"
                f" width=1.00 start={fit.start:.12e} end={fit.end:.12e}"
                f" slope={fit.slope:.12e}"
                f" standard_error={fit.standard_error:.12e}"
                f" R2={fit.r_squared:.12e}"
            )

    invalid_time = (
        None if control.invalid is None else float(control.invalid["base_time"])
    )
    nyquist_fraction = float(final["maximum_radial_nyquist"]) / max(
        float(final["state_l2"]), 1.0e-300
    )
    print(
        "D11_FINAL"
        f" variant={variant} sigma={sigma:.12e}"
        f" last_sample_time={final_time:.12e}"
        f" state_max={float(final['state_max']):.12e}"
        f" state_l2={float(final['state_l2']):.12e}"
        f" state_variable={final['state_max_variable']}"
        f" state_location={final['state_max_location']}"
        f" H_max={float(final['H_max']):.12e}"
        f" H_l2={float(final['H_l2']):.12e}"
        f" Mx_max={float(final['Mx_max']):.12e}"
        f" Mx_l2={float(final['Mx_l2']):.12e}"
        f" Mz_max={float(final['Mz_max']):.12e}"
        f" Mz_l2={float(final['Mz_l2']):.12e}"
        f" determinant_max={float(final['determinant_max']):.12e}"
        f" determinant_l2={float(final['determinant_l2']):.12e}"
        f" weighted_trace_max={float(final['weighted_trace_max']):.12e}"
        f" weighted_trace_l2={float(final['weighted_trace_l2']):.12e}"
        f" radial_nyquist={float(final['maximum_radial_nyquist']):.12e}"
        f" radial_nyquist_fraction_of_state_l2={nyquist_fraction:.12e}"
        f" maximum_z_span={float(final['maximum_z_span']):.12e}"
        f" maximum_fourier={float(final['maximum_fourier']):.12e}"
        f" first_invalid_metric_time={format_crossing(invalid_time)}"
        " first_invalid_metric_cell="
        + (
            "NONE"
            if control.invalid is None
            else (
                f"({control.invalid['index_x']},{control.invalid['index_z']})"
                f":{control.invalid['region']}"
                f":{control.invalid['offending_field']}"
            )
        )
    )
    return {
        "final_time": final_time,
        "state": state,
        "nyquist": nyquist,
        "invalid": control.invalid,
        "constraints_bounded": max(
            float(final["H_max"]),
            float(final["Mx_max"]),
            float(final["Mz_max"]),
        )
        < 1.0,
    }


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--baseline", type=Path, required=True)
    parser.add_argument("--ref", type=Path, required=True)
    parser.add_argument("--double", type=Path)
    parser.add_argument("--fine", type=Path, required=True)
    parser.add_argument("--best-sigma", type=float, required=True)
    parser.add_argument("--ref-wall", type=float, required=True)
    parser.add_argument("--ref-rss", type=int, required=True)
    parser.add_argument("--double-wall", type=float, default=0.0)
    parser.add_argument("--double-rss", type=int, default=0)
    parser.add_argument("--fine-wall", type=float, required=True)
    parser.add_argument("--fine-rss", type=int, required=True)
    arguments = parser.parse_args()

    baseline = read_baseline(arguments.baseline)
    print(
        "D11_SIGMA_PROVENANCE"
        " sigma_ref=3.000000000000e-01"
        " source=locked_GRChombo_KerrBH_params_cheap_and_BinaryBH_params_very_cheap"
        " upstream_default=1.000000000000e-01"
        " selected_conventional_example_value=3.000000000000e-01"
    )
    print(
        "D11_BASELINE_REUSED"
        f" t={max(baseline):.12e}"
        f" state_max={baseline[max(baseline)]:.12e}"
        " sigma=0 rerun=0 source=D6_D7_RECORDED_CONTROL"
        " detailed_variables=UNAVAILABLE_NOT_REGENERATED"
        " detailed_constraints=UNAVAILABLE_NOT_REGENERATED"
    )

    controls = {
        "sigma_ref": (0.3, read_control(arguments.ref)),
        "fine_best": (arguments.best_sigma, read_control(arguments.fine)),
    }
    if arguments.double is not None:
        controls["double_sigma_ref"] = (0.6, read_control(arguments.double))

    results = {
        variant: output_control(variant, selected_sigma, control)
        for variant, (selected_sigma, control) in controls.items()
    }
    medium_variant = (
        "double_sigma_ref"
        if math.isclose(arguments.best_sigma, 0.6)
        else "sigma_ref"
    )
    medium = results[medium_variant]
    fine = results["fine_best"]

    medium_crossing = first_crossing(medium["state"], 1.0e-1)
    fine_crossing = first_crossing(fine["state"], 1.0e-1)
    medium_invalid = medium["invalid"] is not None
    fine_invalid = fine["invalid"] is not None
    no_crossing = medium_crossing is None and fine_crossing is None
    no_invalid = not medium_invalid and not fine_invalid
    fine_not_faster = not (
        fine_crossing is not None
        and (medium_crossing is None or fine_crossing < medium_crossing)
    )
    nyquist_controlled = (
        max(medium["nyquist"].values()) < 1.0e-2
        and max(fine["nyquist"].values()) < 1.0e-2
    )
    constraints_bounded = bool(
        medium["constraints_bounded"] and fine["constraints_bounded"]
    )
    stabilized = (
        no_invalid
        and no_crossing
        and fine_not_faster
        and nyquist_controlled
        and constraints_bounded
        and medium["final_time"] >= 8.0 - 1.0e-12
        and fine["final_time"] >= 8.0 - 1.0e-12
    )

    old_wall_midpoint = 0.5 * (24.78 + 25.36)
    overhead = arguments.ref_wall / old_wall_midpoint - 1.0
    total_wall = (
        arguments.ref_wall + arguments.double_wall + arguments.fine_wall
    )
    peak_rss = max(
        arguments.ref_rss, arguments.double_rss, arguments.fine_rss
    )
    run_count = 3 if arguments.double is not None else 2
    print(
        "D11_RESOLUTION"
        f" best_sigma={arguments.best_sigma:.12e}"
        f" medium_drift_0.1={format_crossing(medium_crossing)}"
        f" fine_drift_0.1={format_crossing(fine_crossing)}"
        f" medium_invalid={int(medium_invalid)}"
        f" fine_invalid={int(fine_invalid)}"
        f" fine_develops_faster={int(not fine_not_faster)}"
    )
    print(
        "D11_PERFORMANCE"
        f" new_long_evolutions={run_count}"
        f" total_wall_seconds={total_wall:.2f}"
        f" peak_rss_kb={peak_rss}"
        f" ref_wall_seconds={arguments.ref_wall:.2f}"
        f" fine_wall_seconds={arguments.fine_wall:.2f}"
        f" approximate_KO_wall_overhead_fraction={overhead:.12e}"
        " old_zero_sigma_wall_reference_seconds=24.78_to_25.36"
        " timing_note=SMALL_NOISY_CROSS_CHECK"
    )
    print(
        "D11_STABILIZATION_GATE"
        f" no_invalid_metric_through_t8={int(no_invalid)}"
        f" no_drift_0.1_crossing={int(no_crossing)}"
        f" radial_nyquist_controlled={int(nyquist_controlled)}"
        f" fine_not_faster={int(fine_not_faster)}"
        f" constraints_bounded={int(constraints_bounded)}"
    )
    classification = (
        "KO_PATH_RESTORED_AND_BACKGROUND_STABILIZED"
        if stabilized
        else "KO_PATH_RESTORED_BUT_INSTABILITY_PERSISTS"
    )
    print(f"D11_CLASSIFICATION {classification}")


if __name__ == "__main__":
    main()
