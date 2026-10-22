#!/usr/bin/env python3
"""Exact rational-field verification of the complete SO(3) linear mode.

Uses the 3D orbit Ricci tensor and separate sphere equation, independently
of the candidate master equation's derivation. kappa=i*k is kept formal,
so k_squared=-kappa_squared; coefficients are exact rational functions.
"""
from sympy import QQ
from sympy.polys.fields import field

F,r,q,o=field('r,kappa,Omega',QQ)
zero=F.zero;one=F.one;k2=-q*q;f=(r-1)/r;D=1+k2*r**3
P=1/(r-1)-5/r+6/(r*D)
Q=3/(r*r*(r-1))-k2*r/(r-1)-3/(r*r*(r-1)*D)
B0=-Q+o*o/(f*f);B1=-P

class Lin:
    def __init__(self,a=zero,b=zero):self.a,self.b=a,b
    def __add__(self,v):
        if v==0:return self
        return Lin(self.a+v.a,self.b+v.b)
    __radd__=__add__
    def __neg__(self):return Lin(-self.a,-self.b)
    def __sub__(self,v):return self+-v
    def __mul__(self,v):return Lin(self.a*v,self.b*v)
    __rmul__=__mul__
    def __truediv__(self,v):return self*(1/v)
    def dr(self):return Lin(self.a.diff(r)+self.b*B0,self.a+self.b.diff(r)+self.b*B1)
    def iszero(self):return self.a==0 and self.b==0
    def __repr__(self):return str((self.a,self.b))

Z=Lin();Y=Lin(one,zero);Yp=Lin(zero,one)

def dm(v,i):return v*o if i==0 else v.dr() if i==1 else v*q
def db(v,i):return v.diff(r) if i==1 else zero

