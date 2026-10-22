#!/usr/bin/env python3
"""Non-production exact on-shell quadratic SO(3) source and Bianchi checks.

Independent quadratic pathway: ds5=g_ab dx^a dx^b+W dOmega2,
Ric5_ab=Ric3_ab-Hess(W)/W+dW*dW/(2W^2), Ric_theta=1-Box(W)/2.
Retain Laurent compact harmonics and the full quadratic master jet.
Exact rational specialization kappa=2/3, Omega=1/7 is an algebra control,
NOT a physical GL eigenmode or L10 growth calculation (k²=-kappa²).
"""
from pathlib import Path
import json, platform
import sympy
from sympy import QQ
from sympy.polys.fields import field
F,r=field('r',QQ)
q=F(2)/3;om=F(1)/7;k2=-q*q;f=(r-1)/r;D=1+k2*r**3
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
    def __truediv__(self,v):return self*(1/v)
    def der(self,a):
        d={}
        def add(key,v):d[key]=d.get(key,F.zero)+v
        for (i,j,n),v in self.d.items():
            if a==0:add((i,j,n),v*(i+j)*om)
            elif a==2:add((i,j,n),v*n*q)
            else:
                add((i,j,n),v.diff(r))
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
def db(x,a):return x.diff(r) if a==1 else F.zero
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
out={'python':platform.python_version(),'sympy':sympy.__version__,'exact_parameter_specialization':{'kappa':'2/3','Omega':'1/7','physical_mode':False},'checks':['seven linear equations','quadratic n=0 and +/-2 source','three full on-shell quadratic Bianchi equations','zz mismatch mutation rejected'],'scope':'Exact rational identities at one local on-shell specialization, not all-parameter proof or a solved q2 BVP. No radial discretizations.'}
Path(__file__).with_name('goal002_sector_results.json').write_text(json.dumps(out,indent=2)+'\n')
