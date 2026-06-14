#!/usr/bin/env python3
"""Stage 3D symbolic checks for nontrivial hidden metric geometry.

This script is a derivation artifact, not evolution code. It verifies
Christoffel identities, angular contractions, Ricci structural identities, and
flat limits for a diagonal SO(3)-symmetric spatial metric with
q(x,z) = gamma_ww.
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


def print_expr(name, expr):
    """Print a simplified expression without treating it as a check."""
    print(f"{name} = {sp.sstr(simplify_expr(expr))}")


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
    q0 = sp.symbols("q0", positive=True)
    coords = (x, theta, phi, z)
    ix, itheta, iphi, iz = range(4)

    a = sp.Function("a")(x, z)
    c = sp.Function("c")(x, z)
    q = sp.Function("q")(x, z)

    angular_scale = x**2 * q
    metric = sp.diag(
        a,
        angular_scale,
        angular_scale * sp.sin(theta) ** 2,
        c,
    )
    gamma, inv_metric = christoffel_symbols(coords, metric)

    print("Stage 3D nontrivial gamma_ww symbolic checks")
    print("Metric ansatz: diagonal a(x,z), c(x,z), q(x,z)=gamma_ww")
    print("Off-diagonal g_xz=b(x,z) is deferred in this scaffold.")
    print("N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2")

    gamma_x_tt = -sp.diff(angular_scale, x) / (2 * a)
    gamma_z_tt = -sp.diff(angular_scale, z) / (2 * c)
    gamma_theta_x_theta = sp.diff(angular_scale, x) / (2 * angular_scale)
    gamma_theta_z_theta = sp.diff(q, z) / (2 * q)

    check_equal("Gamma^x_{theta theta}", gamma[ix][itheta][itheta], gamma_x_tt)
    check_equal("Gamma^z_{theta theta}", gamma[iz][itheta][itheta], gamma_z_tt)
    check_equal(
        "Gamma^theta_{x theta}",
        gamma[itheta][ix][itheta],
        gamma_theta_x_theta,
    )
    check_equal(
        "Gamma^theta_{z theta}",
        gamma[itheta][iz][itheta],
        gamma_theta_z_theta,
    )
    check_equal(
        "Gamma^x_{phi phi}",
        gamma[ix][iphi][iphi],
        gamma_x_tt * sp.sin(theta) ** 2,
    )
    check_equal(
        "Gamma^z_{phi phi}",
        gamma[iz][iphi][iphi],
        gamma_z_tt * sp.sin(theta) ** 2,
    )
    check_equal(
        "Gamma^phi_{x phi}",
        gamma[iphi][ix][iphi],
        gamma_theta_x_theta,
    )
    check_equal(
        "Gamma^phi_{z phi}",
        gamma[iphi][iz][iphi],
        gamma_theta_z_theta,
    )

    flat_metric = sp.diag(1, x**2, x**2 * sp.sin(theta) ** 2, 1)
    flat_gamma, flat_inv_metric = christoffel_symbols(coords, flat_metric)
    flat_ricci = ricci_tensor(coords, flat_gamma)
    check_equal("flat Gamma^x_{theta theta}", flat_gamma[ix][itheta][itheta], -x)
    check_equal(
        "flat Gamma^x_{phi phi}",
        flat_gamma[ix][iphi][iphi],
        -x * sp.sin(theta) ** 2,
    )
    check_equal("flat Gamma^theta_{x theta}", flat_gamma[itheta][ix][itheta], 1 / x)
    check_equal("flat Gamma^phi_{x phi}", flat_gamma[iphi][ix][iphi], 1 / x)
    check_equal("flat Gamma^theta_{z theta}", flat_gamma[itheta][iz][itheta], 0)
    check_equal("flat Gamma^z_{theta theta}", flat_gamma[iz][itheta][itheta], 0)

    t_theta_theta = sp.Function("T_theta_theta")(x, z)
    t_phi_phi = t_theta_theta * sp.sin(theta) ** 2
    theta_contraction = inv_metric[itheta, itheta] * t_theta_theta
    phi_contraction = inv_metric[iphi, iphi] * t_phi_phi
    check_equal(
        "angular contraction phi-theta equality",
        phi_contraction,
        theta_contraction,
    )

    ricci = ricci_tensor(coords, gamma)
    r_theta_theta = ricci[itheta][itheta]
    r_phi_phi_over_sin2 = simplify_expr(ricci[iphi][iphi] / sp.sin(theta) ** 2)
    print_expr("R_theta theta", r_theta_theta)
    print_expr("R_phi phi / sin(theta)^2", r_phi_phi_over_sin2)
    check_equal(
        "R_phi phi / sin(theta)^2 - R_theta theta",
        r_phi_phi_over_sin2 - r_theta_theta,
        0,
    )

    angular_ricci = simplify_expr(
        inv_metric[itheta, itheta] * ricci[itheta][itheta]
        + inv_metric[iphi, iphi] * ricci[iphi][iphi]
    )
    expected_angular_ricci = simplify_expr(2 * inv_metric[itheta, itheta] * r_theta_theta)
    print_expr("angular Ricci scalar contribution", angular_ricci)
    check_equal(
        "angular Ricci contribution equals 2 g^theta theta R_theta theta",
        angular_ricci - expected_angular_ricci,
        0,
    )

    print("Constant-q Ricci regression: a=1, c=1, q=q0")
    print("The flat Ricci check below is the q0=1 special case.")
    constant_q_metric = sp.diag(
        1,
        q0 * x**2,
        q0 * x**2 * sp.sin(theta) ** 2,
        1,
    )
    constant_q_gamma, constant_q_inv_metric = christoffel_symbols(
        coords, constant_q_metric
    )
    constant_q_ricci = ricci_tensor(coords, constant_q_gamma)
    constant_q_scalar = ricci_scalar(constant_q_inv_metric, constant_q_ricci)

    check_equal("constant-q R_xx", constant_q_ricci[ix][ix], 0)
    check_equal("constant-q R_zz", constant_q_ricci[iz][iz], 0)
    check_equal("constant-q R_xz", constant_q_ricci[ix][iz], 0)
    check_equal("constant-q R_theta theta", constant_q_ricci[itheta][itheta], 1 - q0)
    check_equal(
        "constant-q R_phi phi",
        constant_q_ricci[iphi][iphi],
        (1 - q0) * sp.sin(theta) ** 2,
    )
    check_equal(
        "constant-q Ricci scalar",
        constant_q_scalar,
        2 * (1 - q0) / (q0 * x**2),
    )

    check_equal("flat R_xx", flat_ricci[ix][ix], 0)
    check_equal("flat R_zz", flat_ricci[iz][iz], 0)
    check_equal("flat R_theta theta", flat_ricci[itheta][itheta], 0)
    check_equal("flat R_phi phi", flat_ricci[iphi][iphi], 0)

    print("All Stage 3D nontrivial gamma_ww symbolic checks passed.")


if __name__ == "__main__":
    main()
