"""Exact KK, tensor norm, and manufactured retarded checks, with no evolution."""

from collections import defaultdict
import platform

import sympy as sp


def kk_product(a, b):
    terms = defaultdict(int)
    for n, a_n in a.items():
        for m, b_m in b.items():
            terms[n + m] += a_n * b_m
    return {n: sp.simplify(term) for n, term in terms.items() if term != 0}


def kk_checks():
    cosine = {-1: sp.S.Half, 1: sp.S.Half}
    sine = {-1: sp.I / 2, 1: -sp.I / 2}
    ringdown = {0: 1}
    massive_q = {-2: sp.Rational(1, 4), 2: sp.Rational(1, 4)}
    mixed = kk_product(cosine, ringdown)
    square = kk_product(cosine, cosine)
    assert mixed == cosine
    assert square == {**massive_q, 0: sp.S.Half}
    assert kk_product(cosine, mixed)[0] == sp.S.Half
    assert kk_product(square, ringdown)[0] == sp.S.Half
    assert 0 not in kk_product(massive_q, ringdown)
    assert 0 not in kk_product(sine, mixed)
    print("KK: mixed support ±1, two opposite-sign pairings give average 1/2")
    print("KK negative controls: ±2 × 0 and sine × cosine have no zero coefficient")


def tensor_checks():
    x = sp.symbols("x", real=True)
    gamma = sp.diag(1 / (1 - x*x), 1 - x*x)
    inverse = gamma.inv()
    # Connections Gamma^x_xx and Gamma^x_phiphi in x=cos(theta).
    conn_xx = x / (1 - x*x)
    conn_pp = x * (1 - x*x)
    eps_mixed = sp.Matrix([[0, -1], [1, 0]]) * inverse

    def even(ell):
        y = sp.sqrt((2 * ell + 1) / (4 * sp.pi)) * sp.legendre(ell, x)
        hess = sp.diag(y.diff(x, 2) - conn_xx * y.diff(x), -conn_pp * y.diff(x))
        return (hess + ell * (ell + 1) * gamma * y / 2).applyfunc(sp.simplify)

    def inner(a, b):
        integrand = sp.simplify(sp.trace(inverse * a * inverse * b))
        return sp.simplify(2 * sp.pi * sp.integrate(integrand, (x, -1, 1)))

    even2 = even(2)
    odd2 = (eps_mixed * even2 + (eps_mixed * even2).T) / 2
    assert even(0) == even(1) == sp.zeros(2)
    assert inner(even2, even2) == inner(odd2, odd2) == 12
    assert inner(even2, odd2) == 0

    # A trace-free two-tensor with plus and cross coefficients in an
    # orthonormal dyad has norm-squared twice the sum of their squares.
    plus, cross = sp.symbols("n_plus n_cross")
    news = sp.Matrix([[plus, cross], [cross, -plus]])
    assert sp.trace(news * news) == 2 * plus**2 + 2 * cross**2
    print("Unit Y20 even/odd tensor norms = 12 each, mutual product = 0")
    print("Trace-free l=0,1 tensors vanish; dyad contraction = 2(n_plus²+n_cross²)")


def retarded_checks():
    t, u = sp.symbols("t u", real=True)
    r = sp.symbols("r", positive=True)
    profile = ((u + 2) * (u + 1))**6

    def outgoing(profile):
        wave = profile.diff(u, 2) + 3 * profile.diff(u) / r + 3 * profile / r**2
        return wave.subs(u, t - r)

    def master(wave):
        return wave.diff(t, 2) - wave.diff(r, 2) + 6 * wave / r**2

    wave = outgoing(profile)
    assert sp.cancel(master(wave)) == 0
    assert sp.cancel(master(profile.diff(u, 2).subs(u, t - r))) != 0
    # The polynomial wave is continued by zero outside -2 <= u <= -1.
    # The endpoint derivatives below guarantee the required matching.
    assert all(profile.diff(u, i).subs(u, b) == 0
               for b in (-2, -1) for i in range(6))

    ramp = 6*t**5 - 15*t**4 + 10*t**3
    assert ramp.subs(t, 0) == 0 and ramp.subs(t, 1) == 1
    assert all(ramp.diff(t, i).subs(t, b) == 0 for b in (0, 1) for i in (1, 2))
    source = ramp.diff(t, 2) * wave + 2 * ramp.diff(t) * wave.diff(t)
    assert sp.cancel(master(ramp * wave) - source) == 0
    assert sp.cancel(source) != 0
    assert sp.simplify((ramp * wave).subs(t, 0)) == 0
    assert sp.simplify((ramp * wave).diff(t).subs(t, 0)) == 0
    # The same nonzero homogeneous wave fails the retarded zero-data condition.
    assert sp.simplify(wave.subs({t: 0, r: sp.Rational(3, 2)})) != 0

    null_wave = sp.limit(wave.subs(t, u + r), r, sp.oo)
    assert null_wave == sp.factor(profile.diff(u, 2)) or sp.simplify(
        null_wave - profile.diff(u, 2)) == 0
    n = profile.diff(u, 3)
    g4 = sp.symbols("G_4", positive=True)
    energy = sp.factor(12 * sp.integrate(n*n, (u, -2, -1)) / (32 * sp.pi * g4))
    assert energy > 0
    assert sp.integrate(n * n.diff(u), (u, -2, -1)) == 0
    window_shift = sp.integrate(n * n.diff(u), (u, -2, -sp.Rational(7, 4)))
    assert window_shift > 0
    print("Flat l=2 manufactured retarded wave and zero initial pair verified")
    print("Retarded negative controls: omit near-zone terms or add a free homogeneous wave")
    print("Integrated single-polarization energy =", energy)
    print("Constant time-shift interference: full pulse 0, subwindow integral", window_shift)


if __name__ == "__main__":
    print("Python", platform.python_version(), "SymPy", sp.__version__)
    kk_checks()
    tensor_checks()
    retarded_checks()
