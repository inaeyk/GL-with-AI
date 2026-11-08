"""Exact growth ratios and directed early commuted scalar-jet weights."""
from fractions import Fraction as Q
from math import comb
import sympy as sp
from flint import arb, ctx

DELTA=Q(3,25)
K0,K1,K2=522,380,220
Y=sp.Symbol("y")
p=Y*(1-Y)
dpol=[sp.Integer(1)]
for j in range(3):
    dpol.append(sp.expand(sp.diff(dpol[-1],Y)*p*p
                +(1-2*j*p)*sp.diff(p,Y)*dpol[-1]))


def bernstein():
    for j,k in enumerate((K0,K1,K2)):
        f=sp.Poly(dpol[j+1]-k*sp.Rational(3,25)*p*p*dpol[j],Y)
        n=f.degree()
        coeff=[sum(f.nth(jj)*sp.Rational(comb(i,jj),8**jj*comb(n,jj))
                   for jj in range(i+1)) for i in range(n+1)]
        assert min(coeff)>0
        print(f"B^({j+1}) >= {k} B^({j}), minimum Bernstein coefficient {min(coeff)}")


def weights():
    m,w=Q(157,100),Q(1,4)
    C=(1+Q(1,K0*K0))/(1-m/K1)
    ep=Q(1,K0*K1)+m*C/K1
    q=Q(2,K0)+w*C/K1
    pu=(1+Q(1,K0)+Q(6,25)*w+m*C/4)/(K1*K2)+m*(1+ep)/K2
    qv=(10*(1+Q(1,K0))+5*w+(w*C/4+w*q)/K1)/(K1*K2)
    values=[pu,w*C/(K1*K2),qv,(1+ep)/K2,q/(K1*K2),C/(K1*K2)]
    upper=[Q(s) for s in ("0.00720","0.00000301","0.000135","0.004565",
                         "0.000000054","0.00001202")]
    assert all(v<up for v,up in zip(values,upper))
    for name,v,up in zip(("Psi_uu-h''","Psi_uv","Psi_vv","Psi_u","Psi_v","Psi"),
                         values,upper):
        print(f"{name} / h'': {float(v):.13g}, use {float(up):.13g}")
    return upper


def bump_jets(y,omega):
    delta=arb(3)/25
    norm=arb("0.04679151546115").union(arb("0.04679151580708"))
    scale=delta.sqrt()/norm
    if y.upper()<=0:
        return [arb(0)]*4
    if y.lower()<=0:
        yp=y.upper()
        s=1/(yp*(1-yp))
        assert s>6 and yp<arb(1)/8
        # The polynomial coefficient l1 norms safely bound |dpol_j|.
        return [arb(0).union((scale/delta**j
                   *sum(abs(int(t)) for t in sp.Poly(q,Y).all_coeffs())
                   *s**(2*j)*(-s).exp()).upper()) for j,q in enumerate(dpol)]
    yi=y*(1-y)
    z=1/yi
    amp=scale*(-z).exp()
    out=[]
    for j,q in enumerate(dpol):
        value=arb(0)
        for a in sp.Poly(q,Y).all_coeffs():
            value=value*y+int(a)
        out.append(amp*value*z**(2*j)/delta**j)
    return out


def derivative_norm(cells=4096):
    ctx.prec=160
    om=arb("0.06328118").union(arb("0.06328120"))
    h=arb(1)/(8*cells)
    norm=arb(0)
    for i in range(cells):
        y=(i*h).union((i+1)*h)
        _,_,B2,B3=bump_jets(y,om)
        norm+=(B3+2*om*B2)**2*h*arb(3)/25
    print("||h''||_2:",norm.sqrt().lower().str(14),norm.sqrt().upper().str(14))
    return norm.sqrt()


if __name__=="__main__":
    bernstein()
    weights()
    derivative_norm()
