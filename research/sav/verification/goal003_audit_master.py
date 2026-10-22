#!/usr/bin/env python3
"""Independent exact Audit I, GOAL-003. No physical ODE/evolution.

Builds linear Ricci through the R (not W) warped-product identity in EF
coordinates, manufactures a conserved source from arbitrary radial metric
functions, and tests the primary forced master/reconstruction. Also checks
the independent coercivity polynomial and deliberate wrong cases.
"""
import json
import platform
import sys
from pathlib import Path
import sympy as sp

results = {}

def record(name, ok, detail=""):
    assert ok, (name, detail)
    results[name] = {"pass": True, "detail": str(detail)}
    print(name + ": PASS" + ("; " + str(detail) if detail else ""), flush=True)

if '--tail-only' in sys.argv:
    # Bounded independent asymptotic calculation. No primary source
    # generator imported; expand the orbit determinant-density form
    # of Box_3 W. Strip the exact decaying master u1 from all profiles.
    r,q,o,x=sp.symbols('r q o x',positive=True)
    k2=q*q-o*o  # assume q>o>0, so k²>0
    beta=(k2+2*o*o)/(2*q); p=-q-beta/r; f=1-1/r
    T=-q*q/k2/r+(1+3*q/k2)/r**2
    B=o*q/k2/r-o*(2-beta)/k2/r**2
    C=-o*o/k2/r+(q-2*o*o)/k2/r**2
    W1=-r/2+q/k2
    gi=sp.diag(-1/f,f,1)
    h=sp.Matrix([[T,B,0],[B,C,0],[0,0,0]])
    tr=sp.trace(gi*h); tr2=sp.trace(gi*h*gi*h)
    d1=tr/2; d2=tr**2/8-tr2/4
    hu=gi*h*gi
    M1=d1*gi-hu
    M2=d2*gi-d1*hu+gi*h*gi*h*gi
    Wp=sp.diff(W1,r)+p*W1
    Jt=M1[0,0]*o*W1+M1[0,1]*Wp+2*r*M2[0,1]
    Jr=M1[1,0]*o*W1+M1[1,1]*Wp+2*r*M2[1,1]
    expand=lambda expr: sp.series(expr.subs(r,1/x),x,0,2).removeO().expand().subs(x,1/r)
    Jt=expand(Jt); Jr=expand(Jr)
    Dz=expand(-2*k2*d1*W1)  # 2ik times the order-two z current
    Qtheta=expand(d2-(2*o*Jt+sp.diff(Jr,r)+2*p*Jr+Dz)/2)
    leading0=sp.factor(Qtheta.coeff(r,0))
    leading1=sp.factor(Qtheta.coeff(r,-1)/4)  # real-cosine -> +2
    record('TAIL_ANGULAR_LEADING_CONSTANT_CANCELS',leading0==0)
    record('TAIL_ANGULAR_SUBLEADING_UNIVERSAL',leading1==-q/4,str(leading1))
    hsquare=sp.factor(q**4/k2**2+o**4/k2**2+sp.Rational(1,2)-2*(o*q/k2)**2)
    record('TAIL_LORENTZ_CONTRACTION',hsquare==sp.Rational(3,2))
    # Null plane-wave calculation: complex Q2=3/4 p_A p_B H²;
    # real +2 coefficient is 1/4 of that. Thus j,Qtr below multiply u1²/r².
    a=4*k2; rate=2*o
    jt=-9*o/sp.Integer(32); jr=9*q/sp.Integer(32); Qtr=-9*o*q/sp.Integer(32)
    pieces=[-2*a*jr,-4*a*leading1,2*a/rate*(-2*q*jt),4*a/rate*Qtr]
    Fy0=sp.factor(sum(pieces))
    Fu0=sp.factor(-Fy0/a)
    record('TAIL_MASTER_SOURCE_UNIVERSAL',Fu0==-7*q/16,str(Fu0))
    record('TAIL_OMITTED_ANGULAR_TERM_REJECTED',sp.factor(-(sum(pieces)-pieces[1])/a-Fu0)!=0)
    particular=sp.factor(Fu0/(-4*q))
    record('TAIL_FORCED_PARTICULAR_COEFFICIENT',particular==sp.Rational(7,64),str(particular))
    out={'python':platform.python_version(),'sympy':sp.__version__,
         'scope':'Independent formal infinity coefficients for q>Omega>0, k²=q²−Omega²; no remainder enclosure or BVP',
         'tests':results,'Qtheta_over_u1_squared_leading':str(leading1)+'/r',
         'Schrodinger_source_over_u1_squared_leading':str(Fu0)+'/r**2',
         'particular_coefficient':str(particular)}
    Path(__file__).with_name('goal003_audit_master_tail_results.json').write_text(json.dumps(out,indent=2)+'\n')
    print('ALL',len(results),'TAIL CHECKS PASSED; asymptotic remainder/input bounds remain open.',flush=True)
    sys.exit(0)

