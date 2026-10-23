#!/usr/bin/env python3
"""GOAL004 Reviewer I: independent non-production exact geometry checks.

The default exact-geometry mode imports no primary implementation. Optional
--inspect-primary performs an explicitly shared-input physical point audit,
with an independently implemented five-dimensional Rzz/Fourier calculation.
These are conditional-estimate/interface checks, not an enclosed GL mode.
"""
from pathlib import Path
import hashlib
import json
import platform
import sys
import sympy as sp

checks = {}
records = {}


def exact(name, expression):
    residual = sp.factor(sp.together(expression))
    if residual != 0:
        raise AssertionError((name, residual))
    checks[name] = True
    print(name + ': PASS', flush=True)


def rejects(name, expression):
    residual = sp.factor(sp.together(expression))
    if residual == 0:
        raise AssertionError((name, 'mutation was not rejected'))
    checks[name] = True
    records[name] = str(residual)
    print(name + ': REJECTED (expected)', flush=True)


r, z = sp.symbols('r z', positive=True, real=True)
aa = sp.Function('a')(z)
Q = sp.Function('Q')(z)
J = sp.Function('J')(z)
az = sp.diff(aa, z)
Qr = aa * sp.diff(J, z) + 2 * az * J
Jr = aa * sp.diff(Q, z) / 2 - az * Q / 2
flux0 = aa * Q * J
exact('EXTERIOR_E0_LOCAL_DIVERGENCE',
      Q * Qr + 2 * J * Jr - sp.diff(flux0, z))
expected1 = (2 * az * sp.diff(Q, z) * sp.diff(J, z)
             + sp.diff(aa, z, 2) * (2 * sp.diff(Q, z) * J
                                    - Q * sp.diff(J, z)))
energy1 = (sp.diff(Q, z) * sp.diff(Qr, z)
           + 2 * sp.diff(J, z) * sp.diff(Jr, z))
exact('EXTERIOR_E1_LOCAL_DIVERGENCE',
      energy1 - sp.diff(aa * sp.diff(Q, z) * sp.diff(J, z), z)
      - expected1)
rejects('WRONG_MOMENTUM_CONNECTION_SIGN',
        Q * Qr + 2 * J * (aa * sp.diff(Q, z) / 2 + az * Q / 2)
        - sp.diff(flux0, z))
rejects('DROPPED_H1_COMMUTATORS', expected1)

# E0 does not control pointwise amplitudes. Orthogonality makes the exact
# trigonometric family explicit without sampling or a numerical PDE.
N = sp.symbols('N', integer=True, positive=True)
records['HIGH_FREQUENCY_FAMILY'] = {
    'Q_N': 'sqrt(2/N)*sum(cos(j*z), j=1..N), J_N=0, L=2*pi',
    'E0': 'pi', 'Q_N_at_zero': 'sqrt(2*N)',
    'E1': 'pi*(N+1)*(2*N+1)/6',
    'scope': 'Analytic control/counterexample; not GL initial data.'}
for nn in (1, 2, 4):
    qn = sp.sqrt(sp.Rational(2, nn)) * sum(sp.cos(j*z) for j in range(1, nn+1))
    exact('E0_COUNTEREXAMPLE_N' + str(nn),
          sp.integrate(qn**2/2, (z, 0, 2*sp.pi)) - sp.pi)
    exact('E1_COUNTEREXAMPLE_N' + str(nn),
          sp.integrate(sp.diff(qn,z)**2/2, (z, 0, 2*sp.pi))
          - sp.pi*(nn+1)*(2*nn+1)/6)
    exact('POINTWISE_COUNTEREXAMPLE_N' + str(nn), qn.subs(z,0)**2 - 2*nn)

# Independent scalar-curvature calculation on the static K=0 slice.
# This is a bifurcation-surface diagnostic, not the future-EF data family.
eps = sp.symbols('eps')
f = 1-1/r
u, w, v = [sp.Function(n)(r) for n in ('u', 'w', 'v')]
A = sp.sqrt((1+eps*u)/f)
B = sp.sqrt(1+eps*w)
R = r*sp.sqrt(1+eps*v)
baseR = (-2*sp.diff(B,r,2)/(A**2*B)
         +2*sp.diff(A,r)*sp.diff(B,r)/(A**3*B))
