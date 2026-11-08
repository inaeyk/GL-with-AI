"""Point-stencil and log-primitive refinement of the normalized RW W'."""
from math import sqrt

import numpy as np
from flint import acb,arb,ctx
from scipy.integrate import solve_ivp

from certified_launch import spec
from exterior_profiles import Cell,horizon_rw_match,x
from horizon_series import box,upper


def derivative():
    center=spec.CENTER
    radius=arb(".005")
    root_radius=arb("5e-8")
    step=arb(".0001")
    assert 2*step<radius
    cauchy_bound=upper(spec.rw_determinant(spec.shift(center,radius)))
    p=spec.rw_determinant(center+step)-spec.rw_determinant(center-step)
    q=spec.rw_determinant(center+2*step)-spec.rw_determinant(center-2*step)
    ratio=step/radius
    defect=cauchy_bound/radius*(4*ratio**4/(3*(1-ratio**2))
                              +(2*ratio)**4/(3*(1-4*ratio**2)))
    defect+=cauchy_bound/radius*((1-root_radius/radius)**-2-1)
    return (8*p-q)/(12*step)+box(defect.upper())


def tail_amplitude(s):
    """Volterra contraction about 1+3/(sr) for the outgoing RW amplitude."""
    R=arb(spec.OUTER)
    r=spec.R_MATCH+spec.W*R
    a=3/s
    f=1-1/r
    dx=1+1/(R-1)
    attenuation=((s*spec.W).real-upper(s)/(R-1)).lower()
    assert attenuation>0
    K=dx/(2*attenuation)
    v=(1+1/R)*(6+3/R)
    residual=(1+1/R)*upper(3-4*a)
    B=(dx*K*residual).upper()
    assert dx*K*(residual+v*B/R)/2 < B and dx*K*v/(2*R)<1
    w=1+a/r+box(B/R**2)
    wx=-f*a/r**2+box(K*(residual+v*B/R)/R**3)
    assert w.abs_lower()>0
    return w,wx/w,B/R**2


def outgoing_at_match(s):
    wfar,initial,tail=tail_amplitude(s)
    sm=complex(float(s.real.mid()),float(s.imag.mid()))
    pm=complex(float(initial.real.mid()),float(initial.imag.mid()))
    ray=(1+1j)/sqrt(2)
    grid=spec.OUTER*np.linspace(1,0,spec.CELLS+1)**3

    def rhs(q,p):
        r=2.5+ray*q
        f=1-1/r
        potential=f*(6/r**2-3/r**3)
        return ray/f*(potential+2*sm*p-p*p)

    sol=solve_ivp(rhs,(spec.OUTER,0),[pm],t_eval=grid,method="DOP853",
                  rtol=4e-14,atol=1e-15)
    assert sol.success
    samples=[(float(q),p,rhs(q,p)/ray) for q,p in zip(grid,sol.y.T)]

    def local(r,t):
        return r**4,[t*(6*r-3)],-s*r**4,t*r**3

    captured=[]
    spec.directed_flow(samples,1,True,-s,acb(0),local,upper(initial-acb(pm)),arb(0),
                       False,captured,initial_is_error=True)
    log=acb(0)
    for q in captured:
        primitive,error=Cell(*q).log_primitive()
        log+=primitive(1)-primitive(0)+box(error)
    return wfar*(-s*x(acb(spec.R_MATCH))+log).exp(),tail


def wronskian_derivative():
    s=spec.shift(spec.CENTER,arb("5e-8"))
    p=derivative()
    h=horizon_rw_match(s)
    g,tail=outgoing_at_match(s)
    return -p*h*g,p,h,g,tail


if __name__=="__main__":
    ctx.prec=128
    ctx.cap=80
    W,p,h,g,tail=wronskian_derivative()
    print("D' on the root disk",p)
    print("normalized h_m",h,"g_m",g,"far amplitude error",tail)
    print("normalized W' at the pole",W,"modulus",abs(W))
    assert W.abs_lower()>0
