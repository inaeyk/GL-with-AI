#!/usr/bin/env python3
"""Deterministic retained-evidence analysis for the frozen M2-C campaign."""

from __future__ import annotations

import argparse
import csv
import hashlib
import itertools
import json
import math
import sys
from collections.abc import Iterable, Sequence
from dataclasses import dataclass
from pathlib import Path
from typing import Any

BASE_M2B_COMMIT = "b5970454a649f8cbf46f694ebddcfe191b657530"
ACCEPTED = "PHYSICAL_RADIAL_AND_CONSTRAINT_STRATEGY_ACCEPTED"
PARTIAL = "PHYSICAL_RADIAL_AND_CONSTRAINT_STRATEGY_PARTIAL"
BLOCKED = "PHYSICAL_RADIAL_AND_CONSTRAINT_STRATEGY_BLOCKED"
EXPECTED_IDS = (
    "medium-near-gp",
    "medium-near-low-plus",
    "medium-near-low-minus",
    "medium-near-high-plus",
    "medium-near-high-minus",
    "fine-near-gp",
    "fine-near-low-plus",
    "fine-near-high-plus",
    "medium-far-gp",
    "medium-far-low-plus",
    "medium-far-high-plus",
    "medium-near-low-half",
)
PHYSICAL = ("q", "hww_over_chi", "K", "Aww", "GammaX")
CONSTRAINTS = ("H", "Mx", "Mz", "Zx", "Zz")
FIELDS = (
    "chi", "hxx", "hxz", "hzz", "hww", "K", "Axx", "Axz", "Azz",
    "Aww", "Theta", "GammaX", "GammaZ", "lapse", "shiftX", "shiftZ",
    "Bx", "Bz",
)
BULK_X_MIN = 1.25
BULK_X_MAX = 3.0


class Failure(RuntimeError):
    pass


def require(condition: bool, message: str) -> None:
    if not condition:
        raise Failure(message)


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def time_key(value: float | str) -> float:
    return round(float(value), 12)


def read_tsv(path: Path) -> list[dict[str, str]]:
    require(path.is_file(), f"missing retained table: {path}")
    with path.open(encoding="utf-8", newline="") as handle:
        return list(csv.DictReader(handle, delimiter="\t"))


@dataclass
class RunData:
    identifier: str
    directory: Path
    status: dict[float, dict[str, float]]
    measurements: dict[float, dict[tuple[str, str], dict[str, float]]]
    profiles: dict[float, dict[tuple[str, str], tuple[list[float], list[complex]]]]
    seed: dict[str, float] | None

    @property
    def times(self) -> list[float]:
        return sorted(self.status)


@dataclass
class Response:
    name: str
    times: list[float]
    profiles: dict[float, dict[str, tuple[list[float], list[complex]]]]
    harmonic_leakage: dict[float, dict[str, float]]
    floor: dict[str, float]


@dataclass
class Fit:
    slope: float
    stderr: float
    r2: float


@dataclass
class Plateau:
    passed: bool
    endpoint: float | None
    rate: float | None
    fields: list[str]
    field_rates: dict[str, float]


