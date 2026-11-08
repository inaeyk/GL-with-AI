"""Bounded independent checks of GOAL-013. No imports from source solvers.

Decimal checks evaluate explicit majorants. The binary64 calculations are
cross-checks without interval enclosures. Run with one BLAS thread.
"""
from decimal import Decimal, getcontext
from math import pi

import numpy as np
from scipy.integrate import cumulative_trapezoid, quad, solve_ivp
from scipy.special import lambertw


WIDTH = 3 / 25
FRONT = -13 / 8 - 2 * np.log(5 / 8)
END = 8.407362
trap = np.trapezoid


def bump(y):
    inside = (y > 0) & (y < 1)
    t = np.zeros_like(y)
    t[inside] = 1 / (y[inside] * (1 - y[inside]))
    value = np.zeros_like(y)
    value[inside] = np.exp(-t[inside])
    q = 1 - 2 * y
    l1 = q * t**2
    l2 = -2 * t**2 - 2 * q**2 * t**3
    l3 = 12 * q * t**3 + 6 * q**3 * t**4
    return value, value * l1, value * (l3 + 3 * l1 * l2 + l1**3)


BUMP_NORM = quad(lambda y: float(bump(np.asarray(y))[1])**2, 0, 1,
                 epsabs=1e-14, epsrel=1e-13)[0]**0.5


def tortoise(r):
    return r + np.log(r - 1)


def radius(x):
    return 1 + lambertw(np.exp(x - 1)).real


