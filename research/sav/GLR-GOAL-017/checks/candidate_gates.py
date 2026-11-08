"""Directed norms of a binary late waveform candidate, with physical error gates.

The affine candidate is fixed by its nodal values, not a continuum certificate.
"""
import argparse
import hashlib
from itertools import product
from pathlib import Path
from platform import python_version
import sys

import flint
from flint import acb, arb, ctx
import numpy as np

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "GLR-GOAL-013/checks"))
from d_template_norms import cross, rectangle_center, uncertainty

D = "8.407362"


def affine_inner(u, a, b):
    total = arb(0)
    for x0,x1,a0,a1,b0,b1 in zip(u[:-1],u[1:],a[:-1],a[1:],b[:-1],b[1:]):
        total += (x1-x0)*(2*a0*b0+a0*b1+a1*b0+2*a1*b1)/6
    return total


def affine_pole_inner(u, a, s, amp):
    total = acb(0)
    for x0, x1, y0, y1 in zip(u[:-1],u[1:],a[:-1],a[1:]):
        h = x1-x0
        z = h*s
        j0 = z.expm1()/z
        j1 = (z.exp()*(z-1)+1)/z**2
        total += h*(s*x0).exp()*(y0*j0+(y1-y0)*j1)
    return 2*(s*amp*total).real


def bounds(label, x):
    print(f"{label}: [{x.lower().str(16)}, {x.upper().str(16)}]")


def radius_box(mid, rad):
    return mid + (-rad).union(rad)


def subbox_rejection(u, dyad, f2, q, star, radius_q, radius_star, combined):
    # In each complex-amplitude cell, center the physical template at
    # fixed central frequencies. The difference of norms has Lipschitz
    # constant 3/2 in the template L2 metric. The analytically integrated
    # Gram and candidate-inner-product entries do not depend on the cell.
    exponents = [q, q, star, star] if combined else [q, q]
    amps = [acb(1), acb(0,1), acb(1), acb(0,1)][:len(exponents)]
    gram = [[cross(si,ai,sj,aj) for sj,aj in zip(exponents,amps)]
            for si,ai in zip(exponents,amps)]
    linear = [affine_pole_inner(u,dyad,s,a) for s,a in zip(exponents,amps)]
    edges = ((".08223",".18771"),("-.42418","-.31707"),
             ("-.21850","-.16857"),("-.44959","-.39966"))[:len(exponents)]
    n = 12 if combined else 64
    parts = []
    for lo,hi in edges:
        a,b = arb(lo),arb(hi)
        h = (b-a)/n
        parts.append([a+(j+arb(1)/2)*h for j in range(n)])
    widths = [(arb(b)-arb(a))/n/2 for a,b in edges]
    raq = abs(acb(widths[0],widths[1]))
    rastar = abs(acb(widths[2],widths[3])) if combined else arb(0)
    low = arb(100)
    for center in product(*parts):
        norm2 = sum(x*y*gram[i][j] for i,x in enumerate(center)
                    for j,y in enumerate(center))
        mismatch2 = f2+norm2-2*sum(x*g for x,g in zip(center,linear))
        correction = uncertainty(q,acb(*center[:2]),raq,radius_q)
        if combined:
            correction += uncertainty(star,acb(*center[2:]),rastar,radius_star)
        margin = (mismatch2.sqrt()-norm2.sqrt()/2-3*correction/2).lower()
        low = min(low,margin)
    return low


def calculate(filename, field, sharpen):
    raw = np.load(filename)
    assert raw["u"][0] == 0 and raw["u"][-1] == float(D)
    u = [arb(float(t)) for t in raw["u"][:-1]]+[arb(D)]
    cut = np.searchsorted(raw["u_full"],0.)
    sample = raw[field][cut:]
    dyad = [arb(float(t)) for t in sample]
    base = [arb(float(t)) for t in raw["baseline"]]
    assert len(dyad)==len(base)==len(u)
    digest = hashlib.sha256(np.stack((raw["u"],sample,raw["baseline"]),
                                      axis=-1).astype("<f8").tobytes()).hexdigest()
    print(f"candidate digest (u, {field}, baseline) little-endian f64: {digest}")
    q = acb("-.177924631", "-.747343369")
    star = q + 2*arb(".06328119")
    radius_q = arb(2).sqrt()*arb("5e-8")
    radius_star = radius_q + arb("2e-8")
    aq, raq = rectangle_center(".08223",".18771","-.42418","-.31707")
    aa, raa = rectangle_center("-.21850","-.16857","-.44959","-.39966")
    q2 = cross(q,aq,q,aq)
    star2 = cross(star,aa,star,aa)
    q_star = cross(q,aq,star,aa)
    d_q = uncertainty(q,aq,raq,radius_q)
    d_star = uncertainty(star,aa,raa,radius_star)
    f2, b2 = affine_inner(u,dyad,dyad), affine_inner(u,base,base)
    q_hit, s_hit = affine_pole_inner(u,dyad,q,aq), affine_pole_inner(u,dyad,star,aa)
    bounds("candidate leading norm",b2.sqrt())
    bounds("candidate returning norm",f2.sqrt())
    bounds("candidate return / leading", (f2/b2).sqrt())
    bounds("candidate signed leading-return overlap", affine_inner(u,base,dyad))
    for label,p2,hit,unc in (("Q",q2,q_hit,d_q),
        ("Q+S*",q2+star2+2*q_star,q_hit+s_hit,d_q+d_star)):
        pnorm = radius_box(p2.sqrt(),unc)
        dmid = (f2+p2-2*hit).sqrt()
        bounds(f"{label} candidate mismatch to midpoint",dmid)
        bounds(f"{label} candidate ratio to midpoint",dmid/p2.sqrt())
        bounds(f"{label} inherited physical norm",pnorm)
        cap = (dmid.lower()-unc-pnorm.upper()/2).lower()
        bounds(f"{label} sufficient physical E21 ceiling for rejection", cap)
    bounds("sufficient physical E21 ceiling for both via reverse triangle",
           ((q2.sqrt()-d_q)/2-f2.sqrt()).lower())
    if sharpen:
        for name, combined in (("Q",False),("Q+S*",True)):
            low = subbox_rejection(u,dyad,f2,q,star,radius_q,radius_star,combined)
            bounds(f"{name} subbox direct rejection threshold",low)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("data",type=Path)
    p.add_argument("--field",choices=("p_shear_infty","p_infty"),default="p_shear_infty")
    p.add_argument("--sharpen",action="store_true")
    p.add_argument("--bits",type=int,default=160)
    args = p.parse_args()
    ctx.prec = args.bits
    print(f"Python {python_version()}, python-flint {flint.__version__}, Arb {ctx.prec} bits")
    calculate(args.data,args.field,args.sharpen)


if __name__=="__main__":
    main()
