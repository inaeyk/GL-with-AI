#!/usr/bin/env python3
"""Directed finite-tube bounds for the exact quadratic exterior Z source.

The constant-mass Riemann function is exact. All Schwarzschild corrections
are bounded in the continuum. Only the inherited GL mode *candidate* comes
from floating point ODE integration; its whole-cell residual is validated.
"""
import argparse
import hashlib
import json
from math import comb, factorial
from pathlib import Path
import platform
import sys

import flint
from flint import arb
import numpy
import scipy

HERE = Path(__file__).resolve().parent
COMPUTE005 = HERE.parents[2] / "GLR-GOAL-005" / "compute"
sys.path.insert(0, str(COMPUTE005))
from early_interval import cut, describe, source_integrals, x_of
from gl_eigenvalue import OMEGA_HIGH, OMEGA_LOW

ZERO, ONE = arb(0), arb(1)


def bracket(low, high):
    return low.lower().union(high.upper())


def symmetric(radius):
    return arb(0, radius.abs_upper())


def nonnegative(upper):
    return bracket(ZERO, upper)


def build_source(groups, cutoff):
    s, _, stats, norm, m60, _, raw, positive = source_integrals()
    assert positive
    rows = []
    tail = []
    for lo, hi, radial, _, _ in raw:
        mass = radial / 2  # S(r) exp(s x) dr / (2 f)
        assert mass > 0
        if lo < cutoff:
            rows.append((lo, hi, mass, cut(lo), cut(hi)))
        else:
            tail.append((lo, hi, mass))
    grouped = []
    for first in range(0, len(rows), groups):
        group = rows[first:first + groups]
        lo, hi = group[0][0], group[-1][1]
        box = bracket(lo, hi)
        mass = sum((row[2] for row in group), ZERO)
        grouped.append((cut(box), x_of(box), mass))
    assert rows[0][0] < 5.1 < rows[-1][1] < 20 < 24
    return s, stats, norm, m60, rows, grouped, tail


def difference_bound(mu, rmax):
    """Supremum of a ray integral from any r >= 5 to rmax."""
    def primitive(r):
        return -1 / (2 * r**2) - mu**2 * (r - 1).log()

    left = arb(5)
    if mu == 0:
        return (primitive(rmax) - primitive(left)).upper() / 2
    if mu > arb(".15"):
        # r^3/(r-1) increases for r >= 5.
        assert mu**2 * left**3 > left - 1
        return (primitive(left) - primitive(rmax)).upper() / 2
    l_cross, r_cross = arb(9), arb(10)
    assert mu**2 * l_cross**3 < l_cross - 1
    assert mu**2 * r_cross**3 > r_cross - 1
    near_cross = (1 / l_cross**3 + mu**2 / (l_cross - 1)) * (r_cross - l_cross)
    integral = (primitive(l_cross) - primitive(left)
                + near_cross + primitive(r_cross) - primitive(rmax))
    return integral.upper() / 2


def tail_moments(s, m60, tail, D):
    """Upper integrals of M(r) d^p exp(D d) through infinity, for u <= -8."""
    out = []
    for power in range(4):
        finite = ZERO
        for lo, hi, mass in tail:
            d = -8 - cut(hi)
            finite += mass * d**power * (D * d).exp()
        # In the analytic source tail, d<=3r and r/60<=exp((x-x60)/60).
        # Also r<=60+(x-x60). Here M dr <= 50*r^5*m^2*exp(s*x) dx.
        r60 = arb(60)
        rate = arb("1.2") - s - (5 + power) / r60 - 3 * D
        assert rate > 0
        far = (50 * r60**5 * m60**2 * (s * x_of(r60)).exp()
               * (3 * r60)**power * (3 * D * r60).exp() / rate)
        out.append((finite + far).upper())
    return out


def bessel_kernel(t, order):
    """Enclose 0F1(order+1;-t) via its global derivative bound on t >= 0."""
    center = t.mid().max(0)
    value = (-center).hypgeom_0f1(order + 1)
    return value + symmetric((t - center).abs_upper() / (order + 1))


def kernel_derivatives(nu, t1, t2, s):
    """Upper bounds for third and fourth a derivatives of exp(-s*a) F_nu(t(a))."""
    def denom(k):
        return factorial(nu + k) // factorial(nu)

    composed = [
        ONE,
        t1 / denom(1),
        t1**2 / denom(2) + t2 / denom(1),
        t1**3 / denom(3) + 3 * t1 * t2 / denom(2),
        t1**4 / denom(4) + 6 * t1**2 * t2 / denom(3) + 3 * t2**2 / denom(2),
    ]
    return [sum((comb(j, k) * s**(j - k) * composed[k] for k in range(j + 1)), ZERO)
            for j in (3, 4)]


