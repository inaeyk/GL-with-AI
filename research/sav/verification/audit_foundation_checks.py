#!/usr/bin/env python3
"""Independent narrow exact checks for AUDIT_FOUNDATION; nonproduction.

Run: python3 research/sav/verification/audit_foundation_checks.py
Uses direct five-dimensional Christoffels on a flat three-dimensional base
with arbitrary sphere warp R(t,r,z), not the reduced-action field equations.
"""
import platform
import sympy as s


def main():
    print("Python", platform.python_version(), "SymPy", s.__version__)
    t, r, z, th, ph = s.symbols("t r z theta varphi", real=True)
    coords = (t, r, z, th, ph)
    R = s.Function("R")(t, r, z)
    g = s.diag(-1, 1, 1, R**2, R**2*s.sin(th)**2)
    gi = s.diag(-1, 1, 1, R**-2, R**-2/s.sin(th)**2)
    Gamma = {}
    for a in range(5):
        for b in range(5):
            for c in range(b, 5):
                val = s.simplify(sum(gi[a,d]*(s.diff(g[d,c],coords[b])
                    +s.diff(g[d,b],coords[c])-s.diff(g[b,c],coords[d]))
                    for d in range(5))/2)
                if val != 0:
                    Gamma[a,b,c] = val
                    Gamma[a,c,b] = val
    def C(a,b,c):
        return Gamma.get((a,b,c),s.S.Zero)
    Ric = s.zeros(5)
    for a in range(5):
        for b in range(5):
            Ric[a,b] = s.simplify(sum(s.diff(C(c,a,b),coords[c])
                -s.diff(C(c,a,c),coords[b])
                +sum(C(c,c,d)*C(d,a,b)-C(c,b,d)*C(d,a,c)
                     for d in range(5)) for c in range(5)))
    box = -s.diff(R,t,2)+s.diff(R,r,2)+s.diff(R,z,2)
    grad2 = -s.diff(R,t)**2+s.diff(R,r)**2+s.diff(R,z)**2
    scalar = s.simplify(sum(gi[a,b]*Ric[a,b] for a in range(5) for b in range(5)))
    expected = 2/R**2-4*box/R-2*grad2/R**2
    assert s.simplify(scalar-expected)==0
    assert all(s.simplify(Ric[a,b]+2*s.diff(R,coords[a],coords[b])/R)==0
               for a in range(3) for b in range(3))
    assert s.simplify(Ric[3,3]-(1-grad2-R*box))==0
    print("DIRECT_5D_WARP_CURVATURE: PASS (flat g3, arbitrary R(t,r,z))")
    assert s.simplify(scalar-(expected+4*grad2/R**2)) != 0
    assert s.simplify(scalar-(expected-2/R**2)) != 0
    print("MUTATIONS_WARP_GRADIENT_SIGN_AND_SPHERE_CURVATURE: REJECTED")

    r0 = s.symbols("r0", positive=True)
    f = 1-r0/r
    psi = s.Function("psi")(t,r)
    phi = psi/r
    boxphi = -s.diff(phi,t,2)/f+s.diff(r*r*f*s.diff(phi,r),r)/(r*r)
    op = s.diff(psi,t,2)-f*s.diff(f*s.diff(psi,r),r)+f*r0*psi/r**3
    assert s.simplify(-r*f*boxphi-op)==0
    print("RADIAL_SCALAR_OPERATOR_AND_SOURCE_SIGN: PASS")
    assert s.simplify(-r*f*boxphi-(op-2*f*r0*psi/r**3)) !=0
    print("MUTATION_SCALAR_POTENTIAL_SIGN: REJECTED")

    # Five-dimensional transverse traceless scalar polarization, wave radial.
    pol = s.diag(-2/s.sqrt(3),-2/s.sqrt(3),4/s.sqrt(3))
    assert s.trace(pol)==0 and s.trace(pol*pol)==8
    L,G5,G4 = s.symbols("L G5 G4", positive=True)
    factor = s.simplify(4*s.pi*L/(32*s.pi*G5)*s.trace(pol*pol))
    assert s.simplify(factor.subs(G5,L*G4)-1/G4)==0
    assert s.simplify((factor/2).subs(G5,L*G4)-1/G4)!=0
    print("5D_TT_RADION_NORM_8_AND_FLUX_1_OVER_G4: PASS")
    print("MUTATION_FLUX_FACTOR_2: REJECTED")

    # Constant-phi KK vector coefficient from scaling of sqrt(g5) R5.
    p=s.symbols("phi",real=True)
    alpha,beta=-1/s.sqrt(3),2/s.sqrt(3)
    expo=s.simplify((4*alpha+beta)+2*beta-4*alpha)
    assert expo==2*s.sqrt(3)
    assert expo != -2*s.sqrt(3)
    print("KK_VECTOR_EXPONENT_PLUS_2_SQRT3: PASS; NEGATIVE_EXPONENT_REJECTED")

    # Concrete periodic pure gauge, with expansion g+e h+e^2 q.
    # chi has radial and z components b(r)cos(kz),c(r)sin(kz).
    k=s.symbols("k",nonzero=True,real=True)
    b,c=s.Function("b")(r),s.Function("c")(r)
    xx=(t,r,z)
    bg=s.diag(-f,1/f,1)
    chi=s.Matrix([0,b*s.cos(k*z),c*s.sin(k*z)])
    def lie(v,T):
        return s.Matrix(3,3,lambda a,d:s.simplify(sum(v[j]*s.diff(T[a,d],xx[j])
            +T[j,d]*s.diff(v[j],xx[a])+T[a,j]*s.diff(v[j],xx[d])
            for j in range(3))))
    h=lie(chi,bg)
    l2=lie(chi,h)
    q=l2/2
    canonical=s.simplify(q+lie(-chi,h)+lie(-chi,lie(-chi,bg))/2)
    assert canonical==s.zeros(3)
    avg_l2=s.simplify(s.integrate(l2[2,2],(z,0,2*s.pi/k))*k/(2*s.pi))
    assert s.simplify(avg_l2-k**2*(c*c+b*b/f))==0
    assert avg_l2 !=0
    print("SECOND_ORDER_UNITARY_COMPLETION_PURE_GAUGE: PASS")
    print("MUTATION_OMIT_HALF_LIE_SQUARED: REJECTED; spurious qzz0=-k^2(c^2+b^2/f)/2")
    print("SCOPE: exact identities only; not a GL eigenmode, C0, or nonzero-news proof")


if __name__ == "__main__":
    main()
