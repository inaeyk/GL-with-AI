#!/usr/bin/env python3
"""Independently validate and classify a repaired D15 matrix log."""

from __future__ import annotations

import math
import sys
from dataclasses import dataclass
from pathlib import Path


NX = 32
COMMON_COMPONENTS = 18
PHYSICAL_COMPONENTS = 13
BOUNDARY_LIMIT = 0.40
CONSTRAINT_LIMIT = 0.0500752
NYQUIST_LIMIT = 0.20
PROFILE_OVERLAP_LIMIT = 0.90
RATE_RELATIVE_LIMIT = 0.20
MAXIMUM_CELL_SEPARATION = 2


def fields(line: str) -> dict[str, str]:
    result: dict[str, str] = {}
    for token in line.split()[1:]:
        name, separator, value = token.partition("=")
        if separator:
            result[name] = value
    return result


def records(lines: list[str], prefix: str) -> list[dict[str, str]]:
    return [fields(line) for line in lines if line.startswith(prefix + " ")]


@dataclass(frozen=True)
class Candidate:
    variant: str
    index: int
    omega: complex
    matrix_free_residual: float
    parity: float
    reconstruction: float
    boundary: float
    constraint: float
    nyquist: float
    maximum_radial: int
    physical_fields: int
    profile_hash: str

    @classmethod
    def parse(cls, record: dict[str, str]) -> "Candidate":
        return cls(
            variant=record["variant"],
            index=int(record["eigen_index"]),
            omega=complex(
                float(record["Omega_real"]), float(record["Omega_imag"])
            ),
            matrix_free_residual=float(record["matrix_free_residual"]),
            parity=float(record["forbidden_parity_leakage"]),
            reconstruction=float(record["fourier_reconstruction_defect"]),
            boundary=float(record["boundary_fraction"]),
            constraint=float(record["total_constraint"]),
            nyquist=float(record["nyquist_fraction"]),
            maximum_radial=int(record["maximum_radial"]),
            physical_fields=int(record["physical_fields"]),
            profile_hash=record["profile_hash"],
        )

    def bulk_eligible(self) -> bool:
        return (
            self.matrix_free_residual < 1.0e-7
            and self.parity < 1.0e-5
            and self.reconstruction < 1.0e-7
            and self.boundary < BOUNDARY_LIMIT
            and self.constraint < CONSTRAINT_LIMIT
            and self.nyquist < NYQUIST_LIMIT
            and self.physical_fields >= 3
            and 3 <= self.maximum_radial < NX - 3
        )


def parse_profile(text: str) -> list[complex]:
    result: list[complex] = []
    for packed in text.split(","):
        real, separator, imaginary = packed.partition(":")
        if not separator:
            raise RuntimeError("malformed D15 profile coefficient")
        result.append(complex(float(real), float(imaginary)))
    if len(result) != NX * COMMON_COMPONENTS:
        raise RuntimeError(
            f"D15 common profile has {len(result)} rather than 576 entries"
        )
    if not all(math.isfinite(value.real) and math.isfinite(value.imag)
               for value in result):
        raise RuntimeError("D15 common profile contains a nonfinite entry")
    return result


def physical_overlap(left: list[complex], right: list[complex]) -> float:
    numerator = 0.0j
    left_norm = 0.0
    right_norm = 0.0
    for radial in range(NX):
        offset = radial * COMMON_COMPONENTS
        for component in range(PHYSICAL_COMPONENTS):
            a = left[offset + component]
            b = right[offset + component]
            numerator += a.conjugate() * b
            left_norm += abs(a) ** 2
            right_norm += abs(b) ** 2
    return abs(numerator) / max(
        math.sqrt(left_norm * right_norm), sys.float_info.min
    )


def rates_agree(left: Candidate, right: Candidate) -> bool:
    scale = max(abs(left.omega.real), abs(right.omega.real), 1.0e-2)
    return (
        abs(left.omega.real - right.omega.real)
        <= RATE_RELATIVE_LIMIT * scale
    )


def rank_key(metric: str, candidate: Candidate) -> float:
    if metric == "largest_ReOmega":
        return -candidate.omega.real
    if metric == "smallest_constraint":
        return candidate.constraint
    if metric == "smallest_boundary":
        return candidate.boundary
    raise RuntimeError(f"unknown D15 rank metric {metric}")


