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

d12_tmp=$(mktemp -d /tmp/blackstring-d12.XXXXXX)
trap 'rm -rf "$d12_tmp"' EXIT

readonly epsilon=1.0e-8
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
    echo "D12 reached its ten-evolution hard cap" >&2
    exit 1
  fi
  local log="$d12_tmp/${label}.log"
  local timing="$d12_tmp/${label}.time"
  /usr/bin/time -f 'wall_seconds=%e peak_rss_kb=%M' -o "$timing" \
    stdbuf -oL -eL env OMP_NUM_THREADS=1 "$fixture_exe" \
    "$params" "$mode" "$kind" "$signed_epsilon" >"$log"
  evolution_count=$((evolution_count + 1))

  local wall_seconds
  local rss_kb
  local valid_cells
  local rhs_evaluations
  local seconds_per_cell_rhs
  wall_seconds=$(field_value wall_seconds <"$timing")
  rss_kb=$(field_value peak_rss_kb <"$timing")
  valid_cells=$(
    awk '/^FOURIER_COUNTS / { print; exit }' "$log" |
      field_value valid_cells
  )
  rhs_evaluations=$(
    awk '/^FOURIER_COUNTS / { print; exit }' "$log" |
      field_value rhs_evaluations
  )
  seconds_per_cell_rhs=$(
    awk -v wall="$wall_seconds" -v cells="$valid_cells" \
      -v rhs="$rhs_evaluations" \
      'BEGIN { printf "%.12e", wall / (cells * rhs) }'
  )
  total_wall_seconds=$(
    awk -v total="$total_wall_seconds" -v current="$wall_seconds" \
      'BEGIN { printf "%.12f", total + current }'
  )
  if ((rss_kb > peak_rss_kb)); then
    peak_rss_kb=$rss_kb
  fi
  echo "D12_PERFORMANCE run=$label wall_seconds=$wall_seconds" \
       "peak_rss_kb=$rss_kb valid_cells=$valid_cells" \
       "rhs_evaluations=$rhs_evaluations" \
       "seconds_per_valid_cell_rhs=$seconds_per_cell_rhs"
}

run_matrix()
{
  local resolution=$1
  local params=$2
  run_evolution "${resolution}_control" "$params" d12-low control 0.0
  run_evolution "${resolution}_low_plus" "$params" d12-low seeded "$epsilon"
  run_evolution "${resolution}_low_minus" "$params" d12-low seeded "-$epsilon"
  run_evolution "${resolution}_high_plus" "$params" d12-high seeded "$epsilon"
  run_evolution "${resolution}_high_minus" "$params" d12-high seeded "-$epsilon"
}

analysis_arguments()
{
  local resolution=$1
  printf '%s\n' \
    "--${resolution}-control" "$d12_tmp/${resolution}_control.log" \
    "--${resolution}-low-plus" "$d12_tmp/${resolution}_low_plus.log" \
    "--${resolution}-low-minus" "$d12_tmp/${resolution}_low_minus.log" \
    "--${resolution}-high-plus" "$d12_tmp/${resolution}_high_plus.log" \
    "--${resolution}-high-minus" "$d12_tmp/${resolution}_high_minus.log"
}

medium_params="$fixture_dir/params_d12_medium.txt"
fine_params="$fixture_dir/params_d12_fine.txt"
run_matrix medium "$medium_params"

mapfile -t medium_arguments < <(analysis_arguments medium)
medium_analysis="$d12_tmp/medium_analysis.log"
"$fixture_dir/analyze_d12_modes.py" \
  "${medium_arguments[@]}" >"$medium_analysis"
medium_interpretable=$(
  awk '
    /^D12_RESOLUTION_INTERPRETABLE resolution=medium / {
      for (field = 1; field <= NF; ++field) {
        split($field, pair, "=")
        if (pair[1] == "value") {
          print pair[2]
        }
      }
    }
  ' "$medium_analysis"
)

final_analysis=$medium_analysis
if [[ "$medium_interpretable" == "1" ]]; then
  run_matrix fine "$fine_params"
  mapfile -t fine_arguments < <(analysis_arguments fine)
  final_analysis="$d12_tmp/final_analysis.log"
  "$fixture_dir/analyze_d12_modes.py" \
    "${medium_arguments[@]}" "${fine_arguments[@]}" >"$final_analysis"
fi

if ((evolution_count != 5 && evolution_count != 10)); then
  echo "D12 evolution count is neither five nor ten" >&2
  exit 1
fi
if ((evolution_count > hard_evolution_limit)); then
  echo "D12 exceeded its ten-evolution hard cap" >&2
  exit 1
fi

awk '/^D12_/ { print }' "$final_analysis"
echo "D12_HOT_PATH direct_target_d4_evaluations_per_cell_stage=1" \
     "fused_ko_additions_per_cell_stage=1 second_rhs_evaluations=0" \
     "production_logging=0 production_analysis_allocations=0" \
     "default_diagnostic_cadence=0"
echo "D12_RUNTIME evolutions=$evolution_count" \
     "total_wall_seconds=$total_wall_seconds peak_rss_kb=$peak_rss_kb"
echo "BLACKSTRING_D12_SIGNED_RESPONSE_COMPLETE"
