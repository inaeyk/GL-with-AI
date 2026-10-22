#!/usr/bin/env python3
"""Non-production symbolic forced-master elimination, r0=1."""
import sympy as z
from pathlib import Path
r,s,K=z.symbols('r s K', nonzero=True)
f=1-1/r
T,B,C,S=[z.Function(n)(r) for n in ('T','B','C','S')]
g=z.diag(-f,1/f,1); gi=g.inv(); h=z.Matrix([[T,B,0],[B,C,0],[0,0,0]])
hi=-gi*h*gi
DB=lambda a,i:z.diff(a,r) if i==1 else z.S.Zero
DH=lambda a,i:s*a if i==0 else z.diff(a,r) if i==1 else z.I*K*a
G={(c,a,b):z.factor(sum(gi[c,d]*(DB(g[d,b],a)+DB(g[d,a],b)-DB(g[a,b],d))/2 for d in range(3))) for c in range(3) for a in range(3) for b in range(3)}
cc={(c,a,b):z.factor(sum((gi[c,d]*(DH(h[d,b],a)+DH(h[d,a],b)-DH(h[a,b],d))+hi[c,d]*(DB(g[d,b],a)+DB(g[d,a],b)-DB(g[a,b],d)))/2 for d in range(3))) for c in range(3) for a in range(3) for b in range(3)}
rr=S/(2*r)
H={(a,b):DH(DH(rr,b),a)-sum(DH(rr,c)*G[c,a,b] for c in range(3))-cc[1,a,b] for a in range(3) for b in range(3)}
Ric={}
for a in range(3):
 for b in range(a,3):
  val=sum(DH(cc[c,a,b],c)-DH(cc[c,a,c],b) for c in range(3))+sum(cc[c,c,d]*G[d,a,b]+cc[d,a,b]*G[c,c,d]-cc[c,b,d]*G[d,a,c]-cc[d,a,c]*G[c,b,d] for c in range(3) for d in range(3))
  Ric[a,b]=z.factor(val-2*H[a,b]/r-2*rr*G[1,a,b]/r**2)
boxr=-sum(gi[a,b]*G[1,a,b] for a in range(3) for b in range(3))
dbox=sum(-hi[a,b]*G[1,a,b]+gi[a,b]*H[a,b] for a in range(3) for b in range(3))
Ric['sphere']=z.factor(-hi[1,1]-2*f*z.diff(rr,r)-rr*boxr-r*dbox)
tau,jt,jr,y,Qtr,Qss=[z.Function(n)(r) for n in ('tau','jt','jr','y','Qtr','Qss')]
Sx=(r-1)*B/s-y/(2*K**2*r)
Tx=f/s*(f*z.diff(B,r)+(z.diff(f,r)+2*f/r)*B-jt-s*tau)
Cx=Tx/f**2+tau/f-2*Sx/(f*r**2)
def sub(e):return z.factor(e.subs({T:Tx,C:Cx,S:Sx}, simultaneous=True).doit())
divr=-s*B/f+f*z.diff(C,r)+(3*z.diff(f,r)/2+2*f/r)*C+z.diff(f,r)*T/(2*f**2)-2*S/r**3-z.diff(tau,r)-jr
Er=sub(divr); Et=sub(Ric[0,1])+Qtr
# eliminate second derivative B using radial-divergence and tr Einstein
D=1+K**2*r**3; a=K**2
b=2*r**2*(jt-r*Qtr)/D
Bh=-s/(a*D)*(z.diff(y,r)-y/(2*r*(r-1)))
Bs=Bh+b
F=-2*a*r**2*jr-4*a*r**2*Qss/(r-1)-a*r*tau/(r-1)+2*a*r**2*z.diff(jt,r)/s+4*a*r**2*(a*r**3-2)*Qtr/(s*D)-2*a*r*(a*r**3*(2*r-3)-4*r+3)*jt/(s*(r-1)*D)
P=1/(r-1)-5/r+6/(r*D)
Q=3/(r**2*(r-1))-a*r/(r-1)-3/(r**2*(r-1)*D)
Ly=z.diff(y,r,2)+P*z.diff(y,r)+(Q-s**2/f**2)*y
checks={}
def check(name,e):
 assert z.cancel(e)==0,name
 checks[name]=True;print(name+': PASS',flush=True)
