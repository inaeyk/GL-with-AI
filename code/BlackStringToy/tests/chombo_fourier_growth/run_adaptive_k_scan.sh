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

scan_tmp=$(mktemp -d /tmp/blackstring-k-scan.XXXXXX)
trap 'rm -rf "$scan_tmp"' EXIT

readonly pi=3.141592653589793238462643383279502884
readonly medium_dx=0.125
readonly screening_stop=0.8
readonly linearity_tolerance=5.0e-4
readonly leakage_ratio_minimum=1.0e6
readonly hard_evolution_limit=24
readonly endpoint_validation_evolutions=10

echo "SCAN_INTERPRETATION status=INCONCLUSIVE_TRANSIENT physical_stability_classification=DISABLED"

evolution_count=0
total_wall_seconds=0.0
peak_rss_kb=0
last_log=

declare -a candidate_order=()
declare -A candidate_seen=()
declare -A candidate_sign=()
declare -A candidate_nx=()
declare -A candidate_nz=()
declare -A candidate_dx=()
declare -A candidate_x_in=()
declare -A candidate_requested_lz=()
declare -A candidate_actual_lz=()
declare -A candidate_actual_k=()
declare -A candidate_stop=()
declare -A candidate_control_log=()
declare -A candidate_seed_log=()

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

make_params()
{
  local radial_cells=$1
  local compact_cells=$2
  local compact_length=$3
  local stop_time=$4
  local radial_minimum=$5
  local destination=$6
  awk -v radial_cells="$radial_cells" \
      -v compact_cells="$compact_cells" \
      -v compact_length="$compact_length" \
      -v stop_time="$stop_time" \
      -v radial_minimum="$radial_minimum" '
    /^N1[[:space:]]*=/ {
      print "N1 = " radial_cells
      next
    }
    /^N2[[:space:]]*=/ {
      print "N2 = " compact_cells
      next
    }
    /^L[[:space:]]*=/ {
      print "L = " compact_length
      next
    }
    /^stop_time[[:space:]]*=/ {
      print "stop_time = " stop_time
      next
    }
    /^max_steps[[:space:]]*=/ {
      print "max_steps = 600"
      next
    }
    /^block_factor[[:space:]]*=/ {
      print "block_factor = 4"
      next
    }
    /^black_string_coordinate_minimum[[:space:]]*=/ {
      print "black_string_coordinate_minimum = " radial_minimum " 0.0"
      next
    }
    { print }
  ' "$fixture_dir/params_x45_n32.txt" >"$destination"
}

run_evolution()
{
  local params=$1
  local kind=$2
  local epsilon=$3
  local label=$4
  if ((evolution_count >= hard_evolution_limit)); then
    echo "adaptive scan reached the hard evolution limit" >&2
    exit 1
  fi
  local log="$scan_tmp/${label}.log"
  local timing="$scan_tmp/${label}.time"
  /usr/bin/time -f 'wall_seconds=%e peak_rss_kb=%M' -o "$timing" \
    env OMP_NUM_THREADS=1 "$fixture_exe" \
    "$params" scan "$kind" "$epsilon" >"$log"
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
  echo "SCAN_PERFORMANCE label=$label wall_seconds=$wall_seconds peak_rss_kb=$rss_kb"
  awk '/^FOURIER_(CASE|SAMPLE|FIT|COUNTS|PHASE_INVARIANCE)/ { print }' \
    "$log"
  last_log=$log
}

classify_seeded_log()
{
  local seeded_log=$1
  local control_log=$2
  awk -v ratio_minimum="$leakage_ratio_minimum" '
    function value(name,    field, pair) {
      for (field = 1; field <= NF; ++field) {
        split($field, pair, "=")
        if (pair[1] == name) {
          return pair[2]
        }
      }
      return ""
    }
    FILENAME == ARGV[1] && /^FOURIER_SAMPLE/ {
      control_amplitude = value("amplitude") + 0.0
      next
    }
    FILENAME == ARGV[2] && /^FOURIER_SAMPLE/ {
      seeded_amplitude = value("amplitude") + 0.0
      next
    }
    FILENAME == ARGV[2] && /^FOURIER_FIT/ {
      omega = value("Omega") + 0.0
      standard_error = value("standard_error") + 0.0
      ++fits
      if (omega > 0.0) {
        ++positive
      } else if (omega < 0.0) {
        ++negative
      }
      if (omega < 0.0) {
        omega = -omega
      }
      if (!(omega >= 3.0 * standard_error)) {
        confidence = 1
      }
    }
    END {
      ratio = control_amplitude > 0.0 ? seeded_amplitude / control_amplitude : 1.0e300
      if (fits != 3 || confidence || !(ratio >= ratio_minimum)) {
        print "ambiguous"
      } else if (positive == 3) {
        print "positive"
      } else if (negative == 3) {
        print "negative"
      } else {
        print "ambiguous"
      }
    }
  ' "$control_log" "$seeded_log"
}

