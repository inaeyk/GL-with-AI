#!/usr/bin/env python3
"""Directed real-frequency band for an explicitly completed quadratic source."""
import json

from tube_intervals import (
    ONE,
    ZERO,
    arb,
    bracket,
    cut,
    describe,
    source_integrals,
    x_of,
)


def source_bounds(s, mu, m60, a_tail, rows, u1):
    """L1, total variation in time, and L1_t L2_x bounds for baseline forcing."""
    M = ZERO
    weighted_M = ZERO
    weighted_square = ZERO
    for lo, hi, radial, _, _ in rows:
        mass = radial / 2
        M += mass.abs_upper()
        weighted_M += mass.abs_upper() * (mu * (hi - 1)).exp()
        density = (mass / (hi - lo)).abs_upper()
        weighted_square += 4 * density**2 * (1 + 9 * hi**2) * 3 * hi * (hi - lo)
    M += a_tail / 2
    r60 = arb(60)
    weighted_rate = arb("1.2") - s - 5 / r60 - mu
    assert weighted_rate > 0
    weighted_M += (50 * r60**5 * m60**2 * (s * x_of(r60) + mu * (r60 - 1)).exp()
                   / weighted_rate)
    tail_rate = arb("2.4") - 2 * s - 13 / r60
    assert tail_rate > 0
    weighted_square += (300000 * r60**13 * m60**4
                        * (2 * s * x_of(r60)).exp() / tail_rate)
    time_factor = (s * u1).exp()
    return ((2 * time_factor * M / s).upper(),
            (4 * time_factor * M).upper(),
            (time_factor * (arb.pi() * weighted_square).sqrt()).upper(),
            (2 * time_factor * weighted_M / s).upper())


def run():
    mu = arb(".20")
    w0, w1 = arb(4), arb(17) / 4
    u1 = -arb(79) / 10
    x0, T, half_width = x_of(arb(8)), arb(12), ONE / 32
    assert cut(arb(5)) > u1
    assert T - x0 - 2 * half_width > u1
    s, _, stats, _, m60, a_tail, rows, positive = source_integrals()
    assert positive
    source_l1, source_tv, source_l1_l2, weighted_l1 = source_bounds(
        s, mu, m60, a_tail, rows, u1)

    vmax = arb(27) / 256 + mu**2
    pmin = (w0**2 - vmax).sqrt()
    vxx_l1 = 2 * mu**2 / 3 + arb(38) / 15
    vx_l2_squared = 3 * (mu**4 / 12 + arb(9) / 56 + arb(16) / 90)
    eta = vxx_l1 / (4 * pmin**3) + 5 * vx_l2_squared / (16 * pmin**5)
    delta = eta.exp() - 1
    w_error = 2 * (2 * delta + delta**2)
    Wmin, Wmax = 2 - w_error, 2 + w_error
    assert Wmin > 0
    transfer_upper = (1 + delta) / (pmin.sqrt() * Wmin)
    cos_lower = (w1 * half_width).cos()
    pulse_lower = (cos_lower / w1.sqrt() - delta / pmin.sqrt()) * cos_lower / Wmax
    baseline_upper = transfer_upper * source_l1
    completion_lower = pulse_lower - baseline_upper
    completion_upper = transfer_upper * (1 + source_l1)
    assert completion_lower > baseline_upper > 0
    energy_factor = (w1**2 - w0**2) / arb.pi()
    baseline_energy_upper = energy_factor * baseline_upper**2
    completion_energy = energy_factor * bracket(completion_lower, completion_upper)**2
    pulse_energy = energy_factor * bracket(pulse_lower, transfer_upper)**2
    assert completion_energy > baseline_energy_upper

    # For all w>=4, the upper bound on w/p decreases, as do eta and w_error.
    high_constant = (1 + delta)**2 * (w0 / pmin) / Wmin**2
    bump_max = (ONE / 3).exp() / half_width
    pulse_tv = 2 * bump_max
    high_tail_coefficient_base = 2 * high_constant * source_tv**2 / arb.pi()
    high_tail_coefficient_pulsed = 2 * high_constant * (source_tv + pulse_tv)**2 / arb.pi()
    # Current normalization gives |W| >= 2 for all omega > mu.
    # The left horizon Volterra bound grows as exp(mu*(r-1)) at threshold.
    threshold_factor = (ONE / (2 * mu)).exp() / (2 * mu.sqrt())
    threshold_base = threshold_factor * weighted_l1
    threshold_pulsed = threshold_factor * (weighted_l1 + (mu * (7 + half_width)).exp())
    return {
        "mu": ".20", "omega_band": ["4", "4.25"], "cutoff_end_u": "-7.9",
        "late_pulse_center_t": "12", "late_pulse_center_r": "8", "half_width_t_x": "1/32",
        "external_Riccati_enclosure": stats,
        "baseline_source_L1_upper": str(source_l1.upper()),
        "baseline_source_time_BV_integrated_in_x_upper": str(source_tv.upper()),
        "wkb_integral_eta_upper": str(eta.upper()),
        "wronskian_abs": describe(bracket(Wmin, Wmax)),
        "outgoing_or_horizon_normalized_band_energy": {
            "baseline": describe(bracket(ZERO, baseline_energy_upper)),
            "pulse_only": describe(pulse_energy),
            "baseline_plus_pulse": describe(completion_energy),
            "strict_separation_lower_margin": str(
                (completion_energy.lower() - baseline_energy_upper.upper()).lower()),
        },
        "high_tail_bounds_for_omega_H_ge_4": {
            "baseline_coefficient_divided_by_omega_H": str(high_tail_coefficient_base.upper()),
            "pulsed_coefficient_divided_by_omega_H": str(high_tail_coefficient_pulsed.upper()),
        },
        "near_threshold_escape_bounds": {
            "formula": "amplitude_upper^2 * ((mu+delta)^2-mu^2)/pi",
            "baseline_amplitude_upper": str(threshold_base.upper()),
            "pulsed_amplitude_upper": str(threshold_pulsed.upper()),
            "example_delta": "0.000001",
            "baseline_example_energy_upper": str(
                (threshold_base**2 * ((mu + arb("1e-6"))**2 - mu**2) / arb.pi()).upper()),
            "pulsed_example_energy_upper": str(
                (threshold_pulsed**2 * ((mu + arb("1e-6"))**2 - mu**2) / arb.pi()).upper()),
        },
        "coarse_total_work_energy_upper_either_channel": {
            "baseline": str((source_l1_l2**2).upper()),
            "pulsed": str(((source_l1_l2 + bump_max.sqrt())**2).upper()),
        },
    }


if __name__ == "__main__":
    print(json.dumps(run(), indent=2))
