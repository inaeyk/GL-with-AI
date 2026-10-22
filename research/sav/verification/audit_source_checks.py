#!/usr/bin/env python3
"""Independent non-production checks for the GL quadratic radion source.

No physical GL eigenmode/ODE is solved. The final cancellation is manufactured,
not a statement about the physical GL overlap.
Run: python3 research/sav/verification/audit_source_checks.py
"""
import platform
import sympy as sp


def main():
    print("Python", platform.python_version(), "SymPy", sp.__version__)
    th = sp.symbols("th", real=True)
    f, r, j, k = sp.symbols("f r j k", positive=True)
    a, b, c, d = sp.symbols("a b c d", real=True)
    gb = sp.diag(-f, 1/f, r*r, r*r*j*j)
    H = sp.Matrix([[f*a, b, 0, 0], [b, c/f, 0, 0],
                   [0, 0, r*r*d, 0], [0, 0, 0, r*r*j*j*d]])
    M = gb.inv()*H
    norm = sp.factor(sp.trace(M*M))
    assert norm == a*a-2*b*b+c*c+2*d*d
    print("LORENTZ_CONTRACTION: PASS; H2 =", norm)

    # Christoffels of ds^2=dz^2+g_mn dx^m dx^n:
    # Gamma^m_zn=(1/2)g^{mp}g_pn,z. Expand connection directly.
    G1 = -k*sp.sin(th)*M/2
    G2 = k*sp.sin(th)*sp.cos(th)*(M*M)/2
    Ric2 = -k*sp.diff(sp.trace(G2), th)-sp.trace(G1*G1)
    Qzz = sp.simplify(sp.integrate(sp.expand_trig(Ric2),
                                  (th, 0, 2*sp.pi))/(2*sp.pi))
    assert sp.simplify(Qzz+k*k*norm/8) == 0
    print("DIRECT_CONNECTION_RZZ_ZERO_MODE: PASS; Qzz = -k^2 H2/8")

    # Independent cubic-action route. At TT first order,
    # L_z^(2)=-(1/4)e^{-2sqrt(3)phi} tr(h_,z^2).
    phi, K2 = sp.symbols("phi K2", real=True)
    Lz = -sp.exp(-2*sp.sqrt(3)*phi)*K2/4
    cubic_variation = sp.diff(Lz, phi).subs(phi, 0)
    box_phi = sp.simplify(-cubic_variation/4)
    assert box_phi == -sp.sqrt(3)*K2/8
    assert sp.simplify(box_phi.subs(K2,k*k*norm/2)
                       +sp.sqrt(3)*k*k*norm/16) == 0
    print("CUBIC_ACTION_RADION_SOURCE: PASS; box phi = -sqrt(3) k^2 H2/16")

    # Intentional errors must fail exact identities.
    assert sp.simplify(Qzz-k*k*norm/8) != 0
    assert sp.simplify(Qzz+k*k*norm/4) != 0
    assert sp.simplify(box_phi.subs(K2,k*k*norm/2)
                       +sp.sqrt(3)*k*k*norm/8) != 0
    assert sp.simplify(norm-(a*a+2*b*b+c*c+2*d*d)) != 0
    print("SIGN_FOURIER_AND_LORENTZ_MUTATIONS: REJECTED (4)")

    # General Wronskian jump: ( -d_x^2 + V + s^2 )G=delta.
    # G = -yH(x<) yI(x>)/W; derivative jump is -1.
    yH, yHp, yI, yIp = sp.symbols("yH yHp yI yIp")
    W = yH*yIp-yHp*yI
    jump = sp.cancel((-yH*yIp+yHp*yI)/W)
    assert jump == -1
    assert -jump == 1
    assert jump != 1  # wrong +1/W convention has wrong delta sign
    print("RETARDED_GREEN_JUMP: PASS; wrong Wronskian sign rejected")

    # Exact Schwarzschild s=0 scalar basis and Wronskian.
    rr, r0 = sp.symbols("rr r0", positive=True)
    ff = 1-r0/rr
    VV = ff*r0/rr**3
    yyH = rr/r0
    yyI = -rr*sp.log(ff)/r0
    for yy in (yyH, yyI):
        assert sp.simplify(-ff*sp.diff(ff*sp.diff(yy,rr),rr)+VV*yy) == 0
    WW = sp.simplify(ff*(yyH*sp.diff(yyI,rr)-sp.diff(yyH,rr)*yyI))
    assert WW == -1/r0
    print("STATIC_SCHWARZSCHILD_BASIS: PASS; W = -1/r0")

    # Nonzero source need not yield outgoing amplitude, even with positive kernel.
    # Flat manufactured problem: q=e^{-a x^2}, S=(-d_x^2+s^2)q.
    # This exact overlap cancels; it is NOT the physical black-string source.
    x = sp.symbols("x", real=True)
    aa, ss = sp.symbols("aa ss", positive=True)
    q = sp.exp(-aa*x*x)
    S = -sp.diff(q,x,2)+ss*ss*q
    primitive = sp.exp(ss*x)*(ss*q-sp.diff(q,x))
    assert sp.simplify(sp.diff(primitive,x)-sp.exp(ss*x)*S) == 0
    overlap = sp.integrate(sp.exp(ss*x)*S,(x,-sp.oo,sp.oo))
    assert sp.simplify(overlap) == 0
    assert sp.simplify(S) != 0
    print("MANUFACTURED_NONZERO_SOURCE_ZERO_OUTGOING_OVERLAP: PASS")
    print("SCOPE: no physical GL overlap, coefficient, or energy fraction certified")


if __name__ == "__main__":
    main()
