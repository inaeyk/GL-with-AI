"""Non-production exact checks for GOAL-005, with no generated file writes."""

from math import comb
from itertools import product
import platform

import sympy as sp


def bernstein_checks():
    a, t, A, T = sp.symbols("a t A T")
    D = (a + t**3)**2
    P = (1-t)*((a+(6-11*t)*t**2)*D+18*(1-t)*t**8
               -6*(4-5*t)*t**5*(a+t**3))
    bound = sp.Rational(17, 100)
    b = sp.Rational(9, 10)-sp.Rational(2, 5)*t
    comparison = (b**2-sp.Rational(2, 5)*(1-t)*t**2-bound)*D-P

    def minimum(poly, end, cells):
        minima = []
        for j in range(cells):
            ordinary = sp.Poly(
                poly.subs({a: sp.Rational(197, 500)+A/1000,
                           t: end*(j+T)/cells}),
                A, T,
            ).as_dict()
            assert all(i <= 3 and k <= 10 for i, k in ordinary)
            coefficients = [
                sum(c*sp.Rational(comb(i,l),comb(3,l))
                     *sp.Rational(comb(j,k),comb(10,k))
                     for (l,k), c in ordinary.items() if l<=i and k<=j)
                for i, j in product(range(4), range(11))
            ]
            minima.append(min(coefficients))
        return min(minima)

    results = (
        minimum(P+bound*D, 1, 8),
        minimum(P, sp.Rational(1, 2), 1),
        minimum(comparison, sp.Rational(1, 2), 2),
    )
    assert results == (
        sp.Rational(5718686712103, 3440640000000000),
        sp.Rational(28985859, 2000000000),
        sp.Rational(64026149, 20480000000),
    )
    assert all(value > 0 for value in results)
    assert minimum(P, 1, 8) < 0
    print("Bernstein minima", *results, "| rejected global V>0 claim")


def flat_source_check():
    eta = sp.diag(-1,1,1,1,1)
    A = sp.diag(1,sp.Rational(9,25),sp.Rational(8,25),sp.Rational(8,25),0)
    A[0,1] = A[1,0] = -sp.Rational(3,5)
    p = sp.Matrix([3,-5,0,0,4*sp.I])
    dim = range(5)
    assert (p.T*eta*p)[0] == 0 and p.T*eta*A == sp.zeros(1,5)
    assert sp.trace(eta*A) == 0
    C = {
        (a,b,c): sum(eta[a,d]*(p[b]*A[d,c]+p[c]*A[d,b]-p[d]*A[b,c])
                     for d in dim)/2
        for a,b,c in product(dim, repeat=3)
    }
    D = {(a,b,c): -sum((eta*A)[a,d]*C[d,b,c] for d in dim)
         for a,b,c in C}
    linear = sp.Matrix(5,5,lambda b,c: sum(p[a]*C[a,b,c]-p[c]*C[a,b,a]
                                           for a in dim))
    ricci = sp.Matrix(5,5,lambda b,c: sum(
        2*(p[a]*D[a,b,c]-p[c]*D[a,b,a])
        +sum(C[a,a,d]*C[d,b,c]-C[a,c,d]*C[d,b,a] for d in dim)
        for a in dim))
    assert linear == sp.zeros(5)
    norm = sp.trace((eta*A)**2)
    assert ricci[4,4] == -3*16*norm/4 == -sp.Rational(4608,625)
    assert ricci[4,4] != -16*norm/4
    assert p.T*eta*(ricci-eta*sp.trace(eta*ricci)/2) == sp.zeros(1,5)
    four = range(4)
    for b,c in product(four, repeat=2):
        r4 = sum(2*(p[a]*D[a,b,c]-p[c]*D[a,b,a])
                 +sum(C[a,a,d]*C[d,b,c]-C[a,c,d]*C[d,b,a] for d in four)
                 for a in four)
        assert ricci[b,c] == r4 - 8*(A*eta*A)[b,c]
    for b in four:
        mixed = 4*sp.I*(-(p.T*eta*A*eta*A)[b]/2+3*p[b]*norm/4)
        assert ricci[4,b] == mixed
    print("Flat TT Ricci and Bianchi passed | rejected incorrect +2 zz factor")


def constraint_check():
    r,z = sp.symbols("r z", positive=True)
    w,g = (sp.Function(s)(r,z) for s in ("w", "g"))
    Z = sp.Function("Z")(r,z)
    u0 = r/(r+1)
    U = u0+w/r
    a = U**(-sp.Rational(1,2))
    h = 1/r+g.diff(r)
    H = 2*h
    B = 4*h.diff(r)+6*h*h
    curvature = 2*r**-2*sp.exp(-2*g)-4*g.diff(z,2)-6*g.diff(z)**2
    ar = a*a*(a.diff(z,2)+2*g.diff(z)*a.diff(z))/H
    ar += (a*B-a**3*(curvature+Z))/(2*H)
    from_original = U-u0+r*(-2*a**-3*ar-u0.diff(r))
    rhs = (w.diff(z,2)+2*g.diff(z)*w.diff(z))/(H*U)
    rhs -= 3*w.diff(z)**2/(2*r*H*U*U)
    rhs += (1/r-B/H)*w+r*((curvature+Z-u0*B)/H-u0.diff(r))
    assert sp.factor(from_original-rhs) == 0
    assert sp.simplify(from_original-rhs+2*g.diff(z)*w.diff(z)/(H*U)) != 0
    Z0 = -2/(r*r*(r+1)**2)
    assert sp.simplify(rhs.subs({w:0,g:0,Z:Z0}).doit()) == 0
    print("Weighted lapse identity and background passed | rejected missing leaf drift")


if __name__ == "__main__":
    print("Python", platform.python_version(), "SymPy", sp.__version__)
    bernstein_checks()
    flat_source_check()
    constraint_check()
