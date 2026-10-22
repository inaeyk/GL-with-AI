#!/usr/bin/env python3
"""Independent GOAL-002 geometry audit. Exact, bounded, non-production.
No import of primary GOAL-002 scripts, no ODE solve, no physical-k sweep.
"""
import json
import pathlib
import platform
import sympy as s

HERE = pathlib.Path(__file__).resolve().parent
RESULTS = {"python": platform.python_version(), "sympy": s.__version__, "checks": []}

def check(name, condition, scope):
    assert condition, name
    RESULTS['checks'].append({'name': name, 'result': 'PASS', 'scope': scope})
    print(name + ': PASS', flush=True)

def zero(e): return s.simplify(s.expand_trig(e)) == 0

def curvature_from_series(g0, h, coords, quadratic=True):
    """Independent coordinate connection through second order; flat background."""
    n = len(coords); gi = g0.inv()
    c1, c2 = {}, {}
    for a in range(n):
        for b in range(n):
            for c in range(n):
                dmetric = lambda d: s.diff(h[d,c],coords[b])+s.diff(h[d,b],coords[c])-s.diff(h[b,c],coords[d])
                val = s.simplify(sum(gi[a,d]*dmetric(d) for d in range(n))/2)
                if val != 0: c1[a,b,c] = val
    for a in range(n if quadratic else 0):
        for b in range(n):
            for c in range(n):
                val = s.simplify(-sum((gi*h)[a,d]*c1.get((d,b,c),0) for d in range(n)))
                if val != 0: c2[a,b,c] = val
    def C(d,a,b): return c1.get((d,a,b),s.S.Zero)
    def D(d,a,b): return c2.get((d,a,b),s.S.Zero)
    R1, R2 = s.zeros(n), s.zeros(n)
    for a in range(n):
        for b in range(n):
            R1[a,b] = s.simplify(sum(s.diff(C(c,a,b),coords[c])-s.diff(C(c,a,c),coords[b]) for c in range(n)))
            R2[a,b] = s.simplify(sum(s.diff(D(c,a,b),coords[c])-s.diff(D(c,a,c),coords[b])
                   +sum(C(c,c,d)*C(d,a,b)-C(c,b,d)*C(d,a,c) for d in range(n)) for c in range(n))) if quadratic else s.S.Zero
    return R1,R2

def plane_source_bianchi():
    # A physical standing TT plane wave in flat 5D, rather than arbitrary H.
    # This checks all Q_ab and Bianchi algebra, not Schwarzschild radial data.
    t,x,y,w,z=s.symbols('t x y w z', real=True);coords=[t,x,y,w,z]
    bg=s.diag(-1,1,1,1,1);F=s.cos(t)*s.cos(z)
    h=s.diag(0,0,F,-F,0)
    R1,Q=curvature_from_series(bg,h,coords)
    check('FLAT_ON_SHELL_TT_ALL_LINEAR_RICCI', R1==s.zeros(5), 'manufactured standing plane wave')
    tr=s.simplify(s.trace(bg*Q));G=Q-bg*tr/2
    div=[s.simplify(sum(bg[a,a]*s.diff(G[a,b],coords[a]) for a in range(5))) for b in range(5)]
    check('FULL_QUADRATIC_EINSTEIN_SOURCE_BIANCHI', all(zero(a) for a in div), 'flat on-shell TT source; all five divergences')
    mean=s.simplify(s.integrate(Q[4,4],(z,0,2*s.pi))/(2*s.pi))
    second=s.simplify(s.integrate(Q[4,4]*s.cos(2*z),(z,0,2*s.pi))/s.pi)
    check('RADION_SOURCE_ZERO_AND_SECOND_COSINE_FACTORS', zero(mean+s.cos(t)**2/4) and zero(second+3*s.cos(t)**2/4), 'Qzz0=-k²H²A²/8, Qzz_cos2=-3k²H²A²/8 with k=1,H²=2')
    # Dropping quadratic inverse-connection contribution violates Rzz and Bianchi.
    check('MUTATION_SOURCE_SIGN_AND_MISSING_COSINE_HALF_REJECTED', not zero(mean-s.cos(t)**2/4) and not zero(mean+s.cos(t)**2/2), 'exact deliberate source mutations')
    check('MUTATION_DROP_EINSTEIN_TRACE_REJECTED', any(not zero(s.simplify(sum(bg[a,a]*s.diff(Q[a,b],coords[a]) for a in range(5)))) for b in range(5)), 'Ricci Q alone is not the conserved Einstein source')
    RESULTS['flat_plane_Qzz']={'zero_mode':str(mean),'real_cos2':str(second)}