def check():
    a=(r-1)*(F(1)/2-k2*r**3)/(r*D**2)-o*o/(k2*D)
    b=(r-1)/(2*k2*r**3*D)-3*(r-1)**2/D**2
    ht=Y*a+Yp*b
    hs=Y*(-r*r/(2*D))+Yp*(-(r-1)/(k2*D))
    hv=(Yp-Y/(2*r*(r-1)))*(-o/(k2*D))
    hr=ht/(f*f)-hs*(2/(r*(r-1)))
    # Independent gauge map from area-preserving spherical gauge.
    Hvv=(Yp-Y/(2*r*(r-1)))*(r**3/(2*D))
    Hrr=Hvv/(r*(r-1))
    Htt=ht+Y*(o*o/k2)+Hvv*(f*f.diff(r))-Yp*(f*f.diff(r)/(2*k2))
    xi=[Y*(-o/(2*k2)),Hvv-Yp/(2*k2),Y*(-1/(2*q))]
    assert (Y+xi[2]*(2*q)).iszero()
    assert (-Hvv*q+xi[1]*q+xi[2].dr()).iszero()
    assert (xi[0]*q+xi[2]*o).iszero()
    assert (Htt+xi[0]*(2*o)-xi[1]*(f*f.diff(r))-ht).iszero()
    assert (Hvv*o+xi[1]*o+xi[0].dr()-xi[0]*(f.diff(r)/f)-hv).iszero()
    assert (Hrr+xi[1].dr()*2+xi[1]*(f.diff(r)/f)-hr).iszero()
    assert (xi[1]*(2*(r-1))-hs).iszero()
    print('SPHERICAL_TO_UNITARY_GAUGE_MAP: PASS',flush=True)
    assert (-ht/f+hr*f+hs*(2/(r*r))).iszero()
    print('UNITARY_TT_TRACE: PASS',flush=True)
    assert (-ht*(o/f)+hv.dr()*f+hv*(f.diff(r)+2*f/r)).iszero()
    assert (-hv*(o/f)+hr.dr()*f+hr*(3*f.diff(r)/2+2*f/r)+ht*(f.diff(r)/(2*f*f))-hs*(2/r**3)).iszero()
    print('UNITARY_TT_DIVERGENCE: PASS',flush=True)
    g=[[-f,zero,zero],[zero,1/f,zero],[zero,zero,one]]
    gi=[[-1/f,zero,zero],[zero,f,zero],[zero,zero,one]]
    h=[[ht,hv,Z],[hv,hr,Z],[Z,Z,Z]]
    hi=[[-sum(h[e][d]*(gi[c][e]*gi[d][a]) for e in range(3) for d in range(3)) for a in range(3)] for c in range(3)]
    ga={}
    for c in range(3):
        for a in range(3):
            for b in range(3):
                val=sum(gi[c][d]*(db(g[d][b],a)+db(g[d][a],b)-db(g[a][b],d))/2 for d in range(3))
                if val!=0:ga[c,a,b]=val
    def G(c,a,b):return ga.get((c,a,b),zero)
    cc={}
    for c in range(3):
        for a in range(3):
            for b in range(3):
                val=sum((dm(h[d][b],a)+dm(h[d][a],b)-dm(h[a][b],d))*(gi[c][d]/2)+hi[c][d]*((db(g[d][b],a)+db(g[d][a],b)-db(g[a][b],d))/2) for d in range(3))
                if not val.iszero():cc[c,a,b]=val
    def C(c,a,b):return cc.get((c,a,b),Z)
    dR=hs/(2*r)
    dhess={}
    for a in range(3):
        for b in range(3):
            dhess[a,b]=dm(dm(dR,b),a)-sum(dm(dR,c)*G(c,a,b) for c in range(3))-C(1,a,b)
    for a in range(3):
        for b in range(a,3):
            val=sum(dm(C(c,a,b),c)-dm(C(c,a,c),b) for c in range(3))
            val+=sum(C(c,c,d)*G(d,a,b)+C(d,a,b)*G(c,c,d)-C(c,b,d)*G(d,a,c)-C(d,a,c)*G(c,b,d) for c in range(3) for d in range(3))
            # Ric5_ab=Ric3_ab-2 R^-1 Hess3_ab(R).
            val+=dhess[a,b]*(-2/r)-dR*(2*G(1,a,b)/(r*r))
            assert val.iszero(),(a,b,val)
            print('ORBIT_LINEAR_RICCI_'+str(a)+str(b)+': PASS',flush=True)
    boxr=-sum(gi[a][b]*G(1,a,b) for a in range(3) for b in range(3))
    dbox=sum(hi[a][b]*(-G(1,a,b))+dhess[a,b]*gi[a][b] for a in range(3) for b in range(3))
    dnorm=hi[1][1]+dR.dr()*(2*f)
    # Ric5_sphere = [1-(grad R)^2-R BoxR] gamma_sphere.
    sphere=-dnorm-dR*boxr-dbox*r
    assert sphere.iszero(),sphere
    print('SPHERE_LINEAR_RICCI: PASS',flush=True)
    # Sign mutation rejected by actual transverse equation.
    bad=-ht*(o/f)-hv.dr()*f-hv*(f.diff(r)+2*f/r)
    assert not bad.iszero()
    print('TR_SIGN_MUTATION_REJECTED: PASS',flush=True)
    # Master-to-Schrodinger map checked by rational coefficients.
    w=D/r
    V=k2*f+(6*r-11)*(r-1)/r**4+18*(r-1)**2/(r**4*D**2)-6*(4*r-5)*(r-1)/(r**4*D)
    assert (2*w.diff(r)+P*w)-w*f.diff(r)/f==0
    assert w.diff(r).diff(r)+P*w.diff(r)+(Q-o*o/(f*f))*w+w*(V+o*o)/(f*f)==0
    print('SCHRODINGER_MAP: PASS',flush=True)
    # Regular-horizon Lorentz contraction: leading cancellations are exact.
    v1=k2-5+6/(1+k2)
    v2=-k2+26-48/(1+k2)+(18-18*k2)/(1+k2)**2
    p1=v1/(1+2*o);p2=(v2-p1*p1+p1)/(2+2*o)
    ph=o+p1*(r-1)+p2*(r-1)**2
    yyprime=w.diff(r)/w+ph/f
    hT=ht.a+ht.b*yyprime
    hS=(hs.a+hs.b*yyprime)/(r*r)
    hV=hv.a+hv.b*yyprime
    amin=(hT/f-hS-hV)/(r-1)
    aplus=(hT/f-hS+hV)*(r-1)
    actual=(2*amin*aplus+4*hS*hS).evaluate(r,1)
    expected=(32*o**4+32*o**3*k2+20*o**3+6*o*o*k2*k2+16*o*o*k2+6*o*o+5*o*k2*k2+8*o*k2+2*k2*k2)/(2*k2*k2*(o+1)*(2*o+1)*(1+k2)**2)
    assert actual==expected.evaluate(r,1)
    print('HORIZON_CONTRACTION_CANCELLATIONS: PASS',flush=True)

if __name__=='__main__':check()
