#!/usr/bin/env bash
set -euo pipefail

fixture_dir=$(cd "$(dirname "$0")" && pwd)
cycle1_dir=$(cd "$fixture_dir/../chombo_m2b_cycle1" && pwd)
boundary_exe=$(
  rg --files --hidden --no-ignore "$fixture_dir" \
    -g 'BlackStringPerturbativeRadialBoundaryTest2d_ch.*.ex' |
    head -n 1
)
cycle1_exe=$(
  rg --files --hidden --no-ignore "$cycle1_dir" \
    -g 'BlackStringM2BCycle1Test2d_ch.*.ex' |
    head -n 1
)
if [[ -z "$boundary_exe" || -z "$cycle1_exe" ]]; then
  echo "outer-face qualification executables not found" >&2
  exit 1
fi

if [[ $# -gt 1 ]]; then
  echo "usage: run_outer_face_generalization_fixture.sh [evidence-directory]" >&2
  exit 1
fi
if [[ $# -eq 1 ]]; then
  evidence_dir=$1
  mkdir -p "$evidence_dir"
  retain=1
else
  evidence_dir=$(mktemp -d /tmp/blackstring-outer-face.XXXXXX)
  retain=0
fi
if [[ "$retain" -eq 0 ]]; then
  trap 'rm -rf "$evidence_dir"' EXIT
fi

env OMP_NUM_THREADS=1 mpirun -np 2 "$cycle1_exe" \
  >"$evidence_dir/focused-gates.log" 2>&1
rg '^(M2B_TRANSFORM ratio=3.5|M2B_MPI_OWNERSHIP|M2B_.*=PASS|BLACKSTRING_)' \
  "$evidence_dir/focused-gates.log"

for unauthorized in 30 40 50; do
  set +e
  env OMP_NUM_THREADS=1 "$boundary_exe" \
    "$fixture_dir/params_invalid_x${unauthorized}.txt" gp \
    >"$evidence_dir/reject-x${unauthorized}.log" 2>&1
  status=$?
  set -e
  if [[ "$status" -eq 0 ]]; then
    echo "unauthorized x_out=${unauthorized} unexpectedly passed" >&2
    exit 1
  fi
  rg -q 'supported x/r_0 domain \[0.5,3.5\] or \[0.5,4.5\]' \
    "$evidence_dir/reject-x${unauthorized}.log"
  echo "M2B_OUTER_FACE_REJECT x_out=${unauthorized:0:1}.${unauthorized:1:1} status=$status"
done

env OMP_NUM_THREADS=1 "$boundary_exe" "$fixture_dir/params_smoke.txt" gp \
  >"$evidence_dir/gp-x45.log" 2>&1
rg '^(BOUNDARY_LIVE|BLACKSTRING_PERTURBATIVE_BOUNDARY_LIVE_PASS)' \
  "$evidence_dir/gp-x45.log"

for resolution in medium fine; do
  env OMP_NUM_THREADS=1 mpirun -np 2 "$boundary_exe" \
    "$fixture_dir/params_gp_${resolution}_x35.txt" gp \
    >"$evidence_dir/gp-${resolution}-x35.log" 2>&1
  rg '^(BOUNDARY_LIVE|BLACKSTRING_PERTURBATIVE_BOUNDARY_LIVE_PASS)' \
    "$evidence_dir/gp-${resolution}-x35.log"
done

python3 - "$evidence_dir" <<'PY'
import json
import pathlib
import sys

root = pathlib.Path(sys.argv[1])
cases = {}
for name in ("gp-medium-x35", "gp-fine-x35"):
    line = next(
        entry
        for entry in (root / f"{name}.log").read_text(encoding="utf-8").splitlines()
        if entry.startswith("BOUNDARY_LIVE ")
    )
    values = dict(field.split("=", 1) for field in line.split()[1:])
    cases[name] = {
        key: float(values[key])
        for key in (
            "x_out",
            "state_delta",
            "radial_ghost_delta",
            "boundary_delta",
            "boundary_energy_fraction",
            "radial_nyquist_fraction",
            "determinant",
            "weighted_trace",
            "minimum_metric",
        )
    }
if any(case["x_out"] != 3.5 for case in cases.values()):
    raise SystemExit("3.5 qualification did not use the authorized face")
medium = cases["gp-medium-x35"]
fine = cases["gp-fine-x35"]
for metric in (
    "state_delta",
    "radial_ghost_delta",
    "boundary_delta",
    "radial_nyquist_fraction",
):
    if fine[metric] > medium[metric]:
        raise SystemExit(f"fine GP did not improve {metric}")
if min(case["minimum_metric"] for case in cases.values()) <= 0.0:
    raise SystemExit("3.5 qualification produced an inadmissible metric")
if max(case["radial_nyquist_fraction"] for case in cases.values()) >= 0.2:
    raise SystemExit("3.5 qualification exceeded the radial Nyquist gate")
(root / "comparison.json").write_text(
    json.dumps(cases, indent=2, sort_keys=True) + "\n", encoding="utf-8"
)
print("M2B_OUTER_FACE_GP_COMPARISON " + json.dumps(cases, sort_keys=True))
PY

echo "M2B_OUTER_FACE_GENERALIZATION=PASS"