lapR = (sp.diff(R,r,2)+(sp.diff(B,r)/B-sp.diff(A,r)/A)*sp.diff(R,r))/A**2
scalarR = baseR+2/R**2-4*lapR/R-2*sp.diff(R,r)**2/(A**2*R**2)
exact('STATIC_SLICE_BACKGROUND_SCALAR_CURVATURE', scalarR.subs(eps,0))
DR = sp.diff(scalarR,eps).subs(eps,0)
boundary = (r**2*f*(-sp.diff(w,r)-2*sp.diff(v,r)+2*(u-v)/r)
            +r**2*sp.diff(f,r)*(w+2*v)/2)
exact('STATIC_KID_RADIAL_BOUNDARY_PAIRING', r**2*DR-sp.diff(boundary,r))
dm = sp.symbols('delta_r0')
exact('MASS_VARIATION_KID_NORMALIZATION',
      boundary.subs({u:dm/(r*f),v:0,w:0}).doit()-2*dm)
c = sp.symbols('circle_tail')
exact('CIRCLE_TAIL_ADM_MASS_CONTRIBUTION',
      sp.limit(boundary.subs({u:0,v:0,w:c/r}).doit(),r,sp.oo)-c)
wrong_boundary = r**2*f*(-sp.diff(w,r)-2*sp.diff(v,r)+2*(u-v)/r)
rejects('OMITTED_KID_LAPSE_GRADIENT_TERM',r**2*DR-sp.diff(wrong_boundary,r))

# Exact background charge-transversality refinement of the improved rule.
# This does not assert a global z-dependent GL family.
Ri = sp.symbols('R_i',positive=True)
lam = sp.symbols('lambda',real=True)
q0 = 2/(r*sp.sqrt(r*(r+1)))
w0 = q0/2
kappa0 = w0+r*sp.diff(w0,r)
d0 = kappa0+q0/2
Z0 = -q0**2/2+2*d0*q0
u0 = r/(r+1)
cc = Ri**2/(Ri+1)*(sp.exp(-2*lam)-1)
ulambda = u0+cc/r
exact('BACKGROUND_IMPROVED_MOMENTUM',sp.diff(q0,r)-(-2*q0+2*d0)/r)
exact('BACKGROUND_RADIAL_HAMILTONIAN',sp.diff(u0,r)-(1-u0)/r-r*Z0/2)
exact('EXACT_SPHERICAL_LAPSE_PARAMETER_FAMILY',
      sp.diff(ulambda,r)-(1-ulambda)/r-r*Z0/2)
exact('INNER_LAPSE_PARAMETER_NORMALIZATION',
      ulambda.subs(r,Ri)-u0.subs(r,Ri)*sp.exp(-2*lam))
exact('SPHERICAL_ADM_MASS_COEFFICIENT',
      sp.limit(r*(1/ulambda-1),r,sp.oo)-(1-cc))
exact('BACKGROUND_MASS_TUNING_DERIVATIVE',
      sp.diff(1-cc,lam).subs(lam,0)-2*Ri**2/(Ri+1))
rejects('FALSE_ZERO_BACKGROUND_MASS_DERIVATIVE',
        sp.diff(1-cc,lam).subs(lam,0))
records['background_mass_transversality'] = {
    'u_lambda':'r/(r+1)+C(lambda)/r',
    'C(lambda)':'R_i^2/(R_i+1)*(exp(-2*lambda)-1)',
    'M(lambda)/M0':'1-C(lambda)',
    'd_lambda_M_at_zero_over_M0':'2*R_i^2/(R_i+1)>0',
    'scope':'Exact spherical constraints only. Global z-dependent GL-family '
            'continuation, mass-map differentiability and neighborhood bound remain open.'}

# Subsequent audit of the primary note's linear parabolic proposal. These
# are differentiated independently from the original radial Hamiltonian.
a0 = sp.sqrt((r+1)/r)
coefficient_alpha = (1-3*a0**2)/(2*r)-3*r*a0**2*Z0/4
coefficient_W = -r*a0**3*(-q0+2*d0)/(4*r**2)
exact('LINEAR_LAPSE_ALPHA_COEFFICIENT',
      coefficient_alpha+(2*r+5)/(2*r*(r+1)))
