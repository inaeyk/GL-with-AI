#!/usr/bin/env python3
"""Independently classify retained D16 constraint-spectrum evidence."""

from __future__ import annotations

import math
import sys
from pathlib import Path


INVARIANCE_LIMIT = 1.0e-3
FULL_RESIDUAL_LIMIT = 1.0e-6


def fields(line: str) -> dict[str, str]:
    result: dict[str, str] = {}
    for token in line.split()[1:]:
        key, separator, value = token.partition("=")
        if separator:
            result[key] = value
    return result


def records(lines: list[str], prefix: str) -> list[dict[str, str]]:
    return [fields(line) for line in lines if line.startswith(prefix + " ")]


def main() -> int:
    if len(sys.argv) != 2:
        raise SystemExit(
            "usage: analyze_d16_constraint_spectrum.py <run-log>"
        )
    lines = Path(sys.argv[1]).read_text(encoding="utf-8").splitlines()

    matrix = records(lines, "D15_MATRIX_VALIDATION")
    if (
        len(matrix) != 1
        or matrix[0]["rows"] != "576"
        or matrix[0]["columns"] != "576"
        or matrix[0]["matrix_hash"] != "0x73c003dd70b673c6"
    ):
        raise RuntimeError("D16 did not construct the validated baseline once")

    svd = {
        record["projection"]: record
        for record in records(lines, "D16_CONSTRAINT_SVD")
    }
    expected = {
        "all_cells": (160, 160, 416),
        "interior_only": (130, 130, 446),
    }
    if set(svd) != set(expected):
        raise RuntimeError("D16 does not contain both constraint SVDs")
    for projection, (rows, rank, nullity) in expected.items():
        record = svd[projection]
        singular_values = [
            float(value) for value in record["singular_values"].split(",")
        ]
        if (
            int(record["rows"]) != rows
            or int(record["columns"]) != 576
            or int(record["rank"]) != rank
            or int(record["nullspace_dimension"]) != nullity
            or len(singular_values) != rows
            or any(not math.isfinite(value) for value in singular_values)
            or any(
                left < right
                for left, right in zip(
                    singular_values, singular_values[1:]
                )
            )
            or float(record["CN_residual"]) >= 1.0e-10
        ):
            raise RuntimeError(f"{projection} SVD evidence is inconsistent")

    projected = {
        record["projection"]: record
        for record in records(lines, "D16_PROJECTED_MAP")
    }
    if set(projected) != set(expected):
        raise RuntimeError("D16 projected-map evidence is incomplete")
    invariance = {
        name: float(record["invariance_eta"])
        for name, record in projected.items()
    }

    candidates = records(lines, "D16_CANDIDATE")
    leaders = {
        record["projection"]: record
        for record in candidates
        if record["role"] == "largest_ReOmega"
    }
    lowest = {
        record["projection"]: record
        for record in candidates
        if record["role"] == "smallest_constraint"
    }
    if set(leaders) != set(expected) or set(lowest) != set(expected):
        raise RuntimeError("D16 candidate evidence is incomplete")
    for record in candidates:
        required = (
            "Omega_real",
            "Omega_imag",
            "full_matrix_free_residual",
            "constraint_residual",
            "boundary_fraction",
            "maximum_radial",
            "nyquist_fraction",
            "conditioning_proxy",
            "profile_hash",
            "component_fractions",
        )
        if any(key not in record for key in required):
            raise RuntimeError("D16 candidate record is not independently usable")

    cross = records(lines, "D16_CROSS_PROJECTION")
    if len(cross) != 1:
        raise RuntimeError("D16 cross-projection evidence is missing")
    leader_residuals_good = all(
        float(record["full_matrix_free_residual"]) < FULL_RESIDUAL_LIMIT
        for record in leaders.values()
    )
    invariant = all(value < INVARIANCE_LIMIT for value in invariance.values())
    bulk = int(cross[0]["qualifying_matches"]) > 0

    if not invariant or not leader_residuals_good:
        classification = "CONSTRAINT_NULLSPACE_NOT_INVARIANT"
    elif bulk:
        classification = (
            "CREDIBLE_CONSTRAINT_COMPATIBLE_BULK_CANDIDATE_EXPOSED"
        )
    else:
        classification = "PROJECTED_SPECTRUM_REMAINS_BOUNDARY_DOMINATED"

    for projection in ("all_cells", "interior_only"):
        leader = leaders[projection]
        print(
            "D16_PARSER_PROJECTION"
            f" projection={projection}"
            f" rank={svd[projection]['rank']}"
            f" nullspace={svd[projection]['nullspace_dimension']}"
            f" constraint_hash={svd[projection]['matrix_hash']}"
            f" eta={invariance[projection]:.12e}"
            f" leader_ReOmega={float(leader['Omega_real']):.12e}"
            " leader_full_matrix_free_residual="
            f"{float(leader['full_matrix_free_residual']):.12e}"
            f" leader_boundary={float(leader['boundary_fraction']):.12e}"
        )
    print(
        "D16_PARSER_SUMMARY"
        f" invariant={int(invariant)}"
        f" leader_residuals_good={int(leader_residuals_good)}"
        f" qualifying_matches={int(bulk)}"
        " fixture_classification_read=0"
    )
    print(f"D16_CLASSIFICATION {classification}")
    print("BLACKSTRING_D16_RETAINED_EVIDENCE_PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
