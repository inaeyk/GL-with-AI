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

d11_tmp=$(mktemp -d /tmp/blackstring-d11.XXXXXX)
trap 'rm -rf "$d11_tmp"' EXIT

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

run_control()
{
  local variant=$1
  local mode=$2
  local params=$3
  local log="$d11_tmp/${variant}.log"
  local timing="$d11_tmp/${variant}.time"
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
  echo "D11_RUN variant=$variant status=$status wall_seconds=$wall_seconds peak_rss_kb=$rss_kb"
  if ((status != 0 && status != 87)); then
    tail -n 80 "$log" >&2
    return "$status"
  fi
  return 0
}

control_unstable()
{
  local log=$1
  if rg -q '^D9_INVALID_METRIC ' "$log"; then
    return 0
  fi
  awk '
    /^D9_SAMPLE / {
      for (field = 1; field <= NF; ++field) {
        split($field, pair, "=")
        if (pair[1] == "state_max" && pair[2] + 0.0 >= 0.1) {
          found = 1
        }
      }
    }
    END { exit(found ? 0 : 1) }
  ' "$log"
}

run_control ref d11-ref "$fixture_dir/params_d11_ref.txt"
best_sigma=0.3
double_args=()
double_wall=0.0
double_rss=0

if control_unstable "$d11_tmp/ref.log"; then
  run_control double d11-double "$fixture_dir/params_d11_double.txt"
  best_sigma=0.6
  double_wall=$(field_value wall_seconds <"$d11_tmp/double.time")
  double_rss=$(field_value peak_rss_kb <"$d11_tmp/double.time")
  double_args=(--double "$d11_tmp/double.log")
fi

fine_params="$fixture_dir/params_d11_fine.txt"
if [[ "$best_sigma" == "0.6" ]]; then
  fine_params="$d11_tmp/params_d11_fine_sigma_0.6.txt"
  sed 's/^ko_sigma = 0\\.3$/ko_sigma = 0.6/' \
    "$fixture_dir/params_d11_fine.txt" >"$fine_params"
fi
run_control fine d11-fine "$fine_params"

if ((evolution_count > 3)); then
  echo "D11 exceeded the three-evolution hard cap" >&2
  exit 1
fi

ref_wall=$(field_value wall_seconds <"$d11_tmp/ref.time")
ref_rss=$(field_value peak_rss_kb <"$d11_tmp/ref.time")
fine_wall=$(field_value wall_seconds <"$d11_tmp/fine.time")
fine_rss=$(field_value peak_rss_kb <"$d11_tmp/fine.time")

"$fixture_dir/analyze_d11_controls.py" \
  --baseline "$fixture_dir/d6_baseline_control_drift.tsv" \
  --ref "$d11_tmp/ref.log" \
  "${double_args[@]}" \
  --fine "$d11_tmp/fine.log" \
  --best-sigma "$best_sigma" \
  --ref-wall "$ref_wall" --ref-rss "$ref_rss" \
  --double-wall "$double_wall" --double-rss "$double_rss" \
  --fine-wall "$fine_wall" --fine-rss "$fine_rss"

echo "D11_RUNTIME reused_zero_sigma_evolutions=1 new_evolutions=$evolution_count total_wall_seconds=$total_wall_seconds peak_rss_kb=$peak_rss_kb"
echo "BLACKSTRING_D11_CONTROLS_COMPLETE"
