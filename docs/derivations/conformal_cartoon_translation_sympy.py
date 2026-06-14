#!/usr/bin/env python3
"""Stage 3F conformal-variable translation checks.

This script is a derivation artifact, not evolution code. It verifies the
basic symbolic dictionary between physical cartoon variables and the
conformal variables used by GRChombo-style CCZ4 for the diagonal reduced case.
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


def q_first_expected(q_hww, chi, var):
    """Expected first derivative of q = hww / chi."""
    return sp.diff(q_hww, var) / chi - q_hww * sp.diff(chi, var) / chi**2


def q_second_expected(q_hww, chi, var_a, var_b):
    """Expected second derivative of q = hww / chi."""
    h_a = sp.diff(q_hww, var_a)
    h_b = sp.diff(q_hww, var_b)
    h_ab = sp.diff(q_hww, var_a, var_b)
    chi_a = sp.diff(chi, var_a)
    chi_b = sp.diff(chi, var_b)
    chi_ab = sp.diff(chi, var_a, var_b)
    return (
        h_ab / chi
        - (h_a * chi_b + h_b * chi_a + q_hww * chi_ab) / chi**2
        + 2 * q_hww * chi_a * chi_b / chi**3
    )


def main():
    x, z = sp.symbols("x z", positive=True)
    chi = sp.Function("chi")(x, z)
    hww_fn = sp.Function("hww")(x, z)

    q = hww_fn / chi
    f = x * sp.sqrt(q)

    log_q_x = sp.diff(hww_fn, x) / hww_fn - sp.diff(chi, x) / chi
    log_q_z = sp.diff(hww_fn, z) / hww_fn - sp.diff(chi, z) / chi

    print("Stage 3F conformal cartoon translation checks")
    print("q = gamma_ww = hww / chi")
    print("N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2")

    check_zero("q definition", q - hww_fn / chi)

    for name, var in (("x", x), ("z", z)):
        check_zero(
            f"partial_{name} q identity",
            sp.diff(q, var) - q_first_expected(hww_fn, chi, var),
        )

    for name, var_a, var_b in (("xx", x, x), ("xz", x, z), ("zz", z, z)):
        check_zero(
            f"partial_{name} q identity",
            sp.diff(q, var_a, var_b) - q_second_expected(hww_fn, chi, var_a, var_b),
        )

    for name, var, log_q_var in (("x", x, log_q_x), ("z", z, log_q_z)):
        check_zero(
            f"partial_{name} log q identity",
            sp.diff(q, var) / q - log_q_var,
        )

    log_q_by_var = {x: log_q_x, z: log_q_z}
    for name, var_a, var_b in (("xx", x, x), ("xz", x, z), ("zz", z, z)):
        check_zero(
            f"partial_{name} q over q logarithmic identity",
            sp.diff(q, var_a, var_b) / q
            - (
                sp.diff(log_q_by_var[var_b], var_a)
                + log_q_by_var[var_a] * log_q_by_var[var_b]
            ),
        )

    log_f_x = 1 / x + sp.Rational(1, 2) * log_q_x
    log_f_z = sp.Rational(1, 2) * log_q_z
    check_zero("partial_x log f identity", sp.diff(f, x) / f - log_f_x)
    check_zero("partial_z log f identity", sp.diff(f, z) / f - log_f_z)
    check_zero("partial_x f identity", sp.diff(f, x) - f * log_f_x)
    check_zero("partial_z f identity", sp.diff(f, z) - f * log_f_z)

    gamma_xx, gamma_zz, gamma_ww = sp.symbols(
        "gamma_xx gamma_zz gamma_ww", positive=True
    )
    det_gamma = gamma_xx * gamma_zz * gamma_ww**2
    chi_from_det = det_gamma ** sp.Rational(-1, 4)
    h_xx_det = chi_from_det * gamma_xx
    h_zz_det = chi_from_det * gamma_zz
    hww_det = chi_from_det * gamma_ww

    check_zero("physical-to-conformal round trip h_xx", h_xx_det / chi_from_det - gamma_xx)
    check_zero("physical-to-conformal round trip h_zz", h_zz_det / chi_from_det - gamma_zz)
    check_zero("physical-to-conformal round trip hww", hww_det / chi_from_det - gamma_ww)
    print("PASS physical-to-conformal round trip")
    check_zero("determinant condition det h_4D = 1", h_xx_det * h_zz_det * hww_det**2 - 1)

    K_xx, K_zz, K_ww = sp.symbols("K_xx K_zz K_ww")
    K_trace = K_xx / gamma_xx + K_zz / gamma_zz + 2 * K_ww / gamma_ww
    A_xx_conf = chi_from_det * (K_xx - gamma_xx * K_trace / 4)
    A_zz_conf = chi_from_det * (K_zz - gamma_zz * K_trace / 4)
    Aww_conf = chi_from_det * K_ww - hww_det * K_trace / 4

    check_zero(
        "K_xx reconstruction with /4",
        (A_xx_conf + h_xx_det * K_trace / 4) / chi_from_det - K_xx,
    )
    check_zero(
        "K_zz reconstruction with /4",
        (A_zz_conf + h_zz_det * K_trace / 4) / chi_from_det - K_zz,
    )
    check_zero(
        "K_ww reconstruction with /4",
        (Aww_conf + hww_det * K_trace / 4) / chi_from_det - K_ww,
    )
    check_zero(
        "full-4D conformal tracelessness with hidden multiplicity",
        A_xx_conf / h_xx_det + A_zz_conf / h_zz_det + 2 * Aww_conf / hww_det,
    )

    p = sp.symbols("p")
    Kww_reconstructed_p = (Aww_conf + hww_det * K_trace / p) / chi_from_det
    check_zero(
        "p=4 dimension denominator works",
        Kww_reconstructed_p.subs(p, 4) - K_ww,
    )
    numeric_values = {
        gamma_xx: sp.Integer(2),
        gamma_zz: sp.Integer(3),
        gamma_ww: sp.Integer(5),
        K_xx: sp.Integer(7),
        K_zz: sp.Integer(11),
        K_ww: sp.Integer(13),
    }
    check_nonzero_expected(
        "p=2 dimension denominator",
        (Kww_reconstructed_p.subs(p, 2) - K_ww).subs(numeric_values),
    )
    check_nonzero_expected(
        "p=3 dimension denominator",
        (Kww_reconstructed_p.subs(p, 3) - K_ww).subs(numeric_values),
    )

    h_xx, h_zz, hww_trace = sp.symbols("h_xx h_zz hww", nonzero=True)
    A_xx, A_zz, Aww = sp.symbols("A_xx A_zz Aww")
    trace = A_xx / h_xx + A_zz / h_zz + 2 * Aww / hww_trace
    Aww_solution = -hww_trace * (A_xx / h_xx + A_zz / h_zz) / 2
    check_zero(
        "diagonal conformal tracelessness solved for Aww",
        trace.subs(Aww, Aww_solution),
    )
    solved_Aww = sp.solve(sp.Eq(trace, 0), Aww)[0]
    check_zero("solve(trace=0) returns Aww formula", solved_Aww - Aww_solution)

    print("All Stage 3F conformal cartoon translation checks passed.")


if __name__ == "__main__":
    main()
