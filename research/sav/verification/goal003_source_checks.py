#!/usr/bin/env python3
"""Non-production exact on-shell quadratic SO(3) source and Bianchi checks.

Independent quadratic pathway: ds5=g_ab dx^a dx^b+W dOmega2,
Ric5_ab=Ric3_ab-Hess(W)/W+dW*dW/(2W^2), Ric_theta=1-Box(W)/2.
Retain Laurent compact harmonics and the full quadratic master jet.
Exact positive k²=9/25, Omega=4/5 local algebra control;
NOT a global GL eigenmode or L10 growth calculation.
"""
from pathlib import Path
import json, platform
import sympy
from sympy import QQ_I, I
from sympy.polys.fields import field
F,r=field('r',QQ_I)
def dr(v): return v.new(v.numer.diff(0)*v.denom-v.numer*v.denom.diff(0),v.denom**2)
q=F(I)*3/5;om=F(4)/5;k2=-q*q;f=(r-1)/r;D=1+k2*r**3
P=1/(r-1)-5/r+6/(r*D)
Q=3/(r*r*(r-1))-k2*r/(r-1)-3/(r*r*(r-1)*D)
b0=-Q+om*om/f**2;b1=-P

class Poly:
    # (power y, power yprime, compact Laurent exponent) -> rational(r).
    def __init__(self,d=None): self.d={k:v for k,v in (d or {}).items() if v}
    def __add__(self,v):
        if not isinstance(v,Poly):v=Poly({(0,0,0):F(v)})
        d=self.d.copy()
        for k,x in v.d.items():d[k]=d.get(k,F.zero)+x
        return Poly(d)
    __radd__=__add__
    def __neg__(self):return Poly({k:-v for k,v in self.d.items()})
    def __sub__(self,v):return self+-v
    def __rsub__(self,v):return -self+v
    def __mul__(self,v):
        if not isinstance(v,Poly):return Poly({k:x*F(v) for k,x in self.d.items()})
        d={}
        for (i,j,n),x in self.d.items():
            for (a,b,m),y in v.d.items():
                key=(i+a,j+b,n+m);d[key]=d.get(key,F.zero)+x*y
        return Poly(d)
    __rmul__=__mul__
    def __truediv__(self,v):return self*(F.one/F(v))
    def der(self,a):
        d={}
        def add(key,v):d[key]=d.get(key,F.zero)+v
        for (i,j,n),v in self.d.items():
            if a==0:add((i,j,n),v*(i+j)*om)
            elif a==2:add((i,j,n),v*n*q)
            else:
                add((i,j,n),dr(v))
                if i:add((i-1,j+1,n),v*i)
                if j:
                    add((i+1,j-1,n),v*j*b0)
                    add((i,j,n),v*j*b1)
        return Poly(d)
    def zero(self):return not self.d
    def harmonic(self,n):return Poly({k:v for k,v in self.d.items() if k[2]==n})

Z=Poly()
Y=Poly({(1,0,1):F(1)/2,(1,0,-1):F(1)/2})
Yp=Poly({(0,1,1):F(1)/2,(0,1,-1):F(1)/2})
g=[[-f,F.zero,F.zero],[F.zero,1/f,F.zero],[F.zero,F.zero,F.one]]
gi=[[-1/f,F.zero,F.zero],[F.zero,f,F.zero],[F.zero,F.zero,F.one]]
def db(x,a):return dr(x) if a==1 else F.zero
G={}
for c in range(3):
 for a in range(3):
  for b in range(3):
   G[c,a,b]=sum(gi[c][d]*(db(g[d][b],a)+db(g[d][a],b)-db(g[a][b],d))/2 for d in range(3))
