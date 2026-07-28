#!/usr/bin/env bash
set -euo pipefail

fixture_dir=$(cd "$(dirname "$0")" && pwd)
fixture_exe=$(
  find "$fixture_dir" -maxdepth 1 -type f \
    -name 'BlackStringLevelZeroEvolutionTest2d_ch.*.ex' -print -quit
)
if [[ -z "$fixture_exe" ]]; then
  echo "E2 fixture executable not found; build the fixture first" >&2
  exit 1
fi

fixture_tmp=$(mktemp -d /tmp/blackstring-e2.XXXXXX)
trap 'rm -rf "$fixture_tmp"' EXIT

for resolution in 32 64 128; do
  /usr/bin/time -f 'wall_seconds=%e peak_rss_kb=%M' \
    -o "$fixture_tmp/n${resolution}.time" \
    env OMP_NUM_THREADS=1 "$fixture_exe" \
    "$fixture_dir/params_n${resolution}.txt" exact \
    >"$fixture_tmp/n${resolution}.log"
  awk '/^E2_|^BLACKSTRING_/ { print }' \
    "$fixture_tmp/n${resolution}.log"
  awk -v resolution="$resolution" '
    FNR == NR {
      for (field = 1; field <= NF; ++field) {
        split($field, pair, "=")
        timing[pair[1]] = pair[2]
      }
      next
    }
    /^E2_COUNTS/ {
      for (field = 1; field <= NF; ++field) {
        split($field, pair, "=")
        count[pair[1]] = pair[2]
      }
      denominator = count["valid_cells"] * count["rhs_evaluations"]
      printf("E2_PERFORMANCE N=%s wall_seconds=%.6f peak_rss_kb=%d valid_cells=%d rhs_evaluations=%d periodic_exchanges=%d radial_ghost_fills=%d diagnostic_evaluations=%d approximate_seconds_per_cell_rhs=%.17e\n",
             resolution, timing["wall_seconds"], timing["peak_rss_kb"],
             count["valid_cells"], count["rhs_evaluations"],
             count["periodic_exchanges"], count["radial_ghost_fills"],
             count["diagnostic_evaluations"],
             timing["wall_seconds"] / denominator)
    }
  ' "$fixture_tmp/n${resolution}.time" \
    "$fixture_tmp/n${resolution}.log"
done

/usr/bin/time -f 'wall_seconds=%e peak_rss_kb=%M' \
  -o "$fixture_tmp/old-baseline.time" \
  env OMP_NUM_THREADS=1 "$fixture_exe" \
  "$fixture_dir/params_n32.txt" duplicate-exchange-baseline \
  >"$fixture_tmp/old-baseline.log"
awk '
  FNR == NR {
    for (field = 1; field <= NF; ++field) {
      split($field, pair, "=")
      timing[pair[1]] = pair[2]
    }
    next
  }
  /^E2_OLD_BASELINE/ {
    for (field = 1; field <= NF; ++field) {
      split($field, pair, "=")
      count[pair[1]] = pair[2]
    }
    denominator = count["valid_cells"] * count["rhs_evaluations"]
    printf("E2_OLD_PERFORMANCE N=32 wall_seconds=%.6f peak_rss_kb=%d valid_cells=%d rhs_evaluations=%d periodic_exchanges=%d policy_duplicate_exchanges=%d radial_ghost_fills=%d diagnostic_evaluations=%d approximate_seconds_per_cell_rhs=%.17e\n",
           timing["wall_seconds"], timing["peak_rss_kb"],
           count["valid_cells"], count["rhs_evaluations"],
           count["periodic_exchanges"],
           count["policy_duplicate_exchanges"],
           count["radial_ghost_fills"],
           count["diagnostic_evaluations"],
           timing["wall_seconds"] / denominator)
  }
' "$fixture_tmp/old-baseline.time" "$fixture_tmp/old-baseline.log"

for mutation in missing wrong-coordinate; do
  OMP_NUM_THREADS=1 "$fixture_exe" "$fixture_dir/params_n32.txt" \
    "$mutation" >"$fixture_tmp/${mutation}.log"
  awk '/^E2_INITIAL_RHS|^E2_MUTATION/ { print }' \
    "$fixture_tmp/${mutation}.log"
done

OMP_NUM_THREADS=1 "$fixture_exe" "$fixture_dir/params_n32.txt" \
  cadence-disabled >"$fixture_tmp/cadence-disabled.log"
