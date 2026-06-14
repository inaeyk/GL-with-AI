#!/usr/bin/env python3
"""Stage 3G off-diagonal conformal-cartoon checks.

This script is a derivation artifact, not evolution code. It verifies the
algebraic extension from the diagonal Stage 3F conformal-cartoon translation
to a reduced two-dimensional metric block with nonzero h_xz / gamma_xz.
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
    return sp.factor(sp.cancel(sp.simplify(expr)))


def check_zero(name, expr):
    """Assert that an expression simplifies to zero."""
    simplified = simplify_expr(expr)
    if simplified != 0:
        print(f"FAIL {name}", file=sys.stderr)
        print(f"  expression = {sp.sstr(simplified)}", file=sys.stderr)
        sys.exit(1)
    print(f"PASS {name} = 0")


def check_nonzero_expected(name, expr):
    """Assert that an expression is nonzero for a deliberate failure case."""
    simplified = simplify_expr(expr)
    if simplified == 0:
        print(f"FAIL {name}: expected a nonzero mismatch", file=sys.stderr)
        sys.exit(1)
    print(f"PASS {name} fails as expected = {sp.sstr(simplified)}")


def main():
    gamma_xx, gamma_xz, gamma_zz, gamma_ww = sp.symbols(
        "gamma_xx gamma_xz gamma_zz gamma_ww", nonzero=True
    )
    h_xx, h_xz, h_zz, hww, chi = sp.symbols(
        "h_xx h_xz h_zz hww chi", nonzero=True
    )
    A_xx, A_xz, A_zz, Aww = sp.symbols("A_xx A_xz A_zz Aww")
    K_xx, K_xz, K_zz, K_ww = sp.symbols("K_xx K_xz K_zz K_ww")

    D_gamma = gamma_xx * gamma_zz - gamma_xz**2
    det_gamma_4d = D_gamma * gamma_ww**2

    D_h = h_xx * h_zz - h_xz**2
    det_h_4d = D_h * hww**2

    h_inv_xx = h_zz / D_h
    h_inv_xz = -h_xz / D_h
    h_inv_zz = h_xx / D_h
    h_inv_ww = 1 / hww

    print("Stage 3G off-diagonal conformal cartoon checks")
    print("Reduced block includes h_xz / gamma_xz")
    print("N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2")

    check_zero(
        "physical reduced-block determinant D_gamma",
        D_gamma - (gamma_xx * gamma_zz - gamma_xz**2),
    )
    check_zero(
        "physical 4D determinant det gamma_4D",
        det_gamma_4d - (gamma_xx * gamma_zz - gamma_xz**2) * gamma_ww**2,
    )

    h_from_gamma = {
        h_xx: chi * gamma_xx,
        h_xz: chi * gamma_xz,
        h_zz: chi * gamma_zz,
        hww: chi * gamma_ww,
    }
    check_zero(
        "conformal reduced-block determinant from physical block",
        D_h.subs(h_from_gamma) - chi**2 * D_gamma,
    )
    check_zero(
        "conformal 4D determinant relation",
        det_h_4d.subs(h_from_gamma) - chi**4 * det_gamma_4d,
    )
    check_zero(
        "det h_4D normalization ratio",
        det_h_4d.subs(h_from_gamma) / (chi**4 * det_gamma_4d) - 1,
    )

    check_zero("inverse h xx component", h_xx * h_inv_xx + h_xz * h_inv_xz - 1)
    check_zero("inverse h xz component", h_xx * h_inv_xz + h_xz * h_inv_zz)
    check_zero("inverse h zx component", h_xz * h_inv_xx + h_zz * h_inv_xz)
    check_zero("inverse h zz component", h_xz * h_inv_xz + h_zz * h_inv_zz - 1)
    check_zero("inverse h ww component", hww * h_inv_ww - 1)

    trace_A = (
        h_inv_xx * A_xx
        + 2 * h_inv_xz * A_xz
        + h_inv_zz * A_zz
        + 2 * h_inv_ww * Aww
    )
    Aww_solution = -hww * (
        h_inv_xx * A_xx + 2 * h_inv_xz * A_xz + h_inv_zz * A_zz
    ) / 2
    check_zero(
        "full-4D conformal tracelessness solved for Aww",
        trace_A.subs(Aww, Aww_solution),
    )
    check_zero(
        "solve(trace=0) returns off-diagonal Aww formula",
        sp.solve(sp.Eq(trace_A, 0), Aww)[0] - Aww_solution,
    )

    gamma_inv_xx = chi * h_inv_xx
    gamma_inv_xz = chi * h_inv_xz
    gamma_inv_zz = chi * h_inv_zz
    gamma_inv_ww = chi * h_inv_ww
    K_trace = (
        gamma_inv_xx * K_xx
        + 2 * gamma_inv_xz * K_xz
        + gamma_inv_zz * K_zz
        + 2 * gamma_inv_ww * K_ww
    )

    A_xx_conf = chi * K_xx - h_xx * K_trace / 4
    A_xz_conf = chi * K_xz - h_xz * K_trace / 4
    A_zz_conf = chi * K_zz - h_zz * K_trace / 4
    Aww_conf = chi * K_ww - hww * K_trace / 4

    check_zero(
        "K_xx reconstruction with /4",
        (A_xx_conf + h_xx * K_trace / 4) / chi - K_xx,
    )
    check_zero(
        "K_xz reconstruction with /4",
        (A_xz_conf + h_xz * K_trace / 4) / chi - K_xz,
    )
    check_zero(
        "K_zz reconstruction with /4",
        (A_zz_conf + h_zz * K_trace / 4) / chi - K_zz,
    )
    check_zero(
        "K_ww reconstruction with /4",
        (Aww_conf + hww * K_trace / 4) / chi - K_ww,
    )
    check_zero(
        "A_ij definitions are full-4D tracefree",
        (
            h_inv_xx * A_xx_conf
            + 2 * h_inv_xz * A_xz_conf
            + h_inv_zz * A_zz_conf
            + 2 * h_inv_ww * Aww_conf
        ),
    )

    p = sp.symbols("p")
    Kww_reconstructed_p = (Aww_conf + hww * K_trace / p) / chi
    check_zero(
        "p=4 dimension denominator works",
        Kww_reconstructed_p.subs(p, 4) - K_ww,
    )
    numeric_values = {
        h_xx: sp.Integer(2),
        h_xz: sp.Integer(1),
        h_zz: sp.Integer(3),
        hww: sp.Integer(5),
        chi: sp.Integer(7),
        K_xx: sp.Integer(11),
        K_xz: sp.Integer(13),
        K_zz: sp.Integer(17),
        K_ww: sp.Integer(19),
    }
    check_nonzero_expected(
        "p=2 dimension denominator",
        (Kww_reconstructed_p.subs(p, 2) - K_ww).subs(numeric_values),
    )
    check_nonzero_expected(
        "p=3 dimension denominator",
        (Kww_reconstructed_p.subs(p, 3) - K_ww).subs(numeric_values),
    )

    diagonal_subs = {h_xz: 0, A_xz: 0, K_xz: 0, gamma_xz: 0}
    check_zero(
        "diagonal limit D_gamma",
        D_gamma.subs(diagonal_subs) - gamma_xx * gamma_zz,
    )
    check_zero(
        "diagonal limit D_h",
        D_h.subs(diagonal_subs) - h_xx * h_zz,
    )
    check_zero(
        "diagonal limit tracefree A relation",
        trace_A.subs(diagonal_subs)
        - (A_xx / h_xx + A_zz / h_zz + 2 * Aww / hww),
    )
    check_zero(
        "diagonal limit Aww solution",
        Aww_solution.subs(diagonal_subs)
        - (-hww * (A_xx / h_xx + A_zz / h_zz) / 2),
    )
    check_zero(
        "diagonal limit K trace",
        K_trace.subs(diagonal_subs)
        - chi * (K_xx / h_xx + K_zz / h_zz + 2 * K_ww / hww),
    )
    check_zero(
        "diagonal limit det h_4D",
        det_h_4d.subs(diagonal_subs) - h_xx * h_zz * hww**2,
    )

    print("All Stage 3G off-diagonal conformal cartoon checks passed.")


if __name__ == "__main__":
    main()