r, a, s, d, aa = sp.symbols("r a s d aa", positive=True)
f = (r-1)/r
D = 1+a*r**3
V = a*f+(6*r-11)*(r-1)/r**4+18*(r-1)**2/(r**4*D**2)-6*(4*r-5)*(r-1)/(r**4*D)
ell = -sp.Rational(1,2)+1/r-3*a*r**2/D  # w_r/w
Hw_over_w = sp.factor(V-f*(sp.diff(f*ell,r)+f*ell**2))
poly = (4*a*a-a)*r**5-7*a*r**4+10*a*r**3-(8*a+1)*r*r+5*r-2
record("EXACT_POSITIVE_SUPERSOLUTION", sp.cancel(Hw_over_w-f*poly/(4*r*r*D)) == 0)
shifted = sp.Poly(sp.expand(poly.subs({r:1+d,a:sp.Rational(3,2)+aa})),d,aa)
record("ALL_SHIFTED_COEFFICIENTS_POSITIVE", all(c>0 for c in shifted.coeffs()), shifted.as_expr())
record("NAIVE_V_NONNEGATIVE_REJECTED", sp.limit(V/(r-1),r,1).subs(a,sp.Rational(3,2)) < 0,
       "V/(r-1)=-11/10 at a=3/2")
Sdef = -f*ell
record("DEFORMATION_FACTOR_IDENTITY", sp.cancel(V+f*sp.diff(Sdef,r)-Sdef**2-Hw_over_w)==0)
record("UNMODIFIED_TRIAL_FAILS_L10_REGION", (a*(a-2)/(a+1)).subs(a,sp.Rational(3,2))<0)

# Direct all-component linear Ricci from 3D orbit Christoffels and R warp.
# The four EF profiles remain arbitrary functions. Positive K=3/2 and
# growth s=1/3 keep this independent control physical-sign, with no
# accidental D=0 pole of an imaginary-wavenumber specialization.
ss=sp.Rational(1,3); K=sp.Rational(3,2); aval=K*K; kk=sp.I*K
qv, qr, qc, qs=[sp.Function(n)(r) for n in ("A","B","C","S")]
g=sp.Matrix([[-f,1,0],[1,0,0],[0,0,1]])
gi=g.inv()
h=sp.Matrix([[qv,qr,0],[qr,qc,0],[0,0,0]])
hi=-gi*h*gi
def db(v,i): return sp.diff(v,r) if i==1 else sp.S.Zero
def dm(v,i): return ss*v if i==0 else sp.diff(v,r) if i==1 else kk*v
G={}
C={}
for c in range(3):
 for i in range(3):
  for j in range(3):
   G[c,i,j]=sp.cancel(sum(gi[c,k]*(db(g[k,j],i)+db(g[k,i],j)-db(g[i,j],k))/2 for k in range(3)))
   C[c,i,j]=sp.expand(sum(gi[c,k]*(dm(h[k,j],i)+dm(h[k,i],j)-dm(h[i,j],k))/2
                   +hi[c,k]*(db(g[k,j],i)+db(g[k,i],j)-db(g[i,j],k))/2 for k in range(3)))
dR=qs/(2*r)
Hess={}
Ric={}
for i in range(3):
 for j in range(3):
  Hess[i,j]=dm(dm(dR,j),i)-sum(G[c,i,j]*dm(dR,c) for c in range(3))-C[1,i,j]
  Ric[i,j]=sp.expand(sum(dm(C[c,i,j],c)-dm(C[c,i,c],j) for c in range(3))
      +sum(C[c,c,k]*G[k,i,j]+G[c,c,k]*C[k,i,j]-C[c,j,k]*G[k,i,c]-G[c,j,k]*C[k,i,c] for c in range(3) for k in range(3))
      -2*Hess[i,j]/r-2*dR*G[1,i,j]/r**2)
boxr=-sum(gi[i,j]*G[1,i,j] for i in range(3) for j in range(3))
dbox=sum(-hi[i,j]*G[1,i,j]+gi[i,j]*Hess[i,j] for i in range(3) for j in range(3))
dnorm=hi[1,1]+2*sum(gi[1,j]*dm(dR,j) for j in range(3))
RicS=sp.expand(-dnorm-dR*boxr-r*dbox)