def kernel_integrals(d, c, s, k, max_step):
    """Enclose integrals of exp(-s*a)*(F_0, F_1, a*F_1) on a in [0,d]."""
    if k == 0:
        return (1 - (-s * d).exp()) / s, ZERO, ZERO
    # Choosing any positive even n is allowed. No float estimate enters the bound.
    n = 2 * max(1, int(float(d.upper()) / (2 * max_step)) + 1)
    h = d / n
    sums = [ZERO, ZERO, ZERO]
    for i in range(n + 1):
        a = i * h
        t = k * a * (c + a)
        weight = 1 if i in (0, n) else 4 if i % 2 else 2
        factor = weight * (-s * a).exp()
        j0 = bessel_kernel(t, 0)
        j1 = bessel_kernel(t, 1)
        sums[0] += factor * j0
        sums[1] += factor * j1
        sums[2] += factor * a * j1
    t1 = k * (c.upper() + 2 * d.upper())
    t2 = 2 * k
    _, d40 = kernel_derivatives(0, t1, t2, s)
    d31, d41 = kernel_derivatives(1, t1, t2, s)
    scale = d.upper() * h.abs_upper()**4 / 180
    remainders = (d40, d41, d.upper() * d41 + 4 * d31)
    return tuple(h * value / 3 + symmetric(scale * rem)
                 for value, rem in zip(sums, remainders))


def direct_ray(u, rows):
    """Integral of M(r) over the active graph radii through the source cutoff."""
    result = ZERO
    for _, _, mass, g_lo, g_hi in rows:
        if u < g_hi:
            continue
        result += mass if u > g_lo else nonnegative(mass)
    return result


def moment_integral(order, d, rate):
    """Integral of a^order exp(-rate*a)/order! on [0,d]."""
    z = rate * d
    truncated_exp = sum((z**j / factorial(j) for j in range(order + 1)), ZERO)
    return (1 - truncated_exp * (-z).exp()) / rate**(order + 1)


