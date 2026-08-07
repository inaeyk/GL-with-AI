#!/usr/bin/env python3
"""Freeze and execute the bounded M2-C launch matrix without retries."""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import shutil
import subprocess
import sys
import time
from pathlib import Path
from typing import Any

BASE_M2B_COMMIT = "b5970454a649f8cbf46f694ebddcfe191b657530"
MATRIX: tuple[dict[str, Any], ...] = (
    {"id": "medium-near-gp", "nx": 24, "nz": 64, "mode": 1, "epsilon": 0.0, "outer": 3.5},
    {"id": "medium-near-low-plus", "nx": 24, "nz": 64, "mode": 1, "epsilon": 1.0e-8, "outer": 3.5},
    {"id": "medium-near-low-minus", "nx": 24, "nz": 64, "mode": 1, "epsilon": -1.0e-8, "outer": 3.5},
    {"id": "medium-near-high-plus", "nx": 24, "nz": 64, "mode": 2, "epsilon": 1.0e-8, "outer": 3.5},
    {"id": "medium-near-high-minus", "nx": 24, "nz": 64, "mode": 2, "epsilon": -1.0e-8, "outer": 3.5},
    {"id": "fine-near-gp", "nx": 36, "nz": 96, "mode": 1, "epsilon": 0.0, "outer": 3.5},
    {"id": "fine-near-low-plus", "nx": 36, "nz": 96, "mode": 1, "epsilon": 1.0e-8, "outer": 3.5},
    {"id": "fine-near-high-plus", "nx": 36, "nz": 96, "mode": 2, "epsilon": 1.0e-8, "outer": 3.5},
    {"id": "medium-far-gp", "nx": 32, "nz": 64, "mode": 1, "epsilon": 0.0, "outer": 4.5},
    {"id": "medium-far-low-plus", "nx": 32, "nz": 64, "mode": 1, "epsilon": 1.0e-8, "outer": 4.5},
    {"id": "medium-far-high-plus", "nx": 32, "nz": 64, "mode": 2, "epsilon": 1.0e-8, "outer": 4.5},
    {"id": "medium-near-low-half", "nx": 24, "nz": 64, "mode": 1, "epsilon": 5.0e-9, "outer": 3.5},
)

PRODUCTION_SOURCES = (
    "code/BlackStringToy/BlackStringReducedCharacteristics.hpp",
    "code/BlackStringToy/BlackStringAlgebraicReconstruction.hpp",
    "code/BlackStringToy/BlackStringConstraintCorrectedSeed.hpp",
    "code/BlackStringToy/BlackStringGammaZHelper.hpp",
    "code/BlackStringToy/BlackStringPerturbativeRadialBoundary.hpp",
    "code/BlackStringToy/BlackStringToyLevel.cpp",
    "code/BlackStringToy/SimulationParameters.hpp",
)
FIXTURE_SOURCES = (
    "code/BlackStringToy/tests/chombo_m2c_acceptance/M2CAcceptanceRunner.cpp",
    "code/BlackStringToy/tests/chombo_m2c_acceptance/GNUmakefile",
    "code/BlackStringToy/tests/chombo_m2c_acceptance/run_campaign.py",
    "code/BlackStringToy/tests/chombo_m2c_acceptance/analyze_m2c.py",
)
FROZEN_ENVIRONMENT = {
    "OMP_NUM_THREADS": "1",
    "OPENBLAS_NUM_THREADS": "1",
    "OMPI_MCA_rmaps_base_oversubscribe": "1",
}