exact('LINEAR_LAPSE_W_COEFFICIENT',coefficient_W-(2*r+1)/(4*r**4))
rho,kn = sp.symbols('rho k_n',positive=True)
G = (sp.exp(-kn**2*((r**2-rho**2)/4+(r-rho)/2))
     *(rho/r)**sp.Rational(5,2)*((r+1)/(rho+1))**sp.Rational(3,2))
exact('LINEAR_PARABOLIC_KERNEL_EQUATION',
      sp.diff(G,r)/G+kn**2*(r+1)/2+(2*r+5)/(2*r*(r+1)))
wrong_G = G/((r+1)/(rho+1))**sp.Rational(3,2)
rejects('OMITTED_PARABOLIC_KERNEL_FACTOR',
        sp.diff(wrong_G,r)/wrong_G+kn**2*(r+1)/2+(2*r+5)/(2*r*(r+1)))

# The ground-state maximum principle is derived directly from H=-d_x^2+V.
a, ss = sp.symbols('a s', positive=True)
D = 1+a*r**3
ww = sp.exp(-r/2)*r/D
V = (a*f+(6*r-11)*(r-1)/r**4
     +18*(r-1)**2/(r**4*D**2)-6*(4*r-5)*(r-1)/(r**4*D))
dx = lambda g: f*sp.diff(g,r)
H = lambda g: -dx(dx(g))+V*g
P = ((4*a*a-a)*r**5-7*a*r**4+10*a*r**3
     -(8*a+1)*r**2+5*r-2)
h = ss**2+f*P/(4*r**2*D)
exact('SUPER_SOLUTION_IDENTITY', H(ww)/ww+ss**2-h)
yy = sp.Function('y')(r)
S = -dx(sp.log(ww))
exact('WEIGHTED_MAXIMUM_PRINCIPLE_TRANSFORM',
      (H(ww*yy)+ss**2*ww*yy)/ww-(-dx(dx(yy))+2*S*dx(yy)+h*yy))
b, dd = sp.symbols('b d', nonnegative=True)
polynomial = sp.Poly(sp.expand(P.subs({a:sp.Rational(3,2)+b,r:1+dd})),b,dd)
assert all(coef>0 for coef in polynomial.coeffs())
checks['POSITIVE_SUPER_SOLUTION_FOR_A_GE_3_OVER_2'] = True
print('POSITIVE_SUPER_SOLUTION_FOR_A_GE_3_OVER_2: PASS',flush=True)
exact('NEGATIVE_HORIZON_POTENTIAL_CONTROL',
      sp.limit(V.subs(a,sp.Rational(3,2))/(r-1),r,1)+sp.Rational(11,10))
rejects('WRONG_GROUND_STATE_DRIFT_SIGN',
        (H(ww*yy)+ss**2*ww*yy)/ww-(-dx(dx(yy))-2*S*dx(yy)+h*yy))

# Nonzero complete source need not have a nonzero outgoing projection.
# Compact C2 data F=(1-x^2)^3, |x|<=1, make both boundary terms vanish.
x = sp.symbols('x', real=True)
s0 = sp.Rational(2,3)
compact = (1-x*x)**3
source = -sp.diff(compact,x,2)+s0**2*compact
exact('COMPACT_SOURCE_ZERO_GROWING_OVERLAP',
      sp.integrate(sp.exp(s0*x)*source,(x,-1,1)))
assert source != 0
checks['COMPACT_SOURCE_IS_NONZERO'] = True
print('COMPACT_SOURCE_IS_NONZERO: PASS',flush=True)
rejects('WRONG_SOURCE_SIGN_ZERO_OVERLAP',
        sp.integrate(sp.exp(s0*x)*(sp.diff(compact,x,2)+s0**2*compact),(x,-1,1)))
records['scope'] = ('Independent exact conditional geometry and adversarial controls. '
    'No physical GL ODE solve; no physical L10 eigenvalue/source enclosure; '
    'no global KK data existence; no finite-A or radiation certification.')
records['weighted_bound'] = ('If F=L_s u, u/w -> 0 at both ends, '
    'a>=3/2, s>0, C_w=sup |F|/[w*(s^2+H w/w)] < infinity, '
    'then |u|<=C_w*w by the maximum principle.')