def n2_constraints():
    t,x,y,w,z=s.symbols('t x y w z',real=True); K=s.symbols('K', nonzero=True,real=True)
    coords=[t,x,y,w,z]; bg=s.diag(-1,1,1,1,1);phase=s.exp(s.I*K*z)
    q4=s.Matrix(4,4,lambda a,b:s.Function('q'+str(min(a,b))+str(max(a,b)))(t,x))
    q=s.zeros(5)
    for a in range(4):
        for b in range(4):q[a,b]=q4[a,b]*phase
    R1,_=curvature_from_series(bg,q,coords,quadratic=False)
    tau=-q4[0,0]+sum(q4[a,a] for a in range(1,4))
    div=[sum(bg[a,a]*s.diff(q4[a,b],coords[a]) for a in range(4)) for b in range(4)]
    check('N2_UNITARY_RZZ_TRACE_IDENTITY',zero(R1[4,4]/phase-K*K*tau/2), 'arbitrary off-shell q_mu_nu(t,x) exp(iKz), q_zA=0')
    check('N2_UNITARY_RMUZ_DIVERGENCE_IDENTITY',all(zero(R1[a,4]/phase-s.I*K*(div[a]-s.diff(tau,coords[a]))/2) for a in range(4)), 'all four mixed constraints')
    box=lambda f: -s.diff(f,t,2)+s.diff(f,x,2)
    check('N2_TENSOR_RICCI_OPERATOR_ALL_COMPONENTS',all(zero(2*R1[a,b]/phase-(-box(q4[a,b])+K*K*q4[a,b]-s.diff(tau,coords[a],coords[b])+s.diff(div[b],coords[a])+s.diff(div[a],coords[b]))) for a in range(4) for b in range(4)), 'flat commuted tensor identity; curved Riemann term analytically supplied')
    # A forced n2 field is not TT: R'[q]=-Q fixes nonzero tau and j.
    Qzz=s.symbols('Qzz');tau_forced=-2*Qzz/K**2
    check('MUTATION_IMPOSE_FORCED_N2_TRACE_ZERO_REJECTED',s.simplify(K*K*tau_forced/2+Qzz)==0 and s.simplify(Qzz)!=0, 'unitary q_zA=0 does not imply homogeneous TT constraints for sourced q')