def baseline(cells):
    # Common reciprocal-radius grid anchored at the last graph point.
    # At fixed v, other rays have x shifted by (d-u)/2. Newton's method
    # on the difference avoids exponentiating x near reciprocal radius 0.
    y = np.linspace(0, 1, cells + 1)
    u = FRONT + WIDTH * y
    graph = 1 + 2 * lambertw(np.exp(-(1 + u) / 2) / 2).real
    reference = graph[-1] + np.zeros(cells + 1)
    z = np.linspace(0, 1 / reference[-1], cells + 1)
    reference[1:] = 1 / z[1:]
    shift = (u[-1] - u[:, None]) / 2
    difference = (1 - 1 / reference) * shift
    for _ in range(4):
        difference -= (difference + np.log1p(difference / (reference - 1)) - shift
                       ) * (1 - 1 / (reference + difference))
    actual = reference + difference
    coefficient = (1 - 1 / actual) * (6 - 3 / actual) * (
        reference / actual)**2 / (2 * (1 - 1 / reference))
    coefficient[:, 0] = 3
    z_graph = 1 / radius((graph - u[-1]) / 2)
    mass = 3 / graph - 3 / (4 * graph**2)
    value, derivative, third = (jet / BUMP_NORM for jet in bump(y))

    def ray_integral(field):
        primitive = cumulative_trapezoid(coefficient * field, z, axis=1, initial=0)
        edge = np.array([np.interp(z_b, z, row)
                         for z_b, row in zip(z_graph, primitive, strict=True)])
        return np.where(z <= z_graph[:, None], edge[:, None] - primitive, 0)

    field = value[:, None] + np.zeros_like(coefficient)
    for _ in range(8):
        scatter = cumulative_trapezoid(ray_integral(field), y, axis=0, initial=0)
        field = value[:, None] - WIDTH * scatter
    news = derivative - WIDTH * ray_integral(field)[:, 0]
    diagonal = derivative - WIDTH * mass * value + WIDTH**2 / 2 * (
        cumulative_trapezoid(mass**2 * value, y, initial=0))
    print(f"baseline grid {cells} x {cells}")
    for stop in (cells, cells // 8):
        sl = slice(stop + 1)
        err = trap((news[sl] - diagonal[sl])**2, y[sl])**0.5
        norm = trap(news[sl]**2, y[sl])**0.5
        opt = 0.2805 / WIDTH**2 * trap(news[sl] * third[sl], y[sl])
        print(f"  {y[stop]:.3f}  norm {norm:.11g}  diagonal delta {err:.8g}"
              f"  optical {opt:.10g}")


def graph_check(start_offset, tolerance):
    # The horizon-regular n equation and EF metric reconstruction are
    # inherited formulas. IVP solve, normalization, and radial quadrature
    # are independent implementations. A first-order horizon seed is
    # sufficient for this empirical convergence check.
    k, omega = pi / 5, 0.06328119
    k2 = k**2
    slope = (k2 - 5 + 6 / (1 + k2)) / (1 + 2 * omega)

    def rhs(r, n):
        d = 1 + k2 * r**3
        c = k2 * r**3 + 6 * r - 11 + 18 * (r - 1) / d**2 - 6 * (4 * r - 5) / d
        return (n[1], c * n[0] / (r**2 * (r - 1))
                - (1 / r + 2 * omega * r) * n[1] / (r - 1))

    solution = solve_ivp(rhs, (1 + start_offset, 2), (1 + slope * start_offset, slope),
                         method="DOP853", rtol=tolerance, atol=tolerance / 10,
                         dense_output=True)
    assert solution.success

    def metric(r):
        n, nr = solution.sol(r)
        f, d = 1 - 1 / r, 1 + k2 * r**3
        y = d / r * n
        yr = (2 * k2 * r - 1 / r**2) * n + d / r * nr
        angular = -r**2 * y / (2 * d) - ((r - 1) * yr + r * omega * y) / (d * k2)
        tr = -omega * (f * yr + (omega - 1 / (2 * r**2)) * y) / (d * k2)
        vv = (f * (1 - 2 * k2 * r**3) / (2 * d**2) - omega**2 / (d * k2)
              + omega * (1 / (2 * k2 * r**2 * d) - 3 * r * (r - 1) / d**2)) * y
        vv += ((r - 1) / (2 * k2 * r**3 * d) - 3 * (r - 1)**2 / d**2) * yr
        vr = (tr - vv) / f
        rr = -2 * (vr + angular / r**2) / f
        return np.array([vv, vv + f * vr, vv + 2 * f * vr + f**2 * rr]), angular

    scale = 8 / (metric(2)[1] * np.exp(2 * omega))

    def graph_values(r):
        y = (-r - 2 * np.log(r - 1) - FRONT) / WIDTH
        b, derivative, _ = bump(np.asarray(y))
        f, p = 1 - 1 / r, -1 / r
        w = WIDTH**0.5 * b / BUMP_NORM
        wt = derivative / (WIDTH**0.5 * BUMP_NORM)
        at, ax = (f * w / r - wt) / (2 * r), wt / (2 * r)
        htt, htx, hxx = scale * np.exp(omega * r) / 2 * metric(r)[0]
        jt, jx = -4 * r**2 / f * np.array([htt * at - htx * ax, hxx * ax - htx * at])
        velocity = (p * jt - jx) / (1 - p**2)
        compact = k * (jt - p * jx) / (4 * r**2 * (1 - p**2))
        d = 4 + k2 * r**2
        energy = (1 - p**2) / (2 * f) * (velocity**2 / (r**2 * d)
                                         + 4 * r**4 / d * compact**2)
        return velocity, compact, energy

    jet = graph_values(1.61)
    energy = quad(lambda r: graph_values(r)[2], 1.595, 1.625,
                  epsabs=1e-10, epsrel=1e-10)[0]
    print(f"graph {start_offset:g}, {tolerance:g}:"
          f" Ut {jet[0]:.10f}, Im Azt {jet[1]:.10f}, energy {energy:.10f}")


def cap_checks():
    getcontext().prec = 55
    dec = Decimal
    a = -dec(13) / 8 - 2 * (dec(5) / 8).ln()
    length = dec("8.407362") - a
    pi_lower = dec("3.14159265358979323846264338327950288")
    for radius in map(dec, (100, 400, 500)):
        mu = length / 2 * (6 / radius - dec("1.5") / radius**2)
        amplification = (3 * length**2 / (pi_lower**2 * radius**2)).exp()
        third = mu**4 / (24**2 * (1 - mu / 25))
        hilbert_third = mu**4 / (24 * 6 * dec(56).sqrt() * (1 - mu / 20))
        forcing = 10**63 * (-dec(".4") * (radius - 4)).exp() + (
            10**55 * (radius + 6) * (-dec(".8") * (radius - 4)).exp())
        print(f"cap {radius}: amplification {amplification:.11g}, "
              f"third-order tail {third:.11g}, HS {hilbert_third:.11g}, B {forcing:.11g}")


def templates():
    s = -0.177924631 - 0.747343369j
    shifted = s + 2 * 0.06328119
    a = (0.08223 + 0.18771 - (0.42418 + 0.31707) * 1j) / 2
    b = (-0.21850 - 0.16857 - (0.44959 + 0.39966) * 1j) / 2

    def q(u):
        return 2 * (s * a * np.exp(s * u)).real

    def star(u):
        return 2 * (shifted * b * np.exp(shifted * u)).real

    print("midpoint templates:",
          f"Q norm {quad(lambda u: q(u)**2, 0, END)[0]**0.5:.10f},",
          f"Q+S norm {quad(lambda u: (q(u)+star(u))**2, 0, END)[0]**0.5:.10f},",
          f"cross {quad(lambda u: q(u)*star(u), 0, END)[0]:.10f}")


if __name__ == "__main__":
    cap_checks()
    for size in (256, 512, 1024):
        baseline(size)
    for settings in ((1e-4, 1e-9), (1e-6, 1e-11), (1e-8, 1e-12)):
        graph_check(*settings)
    templates()
