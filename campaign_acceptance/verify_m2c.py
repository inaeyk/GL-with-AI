#!/usr/bin/env python3
from __future__ import annotations

import argparse
import fnmatch
import hashlib
import json
import math
import subprocess
import sys
from pathlib import Path
from typing import Any

BASE_M2B_COMMIT = "b5970454a649f8cbf46f694ebddcfe191b657530"

ACCEPTED = "PHYSICAL_RADIAL_AND_CONSTRAINT_STRATEGY_ACCEPTED"
PARTIAL = "PHYSICAL_RADIAL_AND_CONSTRAINT_STRATEGY_PARTIAL"
BLOCKED = "PHYSICAL_RADIAL_AND_CONSTRAINT_STRATEGY_BLOCKED"
CLASSIFICATIONS = {ACCEPTED, PARTIAL, BLOCKED}

EXPECTED_LAUNCH_IDS = (
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

ALLOWED_CHANGED_PATHS = (
    "code/BlackStringToy/BlackStringPerturbativeRadialBoundary.hpp",
    "code/BlackStringToy/BlackStringToyLevel.cpp",
    "code/BlackStringToy/SimulationParameters.hpp",
    "code/BlackStringToy/BlackStringAlgebraicReconstruction.hpp",
    "code/BlackStringToy/BlackStringConstraintCorrectedSeed.hpp",
    "code/BlackStringToy/BlackStringGammaZHelper.hpp",
    "code/BlackStringToy/BlackStringReducedCharacteristics.hpp",
    "code/BlackStringToy/tests/chombo_m2c_acceptance/**",
    "docs/grchombo/milestone2c_*.md",
    "logs/milestone2c_*.log",
    "run_manifests/m2c/**",
    "research_plan/stage_checklists.md",
    "research_plan/project_outline.md",
    "TODO.md",
    "code/BlackStringToy/README.md",
    "docs/grchombo/grchombo_adaptation_backlog.md",
)

PROTECTED_PATHS = (
    "external/**",
    "scripts/**",
    "campaign_acceptance/**",
    "docs/derivations/**",
    "docs/grchombo/milestone1*.md",
    "docs/grchombo/milestone2a*.md",
    "docs/grchombo/milestone2b*.md",
    "logs/milestone1*.log",
    "logs/milestone2a*.log",
    "logs/milestone2b*.log",
    "code/BlackStringToy/tests/chombo_m2b_cycle1/**",
)


class Failure(RuntimeError):
    pass


def require(condition: bool, message: str) -> None:
    if not condition:
        raise Failure(message)


def load_json(path: Path) -> dict[str, Any]:
    require(path.is_file(), f"missing required JSON: {path}")
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeDecodeError, json.JSONDecodeError) as exc:
        raise Failure(f"invalid JSON: {path}: {exc}") from exc
    require(isinstance(value, dict), f"JSON root must be an object: {path}")
    return value


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def git(*args: str) -> str:
    result = subprocess.run(
        ["git", "--no-pager", *args],
        cwd=Path.cwd(),
        text=True,
        capture_output=True,
        check=False,
        timeout=60,
    )
    require(result.returncode == 0, f"git {' '.join(args)} failed: {result.stderr}")
    return result.stdout.strip()


def matches(path: str, patterns: tuple[str, ...]) -> bool:
    return any(fnmatch.fnmatchcase(path, pattern) for pattern in patterns)


def common_paths() -> tuple[Path, Path, Path]:
    workspace = Path.cwd().resolve()
    summary = workspace / "run_manifests/m2c/acceptance_summary.json"
    raw_manifest = workspace / "tmp/m2c/raw_manifest.json"
    return workspace, summary, raw_manifest