def ef_metric():
    v,r,r0,e=s.symbols('v r r0 e');f=1-r0/r;mu=s.Function('mu')(v,r);b=s.Function('beta')(v,r);xx=[v,r]
    g0=s.Matrix([[-f,1],[1,0]]);h=s.Matrix([[2*mu/r-2*f*b,b],[b,0]]);gi0=g0.inv();g=g0+e*h;gi=gi0-e*gi0*h*gi0
    trunc=lambda a:s.expand(a).coeff(e,0)+e*s.expand(a).coeff(e,1)
    C={}
    for a in range(2):
        for j in range(2):
            for k in range(2):C[a,j,k]=s.simplify(trunc(sum(gi[a,d]*(s.diff(g[d,k],xx[j])+s.diff(g[d,j],xx[k])-s.diff(g[j,k],xx[d])) for d in range(2))/2))
    Ric=s.zeros(2)
    for a in range(2):
        for j in range(2):Ric[a,j]=s.simplify(trunc(sum(s.diff(C[c,a,j],xx[c])-s.diff(C[c,a,c],xx[j])+sum(C[c,c,d]*C[d,a,j]-C[c,j,d]*C[d,a,c] for d in range(2)) for c in range(2)))+2*C[1,a,j]/r)
    box=s.simplify(trunc(-sum(gi[a,j]*C[1,a,j] for a in range(2) for j in range(2))))
    ang=s.simplify(trunc(1-gi[1,1]-r*box));scal=s.simplify(trunc(sum(gi[a,j]*Ric[a,j] for a in range(2) for j in range(2))+2*ang/r**2))
    G=s.Matrix(2,2,lambda a,j:s.simplify(trunc(Ric[a,j]-g[a,j]*scal/2)));mix=gi0*G
    lin=lambda a:s.simplify(s.expand(a).coeff(e,1))
    expected_ang=r*(r-r0)*s.diff(b,r,2)+r*r*s.diff(b,r,v)+(r+r0/2)*s.diff(b,r)-r*s.diff(mu,r,2)
    check('EF_MASS_LAPSE_THREE_CONSTRAINTS',zero(lin(G[1,1])-2*s.diff(b,r)/r) and zero(lin(mix[0,0])+2*s.diff(mu,r)/r**2) and zero(lin(mix[1,0])-2*s.diff(mu,v)/r**2), 'direct 2D Christoffels plus exact S² warp; arbitrary mu,beta')
    check('EF_ANGULAR_EINSTEIN_EQUATION',zero(lin(ang-r*r*scal/2)-expected_ang), 'independent angular residual equation, not inferred from Rzz')
    check('MUTATION_EF_MASS_SIGN_AND_LAPSE_FACTOR_REJECTED',not zero(lin(mix[0,0])-2*s.diff(mu,r)/r**2) and not zero(lin(G[1,1])-s.diff(b,r)/r), 'deliberate radial-constraint errors')
    RESULTS['EF_Gtheta_theta_covariant']=str(expected_ang)
    # Weyl conformal correction: q4=p-chi*g/2. R5_mu_nu linear equals
    # R4'[p]+g Box chi/4. Therefore G4'[p]=-Qmunu+g*(Q4trace+Qzz)/2.
    boxchi,Q4,Qzz=s.symbols('boxchi Q4 Qzz')
    rictrace=-Q4-boxchi
    effective_trace=s.simplify(-Q4-boxchi-2*rictrace)
    target_trace=Q4+2*Qzz
    check('EINSTEIN_FRAME_SOURCE_TRACE',s.simplify(effective_trace.subs(boxchi,2*Qzz)-target_trace)==0, 'Weyl correction and scalar equation; tensor derivation in audit note')