def load_run(root: Path, identifier: str, launch: dict[str, Any]) -> RunData:
    directory = root / "run_manifests/m2c/launches" / identifier
    status_rows = read_tsv(directory / "status.tsv")
    measurement_rows = read_tsv(directory / "measurements.tsv")
    profile_rows = read_tsv(directory / "profiles.tsv")
    status: dict[float, dict[str, float]] = {}
    for row in status_rows:
        key = time_key(row["time"])
        status[key] = {name: float(value) for name, value in row.items() if name != "time"}
    measurements: dict[float, dict[tuple[str, str], dict[str, float]]] = {}
    for row in measurement_rows:
        key = time_key(row["time"])
        kind_name = (row["kind"], row["name"])
        measurements.setdefault(key, {})[kind_name] = {
            name: float(value)
            for name, value in row.items()
            if name not in {"time", "kind", "name"}
        }
    temporary: dict[float, dict[tuple[str, str], list[tuple[int, float, complex]]]] = {}
    for row in profile_rows:
        key = time_key(row["time"])
        item = (
            int(row["radial_index"]),
            float(row["x"]),
            complex(float(row["cosine"]), float(row["sine"])),
        )
        temporary.setdefault(key, {}).setdefault((row["kind"], row["name"]), []).append(item)
    profiles: dict[float, dict[tuple[str, str], tuple[list[float], list[complex]]]] = {}
    for key, quantities in temporary.items():
        profiles[key] = {}
        for quantity, items in quantities.items():
            ordered = sorted(items)
            require([item[0] for item in ordered] == list(range(len(ordered))),
                    f"noncontiguous profile in {identifier} {key} {quantity}")
            profiles[key][quantity] = (
                [item[1] for item in ordered],
                [item[2] for item in ordered],
            )
    seed_path = directory / "seed_report.tsv"
    seed: dict[str, float] | None = None
    if seed_path.is_file():
        rows = read_tsv(seed_path)
        require(len(rows) == 1, f"invalid seed report: {identifier}")
        seed = {name: float(value) for name, value in rows[0].items()}
    require(status, f"empty status history: {identifier}")
    require(set(status) == set(measurements) == set(profiles),
            f"sample histories disagree: {identifier}")
    expected_samples = 321
    require(len(status) == expected_samples, f"{identifier} has {len(status)} samples, expected 321")
    times = sorted(status)
    require(abs(times[0]) <= 1.0e-12 and abs(times[-1] - 8.0) <= 1.0e-10,
            f"wrong time span: {identifier}")
    require(max(right - left for left, right in itertools.pairwise(times)) <= 0.0250000001,
            f"sampling cadence exceeds 0.025: {identifier}")
    for key in times:
        observed_fields = {
            name for kind, name in measurements[key] if kind == "field"
        }
        require(observed_fields == set(FIELDS), f"not all 18 fields retained: {identifier} t={key}")
        for name in (*PHYSICAL, *CONSTRAINTS):
            kind = "physical" if name in PHYSICAL else "constraint"
            require((kind, name) in profiles[key], f"missing profile {identifier} {key} {name}")
    return RunData(identifier, directory, status, measurements, profiles, seed)


def weighted_amplitude(x: Sequence[float], values: Sequence[complex]) -> float:
    selected = [
        (radius, value)
        for radius, value in zip(x, values)
        if BULK_X_MIN - 1.0e-12 <= radius <= BULK_X_MAX + 1.0e-12
    ]
    numerator = sum(radius * radius * abs(value) ** 2 for radius, value in selected)
    denominator = sum(radius * radius for radius, _ in selected)
    return math.sqrt(numerator / denominator) if denominator > 0.0 else 0.0


def combine_response(
    name: str,
    runs: dict[str, RunData],
    coefficients: dict[str, float],
    control: str,
    epsilon_for_floor: float,
) -> Response:
    selected = [runs[identifier] for identifier in coefficients]
    common_times = sorted(set.intersection(*(set(run.times) for run in selected)))
    profiles: dict[float, dict[str, tuple[list[float], list[complex]]]] = {}
    harmonic: dict[float, dict[str, float]] = {}
    for key in common_times:
        profiles[key] = {}
        harmonic[key] = {}
        for quantity in (*PHYSICAL, *CONSTRAINTS):
            kind = "physical" if quantity in PHYSICAL else "constraint"
            reference_x, reference_values = selected[0].profiles[key][(kind, quantity)]
            combined = [0j] * len(reference_values)
            for identifier, coefficient in coefficients.items():
                x, values = runs[identifier].profiles[key][(kind, quantity)]
                require(len(x) == len(reference_x) and all(abs(a - b) <= 1.0e-12 for a, b in zip(x, reference_x)),
                        f"response grids differ: {name} {key} {quantity}")
                for index, value in enumerate(values):
                    combined[index] += coefficient * value
            profiles[key][quantity] = (list(reference_x), combined)
            if quantity in PHYSICAL:
                harmonic[key][quantity] = max(
                    runs[identifier].measurements[key][("physical", quantity)]["harmonic_leakage"]
                    for identifier in coefficients
                )
    floors: dict[str, float] = {}
    control_run = runs[control]
    for quantity in (*PHYSICAL, *CONSTRAINTS):
        kind = "physical" if quantity in PHYSICAL else "constraint"
        leakage = max(
            weighted_amplitude(*control_run.profiles[key][(kind, quantity)])
            for key in common_times
        )
        floors[quantity] = max(1.0e-14, 10.0 * leakage / abs(epsilon_for_floor))
    return Response(name, common_times, profiles, harmonic, floors)


def response_amplitude(response: Response, key: float, quantity: str) -> float:
    return weighted_amplitude(*response.profiles[key][quantity])


def physical_norm(response: Response, key: float) -> float:
    return math.sqrt(sum(response_amplitude(response, key, name) ** 2 for name in PHYSICAL) / len(PHYSICAL))


