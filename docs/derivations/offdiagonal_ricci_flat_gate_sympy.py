#!/usr/bin/env python3
"""Stage 3G sheared-flat off-diagonal Ricci gate.

This script is a derivation artifact, not evolution code. It promotes the
off-diagonal Ricci flatness check into the repository so the geometry engine is
tested on metrics with genuine g_xz cross terms before CCZ4 RHS work begins.
"""

from __future__ import annotations

import sys

try:
    import sympy as sp
except ImportError:
    print(
        "SymPy is required for this derivation check. "
        "Install it with: python3 -m pip install sympy",
        file=sys.stderr,
    )
    sys.exit(1)


def simplify_expr(expr):
    """Apply a conservative simplification stack for symbolic checks."""
    return sp.factor(sp.cancel(sp.trigsimp(sp.simplify(expr))))


def check_zero(name, expr):
    """Assert that an expression simplifies to zero."""
    simplified = simplify_expr(expr)
    if simplified != 0:
        print(f"FAIL {name}", file=sys.stderr)
        print(f"  expression = {sp.sstr(simplified)}", file=sys.stderr)
        sys.exit(1)
    print(f"PASS {name} = 0")


def check_equal(name, actual, expected):
    """Assert symbolic equality and print the verified value."""
    difference = simplify_expr(actual - expected)
    if difference != 0:
        print(f"FAIL {name}", file=sys.stderr)
        print(f"  actual   = {sp.sstr(simplify_expr(actual))}", file=sys.stderr)
        print(f"  expected = {sp.sstr(simplify_expr(expected))}", file=sys.stderr)
        print(f"  diff     = {sp.sstr(difference)}", file=sys.stderr)
        sys.exit(1)
    print(f"PASS {name} = {sp.sstr(simplify_expr(actual))}")


def christoffel_symbols(coords, metric):
    """Return Gamma^i_{jk} and g^{ij} for a coordinate metric."""
    n = len(coords)
    inv_metric = simplify_expr(metric.inv())
    gamma = [[[sp.Integer(0) for _ in range(n)] for _ in range(n)] for _ in range(n)]

    for i in range(n):
        for j in range(n):
            for k in range(n):
                value = sp.Integer(0)
                for ell in range(n):
                    value += inv_metric[i, ell] * (
                        sp.diff(metric[ell, k], coords[j])
                        + sp.diff(metric[ell, j], coords[k])
                        - sp.diff(metric[j, k], coords[ell])
                    )
                gamma[i][j][k] = simplify_expr(sp.Rational(1, 2) * value)

    return gamma, inv_metric


def ricci_tensor(coords, gamma):
    """Compute R_ij from Christoffel symbols."""
    n = len(coords)
    ricci = [[sp.Integer(0) for _ in range(n)] for _ in range(n)]

    for i in range(n):
        for j in range(n):
            value = sp.Integer(0)
            for k in range(n):
                value += sp.diff(gamma[k][i][j], coords[k])
                value -= sp.diff(gamma[k][i][k], coords[j])
                for ell in range(n):
                    value += gamma[k][i][j] * gamma[ell][k][ell]
                    value -= gamma[ell][i][k] * gamma[k][j][ell]
            ricci[i][j] = simplify_expr(value)

    return ricci


def ricci_scalar(inv_metric, ricci):
    """Compute R = g^{ij} R_ij."""
    n = inv_metric.shape[0]
    value = sp.Integer(0)
    for i in range(n):
        for j in range(n):
            value += inv_metric[i, j] * ricci[i][j]
    return simplify_expr(value)


def check_matrix_identity(name, metric, inv_metric):
    """Check metric * inverse_metric = identity."""
    n = metric.shape[0]
    product = metric * inv_metric
    for i in range(n):
        for j in range(n):
            expected = sp.Integer(1) if i == j else sp.Integer(0)
            check_equal(f"{name} inverse identity ({i},{j})", product[i, j], expected)


def check_ricci_flat(name, coords, metric):
    """Check all Ricci tensor components and the Ricci scalar vanish."""
    gamma, inv_metric = christoffel_symbols(coords, metric)
    ricci = ricci_tensor(coords, gamma)
    scalar = ricci_scalar(inv_metric, ricci)
    labels = ("x", "theta", "phi", "z")

    check_matrix_identity(name, metric, inv_metric)
    for i, label_i in enumerate(labels):
        for j, label_j in enumerate(labels):
            check_zero(f"{name} R_{label_i}{label_j}", ricci[i][j])
    check_zero(f"{name} Ricci scalar", scalar)


def main():
    x, theta, phi, z = sp.symbols("x theta phi z", positive=True)
    lam = sp.symbols("lambda")
    coords = (x, theta, phi, z)
    ix, itheta, iphi, iz = range(4)
    sin2 = sp.sin(theta) ** 2

    print("Stage 3G sheared-flat off-diagonal Ricci gate")
    print("Coordinate order: (x, theta, phi, z)")
    print("Both metrics are flat pullbacks with genuine g_xz cross terms.")

    constant_shear_metric = sp.Matrix(
        [
            [1 + lam**2, 0, 0, lam],
            [0, x**2, 0, 0],
            [0, 0, x**2 * sin2, 0],
            [lam, 0, 0, 1],
        ]
    )
    check_equal("constant-shear g_xz", constant_shear_metric[ix, iz], lam)
    check_equal("constant-shear partial_x g_xz", sp.diff(constant_shear_metric[ix, iz], x), 0)
    check_ricci_flat("constant-shear flat metric", coords, constant_shear_metric)

    x_dependent_shear_metric = sp.Matrix(
        [
            [1 + x**2, 0, 0, x],
            [0, x**2, 0, 0],
            [0, 0, x**2 * sin2, 0],
            [x, 0, 0, 1],
        ]
    )
    check_equal("x-shear g_xz", x_dependent_shear_metric[ix, iz], x)
    check_equal("x-shear partial_x g_xz", sp.diff(x_dependent_shear_metric[ix, iz], x), 1)
    check_ricci_flat("x-shear flat metric", coords, x_dependent_shear_metric)

    print("All Stage 3G sheared-flat off-diagonal Ricci checks passed.")


if __name__ == "__main__":
    main()