def validate_accepted(value: dict[str, Any]) -> None:
    require(value.get("launch_count") == 12, "ACCEPTED requires all 12 launches")
    require(value.get("all_launches_exit_zero") is True, "one or more launches failed")
    require(value.get("gp_controls_pass") is True, "GP controls did not pass")
    require(value.get("mpi_ownership_passed") is True, "MPI ownership gate failed")
    require(value.get("scientific_acceptance_passed") is True, "acceptance flag is false")
    require(value.get("human_review_required") is False, "accepted result requests review")
    require(not value.get("failed_gates"), "accepted result contains failed gates")
    require(not value.get("stop_reason"), "accepted result contains a stop reason")

    low = value.get("low_mode")
    high = value.get("high_mode")
    performance = value.get("performance")

    require(isinstance(low, dict), "missing low_mode object")
    require(isinstance(high, dict), "missing high_mode object")
    require(isinstance(performance, dict), "missing performance object")

    require(abs(float(low.get("k", 0.0)) - math.pi / 4.0) < 1.0e-12, "wrong low k")
    require(low.get("positive_common_rate") is True, "low-k common positive rate absent")
    require(int(low.get("common_field_count", 0)) >= 4, "too few common-rate fields")
    require(
        float(low.get("rate_resolution_relative_difference", 1.0)) <= 0.20,
        "low-k rate is not resolution robust",
    )
    require(
        float(low.get("rate_boundary_relative_difference", 1.0)) <= 0.20,
        "low-k rate is not boundary robust",
    )
    require(
        float(low.get("profile_overlap_resolution", 0.0)) >= 0.99,
        "low-k profile lacks resolution overlap",
    )
    require(
        float(low.get("profile_overlap_boundary", 0.0)) >= 0.99,
        "low-k profile lacks boundary overlap",
    )
    require(
        float(low.get("half_amplitude_relative_difference", 1.0)) <= 5.0e-4,
        "low-k amplitude-linearity gate failed",
    )
    require(
        float(low.get("even_odd_ratio", 1.0)) <= 1.0e-3,
        "low-k even/odd contamination too large",
    )
    require(
        float(low.get("max_boundary_fraction", 1.0)) < 0.30,
        "low-k candidate is boundary dominated",
    )
    require(
        float(low.get("max_nyquist_fraction", 1.0)) < 0.10,
        "low-k candidate is grid-scale dominated",
    )
    require(
        float(low.get("max_parity_leakage", 1.0)) < 1.0e-5,
        "low-k parity leakage too large",
    )
    require(
        float(low.get("max_harmonic_leakage", 1.0)) < 1.0e-5,
        "low-k harmonic leakage too large",
    )
    require(low.get("constraints_controlled") is True, "low-k constraints uncontrolled")

    require(abs(float(high.get("k", 0.0)) - math.pi / 2.0) < 1.0e-12, "wrong high k")
    require(
        high.get("no_positive_common_plateau") is True,
        "high-k case has an unresolved positive common plateau",
    )
    require(high.get("separated_from_low") is True, "high-k is not distinct from low-k")
    require(high.get("constraints_controlled") is True, "high-k constraints uncontrolled")
    require(
        float(high.get("max_boundary_fraction", 1.0)) < 0.30,
        "high-k case is boundary dominated",
    )
    require(
        float(high.get("max_nyquist_fraction", 1.0)) < 0.10,
        "high-k case is grid-scale dominated",
    )
    require(
        float(high.get("max_parity_leakage", 1.0)) < 1.0e-5,
        "high-k parity leakage too large",
    )

    require(
        performance.get("status") in {"reused_m2b", "rerun_pass"},
        "invalid performance status",
    )
    require(
        float(performance.get("slowdown_fraction", 1.0)) <= 0.15,
        "production slowdown exceeds 15%",
    )
    require(
        value.get("restart_regression_status") in {"reused_m2b", "rerun_pass"},
        "restart regression is missing",
    )


def validate_nonaccepted(value: dict[str, Any], classification: str) -> None:
    require(
        value.get("scientific_acceptance_passed") is False,
        "nonaccepted result claims scientific acceptance",
    )
    require(value.get("human_review_required") is True, "human review must be required")
    require(
        isinstance(value.get("stop_reason"), str) and value["stop_reason"].strip(),
        "nonaccepted result requires a stop reason",
    )
    failed = value.get("failed_gates")
    require(
        isinstance(failed, list)
        and failed
        and all(isinstance(item, str) and item.strip() for item in failed),
        "nonaccepted result requires explicit failed gates",
    )
    if classification == PARTIAL:
        require(
            value.get("all_launches_exit_zero") is True,
            "PARTIAL is reserved for completed but scientifically insufficient runs",
        )


def validate_summary() -> None:
    _, summary_path, _ = common_paths()
    value = load_json(summary_path)

    require(value.get("schema_version") == 1, "summary schema_version must be 1")
    require(
        value.get("base_m2b_commit") == BASE_M2B_COMMIT,
        "summary base_m2b_commit mismatch",
    )

    classification = value.get("classification")
    require(classification in CLASSIFICATIONS, "invalid Milestone 2 classification")

    launch_count = value.get("launch_count")
    require(
        isinstance(launch_count, int) and 0 <= launch_count <= 12,
        "launch_count must be between 0 and 12",
    )
    launch_ids = value.get("launch_ids")
    require(isinstance(launch_ids, list), "launch_ids must be a list")
    require(
        tuple(launch_ids) == EXPECTED_LAUNCH_IDS[:launch_count],
        "launch IDs must be the frozen ordered prefix",
    )

    if classification == ACCEPTED:
        validate_accepted(value)
    else:
        validate_nonaccepted(value, classification)