def linear_fit(times: Sequence[float], amplitudes: Sequence[float]) -> Fit | None:
    if len(times) < 4 or any(not math.isfinite(value) or value <= 0.0 for value in amplitudes):
        return None
    values = [math.log(value) for value in amplitudes]
    mean_t = sum(times) / len(times)
    mean_y = sum(values) / len(values)
    sxx = sum((value - mean_t) ** 2 for value in times)
    if sxx <= 0.0:
        return None
    slope = sum((t - mean_t) * (y - mean_y) for t, y in zip(times, values)) / sxx
    intercept = mean_y - slope * mean_t
    residual = sum((y - (intercept + slope * t)) ** 2 for t, y in zip(times, values))
    total = sum((y - mean_y) ** 2 for y in values)
    r2 = 1.0 if total <= 1.0e-30 and residual <= 1.0e-30 else 1.0 - residual / total if total > 0.0 else 0.0
    stderr = math.sqrt(residual / (len(times) - 2) / sxx) if len(times) > 2 else math.inf
    return Fit(slope, stderr, r2)


def field_candidates(response: Response, quantity: str) -> dict[float, float]:
    by_width: dict[float, dict[float, Fit]] = {}
    for width in (0.5, 1.0):
        fits: dict[float, Fit] = {}
        for endpoint in response.times:
            if endpoint + 1.0e-12 < width:
                continue
            selected = [key for key in response.times if endpoint - width - 1.0e-12 <= key <= endpoint + 1.0e-12]
            amplitudes = [response_amplitude(response, key, quantity) for key in selected]
            if not selected or selected[0] > endpoint - width + 0.0250001:
                continue
            if min(amplitudes, default=0.0) <= response.floor[quantity]:
                continue
            fit = linear_fit(selected, amplitudes)
            if fit is not None and fit.r2 >= 0.90 and fit.stderr < abs(fit.slope) and fit.slope != 0.0:
                fits[endpoint] = fit
        candidates: dict[float, Fit] = {}
        endpoints = sorted(fits)
        for index in range(2, len(endpoints)):
            recent_keys = endpoints[index - 2 : index + 1]
            if any(abs(recent_keys[pos] - recent_keys[pos - 1] - 0.025) > 1.0e-8 for pos in (1, 2)):
                continue
            recent = [fits[key] for key in recent_keys]
            if not (all(item.slope > 0.0 for item in recent) or all(item.slope < 0.0 for item in recent)):
                continue
            rates = [item.slope for item in recent]
            variation = (max(rates) - min(rates)) / max(abs(sum(rates) / len(rates)), 1.0e-30)
            if variation <= 0.20:
                candidates[recent_keys[-1]] = Fit(sum(rates) / len(rates), max(item.stderr for item in recent), min(item.r2 for item in recent))
        by_width[width] = candidates
    common: dict[float, float] = {}
    for endpoint in set(by_width[0.5]) & set(by_width[1.0]):
        rates = [by_width[0.5][endpoint].slope, by_width[1.0][endpoint].slope]
        variation = (max(rates) - min(rates)) / max(abs(sum(rates) / 2.0), 1.0e-30)
        if variation <= 0.20 and rates[0] * rates[1] > 0.0:
            common[endpoint] = sum(rates) / 2.0
    return common


def detect_common_plateau(response: Response) -> Plateau:
    candidates = {name: field_candidates(response, name) for name in PHYSICAL}
    best: Plateau | None = None
    for endpoint in response.times:
        available = {name: values[endpoint] for name, values in candidates.items() if endpoint in values and values[endpoint] > 0.0}
        names = sorted(available)
        for count in range(len(names), 3, -1):
            accepted_subset: tuple[str, ...] | None = None
            for subset in itertools.combinations(names, count):
                rates = [available[name] for name in subset]
                mean = sum(rates) / len(rates)
                if (max(rates) - min(rates)) / max(abs(mean), 1.0e-30) <= 0.20:
                    accepted_subset = subset
                    break
            if accepted_subset is not None:
                field_rates = {name: available[name] for name in accepted_subset}
                candidate = Plateau(True, endpoint, sum(field_rates.values()) / len(field_rates), list(accepted_subset), field_rates)
                if best is None or len(candidate.fields) > len(best.fields) or (len(candidate.fields) == len(best.fields) and float(candidate.endpoint) > float(best.endpoint)):
                    best = candidate
                break
    return best or Plateau(False, None, None, [], {})


