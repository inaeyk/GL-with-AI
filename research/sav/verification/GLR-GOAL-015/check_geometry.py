"""Exact rational local tests for the GLR-015 source maps.

Pass research/sav in the pinned checkout as the argument. Computes the
full 5D coordinate Ricci coefficient with no GL equation or reduced
curvature identities. Four equally spaced compact phases exactly
average any coefficient quadratic in cos(kz), sin(kz).
"""
from fractions import Fraction as F
from itertools import product
from pathlib import Path
import sys

from sympy import Matrix, eye, simplify, symbols

N = 5
AXES = range(N)
PAIRS = tuple(product(AXES, repeat=2))
ZERO = (F(0),) * 3


def add(x, y):
    return tuple(a + b for a, b in zip(x, y))


def negative(x):
    return tuple(-a for a in x)


def multiply(x, y):
    return tuple(sum(x[j] * y[i - j] for j in range(i + 1)) for i in range(3))


def jets(r, k, om, h, hr, hrr, phase):
    f = 1 - 1 / r
    cos, sin = phase
    g = {}
    dg = {}
    ddg = {}
    entries = {
        (0, 0): (-f, -1 / r**2, 2 / r**3, h[0], hr[0], hrr[0]),
        (0, 1): (1, 0, 0, h[1], hr[1], hrr[1]),
        (1, 1): (0, 0, 0, h[2], hr[2], hrr[2]),
        (2, 2): (1, 0, 0, 0, 0, 0),
        (3, 3): (r**2, 2 * r, 2, h[3], hr[3], hrr[3]),
        (4, 4): (r**2, 2 * r, 2, h[3], hr[3], hrr[3]),
    }
    for i, j in PAIRS:
        g0, gr, grr, p, pr, prr = entries.get(tuple(sorted((i, j))), (0,) * 6)
        g[i, j] = (g0, p * cos, 0)
        for a in AXES:
            dg[a, i, j] = (gr if a == 1 else 0, (om*p*cos, pr*cos, -k*p*sin, 0, 0)[a], 0)
        second = {
            (0, 0): (0, om**2*p*cos, 0),
            (0, 1): (0, om*pr*cos, 0),
            (0, 2): (0, -om*k*p*sin, 0),
            (1, 1): (grr, prr*cos, 0),
            (1, 2): (0, -k*pr*sin, 0),
            (2, 2): (0, -k**2*p*cos, 0),
            # sin²(theta) at the equator, including its second derivative.
            (3, 3): (-2*g0, -2*p*cos, 0) if i == j == 4 else ZERO,
        }
        for a, b in PAIRS:
            ddg[a, b, i, j] = second.get(tuple(sorted((a, b))), ZERO)
    return g, dg, ddg


def ricci_second(r, k, om, h, hr, hrr, phase):
    g, dg, ddg = jets(r, k, om, h, hr, hrr, phase)
    g0 = Matrix(N, N, lambda i, j: g[i, j][0])
    h0 = Matrix(N, N, lambda i, j: g[i, j][1])
    inv0 = g0.inv()
    inv1 = -inv0 * h0 * inv0
    inv2 = inv0 * h0 * inv0 * h0 * inv0
    inv = {(i, j): tuple(F(m[i, j]) for m in (inv0, inv1, inv2)) for i, j in PAIRS}
    dinv = {}
    for a in AXES:
        for i, j in PAIRS:
            acc = ZERO
            for b, c in PAIRS:
                acc = add(acc, multiply(multiply(inv[i, b], dg[a, b, c]), inv[c, j]))
            dinv[a, i, j] = negative(acc)

    conn = {}
    dconn = {}
    for a, i, j in product(AXES, repeat=3):
        conn[a, i, j] = ZERO
        for l in AXES:
            term = add(add(dg[i, j, l], dg[j, i, l]), negative(dg[l, i, j]))
            conn[a, i, j] = add(conn[a, i, j], multiply(inv[a, l], term))
            for b in AXES:
                dterm = add(add(ddg[b, i, j, l], ddg[b, j, i, l]), negative(ddg[b, l, i, j]))
                acc = add(multiply(dinv[b, a, l], term), multiply(inv[a, l], dterm))
                dconn[b, a, i, j] = add(dconn.get((b, a, i, j), ZERO), acc)

    # conn and dconn contain twice the Christoffel symbols.
    out = {}
    for i, j in ((0, 0), (0, 1), (1, 1), (2, 2), (3, 3), (4, 4)):
        deriv = sum(dconn[a, a, i, j][2] - dconn[j, a, i, a][2] for a in AXES)
        square = sum(
            multiply(conn[a, a, b], conn[b, i, j])[2] -
            multiply(conn[a, j, b], conn[b, i, a])[2] for a, b in PAIRS
        )
        out[i, j] = deriv / 2 + square / 4
    return out


