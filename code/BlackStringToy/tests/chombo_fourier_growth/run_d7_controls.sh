#!/usr/bin/env bash
set -euo pipefail

fixture_dir=$(cd "$(dirname "$0")" && pwd)
fixture_exe=$(
  rg --files --hidden --no-ignore "$fixture_dir" \
    -g 'BlackStringFourierGrowthTest2d_ch.*.ex' |
    head -n 1
)
if [[ -z "$fixture_exe" ]]; then
  echo "Fourier growth fixture executable not found" >&2
  exit 1
fi

d7_tmp=$(mktemp -d /tmp/blackstring-d7.XXXXXX)
trap 'rm -rf "$d7_tmp"' EXIT

evolution_count=0
total_wall_seconds=0.0
peak_rss_kb=0
exact_gp_status=0

field_value()
{
  local field_name=$1
  awk -v field_name="$field_name" '
    {
      for (field = 1; field <= NF; ++field) {
        split($field, pair, "=")
        if (pair[1] == field_name) {
          print pair[2]
          exit
        }
      }
    }
  '
}

run_control()
{
  local variant=$1
  local mode=$2
  local params=$3
  local log="$d7_tmp/${variant}.log"
  local timing="$d7_tmp/${variant}.time"
  set +e
  /usr/bin/time -f 'wall_seconds=%e peak_rss_kb=%M exit_status=%x' -o "$timing" \
    stdbuf -oL -eL env OMP_NUM_THREADS=1 "$fixture_exe" \
    "$params" "$mode" control 0.0 >"$log" 2>&1
  local status=$?
  set -e
  evolution_count=$((evolution_count + 1))

  local wall_seconds
  local rss_kb
  wall_seconds=$(field_value wall_seconds <"$timing")
  rss_kb=$(field_value peak_rss_kb <"$timing")
  total_wall_seconds=$(
    awk -v total="$total_wall_seconds" -v current="$wall_seconds" \
      'BEGIN { printf "%.12f", total + current }'
  )
  if ((rss_kb > peak_rss_kb)); then
    peak_rss_kb=$rss_kb
  fi
  echo "D7_PERFORMANCE variant=$variant wall_seconds=$wall_seconds peak_rss_kb=$rss_kb"
  if ((status != 0)); then
    echo "D7_CONTROL_FAILURE variant=$variant exit_status=$status"
  fi
  awk '/^(FOURIER_CASE|FOURIER_COUNTS|FOURIER_DRIFT|BLACKSTRING_FOURIER_GROWTH_CASE_PASS)/ { print }' \
    "$log"
  return "$status"
}

run_control exact_gp d7-exact-gp "$fixture_dir/params_d7_exact_gp.txt" ||
  exact_gp_status=$?
run_control frozen_gauge d7-frozen-gauge \
  "$fixture_dir/params_d7_frozen_gauge.txt" || true
run_control half_cfl d7-half-cfl "$fixture_dir/params_d7_half_cfl.txt" || true
run_control fine d7-fine "$fixture_dir/params_d7_fine.txt" || true

if ((evolution_count != 4)); then
  echo "D7 must execute exactly four new evolutions" >&2
  exit 1
fi

analysis_args=(
  --baseline "$fixture_dir/d6_baseline_control_drift.tsv"
  --frozen-gauge "$d7_tmp/frozen_gauge.log"
  --half-cfl "$d7_tmp/half_cfl.log"
  --fine "$d7_tmp/fine.log"
)
if ((exact_gp_status == 0)); then
  analysis_args+=(--exact-gp "$d7_tmp/exact_gp.log")
else
  analysis_args+=(--exact-gp-failure INVALID_REDUCED_METRIC)
fi
"$fixture_dir/analyze_d7_controls.py" "${analysis_args[@]}"

echo "D7_RUNTIME reused_evolutions=1 new_evolutions=$evolution_count total_wall_seconds=$total_wall_seconds peak_rss_kb=$peak_rss_kb"
echo "BLACKSTRING_D7_CONTROLS=COMPLETE_WITH_RECORDED_FAILURES"