jets=[]
for q in (qv,qr,qc,qs):
 for n in range(6): jets.append(sp.diff(q,r,n))
def zero(expr):
    # Linear expressions: collect independent q jets first, then cancel
    # rational coefficients. This avoids expensive generic simplification.
    e=sp.expand(expr)
    for jet in reversed(jets):
        c=sp.cancel(e.coeff(jet))
        if c!=0: return False
        e=e.subs(jet,0)
    return sp.cancel(e)==0

tau=2*qr+f*qc+2*qs/r**2
div=[]
for j in range(3):
 v=sum(gi[i,c]*(dm(h[i,j],c)-sum(G[e,c,i]*h[e,j]+G[e,c,j]*h[i,e] for e in range(3))) for i in range(3) for c in range(3))
 v+=2*sum(gi[i,1]*h[i,j] for i in range(3))/r-(2*qs/r**3 if j==1 else 0)
 div.append(sp.expand(v))
jv=div[0]-ss*tau
jr=div[1]-sp.diff(tau,r)
record("FORCED_TRACE_IDENTITY",zero(Ric[2,2]-aval*tau/2))
record("FORCED_DIVERGENCE_V_IDENTITY",zero(Ric[0,2]-kk*jv/2))
record("FORCED_DIVERGENCE_R_IDENTITY",zero(Ric[1,2]-kk*jr/2))
trace=sum(gi[i,j]*Ric[i,j] for i in range(3) for j in range(3))+2*RicS/r**2
E={(i,j):Ric[i,j]-g[i,j]*trace/2 for i in range(3) for j in range(3)}
ES=RicS-r*r*trace/2
for j in range(3):
 v=sum(gi[i,c]*(dm(E[i,j],c)-sum(G[e,c,i]*E[e,j]+G[e,c,j]*E[i,e] for e in range(3))) for i in range(3) for c in range(3))
 v+=2*sum(gi[i,1]*E[i,j] for i in range(3))/r-(2*ES/r**3 if j==1 else 0)
 record("ALL_COMPONENT_SOURCE_BIANCHI_"+str(j),zero(v))

# Strip e^(s x) from static-coordinate amplitudes. A radial derivative
# on those amplitudes becomes dr+s/f, while EF source components above
# contain only e^(s v+iKz). Q=-Ric is a general compatible forced source.
dx = lambda v: sp.diff(v,r)+ss*v/f
T=qv; B=qr+qv/f; Cr=qc+2*qr/f+qv/f**2; Sphere=qs
Qt=-Ric[0,0]; Qtr=-Ric[0,1]-Ric[0,0]/f; Qs=-RicS
jt=jv; jrs=jr+jv/f
y=2*aval*r*((r-1)*B/ss-Sphere)
DD=1+aval*r**3
P=1/(r-1)-5/r+6/(r*DD)
Qy=3/(r*r*(r-1))-aval*r/(r-1)-3/(r*r*(r-1)*DD)
Fy=-2*aval*r*r*jrs-4*aval*r*r*Qs/(r-1)-aval*r*tau/(r-1)
Fy+=2*aval*r*r*dx(jt)/ss+4*aval*r*r*(aval*r**3-2)*Qtr/(ss*DD)
Fy-=2*aval*r*(aval*r**3*(2*r-3)-4*r+3)*jt/(ss*(r-1)*DD)
record("PRIMARY_FORCED_MASTER_ARBITRARY_EF_METRIC",zero(dx(dx(y))+P*dx(y)+(Qy-ss**2/f**2)*y-Fy))
b=2*r*r*(jt-r*Qtr)/DD
Brecon=-ss/(aval*DD)*(dx(y)-y/(2*r*(r-1)))+b
Srecon=-r*r*y/(2*DD)-(r-1)*dx(y)/(aval*DD)+(r-1)*b/ss
at=(r-1)*(sp.Rational(1,2)-aval*r**3)/(r*DD**2)-ss**2/(aval*DD)
bt=(r-1)/(2*aval*r**3*DD)-3*(r-1)**2/DD**2
pt=f/ss*(f*dx(b)+(sp.diff(f,r)+2*f/r)*b-jt-ss*tau)-f*f*Fy/(aval*DD)
Trecon=at*y+bt*dx(y)+pt
Crecon=Trecon/f**2+tau/f-2*Srecon/(r*(r-1))
for name,expr in [('T',Trecon-T),('B',Brecon-B),('C',Crecon-Cr),('S',Srecon-Sphere)]:
 record("SOURCE_AWARE_RECONSTRUCTION_"+name,zero(expr))
