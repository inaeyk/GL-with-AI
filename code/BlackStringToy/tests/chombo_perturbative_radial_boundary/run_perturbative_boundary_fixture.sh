#!/usr/bin/env bash
set -euo pipefail

fixture_dir=$(cd "$(dirname "$0")" && pwd)
fixture_exe=$(
  rg --files --hidden --no-ignore "$fixture_dir" \
    -g 'BlackStringPerturbativeRadialBoundaryTest2d_ch.*.ex' |
    head -n 1
)
if [[ -z "$fixture_exe" ]]; then
  echo "perturbative boundary fixture executable not found" >&2
  exit 1
fi

fixture_tmp=$(mktemp -d /tmp/blackstring-boundary.XXXXXX)
trap 'rm -rf "$fixture_tmp"' EXIT

OMP_NUM_THREADS=1 "$fixture_exe" "$fixture_dir/params_smoke.txt" manufactured

for mode in gp scalar one-z; do
  /usr/bin/time -f 'wall_seconds=%e peak_rss_kb=%M' \
    -o "$fixture_tmp/${mode}.time" \
    env OMP_NUM_THREADS=1 "$fixture_exe" \
    "$fixture_dir/params_smoke.txt" "$mode" \
    >"$fixture_tmp/${mode}.log"
  awk '/^BOUNDARY_|^BLACKSTRING_/ { print }' "$fixture_tmp/${mode}.log"
  awk -v mode="$mode" '
    {
      for (field = 1; field <= NF; ++field) {
        split($field, pair, "=")
        timing[pair[1]] = pair[2]
      }
    }
    END {
      printf("BOUNDARY_PERFORMANCE mode=%s wall_seconds=%.6f peak_rss_kb=%d\n",
             mode, timing["wall_seconds"], timing["peak_rss_kb"])
    }
  ' "$fixture_tmp/${mode}.time"
done

set +e
{
  OMP_NUM_THREADS=1 "$fixture_exe" \
    "$fixture_dir/params_invalid_inner.txt" gp \
    >"$fixture_tmp/invalid-inner.log" 2>&1
} 2>>"$fixture_tmp/invalid-inner.log"
invalid_status=$?
set -e
if [[ "$invalid_status" -eq 0 ]]; then
  echo "invalid inner boundary unexpectedly passed" >&2
  exit 1
fi
grep -q 'physical inner radial boundary requires x_in < r_0' \
  "$fixture_tmp/invalid-inner.log"
echo "BOUNDARY_INVALID_INNER=REJECT"
echo "BLACKSTRING_PERTURBATIVE_RADIAL_BOUNDARY_FIXTURE=PASS"
