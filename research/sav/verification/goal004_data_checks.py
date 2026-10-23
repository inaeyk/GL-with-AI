#!/usr/bin/env python3
"""GOAL004 primary exact constraints/kernel and manufactured EFT checks."""
from pathlib import Path
import hashlib
import json
import platform
import sympy as s

HERE=Path(__file__).resolve().parent
checks=[]
def check(label,expr):
    answer=s.simplify(expr)
    assert answer==0,(label,answer)
    checks.append(label)
    print(label+': PASS')
r,R,lam,k,n=s.symbols('r R lambda k n',positive=True)
a0=s.sqrt((r+1)/r);q0=2/(r*s.sqrt(r*(r+1)))
d0=1/(2*r**s.Rational(3,2)*(r+1)**s.Rational(3,2))
Z0=s.simplify(-q0*q0/2+2*d0*q0)
check('background_Z',Z0+2/(r*r*(r+1)**2))
C=R*R/(R+1)*(s.exp(-2*lam)-1)
u=r/(r+1)+C/r
check('exact_global_spherical_constraint',s.diff(u,r)-(1-u)/r-r*Z0/2)
check('inner_exponential_lapse',u.subs(r,R)-R/(R+1)*s.exp(-2*lam))
check('background_mass_derivative_9_over_14',
      s.diff(1-C,lam).subs({lam:0,R:s.Rational(3,4)})-s.Rational(9,14))
A=(1-3*a0*a0)/(2*r)-3*r*a0*a0*Z0/4
B=-r*a0**3*(-q0+2*d0)/(4*r*r)
check('linear_lapse_coefficient',A+(2*r+5)/(2*r*(r+1)))
check('linear_W_coefficient',B-(2*r+1)/(4*r**4))
G=s.exp(-k*k*((r*r-R*R)/4+(r-R)/2))*(R/r)**s.Rational(5,2)*((r+1)/(R+1))**s.Rational(3,2)
check('exact_parabolic_kernel',s.diff(G,r)/G-A+k*k*(r+1)/2)
wrong=s.simplify(s.diff(G,r)/G-A-k*k*(r+1)/2)
assert wrong!=0
checks.append('wrong_angular_diffusion_sign_REJECTED')

# Independent finite-dimensional scalar KG current normalization and
# closed-form two manufactured pulse transport ratios, NOT GL predictions.
mu=s.symbols('mu',nonnegative=True)
z=s.symbols('z',positive=True)
Rmem=s.exp(-mu*mu)-s.sqrt(s.pi)*mu*s.erfc(mu)
check('memory_transport_derivative',s.diff(Rmem,mu)+s.sqrt(s.pi)*s.erfc(mu))
check('memory_massless_limit',Rmem.subs(mu,0)-1)
check('memory_small_mass_linear',s.diff(Rmem,mu).subs(mu,0)+s.sqrt(s.pi))
check('zero_memory_Gaussian_weight',
      s.integrate(z*z*s.exp(-z*z-mu*mu),(z,0,s.oo))-
      s.sqrt(s.pi)*s.exp(-mu*mu)/4)
assert s.simplify(s.diff(Rmem,mu).subs(mu,0))!=0
checks.append('universal_quadratic_small_mass_error_REJECTED')
# Equally matched kernels are not unique, even at BOTH calibration points.
v=s.symbols('v',positive=True)
x=s.symbols('x',positive=True)
# delta h at delays 0,tau,2tau, Laplace variable x=exp(-s0*tau).
c0=x;c1=-(1+x);c2=1
check('kernel_ambiguity_static',c0+c1+c2)
check('kernel_ambiguity_GL_point',c0+c1*x+c2*x*x)
assert s.expand(c0+c1*x**2+c2*x**4)!=0
checks.append('two_transfer_points_do_not_fix_kernel_REJECTED')
report=dict(python=platform.python_version(),sympy=s.__version__,checks=checks,
    script_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
    scope='Exact identities/manufactured controls, not global GL closure',
    gaussian_transport=[dict(mu=float(m),memory_ratio=float(Rmem.subs(mu,m)),
        zero_memory_ratio=float(s.exp(-m*m))) for m in (s.Rational(1,10),s.Rational(1,2),s.Integer(1))])
(HERE/'goal004_data_checks_results.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report,indent=2))
