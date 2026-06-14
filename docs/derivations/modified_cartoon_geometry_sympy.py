#!/usr/bin/env python3
"""Stage 3C symbolic checks for modified-cartoon geometry bookkeeping.

This script is a derivation artifact, not evolution code. It verifies the
basic spherical Christoffels, GP extrinsic-curvature components, hidden
cartoon contractions, and trace multiplicity used in the Stage 3B notes.
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
    """Apply a conservative simplification stack for symbolic equality checks."""
    return sp.trigsimp(sp.simplify(expr))


def check_equal(name, actual, expected):
    """Check symbolic equality and print the verified expression."""
    difference = simplify_expr(actual - expected)
    if difference != 0:
        print(f"FAIL {name}", file=sys.stderr)
        print(f"  actual   = {sp.sstr(simplify_expr(actual))}", file=sys.stderr)
        print(f"  expected = {sp.sstr(simplify_expr(expected))}", file=sys.stderr)
        print(f"  diff     = {sp.sstr(difference)}", file=sys.stderr)
        sys.exit(1)

    print(f"PASS {name} = {sp.sstr(simplify_expr(actual))}")


def christoffel_symbols(coords, metric):
    """Return Gamma^i_{jk} for a coordinate metric."""
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


def covariant_derivative_covector(coords, gamma, covector, i, j):
    """Compute D_i beta_j for a covector beta_j."""
    value = sp.diff(covector[j], coords[i])
    for k in range(len(coords)):
        value -= gamma[k][i][j] * covector[k]
    return simplify_expr(value)


def main():
    x, theta, phi, z = sp.symbols("x theta phi z", positive=True)
    r0 = sp.symbols("r0", positive=True)
    s = sp.symbols("s", real=True)

    coords = (x, theta, phi, z)
    ix, itheta, iphi, iz = range(4)

    metric = sp.diag(
        1,
        x**2,
        x**2 * sp.sin(theta) ** 2,
        1,
    )

    gamma, inv_metric = christoffel_symbols(coords, metric)

    beta_x_up = s * sp.sqrt(r0 / x)
    beta_cov = [beta_x_up, sp.Integer(0), sp.Integer(0), sp.Integer(0)]

    n = len(coords)
    K = sp.MutableDenseMatrix.zeros(n, n)
    for i in range(n):
        for j in range(n):
            d_i_beta_j = covariant_derivative_covector(coords, gamma, beta_cov, i, j)
            d_j_beta_i = covariant_derivative_covector(coords, gamma, beta_cov, j, i)
            K[i, j] = simplify_expr(sp.Rational(1, 2) * (d_i_beta_j + d_j_beta_i))

    K_ww = simplify_expr(beta_x_up / x)
    trace = simplify_expr(
        inv_metric[ix, ix] * K[ix, ix]
        + inv_metric[iz, iz] * K[iz, iz]
        + 2 * K_ww
    )

    print("Stage 3C modified-cartoon geometry symbolic checks")
    print("N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2")

    check_equal(
        "Gamma^x_{theta theta}",
        gamma[ix][itheta][itheta],
        -x,
    )
    check_equal(
        "Gamma^x_{phi phi}",
        gamma[ix][iphi][iphi],
        -x * sp.sin(theta) ** 2,
    )
    check_equal(
        "Gamma^theta_{x theta}",
        gamma[itheta][ix][itheta],
        1 / x,
    )
    check_equal(
        "Gamma^phi_{x phi}",
        gamma[iphi][ix][iphi],
        1 / x,
    )

    check_equal(
        "K_xx",
        K[ix, ix],
        -sp.Rational(1, 2) * s * sp.sqrt(r0 / x**3),
    )
    check_equal(
        "K_theta theta",
        K[itheta, itheta],
        x * beta_x_up,
    )
    check_equal(
        "K_phi phi",
        K[iphi, iphi],
        x * beta_x_up * sp.sin(theta) ** 2,
    )
    check_equal(
        "K_zz",
        K[iz, iz],
        0,
    )

    theta_contraction = simplify_expr(inv_metric[itheta, itheta] * K[itheta, itheta])
    phi_contraction = simplify_expr(inv_metric[iphi, iphi] * K[iphi, iphi])
    check_equal(
        "gamma^theta theta K_theta theta",
        theta_contraction,
        beta_x_up / x,
    )
    check_equal(
        "gamma^phi phi K_phi phi",
        phi_contraction,
        beta_x_up / x,
    )
    check_equal(
        "K_ww",
        K_ww,
        beta_x_up / x,
    )
    check_equal(
        "K trace",
        trace,
        sp.Rational(3, 2) * s * sp.sqrt(r0 / x**3),
    )

    print("All Stage 3C modified-cartoon geometry checks passed.")


if __name__ == "__main__":
    main()