def validate_retained() -> None:
    workspace, summary_path, raw_manifest_path = common_paths()
    summary = load_json(summary_path)
    raw = load_json(raw_manifest_path)

    classification = summary.get("classification")
    launch_count = int(summary.get("launch_count", -1))

    require(raw.get("schema_version") == 1, "raw manifest schema_version must be 1")
    require(
        raw.get("base_m2b_commit") == BASE_M2B_COMMIT,
        "raw manifest base commit mismatch",
    )
    require(raw.get("attempt_count") == launch_count, "raw attempt count mismatch")

    launches = raw.get("launches")
    require(isinstance(launches, list), "raw launches must be a list")
    require(len(launches) == launch_count, "raw launch count mismatch")

    observed_ids: list[str] = []
    all_exit_zero = True

    for launch in launches:
        require(isinstance(launch, dict), "launch record must be an object")
        identifier = str(launch.get("id", ""))
        observed_ids.append(identifier)

        exit_code = int(launch.get("exit_code", -1))
        all_exit_zero = all_exit_zero and exit_code == 0

        for key in ("parameter_sha256", "executable_sha256"):
            digest = str(launch.get(key, ""))
            require(len(digest) == 64, f"invalid {key} for {identifier}")

        evidence = launch.get("evidence_files")
        require(isinstance(evidence, list) and evidence, f"missing evidence: {identifier}")

        for item in evidence:
            require(isinstance(item, dict), f"bad evidence entry: {identifier}")
            relative = Path(str(item.get("path", "")))
            require(
                not relative.is_absolute() and ".." not in relative.parts,
                "unsafe evidence path",
            )
            path = workspace / relative
            require(path.is_file(), f"missing evidence file: {relative}")
            require(
                int(item.get("size", -1)) == path.stat().st_size,
                f"size mismatch: {relative}",
            )
            require(
                str(item.get("sha256", "")) == sha256(path),
                f"hash mismatch: {relative}",
            )

    require(
        tuple(observed_ids) == EXPECTED_LAUNCH_IDS[:launch_count],
        "raw launch order mismatch",
    )
    require(
        tuple(summary.get("launch_ids", ())) == tuple(observed_ids),
        "summary/raw launch mismatch",
    )
    require(
        summary.get("all_launches_exit_zero") is all_exit_zero,
        "summary/raw exit-status mismatch",
    )

    if classification in {ACCEPTED, PARTIAL}:
        require(all_exit_zero, "ACCEPTED/PARTIAL requires successful executed launches")

    analyzer = workspace / "code/BlackStringToy/tests/chombo_m2c_acceptance/analyze_m2c.py"
    require(analyzer.is_file(), "missing retained-evidence analyzer")

    result = subprocess.run(
        [
            "/usr/bin/python3",
            str(analyzer.relative_to(workspace)),
            "--summary",
            str(summary_path.relative_to(workspace)),
            "--raw-manifest",
            str(raw_manifest_path.relative_to(workspace)),
            "--verify",
        ],
        cwd=workspace,
        text=True,
        capture_output=True,
        check=False,
        timeout=600,
    )
    require(
        result.returncode == 0,
        f"retained analyzer failed:\nSTDOUT:\n{result.stdout}\nSTDERR:\n{result.stderr}",
    )


def validate_hygiene() -> None:
    workspace, _, _ = common_paths()

    git("rev-parse", "HEAD")
    git("merge-base", "--is-ancestor", BASE_M2B_COMMIT, "HEAD")

    subprocess.run(
        ["git", "diff", "--check"],
        cwd=workspace,
        check=True,
        timeout=60,
    )
    cached = subprocess.run(
        ["git", "diff", "--cached", "--quiet"],
        cwd=workspace,
        check=False,
        timeout=60,
    )
    require(cached.returncode == 0, "staged area must remain empty")

    changed = set(
        line[3:]
        for line in git(
            "status", "--porcelain=v1", "--untracked-files=all"
        ).splitlines()
        if len(line) >= 4
    )
    require(changed, "campaign produced no workspace changes")

    for path in sorted(changed):
        require(not matches(path, PROTECTED_PATHS), f"protected path changed: {path}")
        require(matches(path, ALLOWED_CHANGED_PATHS), f"out-of-scope path changed: {path}")

    for dependency, expected in (
        ("external/GRChombo", "37e659523830418b210acea1661dac0e00bb1b75"),
        ("external/Chombo", "8684f2e000106f1abadb72642e1d15351867f98f"),
    ):
        root = workspace / dependency
        require(root.is_dir(), f"missing dependency: {dependency}")

        head = subprocess.run(
            ["git", "rev-parse", "HEAD"],
            cwd=root,
            text=True,
            capture_output=True,
            check=True,
            timeout=30,
        ).stdout.strip()
        require(head == expected, f"dependency commit changed: {dependency}")

        status = subprocess.run(
            ["git", "status", "--porcelain"],
            cwd=root,
            text=True,
            capture_output=True,
            check=True,
            timeout=30,
        ).stdout.strip()
        require(not status, f"dependency worktree dirty: {dependency}")

    for required in (
        "docs/grchombo/milestone2c_strategy_closure.md",
        "logs/milestone2c_strategy_closure.log",
        "run_manifests/m2c/acceptance_summary.json",
    ):
        require((workspace / required).is_file(), f"missing deliverable: {required}")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--mode",
        choices=("summary", "retained", "hygiene"),
        required=True,
    )
    args = parser.parse_args()

    try:
        if args.mode == "summary":
            validate_summary()
        elif args.mode == "retained":
            validate_retained()
        else:
            validate_hygiene()
    except (Failure, OSError, subprocess.SubprocessError, ValueError) as exc:
        print(f"M2C_ACCEPTANCE_FAIL: {exc}", file=sys.stderr)
        return 1

    print(f"M2C_ACCEPTANCE_PASS mode={args.mode}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