def tube(radius, mu, u_cells, max_step, source, sharper_resolvent=False):
    s, _, _, m60, rows, groups, tail = source
    r = arb(radius)
    xwall = x_of(r)
    vmax = -8 + 2 * xwall
    assert vmax > radius
    assert cut(arb(5)) > -8
    k = mu**2 / 4
    # Round the common upper domain radius upwards to a nearby integer.
    domain_r = arb(int(float(vmax.upper())) + 2)
    assert domain_r > vmax
    D = difference_bound(mu, domain_r)
    D_wall = difference_bound(mu, r)
    assert D < s
    tail_m = tail_moments(s, m60, tail, ZERO if sharper_resolvent else D)
    tail_psi = tail_m[1]
    tail_pu = tail_m[0] + k * (vmax - 5) * tail_m[1]
    tail_pv = tail_m[0] + k * tail_m[2] / 2
    integral, reference_integral = ZERO, ZERO
    max_errors = [ZERO, ZERO, ZERO]
    max_power_error = ZERO
    field_bounds = [None, None, None]
    first_power = None
    for i in range(u_cells):
        a = arb(-10) + arb(2 * i) / u_cells
        b = arb(-10) + arb(2 * (i + 1)) / u_cells
        u = bracket(a, b)
        prefactor = (s * u).exp()
        base_psi, base_pu, base_pv = ZERO, direct_ray(u, rows), ZERO
        moments = tail_m[1:].copy()
        if sharper_resolvent:
            moments = [m / factorial(j + 1) for j, m in enumerate(moments)]
        for g, x, mass in groups:
            d = (u - g).max(0)
            if not d > 0 and u < g:
                continue
            c = 2 * (xwall - x)
            assert c > 0
            H, H1, H2 = kernel_integrals(d, c, s, k, max_step)
            base_psi += mass * H
            base_pu -= k * mass * (c * H1 + H2)
            base_pv -= k * mass * H2
            d_max = d.upper()
            if sharper_resolvent:
                for j in range(3):
                    moments[j] += mass.upper() * moment_integral(j, d_max, s - D)
            else:
                weight = mass.upper() * (D * d_max).exp()
                for j in range(3):
                    moments[j] += weight * d_max**(j + 1)
        J, J1, J2 = (m * prefactor.upper() / (1 if sharper_resolvent else factorial(j + 1))
                     for j, m in enumerate(moments))
        errors = (
            D * J1,
            D_wall * J + k * (vmax - 5) * D * J1,
            (mu**2 / r + 1 / r**3) * J1 / 4 + k * D * J2,
        )
        with_tail = (
            prefactor * (base_psi + symmetric(tail_psi)),
            prefactor * (base_pu + symmetric(tail_pu)),
            prefactor * (base_pv + symmetric(tail_pv)),
        )
        psi, pu, pv = [value + symmetric(error) for value, error in zip(with_tail, errors)]
        a_wall = (1 - 1 / r) / r
        power = 2 * (pu + pv) * (pu - pv + a_wall * psi)
        base_psi, base_pu, base_pv = with_tail
        reference_power = 2 * (base_pu + base_pv) * (base_pu - base_pv + a_wall * base_psi)
        integral += power * (b - a)
        reference_integral += reference_power * (b - a)
        for j, (val, err) in enumerate(zip((psi, pu, pv), errors)):
            max_errors[j] = max_errors[j].max(err.upper()).upper()
            field_bounds[j] = val if field_bounds[j] is None else field_bounds[j].union(val)
        max_power_error = max_power_error.max((power - reference_power).abs_upper())
        if first_power is None:
            first_power = power
        last_power = power
    return {
        "r": radius, "mu": str(mu.mid()), "u_cells": u_cells,
        "delta_potential_ray_norm_upper": str(D.upper()),
        "delta_potential_tube_ray_norm_upper": str(D_wall.upper()),
        "source_tail_p0_p1_p2_p3_upper_before_time_factor": [str(t.upper()) for t in tail_m],
        "max_schwarzschild_correction_psi_pu_pv": [str(e.upper()) for e in max_errors],
        "tube_psi_pu_pv_uniform": [describe(v) for v in field_bounds],
        "first_cell_power": describe(first_power),
        "last_cell_power": describe(last_power),
        "constant_mass_reference_signed_flux": describe(reference_integral),
        "schwarzschild_signed_flux": describe(integral),
        "max_power_difference_loose_upper": str(max_power_error.upper()),
    }, integral


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--u-cells", type=int, default=128)
    parser.add_argument("--radial-group", type=int, default=6)
    parser.add_argument("--cutoff", type=int, default=18)
    parser.add_argument("--max-step", type=float, default=.5)
    parser.add_argument("--radius", nargs="+", type=int, default=[48, 24])
    parser.add_argument("--sharper-resolvent", action="store_true")
    parser.add_argument("--mu", nargs="+", default=["0", ".10", ".20"])
    args = parser.parse_args()
    assert args.u_cells > 0 and args.radial_group > 0 and args.max_step > 0
    source = build_source(args.radial_group, args.cutoff)
    files = ("early_interval.py", "gl_eigenvalue.py", "gl_eigenvalue_results.json")
    out = {
        "versions": {
            "python": platform.python_version(), "numpy": numpy.__version__,
            "scipy": scipy.__version__, "python-flint": flint.__version__,
            "arb_bits": flint.ctx.prec,
        },
        "GLR005_compute_sha256": {
            name: hashlib.sha256((COMPUTE005 / name).read_bytes()).hexdigest()
            for name in files
        },
        "certified_root_hypothesis": [OMEGA_LOW, OMEGA_HIGH],
        "external_Riccati_enclosure": source[1],
        "master_normalization": describe(source[2]),
        "master_at_60": describe(source[3]),
        "radial_group": args.radial_group,
        "source_cells": len(source[4]), "grouped_cells": len(source[5]),
        "actual_source_cutoff": str(source[4][-1][1]),
        "quadrature_requested_max_step": args.max_step,
        "radii": [],
    }
    if args.sharper_resolvent:
        out["sharper_resolvent"] = True
    for radius in args.radius:
        assert 24 <= radius <= 48
        results = [tube(radius, arb(mu), args.u_cells, args.max_step, source, args.sharper_resolvent)
                   for mu in args.mu]
        pairs = []
        for i, (row, box) in enumerate(results):
            for next_row, next_box in results[i + 1:]:
                pairs.append({"mu_pair": [row["mu"], next_row["mu"]],
                              "ordered_positive_margin": describe(box.lower() - next_box.upper()),
                              "strictly_separated_in_requested_order": bool(box > next_box)})
        out["radii"].append({"r": radius, "values": [row for row, _ in results],
                             "pairwise": pairs})
    print(json.dumps(out, indent=2))


if __name__ == "__main__":
    main()
