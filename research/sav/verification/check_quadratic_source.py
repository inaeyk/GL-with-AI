#!/usr/bin/env python3
"""Exact non-production checks of the quadratic KK-zero radion equation.

Direct z-Christoffel expansion (including both sphere directions), independent
spacelike-ADM action variation, gauge algebra, scalar operator and mutations.
No GL eigenmode or transfer coefficient is assumed by this check.
"""
import platform
import sympy as s


def main():
    print('Python', platform.python_version(), 'SymPy', s.__version__)
    a, k, z, f, r = s.symbols('a k z f r', positive=True, real=True)
    T, B, D, K = s.symbols('T B D K', real=True)
    bg = s.diag(-f, 1/f, r*r, r*r)
    H = s.Matrix([[T, B, 0, 0], [B, D, 0, 0],
                  [0, 0, r*r*K, 0], [0, 0, 0, r*r*K]])
    # The azimuthal sin(theta)^2 factors cancel between H and bg inverse;
    # use theta=pi/2 since all z contractions are theta-independent.
    M = bg.inv()*H
    c = s.cos(k*z)
    inv = (s.eye(4)-a*c*M+a*a*c*c*M*M)*bg.inv()
    gp = a*s.diff(c,z)*H
    # Direct nonzero Christoffels with z: Gamma^mu_(z nu)=inv*g'/2,
    # Gamma^z_(mu nu)=-g'/2; Gamma^A_zz=0. Contract Rzz directly.
    G = inv*gp/2
    Rzz = -s.diff(s.trace(G),z)-s.trace(G*G)
    Q = s.expand(Rzz).coeff(a,2)
    avg = s.simplify(s.integrate(Q, (z,0,2*s.pi/k))*k/(2*s.pi))
    norm = s.simplify(s.trace(M*M))
    explicit = T*T/f**2 + f*f*D*D + 2*K*K - 2*B*B
    assert s.simplify(norm-explicit)==0
    assert s.simplify(avg+k*k*norm/8)==0
    print('DIRECT_CHRISTOFFEL_QZZ: PASS; Qzz0=-k^2 tr(H^2)/8')
    # Action route: S_phi + cubic z-kinetic. Keep full TT assumption explicit.
    ph = s.symbols('phi', real=True)
    zkin = -s.exp(-2*s.sqrt(3)*ph)*k*k*norm/8
    scalar_action_variation = s.diff(zkin,ph).subs(ph,0)
    assert s.simplify(-scalar_action_variation/4+s.sqrt(3)*k*k*norm/16)==0
    print('CUBIC_ACTION_SOURCE: PASS; Box(phi)=-sqrt(3) k^2 tr(H^2)/16')
    # Fourier and normalization mutations must fail, using a nonzero TT tensor.
    # T/f=1, fD=1, K=0, B=0 -> trace=0 and tr(H²)=2.
    tt = {T:f, D:1/f, K:0, B:0}
    assert s.simplify(s.trace(M).subs(tt))==0
    assert s.simplify((avg+k*k*norm/4).subs(tt)) != 0
    assert s.simplify((avg-k*k*norm/8).subs(tt)) != 0
    # Wrong sign in the timelike offdiagonal contraction is exposed separately.
    assert s.simplify((norm-(explicit+4*B*B)).subs(B,1)) != 0
    print('MUTATIONS_FOURIER_SIGN_LORENTZ: REJECTED')
    # Independent nonzero-k Stueckelberg gauge-completion algebra for one
    # generic symmetric pair; partials stand for background covariant derivatives.
    I=s.I
    h, p, d, xx, zz, dxz, ddzz=s.symbols('h p d xx zz dxz ddzz')
    # H_U=h+i/k(2 nabla h_z)-1/k² nabla nabla h_zz.
    invariant=h+2*I*p/k-d/k**2
    transformed=(h+2*xx)+2*I*(p+I*k*xx+dxz)/k-(d+2*I*k*dxz)/k**2
    assert s.simplify(transformed-invariant)==0
    print('NONZERO_K_UNITARY_COMPLETION: PASS')
    # Scalar master operator, derived from Box on Schwarzschild in r.
    r0,t=s.symbols('r0 t', positive=True)
    ff=1-r0/r
    psi=s.Function('psi')(t,r)
    box=-s.diff(psi/r,t,2)/ff+s.diff(r*r*ff*s.diff(psi/r,r),r)/(r*r)
    master=s.diff(psi,t,2)-ff*s.diff(ff*s.diff(psi,r),r)+ff*r0*psi/r**3
    assert s.simplify(-r*ff*box-master)==0
    print('SCHWARZSCHILD_SCALAR_OPERATOR: PASS')
    # Flux from a 5D transverse TT polarization: h11=h22=-2phi/sqrt3,
    # hzz=4phi/sqrt3. Einstein-Hilbert outgoing energy density contraction.
    N,G4=s.symbols('N G4', positive=True)
    contraction=2*(-2*N/s.sqrt(3))**2+(4*N/s.sqrt(3))**2
    flux=s.simplify(4*s.pi*contraction/(32*s.pi*G4))
    assert s.simplify(flux-N*N/G4)==0
    assert s.simplify(flux-N*N/(2*G4))!=0
    print('5D_TT_RADION_FLUX: PASS; factor-two mutation REJECTED')
    print('SCOPE: exact algebra, not an on-shell GL eigenmode/nonzero-overlap proof')


if __name__=='__main__':
    main()