def response_quality(response: Response, keys: Iterable[float]) -> dict[str, float]:
    maximum_boundary = 0.0
    maximum_nyquist = 0.0
    maximum_parity = 0.0
    maximum_harmonic = 0.0
    for key in keys:
        for quantity in PHYSICAL:
            x, values = response.profiles[key][quantity]
            energy = sum(radius * radius * abs(value) ** 2 for radius, value in zip(x, values))
            boundary = sum(
                radius * radius * abs(value) ** 2
                for index, (radius, value) in enumerate(zip(x, values))
                if index < 5 or index >= len(values) - 5
            )
            parity = sum(radius * radius * value.imag ** 2 for radius, value in zip(x, values))
            nyquist = abs(sum((1 if index % 2 == 0 else -1) * value for index, value in enumerate(values)))
            nyquist_fraction = nyquist / math.sqrt(len(values) * sum(abs(value) ** 2 for value in values)) if any(values) else 0.0
            maximum_boundary = max(maximum_boundary, boundary / energy if energy > 0.0 else 0.0)
            maximum_nyquist = max(maximum_nyquist, nyquist_fraction)
            maximum_parity = max(maximum_parity, math.sqrt(parity / energy) if energy > 0.0 else 0.0)
            maximum_harmonic = max(maximum_harmonic, response.harmonic_leakage[key][quantity])
    return {
        "max_boundary_fraction": maximum_boundary,
        "max_nyquist_fraction": maximum_nyquist,
        "max_parity_leakage": maximum_parity,
        "max_harmonic_leakage": maximum_harmonic,
    }


def interpolate(x: Sequence[float], values: Sequence[complex], target: float) -> complex:
    if target <= x[0]:
        return values[0]
    if target >= x[-1]:
        return values[-1]
    for index in range(1, len(x)):
        if x[index] >= target:
            fraction = (target - x[index - 1]) / (x[index] - x[index - 1])
            return values[index - 1] * (1.0 - fraction) + values[index] * fraction
    return values[-1]


def profile_overlap(left: tuple[list[float], list[complex]], right: tuple[list[float], list[complex]]) -> float:
    x_left, values_left = left
    x_right, values_right = right
    common = [
        (x, value)
        for x, value in zip(x_left, values_left)
        if BULK_X_MIN - 1.0e-12 <= x <= BULK_X_MAX + 1.0e-12
        and x_right[0] - 1.0e-12 <= x <= x_right[-1] + 1.0e-12
    ]
    if not common:
        return 0.0
    dot = 0j
    norm_left = 0.0
    norm_right = 0.0
    for x, left_value in common:
        right_value = interpolate(x_right, values_right, x)
        weight = x * x
        dot += weight * left_value.conjugate() * right_value
        norm_left += weight * abs(left_value) ** 2
        norm_right += weight * abs(right_value) ** 2
    return abs(dot) / math.sqrt(norm_left * norm_right) if norm_left > 0.0 and norm_right > 0.0 else 0.0


def nearest_time(response: Response, target: float) -> float:
    return min(response.times, key=lambda value: abs(value - target))


def seed_gate(run: RunData, epsilon: float) -> bool:
    report = run.seed
    if report is None:
        return False
    return (
        report["rank"] > 0
        and report["retained_condition"] <= 1.0e12
        and report["scaled_rms"] <= 1.0e-8 * abs(epsilon)
        and report["max_hm"] <= 5.0e-8 * abs(epsilon)
        and report["max_gamma_z"] <= 1.0e-10 * abs(epsilon)
        and report["incoming_boundary"] <= 1.0e-11 * abs(epsilon)
        and report["determinant"] <= 5.0e-13
        and report["weighted_trace"] <= 5.0e-13
        and report["amplitude_error"] <= 5.0e-13
        and report["antisymmetry"] <= 5.0e-13
        and report["retained_physical_fraction"] >= 0.25
    )


def constraint_gate(
    response: Response,
    endpoint: float | None,
    physical_rate: float | None,
    fine: Response | None = None,
) -> tuple[bool, dict[str, Any]]:
    if endpoint is None:
        keys = [key for key in response.times if key >= 1.0]
    else:
        keys = [key for key in response.times if endpoint - 1.0 - 1.0e-12 <= key <= endpoint + 1.0e-12]
    ratios: dict[str, float] = {}
    faster: list[str] = []
    for name in CONSTRAINTS:
        ratios[name] = max(
            response_amplitude(response, key, name) / max(physical_norm(response, key), 1.0e-30)
            for key in keys
        )
        candidates = field_candidates(response, name)
        if physical_rate is not None and any(rate > physical_rate for key, rate in candidates.items() if key in keys):
            faster.append(name)
    fine_ratios: dict[str, float] = {}
    resolution_pass = True
    if fine is not None:
        fine_endpoint = nearest_time(fine, endpoint if endpoint is not None else 8.0)
        fine_keys = [key for key in fine.times if fine_endpoint - 1.0 - 1.0e-12 <= key <= fine_endpoint + 1.0e-12]
        for name in CONSTRAINTS:
            fine_ratios[name] = max(
                response_amplitude(fine, key, name) / max(physical_norm(fine, key), 1.0e-30)
                for key in fine_keys
            )
            if fine_ratios[name] > 1.25 * ratios[name] and max(fine_ratios[name], ratios[name]) > 1.0e-10:
                resolution_pass = False
    passed = all(value < 0.10 for value in ratios.values()) and not faster and resolution_pass
    return passed, {
        "ratios": ratios,
        "faster_constraints": faster,
        "fine_ratios": fine_ratios,
        "resolution_pass": resolution_pass,
    }