def main() -> int:
    if len(sys.argv) != 2:
        raise SystemExit("usage: analyze_d15_sector_isolation.py <run-log>")
    lines = Path(sys.argv[1]).read_text(encoding="utf-8").splitlines()

    baseline = records(lines, "D15_BASELINE_REUSED")
    if len(baseline) != 1:
        raise RuntimeError("D15 log lacks exactly one reused baseline record")
    if (
        baseline[0]["matrix_hash"] != "0x73c003dd70b673c6"
        or baseline[0]["matrix_reconstructed"] != "0"
        or baseline[0]["tangent_actions"] != "0"
    ):
        raise RuntimeError("D14 baseline was not reused exactly as committed")

    summaries = {
        record["variant"]: record
        for record in records(lines, "D15_SPECTRUM_SUMMARY")
    }
    expected = {
        "frozen_gauge": ("416", "416"),
        "exact_gp_ghosts": ("576", "576"),
    }
    if set(summaries) != set(expected):
        raise RuntimeError("D15 log does not contain exactly two new spectra")
    for variant, dimensions in expected.items():
        actual = (
            summaries[variant]["dimension"],
            summaries[variant]["eigenvalue_count"],
        )
        if actual != dimensions:
            raise RuntimeError(
                f"{variant} dimension/eigenvalue count {actual} != {dimensions}"
            )

    frozen_gate = records(lines, "D15_FROZEN_DIMENSION_GATE")
    ghost_gate = records(lines, "D15_GHOST_ONLY_GATE")
    if (
        len(frozen_gate) != 1
        or frozen_gate[0]["matrix"] != "416x416"
        or frozen_gate[0]["eigenvalues"] != "416"
        or frozen_gate[0]["old_embedded_576_mutation"] != "REJECTED"
    ):
        raise RuntimeError("native frozen-gauge mutation gate did not pass")
    if (
        len(ghost_gate) != 1
        or float(ghost_gate[0]["maximum_provisional_exact_ghost_difference"])
        <= 0.0
        or ghost_gate[0]["rhs_calls"] != ghost_gate[0]["outer_rhs_calls"]
        or ghost_gate[0]["outer_override_same_as_baseline"] != "1"
        or ghost_gate[0]["disabled_outer_override_mutation"] != "REJECTED"
    ):
        raise RuntimeError("exact-GP ghost-only ownership gate did not pass")

    thresholds = {
        record["variant"]: record
        for record in records(lines, "D15_ARTIFACT_THRESHOLDS")
    }
    if set(thresholds) != set(expected):
        raise RuntimeError("D15 artifact thresholds are incomplete")
    for variant, threshold in thresholds.items():
        if (
            float(threshold["eigenvalue_floor"]) <= 0.0
            or float(threshold["matrix_free_residual_limit"]) != 1.0e-7
            or float(threshold["forbidden_parity_limit"]) != 1.0e-5
            or float(threshold["reconstruction_limit"]) != 1.0e-7
        ):
            raise RuntimeError(f"{variant} artifact threshold is invalid")

    rejected = records(lines, "D15_CANDIDATE_REJECT")
    for rejected_record in rejected:
        if not rejected_record.get("reason"):
            raise RuntimeError("D15 artifact rejection lacks a reason")
        if (
            rejected_record.get("Omega_not_evaluated") == "1"
            and "eigenvalue_floor" not in rejected_record
        ):
            raise RuntimeError("below-floor rejection lacks its floor")
    rejected_by_variant = {
        variant: sum(
            item["variant"] == variant for item in rejected
        )
        for variant in expected
    }
    for variant in expected:
        if rejected_by_variant[variant] != int(
            summaries[variant]["rejected_candidates"]
        ):
            raise RuntimeError(
                f"{variant} did not emit one reason per rejected candidate"
            )

    candidates_by_variant: dict[str, list[Candidate]] = {
        variant: [] for variant in expected
    }
    for record in records(lines, "D15_CANDIDATE_RECORD"):
        candidate = Candidate.parse(record)
        if candidate.variant not in candidates_by_variant:
            raise RuntimeError("candidate belongs to an unknown D15 variant")
        if not (
            candidate.matrix_free_residual < 1.0e-7
            and candidate.parity < 1.0e-5
            and candidate.reconstruction < 1.0e-7
        ):
            raise RuntimeError("artifact candidate was retained")
        candidates_by_variant[candidate.variant].append(candidate)

    emitted_ranks = records(lines, "D15_RANK")
    for variant, candidates in candidates_by_variant.items():
        if not candidates:
            raise RuntimeError(f"{variant} retained no candidates")
        for metric in (
            "largest_ReOmega",
            "smallest_constraint",
            "smallest_boundary",
        ):
            independently_ranked = sorted(
                candidates,
                key=lambda candidate: (
                    rank_key(metric, candidate),
                    candidate.index,
                ),
            )[:3]
            recorded = [
                item
                for item in emitted_ranks
                if item["variant"] == variant and item["metric"] == metric
            ]
            recorded.sort(key=lambda item: int(item["rank"]))
            if len(recorded) != 3:
                raise RuntimeError(
                    f"{variant} {metric} does not contain three ranks"
                )
            by_index = {candidate.index: candidate for candidate in candidates}
            cutoff = rank_key(metric, independently_ranked[-1])
            if any(
                int(item["eigen_index"]) not in by_index
                or rank_key(
                    metric, by_index[int(item["eigen_index"])]
                ) > cutoff + 1.0e-12
                for item in recorded
            ):
                raise RuntimeError(
                    f"{variant} {metric} emitted a candidate below the top set"
                )
            for rank, candidate in enumerate(independently_ranked, start=1):
                print(
                    "D15_PARSER_RANK"
                    f" variant={variant} metric={metric} rank={rank}"
                    f" eigen_index={candidate.index}"
                    f" ReOmega={candidate.omega.real:.12e}"
                    f" constraint={candidate.constraint:.12e}"
                    f" boundary={candidate.boundary:.12e}"
                    f" profile_hash={candidate.profile_hash}"
                )

    profile_records = records(lines, "D15_MATCH_PROFILE")
    profiles = {
        (item["variant"], int(item["eigen_index"])): parse_profile(
            item["coefficients"]
        )
        for item in profile_records
    }
    eligible = {
        variant: [item for item in candidates if item.bulk_eligible()]
        for variant, candidates in candidates_by_variant.items()
    }
    for variant, candidates in eligible.items():
        for candidate in candidates:
            if (variant, candidate.index) not in profiles:
                raise RuntimeError(
                    f"{variant} bulk candidate lacks retained coefficients"
                )

    matches: list[tuple[Candidate, Candidate, float]] = []
    for frozen in eligible["frozen_gauge"]:
        for exact in eligible["exact_gp_ghosts"]:
            overlap = physical_overlap(
                profiles[(frozen.variant, frozen.index)],
                profiles[(exact.variant, exact.index)],
            )
            rate = rates_agree(frozen, exact)
            location = abs(
                frozen.maximum_radial - exact.maximum_radial
            ) <= MAXIMUM_CELL_SEPARATION
            passes = overlap > PROFILE_OVERLAP_LIMIT and rate and location
            print(
                "D15_PARSER_MATCH_EVALUATION"
                f" frozen_index={frozen.index} exact_index={exact.index}"
                f" physical_profile_overlap={overlap:.12e}"
                f" rate_agreement={int(rate)}"
                " radial_maximum_separation="
                f"{abs(frozen.maximum_radial - exact.maximum_radial)}"
                f" passes={int(passes)}"
            )
            if passes:
                matches.append((frozen, exact, overlap))

    classification = (
        "CREDIBLE_BULK_MODE_CANDIDATE_EXPOSED"
        if matches
        else "NO_BULK_PHYSICAL_CANDIDATE_AFTER_SECTOR_ISOLATION"
    )
    print(
        "D15_PARSER_SUMMARY"
        f" frozen_retained={len(candidates_by_variant['frozen_gauge'])}"
        f" exact_retained={len(candidates_by_variant['exact_gp_ghosts'])}"
        f" artifact_rejections={len(rejected)}"
        f" frozen_bulk_eligible={len(eligible['frozen_gauge'])}"
        f" exact_bulk_eligible={len(eligible['exact_gp_ghosts'])}"
        f" qualifying_shared_candidate={len(matches)}"
        " fixture_match_boolean_read=0"
    )
    print(f"D15_CLASSIFICATION {classification}")
    print("BLACKSTRING_D15_LOG_REANALYSIS_PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
