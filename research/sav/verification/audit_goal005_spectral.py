"""Independent GOAL005 spectral endpoint audit. No primary solver imports.

Exact SymPy residual identities and outward-rounded Arb rational-domain
bounds. This does not integrate a physical eigenmode or prove a root.
"""
from pathlib import Path
import hashlib
import json
import platform
import math
import sympy as S
import flint
from flint import arb, ctx

ctx.prec = 160
HERE = Path(__file__).resolve().parent
checks = []
def check(name, ok, evidence):
    checks.append(dict(name=name, passed=bool(ok), evidence=str(evidence)))
    if not ok:
        raise AssertionError(name + ': ' + str(evidence))

def ev(expr, env):
    if expr in env: return env[expr]
    if expr.is_Integer: return arb(int(expr))
    if expr.is_Rational: return arb(int(expr.p))/int(expr.q)
    if expr.is_Add:
        out=arb(0)
        for e in expr.args: out += ev(e,env)
        return out
    if expr.is_Mul:
        out=arb(1)
        for e in expr.args: out *= ev(e,env)
        return out
    if expr.is_Pow and expr.exp.is_Integer:
        return ev(expr.base,env)**int(expr.exp)
    raise TypeError(expr)

def bound_rational(expr, var, domain, env):
    num,den = S.fraction(S.cancel(expr))
    def horner(poly):
        z=arb(0)
        for c in S.Poly(poly,var).all_coeffs(): z=z*domain+ev(c,env)
        return z
    n,d=horner(num),horner(den)
    check('nonzero rational denominator '+str(var),not d.contains(0),d)
    return n/d

r,d,t,a,o,q = S.symbols('r d t a o q',positive=True)
f=(r-1)/r
D=1+a*r**3
V=a*f+(6*r-11)*(r-1)/r**4+18*(r-1)**2/(r**4*D**2)-6*(4*r-5)*(r-1)/(r**4*D)
# Independent Wronskian route: W_x = u*u_{xOmega,x} - u_xx*u_Omega.
U,ZO,V0,OP=S.symbols('U ZO V0 OP')
wronskian_derivative=S.expand(U*((V0+OP**2)*ZO+2*OP*U)-(V0+OP**2)*U*ZO)
check('linear parameter Wronskian identity',S.expand(wronskian_derivative-2*OP*U**2)==0,wronskian_derivative)

# p0H = Omega+c delta, derive c by cancelling the linear residual.
cc=S.symbols('cc')
vh=S.cancel(V.subs(r,1+d))
ph=o+cc*d
rh=S.cancel(d/(1+d)*S.diff(ph,d)-vh-o**2+ph**2)
c=S.solve(S.Eq(S.limit(rh/d,d,0),0),cc)[0]
check('horizon first coefficient',S.simplify(c-(a-5+6/(1+a))/(1+2*o))==0,c)
rh=S.cancel(rh.subs(cc,c)/d**2)

# Infinity: derive p=-q+A1/r+A2/r2+A3/r3, q2=a+Omega2.
A1,A2,A3=S.symbols('A1 A2 A3')
pi=-q+A1*t+A2*t**2+A3*t**3
vt=S.cancel(V.subs(r,1/t))
ri=S.cancel(-(1-t)*t**2*S.diff(pi,t)-vt-(q**2-a)+pi**2)
sol={}
for power,coef in [(1,A1),(2,A2),(3,A3)]:
    term=S.expand(S.series(ri.subs(sol),t,0,power+1).removeO()).coeff(t,power)
    sol[coef]=S.factor(S.solve(term,coef)[0])
