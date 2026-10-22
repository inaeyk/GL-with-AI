#!/usr/bin/env python3
"""Tiny symbolic algebra checks, NOT a GRChombo or spacetime simulation.

Reproduces three narrow algebraic tests used to seed the foundation record.
Run: python3 research/sav/verification/check_foundation.py
Requires: sympy. Tested with SymPy 1.14.0 and Python 3.13.5.
"""
import sympy as sp


def main():
    z, k, y = sp.symbols("z k y", real=True, positive=True)
    Q, r0, tau, G4, M, n = sp.symbols(
        "Q r0 tau G4 M n", real=True, positive=True
    )
    ct, cz = sp.symbols("ct cz", real=True)
    assert sp.simplify(
        sp.cos(k * z) ** 2 - (1 + sp.cos(2 * k * z)) / 2
    ) == 0
    print("FOURIER_SELECTION: PASS")

    # For Phi=Q*r0/(1+exp(-u/tau)), let y=exp(-u/tau):
    # integral du Phi_dot^2=(Q*r0)^2/tau * int_0^infty y/(1+y)^4 dy.
    integral = sp.integrate(y / (1 + y) ** 4, (y, 0, sp.oo))
    assert integral == sp.Rational(1, 6)
    fraction = sp.simplify(
        (2 * G4 / r0) * (Q**2 * r0**2 / (G4 * tau)) * integral
    )
    assert sp.simplify(fraction - Q**2 * r0 / (3 * tau)) == 0
    print("SIGMOID_ENERGY: PASS; beta_integral=1/6")

    # Static Kaluza–Klein charge/tension linear algebra for d=4.
    solution = sp.solve(
        [sp.Eq(2 * ct - cz, 4 * G4 * M),
         sp.Eq(ct - 2 * cz, 4 * G4 * M * n)],
        [ct, cz],
    )
    assert sp.simplify(
        sp.sqrt(3) * solution[cz] / 4 -
        G4 * M * (1 - 2 * n) / sp.sqrt(3)
    ) == 0
    print("STATIC_CHARGE_TENSION_ALGEBRA: PASS")
    print("NOTE: no Einstein field-equation/gauge or radiation calculation tested")


if __name__ == "__main__":
    main()
