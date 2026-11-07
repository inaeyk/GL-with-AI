#!/usr/bin/env python3
"""Outward-rounded enclosure for the exact quadratic L10 early-Z waveform.

Run gl_eigenvalue.py first to check the exact root interval. This script
encloses its exterior mode and performs interval Riemann integration with
the exact full curved-background Volterra correction bounded analytically.
"""

import json
from pathlib import Path

from flint import arb

from gl_eigenvalue import OMEGA_HIGH, OMEGA_LOW, RIGHT, validate_branch


HERE = Path(__file__).resolve().parent
U_START, U_END = arb(-10), arb(-8)


def cut(r):
    return -r - 2 * (r - 1).log()


def x_of(r):
    return r + (r - 1).log()


def describe(ball):
    return dict(lower=str(ball.lower()), upper=str(ball.upper()))


def master_ratios(r, p, omega, k2):
    """Normalized tensor components divided by the radial Schrödinger field."""
    delta = r - 1
    f = delta / r
    d = 1 + k2 * r**3
    y = d / r
    yp = 2 * k2 * r - 1 / r**2 + y * p / f
    tt = (delta * (arb(".5") - k2 * r**3) / (r * d**2) - omega**2 / (k2 * d)) * y
    tt += (delta / (2 * k2 * r**3 * d) - 3 * delta**2 / d**2) * yp
    tr = -omega * (yp - y / (2 * r * delta)) / (k2 * d)
    angular = -y / (2 * d) - delta * yp / (k2 * d * r**2)
    return tt / f, tr, angular


def source_integrals():
    """Enclose all finite-radius source integrals and the normalized tail."""
    omega = arb(OMEGA_LOW).union(arb(OMEGA_HIGH))
    k2 = (arb.pi() / 5) ** 2
    s = 2 * omega
    p_at_two, stats, cells = validate_branch(omega, False, capture_cells=True)
    _, _, angular_at_two = master_ratios(arb(2), p_at_two, omega, k2)
    normalization = abs(2 / angular_at_two)
    log_master = normalization.log()
    contributions = []
    positive_source = True
    for r_outer, r_inner, p, reverse_dx in reversed(cells):
        assert p < 0
        after = log_master - reverse_dx * p
        if r_outer > 5:
            r = r_inner.union(r_outer)
            master2 = (2 * log_master.union(after)).exp()
            tf, tr, angular = master_ratios(r, p, omega, k2)
            tr_h_squared = 2 * ((tf - angular)**2 - tr**2) + 4 * angular**2
            b_over_f = arb(3).sqrt() * k2 * r * tr_h_squared * master2 / 16
            positive_source &= bool(b_over_f > 0)
            delta_r = r_outer - r_inner
            time_factor = (s * x_of(r)).exp()
            a_integral = time_factor * b_over_f * delta_r
            a_abs = time_factor * abs(b_over_f) * delta_r
            c_abs = (-s * (r - 1).log()).exp() * abs(b_over_f) * delta_r
            contributions.append((r_inner, r_outer, a_integral, a_abs, c_abs))
        log_master = after

    # For r>=60 the decaying master is bounded by m60*exp(-.6*(x-x60)).
    # A coarse component estimate gives |B|<=100*r^5*m^2; dr/f=dx.
    # r/r60<=exp((x-x60)/r60) then integrates the positive exponential.
    m60 = log_master.exp()
    r60 = arb(RIGHT)
    exponent = arb("1.2") - s - 5 / r60
    assert exponent > 0
    a_tail = 100 * r60**5 * m60**2 * (s * x_of(r60)).exp() / exponent
    return s, p_at_two, stats, normalization, m60, a_tail, contributions, positive_source


def energy_bound(s, a_tail, contributions):
    zero = arb(0)
    after_a, after_abs_a, after_abs_c = zero, zero, zero
    full_energy = zero
    free_energy = zero
    first_window_cell = True
    cells = 0
    minimum_news = None
    maximum_news = None
    absolute_correction = zero
    for r0, r1, delta_a, delta_abs_a, delta_abs_c in reversed(contributions):
        before_a = after_a + delta_a
        before_abs_a = after_abs_a + delta_abs_a
        before_abs_c = after_abs_c + delta_abs_c
        u_max_ray = cut(r0)
        u0 = cut(r1).max(U_START)
        u1 = u_max_ray.min(U_END)
        if u0 < u1:
            u = u0.union(u1)
            # A partial cell integral is its interval-valued integrand times
            # a fraction of the full cell width. Handle either source sign.
            cut_integral = after_a + arb(".5", ".5") * delta_a
            free_news = (s * u).exp() * (cut_integral + arb(0, a_tail.upper())) / 2

            # Use the slightly larger full wedge ending at u_max_ray for
            # the absolute free-waveform norm. Its lower cut is exactly r0.
            phi_free_abs = ((s * u_max_ray).exp() * (before_abs_a + a_tail)
                            - before_abs_c) / (2 * s)
            kappa = (2 * (r0 / (r0 - 1)).log() - 1 / r0) / 4
            assert kappa < 1 and phi_free_abs.upper() > 0
            correction = (phi_free_abs.upper() / (4 * r0**2 * (1 - kappa))).upper()
            news = free_news + arb(0, correction)
            assert free_news > 0 and news > 0
            width = u1 - u0
            full_energy += 2 * news**2 * width
            free_energy += 2 * free_news**2 * width
            absolute_correction = absolute_correction.max(correction).upper()
            if first_window_cell:
                assert u0 == U_START
                minimum_news, maximum_news = news.lower(), news.upper()
                first_window_cell = False
            else:
                minimum_news = minimum_news.min(news.lower()).lower()
                maximum_news = maximum_news.max(news.upper()).upper()
            final_u = u1
            cells += 1
        else:
            assert u0 > u1
        after_a, after_abs_a, after_abs_c = before_a, before_abs_a, before_abs_c
    assert cells > 0 and final_u == U_END
    return dict(window_cells=cells, free_K0=describe(free_energy), exact_quadratic_K0=describe(full_energy),
                uniform_news=describe(minimum_news.union(maximum_news)),
                max_news_potential_correction_upper=str(absolute_correction))


def run():
    bracket = json.loads((HERE / "gl_eigenvalue_results.json").read_text())
    assert bracket["certified_root_open_interval"] == [OMEGA_LOW, OMEGA_HIGH]
    s, p2, branch, norm, m60, a_tail, contributions, positive_source = source_integrals()
    result = dict(omega_interval=[OMEGA_LOW, OMEGA_HIGH], u_window=[-10, -8],
                  branch=branch, p_at_two=describe(p2), normalized_master_at_two_abs=describe(norm),
                  normalized_master_at_sixty_abs=describe(m60),
                  direct_news_tail_upper=str(((s * U_END).exp() * a_tail / 2).upper()),
                  positive_source_on_all_enclosed_cells=positive_source,
                  **energy_bound(s, a_tail, contributions))
    print(json.dumps(result, indent=2))
    (HERE / "early_interval_results.json").write_text(json.dumps(result, indent=2) + "\n")


if __name__ == "__main__":
    run()
