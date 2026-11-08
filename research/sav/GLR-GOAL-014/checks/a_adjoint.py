"""Exact polynomial fixtures and rational upper bounds for the sharp adjoint."""

from fractions import Fraction as Q
from math import factorial

import sympy as sp


def rational_exp_upper(x):
    """Taylor through degree 9, then a positive geometric bound."""
    partial = sum(x**j / factorial(j) for j in range(10))
    return partial + x**10 / factorial(10) / (1 - x / 11)


def upper_decimal(x, digits):
    """Round a nonnegative rational towards +infinity to a decimal."""
    scale = 10**digits
    integer = -(-x.numerator * scale // x.denominator)
    return f"{integer // scale}.{integer % scale:0{digits}d}"


def weights():
    # A common lower radius 159/100 follows from log(125/118) >= 7/125.
    r_min, r_cap = Q("1.59"), Q(400)
    assert Q("0.035") + 2 * Q(7, 125) > Q("0.12")
    ray_mass = lambda r: 3 / r - Q(3, 4) / r**2
    M, m_cap = ray_mass(r_min), ray_mass(r_cap)
    assert M < Q("1.6")
    for name, D, gL1, gpL1, gd, test0, test1, testd, sqrtD, gL2 in [
        ("early", ".015", ".000791631", ".413693", ".412547300",
         "2.491e-10", "1.900e-7", "9.709e-9", ".123", ".013660"),
        ("packet", ".12", ".271373", "19.176120", ".009754934",
         "2.831e-4", ".007343", ".002026", ".347", "1.001"),
    ]:
        D, gL1, gpL1, gd, test0, test1, testd, sqrtD, gL2 = map(
            Q, (D, gL1, gpL1, gd, test0, test1, testd, sqrtD, gL2)
        )
        assert sqrtD**2 >= D
        A1 = rational_exp_upper(D * m_cap)
        A2 = rational_exp_upper(3 * D**2 / (Q("3.14159") * r_cap)**2)
        compact = rational_exp_upper(M * D)
        sigma = compact * A1 * gL1
        F_weight = (gd + gpL1 + m_cap * A1 * gL1 + M * sigma) / 4
        GH_weight = sigma / 4
        no_cap_G_weight = (compact - 1) * A1 * gL1 / 4
        tail_weak = (
            (gd + gpL1 + m_cap * A1 * gL1 + A1 * gL1) * Q("3.36e-15")
            + (A1 - 1) * gL1 * Q("2.74e-15")
        ) / 4
        # Inherited conservative B_c uses the longer source window.
        tail_strong = A2 * gL2 * Q("1.7e-6")
        # h_1 applies the first homogeneous cap correction to the test candidate.
        e0, e1 = sqrtD * test0, sqrtD * test1
        g0L1 = gL1 + e0
        eh0 = A1 * e0 + (D * m_cap)**2 * A1 * g0L1 / 2
        eh1 = e1 + m_cap * A1 * e0 + D * m_cap**2 * A1 * g0L1 / 2
        test_sigma = compact * eh0
        test_F = (testd + eh1 + M * test_sigma) / 4
        test_GH = test_sigma / 4
        test_G_no_cap = (compact - 1) * eh0 / 4
        print(name)
        for label, val, precision in [
            ("compact F sup-ray weight", F_weight, 9),
            ("compact G/H sup-ray weight with cap G", GH_weight, 9),
            ("compact G sup-ray weight without cap G", no_cap_G_weight, 9),
            ("weak exterior scalar error", tail_weak, 19),
            ("strong exterior scalar error", tail_strong, 13),
            ("test error F sup-ray weight", test_F, 12),
            ("test error G/H sup-ray weight with cap G", test_GH, 12),
            ("test error G sup-ray weight without cap G", test_G_no_cap, 12),
        ]:
            print(f"  {label} < {upper_decimal(val, precision)}")


def fixtures():
    u, v = sp.symbols("u v", real=True)
    h = 1 + u
    Z = h - (1 - u) * (1 - v) / 2
    V = 2 / Z
    assert sp.simplify(4 * sp.diff(Z, u, v) + V * Z) == 0
    b = 1 - u
    tau = u + v - 1
    w = u * tau
    F = 2 * u**2
    G = 4 * u - 2 * u**2
    H = 4 * u - 4 - V * w
    residual = 4 * sp.diff(w, u, v) + V * w - sp.diff(F, u) - sp.diff(G, v) + H
    assert sp.simplify(residual) == 0
    assert sp.simplify(4 * sp.diff(w, u).subs(v, b) - (G + F)) == 0
    F_ray = sp.integrate(F, (v, b, 1))
    terminal = (h * F_ray).subs(u, 1)
    cap = sp.integrate(h * G, (u, 0, 1))
    bulk = sp.integrate(
        sp.expand(sp.diff(Z, u) * F + sp.diff(Z, v) * G + Z * H),
        (v, b, 1), (u, 0, 1)
    )
    strong = sp.integrate(h * sp.diff(w, u).subs(v, 1), (u, 0, 1))
    adjoint = (terminal + cap - bulk) / 4
    assert strong == adjoint == sp.Rational(5, 3)
    print(f"nonzero graph fixture: 4P={4 * adjoint}, terminal={terminal}, cap={cap}, bulk={bulk}")
    print(f"  if terminal omitted: P={(cap - bulk) / 4}")

    # Non-null seam v=u, positive region v>u. The graph and front jets of wj vanish.
    amp = u * tau**2
    wj = amp * (v - u)
    Fj = 4 * sp.diff(wj, v)
    assert sp.simplify(Fj.subs(v, u) - 4 * amp.subs(v, u)) == 0
    assert sp.simplify(sp.diff(wj, u).subs(v, u) + amp.subs(v, u)) == 0
    seam = sp.integrate((-Z * Fj).subs(v, u), (u, sp.Rational(1, 2), 1))
    assert seam != 0
    q = sp.expand(sp.diff(Z, u) * Fj - Z * V * wj)
    bulk_j = sp.integrate(q, (v, b, 1), (u, 0, sp.Rational(1, 2)))
    bulk_j += sp.integrate(q, (v, u, 1), (u, sp.Rational(1, 2), 1))
    exact_j = sp.integrate(h * sp.diff(wj, u).subs(v, 1), (u, 0, 1))
    assert exact_j == -bulk_j / 4
    print(f"non-null seam fixture: P={exact_j}, delta contribution to 4P={seam}")
    print(f"  cell-interior strong derivative only: P={exact_j - seam / 4}")


if __name__ == "__main__":
    fixtures()
    weights()
