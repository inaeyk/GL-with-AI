"""Explicit source-evaluated n2 scalar-master upper bounds; not a metric M2.
Uses the audited physical source envelope, exact coefficient formulas and Arb.
"""
from pathlib import Path
import sympy as S
from flint import arb,ctx
import json,math,hashlib
ctx.prec=160
HERE=Path(__file__).resolve().parent
r,t,a,s,K=S.symbols('r t a s K',positive=True)
f=(r-1)/r;D=1+a*r**3
# Real coefficient magnitudes: replacing i by1 in j=-2Q/(iK)
# changes phase only, irrelevant to this absolute bound.
J=[-4*a*r*r*f/K,-4*a*r,2,4*a*r*r*f/(s*K),
   4*a*r*r*(a*r**3-2)*f/(s*D),4*a*r*r*(a*r**3-2)/(s*D),
   -4*a*(a*r**3*(2*r-3)-4*r+3)/(s*D*K)]
Fy=[-4*a*r*r/K,-4*a*r*r/(r-1),2*r/(r-1),4*a*r*r/(s*K),
    4*a*r*r*(a*r**3-2)/(s*D),-4*a*r*(a*r**3*(2*r-3)-4*r+3)/(s*(r-1)*D*K)]
Fc=[S.cancel(-f*f*r/D*v) for v in Fy]
av=(2*arb.pi()/5)**2;sv=2*arb('0.06328119','0.00000001');kv=av.sqrt()
env={a:av,s:sv,K:kv}
def ev(e,en):
    if e in en:return en[e]
    if e.is_Integer:return arb(int(e))
    if e.is_Rational:return arb(int(e.p))/int(e.q)
    if e.is_Add:return sum(ev(x,en) for x in e.args)
    if e.is_Mul:
        z=arb(1)
        for x in e.args:z*=ev(x,en)
        return z
    if e.is_Pow and e.exp.is_Integer:return ev(e.base,en)**int(e.exp)
    raise TypeError(e)
def rat(e,var,interval):
    n,d=S.fraction(S.cancel(e))
    def poly(z):
        out=arb(0)
        for c in S.Poly(z,var).all_coeffs():out=out*interval+ev(c,env)
        return out
    den=poly(d);assert not den.contains(0)
    return poly(n)/den
checks=[]
def test(name,z,hi):
    assert z<hi,name
    checks.append(dict(name=name,bound=str(z),strict_upper=str(hi)))
# Domain subdivision affects only certified coefficient suprema.
jmax=arb(0)
for expr in J:
    for n in range(3):
        ee=S.diff(expr,r,n)
        for m in range(48):
            val=rat(ee,r,(1+arb(m)/16).union(1+arb(m+1)/16))
            jmax=jmax.union(val.abs_upper()).abs_upper()
test('regular J coefficient derivatives0..2',jmax,arb('1e10'))
fmax=arb(0)
for expr in Fc:
    for n in range(2):
        ee=S.cancel((S.diff(expr,r,n)/r**6).subs(r,1/t))
        for m in range(16):
            val=rat(ee,t,(arb(m)/64).union(arb(m+1)/64))
            fmax=fmax.union(val.abs_upper()).abs_upper()
test('static F coefficients derivatives0..1 divided r6',fmax,arb('1e10'))
Q=arb('1e110');Cj=7*arb('1e10')*Q
# Every raw source derivative used here <= Q*r²*exp(-(r-4)) outside.
CF=arb('1e12')*Q
sigma=(sv*sv+av/4).sqrt()
test('exterior force weighted multiplier',(4+8/arb('0.35'))**8,arb('1e12'))
# Integral r^16 exp(-2(r-4)) dr from4∞, exact rational moments.
moment=sum(arb(math.comb(16,j))*4**(16-j)*math.factorial(j)/arb(2)**(j+1) for j in range(17))
tail2=arb('4/3')*CF*CF*moment
# For 1<r<4, |F| <=4 exp(4s) Cj delta^(s+1), dx<=4 ddelta/delta.
left2=64*(8*sv).exp()*Cj*Cj*arb(3)**(2*sv+2)/(2*sv+2)
F2=(tail2+left2).sqrt()
test('global L2 master forcing',F2,arb('1e128'))
U2=F2/(sv*sv)
Uinf=(2/(sv*sv)*(1/sv+arb('5/2')/(sv*sv))).sqrt()*F2
test('physical exact particular master L2',U2,arb('5e129'))
test('physical exact particular master sup',Uinf,arb('2e130'))
out=dict(scope='Certified very coarse exact separated +2 scalar master bounds; no all-component M2 or approximate-response error certificate',
    L2_source_upper='1e128',L2_u_upper='5e129',sup_u_upper='2e130',
    exact_source_envelope='each Q derivative<=1e110*r²*exp(-(r-4)) at r>=4; EF C3<=1e110 at1..4',
    coefficients=dict(J=[str(x) for x in J],F=[str(x) for x in Fc]),checks=checks,
    source_sha256={p:hashlib.sha256((HERE/p).read_bytes()).hexdigest() for p in ['goal005_master_bounds.py','goal005_source_bounds.py','goal005_source_bounds.json']})
(HERE/'goal005_master_bounds.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps(out,indent=2))
