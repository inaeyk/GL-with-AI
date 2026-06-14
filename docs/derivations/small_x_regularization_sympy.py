#!/usr/bin/env python3
"""Stage 3I small-x regularity checks.

This is a symbolic derivation aid, not evolution code. It checks a few Taylor
limit identities that turn apparent 1/x and 1/x^2 cartoon singularities into
finite axis limits when the expected parity and matching conditions hold.
"""

from __future__ import annotations

import sys

try:
    import sympy as sp
except ImportError:
    print(
        "SymPy is required for this derivation check. "
        "Install it with: python3 -m pip install sympy"
    )
    sys.exit(1)


def simplify_expr(expr):
    return sp.simplify(sp.factor(expr))


def check_zero(label, expr):
    value = simplify_expr(expr)
    if value != 0:
        print(f"FAIL {label}: {value}")
        sys.exit(1)
    print(f"PASS {label} = 0")


def check_equal(label, expr, expected):
    value = simplify_expr(expr - expected)
    if value != 0:
        print(f"FAIL {label}: got {simplify_expr(expr)}, expected {expected}")
        sys.exit(1)
    print(f"PASS {label} = {simplify_expr(expected)}")


def main():
    x = sp.symbols("x", positive=True)

    f0, f2, f4 = sp.symbols("f0 f2 f4")
    q0, q2, q4 = sp.symbols("q0 q2 q4")
    gx2, gx4, gw2, gw4 = sp.symbols("gx2 gx4 gw2 gw4")
    h0, hx2, hx4, hw2, hw4 = sp.symbols("h0 hx2 hx4 hw2 hw4", nonzero=True)
    hxx0_bad, hww0_bad = sp.symbols("hxx0_bad hww0_bad", nonzero=True)
    chi0, chi2, chi4 = sp.symbols("chi0 chi2 chi4", nonzero=True)
    s1, s3, s5 = sp.symbols("s1 s3 s5")
    z0, z1, z2, z3 = sp.symbols("z0 z1 z2 z3")
    n_hidden = sp.Integer(2)

    print("Stage 3I small-x regularization symbolic checks")
    print("Taylor expansions are truncated representatives near x = 0.")

    scalar_even = f0 + f2 * x**2 + f4 * x**4
    check_equal(
        "even scalar partial_x f / x limit",
        sp.limit(sp.diff(scalar_even, x) / x, x, 0),
        2 * f2,
    )

    q_even = q0 + q2 * x**2 + q4 * x**4
    check_equal(
        "gamma_ww-like q partial_x q / x limit",
        sp.limit(sp.diff(q_even, x) / x, x, 0),
        2 * q2,
    )

    gamma_xx = q0 + gx2 * x**2 + gx4 * x**4
    gamma_ww = q0 + gw2 * x**2 + gw4 * x**4
    check_equal(
        "(gamma_xx - gamma_ww) / x^2 finite limit",
        sp.limit((gamma_xx - gamma_ww) / x**2, x, 0),
        gx2 - gw2,
    )

    h_xx = h0 + hx2 * x**2 + hx4 * x**4
    hww = h0 + hw2 * x**2 + hw4 * x**4
    chi = chi0 + chi2 * x**2 + chi4 * x**4
    conformal_gamma_xx = h_xx / chi
    conformal_gamma_ww = hww / chi
    check_equal(
        "conformal (gamma_xx - gamma_ww) / x^2 finite limit",
        sp.limit((conformal_gamma_xx - conformal_gamma_ww) / x**2, x, 0),
        (hx2 - hw2) / chi0,
    )

    gamma_xz = s1 * x + s3 * x**3 + s5 * x**5
    check_equal(
        "odd gamma_xz / x finite limit",
        sp.limit(gamma_xz / x, x, 0),
        s1,
    )
    check_equal(
        "odd gamma_xz radial regular combination finite limit",
        sp.limit((sp.diff(gamma_xz, x) - gamma_xz / x) / x**2, x, 0),
        2 * s3,
    )

    h_xz = chi * gamma_xz
    check_equal(
        "conformal h_xz / x finite limit",
        sp.limit(h_xz / x, x, 0),
        chi0 * s1,
    )

    A_xz = s1 * x + s3 * x**3
    check_equal(
        "odd A_xz / x finite limit",
        sp.limit(A_xz / x, x, 0),
        s1,
    )

    beta_x = s1 * x + s3 * x**3
    beta_z = f0 + f2 * x**2 + f4 * x**4
    check_equal(
        "odd radial vector beta_x / x finite limit",
        sp.limit(beta_x / x, x, 0),
        s1,
    )
    check_equal(
        "even axial vector partial_x beta_z / x limit",
        sp.limit(sp.diff(beta_z, x) / x, x, 0),
        2 * f2,
    )

    # Local convention checked here: the hidden-sector singular part of the
    # cartoon conformal connection is represented as
    #   N_hidden / x * (1 - h_xx / hww)
    # with N_hidden = 2. The overall sign and full GRChombo-facing assembly still
    # require implementation-level confirmation, but the finiteness condition
    # and hidden multiplicity are tested explicitly.
    tilde_gamma_x_singular = n_hidden * (1 - h_xx / hww) / x
    check_equal(
        "assembled cartoon tilde_Gamma^x finite under h_xx - hww = O(x^2)",
        sp.limit(tilde_gamma_x_singular, x, 0),
        0,
    )
    check_equal(
        "assembled cartoon tilde_Gamma^x odd coefficient finite",
        sp.limit(tilde_gamma_x_singular / x, x, 0),
        n_hidden * (hw2 - hx2) / h0,
    )

    bad_h_xx = hxx0_bad + hx2 * x**2
    bad_hww = hww0_bad + hw2 * x**2
    bad_tilde_gamma_x = n_hidden * (1 - bad_h_xx / bad_hww) / x
    check_equal(
        "assembled cartoon tilde_Gamma^x exposes 1/x divergence if matching is violated",
        sp.limit(x * bad_tilde_gamma_x, x, 0),
        n_hidden * (1 - hxx0_bad / hww0_bad),
    )

    z4_x = z1 * x + z3 * x**3
    z4_z = z0 + z2 * x**2
    hat_gamma_x = tilde_gamma_x_singular + 2 * z4_x
    hat_gamma_z = z4_z
    check_equal(
        "hat_Gamma^x finite with regular Z^x contribution",
        sp.limit(hat_gamma_x, x, 0),
        0,
    )
    check_equal(
        "hat_Gamma^x odd coefficient finite",
        sp.limit(hat_gamma_x / x, x, 0),
        n_hidden * (hw2 - hx2) / h0 + 2 * z1,
    )
    check_equal(
        "hat_Gamma^z even representative partial_x / x finite",
        sp.limit(sp.diff(hat_gamma_z, x) / x, x, 0),
        2 * z2,
    )
    print("PASS hat_Gamma^x / hat_Gamma^z parity convention documented")

    # Demonstrate that the matching condition is load-bearing: without the
    # shared axis value q0, the quotient would contain a pole.
    bad_gamma_xx = sp.symbols("g0_bad") + gx2 * x**2
    bad_quotient = simplify_expr((bad_gamma_xx - gamma_ww) / x**2)
    has_pole = bool(bad_quotient.has(x**-2))
    if not has_pole:
        print(f"FAIL unmatched axis values should expose a pole: {bad_quotient}")
        sys.exit(1)
    print("PASS unmatched gamma_xx/gamma_ww axis values expose x^-2 pole")

    check_zero(
        "diagonal matching removes x^-2 pole",
        simplify_expr((gamma_xx - gamma_ww) / x**2 - (gx2 - gw2 + (gx4 - gw4) * x**2)),
    )

    print("All Stage 3I small-x regularization checks passed.")


if __name__ == "__main__":
    main()
