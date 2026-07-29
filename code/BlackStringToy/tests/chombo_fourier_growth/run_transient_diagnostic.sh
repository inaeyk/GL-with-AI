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

diagnostic_tmp=$(mktemp -d /tmp/blackstring-transient.XXXXXX)
trap 'rm -rf "$diagnostic_tmp"' EXIT

readonly epsilon=1.0e-8
readonly half_epsilon=5.0e-9
readonly hard_evolution_limit=10

evolution_count=0
total_wall_seconds=0.0
peak_rss_kb=0

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

run_evolution()
{
  local label=$1
  local params=$2
  local mode=$3
  local kind=$4
  local signed_epsilon=$5
  if ((evolution_count >= hard_evolution_limit)); then
    echo "transient diagnostic reached the hard evolution limit" >&2
    exit 1
  fi
  local log="$diagnostic_tmp/${label}.log"
  local timing="$diagnostic_tmp/${label}.time"
  /usr/bin/time -f 'wall_seconds=%e peak_rss_kb=%M' -o "$timing" \
    env OMP_NUM_THREADS=1 "$fixture_exe" \
    "$params" "$mode" "$kind" "$signed_epsilon" >"$log"
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
  echo "TRANSIENT_PERFORMANCE label=$label wall_seconds=$wall_seconds peak_rss_kb=$rss_kb"
  awk '/^(FOURIER_CASE|FOURIER_SEED_CHECK|FOURIER_COUNTS|FOURIER_DRIFT|BLACKSTRING_FOURIER_GROWTH_CASE_PASS)/ { print }' \
    "$log"
}

conclusion_for()
{
  local analysis=$1
  local label=$2
  awk -v requested_label="$label" '
    /^TRANSIENT_MODE_RESULT/ {
      label = ""
      conclusion = ""
      for (field = 1; field <= NF; ++field) {
        split($field, pair, "=")
        if (pair[1] == "label") {
          label = pair[2]
        } else if (pair[1] == "conclusion") {
          conclusion = pair[2]
        }
      }
      if (label == requested_label) {
        answer = conclusion
      }
    }
    END { print answer }
  ' "$analysis"
}

analyze()
{
  local output=$1
  local control_low=$2
  local low_plus=$3
  local low_minus=$4
  local control_high=$5
  local high_plus=$6
  local high_minus=$7
  shift 7
  "$fixture_dir/analyze_transient_response.py" \
    --control-low "$control_low" \
    --low-plus "$low_plus" \
    --low-minus "$low_minus" \
    --control-high "$control_high" \
    --high-plus "$high_plus" \
    --high-minus "$high_minus" "$@" >"$output"
  awk '/^TRANSIENT_/ { print }' "$output"
}

params_t4="$fixture_dir/params_transient_t4.txt"
run_evolution control_t4 "$params_t4" transient-low control 0.0
run_evolution low_plus_t4 "$params_t4" transient-low seeded "$epsilon"
run_evolution low_minus_t4 "$params_t4" transient-low seeded "-$epsilon"
run_evolution high_plus_t4 "$params_t4" transient-high seeded "$epsilon"
run_evolution high_minus_t4 "$params_t4" transient-high seeded "-$epsilon"

initial_analysis="$diagnostic_tmp/initial_analysis.log"
analyze "$initial_analysis" \
  "$diagnostic_tmp/control_t4.log" \
  "$diagnostic_tmp/low_plus_t4.log" \
  "$diagnostic_tmp/low_minus_t4.log" \
  "$diagnostic_tmp/control_t4.log" \
  "$diagnostic_tmp/high_plus_t4.log" \
  "$diagnostic_tmp/high_minus_t4.log"

low_conclusion=$(conclusion_for "$initial_analysis" k_pi_over_4)
final_analysis=$initial_analysis
active_control="$diagnostic_tmp/control_t4.log"
active_low_plus="$diagnostic_tmp/low_plus_t4.log"
active_low_minus="$diagnostic_tmp/low_minus_t4.log"
active_params=$params_t4

if [[ "$low_conclusion" == "NO_MODE_PLATEAU_WITHIN_TESTED_TIME" ]]; then
  params_t8="$diagnostic_tmp/params_transient_t8.txt"
  awk '
    /^stop_time[[:space:]]*=/ {
      print "stop_time = 8.0"
      next
    }
    { print }
  ' "$params_t4" >"$params_t8"
  run_evolution control_t8 "$params_t8" transient-low control 0.0
  run_evolution low_plus_t8 "$params_t8" transient-low seeded "$epsilon"
  run_evolution low_minus_t8 "$params_t8" transient-low seeded "-$epsilon"
  active_control="$diagnostic_tmp/control_t8.log"
  active_low_plus="$diagnostic_tmp/low_plus_t8.log"
  active_low_minus="$diagnostic_tmp/low_minus_t8.log"
  active_params=$params_t8
  extended_analysis="$diagnostic_tmp/extended_analysis.log"
  analyze "$extended_analysis" \
    "$active_control" "$active_low_plus" "$active_low_minus" \
    "$diagnostic_tmp/control_t4.log" \
    "$diagnostic_tmp/high_plus_t4.log" \
    "$diagnostic_tmp/high_minus_t4.log"
  final_analysis=$extended_analysis
  low_conclusion=$(conclusion_for "$extended_analysis" k_pi_over_4)
fi

if [[ "$low_conclusion" != "NO_MODE_PLATEAU_WITHIN_TESTED_TIME" ]]; then
  run_evolution low_plus_half "$active_params" transient-low seeded \
    "$half_epsilon"
  run_evolution low_minus_half "$active_params" transient-low seeded \
    "-$half_epsilon"
  linearity_analysis="$diagnostic_tmp/linearity_analysis.log"
  analyze "$linearity_analysis" \
    "$active_control" "$active_low_plus" "$active_low_minus" \
    "$diagnostic_tmp/control_t4.log" \
    "$diagnostic_tmp/high_plus_t4.log" \
    "$diagnostic_tmp/high_minus_t4.log" \
    --low-plus-half "$diagnostic_tmp/low_plus_half.log" \
    --low-minus-half "$diagnostic_tmp/low_minus_half.log"
  final_analysis=$linearity_analysis
fi

low_conclusion=$(conclusion_for "$final_analysis" k_pi_over_4)
high_conclusion=$(conclusion_for "$final_analysis" k_pi_over_2)
if ((evolution_count > hard_evolution_limit)); then
  echo "transient diagnostic exceeded its hard evolution limit" >&2
  exit 1
fi

echo "TRANSIENT_RUNTIME evolutions=$evolution_count total_wall_seconds=$total_wall_seconds peak_rss_kb=$peak_rss_kb"
echo "TRANSIENT_FINAL low=$low_conclusion high=$high_conclusion"
echo "BLACKSTRING_TRANSIENT_DIAGNOSTIC=PASS"