pi=S.factor(pi.subs(sol))
ri=S.cancel(ri.subs(sol)/t**4)
check('infinity cubic coefficients cancel powers 0 through 3',S.limit(t**4*ri/t**3,t,0)==0,sol)
beta=q-a/(2*q)
check('Coulomb r exponent from p=f(log u)_r',S.expand((1-t)*(-q-beta*t)+q-sol[A1]*t).coeff(t,1)==0,beta)
# Reconstruction normalization is a nontrivial functional, not u(2)=1.
pp=S.symbols('pp')
w=D/r
normalization=-w/(4*D)-(r-1)*(S.diff(w,r)+w*pp/f)/(2*a*D*r**2)
norm2=S.factor(normalization.subs(r,2))
check('physical sphere normalization functional',S.simplify(norm2-(-S.Rational(1,8)-(16*a-1)/(32*a*(1+8*a))-pp/(8*a)))==0,norm2)
P=1/(r-1)-5/r+6/(r*D)
Q=3/(r**2*(r-1))-a*r/(r-1)-3/(r**2*(r-1)*D)
bb=2*o+1/r-5*f/r+6*f/(r*D)
ce=3/r**3-a-3/(r**3*D)+o*(-4/r+6/(r*D))
check('regular EF scalar first derivative coefficient',S.cancel(bb-(f*P+2*o))==0,bb)
check('regular EF scalar zeroth derivative coefficient',S.cancel(ce-(f*Q+o*P-o*S.diff(f,r)/f))==0,ce)
check('regular EF indicial damping positive',S.simplify(bb.subs(r,1)-(1+2*o))==0,bb.subs(r,1))
YY=S.Function('YY')(r)
ZZ=-o/(a*D)*(f*S.diff(YY,r)+(o-1/(2*r*r))*YY)
BV=-S.diff(ZZ,r)/o-2*ZZ/(o*r)
TV=ZZ-f*BV
SV=r*ZZ/o-YY/(2*a*r)
NN=2*S.diff(ZZ,r)/o+2*ZZ/(o*r)+YY/(a*r**3)
horizon_N=S.simplify(NN.subs(r,1).subs(S.Subs(S.Derivative(YY,r),r,1),-ce.subs(r,1)*YY.subs(r,1)/bb.subs(r,1)))
check('exact regular Einstein reconstruction N horizon cancellation',horizon_N==0,horizon_N)
ap=(r-1)*(S.Rational(1,2)-a*r**3)/(r*D**2)-o**2/(a*D)
bp=(r-1)/(2*a*r**3*D)-3*(r-1)**2/D**2
yyp=S.diff(YY,r)+o*YY/f
ts=ap*YY+bp*yyp
bs=-o/(a*D)*(yyp-YY/(2*r*(r-1)))
ss=-r*r*YY/(2*D)-(r-1)*yyp/(a*D)
cs=ts/f**2-2*ss/(r*(r-1))
for name,expr in [('vv',TV-ts),('vr',BV-(bs-ts/f)),('rr',NN/f-(cs-2*bs/f+ts/f**2)),('sphere',SV-ss)]:
    reduced=S.cancel(expr.subs(S.diff(YY,r,2),-(bb*S.diff(YY,r)+ce*YY)/f))
    check('independent static-to-EF reconstruction '+name,reduced==0,reduced)
wrong_beta_p=(1-t)*(-q+beta*t)
wrong_coulomb=S.expand(-(1-t)*t**2*S.diff(wrong_beta_p,t)-vt-(q**2-a)+wrong_beta_p**2)
wrong_coulomb_lead=S.simplify(S.limit(wrong_coulomb/t,t,0))
check('negative: wrong Coulomb sign leaves 1/r residual',wrong_coulomb_lead!=0,wrong_coulomb_lead)

k=arb.pi()/5
aa=k*k
oo=arb('0.063285','0.000015')
qq=(aa+oo*oo).sqrt()
deltaH=arb(-28).exp()
R=arb(40)
dc=arb(0).union(deltaH)
tc=arb(0).union(1/R)
env={a:aa,o:oo,q:qq}
cv=ev(c,env)
CH=bound_rational(rh,d,dc,env).abs_upper()
# p0 >= hmin; for delta'≤deltaH, delta(t)/delta(x)≤exp(-(x-t)/(1+deltaH)).
hmin=(oo-cv.abs_upper()*deltaH).lower()
lamH=2*hmin+2/(1+deltaH)
MH=arb(2)*CH/lamH
check('horizon invariant weighted error ball',CH+MH*MH*deltaH**2 < lamH*MH,(CH+MH*MH*deltaH**2)/(lamH*MH))
check('horizon contraction',2*MH*deltaH**2/lamH < 1,2*MH*deltaH**2/lamH)
errH=MH*deltaH**2
ph0=oo+cv*deltaH
phbox=ph0+arb(0,errH.upper())

