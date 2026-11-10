"""Directed endpoint-flat witnesses on the pinned GOAL-017 late window."""
import argparse
import hashlib
from importlib.util import module_from_spec, spec_from_file_location
from itertools import product
from pathlib import Path
from platform import python_version
from resource import RUSAGE_SELF, getrusage

import flint
from flint import acb, arb, ctx
import numpy as np

ctx.prec = 192
D = arb("8.407362")
OMEGA = acb(0, arb.pi() / D)
Q = acb("-.177924631", "-.747343369")
STAR = Q + arb(".12656238")
Q_BOX = tuple(acb(x, y) for x, y in product(
    (".08223", ".18771"), ("-.42418", "-.31707")
))
S_BOX = tuple(acb(x, y) for x, y in product(
    ("-.21850", "-.16857"), ("-.44959", "-.39966")
))
DIGEST = "34284f389420d62aaacd30b111b17f0c740fca3528b9d7cc85290909996d9c07"


def affine_input(archive):
    with np.load(archive, allow_pickle=False) as data:
        x, full, n, b = [data[key] for key in ("u", "u_full", "p_shear_infty", "baseline")]
    if not all(a.ndim == 1 and a.dtype == np.float64 and np.isfinite(a).all()
               for a in (x, full, n, b)):
        raise ValueError("expected finite binary64 vectors")
    if len(x) <= 1 or x[0] != 0.0 or x[-1] != float("8.407362"):
        raise ValueError("unexpected late window")
    if len(full) != len(n) or len(x) != len(b):
        raise ValueError("inconsistent vector lengths")
    if not all(np.all(np.diff(a) > 0.0) for a in (x, full)):
        raise ValueError("abscissae must be strictly increasing")
    cut = np.searchsorted(full, 0.0)
    if not np.array_equal(full[cut:], x):
        raise ValueError("late abscissae do not match full-grid slice")
    n = n[cut:]
    digest = hashlib.sha256(np.stack((x, n, b), axis=-1).astype("<f8").tobytes()).hexdigest()
    if digest != DIGEST:
        raise ValueError("return archive digest mismatch")
    print("validated affine return", len(x), "nodes, digest", digest)
    return [arb(float(t)) for t in x[:-1]] + [D], [arb(float(t)) for t in n]


# A real function is represented as a finite list of (rate, coefficient) pairs.
def multiply(left, right):
    return [(s + t, z * y) for s, z in left for t, y in right]


def scale(profile, c):
    return [(s, z * c) for s, z in profile]


def cosine(j):
    return [(j * OMEGA, acb(".5")), (-j * OMEGA, acb(".5"))]


def sine(j):
    return [(j * OMEGA, acb(0, "-.5")), (-j * OMEGA, acb(0, ".5"))]


def pole(rate, residue):
    z = rate * residue
    return [(rate, z), (rate.conjugate(), z.conjugate())]


def derivative(profile):
    return [(s, s * z) for s, z in profile]


def integral(s):
    if abs(s) < arb("1e-40"):
        # The only near-zero rates here are exact algebraic cancellations of
        # integer multiples of i*pi/D; still use a uniform small-rate bound.
        error = (abs(s) * D * (abs(s) * D).exp() * D).upper()
        return acb(D) + acb((-error).union(error), (-error).union(error))
    return (s * D).expm1() / s


def inner(left, right):
    return sum(z * integral(s) for s, z in multiply(left, right)).real


def norm(profile):
    value = inner(profile, profile)
    assert value.is_finite() and value > 0
    return value.sqrt()


def unit(profile):
    return scale(profile, 1 / norm(profile))


def affine_inner(profile, x, y):
    total = acb(0)
    for s, z in profile:
        if abs(s) < arb("1e-40"):
            area = sum((b - a) * (ya + yb) / 2
                       for a, b, ya, yb in zip(x, x[1:], y, y[1:]))
            ymax = max(abs(v).upper() for v in y)
            error = (ymax * abs(s) * D**2 * (abs(s) * D).exp()).upper()
            radius = (-error).union(error)
            total += z * acb(area + radius, radius)
            continue
        for a, b, ya, yb in zip(x, x[1:], y, y[1:]):
            h = b - a
            sh = s * h
            j0 = sh.expm1() / sh
            j1 = (sh.exp() * (sh - 1) + 1) / sh**2
            total += z * h * (s * a).exp() * (ya * j0 + (yb - ya) * j1)
    assert total.is_finite()
    return total.real


def abs_derivative_bound(profile, j):
    return sum(abs(z) * abs(s)**j for s, z in profile if j == 0 or not s.is_zero()).upper()


def midpoint_check(profile, x, y, divisions=8):
    """Independent interval quadrature, using the exact affine interpolant."""
    first = abs_derivative_bound(profile, 1)
    second = abs_derivative_bound(profile, 2)
    center, error = arb(0), arb(0)
    for a, b, ya, yb in zip(x, x[1:], y, y[1:]):
        width = b - a
        h = width / divisions
        slope = (yb - ya) / width
        ymax = max(abs(ya).upper(), abs(yb).upper())
        m = 2 * abs(slope) * first + ymax * second
        error += width * h**2 * m / 24
        for i in range(divisions):
            f = (arb(i) + arb(".5")) / divisions
            u = a + width * f
            g = sum(z * (s * u).exp() for s, z in profile).real
            center += h * (ya + (yb - ya) * f) * g
    return inflate(center, error)