def gp_gate(run: RunData) -> tuple[bool, dict[str, Any]]:
    statuses = list(run.status.values())
    maximum_drift = max(item["maximum_state_drift"] for item in statuses)
    determinant = max(item["determinant_max"] for item in statuses)
    trace = max(item["weighted_trace_max"] for item in statuses)
    maximum_z = max(
        run.measurements[key][("field", field)]["amplitude"]
        for key in run.times for field in FIELDS
    )
    maximum_nyquist = max(
        run.measurements[key][("field", field)]["nyquist_amplitude"]
        for key in run.times for field in FIELDS
    )
    passed = (
        all(item["finite"] == 1.0 and item["metric_admissible"] == 1.0 and item["ownership_pass"] == 1.0 for item in statuses)
        and abs(run.times[-1] - 8.0) <= 1.0e-10
        and maximum_drift < 0.10
        and determinant < 1.0e-12
        and trace < 1.0e-12
        and maximum_z < 1.0e-11
        and maximum_nyquist < 1.0e-11
    )
    return passed, {
        "maximum_drift": maximum_drift,
        "determinant_max": determinant,
        "weighted_trace_max": trace,
        "maximum_z_dependent_amplitude": maximum_z,
        "maximum_radial_nyquist_amplitude": maximum_nyquist,
    }


def relative_difference(left: float | None, right: float | None) -> float | None:
    if left is None or right is None:
        return None
    return abs(left - right) / max(abs(left), abs(right), 1.0e-30)


def finite_or_none(value: float | None) -> float | None:
    return value if value is not None and math.isfinite(value) else None