report_pair()
{
  local requested_k=$1
  local variant=$2
  local control_log=$3
  local seeded_log=$4
  awk -v requested_k="$requested_k" -v variant="$variant" '
    function value(name,    field, pair) {
      for (field = 1; field <= NF; ++field) {
        split($field, pair, "=")
        if (pair[1] == name) {
          return pair[2]
        }
      }
      return ""
    }
    FILENAME == ARGV[1] && /^FOURIER_SAMPLE/ {
      control_amplitude = value("amplitude") + 0.0
      control_drift = value("state_drift") + 0.0
      control_h = value("H") + 0.0
      control_mx = value("Mx") + 0.0
      control_mz = value("Mz") + 0.0
      time = value("t")
      next
    }
    FILENAME == ARGV[2] && /^FOURIER_SAMPLE/ {
      seeded_amplitude = value("amplitude") + 0.0
      seeded_drift = value("state_drift") + 0.0
      seeded_h = value("H") + 0.0
      seeded_mx = value("Mx") + 0.0
      seeded_mz = value("Mz") + 0.0
      time = value("t")
    }
    END {
      ratio = control_amplitude > 0.0 ? seeded_amplitude / control_amplitude : 1.0e300
      printf("SCAN_PAIRED requested_k=%s variant=%s t=%s leakage=%.12e seeded_to_leakage=%.12e delta_state_drift=%.12e delta_H=%.12e delta_Mx=%.12e delta_Mz=%.12e\n",
             requested_k, variant, time, control_amplitude, ratio,
             seeded_drift - control_drift, seeded_h - control_h,
             seeded_mx - control_mx, seeded_mz - control_mz)
    }
  ' "$control_log" "$seeded_log"
}

run_pair()
{
  local requested_k=$1
  local variant=$2
  local radial_cells=$3
  local compact_cells=$4
  local compact_length=$5
  local stop_time=$6
  local radial_minimum=$7
  local params="$scan_tmp/${requested_k}_${variant}.params"
  make_params "$radial_cells" "$compact_cells" "$compact_length" \
    "$stop_time" "$radial_minimum" "$params"

  run_evolution "$params" control 0.0 \
    "${requested_k}_${variant}_control"
  local control_log=$last_log
  run_evolution "$params" seeded 1.0e-9 \
    "${requested_k}_${variant}_seeded"
  local seeded_log=$last_log
  report_pair "$requested_k" "$variant" "$control_log" "$seeded_log"
  pair_control_log=$control_log
  pair_seed_log=$seeded_log
  pair_sign=$(classify_seeded_log "$seeded_log" "$control_log")
}

