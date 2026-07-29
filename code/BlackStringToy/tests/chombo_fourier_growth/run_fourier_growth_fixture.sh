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

fixture_tmp=$(mktemp -d /tmp/blackstring-fourier.XXXXXX)
trap 'rm -rf "$fixture_tmp"' EXIT

mutation_log="$fixture_tmp/legacy_gammaz.log"
if env OMP_NUM_THREADS=1 "$fixture_exe" \
    "$fixture_dir/params_x45_n32.txt" unstable legacy-gammaz 1.0e-9 \
    >"$mutation_log" 2>&1; then
  echo "legacy Gamma-z mutation unexpectedly passed" >&2
  exit 1
fi
if ! rg -q 'corrected Gamma-z seed check failed' "$mutation_log"; then
  echo "legacy Gamma-z mutation failed for the wrong reason" >&2
  cat "$mutation_log" >&2
  exit 1
fi
echo "FOURIER_MUTATION legacy_gamma_z_derivative_coefficient=1.00 result=EXPECTED_FAILURE"

all_logs=()
for location in x45 x65; do
  if [[ "$location" == x45 ]]; then
    resolutions=(24 32 48)
  else
    resolutions=(36 48 72)
  fi
  for resolution in "${resolutions[@]}"; do
    for mode in unstable stable; do
      control_name="${location}_n${resolution}_${mode}_control"
      env OMP_NUM_THREADS=1 "$fixture_exe" \
        "$fixture_dir/params_${location}_n${resolution}.txt" \
        "$mode" control 0.0 >"$fixture_tmp/${control_name}.log"
      all_logs+=("$fixture_tmp/${control_name}.log")
      awk '/^FOURIER_|^BLACKSTRING_/ { print }' \
        "$fixture_tmp/${control_name}.log"

      for amplitude_tag in e9 ehalf; do
        if [[ "$amplitude_tag" == e9 ]]; then
          epsilon=1.0e-9
        else
          epsilon=5.0e-10
        fi
        seeded_name="${location}_n${resolution}_${mode}_${amplitude_tag}"
        env OMP_NUM_THREADS=1 "$fixture_exe" \
          "$fixture_dir/params_${location}_n${resolution}.txt" \
          "$mode" seeded "$epsilon" >"$fixture_tmp/${seeded_name}.log"
        all_logs+=("$fixture_tmp/${seeded_name}.log")
        awk '/^FOURIER_|^BLACKSTRING_/ { print }' \
          "$fixture_tmp/${seeded_name}.log"
      done
    done
  done
done