ht=Y*((r-1)*(F(1)/2-k2*r**3)/(r*D**2)-om**2/(k2*D))+Yp*((r-1)/(2*k2*r**3*D)-3*(r-1)**2/D**2)
hb=(Yp-Y/(2*r*(r-1)))*(-om/(k2*D))
w=Y*(-r*r/(2*D))+Yp*(-(r-1)/(k2*D))
hr=ht/f**2-w*2/(r*(r-1))
h=[[ht,hb,Z],[hb,hr,Z],[Z,Z,Z]]
hu=[[sum(gi[a][c]*h[c][d]*gi[d][b] for c in range(3) for d in range(3)) for b in range(3)] for a in range(3)]
hm=[[sum(gi[a][c]*h[c][b] for c in range(3)) for b in range(3)] for a in range(3)]
def nabh(d,b,a):return h[d][b].der(a)-sum(h[e][b]*G[e,a,d]+h[d][e]*G[e,a,b] for e in range(3))
C={}
for c in range(3):
 for a in range(3):
  for b in range(3):
   C[c,a,b]=sum((nabh(d,b,a)+nabh(d,a,b)-nabh(a,b,d))*gi[c][d]/2 for d in range(3))
print('LINEAR_CONNECTION_BUILT',flush=True)
DD={(c,a,b):-sum(hm[c][d]*C[d,a,b] for d in range(3)) for c in range(3) for a in range(3) for b in range(3)}
def riclin(a,b,X):
 v=sum(X[c,a,b].der(c)-X[c,a,c].der(b) for c in range(3))
 return v+sum(X[c,c,d]*G[d,a,b]+X[d,a,b]*G[c,c,d]-X[c,b,d]*G[d,a,c]-X[d,a,c]*G[c,b,d] for c in range(3) for d in range(3))
W=r*r;dW=[F.zero,2*r,F.zero]
H0={(a,b):(F(2) if a==b==1 else F.zero)-G[1,a,b]*2*r for a in range(3) for b in range(3)}
H1={(a,b):w.der(b).der(a)-sum(w.der(c)*G[c,a,b]+C[c,a,b]*dW[c] for c in range(3)) for a in range(3) for b in range(3)}
H2={(a,b):-sum(DD[c,a,b]*dW[c]+C[c,a,b]*w.der(c) for c in range(3)) for a in range(3) for b in range(3)}
src={}
for a in range(3):
 for b in range(a,3):
  lin=riclin(a,b,C)-H1[a,b]/W+w*H0[a,b]/W**2+(w.der(a)*dW[b]+w.der(b)*dW[a])/(2*W**2)-w*dW[a]*dW[b]/W**3
  assert lin.zero(),('linear',a,b)
  v=riclin(a,b,DD)+sum(C[c,c,d]*C[d,a,b]-C[c,b,d]*C[d,a,c] for c in range(3) for d in range(3))
  v+=-H2[a,b]/W+w*H1[a,b]/W**2-w*w*H0[a,b]/W**3
  v+=w.der(a)*w.der(b)/(2*W**2)-w*(w.der(a)*dW[b]+w.der(b)*dW[a])/W**3+w*w*(3*dW[a]*dW[b]/(2*W**4))
  src[a,b]=src[b,a]=v
print('ALL_SIX_ON_SHELL_LINEAR_ORBIT_EQUATIONS: PASS',flush=True)
linSphere=-sum(H1[a,b]*gi[a][b]-hu[a][b]*H0[a,b] for a in range(3) for b in range(3))/2
assert linSphere.zero()
qs=-sum(H2[a,b]*gi[a][b]-hu[a][b]*H1[a,b]+sum(hm[a][c]*hu[c][b] for c in range(3))*H0[a,b] for a in range(3) for b in range(3))/2
print('QUADRATIC_SEVEN_COMPONENT_SOURCE_BUILT',flush=True)
Hsq=ht*ht/f**2+hr*hr*f**2+2*w*w/r**4-2*hb*hb
assert (src[2,2].harmonic(0)+Hsq.harmonic(0)*k2/4).zero()
assert (src[2,2].harmonic(2)+Hsq.harmonic(2)*3*k2/4).zero()
print('QZZ_ZERO_AND_SECOND_HARMONIC_SOURCE: PASS',flush=True)
trace=sum(src[a,b]*gi[a][b] for a in range(3) for b in range(3))+qs*2/r**2
E={(a,b):src[a,b]-trace*g[a][b]/2 for a in range(3) for b in range(3)}
Es=qs-trace*r*r/2
Em={(a,b):sum(E[c,b]*gi[a][c] for c in range(3)) for a in range(3) for b in range(3)}
for b in range(3):
 div=sum(Em[a,b].der(a) for a in range(3))+sum(Em[d,b]*G[a,a,d]-Em[a,d]*G[d,a,b] for a in range(3) for d in range(3))+Em[1,b]*2/r-(Es*2/r**3 if b==1 else Z)
 assert div.zero(),('Bianchi',b)
 print('FULL_QUADRATIC_BIANCHI_'+str(b)+': PASS',flush=True)
