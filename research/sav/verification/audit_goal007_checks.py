"""Non-production algebra and comparison checks for source 0864d9d.

Run with Python 3.14.8 and SymPy 1.14.0, using no repository imports.
"""

from fractions import Fraction as Q

import sympy as sp


def rw_and_shear():
    """Eliminate the two Proca components with arbitrary density sources."""
    t, r, r0, lam = sp.symbols("t r r0 lambda", positive=True)
    psi, jt, jx = [sp.Function(name)(t, r) for name in ("psi", "Jt", "Jx")]
    f = 1 - r0 / r

    def dx(value):
        return f * sp.diff(value, r)

    at = (jt - dx(r * psi)) / (lam * r**2)
    ax = -(jx + sp.diff(r * psi, t)) / (lam * r**2)
    curl_equation = lam * r * (sp.diff(ax, t) - dx(at) - f * psi / r**3)
    source = r * (sp.diff(jx / r**2, t) + dx(jt / r**2))
    potential = f * ((lam + 2) / r**2 - 3 * r0 / r**3)
    assert sp.simplify(
        curl_equation - (-sp.diff(psi, t, 2) + dx(dx(psi)) - potential * psi - source)
    ) == 0

    shear = sp.Symbol("C", real=True)
    bondi_au = -lam * shear / (4 * r**2)
    assert sp.expand(-r**3 * sp.diff(bondi_au, r)) == -lam * shear / 2
    print("Sourced RW potential and both source signs checked. psi_I = -lambda*C/2.")


def tt_and_phase():
    """Get the optical contraction from flat-end TT and test two insertions."""
    q, omega, k2, u0 = sp.symbols("q Omega k2 U_over_r", positive=True)
    htt, htr, hrr = sp.symbols("Htt Htr Hrr")
    angular = -u0 / 2
    tt = sp.solve([
        -omega * htt - q * htr,
        -omega * htr - q * hrr,
        -htt + hrr + 2 * angular,
    ], [htt, htr, hrr])
    contraction = (htt + 2 * htr + hrr).subs(tt)
    assert sp.factor((contraction + (q - omega)**2 * u0 / k2).subs(k2, q**2 - omega**2)) == 0

    v, z, chi, s = sp.symbols("v z chi s", real=True)
    # Model normalized principal perturbation -4 exp(-v) cos(z) d_u^2.
    # The coefficient equations involve one v integral at each insertion.
    first = -sp.integrate(sp.exp(-s) * sp.cos(z), (s, 0, v))
    second = -sp.integrate(sp.exp(-s) * sp.cos(z) * first.subs(v, s), (s, 0, v))
    assert sp.simplify(second - first**2 / 2) == 0
    assert sp.integrate(first, (z, 0, 2 * sp.pi)) == 0
    average = sp.integrate(second, (z, 0, 2 * sp.pi)) / (2 * sp.pi)
    assert sp.simplify(average - (1 - sp.exp(-v))**2 / 4) == 0

    # A smooth rapidly decaying profile can have the opposite *total* sign
    # from the rising front if the correction consists of pure delay variance.
    profile = sp.exp(-s**2)
    interference = sp.integrate(
        sp.diff(profile, s) * chi**2 / 4 * sp.diff(profile, s, 3),
        (s, -sp.oo, sp.oo),
    )
    assert sp.simplify(interference + 3 * sp.sqrt(2 * sp.pi) * chi**2 / 8) == 0
    print("Flat-end TT contraction, ordered double insertion, and negative whole-pulse control checked.")


def tail_arithmetic():
    """Use the pinned GOAL-005 input envelopes and exact rational inequalities."""
    kmin, kmax = Q("0.394"), Q("0.395")
    qmin, qmax, omin, omax = map(Q, ("0.62", "0.76", "0.06", "0.07"))
    dmin = (qmin - omax)**2 / kmax - Q("0.28")
    dmax = (qmax - omin)**2 / kmin + Q("0.28")
    # Recheck the three tensor component errors from the printed reconstruction.
    fmin = Q("0.99")
    dabs = Q("0.79")
    ett = Q(3, 2) * Q("0.01")**2 / kmin + omax**2 / (kmin * fmin)
    ett += (Q("0.01") / 2 + 3) * dabs / kmin
    etr = omax / kmin * (3 + Q("0.01") / (2 * fmin))
    ek = dabs / kmin
    assert ett < 7 and etr < 4 and ek < 3
    assert ett + 2 * ek < 13

    a = (Q("0.9") - omin) / fmin
    upper = dmax / 2 / fmin / (Q("0.59") - omax)
    assert dmin > Q("0.48") and dmax < Q("1.53")
    assert upper < Q("1.5")

    # Integrate the cubic Taylor minorant on the first radial unit.
    lower = dmin / 2 * Q(100, 101) * (1 - a/2 + a**2/6 - a**3/24)
    assert lower > Q("0.16")
    chi_lower = lower * Q("1e-40") / 101
    chi_upper = upper * Q("3e-21") / 100
    assert chi_lower**2 / 4 > Q("2e-87")
    assert chi_upper**2 / 4 < Q("6e-46")
    print(f"Delay ratio enclosed by the outward-rounded interval (0.16, 1.5).")
    print("Pinned dimensionless delay-square bounds 2e-87 and 6e-46 reproduced.")


if __name__ == "__main__":
    rw_and_shear()
    tt_and_phase()
    tail_arithmetic()