def compute(root: Path, raw: dict[str, Any]) -> tuple[dict[str, Any], dict[str, Any]]:
    launches = raw.get("launches", [])
    launch_ids = [str(item.get("id", "")) for item in launches]
    launch_count = int(raw.get("attempt_count", -1))
    all_exit_zero = all(int(item.get("exit_code", -1)) == 0 for item in launches)
    common: dict[str, Any] = {
        "schema_version": 1,
        "base_m2b_commit": BASE_M2B_COMMIT,
        "launch_count": launch_count,
        "launch_ids": launch_ids,
        "all_launches_exit_zero": all_exit_zero,
        "gp_controls_pass": False,
        "mpi_ownership_passed": False,
        "scientific_acceptance_passed": False,
        "human_review_required": True,
        "stop_reason": "",
        "failed_gates": [],
        "performance": {
            "status": "reused_m2b",
            "slowdown_fraction": -0.00116695,
        },
        "restart_regression_status": "reused_m2b",
        "comparison_region": {"x_min": BULK_X_MIN, "x_max": BULK_X_MAX},
    }
    details: dict[str, Any] = {"schema_version": 1, "launch_ids": launch_ids}
    prelaunch_path = root / "run_manifests/m2c/prelaunch/prelaunch_summary.json"
    if launch_count == 0 and prelaunch_path.is_file():
        prelaunch = json.loads(prelaunch_path.read_text(encoding="utf-8"))
        details["prelaunch"] = prelaunch
        if not bool(prelaunch.get("all_passed", False)):
            failed_gates = prelaunch.get("failed_gates", [])
            require(isinstance(failed_gates, list) and failed_gates,
                    "failed pre-launch summary omitted failed gates")
            common.update(
                classification=BLOCKED,
                stop_reason=str(prelaunch.get("stop_reason", "pre-launch gate failed")),
                failed_gates=[str(item) for item in failed_gates],
            )
            return common, details
    if not all_exit_zero:
        failed = next((item for item in launches if int(item.get("exit_code", -1)) != 0), None)
        identifier = str(failed.get("id")) if failed else "unknown"
        common.update(
            classification=BLOCKED,
            stop_reason=f"numerical launch failed: {identifier}",
            failed_gates=[f"launch_exit_zero:{identifier}"],
        )
        return common, details
    if launch_count != 12 or tuple(launch_ids) != EXPECTED_IDS:
        common.update(
            classification=BLOCKED,
            stop_reason="incomplete frozen launch matrix or provenance",
            failed_gates=["frozen_launch_matrix_complete"],
        )
        return common, details

    runs = {identifier: load_run(root, identifier, launch) for identifier, launch in zip(launch_ids, launches)}
    ownership = all(all(item["ownership_pass"] == 1.0 for item in run.status.values()) for run in runs.values())
    common["mpi_ownership_passed"] = ownership
    gp_results = {identifier: gp_gate(runs[identifier]) for identifier in ("medium-near-gp", "fine-near-gp", "medium-far-gp")}
    medium_drift = gp_results["medium-near-gp"][1]["maximum_drift"]
    no_regression = all(
        gp_results[identifier][1]["maximum_drift"] <= max(1.25 * medium_drift, 1.0e-6)
        for identifier in ("fine-near-gp", "medium-far-gp")
    )
    gp_pass = all(value[0] for value in gp_results.values()) and no_regression
    common["gp_controls_pass"] = gp_pass
    details["gp_controls"] = {identifier: value[1] | {"passed": value[0]} for identifier, value in gp_results.items()}
    details["gp_no_material_regression"] = no_regression

    low_medium = combine_response(
        "low_medium_odd", runs,
        {"medium-near-low-plus": 1.0 / (2.0e-8), "medium-near-low-minus": -1.0 / (2.0e-8)},
        "medium-near-gp", 1.0e-8,
    )
    low_even = combine_response(
        "low_medium_even", runs,
        {"medium-near-low-plus": 1.0 / (2.0e-8), "medium-near-low-minus": 1.0 / (2.0e-8), "medium-near-gp": -1.0 / 1.0e-8},
        "medium-near-gp", 1.0e-8,
    )
    low_plus = combine_response(
        "low_medium_plus", runs,
        {"medium-near-low-plus": 1.0 / 1.0e-8, "medium-near-gp": -1.0 / 1.0e-8},
        "medium-near-gp", 1.0e-8,
    )
    low_fine = combine_response(
        "low_fine", runs,
        {"fine-near-low-plus": 1.0 / 1.0e-8, "fine-near-gp": -1.0 / 1.0e-8},
        "fine-near-gp", 1.0e-8,
    )
    low_far = combine_response(
        "low_far", runs,
        {"medium-far-low-plus": 1.0 / 1.0e-8, "medium-far-gp": -1.0 / 1.0e-8},
        "medium-far-gp", 1.0e-8,
    )
    low_half = combine_response(
        "low_half", runs,
        {"medium-near-low-half": 1.0 / 5.0e-9, "medium-near-gp": -1.0 / 5.0e-9},
        "medium-near-gp", 5.0e-9,
    )
    high_medium = combine_response(
        "high_medium_odd", runs,
        {"medium-near-high-plus": 1.0 / (2.0e-8), "medium-near-high-minus": -1.0 / (2.0e-8)},
        "medium-near-gp", 1.0e-8,
    )
    high_fine = combine_response(
        "high_fine", runs,
        {"fine-near-high-plus": 1.0 / 1.0e-8, "fine-near-gp": -1.0 / 1.0e-8},
        "fine-near-gp", 1.0e-8,
    )
    high_far = combine_response(
        "high_far", runs,
        {"medium-far-high-plus": 1.0 / 1.0e-8, "medium-far-gp": -1.0 / 1.0e-8},
        "medium-far-gp", 1.0e-8,
    )

    low_plateau = detect_common_plateau(low_medium)
    fine_plateau = detect_common_plateau(low_fine)
    far_plateau = detect_common_plateau(low_far)
    high_plateaus = {
        "medium": detect_common_plateau(high_medium),
        "fine": detect_common_plateau(high_fine),
        "far": detect_common_plateau(high_far),
    }
    low_endpoint = low_plateau.endpoint if low_plateau.endpoint is not None else 8.0
    low_keys = [key for key in low_medium.times if low_endpoint - 1.0 - 1.0e-12 <= key <= low_endpoint + 1.0e-12]
    fine_endpoint = fine_plateau.endpoint if fine_plateau.endpoint is not None else low_endpoint
    far_endpoint = far_plateau.endpoint if far_plateau.endpoint is not None else low_endpoint
    resolution_overlap = profile_overlap(
        low_medium.profiles[nearest_time(low_medium, low_endpoint)]["q"],
        low_fine.profiles[nearest_time(low_fine, fine_endpoint)]["q"],
    )
    boundary_overlap = profile_overlap(
        low_medium.profiles[nearest_time(low_medium, low_endpoint)]["q"],
        low_far.profiles[nearest_time(low_far, far_endpoint)]["q"],
    )
    half_difference = max(
        abs(physical_norm(low_plus, key) - physical_norm(low_half, key)) /
        max(physical_norm(low_plus, key), physical_norm(low_half, key), low_plus.floor["q"])
        for key in low_keys
    )
    even_odd_ratio = max(
        physical_norm(low_even, key) / max(physical_norm(low_medium, key), 1.0e-30)
        for key in low_keys
    )
    low_quality_values = [
        response_quality(response, [key for key in response.times if (response is low_medium and key in low_keys) or (response is not low_medium and key >= max(0.0, (fine_endpoint if response is low_fine else far_endpoint) - 1.0))])
        for response in (low_medium, low_fine, low_far)
    ]
    low_quality = {name: max(value[name] for value in low_quality_values) for name in low_quality_values[0]}
    low_constraints_pass, low_constraints = constraint_gate(low_medium, low_plateau.endpoint, low_plateau.rate, low_fine)

    high_quality_values = [response_quality(response, [key for key in response.times if key >= 1.0]) for response in (high_medium, high_fine, high_far)]
    high_quality = {name: max(value[name] for value in high_quality_values) for name in high_quality_values[0]}
    high_constraints_pass, high_constraints = constraint_gate(high_medium, None, None, high_fine)
    no_high_positive = not any(item.passed for item in high_plateaus.values())
    separated = low_plateau.passed and no_high_positive

    seeded_epsilons = {
        "medium-near-low-plus": 1.0e-8,
        "medium-near-low-minus": -1.0e-8,
        "medium-near-high-plus": 1.0e-8,
        "medium-near-high-minus": -1.0e-8,
        "fine-near-low-plus": 1.0e-8,
        "fine-near-high-plus": 1.0e-8,
        "medium-far-low-plus": 1.0e-8,
        "medium-far-high-plus": 1.0e-8,
        "medium-near-low-half": 5.0e-9,
    }
    seeds_pass = all(seed_gate(runs[identifier], epsilon) for identifier, epsilon in seeded_epsilons.items())
    low_mode = {
        "k": math.pi / 4.0,
        "positive_common_rate": low_plateau.passed,
        "common_rate": finite_or_none(low_plateau.rate),
        "common_field_count": len(low_plateau.fields),
        "common_fields": low_plateau.fields,
        "rate_resolution_relative_difference": finite_or_none(relative_difference(low_plateau.rate, fine_plateau.rate)),
        "rate_boundary_relative_difference": finite_or_none(relative_difference(low_plateau.rate, far_plateau.rate)),
        "profile_overlap_resolution": resolution_overlap,
        "profile_overlap_boundary": boundary_overlap,
        "half_amplitude_relative_difference": half_difference,
        "even_odd_ratio": even_odd_ratio,
        **low_quality,
        "constraints_controlled": low_constraints_pass and seeds_pass,
    }
    high_mode = {
        "k": math.pi / 2.0,
        "no_positive_common_plateau": no_high_positive,
        "separated_from_low": separated,
        "constraints_controlled": high_constraints_pass and seeds_pass,
        "max_boundary_fraction": high_quality["max_boundary_fraction"],
        "max_nyquist_fraction": high_quality["max_nyquist_fraction"],
        "max_parity_leakage": high_quality["max_parity_leakage"],
        "max_harmonic_leakage": high_quality["max_harmonic_leakage"],
    }
    common["low_mode"] = low_mode
    common["high_mode"] = high_mode
    common["performance"] = {"status": "reused_m2b", "slowdown_fraction": -0.00116695}
    common["restart_regression_status"] = "reused_m2b"
    details.update(
        low_plateaus={
            "medium": low_plateau.__dict__,
            "fine": fine_plateau.__dict__,
            "far": far_plateau.__dict__,
        },
        high_plateaus={name: value.__dict__ for name, value in high_plateaus.items()},
        low_constraints=low_constraints,
        high_constraints=high_constraints,
        corrected_seed_gate=seeds_pass,
        low_mode=low_mode,
        high_mode=high_mode,
    )

    gates = {
        "all_launches_exit_zero": all_exit_zero,
        "gp_controls": gp_pass,
        "mpi_ownership": ownership,
        "corrected_seed_residuals": seeds_pass,
        "low_positive_common_plateau": low_plateau.passed,
        "low_common_field_count": len(low_plateau.fields) >= 4,
        "low_resolution_rate": fine_plateau.passed and relative_difference(low_plateau.rate, fine_plateau.rate) is not None and float(relative_difference(low_plateau.rate, fine_plateau.rate)) <= 0.20,
        "low_boundary_rate": far_plateau.passed and relative_difference(low_plateau.rate, far_plateau.rate) is not None and float(relative_difference(low_plateau.rate, far_plateau.rate)) <= 0.20,
        "low_profile_resolution": resolution_overlap >= 0.99,
        "low_profile_boundary": boundary_overlap >= 0.99,
        "low_half_amplitude": half_difference <= 5.0e-4,
        "low_even_odd": even_odd_ratio <= 1.0e-3,
        "low_boundary_fraction": low_quality["max_boundary_fraction"] < 0.30,
        "low_nyquist_fraction": low_quality["max_nyquist_fraction"] < 0.10,
        "low_parity_leakage": low_quality["max_parity_leakage"] < 1.0e-5,
        "low_harmonic_leakage": low_quality["max_harmonic_leakage"] < 1.0e-5,
        "low_constraints": low_constraints_pass,
        "high_no_positive_common_plateau": no_high_positive,
        "high_separated_from_low": separated,
        "high_constraints": high_constraints_pass,
        "high_boundary_fraction": high_quality["max_boundary_fraction"] < 0.30,
        "high_nyquist_fraction": high_quality["max_nyquist_fraction"] < 0.10,
        "high_parity_leakage": high_quality["max_parity_leakage"] < 1.0e-5,
        "performance_slowdown": True,
        "restart_regression": True,
    }
    failed_gates = [name for name, passed in gates.items() if not passed]
    details["gates"] = gates
    common["failed_gates"] = failed_gates
    accepted = not failed_gates
    common["scientific_acceptance_passed"] = accepted
    common["classification"] = ACCEPTED if accepted else PARTIAL
    common["human_review_required"] = not accepted
    common["stop_reason"] = "" if accepted else "all twelve launches completed but frozen scientific gates did not all pass"
    return common, details


