"""Bounded manufactured GLR-011 signed-adjoint and massive-tail controls."""

from cmath import sqrt
from math import exp, pi

import numpy as np
import sympy as sy
from flint import arb, ctx


def adjoint_control():
    """Integrate an exact complex polynomial problem on two matched legs."""
    x = sy.symbols("x", real=True)
    i = sy.I
    vec = sy.Matrix
    U = vec([1 + i*x + x**2, i - x + x**3])
    J = vec([1 + i + x**2, 2*i + x - x**2])
    P_left = vec([[x + i, 1 - i*x], [x**2, -1 + i*x]])
    P_right = vec([[x - i, 2 + i*x], [1 - x**2, 1 + i*x]])
    z_left = vec([1 + i*x, 2*x + x**2])
    z_right = vec([i*(1-x), 1-x**2])
    w_left = vec([x + x**2 + i, 1 + i*x])
    w_right = vec([2*x + i, 1 - x + i*x**2])
    a = vec([1 + x**2, x - i])
    b = vec([x + i, 1 - x**2])
    c = i*x**2 + x
    z_collar = vec([1, 2 + i])
    C = 4 - i

    def scalar(v, w):
        return (v.T * w)[0]

    def integrate(f, lo, hi):
        polynomial = sy.Poly(sy.expand(f), x)
        return sum(value * (hi**(power[0]+1) - lo**(power[0]+1)) / (power[0]+1)
                   for power, value in polynomial.terms())

    pieces = []
    for P, z, w, lo, hi in (
            (P_left, z_left, w_left, -1, 0),
            (P_right, z_right, w_right, 0, 1)):
        F = U.diff(x) - J
        L = J - P*U
        H = L.diff(x) + P*L
        dz = z.diff(x) + P.T*z + a
        dw = w.diff(x) - P.T*w + z + b
        direct = integrate(scalar(a,U) + scalar(b,L) + c, lo, hi)
        signed = integrate(scalar(z,F) + scalar(w,H) + c, lo, hi)
        defect = integrate(scalar(dz,U) + scalar(dw,L), lo, hi)
        hermitian = integrate(scalar(sy.conjugate(dz),U) +
                              scalar(sy.conjugate(dw),L), lo, hi)
        pieces.append((direct, signed, defect, hermitian, L))

    left, right = pieces
    q = w_left.subs(x,0)
    assert q == w_right.subs(x,0) and z_right.subs(x,1) == vec([0,0])
    match = scalar((z_right-z_left+(P_left-P_right).T*q).subs(x,0),U.subs(x,0))
    collar = C + scalar((z_left-z_collar).subs(x,-1),U.subs(x,-1))
    collar += scalar(w_left.subs(x,-1),left[4].subs(x,-1))
    far = -scalar(w_right.subs(x,1),right[4].subs(x,1))
    reference = sy.expand(left[0] + right[0] + C - scalar(z_collar,U.subs(x,-1)))
    result = sy.expand(sum(part[1]+part[2] for part in pieces)+match+collar+far)
    assert result == reference
    wrong_match = sy.expand(result-match)
    wrong_dual = sy.expand(result+sum(part[3]-part[2] for part in pieces))
    assert wrong_match != reference and wrong_dual != reference
    print("exact matched adjoint", result)
    print("missing match discrepancy", sy.expand(wrong_match-reference))
    print("conjugating defect discrepancy", sy.expand(wrong_dual-reference))


def interval_controls():
    ctx.prec = 128
    h = arb(1)/4
    error = h**3/3 - h*(h/2)**2
    assert sy.Rational(1, 192) - sy.Rational(1, 256) == sy.Rational(1, 768)
    assert error.overlaps(h**3/12)
    print("exact midpoint error for t^2 and h=1/4", error)
    # Nonnormal stable spectrum gives no unit-prefactor exponential bound.
    evolution = exp(-0.5)*np.array([[1.0, 2.0], [0.0, 1.0]])
    actual = np.linalg.norm(evolution, ord=2)
    assert exp(-0.5) < actual < exp(0.5)
    print("nonnormal evolution norm and spectral shortcut", actual, exp(-0.5))

    coordinates = ("-.18746108862784384", "-.41681210448350057",
                   "-.0588905954473658", "-.49465461340480154")
    center_norm = sum(arb(float(v))**2 for v in coordinates).sqrt()
    bound = arb(".33257")
    # ||4U|| >= 4(||center||-bound) > ||center||+bound.
    assert 4*(center_norm-bound) > center_norm+bound
    print("match norm upper / lower after 4x packet", center_norm+bound, 4*(center_norm-bound))


def massive_tail_control():
    """Point stress test of the majorant via an independent potential form."""
    k = pi/5
    w = (1+1j)/sqrt(2)
    largest = 0.0
    for omega in (0.06328118, 0.06328119, 0.06328120):
        for ds in (0, 5e-8*(1+1j), 5e-8*(-1+1j), 5e-8*(1-1j), -5e-8*(1+1j)):
            s1 = -0.177924631-0.747343369j + ds - omega
            kappa = -sqrt(s1*s1+k*k)
            alpha = k*k/(2*kappa)
            for rho in (64, 64.125, 80, 128, 512, 2048):
                r = 2.5+w*rho
                f = 1-1/r
                f_r = 1/r**2
                d = 4+k*k*r*r
                b0 = k*k*r/d
                b0_r = k*k/d - 2*k**4*r*r/d**2
                a = f*(1/r+b0)
                b = f*(2/r-b0)
                a_x = f*(f_r*(1/r+b0) + f*(-1/r**2+b0_r))
                b_x = f*(f_r*(2/r-b0) + f*(-2/r**2-b0_r))
                ell = -kappa + alpha*f/r
                ell_x = alpha*f*(f_r/r-f/r**2)
                diagonal = s1*s1+f*(k*k+4/r**2)-ell*ell-ell_x
                off_diagonal = -4j*k*s1*f/d
                Q = np.array([[diagonal+a*a-a_x, off_diagonal],
                              [off_diagonal, diagonal+b*b+b_x]])
                scaled = np.linalg.norm(Q, ord="fro")*(rho/64)**2
                largest = max(largest, scaled)
    assert largest < 0.003090065
    print("max scaled massive tail Q Frobenius norm, 90 points", f"{largest:.12g}")


if __name__ == "__main__":
    adjoint_control()
    interval_controls()
    massive_tail_control()