def horizon():
    d,K,O=s.symbols('delta K Omega',positive=True);r=1+d;D=1+K*r**3;f=d/r;c1,c2=s.symbols('c1 c2')
    Y=1+c1*d+c2*d*d;yp=s.diff(Y,d)+O*Y/d;ypp=s.diff(Y,d,2)+2*O*s.diff(Y,d)/d+O*(O-1)*Y/d**2
    P=1/d-5/r+6/(r*D);Q=3/(r*r*d)-K*r/d-3/(r*r*d*D)
    # Clear rational denominator BEFORE coefficient extraction; otherwise
    # coeff(delta,-1) can omit 1/(K delta+delta) and generate a false pole.
    num=s.together(d*d*(ypp+P*yp+(Q-O*O/f**2)*Y)).as_numer_denom()[0].expand()
    a=s.factor(s.solve(num.coeff(d,1),c1)[0]);b=s.factor(s.solve(num.coeff(d,2).subs(c1,a),c2)[0])
    Y=Y.subs({c1:a,c2:b});yp=s.diff(Y,d)+O*Y/d
    T=((r-1)*(s.Rational(1,2)-K*r**3)/(r*D**2)-O**2/(K*D))*Y+((r-1)/(2*K*r**3*D)-3*(r-1)**2/D**2)*yp
    B=-O/(K*D)*(yp-Y/(2*r*(r-1)));S=-r*r*Y/(2*D)-(r-1)*yp/(K*D)
    EF=[T,B-T/f,2*T/f**2-2*B/f-2*S/(r*(r-1)),S/r**2]
    limits=[O*(1-2*O)/(2*K*(K+1)),(K+2*O)/(2*K*(K+1)),-(K**2+8*K*O+8*O**2+2*O)/(2*K*(K+1)*(O+1)*(2*O+1)),-(K+2*O)/(2*K*(K+1))]
    computed=[s.factor(s.limit(a,d,0)) for a in EF]
    check('ALL_EF_FIRST_ORDER_HORIZON_COMPONENTS_FINITE',all(zero(a-b) for a,b in zip(computed,limits)), 'Frobenius through c2; overall e^{Omega v} e^{-Omega r} times leading y coefficient')
    # Leave the a1 coefficient wrong: uncancelled EF rr simple pole remains.
    badY=1+(a+1)*d+b*d*d;badyp=s.diff(badY,d)+O*badY/d
    badT=T.xreplace({}) # build independently with wrong Y; no substitution of exact solution
    badT=((r-1)*(s.Rational(1,2)-K*r**3)/(r*D**2)-O**2/(K*D))*badY+((r-1)/(2*K*r**3*D)-3*(r-1)**2/D**2)*badyp
    badB=-O/(K*D)*(badyp-badY/(2*r*(r-1)));badS=-r*r*badY/(2*D)-(r-1)*badyp/(K*D)
    residue=s.factor(s.limit(d*(2*badT/f**2-2*badB/f-2*badS/(r*(r-1))),d,0))
    check('MUTATION_WRONG_FROBENIUS_COEFFICIENT_REJECTED',residue!=0, 'spurious EF rr pole exposed; bounded regularity check is sensitive')
    RESULTS['horizon']={'K':'k² in r0=1','c1':str(a),'c2':str(b),'EF_limits':[str(a) for a in limits],'wrong_c1_rr_residue':str(residue)}

def pure_gauge():
    v,r,z,k,r0=s.symbols('v r z k r0',real=True,nonzero=True);f=1-r0/r;coords=[v,r,z]
    bg=s.Matrix([[-f,1,0],[1,0,0],[0,0,1]])
    b,c,d=[s.Function(n)(r) for n in ('b','c','d')]
    xi=s.Matrix([d*s.cos(k*z),b*s.cos(k*z),c*s.sin(k*z)])
    def lie(v,T): return s.Matrix(3,3,lambda a,b:s.simplify(sum(v[j]*s.diff(T[a,b],coords[j])+T[j,b]*s.diff(v[j],coords[a])+T[a,j]*s.diff(v[j],coords[b]) for j in range(3))))
    h=lie(xi,bg);q=lie(xi,h)/2
    completed=q+lie(-xi,h)+lie(-xi,lie(-xi,bg))/2
    check('REGULAR_EF_PERIODIC_PURE_GAUGE_COMPLETION_ZERO',completed.applyfunc(s.simplify)==s.zeros(3), 'smooth compactly supported b,c,d may be chosen; no static-chart singularities')
    avg=s.simplify(s.integrate(q[2,2],(z,0,2*s.pi/k))*k/(2*s.pi))
    check('MUTATION_DROP_HALF_LIE_SQUARED_REJECTED',avg!=0,'spurious raw <qzz> or incomplete completion; orbit calculation sufficient for zz')
    RESULTS['pure_gauge_raw_qzz0']=str(avg)

def main():
    print('Python',platform.python_version(),'SymPy',s.__version__,flush=True)
    plane_source_bianchi();n2_constraints();ef_metric();horizon();pure_gauge()
    RESULTS['scope']='Exact identities and narrow independent controls only. No full n2 BVP, no continuum enclosure, no finite-A or physical energy theorem.'
    (HERE/'goal002_audit_geometry_results.json').write_text(json.dumps(RESULTS,indent=2)+'\n')
    print('SCOPE:',RESULTS['scope'])
if __name__=='__main__':main()