register_candidate()
{
  local requested_k=$1
  if [[ -n "${candidate_seen[$requested_k]:-}" ]]; then
    return
  fi
  local requested_lz
  local compact_cells
  local actual_lz
  local actual_k
  local actual_dx
  local radial_cells
  local radial_minimum
  requested_lz=$(
    awk -v pi="$pi" -v k="$requested_k" \
      'BEGIN { printf "%.15f", 2.0 * pi / k }'
  )
  compact_cells=$(
    awk -v compact_length="$requested_lz" -v dx="$medium_dx" \
      'BEGIN {
         estimate = compact_length / dx
         printf "%d", 4 * int(estimate / 4.0 + 0.5)
       }'
  )
  actual_lz=$requested_lz
  actual_dx=$(
    awk -v cells="$compact_cells" -v compact_length="$actual_lz" \
      'BEGIN { printf "%.15f", compact_length / cells }'
  )
  actual_k=$(
    awk -v pi="$pi" -v compact_length="$actual_lz" \
      'BEGIN { printf "%.15f", 2.0 * pi / compact_length }'
  )
  radial_cells=$(
    awk -v dx="$actual_dx" \
      'BEGIN { printf "%d", 4 * int((4.0 / dx) / 4.0 + 0.5) }'
  )
  radial_minimum=$(
    awk -v cells="$radial_cells" -v dx="$actual_dx" \
      'BEGIN { printf "%.15f", 4.5 - cells * dx }'
  )

  candidate_seen[$requested_k]=1
  candidate_order+=("$requested_k")
  candidate_nx[$requested_k]=$radial_cells
  candidate_nz[$requested_k]=$compact_cells
  candidate_dx[$requested_k]=$actual_dx
  candidate_x_in[$requested_k]=$radial_minimum
  candidate_requested_lz[$requested_k]=$requested_lz
  candidate_actual_lz[$requested_k]=$actual_lz
  candidate_actual_k[$requested_k]=$actual_k
  candidate_stop[$requested_k]=$screening_stop

  echo "SCAN_CONFIG pass=screen requested_k=$requested_k actual_k=$actual_k requested_Lz=$requested_lz actual_Lz=$actual_lz requested_dx=$medium_dx Nx=$radial_cells Nz=$compact_cells dx=$actual_dx dz=$actual_dx x_in=$radial_minimum x_out=4.5 CFL=0.05 final_time=$screening_stop"
  run_pair "$requested_k" screen "$radial_cells" "$compact_cells" \
    "$actual_lz" "$screening_stop" "$radial_minimum"
  candidate_control_log[$requested_k]=$pair_control_log
  candidate_seed_log[$requested_k]=$pair_seed_log
  candidate_sign[$requested_k]=$pair_sign
  echo "SCAN_SIGN requested_k=$requested_k actual_k=$actual_k fit_sign=$pair_sign physical_classification=INCONCLUSIVE_TRANSIENT final_time=$screening_stop"
}

extend_ambiguous_candidate()
{
  local requested_k=$1
  if ((evolution_count + 2 + endpoint_validation_evolutions >
       hard_evolution_limit)); then
    return
  fi
  local compact_cells=${candidate_nz[$requested_k]}
  local actual_lz=${candidate_actual_lz[$requested_k]}
  local radial_cells=${candidate_nx[$requested_k]}
  local radial_minimum=${candidate_x_in[$requested_k]}
  echo "SCAN_EXTENSION requested_k=$requested_k final_time=1.6 reason=ambiguous"
  run_pair "$requested_k" extended "$radial_cells" "$compact_cells" \
    "$actual_lz" 1.6 "$radial_minimum"
  candidate_control_log[$requested_k]=$pair_control_log
  candidate_seed_log[$requested_k]=$pair_seed_log
  candidate_sign[$requested_k]=$pair_sign
  candidate_stop[$requested_k]=1.6
  echo "SCAN_SIGN requested_k=$requested_k actual_k=${candidate_actual_k[$requested_k]} fit_sign=$pair_sign physical_classification=INCONCLUSIVE_TRANSIENT final_time=1.6"
}

