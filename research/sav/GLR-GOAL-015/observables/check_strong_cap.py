"""Directed readout for the exact weak-current candidate and manufactured tests.

The lower physical mismatch is outside this calculation. Use the GOAL-014
192x720 cap=400 archive and GOAL-015 candidate definition. No evolution.
"""
import argparse
import hashlib
from pathlib import Path
import sys

from flint import arb, ctx
import numpy as np
import sympy as sp

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "checks"))
from weak_candidate import array, graph, J


def manufactured():
    u, v = sp.symbols("u v", real=True)
    z = sp.symbols("z", real=True)
    b = 1-u/3
    c = sp.Rational(3)
    V = 1+v
    w = u**2*(v-b)*(1+u*v)
    F = u**2*(1+v+v**2)
    S = 4*sp.diff(w,u,v) - sp.diff(F,u) + V*w
    # Choose H independently nonzero and solve for G so the exact wave
    # equation and induced (nonzero) upper graph velocity both hold.
    H = u**2*(1+v)
    Gg = 4*sp.diff(w,u).subs(v,b) + sp.diff(b,u)*F.subs(v,b)
    G = Gg + sp.integrate((S+H).subs(v,z),(z,b,v))
    assert sp.expand(4*sp.diff(w,u,v)-sp.diff(F,u)-sp.diff(G,v)+H+V*w)==0
    graph_term = (1-sp.diff(b,u))*F.subs(v,b)
    cap_term = (G-F).subs(v,c)
    ray = sp.integrate((sp.diff(F,u)+sp.diff(F,v)-H-V*w).subs(v,z),(z,b,c))
    direct = 4*sp.diff(w,u).subs(v,c)
    assert sp.expand(direct-cap_term-graph_term-ray)==0
    assert w.subs(v,b)==0 and w.subs(u,0)==0
    for name, omitted in (("graph",graph_term),("cap",cap_term),
                          ("Vw",-sp.integrate((V*w).subs(v,z),(z,b,c)))):
        print(f"manufactured omission {name}, error in 4*news at u=1: {omitted.subs(u,1)}")
        assert omitted.subs(u,1)!=0
    print("exact manufactured cap identity: verified")


def check(path, subcells, bits):
    ctx.prec = bits
    data = array(path)
    with np.load(path) as file:
        candidate = np.asarray(file["p_cap"],dtype="<f8")
    digest=hashlib.sha256(candidate.tobytes()).hexdigest()
    assert digest=="38ecd9ed4d942aeb261cc197bf5662e9d80a584fa6dae955a322a61817915c39",digest
    assert np.isfinite(candidate).all()
    n=192
    D=arb(3)/200
    R=arb(13)/8
    a=-R-2*(R-1).log()
    d=a+D
    rc=arb(400)
    c=d+2*(rc+(rc-1).log())
    v=[R,*(arb(float(x)) for x in data["v"][n+1:-1]),c]
    assert all(r>l for l,r in zip(v,v[1:]))
    dv=[r-l for l,r in zip(v,v[1:])]
    bd=graph(d)
    M=(D/4).exp()*(J(bd)-J(rc))
    kap=2*D/arb.pi()
    m=arb(0)
    max_ray_F=arb(0)
    max_ray_H=arb(0)
    max_G=arb(float(np.max(np.abs(data["G"]))))
    rows=[]
    for i in range(n+1):
        row=[]
        for key in "FH":
            q=[arb(float(x)) for x in data[key][i,n:]]
            signed=sum(h*(l+r)/2 for h,l,r in zip(dv,q,q[1:]))
            absray=sum(h*(abs(l)+abs(r))/2 for h,l,r in zip(dv,q,q[1:]))
            qg=arb(float(data[key][i,n-i]))
            row += [signed,qg+q[0]]
            # A single global wedge bound is appended separately below.
            if key=="F":
                max_ray_F=max(max_ray_F,absray.upper())
            else:
                max_ray_H=max(max_ray_H,absray.upper())
        row += [arb(float(data["G"][i,-1])),arb(float(candidate[i]))]
        rows.append(row)
    for key in "FH":
        wmax=arb(float(np.max(np.abs(data[key]))))*(R-bd)
        if key=="F":
            max_ray_F += wmax
        else:
            max_ray_H += wmax
    h=D/n
    for i,(lo,hi) in enumerate(zip(rows,rows[1:])):
        slope=(hi[0]-lo[0])/h
        graph_slope=(hi[1]-lo[1])/h
        # The signed rectangular F primitive is affine in u; only its
        # wedge has a moving boundary. Convex form avoids inflated interval
        # interpolation for two equal-sign ordinates.
        for j in range(subcells):
            t=(arb(j)/subcells).union(arb(j+1)/subcells)
            b=graph(a+h*(i+t))
            bp=-(b-1)/(b+1)
            vals=[l+(r-l)*t for l,r in zip(lo,hi)]
            Fw_u=(-bp*vals[1]+(R-b)*graph_slope)/2
            Hr=vals[2]+(R-b)*vals[3]/2
            residual=(vals[4]+slope+Fw_u-Hr)/4-vals[5]
            m += (h/subcells)*residual.abs_upper()**2
    residual=m.sqrt()
    # Volterra solution of this candidate is exact by definition.
    # Sup ray integrals bound the primitive norms on all slices.
    wbound=(kap*M).exp()*D.sqrt()*(max_ray_F+kap*(max_G+max_ray_H))/4
    read=residual+M*wbound
    print(f"affine cap digest = {digest}")
    for name,val in (("potential ray mass upper",M),
                     ("Volterra norm exponent factor", (kap*M).exp()),
                     ("graph factor",arb(26)/21),
                     ("candidate sup absolute F-ray bound",max_ray_F),
                     ("candidate sup absolute H-ray bound",max_ray_H),
                     ("candidate sup absolute G",max_G),
                     ("candidate cap mismatch without Vw",residual),
                     ("candidate potential readout bound",M*wbound),
                     ("E_read upper bound",read),
                     ("E_F multiplier",M*(kap*M).exp()/4),
                     ("E_G multiplier",M*(kap*M).exp()*kap/4),
                     ("E_H multiplier", (1+M*(kap*M).exp()*kap)/4),
                     ("E_vol multiplier",M*(kap*M).exp())):
        print(f"{name}: {val.upper().str(16)}")
    assert read < 25
    print("Physical lower-current mismatch is not included.")


if __name__=="__main__":
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument("candidate",nargs="?")
    p.add_argument("--subcells",type=int,default=12)
    p.add_argument("--bits",type=int,default=160)
    args=p.parse_args()
    manufactured()
    if args.candidate:
        check(args.candidate,args.subcells,args.bits)