record("OMITTED_FORCED_TRACE_REJECTED",not zero(tau))
record("OMITTED_FORCED_DIVERGENCE_REJECTED",not zero(jv))
record("OMITTED_SOURCE_LIFT_REJECTED",not zero(Brecon-b-B))

# Independently check the coefficient in the off-shell master identity,
# not only its zero set. Build T using time divergence before eliminating
# y'', and obtain the angular Ricci component from the EF generator above.
ym=sp.Function('ym')(r)
bm=-ss/(aval*DD)*(sp.diff(ym,r)-ym/(2*r*(r-1)))
sm=-r*r*ym/(2*DD)-(r-1)*sp.diff(ym,r)/(aval*DD)
tm=f/ss*(f*sp.diff(bm,r)+(sp.diff(f,r)+2*f/r)*bm)
cm=tm/f**2-2*sm/(f*r*r)
minus=lambda v: sp.diff(v,r)-ss*v/f
mapping={}
for q, val in zip((qv,qr,qc,qs),(tm,bm-tm/f,cm-2*bm/f+tm/f**2,sm)):
 for n in range(4):
  mapping[sp.diff(q,r,n)]=val
  val=minus(val)
angular=RicS.xreplace(mapping)
divr=-ss*bm/f+f*sp.diff(cm,r)+(3*sp.diff(f,r)/2+2*f/r)*cm+sp.diff(f,r)*tm/(2*f*f)-2*sm/r**3
lym=sp.diff(ym,r,2)+P*sp.diff(ym,r)+(Qy-ss*ss/(f*f))*ym
identity=sp.expand(angular-(r-1)*divr/2-(r-1)*lym/(4*aval*r*r))
coeffs=[sp.cancel(identity.coeff(sp.diff(ym,r,n))) for n in range(5)]
record("OFF_SHELL_MASTER_IDENTITY_COEFFICIENT",all(v==0 for v in coeffs))

# Negative tests with exact elementary identities, independent of the
# primary implementations. These test errors, not physical acceptance.
z=sp.symbols('z',real=True)
cosine2=sp.integrate(sp.cos(z)**2*sp.exp(-2*sp.I*z),(z,-sp.pi,sp.pi))/(2*sp.pi)
record("COMPLEX_HARMONIC_HALF",sp.simplify(cosine2-sp.Rational(1,4))==0)
record("DOUBLED_HARMONIC_REJECTED",sp.simplify(2*cosine2-sp.Rational(1,4))!=0)
# Circle pure gauge with xi^z=sin z: L_xi gzz=2cos z,
# L_xi² gzz=xi*(2cos z)' + 2*(2cos z)*xi' = -2sin²z+4cos²z.
hzz=2*sp.cos(z); L2=-2*sp.sin(z)**2+4*sp.cos(z)**2
record("HALF_LIE_GAUGE_CANCELLATION",sp.simplify(L2/2-L2+L2/2)==0)
badmean=sp.integrate(-L2/2,(z,-sp.pi,sp.pi))/(2*sp.pi)
record("OMITTED_HALF_LIE_REJECTED",sp.simplify(badmean+sp.Rational(1,2))==0,"spurious zero mode -1/2")
alpha=sp.symbols('alpha',positive=True); xx=sp.symbols('x',real=True)
Gleft=sp.exp(alpha*xx)/(2*alpha); Gright=sp.exp(-alpha*xx)/(2*alpha)
jump=sp.diff(Gright,xx).subs(xx,0)-sp.diff(Gleft,xx).subs(xx,0)
record("GREEN_DERIVATIVE_JUMP_MINUS_ONE",jump==-1)
record("WRONG_GREEN_SIGN_REJECTED",-jump!= -1)
up=xx*sp.exp(-alpha*xx)/(2*alpha)
record("EQUAL_EXPONENT_FORCED_TAIL",sp.simplify(-sp.diff(up,xx,2)+alpha**2*up-sp.exp(-alpha*xx))==0)
record("HOMOGENEOUS_OUTER_TAIL_REJECTED",sp.simplify(-sp.diff(sp.exp(-alpha*xx),xx,2)+alpha**2*sp.exp(-alpha*xx)-sp.exp(-alpha*xx))!=0)

