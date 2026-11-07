"""Fit-free physical horizon contribution with conditional local ball tails."""
import argparse
import platform

import flint
import numpy as np
from flint import acb, arb, ctx
from scipy.integrate import quad_vec

from horizon_series import LocalHorizon,box,upper
from overlap_gl import tortoise
from overlap_mixed import MixedSolution
from returning_overlap import Experiment


def midpoint(value):
    return complex(float(value.real.mid()),float(value.imag.mid()))


def run(cap,length,tol,angle,degree,certified_lower=False):
    e=Experiment(13/8,.12,length,1e-6,tol,angle)
    old,*_=e.evaluate(cap,6)
    omega=e.gl.omega
    sigma_amplitude=-e.baseline.full[1]/e.baseline.jost.w
    model=LocalHorizon(acb(e.s),acb(arb.pi()/5),acb(omega),acb(e.gl.scale),
                       acb(sigma_amplitude),degree)
    print("s =",e.s,"GL horizon scale =",e.gl.scale,
          "baseline horizon amplitude =",sigma_amplitude,flush=True)
    print("mixed Taylor contractions", [float(v[0]) for v in model.diagnostics],flush=True)
    print("mixed l1 tail at radius .043", [float(v[1]) for v in model.diagnostics],flush=True)
    initial_widths=[]

    def initial(r):
        columns=model.initial(r)
        for column in columns:
            initial_widths.extend(float(value.rad().upper()) for value in column)
        return np.asarray([[midpoint(value) for value in column]
                           for column in columns]).T.reshape(2,2,3)

    e.mixed=MixedSolution(e.mixed_source,e.s-omega,e.gl.k,e.rmatch,e.ray,length,
                          cap/2,tol,regular_initial=initial)
    amp=[acb(z) for z in e.mixed._left_amplitudes]
    state=model.matched(amp)
    mean_at_cap=np.asarray(e.mean(1+cap))* np.exp(-2*omega*tortoise(1+cap))
    mean=model.mean(arb(cap),[acb(v) for v in mean_at_cap])
    local,tails=model.contribution(arb(cap),state,mean)
    print("ingoing amplitudes",[midpoint(z) for z in amp],flush=True)
    print("mean EF cap amplitudes",mean_at_cap,"max initial ball width",max(initial_widths),flush=True)
    print("conditional local contribution with split boundary",local,flush=True)
    print("conditional local Cauchy tails",[float(v) for v in tails],flush=True)
    opts=dict(epsabs=tol**.8,epsrel=tol**.8,limit=400)
    real,err_real=quad_vec(e.weak,1+cap,e.rmatch,
                          points=[e.baseline.inner,e.baseline.radius],**opts)
    right,err_right=quad_vec(lambda d:e.ray*e.weak(e.rmatch+e.ray*d,d),0,length,**opts)
    value=np.array([midpoint(z) for z in local])+real+right
    print("new numerator parts",value,"external quadrature estimates",err_real,err_right,flush=True)
    print("old residue",sum(old)/e.wprime,"fit-free residue",sum(value)/e.wprime,flush=True)
    print("difference",sum(value-old)/e.wprime,"mixed match error",e.mixed.match_error,flush=True)
    if certified_lower:
        from certified_launch import inputs
        data=inputs()
        certified=LocalHorizon(data.s,data.k,data.omega,acb(1),acb(1),degree)
        amp_box=[z+box(arb(".0002")) for z in amp[:2]]
        mean_box=[acb(z)+box(arb(".002")) for z in mean_at_cap]
        enclosure=certified.factorized_contribution(arb(cap),data.gl_scale,data.launch_left,
                                                    amp_box,mean_box)
        assert all(v.overlaps(w) for v,w in zip(enclosure,local))
        print("validated GL scale =",data.gl_scale,"validated launch_left =",data.launch_left)
        print("conditional mixed box half-width .0002, EF mean box half-width .002")
        print("conditional contribution with physical lower-input intervals",enclosure)

    return sum(value)/e.wprime


if __name__=="__main__":
    p=argparse.ArgumentParser()
    p.add_argument("--cap",type=float,default=1/128)
    p.add_argument("--length",type=float,default=64)
    p.add_argument("--tol",type=float,default=2e-11)
    p.add_argument("--angle",type=float,default=np.pi/4)
    p.add_argument("--degree",type=int,default=34)
    p.add_argument("--certified-lower",action="store_true")
    args=p.parse_args()
    ctx.prec=128
    ctx.cap=args.degree+24
    print("Python",platform.python_version(),"python-flint",flint.__version__,args,flush=True)
    run(args.cap,args.length,args.tol,args.angle,args.degree,args.certified_lower)