def verify_manifest(root: Path, raw: dict[str, Any]) -> None:
    require(raw.get("schema_version") == 1, "raw schema mismatch")
    require(raw.get("base_m2b_commit") == BASE_M2B_COMMIT, "raw base mismatch")
    launches = raw.get("launches")
    require(isinstance(launches, list), "raw launches is not a list")
    require(raw.get("attempt_count") == len(launches), "raw attempt count mismatch")
    require(tuple(str(item.get("id", "")) for item in launches) == EXPECTED_IDS[: len(launches)], "raw launch order mismatch")
    for launch in launches:
        evidence = launch.get("evidence_files")
        require(isinstance(evidence, list) and evidence, f"empty evidence list: {launch.get('id')}")
        for item in evidence:
            relative = Path(str(item.get("path", "")))
            require(not relative.is_absolute() and ".." not in relative.parts, "unsafe evidence path")
            require(relative.parts[:2] == ("run_manifests", "m2c"), f"nonportable evidence path: {relative}")
            path = root / relative
            require(path.is_file(), f"missing evidence: {relative}")
            require(path.stat().st_size == int(item.get("size", -1)), f"evidence size mismatch: {relative}")
            require(sha256(path) == str(item.get("sha256", "")), f"evidence hash mismatch: {relative}")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--summary", required=True, type=Path)
    parser.add_argument("--raw-manifest", required=True, type=Path)
    parser.add_argument("--verify", action="store_true")
    parser.add_argument("--write", action="store_true")
    args = parser.parse_args()
    root = Path.cwd().resolve()
    try:
        raw = json.loads((root / args.raw_manifest).read_text(encoding="utf-8"))
        verify_manifest(root, raw)
        summary, details = compute(root, raw)
        if args.write:
            summary_path = root / args.summary
            summary_path.parent.mkdir(parents=True, exist_ok=True)
            summary_path.write_text(json.dumps(summary, indent=2, sort_keys=True) + "\n", encoding="utf-8")
            analysis_path = root / "run_manifests/m2c/analysis.json"
            analysis_path.write_text(json.dumps(details, indent=2, sort_keys=True) + "\n", encoding="utf-8")
        if args.verify:
            observed = json.loads((root / args.summary).read_text(encoding="utf-8"))
            require(observed == summary, "acceptance summary does not match retained numerical evidence")
        print(f"M2C_ANALYZER_PASS classification={summary['classification']} launches={summary['launch_count']}")
        return 0
    except (OSError, ValueError, KeyError, csv.Error, Failure) as error:
        print(f"M2C_ANALYZER_FAIL: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