script = Path(__file__)
out = {'python':platform.python_version(),'sympy':sp.__version__,
       'author':'GOAL004 independent AI Reviewer I / audit_geometry',
       'input_head':'2f4648c379c474ed124e3ef10b62efcd4a783c66',
       'script_sha256':hashlib.sha256(script.read_bytes()).hexdigest(),
       'checks':checks,'records':records}
script.with_name('goal004_audit_geometry_results.json').write_text(json.dumps(out,indent=2)+'\n')
print('TOTAL_CHECKS',len(checks),flush=True)

if '--inspect-primary' in sys.argv:
    import importlib.util
    import mpmath as mp
    primary = script.with_name('goal004_source_geometry.py')
    spec = importlib.util.spec_from_file_location('goal004_primary_point_input',primary)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    rr = mp.mpf(2)
    row, diagnostics = module.source_at(float(rr),checks=True)
    _,ff,tt,bb,cc,stheta,_ = module.regular_jets(rr)
    h5 = mp.matrix(5)
    h5[0,0]=tt.c[0];h5[0,1]=h5[1,0]=bb.c[0];h5[1,1]=cc.c[0]
    h5[2,2]=h5[3,3]=stheta.c[0]
    inverse = mp.matrix(5)
    inverse[0,1]=inverse[1,0]=1;inverse[1,1]=ff.c[0]
    inverse[2,2]=inverse[3,3]=1/rr**2;inverse[4,4]=1
    trace = lambda m: sum(m[i,i] for i in range(5))
    # Direct exact block-metric identity for g_zz=1,g_zmu=0:
    # Rzz=-tr(g^-1 g_zz)/2+tr(g^-1 g_z g^-1 g_z)/4.
    q0 = mp.mpc(0);q2 = mp.mpc(0)
    for jphase in range(8):
        phase = mp.pi*jphase/4
        h = h5*mp.cos(phase)
        hz = -module.km*h5*mp.sin(phase)
        hzz = -module.km**2*h5*mp.cos(phase)
        qzz = trace(inverse*h*inverse*hzz)/2+trace(inverse*hz*inverse*hz)/4
        q0 += qzz/8
        q2 += qzz*mp.exp(-2j*phase)/8
    lorentz = trace((inverse*h5)**2)
    reference2 = -2*module.km**2*mp.mpf(str(row[2]))
    tests = {
        'PHYSICAL_POSITIVE_K_LORENTZ_CONTRACTION':abs(lorentz-row[11])<mp.mpf('1e-12')*max(1,abs(lorentz)),
        'PHYSICAL_POSITIVE_K_ZERO_FOURIER_WEIGHT':abs(q0+module.km**2*lorentz/8)<mp.mpf('1e-50'),
        'PHYSICAL_POSITIVE_K_COMPLETE_PLUS2_SOURCE':abs(q2-reference2)<mp.mpf('1e-12')*max(1,abs(q2)),
        'WRONG_PLUS2_FACTOR_REJECTED':abs(2*q2-reference2)>mp.mpf('1e-6'),
        'WRONG_EUCLIDEAN_CONTRACTION_REJECTED':abs(q2+3*module.km**2*trace(h5*h5)/16)>mp.mpf('1e-6'),
    }
    assert all(tests.values()),tests
    for name in tests:print(name+': PASS',flush=True)
    point_output = {
        'python':platform.python_version(),'mpmath':mp.__version__,
        'primary_script_sha256':hashlib.sha256(primary.read_bytes()).hexdigest(),
        'r':2.0,'k':float(module.km),'Omega':float(module.wm),
        'tests':tests,'primary_local_diagnostics':diagnostics,
        'independent_Rzz_zero':float(mp.re(q0)),
        'independent_Rzz_plus2':float(mp.re(q2)),
        'primary_complete_Rzz_plus2':float(mp.re(reference2)),
        'Lorentz_Hsq':float(mp.re(lorentz)),
        'scope':'Shared empirical GL radial input from primary regular_jets; '
                'independent block-metric Rzz/Fourier implementation. '
                'A physical-positive-k local algebra audit, not an independent '
                'eigenfunction, radial source derivation or global enclosure.'}
    script.with_name('goal004_audit_geometry_physical_point.json').write_text(json.dumps(point_output,indent=2)+'\n')
    print(json.dumps(point_output,indent=2),flush=True)