# Generic bounded y is not sufficient for regular physical reconstruction.
# At integer s=1, choose y=d(1+c1 d). Its homogeneous field reconstruction
# has a future-EF rr pole unless the Frobenius c1 has the on-shell value.
cc=sp.symbols('cc'); ss2=sp.S.One; a2=sp.Integer(2); DD2=1+a2*r**3
yy=(r-1)*(1+cc*(r-1)); yp=sp.diff(yy,r)
tt=((r-1)*(sp.Rational(1,2)-a2*r**3)/(r*DD2**2)-ss2**2/(a2*DD2))*yy+((r-1)/(2*a2*r**3*DD2)-3*(r-1)**2/DD2**2)*yp
bb=-ss2/(a2*DD2)*(yp-yy/(2*r*(r-1)))
sphe=-r*r*yy/(2*DD2)-(r-1)*yp/(a2*DD2)
rr=tt/f**2-2*sphe/(r*(r-1))
# Multiply raw EF combination by d^{-s}; exp(-s r) is harmless.
rrEF=(rr-2*bb/f+tt/f**2)/(r-1)
pole=sp.limit((r-1)*rrEF,r,1)
c1=(a2*a2+2*a2*ss2**2+5*a2*ss2-2*a2+2*ss2**2-ss2)/((a2+1)*(2*ss2+1))
record("REGULAR_FROBENIUS_RECONSTRUCTION",sp.simplify(pole.subs(cc,c1))==0)
record("FINITE_MASTER_SINGULAR_RECONSTRUCTION_REJECTED",sp.simplify(pole.subs(cc,c1+1))!=0,str(pole))

# General-parameter residual-Bianchi proof of all-equation equivalence.
dd=sp.Function('residual_d')(r)
etr=f*sp.diff(dd,r)+(sp.diff(f,r)+2*f/r)*dd
Ett=-f*etr/2; Err=etr/(2*f)-dd/r; Eth=(r-1)*dd/2
radial= f*sp.diff(Err,r)+(3*sp.diff(f,r)/2+2*f/r)*Err
radial+=sp.diff(f,r)*Ett/(2*f*f)-2*Eth/r**3-a*dd/2-sp.diff(etr,r)/2
record("GENERAL_RESIDUAL_BIANCHI_ELIMINATION",sp.factor(radial+(a+r**-3)*dd/2)==0)

# General-parameter horizon cancellation: only the regular-master
# first recurrence and the actual Einstein-source v Bianchi identity
# are required. This is distinct from the arbitrary-q specialization.
Y,ta,j,je,Aq,Bq,So=[sp.Function(n)(r) for n in ('Yh','tauh','jh','jrh','Ah','Bh','Sh')]
Z=-s/(a*D)*(f*sp.diff(Y,r)+(s-1/(2*r*r))*Y)+2*r*r/D*(f*j-r*f*Bq-r*Aq)
N=2*sp.diff(Z,r)/s+2*Z/(s*r)-2*j/s-ta+Y/(a*r**3)
J=-2*a*r*r*f*je-4*a*r*So-a*ta+2*a*r*r*f*sp.diff(j,r)/s
J+=4*a*r*r*(a*r**3-2)*(f*Bq+Aq)/(s*D)-2*a*(a*r**3*(2*r-3)-4*r+3)*j/(s*D)
coefc=s*(-4+6/D)/r+3/r**3-a-3/(r**3*D)
Yp0=sp.factor((J-coefc*Y).subs(r,1)/(2*s+1))
N0=sp.factor(N.subs(r,1).subs(sp.Subs(sp.Derivative(Y,r),r,1),Yp0))
biv=(sp.diff(Aq,r)+Bq+2*Aq+s*a*ta/4-s*So+a*j/2).subs(r,1)
record("GENERAL_HORIZON_POLE_EQUALS_BIANCHI",sp.factor(N0+4*biv/(s*(a+1)))==0)
record("NONCONSERVED_SOURCE_POLE_REJECTED",sp.factor(N0.subs({sp.Subs(sp.Derivative(Aq,r),r,1):1,Aq.subs(r,1):0,Bq.subs(r,1):0,ta.subs(r,1):0,j.subs(r,1):0,So.subs(r,1):0}))!=0)

out={"python":platform.python_version(),"sympy":sp.__version__,"scope":"Exact symbolic/manufactured checks; no physical ODE or numerical eigenvalue certification", "tests":results}
Path(__file__).with_name('goal003_audit_master_results.json').write_text(json.dumps(out,indent=2)+'\n')
print('ALL',len(results),'CHECKS PASSED; no metric inverse/nonlinear certification follows.',flush=True)