bracket_low=
bracket_high=
find_adjacent_bracket()
{
  local index
  for ((index = 0; index + 1 < ${#candidate_order[@]}; ++index)); do
    local low=${candidate_order[$index]}
    local high=${candidate_order[$((index + 1))]}
    if [[ "${candidate_sign[$low]}" == positive &&
          "${candidate_sign[$high]}" == negative ]]; then
      local width
      width=$(
        awk -v low="${candidate_actual_k[$low]}" \
            -v high="${candidate_actual_k[$high]}" \
          'BEGIN { printf "%.15f", high - low }'
      )
      if awk -v width="$width" 'BEGIN { exit !(width <= 0.05) }'; then
        bracket_low=$low
        bracket_high=$high
        return 0
      fi
    fi
  done
  return 1
}

for requested_k in 0.82 0.86 0.90 0.94; do
  register_candidate "$requested_k"
done

for requested_k in "${candidate_order[@]}"; do
  if [[ "${candidate_sign[$requested_k]}" == ambiguous ]]; then
    extend_ambiguous_candidate "$requested_k"
  fi
done

if ! find_adjacent_bracket; then
  while ((evolution_count + 2 + endpoint_validation_evolutions <=
          hard_evolution_limit)); do
    first=${candidate_order[0]}
    last=${candidate_order[$((${#candidate_order[@]} - 1))]}
    next=
    if [[ "${candidate_sign[$last]}" == positive ]]; then
      next=$(awk -v current="$last" 'BEGIN { printf "%.2f", current + 0.04 }')
      if ! awk -v candidate="$next" 'BEGIN { exit !(candidate <= 1.05) }'; then
        if awk -v current="$last" 'BEGIN { exit !(current < 1.05) }'; then
          next=1.05
        else
          break
        fi
      fi
    elif [[ "${candidate_sign[$first]}" == negative ]]; then
      next=$(awk -v current="$first" 'BEGIN { printf "%.2f", current - 0.04 }')
      if ! awk -v candidate="$next" 'BEGIN { exit !(candidate >= 0.75) }'; then
        if awk -v current="$first" 'BEGIN { exit !(current > 0.75) }'; then
          next=0.75
        else
          break
        fi
      fi
    else
      break
    fi
    register_candidate "$next"
    mapfile -t candidate_order < <(
      printf '%s\n' "${candidate_order[@]}" | sort -n -u
    )
    if [[ "${candidate_sign[$next]}" == ambiguous ]]; then
      extend_ambiguous_candidate "$next"
    fi
    if find_adjacent_bracket; then
      break
    fi
  done
fi

if [[ -z "$bracket_low" || -z "$bracket_high" ]]; then
  echo "SCAN_RESULT status=AMBIGUOUS evolutions=$evolution_count hard_limit=$hard_evolution_limit"
  echo "SCAN_RUNTIME evolutions=$evolution_count total_wall_seconds=$total_wall_seconds peak_rss_kb=$peak_rss_kb"
  echo "BLACKSTRING_ADAPTIVE_K_SCAN=AMBIGUOUS"
  exit 2
fi

validate_endpoint()
{
  local requested_k=$1
  local expected_sign=$2
  local compact_cells=${candidate_nz[$requested_k]}
  local compact_length=${candidate_actual_lz[$requested_k]}
  local stop_time=${candidate_stop[$requested_k]}
  local fine_compact_cells
  local fine_dx
  local fine_radial_cells
  local fine_radial_minimum
  local far_radial_cells
  local far_radial_minimum
  fine_compact_cells=$(
    awk -v compact_length="$compact_length" \
      'BEGIN {
         estimate = compact_length * 12.0
         printf "%d", 4 * int(estimate / 4.0 + 0.5)
       }'
  )
  fine_dx=$(
    awk -v compact_length="$compact_length" -v cells="$fine_compact_cells" \
      'BEGIN { printf "%.15f", compact_length / cells }'
  )
  fine_radial_cells=$(
    awk -v dx="$fine_dx" \
      'BEGIN { printf "%d", 4 * int((4.0 / dx) / 4.0 + 0.5) }'
  )
  fine_radial_minimum=$(
    awk -v cells="$fine_radial_cells" -v dx="$fine_dx" \
      'BEGIN { printf "%.15f", 4.5 - cells * dx }'
  )
  far_radial_cells=$(
    awk -v dx="${candidate_dx[$requested_k]}" \
      'BEGIN { printf "%d", 4 * int((6.0 / dx) / 4.0 + 0.5) }'
  )
  far_radial_minimum=$(
    awk -v cells="$far_radial_cells" \
        -v dx="${candidate_dx[$requested_k]}" \
      'BEGIN { printf "%.15f", 6.5 - cells * dx }'
  )

  echo "SCAN_CONFIG pass=endpoint_fine requested_k=$requested_k actual_k=${candidate_actual_k[$requested_k]} requested_Lz=${candidate_requested_lz[$requested_k]} actual_Lz=$compact_length requested_dx=0.0833333333333333 Nx=$fine_radial_cells Nz=$fine_compact_cells dx=$fine_dx dz=$fine_dx x_in=$fine_radial_minimum x_out=4.5 CFL=0.05 final_time=$stop_time"
  run_pair "$requested_k" fine_near "$fine_radial_cells" \
    "$fine_compact_cells" "$compact_length" "$stop_time" \
    "$fine_radial_minimum"
  local fine_control=$pair_control_log
  local fine_seed=$pair_seed_log
  local fine_sign=$pair_sign

  echo "SCAN_CONFIG pass=endpoint_far requested_k=$requested_k actual_k=${candidate_actual_k[$requested_k]} requested_Lz=${candidate_requested_lz[$requested_k]} actual_Lz=$compact_length requested_dx=$medium_dx Nx=$far_radial_cells Nz=$compact_cells dx=${candidate_dx[$requested_k]} dz=${candidate_dx[$requested_k]} x_in=$far_radial_minimum x_out=6.5 CFL=0.05 final_time=$stop_time"
  run_pair "$requested_k" medium_far "$far_radial_cells" "$compact_cells" \
    "$compact_length" "$stop_time" "$far_radial_minimum"
  local far_control=$pair_control_log
  local far_seed=$pair_seed_log
  local far_sign=$pair_sign

  local fine_params="$scan_tmp/${requested_k}_fine_near.params"
  run_evolution "$fine_params" seeded 5.0e-10 \
    "${requested_k}_fine_near_half"
  local half_seed=$last_log
  local half_sign
  half_sign=$(classify_seeded_log "$half_seed" "$fine_control")

  if [[ "$fine_sign" != "$expected_sign" ||
        "$far_sign" != "$expected_sign" ||
        "$half_sign" != "$expected_sign" ]]; then
    echo "endpoint sign did not survive validation" >&2
    exit 1
  fi

  local linearity_error
  linearity_error=$(
    awk '
      function value(name,    field, pair) {
        for (field = 1; field <= NF; ++field) {
          split($field, pair, "=")
          if (pair[1] == name) {
            return pair[2]
          }
        }
        return ""
      }
      FILENAME == ARGV[1] && /^FOURIER_SAMPLE/ {
        reference[value("t")] = value("normalized_amplitude") + 0.0
        next
      }
      FILENAME == ARGV[2] && /^FOURIER_SAMPLE/ {
        time = value("t")
        denominator = reference[time]
        if (denominator < 0.0) {
          denominator = -denominator
        }
        if (denominator < 1.0e-30) {
          denominator = 1.0e-30
        }
        difference = (value("normalized_amplitude") - reference[time]) / denominator
        if (difference < 0.0) {
          difference = -difference
        }
        if (difference > maximum_difference) {
          maximum_difference = difference
        }
      }
      END { printf "%.15e", maximum_difference }
    ' "$fine_seed" "$half_seed"
  )
  if ! awk -v error="$linearity_error" -v tolerance="$linearity_tolerance" \
      'BEGIN { exit !(error <= tolerance) }'; then
    echo "endpoint amplitude linearity check failed" >&2
    exit 1
  fi
  echo "SCAN_LINEARITY requested_k=$requested_k maximum_normalized_history_relative_difference=$linearity_error tolerance=$linearity_tolerance result=PASS"

  awk -v requested_k="$requested_k" -v expected_sign="$expected_sign" '
    function value(name,    field, pair) {
      for (field = 1; field <= NF; ++field) {
        split($field, pair, "=")
        if (pair[1] == name) {
          return pair[2]
        }
      }
      return ""
    }
    /^FOURIER_FIT/ {
      key = value("window_start")
      omega[FILENAME, key] = value("Omega") + 0.0
      windows[key] = 1
    }
    END {
      for (window in windows) {
        screen = omega[ARGV[1], window]
        fine = omega[ARGV[2], window]
        far = omega[ARGV[3], window]
        sign = expected_sign == "positive" ? 1.0 : -1.0
        if (!(sign * screen > 0.0 && sign * fine > 0.0 &&
              sign * far > 0.0)) {
          print "endpoint shift crossed zero" > "/dev/stderr"
          exit 1
        }
        printf("SCAN_SHIFT requested_k=%s window_start=%s resolution_shift=%.12e boundary_shift=%.12e screen_Omega=%.12e fine_Omega=%.12e far_Omega=%.12e\n",
               requested_k, window, fine - screen, far - screen,
               screen, fine, far)
      }
    }
  ' "${candidate_seed_log[$requested_k]}" "$fine_seed" "$far_seed"
}

validate_endpoint "$bracket_low" positive
validate_endpoint "$bracket_high" negative

bracket_width=$(
  awk -v low="${candidate_actual_k[$bracket_low]}" \
      -v high="${candidate_actual_k[$bracket_high]}" \
    'BEGIN { printf "%.15f", high - low }'
)
if ! awk -v width="$bracket_width" 'BEGIN { exit !(width <= 0.05) }'; then
  echo "validated bracket is wider than 0.05" >&2
  exit 1
fi
if ((evolution_count > hard_evolution_limit)); then
  echo "adaptive scan exceeded its hard evolution limit" >&2
  exit 1
fi

echo "SCAN_BRACKET requested_k_minus=$bracket_low requested_k_plus=$bracket_high k_minus=${candidate_actual_k[$bracket_low]} k_plus=${candidate_actual_k[$bracket_high]} width=$bracket_width status=PROVISIONAL_SIGN_BRACKET"
echo "SCAN_RUNTIME evolutions=$evolution_count total_wall_seconds=$total_wall_seconds peak_rss_kb=$peak_rss_kb"
echo "BLACKSTRING_ADAPTIVE_K_SCAN=PASS"