def workspace() -> Path:
    return Path(__file__).resolve().parents[4]


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def write_text(path: Path, value: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_suffix(path.suffix + ".tmp")
    temporary.write_text(value, encoding="utf-8")
    temporary.replace(path)


def write_json(path: Path, value: Any) -> None:
    write_text(path, json.dumps(value, indent=2, sort_keys=True) + "\n")


def git(root: Path, *args: str) -> str:
    result = subprocess.run(
        ["git", "--no-pager", *args],
        cwd=root,
        text=True,
        capture_output=True,
        check=False,
        timeout=60,
    )
    if result.returncode != 0:
        raise RuntimeError(f"git {' '.join(args)} failed: {result.stderr}")
    return result.stdout.strip()


def safe_path(root: Path, relative: str, required_prefix: str) -> Path:
    candidate = Path(relative)
    if candidate.is_absolute() or ".." in candidate.parts:
        raise RuntimeError(f"unsafe frozen path: {relative}")
    if candidate.parts[: len(Path(required_prefix).parts)] != Path(
        required_prefix
    ).parts:
        raise RuntimeError(f"frozen path escaped {required_prefix}: {relative}")
    resolved_root = root.resolve()
    resolved = (root / candidate).resolve()
    if resolved != resolved_root and resolved_root not in resolved.parents:
        raise RuntimeError(f"frozen path escaped workspace: {relative}")
    return resolved


def safe_dependency_path(root: Path, relative: str) -> Path:
    if relative not in {"external/GRChombo", "external/Chombo"}:
        raise RuntimeError(f"unexpected dependency path: {relative}")
    candidate = Path(relative)
    if candidate.is_absolute() or ".." in candidate.parts:
        raise RuntimeError(f"unsafe dependency path: {relative}")
    path = root / candidate
    if not path.is_dir():
        raise RuntimeError(f"missing dependency: {relative}")
    return path


def expected_command(
    root: Path, executable: Path, case: dict[str, Any], mpirun: Path
) -> list[str]:
    identifier = str(case["id"])
    parameter = root / "run_manifests/m2c/parameters" / f"{identifier}.txt"
    evidence = root / "run_manifests/m2c/launches" / identifier
    return [
        "/usr/bin/time",
        "-v",
        str(mpirun),
        "-np",
        "2",
        str(executable),
        "run",
        identifier,
        str(parameter.resolve()),
        str(evidence.resolve()),
    ]


def parameter_text(case: dict[str, Any]) -> str:
    nx = int(case["nx"])
    nz = int(case["nz"])
    fine = nz == 96
    cadence = 6 if fine else 4
    steps = 1920 if fine else 1280
    seeded = float(case["epsilon"]) != 0.0
    epsilon = f"{float(case['epsilon']):.17e}"
    return f"""# Frozen M2-C launch: {case['id']}
N1 = {nx}
N2 = {nz}
L = 8.0
max_level = 0
regrid_interval = 0
max_grid_size = 32
block_factor = 4
max_spatial_derivative_order = 4
num_ghosts = 3
dt_multiplier = 0.05
stop_time = 8.0
max_steps = {steps}
checkpoint_interval = -1
plot_interval = -1
output_path = .
chk_prefix = m2c_unused_chk
plot_prefix = m2c_unused_plt
verbosity = 0
write_plot_ghosts = 0
num_plot_vars = 18
plot_vars = chi hxx hxz hzz hww K Axx Axz Azz Aww Theta GammaX GammaZ lapse shiftX shiftZ Bx Bz

isPeriodic = 0 1
lo_boundary = 0 0
hi_boundary = 0 0
vars_parity = 0 0 4 0 0 0 0 4 0 0 0 1 2 0 1 2 1 2
vars_parity_diagnostic = 0 1 2

r_0 = 1.0
black_string_coordinate_minimum = 0.5 0.0
fixed_lapse_source = 1
ko_sigma = 0.3
constraint_diagnostic_cadence = {cadence}
background_preserving_gp_radial_ghosts = 0
physical_radial_boundaries = 1
constraint_corrected_fourier_seed = {1 if seeded else 0}
fourier_seed_mode_number = {case['mode']}
fourier_seed_amplitude = {epsilon}
outer_sommerfeld_speed = 1.0

lapse_advec_coeff = 0.0
lapse_power = 1.0
lapse_coeff = 2.0
shift_Gamma_coeff = 0.75
shift_advec_coeff = 0.0
eta = 1.0
"""


def dependency_record(root: Path, relative: str) -> dict[str, Any]:
    dependency = root / relative
    return {
        "path": relative,
        "head": git(dependency, "rev-parse", "HEAD"),
        "status": git(dependency, "status", "--porcelain"),
    }


def freeze(executable: Path) -> None:
    root = workspace()
    executable = executable.resolve()
    if not executable.is_file():
        raise RuntimeError(f"missing executable: {executable}")
    raw_path = root / "tmp/m2c/raw_manifest.json"
    if raw_path.exists():
        raw = json.loads(raw_path.read_text(encoding="utf-8"))
        if int(raw.get("attempt_count", -1)) != 0:
            raise RuntimeError("cannot refreeze after a numerical attempt")

    retained = root / "run_manifests/m2c"
    parameters = retained / "parameters"
    parameters.mkdir(parents=True, exist_ok=True)
    for case in MATRIX:
        write_text(parameters / f"{case['id']}.txt", parameter_text(case))

    status_text = git(root, "status", "--porcelain=v1", "--untracked-files=all")
    write_text(retained / "provenance/git_status_before_launch.txt", status_text + "\n")
    source_hashes = {
        relative: sha256(root / relative)
        for relative in (*PRODUCTION_SOURCES, *FIXTURE_SOURCES)
    }
    executable_digest = sha256(executable)
    mpirun = Path(shutil.which("mpirun") or "/usr/bin/mpirun").resolve()
    launches: list[dict[str, Any]] = []
    for case in MATRIX:
        parameter = parameters / f"{case['id']}.txt"
        evidence = retained / "launches" / str(case["id"])
        command = expected_command(root, executable, case, mpirun)
        launches.append(
            {
                **case,
                "parameter_path": str(parameter.relative_to(root)),
                "parameter_sha256": sha256(parameter),
                "evidence_directory": str(evidence.relative_to(root)),
                "command": command,
            }
        )
    frozen = {
        "schema_version": 1,
        "base_m2b_commit": BASE_M2B_COMMIT,
        "repository_head": git(root, "rev-parse", "HEAD"),
        "branch": git(root, "branch", "--show-current"),
        "status_sha256": hashlib.sha256(status_text.encode()).hexdigest(),
        "dependencies": [
            dependency_record(root, "external/GRChombo"),
            dependency_record(root, "external/Chombo"),
        ],
        "source_sha256": source_hashes,
        "executable_path": str(executable.relative_to(root)),
        "executable_size": executable.stat().st_size,
        "executable_sha256": executable_digest,
        "environment": FROZEN_ENVIRONMENT,
        "mpirun_path": str(mpirun),
        "launches": launches,
    }
    frozen_path = retained / "frozen_campaign.json"
    write_json(frozen_path, frozen)
    write_text(retained / "frozen_campaign.sha256", sha256(frozen_path) + "  frozen_campaign.json\n")
    raw = {
        "schema_version": 1,
        "base_m2b_commit": BASE_M2B_COMMIT,
        "attempt_count": 0,
        "launches": [],
    }
    write_json(raw_path, raw)
    write_json(retained / "raw_manifest.json", raw)
    print(f"M2C_MATRIX_FROZEN launches={len(MATRIX)} executable_sha256={executable_digest}")


def evidence_records(root: Path, evidence: Path) -> list[dict[str, Any]]:
    records: list[dict[str, Any]] = []
    for path in sorted(item for item in evidence.iterdir() if item.is_file()):
        records.append(
            {
                "path": str(path.relative_to(root)),
                "size": path.stat().st_size,
                "sha256": sha256(path),
            }
        )
    if not records:
        raise RuntimeError(f"launch retained no evidence: {evidence}")
    return records


def verify_frozen(root: Path, frozen: dict[str, Any]) -> None:
    if frozen.get("schema_version") != 1 or frozen.get("base_m2b_commit") != BASE_M2B_COMMIT:
        raise RuntimeError("invalid frozen campaign identity")
    frozen_path = root / "run_manifests/m2c/frozen_campaign.json"
    digest_path = root / "run_manifests/m2c/frozen_campaign.sha256"
    expected_digest_record = f"{sha256(frozen_path)}  frozen_campaign.json\n"
    if not digest_path.is_file() or digest_path.read_text(
        encoding="utf-8"
    ) != expected_digest_record:
        raise RuntimeError("frozen campaign digest mismatch")
    if frozen.get("environment") != FROZEN_ENVIRONMENT:
        raise RuntimeError("frozen environment changed")
    if frozen.get("repository_head") != git(root, "rev-parse", "HEAD"):
        raise RuntimeError("frozen source commit changed")
    git(root, "merge-base", "--is-ancestor", BASE_M2B_COMMIT, "HEAD")
    executable = safe_path(
        root,
        str(frozen["executable_path"]),
        "code/BlackStringToy/tests/chombo_m2c_acceptance",
    )
    if (
        not executable.is_file()
        or executable.stat().st_size != int(frozen["executable_size"])
        or sha256(executable) != frozen["executable_sha256"]
    ):
        raise RuntimeError("frozen executable changed")
    if set(frozen.get("source_sha256", {})) != {
        *PRODUCTION_SOURCES,
        *FIXTURE_SOURCES,
    }:
        raise RuntimeError("frozen source set changed")
    for relative, digest in frozen["source_sha256"].items():
        source = safe_path(root, relative, relative)
        if not source.is_file() or sha256(source) != digest:
            raise RuntimeError(f"frozen source changed: {relative}")
    dependencies = frozen.get("dependencies")
    if not isinstance(dependencies, list) or [
        item.get("path") for item in dependencies
    ] != ["external/GRChombo", "external/Chombo"]:
        raise RuntimeError("frozen dependency set changed")
    for dependency in dependencies:
        path = safe_dependency_path(root, str(dependency["path"]))
        if dependency.get("status") != "" or git(path, "status", "--porcelain"):
            raise RuntimeError(f"dependency is dirty: {dependency['path']}")
        if dependency.get("head") != git(path, "rev-parse", "HEAD"):
            raise RuntimeError(f"dependency commit changed: {dependency['path']}")
    mpirun = Path(str(frozen.get("mpirun_path", "")))
    if not mpirun.is_absolute() or not mpirun.is_file():
        raise RuntimeError("invalid frozen mpirun path")
    launches = frozen.get("launches")
    if not isinstance(launches, list) or len(launches) != len(MATRIX):
        raise RuntimeError("frozen launch count changed")
    for index, (launch, case) in enumerate(zip(launches, MATRIX, strict=True)):
        identifier = str(case["id"])
        expected_fields = {
            key: case[key] for key in ("id", "nx", "nz", "mode", "epsilon", "outer")
        }
        if any(launch.get(key) != value for key, value in expected_fields.items()):
            raise RuntimeError(f"frozen launch matrix changed at {index + 1}")
        expected_parameter = f"run_manifests/m2c/parameters/{identifier}.txt"
        expected_evidence = f"run_manifests/m2c/launches/{identifier}"
        if launch.get("parameter_path") != expected_parameter or launch.get(
            "evidence_directory"
        ) != expected_evidence:
            raise RuntimeError(f"frozen launch path changed: {identifier}")
        parameter = safe_path(root, expected_parameter, "run_manifests/m2c/parameters")
        safe_path(root, expected_evidence, "run_manifests/m2c/launches")
        if (
            not parameter.is_file()
            or parameter.read_text(encoding="utf-8") != parameter_text(case)
            or sha256(parameter) != launch["parameter_sha256"]
        ):
            raise RuntimeError(f"frozen parameter changed: {identifier}")
        if launch.get("command") != expected_command(root, executable, case, mpirun):
            raise RuntimeError(f"frozen command changed: {identifier}")


def preflight_parameters() -> int:
    root = workspace()
    retained = root / "run_manifests/m2c"
    frozen = json.loads((retained / "frozen_campaign.json").read_text(encoding="utf-8"))
    verify_frozen(root, frozen)
    executable = root / str(frozen["executable_path"])
    records: list[dict[str, Any]] = []
    stdout_sections: list[str] = []
    stderr_sections: list[str] = []
    for launch in frozen["launches"]:
        parameter = root / launch["parameter_path"]
        command = [str(executable), "preflight", str(launch["id"]), str(parameter)]
        result = subprocess.run(
            command,
            cwd=root,
            env={**os.environ, **frozen["environment"]},
            text=True,
            capture_output=True,
            check=False,
        )
        stdout_sections.append(f"===== {launch['id']} =====\n{result.stdout}")
        stderr_sections.append(f"===== {launch['id']} =====\n{result.stderr}")
        records.append(
            {
                "id": launch["id"],
                "command": command,
                "exit_code": result.returncode,
                "parameter_sha256": launch["parameter_sha256"],
                "executable_sha256": frozen["executable_sha256"],
            }
        )
        if result.returncode != 0:
            break
    prelaunch = retained / "prelaunch"
    write_text(prelaunch / "parameter_preflight.stdout.log", "\n".join(stdout_sections))
    write_text(prelaunch / "parameter_preflight.stderr.log", "\n".join(stderr_sections))
    write_json(
        prelaunch / "parameter_preflight.json",
        {
            "schema_version": 1,
            "numerical_launch_count": 0,
            "all_passed": len(records) == len(MATRIX)
            and all(record["exit_code"] == 0 for record in records),
            "records": records,
        },
    )
    if len(records) != len(MATRIX) or records[-1]["exit_code"] != 0:
        return records[-1]["exit_code"] or 1
    print(f"M2C_PARAMETER_PREFLIGHT_PASS cases={len(records)} numerical_launches=0")
    return 0


def execute() -> int:
    root = workspace()
    retained = root / "run_manifests/m2c"
    frozen = json.loads((retained / "frozen_campaign.json").read_text(encoding="utf-8"))
    verify_frozen(root, frozen)
    raw_path = root / "tmp/m2c/raw_manifest.json"
    raw = json.loads(raw_path.read_text(encoding="utf-8"))
    if raw.get("schema_version") != 1 or raw.get("base_m2b_commit") != BASE_M2B_COMMIT:
        raise RuntimeError("raw launch identity changed")
    completed = int(raw["attempt_count"])
    if completed != len(raw["launches"]) or not 0 <= completed <= len(MATRIX):
        raise RuntimeError("raw launch accounting is inconsistent")
    observed_ids = tuple(str(item.get("id", "")) for item in raw["launches"])
    if observed_ids != tuple(str(case["id"]) for case in MATRIX[:completed]):
        raise RuntimeError("raw launch order changed")
    if completed and int(raw["launches"][-1]["exit_code"]) != 0:
        raise RuntimeError("the last launch failed; retry is forbidden")
    if completed >= len(MATRIX):
        print("M2C_MATRIX_ALREADY_COMPLETE")
        return 0

    environment = os.environ.copy()
    environment.update(frozen["environment"])
    for index in range(completed, len(MATRIX)):
        launch = frozen["launches"][index]
        evidence = root / launch["evidence_directory"]
        if evidence.exists() and any(evidence.iterdir()):
            raise RuntimeError(f"pre-existing launch evidence forbids a retry: {launch['id']}")
        evidence.mkdir(parents=True, exist_ok=True)
        parameter = root / launch["parameter_path"]
        shutil.copyfile(parameter, evidence / "parameters.txt")
        command_text = "environment " + " ".join(
            f"{key}={value}" for key, value in sorted(frozen["environment"].items())
        ) + "\n" + " ".join(launch["command"]) + "\n"
        write_text(evidence / "command.txt", command_text)
        started = time.time()
        with (evidence / "stdout.log").open("wb") as stdout, (evidence / "stderr.log").open("wb") as stderr:
            result = subprocess.run(
                launch["command"],
                cwd=root,
                env=environment,
                stdout=stdout,
                stderr=stderr,
                check=False,
            )
        metadata = {
            "schema_version": 1,
            "id": launch["id"],
            "ordered_attempt": index + 1,
            "command": launch["command"],
            "environment": frozen["environment"],
            "exit_code": result.returncode,
            "wall_seconds_outer": time.time() - started,
            "parameter_sha256": launch["parameter_sha256"],
            "executable_sha256": frozen["executable_sha256"],
        }
        write_json(evidence / "launch_metadata.json", metadata)
        record = {
            "id": launch["id"],
            "exit_code": result.returncode,
            "parameter_sha256": launch["parameter_sha256"],
            "executable_sha256": frozen["executable_sha256"],
            "evidence_files": evidence_records(root, evidence),
        }
        raw["launches"].append(record)
        raw["attempt_count"] = len(raw["launches"])
        write_json(raw_path, raw)
        write_json(retained / "raw_manifest.json", raw)
        print(f"M2C_ATTEMPT_RECORDED attempt={index + 1} id={launch['id']} exit={result.returncode}")
        if result.returncode != 0:
            return result.returncode or 1
    return 0


def main() -> int:
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest="command", required=True)
    freeze_parser = subparsers.add_parser("freeze")
    freeze_parser.add_argument("--executable", required=True, type=Path)
    subparsers.add_parser("preflight")
    subparsers.add_parser("run")
    args = parser.parse_args()
    try:
        if args.command == "freeze":
            freeze(args.executable)
            return 0
        if args.command == "preflight":
            return preflight_parameters()
        return execute()
    except (OSError, ValueError, KeyError, RuntimeError, subprocess.SubprocessError) as error:
        print(f"M2C_CAMPAIGN_FAILURE: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
