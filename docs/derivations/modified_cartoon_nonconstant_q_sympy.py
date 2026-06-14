#!/usr/bin/env python3
"""Stage 3E nonconstant-q Ricci regression for hidden-sector derivatives.

This script is a derivation artifact, not evolution code. It checks the direct
Ricci tensor for

    dl^2 = dx^2 + dz^2 + f(x,z)^2 dOmega_2^2

against independent warped-product formulas, using
f = x (1 + lambda x + mu z) and q = gamma_ww = (1 + lambda x + mu z)^2.
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
    """Assert actual == expected and print the checked value."""
    check_zero(name, actual - expected)


def check_not_identically_zero(name, expr):
    """Assert that an expression is not identically zero."""
    simplified = simplify_expr(expr)
    if simplified == 0:
        print(f"FAIL {name}: expression is identically zero", file=sys.stderr)
        sys.exit(1)
    print(f"PASS {name} = {sp.sstr(simplified)}")


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


def main():
    x, theta, phi, z = sp.symbols("x theta phi z", positive=True)
    lam = sp.Symbol("lambda")
    mu = sp.Symbol("mu")
    coords = (x, theta, phi, z)
    ix, itheta, iphi, iz = range(4)

    p = 1 + lam * x + mu * z
    f = x * p
    q = p**2

    metric = sp.diag(
        1,
        f**2,
        f**2 * sp.sin(theta) ** 2,
        1,
    )
    gamma, inv_metric = christoffel_symbols(coords, metric)
    ricci = ricci_tensor(coords, gamma)
    scalar = ricci_scalar(inv_metric, ricci)

    f_x = sp.diff(f, x)
    f_z = sp.diff(f, z)
    f_xx = sp.diff(f, x, 2)
    f_zz = sp.diff(f, z, 2)
    f_xz = sp.diff(f, x, z)
    grad_f_sq = simplify_expr(f_x**2 + f_z**2)
    delta_f = simplify_expr(f_xx + f_zz)

    reference_r_xx = simplify_expr(-2 * f_xx / f)
    reference_r_zz = simplify_expr(-2 * f_zz / f)
    reference_r_xz = simplify_expr(-2 * f_xz / f)
    reference_r_theta_theta = simplify_expr(1 - grad_f_sq - f * delta_f)
    reference_r_phi_phi = simplify_expr(reference_r_theta_theta * sp.sin(theta) ** 2)
    reference_scalar = simplify_expr(
        -4 * delta_f / f + 2 * (1 - grad_f_sq) / f**2
    )

    print("Stage 3E nonconstant q(x,z) Ricci regression")
    print("Metric: dl^2 = dx^2 + dz^2 + f(x,z)^2 dOmega_2^2")
    print("f = x*(1 + lambda*x + mu*z)")
    print("q = gamma_ww = (1 + lambda*x + mu*z)^2")

    check_not_identically_zero("partial_x q", sp.diff(q, x))
    check_not_identically_zero("partial_z q", sp.diff(q, z))
    check_not_identically_zero("partial_x partial_z q", sp.diff(q, x, z))

    check_equal("direct R_xx matches warped-product reference", ricci[ix][ix], reference_r_xx)
    check_equal("direct R_zz matches warped-product reference", ricci[iz][iz], reference_r_zz)
    check_equal("direct R_xz matches warped-product reference", ricci[ix][iz], reference_r_xz)
    check_equal(
        "direct R_theta theta matches warped-product reference",
        ricci[itheta][itheta],
        reference_r_theta_theta,
    )
    check_equal(
        "direct R_phi phi matches warped-product reference",
        ricci[iphi][iphi],
        reference_r_phi_phi,
    )
    check_equal("direct Ricci scalar matches warped-product reference", scalar, reference_scalar)

    flat_subs = {lam: 0, mu: 0}
    check_zero("flat-special R_xx", ricci[ix][ix].subs(flat_subs))
    check_zero("flat-special R_zz", ricci[iz][iz].subs(flat_subs))
    check_zero("flat-special R_xz", ricci[ix][iz].subs(flat_subs))
    check_zero("flat-special R_theta theta", ricci[itheta][itheta].subs(flat_subs))
    check_zero("flat-special R_phi phi", ricci[iphi][iphi].subs(flat_subs))
    check_zero("flat-special Ricci scalar", scalar.subs(flat_subs))

    nontrivial_subs = {lam: 2, mu: 3}
    check_zero(
        "lambda=2, mu=3 R_xx direct-reference",
        (ricci[ix][ix] - reference_r_xx).subs(nontrivial_subs),
    )
    check_zero(
        "lambda=2, mu=3 R_zz direct-reference",
        (ricci[iz][iz] - reference_r_zz).subs(nontrivial_subs),
    )
    check_zero(
        "lambda=2, mu=3 R_xz direct-reference",
        (ricci[ix][iz] - reference_r_xz).subs(nontrivial_subs),
    )
    check_zero(
        "lambda=2, mu=3 R_theta theta direct-reference",
        (ricci[itheta][itheta] - reference_r_theta_theta).subs(nontrivial_subs),
    )
    check_zero(
        "lambda=2, mu=3 R_phi phi direct-reference",
        (ricci[iphi][iphi] - reference_r_phi_phi).subs(nontrivial_subs),
    )
    check_zero(
        "lambda=2, mu=3 Ricci scalar direct-reference",
        (scalar - reference_scalar).subs(nontrivial_subs),
    )

    print("All Stage 3E nonconstant q(x,z) Ricci regression checks passed.")


if __name__ == "__main__":
    main()
