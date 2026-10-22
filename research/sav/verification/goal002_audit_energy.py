#!/usr/bin/env python3
"""Independent GOAL-002 audit II: exact constraints, energy and data freedom.

Non-production symbolic checks and manufactured 1D quadratures only.
No physical GL eigenvalue/BVP or spacetime evolution is executed.
"""
from pathlib import Path
import json
import platform
import sympy as s
import numpy as np
import scipy
from scipy.integrate import quad
from scipy.special import erfc, logsumexp

HERE = Path(__file__).resolve().parent


def main():
    checks = []
    rejected = []

    def passed(name):
        checks.append(name)
        print(name + ': PASS')

    def reject(name, difference):
        assert s.simplify(difference) != 0
        rejected.append(name)
        print(name + ': REJECTED')

    print('Python', platform.python_version(), 'SymPy', s.__version__,
          'NumPy', np.__version__, 'SciPy', scipy.__version__)
    r, th, ph, r0 = s.symbols('r theta varphi r0', positive=True)
    xx = (r, th, ph)
    a = s.Function('a')(r)
    kr, kt = s.Function('kR')(r), s.Function('kT')(r)
    F, P = s.Function('F')(r), s.Function('P')(r)
    g = s.diag(a*a, r*r, r*r*s.sin(th)**2)
    gi = g.inv()
    C = {}
    for i in range(3):
        for j in range(3):
            for k in range(3):
                c = s.simplify(sum(gi[i,l]*(s.diff(g[l,k],xx[j])
                    +s.diff(g[l,j],xx[k])-s.diff(g[j,k],xx[l]))
                    for l in range(3))/2)
                if c != 0:
                    C[i,j,k] = c

    def cc(i,j,k):
        return C.get((i,j,k), s.S.Zero)

    ric = s.Matrix(3,3,lambda i,j:s.simplify(sum(
        s.diff(cc(k,i,j),xx[k])-s.diff(cc(k,i,k),xx[j])
        +sum(cc(k,k,l)*cc(l,i,j)-cc(k,j,l)*cc(l,i,k)
             for l in range(3)) for k in range(3))))
    R = s.simplify(s.trace(gi*ric))
    m = r*(1-a**-2)/2
    assert s.simplify(R-4*s.diff(m,r)/r**2) == 0
    passed('DIRECT_3D_SCALAR_CURVATURE_R_EQUALS_4_MPRIME_OVER_R2')
    kmix = s.diag(kr,kt,kt)
    # Direct covariant divergence of the (1,1) tensor K, minus d(tr K).
    mom = s.simplify(sum(s.diff(kmix[j,0],xx[j])
        +sum(cc(j,j,l)*kmix[l,0]-cc(l,j,0)*kmix[j,l]
             for l in range(3)) for j in range(3))
        -s.diff(s.trace(kmix),r))
    assert s.simplify(mom-2*((kr-kt)/r-s.diff(kt,r))) == 0
    passed('DIRECT_RADIAL_MOMENTUM_DIVERGENCE')
    # K_ij=-L_n gamma_ij/2: D_jK^j_i-D_iK=-2 Pi D_i phi.
    krsol = kt+r*s.diff(kt,r)-r*P*s.diff(F,r)
    assert s.simplify(mom.subs(kr,krsol)+2*P*s.diff(F,r)) == 0
    passed('SCALAR_MOMENTUM_CONSTRAINT_SOLUTION')
    reject('MOMENTUM_SOURCE_SIGN_MUTATION',
           mom.subs(kr,kt+r*s.diff(kt,r)+r*P*s.diff(F,r))
           +2*P*s.diff(F,r))
    ham = R+s.trace(kmix)**2-s.trace(kmix*kmix)
    expected = 4*s.diff(m,r)/r**2+4*kr*kt+2*kt**2
    assert s.simplify(ham-expected) == 0
    passed('DIRECT_HAMILTONIAN_EXTRINSIC_CURVATURE_FACTORS')
    reject('HAMILTONIAN_SCALAR_FACTOR_TWO_MUTATION',
           2*(P**2+s.diff(F,r)**2/a**2)
           -(P**2+s.diff(F,r)**2/a**2))

    k0 = r0/(r*s.sqrt(r*(r+r0)))
    m0 = r*r0/(2*(r+r0))
    kr0 = k0+r*s.diff(k0,r)
    assert s.simplify(4*s.diff(m0,r)/r**2+4*kr0*k0+2*k0*k0) == 0
    passed('HORIZON_CROSSING_SCHWARZSCHILD_CONSTRAINTS')
    delta, dm, dmp = s.symbols('delta dm dmprime', real=True)
    ff, pp, ffp = s.symbols('F P Fprime', real=True)
    ao_inv = r/(r+r0)
    S2 = r*r*(pp**2+ao_inv*ffp**2+2*r*k0*pp*ffp)/2
    # Substitute the full data into the Hamiltonian residual and solve for dm'.
    mfull = m0+dm
    krfull = kr0-r*delta**2*pp*ffp
    hamfull = (4*(s.diff(m0,r)+dmp)/r**2+4*krfull*k0+2*k0*k0
               -2*delta**2*(pp**2+(1-2*mfull/r)*ffp**2))
    assert s.simplify(hamfull-4*(dmp+delta**2*r*ffp**2*dm-delta**2*S2)/r**2) == 0
    passed('EXACT_SCALAR_PULSE_LINEAR_MASS_ODE')
    assert s.simplify(ao_inv-r*r*k0*k0-(1-r0/r)) == 0
    passed('EXTERIOR_POSITIVE_SCALAR_MASS_SOURCE_DETERMINANT_EQUALS_F')
    reject('OMIT_SCALAR_MOMENTUM_BACKREACTION_MUTATION',
           S2-r*r*(pp**2+ao_inv*ffp**2)/2)

    # Canonical scalar reduction. d/dx=f d/dr; retain the boundary term.
    f = 1-r0/r
    q = s.Function('q')(r)
    qx = f*s.diff(q,r)
    V = f*r0/r**3
    kinetic_difference = (qx-f*q/r)**2-qx**2-V*q*q
    assert s.simplify(kinetic_difference+f*s.diff(f*q*q/r,r)) == 0
    passed('SCALAR_KILLING_ENERGY_REDUCTION_WITH_BOUNDARY_TERM')
    reject('NEGATIVE_SCHWARZSCHILD_POTENTIAL_MUTATION',
           kinetic_difference+f*s.diff(f*q*q/r,r)+2*V*q*q)

    h, px, FF, PP = s.symbols('h Fx F P', real=True)
    # On tau=t+h(x), h'=r0/r; psi_x|t = F_x+h' P.
    dens = ((PP**2+(px+h*PP)**2+V*FF**2)/2
            -h*PP*(px+h*PP))
    target = ((1-h*h)*PP**2+px*px+V*FF**2)/2
    assert s.expand(dens-target) == 0
    passed('HORIZON_CROSSING_KILLING_ENERGY_DENSITY')
    fformal = s.symbols('f', real=True)
    gef = s.Matrix([[-fformal,1],[1,0]])
    eframe = s.Matrix([[1,fformal/2-1],[1,fformal/2+1]])/s.sqrt(2)
    assert s.simplify(eframe*gef*eframe.T) == s.diag(-1,1)
    passed('CENTRAL_SCALAR_SCRIPT_EF_TETRAD_ORTHONORMALITY')
    # A right-moving flat wave has e=F'^2 and outward flux=F'^2.
    z = s.symbols('z', real=True)
    assert s.simplify((z*z+(-z)**2)/2-z*z) == 0
    passed('NULL_FLUX_EQUALS_NEWS_SQUARED_NO_EXTRA_HALF')
    reject('NEWS_FLUX_HALF_MUTATION',z*z/2-z*z)

    # Relative energy target is with respect to the predicted energy.
    eta = s.sqrt(s.Rational(11,10))-1
    assert s.simplify(2*eta+eta**2-s.Rational(1,10)) == 0
    passed('TEN_PERCENT_ENERGY_L2_ERROR_THRESHOLD')
    QQ, ssym, CC = s.symbols('Q s C', positive=True)
    exp2sT = 1+2*QQ/(eta**2*ssym*CC**2)
    Ip = ssym*CC**2*(exp2sT-1)/2
    assert s.simplify(QQ/Ip-eta**2) == 0
    passed('CENTRAL_SCALAR_SCRIPT_TRANSIENT_WINDOW_ALGEBRA')
    reject('TEN_PERCENT_NEWS_IMPLIES_TEN_PERCENT_ENERGY_MUTATION',
           2*s.Rational(1,10)+s.Rational(1,10)**2-s.Rational(1,10))
    proxy_ratio = s.Rational(1,10)*10/100
    actual_ratio = s.Rational(1,10)*10/1
    assert proxy_ratio <= s.Rational(1,10) and actual_ratio > s.Rational(1,10)
    reject('UPPER_BOUND_DENOMINATOR_HIERARCHY_MUTATION',actual_ratio-proxy_ratio)
    bg5 = s.diag(-1,1,1,1,1)
    perturb5 = s.diag(s.zeros(1),-s.Rational(3,10)*s.ones(4))
    spatial_vector = s.Matrix([0,1,1,1,1])
    assert max(abs(x) for x in perturb5) < 1
    assert (spatial_vector.T*(bg5+perturb5)*spatial_vector)[0] < 0
    reject('COMPONENTWISE_SMALLNESS_IMPLIES_METRIC_SIGNATURE_MUTATION',
           (spatial_vector.T*(bg5+perturb5)*spatial_vector)[0])

    A, c, Om, gam, d, B = s.symbols('A c Omega gamma d B', real=True)
    Adot = Om*A+gam*A**3
    Bdot = (1+3*c*A*A)*Adot/(1+d*A*A)
    rhs = s.series(Bdot,A,0,4).removeO()
    rhsB = s.series(rhs.subs(A,B-c*B**3),B,0,4).removeO().expand()
    assert s.simplify(rhsB-(Om*B+(gam+2*c*Om-d*Om)*B**3)) == 0
    passed('CUBIC_AMPLITUDE_AND_CLOCK_REDEFINITION')
    reject('GAMMA_INVARIANT_AT_FINITE_OMEGA_MUTATION',2*c*Om)
    b, aa, cross, dd = s.symbols('b a cross d', real=True)
    poly = aa+2*b*cross+b*b*dd
    assert s.diff(poly,b,2) == 2*dd
    passed('FREE_SCALAR_DATA_NONUNIVERSAL_ENERGY_QUADRATIC')
    reject('DROP_ENERGY_INTERFERENCE_MUTATION',poly-(aa+b*b*dd))
    eps, p2, p4 = s.symbols('epsilon p2 p4', real=True)
    stress = s.expand((eps**2*p2+eps**4*b*p4)**2)
    assert stress.coeff(eps,4) == p2*p2
    assert stress.coeff(eps,6) == 2*b*p2*p4
    assert stress.coeff(eps,8) == b*b*p4*p4
    passed('FOURTH_ORDER_FREE_RADION_UNBOUNDED_BY_LOWER_ORDER_DATA')

    # Smooth finite-energy manufactured news and frequency transform.
    # These are exact flat outgoing-wave controls, NOT Schwarzschild GL news.
    u, w = s.symbols('u omega', real=True)
    tau, amp = s.symbols('tau B', positive=True)
    gaussian_news = amp/(s.sqrt(s.pi)*tau)*s.exp(-u*u/tau**2)
    Itime = s.integrate(gaussian_news**2,(u,-s.oo,s.oo))
    Iexpected = amp**2/(s.sqrt(2*s.pi)*tau)
    assert s.simplify(Itime-Iexpected) == 0
    Iomega = s.integrate(amp**2*s.exp(-tau*tau*w*w/2)/s.pi,(w,0,s.oo))
    assert s.simplify(Iomega-Itime) == 0
    passed('SMOOTH_MEMORY_NEWS_POSITIVE_FREQUENCY_PARSEVAL')
    reject('TWO_SIDED_AS_POSITIVE_FREQUENCY_MUTATION',2*Iomega-Itime)
    # Independent numerical quadratures including nonzero interference.
    # Finite bounds; Gaussian tails have elementary erfc bounds.
    T, ss, shift, width = 2.0, .2, 1.0, 1.5
    def n0(x): return np.exp(ss*x-x*x/(2*T*T))
    def nh(x): return np.exp(-(x-shift)**2/width**2)/(np.sqrt(np.pi)*width)
    bounds = (-30.0,30.0)
    q0, e0 = quad(lambda x:n0(x)**2,*bounds,epsabs=1e-12,epsrel=1e-12)
    qh, eh = quad(lambda x:nh(x)**2,*bounds,epsabs=1e-12,epsrel=1e-12)
    qc, ec = quad(lambda x:n0(x)*nh(x),*bounds,epsabs=1e-12,epsrel=1e-12)
    q0_exact = T*np.sqrt(np.pi)*np.exp(ss*ss*T*T)
    qh_exact = 1/(np.sqrt(2*np.pi)*width)
    ga = 1/(2*T*T)+1/width**2
    gb = ss+2*shift/width**2
    qc_exact = np.exp(-shift**2/width**2+gb**2/(4*ga))/(width*np.sqrt(ga))
    tail0 = q0_exact*(erfc((30-ss*T*T)/T)+erfc((30+ss*T*T)/T))/2
    zh = np.sqrt(2)*np.array([30-shift,30+shift])/width
    # erfc(z)<=exp(-z^2)/(sqrt(pi)*z). Avoid underflowing a tail to zero.
    log_tailh_upper = np.log(qh_exact/2)+logsumexp(-zh*zh-.5*np.log(np.pi)-np.log(zh))
    assert log_tailh_upper < np.log(1e-300)
    tailh = 1e-300  # conservative positive bound, not a false zero tail
    tailcross = np.sqrt(tail0)*np.sqrt(tailh)
    for computed,exact,error,tail in [(q0,q0_exact,e0,tail0),
            (qh,qh_exact,eh,tailh),(qc,qc_exact,ec,tailcross)]:
        assert abs(computed-exact) <= error+tail+5e-14
    vals = []
    for bval in [0.,1.]:
        val, err = quad(lambda x:(n0(x)+bval*nh(x))**2,
                        *bounds,epsabs=1e-12,epsrel=1e-12)
        assert abs(val-(q0+2*bval*qc+bval*bval*qh)) < 5e-12
        vals.append(dict(free_data_parameter=bval,news_squared_integral=val,
                         quadpack_error_estimate=err))
    assert vals[1]['news_squared_integral'] > vals[0]['news_squared_integral']
    passed('DISTINCT_SMOOTH_NEWS_DATA_INTERFERENCE_QUADRATURE')
    out = dict(python=platform.python_version(),sympy=s.__version__,
        numpy=np.__version__,scipy=scipy.__version__,checks=checks,
        rejected_mutations=rejected,energy_relative_news_tolerance=float(eta),
        manufactured_gaussian_control=dict(time_bounds=list(bounds),
            base_integral=q0,free_integral=qh,interference_inner_product=qc,
            analytic_values=[q0_exact,qh_exact,qc_exact],
            gaussian_tail_bounds=[tail0,tailh,tailcross],
            free_tail_analytic_log_upper=float(log_tailh_upper),
            quadpack_error_estimates=[e0,eh,ec],two_choices=vals,
            scope='Smooth manufactured flat-space news only; no Schwarzschild GL waveform inferred'),
        verdict='Conditional energy bounds and formal second-order non-universality verified; exact scalar-only constrained data construction verified. Exact simultaneous finite-A GL-seeded family and finite-A error bound NOT certified.',
        physical_GL_solves=0,production_simulations=0)
    (HERE/'goal002_audit_energy_results.json').write_text(json.dumps(out,indent=2)+'\n')
    print('SCOPE: exact scalar-only constraints; conditional/formal GL energy results; no finite-A GL certification')


if __name__ == '__main__':
    main()
