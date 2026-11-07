"""Exact rational-jet and symbolic checks for the GOAL-009 optical audit.

Run with Python 3 and SymPy. No vacuum evolution or physical GL data.
A rational-jet pass checks manufactured points, not an identity on all metrics.
"""
from itertools import product
import platform

import sympy as sp


u, r, z = coordinates = sp.symbols("u r z", real=True)
indices = range(3)


def curvature(metric, radius, point=None):
    """Compute warped 5D Ricci on quotient slots and on the unit sphere."""
    def evaluate(expr):
        return sp.cancel(expr if point is None else expr.subs(point, simultaneous=True))

    g = metric.applyfunc(evaluate)
    gi = g.inv().applyfunc(sp.cancel)
    dg = [metric.diff(x).applyfunc(evaluate) for x in coordinates]
    ddg = [[metric.diff(x, y).applyfunc(evaluate) for y in coordinates]
           for x in coordinates]
    dgi = [-gi * derivative * gi for derivative in dg]

    def koszul(d, i, j, k):
        return d[j][i, k] + d[k][i, j] - d[i][j, k]

    gamma = {(i, j, k): sum(gi[i, l] * koszul(dg, l, j, k) for l in indices) / 2
             for i, j, k in product(indices, repeat=3)}
    dgamma = {
        (a, i, j, k): sum(dgi[a][i, l] * koszul(dg, l, j, k)
                          + gi[i, l] * koszul(ddg[a], l, j, k)
                          for l in indices) / 2
        for a, i, j, k in product(indices, repeat=4)
    }
    rad = evaluate(radius)
    dr = [evaluate(radius.diff(x)) for x in coordinates]
    hess = sp.Matrix(3, 3, lambda i, j:
                     evaluate(radius.diff(coordinates[i], coordinates[j]))
                     - sum(gamma[k, i, j] * dr[k] for k in indices))

    def warped_ricci(i, j):
        ric = sum(dgamma[k, k, i, j] - dgamma[j, k, i, k]
                  + sum(gamma[k, i, j] * gamma[l, k, l]
                        - gamma[k, i, l] * gamma[l, j, k] for l in indices)
                  for k in indices)
        return sp.cancel(ric - 2 * hess[i, j] / rad)

    ric = sp.Matrix(3, 3, warped_ricci)
    sphere = 1 - sum(gi[i, j] * (dr[i] * dr[j] + rad * hess[i, j])
                     for i, j in product(indices, repeat=2))
    return ric, sp.cancel(sphere)


def quotient(B, b, V, shift):
    """The shift argument is the actual metric coefficient g_uz."""
    return sp.Matrix([[-B * V, -B, shift], [-B, 0, 0], [shift, 0, b**2]])


def manufactured_jets():
    B = 2 + u*z + r*z/11 + r**2/13
    b = 3 + u*r/7 + z*r/9 + r**2/19
    V = 1 + u*r + z**2/17 + r**2/23
    U = sp.Rational(2, 9) + u*z/5 + r*z/11 + r**2/29
    true_areal_metric = quotient(B, b, V - b**2 * U**2 / B, b**2 * U)
    q_r = b.diff(r) / b
    lapse_z = B.diff(z) / B
    twist = b**3 * U.diff(r) / B
    C = twist + b * lapse_z
    rr = (2/r + q_r) * B.diff(r) / B - b.diff(r, 2) / b
    rz = (-(twist.diff(r) + 2*twist/r)/b - lapse_z.diff(r)
          + (2/r + q_r) * lapse_z) / 2
    sphere = 1 - ((r*b*V).diff(r) - r*b.diff(u) + r*(b*U).diff(z)) / (B*b)
    zz = (2*(b.diff(u, r) - U*b.diff(r, z)) - V*b.diff(r, 2)
          - (V.diff(r) + 2*U.diff(z))*b.diff(r)
          + 2*(b.diff(u) - U*b.diff(z) - V*b.diff(r) - b*U.diff(z))/r
          - (B*C/b**2).diff(z) + B*lapse_z**2/(2*b)
          - b**3*U.diff(r)**2/(2*B))
    points = [{u: 0, r: 2, z: 1}, {u: -1, r: 3, z: -1}]
    for point in points:
        ric, angular = curvature(true_areal_metric, r, point)
        actual = sp.Matrix([ric[1, 1], ric[1, 2],
                            ric[2, 2]*(B/b).subs(point), angular])
        expected = sp.Matrix([rr, rz, zz, sphere]).subs(point)
        assert actual == expected
        assert actual[2] != (zz + (B*C/b**2).diff(z)).subs(point)
    print("PASS: rr, rz, zz and sphere hierarchy at two exact nonvacuum jets")
    print("PASS negative control: omitting the compact derivative changes zz")

    # Here r is the local area coordinate, while sphere_radius is geometric.
    A = U
    sphere_radius = r / sp.sqrt(b)
    local_area_metric = quotient(B, b, V, -B*A)
    beta_r = B.diff(r)/B + b.diff(r)/b
    beta_z = B.diff(z)/B + b.diff(z)/b
    q_z = b.diff(z)/b
    P = A.diff(r) + A*(beta_r - 3*q_r) - beta_z + 3*q_z
    area_rr = 2*beta_r/r - 3*q_r**2/2
    area_rz = (P.diff(r) + 2*P/r + (4*beta_z - 6*q_z)/r - 3*q_r*q_z)/2
    for point in points:
        ric, _ = curvature(local_area_metric, sphere_radius, point)
        assert ric[1, 1] == area_rr.subs(point)
        assert ric[1, 2] == area_rz.subs(point)
    print("PASS: local-area lapse and shift constraints at two exact nonvacuum jets")


def radial_normal_form():
    d, a = sp.symbols("d a", real=True)
    F = lambda d: d*(d-1)/((d+2)*(d+3))
    H = 1/(d*(d+1))
    assert sp.cancel(F(d)*(1-6*H) - F(d-1)) == 0
    assert sp.cancel(F(d-2)/F(d-1) - (1-6/(d*(d-1)))) == 0
    assert sp.cancel(F(d-3)*(d-1)**2/F(d-1)
                     - (d**2-2*d-11+24/(d*(d-2)))) == 0
    assert sp.cancel(F(d)*(1-6*H) - F(d)) != 0
    print("PASS: three exact rational Euler-symbol identities; no-shift variant rejected")

    # W=r^(-a) is a test mode with a>1, before introducing any GL profile.
    P = -6 * r**(-a-1)/(a-1)
    A = (6/(a-1) - 3/a)*r**(-a)
    assert sp.simplify((r**2*P).diff(r) - 6*r**(-a)) == 0
    assert sp.simplify(A.diff(r) - P + 3*r**(-a-1)) == 0
    assert sp.simplify(r*P + 2*A - 6*r**(-a)/(a*(a-1))) == 0
    print("PASS: power-law test for terminal inverses and nonlocal compact coefficient")


def supplementary_counterexample():
    # Outgoing Vaidya product metric with arbitrary mass coefficient mu(u).
    mu = sp.Function("mu")(u)
    metric = quotient(sp.S.One, sp.S.One, 1-mu/r, sp.S.Zero)
    ric, angular = curvature(metric, r)
    assert angular == 0
    assert ric == sp.diag(-mu.diff(u)/r**2, 0, 0)
    print("PASS negative control: all radial and scalar equations hold, but")
    print("  outgoing Vaidya times circle has Ric_uu = -mu'(u)/r**2")


if __name__ == "__main__":
    print(f"Python {platform.python_version()}, SymPy {sp.__version__}")
    manufactured_jets()
    radial_normal_form()
    supplementary_counterexample()