def density_check():
    # Any trace-free compact-unitary GL orbit coefficient, with EF data
    # converted to (t,x) and d=H_theta_theta/(2r^2).
    f, a, b, d, lam, dk = symbols("f a b d lambda d_k", nonzero=True)
    c = a - 4 * f * d
    eta = Matrix.diag(-1, 1)
    x = eta * Matrix([[a, b], [b, c]]) / f
    # det(I+epsilon x)=1+eps tr(x)+eps^2 det(x).
    det1, det2 = x.trace(), x.det()
    volume2 = det2 / 2 - det1**2 / 8
    reciprocal2 = 3 * det1**2 / 8 - det2 / 2
    m1 = (eye(2) * (det1 / 2 + 2*d) - x) * eta
    m2 = ((x*x) - (det1/2 + 2*d)*x + (volume2 + d*det1)*eye(2)) * eta / 2
    p1 = 4*d - det1/2
    p2 = (4*d*d - 2*d*det1 + reciprocal2) / 2
    maff = -lam * m1 * eta * m1 / (2*dk)
    L = Matrix([-1, 1])
    direct = (L.T*m2*L)[0]
    affine = (L.T*maff*L)[0]
    assert simplify(p1 - 6*d) == 0
    assert simplify(p2 - (7*d*d + (x*x).trace()/8)) == 0
    assert simplify(direct + 2*d*(a+2*b+c)/f) == 0
    assert simplify(affine + lam*direct/dk) == 0
    print("Symbolic direct and affine outgoing coefficients: PASS")


def graph_check():
    p, D, h, jt, jx, k, ut, zt, at, ax = symbols(
        "p D h jt jx k ut zt at ax", real=True
    )
    # Compact Fourier potential A_z=i z, h=r^4. Insert graph tangency
    # U_x=-p U_t, z_x=-p z_t, A_x=-p A_t.
    # Obtain U_t and z_t by linear elimination independent of source code.
    from sympy import linsolve
    eqs = [D*at - jt - p*ut - k*h*zt,
           D*ax + jx + ut + k*h*p*zt,
           ax + p*at,
           (1-p*p)*zt - k*(at+p*ax)]
    got = next(iter(linsolve(eqs, (ut, zt, at, ax))))
    want = ((p*jt-jx)/(1-p*p), k*(jt-p*jx)/((D-k*k*h)*(1-p*p)))
    assert all(simplify(a-b) == 0 for a, b in zip(got[:2], want))
    print("Symbolic mixed graph velocities with compact constraint: PASS")


if __name__ == "__main__":
    base = Path(sys.argv[1])
    sys.path.insert(0, str(base / "GLR-GOAL-015/source"))
    sys.path.insert(0, str(base / "GLR-GOAL-011/checks/physical_overlap"))
    from mean_flux import quadratic_ricci_flux
    from mean_series import ricci_from_jet

    cases = [
        (F(7, 4), F(3, 5), F(1, 16), (3, -2, 1, 5), (2, 7, -1, 3), (5, 2, -3, 9)),
        (F(13, 8), F(2, 3), F(2, 15), (-3, 4, 1, 2), (-6, 1, -2, 7), (3, -1, 2, -4)),
        (F(5, 2), F(0), F(1, 14), (2, 3, -1, 6), (3, -5, 2, 1), (0, 5, -2, 8)),
        (F(11, 9), F(4, 7), F(0), (-1, -3, 4, 8), (-4, 5, 2, 1), (-2, 5, -2, 2)),
        (F(5), F(1, 4), F(1, 5), (0, 0, 0, 9), (0, 0, 0, 3), (0, 0, 0, 5)),
    ]
    for j, (r, k, om, h, hr, hrr) in enumerate(cases, 1):
        h, hr, hrr = (tuple(map(F, row)) for row in (h, hr, hrr))
        vals = [ricci_second(r, k, om, h, hr, hrr, phase)
                for phase in ((1, 0), (0, 1), (-1, 0), (0, -1))]
        avg = {ij: sum(v[ij] for v in vals)/4 for ij in vals[0]}
        f = 1 - 1/r
        assert avg[0, 0] + f*avg[0, 1] == quadratic_ricci_flux(r, k, om, h, hr, hrr)
        assert (avg[1, 1], avg[3, 3], avg[2, 2]) == ricci_from_jet(r, om, k, h, hr, hrr)
        assert avg[3, 3] == avg[4, 4]
        print(f"Exact 5D Ricci manufactured jet {j}: PASS")
    density_check()
    graph_check()