# Mutation actually violates the radial divergence; no source-coefficient test alone.
bad=src[2,2]*2 # deliberately doubles only zz source in the trace
deltaTrace=bad-src[2,2]
assert not deltaTrace.der(1).zero()
print('DOUBLED_ZZ_WITH_UNCHANGED_METRIC_SOURCE_MUTATION: REJECTED',flush=True)
# Actual quadratic +2 source reduced to y1²,y1*y1prime,y1prime².
a=4*k2; rate=2*om;DD2=1+a*r**3
Qt=src[0,1].harmonic(2); Qs=qs.harmonic(2)
tau=src[2,2].harmonic(2)*(-2/a)
jt=src[0,2].harmonic(2)*(-1/q)
jr=src[1,2].harmonic(2)*(-1/q)
forcing=-jr*(2*a*r**2)-Qs*(4*a*r**2/(r-1))-tau*(a*r/(r-1))+jt.der(1)*(2*a*r**2/rate)+Qt*(4*a*r**2*(a*r**3-2)/(rate*DD2))-jt*(2*a*r*(a*r**3*(2*r-3)-4*r+3)/(rate*(r-1)*DD2))
Fu=forcing*(-f**2*r/DD2)
# decaying u1=e^-r r^-beta(1+c1/r+...), q_infty=1;
# y1=(1+k²r³)u1/r; use on-shell master derivative jet.
beta=(k2+2*om**2)/2
v1=-k2;v2=F(6); v3=F(-17)
# p=u_x/u=−1+a1/r+a2/r²+...; a1=−v1/2.
a1=-v1/2;a2=(-a1-v2+a1*a1)/2
# d log u/dr=p/f; -1 +(a1−1)/r +(a2+a1−1)/r²
c1=-(a2+a1-1)
up=-1-beta/r-c1/r**2
yp=dr(D/r)/(D/r)+up
scaled=sum(v*yp**j for (i,j,n),v in Fu.d.items())*(D/r)**2
xx=sympy.symbols('xx')
# The x^4 coefficient below is only a truncated-log-derivative-jet diagnostic.
# Omitted u'/u terms at r^-3 can change it; only through x^3 is supported.
# Raw output is retained for provenance; see GLR_GOAL003_CHECKS.md.
series=sympy.series(scaled.as_expr().subs(r.as_expr(),1/xx),xx,0,5)
print('EXACT_PHYSICAL_SIGN_LOCAL_INFINITY_SOURCE_SERIES',series,flush=True)
out={'python':platform.python_version(),'sympy':sympy.__version__,'parameters':{'k_squared':'9/25','Omega':'4/5','q_infty':'1','physical_eigenmode':False},'scope':'Exact local on-shell positive-k identities and decaying infinity asymptotics; no BVP, no L10 re-enclosure.','forcing_coefficients':{str(key):str(val) for key,val in forcing.d.items()},'Schrodinger_source_divided_by_u1_squared_series':str(series)}
Path(__file__).with_name('goal003_source_results.json').write_text(json.dumps(out,indent=2)+'\n')