check('TR_ALGEBRAIC_RECONSTRUCTION',Et.subs(B,Bs).doit())
Er2=Er.subs(B,Bs).doit()
Es2=(sub(Ric['sphere'])+Qss).subs(B,Bs).doit()
check('FORCED_MASTER_COMBINATION',Es2-(r-1)*Er2/2-(r-1)/(4*a*r**2)*(Ly-F))
# Arbitrary smooth q manufactured source. This checks physical K real,
# not the earlier imaginary-k on-shell specialization.
manufact={tau:2*Ric[2,2]/a,jt:2*Ric[0,2]/(z.I*K),jr:2*Ric[1,2]/(z.I*K),Qtr:-Ric[0,1],Qss:-Ric['sphere'],y:2*a*r*((r-1)*B/s-S)}
check('ARBITRARY_Q_MASTER_IDENTITY',(Ly-F).subs(manufact,simultaneous=True).doit())
check('ARBITRARY_Q_B_RECONSTRUCTION',(Bs-B).subs(manufact,simultaneous=True).doit())
# Source-dependent algebraic reconstruction, with the master used once.
pt=f/s*(f*z.diff(b,r)+(z.diff(f,r)+2*f/r)*b-jt-s*tau)-f**2*F/(a*D)
Ah=(r-1)*(z.Rational(1,2)-a*r**3)/(r*D**2)-s**2/(a*D)
At=(r-1)/(2*a*r**3*D)-3*(r-1)**2/D**2
Th=Ah*y+At*z.diff(y,r)
TT=Th+pt
SS=-r**2*y/(2*D)-(r-1)*z.diff(y,r)/(a*D)+(r-1)*b/s
CC=TT/f**2+tau/f-2*SS/(r*(r-1))
for name,e in [('T',TT-T),('S',SS-S),('C',CC-C)]:
 check('ARBITRARY_Q_'+name+'_RECONSTRUCTION',e.subs(manufact,simultaneous=True).doit())
# Residual Bianchi closes remaining constraints, independently of source shape.
d=z.Function('d')(r);e=f*z.diff(d,r)+(z.diff(f,r)+2*f/r)*d
Ett=-f*e/2;Err=e/(2*f)-d/r;Ess=(r-1)*d/2;Erz=z.I*K*d/2
bianchi=f*z.diff(Err,r)+(3*z.diff(f,r)/2+2*f/r)*Err+z.diff(f,r)*Ett/(2*f**2)-2*Ess/r**3+z.I*K*Erz-z.diff(e,r)/2
check('REMAINING_BIANCHI_ALGEBRAIC_CLOSURE',bianchi+(a+1/r**3)*d/2)
# Independent off-shell scalar transform.
w=D/r
V=a*f+(6*r-11)*(r-1)/r**4+18*(r-1)**2/(r**4*D**2)-6*(4*r-5)*(r-1)/(r**4*D)
u=z.Function('u')(r)
check('INHOMOGENEOUS_SCHRODINGER_SIGN',Ly.subs(y,w*u).doit()+w/f**2*(-f*z.diff(f*z.diff(u,r),r)+(V+s**2)*u))
# A source-free lift cannot reconstruct a generic forced q.
mut=(Bh-B).subs(manufact,simultaneous=True).doit()
assert z.cancel(mut)!=0
checks['OMITTED_AFFINE_SOURCE_REJECTED']=True
print('OMITTED_AFFINE_SOURCE_REJECTED: PASS')
import platform,json
result={'python':platform.python_version(),'sympy':z.__version__,'checks':checks,'scope':'Exact arbitrary-q curved Schwarzschild identities, K != 0, s != 0; no physical BVP or interval inputs.'}
Path(__file__).with_name('goal003_elimination_results.json').write_text(json.dumps(result,indent=2)+'\n')