awk '/^E2_DIAGNOSTIC_CADENCE|^BLACKSTRING_LEVEL_ZERO_CADENCE/ { print }' \
  "$fixture_tmp/cadence-disabled.log"

extract_summary()
{
  local file=$1
  local key=$2
  awk -v wanted="$key" '
    /^E2_SUMMARY/ {
      for (field = 1; field <= NF; ++field) {
        split($field, pair, "=")
        if (pair[1] == wanted) {
          print pair[2]
          exit
        }
      }
    }
  ' "$file"
}

extract_constraint()
{
  local file=$1
  local key=$2
  awk -v wanted="$key" '
    /^E2_CONSTRAINT/ {
      for (field = 1; field <= NF; ++field) {
        split($field, pair, "=")
        if (pair[1] == wanted) {
          print pair[2]
          exit
        }
      }
    }
  ' "$file"
}

state32=$(extract_summary "$fixture_tmp/n32.log" state_max)
state64=$(extract_summary "$fixture_tmp/n64.log" state_max)
state128=$(extract_summary "$fixture_tmp/n128.log" state_max)
lapse32=$(extract_summary "$fixture_tmp/n32.log" lapse_drift)
lapse64=$(extract_summary "$fixture_tmp/n64.log" lapse_drift)
lapse128=$(extract_summary "$fixture_tmp/n128.log" lapse_drift)
h32=$(extract_constraint "$fixture_tmp/n32.log" H)
h64=$(extract_constraint "$fixture_tmp/n64.log" H)
h128=$(extract_constraint "$fixture_tmp/n128.log" H)
mx32=$(extract_constraint "$fixture_tmp/n32.log" Mx)
mx64=$(extract_constraint "$fixture_tmp/n64.log" Mx)
mx128=$(extract_constraint "$fixture_tmp/n128.log" Mx)

awk -v state32="$state32" -v state64="$state64" \
    -v state128="$state128" -v lapse32="$lapse32" \
    -v lapse64="$lapse64" -v lapse128="$lapse128" -v h32="$h32" \
    -v h64="$h64" -v h128="$h128" -v mx32="$mx32" -v mx64="$mx64" \
    -v mx128="$mx128" '
  BEGIN {
    state_order_32_64 = log(state32 / state64) / log(2.0)
    state_order_64_128 = log(state64 / state128) / log(2.0)
    lapse_order_32_64 = log(lapse32 / lapse64) / log(2.0)
    lapse_order_64_128 = log(lapse64 / lapse128) / log(2.0)
    h_order_32_64 = log(h32 / h64) / log(2.0)
    h_order_64_128 = log(h64 / h128) / log(2.0)
    mx_order_32_64 = log(mx32 / mx64) / log(2.0)
    mx_order_64_128 = log(mx64 / mx128) / log(2.0)
    printf("E2_MATCHED_DOMAIN_CONVERGENCE N1=32 N2=8 state_max=%.17e lapse=%.17e H=%.17e Mx=%.17e\n",
           state32, lapse32, h32, mx32)
    printf("E2_MATCHED_DOMAIN_CONVERGENCE N1=64 N2=16 state_max=%.17e order=%.8f lapse=%.17e lapse_order=%.8f H=%.17e H_order=%.8f Mx=%.17e Mx_order=%.8f\n",
           state64, state_order_32_64, lapse64, lapse_order_32_64,
           h64, h_order_32_64, mx64, mx_order_32_64)
    printf("E2_MATCHED_DOMAIN_CONVERGENCE N1=128 N2=32 state_max=%.17e order=%.8f lapse=%.17e lapse_order=%.8f H=%.17e H_order=%.8f Mx=%.17e Mx_order=%.8f\n",
           state128, state_order_64_128, lapse128, lapse_order_64_128,
           h128, h_order_64_128, mx128, mx_order_64_128)
    if (!(state32 > state64 && state64 > state128 &&
          lapse32 > lapse64 && lapse64 > lapse128 &&
          h32 > h64 && h64 > h128 &&
          mx32 > mx64 && mx64 > mx128 &&
          state_order_32_64 > 3.5 && state_order_64_128 > 3.5 &&
          lapse_order_32_64 > 3.5 && lapse_order_64_128 > 3.5 &&
          mx_order_32_64 > 3.5 && mx_order_64_128 > 3.5)) {
      exit 1
    }
  }
'

echo "BLACKSTRING_LEVEL_ZERO_EVOLUTION_FIXTURE=PASS"