CI=bound_rational(ri,t,tc,env).abs_upper()
base_range=bound_rational(pi,t,tc,env)
imin=(-base_range).lower()
check('infinity reference logarithmic derivative negative',imin>0,imin)
MI=CI/imin
check('infinity invariant weighted error ball',CI+MI*MI/R**4 < 2*imin*MI,(CI+MI*MI/R**4)/(2*imin*MI))
check('infinity contraction',MI/(imin*R**4)<1,MI/(imin*R**4))
errI=MI/R**4
pi0=ev(pi,{**env,t:1/R})
pibox=pi0+arb(0,errI.upper())

# Deliberately wrong cases. Algebraic endpoint residual alone cannot choose branch.
check('negative: outgoing horizon branch is EF singular',(-2*oo).upper()<0,'e^(-Omega*x)*e^(-Omega*x)=e^(-2Omega*x), divergent as x->-infinity')
check('negative: radial Jacobian omitted',S.simplify(o/f-o)!=0,S.factor(o/f-o))
# Cos x solves u_xx=(V+Omega²)u at V=-2, Omega=1; coarse samples miss its nodes.
node_domain=arb(0).union(2*arb.pi())
check('negative: node-free claim from endpoint samples',node_domain.cos().contains(0),'u=cos(x), x in [0,2pi]; endpoints u=1, interior two zeros')
# Exact manufactured logarithmic derivative with h=1+delta.
exact_p=oo+deltaH/(1+deltaH)**2
truncated_p=oo+deltaH
# Cancel Omega symbolically to avoid parameter interval dependency.
tail=deltaH**2*(2+deltaH)/(1+deltaH)**2
check('negative: endpoint tail underestimated by factor four',tail>deltaH**2/2,tail/(deltaH**2/2))
check('negative: false mode normalization denominator',arb(0,1).contains(0),'A normalization enclosure containing zero cannot be divided out')

# Independent finite conversion from mixed static/EF profiles to common
# tau=0 EF components on r>=4. Omega<1, delta>=3.
def gg0(j):return S.factorial(j)/S.Integer(3)**j
def gg1(j):return gg0(j)+S.factorial(j+1)/S.Integer(3)**(j+1)
def gg2(j):return gg0(j)+2*S.factorial(j+1)/S.Integer(3)**(j+1)+S.factorial(j+2)/(2*S.Integer(3)**(j+2))
conversion=[sum(S.binomial(n,j)*(gg0(j)+2*gg1(j)+gg2(j)) for j in range(n+1)) for n in range(6)]
check('common tau EF profile conversion bound',max(conversion)==S.Rational(20638,243) and max(conversion)<85,conversion)

sources=['physics/gl_linear_mode.md','physics/goal004_source_geometry.md','verification/GOAL003_REVIEW_QUALIFICATIONS.md','goals/GLR-GOAL-005.md','goals/GLR-GOAL-005_REVIEW_GATES.md']
out=dict(classification='CERTIFIED_ENDPOINT_BOUNDS_ONLY_NO_ROOT',python=platform.python_version(),sympy=S.__version__,flint=flint.__version__,arb_bits=ctx.prec,
         premise='Exact historical GL Schrodinger potential; independently rederived endpoint/error estimator, no primary module import',
         input_head='0dd5be7818b2e71d28ebff2e122ac0a318a220f5',
         parameters={key:str(val) for key,val in dict(k=k,omega_trial=oo,deltaH=deltaH,R=R).items()},
         horizon={key:str(val) for key,val in dict(c=cv,residual_C=CH,lambda_=lamH,M=MH,error=errH,p=phbox).items()},
         infinity={key:str(val) for key,val in dict(q=qq,c=imin,residual_C=CI,M=MI,error=errI,p=pibox).items()},
         expressions=dict(common_tau_EF_conversion=[str(z) for z in conversion],regular_EF_b=str(bb),regular_EF_c=str(ce),physical_normalization_at_r2_over_u=str(norm2),horizon_c=str(c),horizon_scaled_residual=str(rh),infinity_coefficients={str(key):str(val) for key,val in sol.items()},infinity_scaled_residual=str(ri)),
         checks=checks,
         sha256={name:hashlib.sha256((HERE.parent/name).read_bytes()).hexdigest() for name in sources})
out['script_sha256']=hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
(HERE/'audit_goal005_spectral.json').write_text(json.dumps(out,indent=2)+'\n',encoding='utf-8')
print(json.dumps(out,indent=2))