def inflate(x, r):
    r = r.upper()
    return x + (-r).union(r)


def perturbation(s, amplitudes, radius):
    assert s.real + radius < 0
    amplitude = max(abs(a).upper() for a in amplitudes)
    return 2 * amplitude * radius * (1 + (abs(s) + radius) * D) * D.sqrt()


def threshold(g, corners, error):
    margins = [inner(g, p) - norm(p) / 2 for p in corners]
    assert all(value.is_finite() for value in margins)
    return (min(value.lower() for value in margins) - arb("1.5") * error).lower()


def cross(a, b):
    return [a[j] * b[k] - a[k] * b[j] for j, k in ((1, 2), (2, 0), (0, 1))]


def baseline(path):
    spec = spec_from_file_location("baseline_source", path)
    module = module_from_spec(spec)
    spec.loader.exec_module(module)
    _, trace, error = module.integrate(128, 256, 256, 8192)
    assert len(trace) == 257 and error < arb("3.898e-5")
    return [D * i / (len(trace) - 1) for i in range(len(trace))], trace, error


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("archive", type=Path)
    parser.add_argument("baseline", type=Path)
    args = parser.parse_args()
    print(f"Python {python_version()}, python-flint {flint.__version__}, NumPy {np.__version__}, Arb {ctx.prec} bits")
    x, n = affine_input(args.archive)
    xb, nb, error = baseline(args.baseline)

    # T13 = sin(theta) - sin(13 theta)/13.
    t13 = sine(1) + scale(sine(13), -arb(1) / 13)
    early_weight = [(acb(0), acb(1))] + scale(cosine(1), arb(".5"))
    g_reject = unit(multiply(multiply(t13, early_weight), pole(Q, acb(".13497", "-.370625"))))
    t2 = [(acb(0), acb(".5"))] + scale(cosine(2), arb("-.5"))
    g_match = unit(multiply(t2, cosine(1)))

    q = [pole(Q, a) for a in Q_BOX]
    star = [pole(STAR, a) for a in S_BOX]
    eq = perturbation(Q, Q_BOX, arb("5e-8"))
    es = perturbation(STAR, S_BOX, arb("7e-8"))
    candidate_reject = affine_inner(g_reject, x, n)
    candidate_match = affine_inner(g_match, x, n)
    for label, g, v in (("reject", g_reject, candidate_reject), ("matched", g_match, candidate_match)):
        independent = midpoint_check(g, x, n)
        assert independent.contains(v) and independent.rad() < arb("3e-5")
        print(label, "candidate", v.str(20), "midpoint check", independent.str(12),
              "derivative norm", norm(derivative(g)).str(20))
    for label, corners, e, minimum in (
        ("Q", q, eq, ".1775"),
        ("Q+S*", [a + b for a, b in product(q, star)], eq + es, ".3758"),
    ):
        gate = threshold(g_reject, corners, e)
        margin = (gate - candidate_reject).lower()
        print(label, "template threshold lower", gate.str(20), "error allowance lower", margin.str(20))
        assert margin > arb(minimum)
    assert candidate_match < arb("-.07999")
    b_match = affine_inner(g_match, xb, nb)
    print("physical baseline matched-test moment endpoints",
          (b_match - error).lower().str(20), (b_match + error).upper().str(20))

    # Endpoint-flat orthonormal basis. Under sin^4(theta) weighting, the
    # mean of cos(2 theta) is -2/3 and the odd cosine is orthogonal to both.
    g0 = unit(t2)
    g1 = g_match
    g2 = unit(multiply(t2, cosine(2) + [(acb(0), acb(arb(2) / 3))]))
    basis = (g0, g1, g2)
    for i, gi in enumerate(basis):
        for j, gj in enumerate(basis):
            overlap = inner(gi, gj)
            assert overlap.contains(int(i == j)) and overlap.rad() < arb("1e-40")
    a, b, c = [], [], []
    for g in basis:
        dg = derivative(g)
        a.append(inflate(1000 * affine_inner(g, xb, nb), 1000 * error))
        b.append(inflate(-1000 * affine_inner(dg, xb, nb), 1000 * norm(dg) * error))
        c.append(affine_inner(g, x, n))
        assert midpoint_check(g, x, n).contains(c[-1])
    direction = cross(a, b)
    assert all(v.is_finite() for v in direction)
    support = sum(v * p for v, p in zip(direction, c)).lower()
    direction_upper = sum(abs(v).upper()**2 for v in direction).sqrt().upper()
    assert support > 0 and direction_upper > 0
    print("1000 times physical amplitude moments", [v.str(12) for v in a])
    print("1000 times physical clock tangent moments", [v.str(12) for v in b])
    print("affine return moments", [v.str(16) for v in c])
    lower = (support / direction_upper).lower()
    print("candidate distance from exact baseline amplitude and clock tangent span >", lower.str(20))
    assert lower > arb(".04")
    usage = getrusage(RUSAGE_SELF)
    print(f"process CPU {usage.ru_utime + usage.ru_stime:.2f} s, ru_maxrss {usage.ru_maxrss} (bytes on macOS)")


if __name__ == "__main__":
    main()