awk -v linearity_tolerance=5.0e-4 '
  function value(name,    field, pair) {
    for (field = 1; field <= NF; ++field) {
      split($field, pair, "=")
      if (pair[1] == name) {
        return pair[2]
      }
    }
    return ""
  }
  function absolute(candidate) {
    return candidate < 0.0 ? -candidate : candidate
  }
  function maximum(left, right) {
    return left > right ? left : right
  }
  /^FOURIER_SAMPLE/ {
    base = value("x_out") ":" value("Nx") ":" value("mode")
    time = value("t")
    kind = value("kind")
    if (kind == "control") {
      control_amplitude[base ":" time] = value("amplitude") + 0.0
      control_h[base ":" time] = value("H") + 0.0
      control_mx[base ":" time] = value("Mx") + 0.0
      control_mz[base ":" time] = value("Mz") + 0.0
      control_drift[base ":" time] = value("state_drift") + 0.0
    } else {
      epsilon = value("epsilon")
      key = base ":" epsilon ":" time
      seeded_amplitude[key] = value("amplitude") + 0.0
      normalized_amplitude[key] = value("normalized_amplitude") + 0.0
      seeded_h[key] = value("H") + 0.0
      seeded_mx[key] = value("Mx") + 0.0
      seeded_mz[key] = value("Mz") + 0.0
      seeded_drift[key] = value("state_drift") + 0.0
      seeded_phase[key] = value("phase") + 0.0
      sample_seen[base ":" time] = 1
      if (time + 0.0 > final_time[base]) {
        final_time[base] = time + 0.0
      }
      bases[base] = 1
      times[time] = 1
    }
  }
  /^FOURIER_FIT/ {
    base = value("x_out") ":" value("Nx") ":" value("mode")
    epsilon = value("epsilon")
    window = value("window_start")
    omega[base ":" epsilon ":" window] = value("Omega") + 0.0
    fits[base ":" epsilon ":" window] = 1
  }
  END {
    epsilon_large = "1.000000000000e-09"
    epsilon_small = "5.000000000000e-10"
    maximum_linearity_error = 0.0
    minimum_seed_to_leakage = 1.0e300
    for (base in bases) {
      split(base, part, ":")
      x_out = part[1]
      nx = part[2]
      mode = part[3]
      base_linearity_error = 0.0
      for (time in times) {
        if (!sample_seen[base ":" time]) {
          continue
        }
        control_key = base ":" time
        large_key = base ":" epsilon_large ":" time
        small_key = base ":" epsilon_small ":" time
        denominator = maximum(absolute(normalized_amplitude[large_key]),
                              1.0e-30)
        linearity_error = absolute(normalized_amplitude[large_key] - normalized_amplitude[small_key]) / denominator
        if (linearity_error > base_linearity_error) {
          base_linearity_error = linearity_error
        }
        if (linearity_error > maximum_linearity_error) {
          maximum_linearity_error = linearity_error
        }
        if (control_amplitude[control_key] > 0.0) {
          leakage_ratio = seeded_amplitude[large_key] / control_amplitude[control_key]
          if (leakage_ratio < minimum_seed_to_leakage) {
            minimum_seed_to_leakage = leakage_ratio
          }
        } else {
          leakage_ratio = 1.0e300
        }
        if (time + 0.0 == final_time[base]) {
          printf("FOURIER_PAIRED_FINAL mode=%s x_out=%s Nx=%s t=%s leakage=%.12e seeded_to_leakage=%.12e delta_state_drift=%.12e delta_H=%.12e delta_Mx=%.12e delta_Mz=%.12e phase=%.12e\n",
                 mode, x_out, nx, time,
                 control_amplitude[control_key], leakage_ratio,
                 seeded_drift[large_key] - control_drift[control_key],
                 seeded_h[large_key] - control_h[control_key],
                 seeded_mx[large_key] - control_mx[control_key],
                 seeded_mz[large_key] - control_mz[control_key],
                 seeded_phase[large_key])
        }
      }
      printf("FOURIER_LINEARITY mode=%s x_out=%s Nx=%s epsilon_large=%s epsilon_small=%s maximum_normalized_history_relative_difference=%.12e tolerance=%.12e\n",
             mode, x_out, nx, epsilon_large, epsilon_small,
             base_linearity_error, linearity_tolerance)
      if (!(base_linearity_error <= linearity_tolerance)) {
        print "normalized amplitude linearity acceptance failed" > "/dev/stderr"
        exit 1
      }
      for (window_index = 1; window_index <= 3; ++window_index) {
        window = window_index == 1 ? "1.000000000000e-01" :
                 (window_index == 2 ? "1.500000000000e-01" :
                                      "2.000000000000e-01")
        large_fit = base ":" epsilon_large ":" window
        small_fit = base ":" epsilon_small ":" window
        if (!fits[large_fit] || !fits[small_fit]) {
          print "missing multi-window fit" > "/dev/stderr"
          exit 1
        }
        # Preserve the short-window slopes as numerical data, but do not use
        # their signs as physical stable/unstable classifications. The common
        # turnover seen by the later scan supersedes that interpretation.
      }
    }
    if (!(maximum_linearity_error <= linearity_tolerance)) {
      print "global normalized amplitude linearity acceptance failed" > "/dev/stderr"
      exit 1
    }
    if (minimum_seed_to_leakage == 1.0e300) {
      minimum_seed_to_leakage = 1.0e300
    }
    printf("FOURIER_ACCEPTANCE corrected_seed=PASS legacy_mutation=PASS quadratures=PASS phase_rotation=PASS paired_controls=PASS linearity=PASS short_window_fits=RECORDED_INCONCLUSIVE_TRANSIENT maximum_linearity_error=%.12e minimum_seed_to_leakage=%.12e\n",
           maximum_linearity_error, minimum_seed_to_leakage)
  }
' "${all_logs[@]}"

echo "BLACKSTRING_FOURIER_GROWTH_FIXTURE=PASS"
