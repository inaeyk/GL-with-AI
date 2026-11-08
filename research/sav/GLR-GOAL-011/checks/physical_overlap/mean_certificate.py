"""Enclose the physical EF mean quadrature from the ray to a horizon cap."""
from math import factorial

from flint import acb,arb,ctx

from certified_launch import inputs
from exterior_profiles import FAR,MATCH,x,CompactGL
from exterior_source import Exterior
from horizon_series import box,gl_horizon,poly_at,upper
from mean_series import ricci_from_jet
from outer_gljet import njet
from enclosure_math import centered


def mean_rhs(r,k,omega,fields):
    rr,angular,zz=ricci_from_jet(r,omega,k,*fields)
    tv=(1-1/r)*rr/2+angular/r**2+zz/2
    return -r*r*tv/2,-r*rr/2


def normalized_mean(r,k,omega,p):
    f=1-1/r
    return mean_rhs(r,k,omega,njet(r,k,omega,1,(p-omega)/f))


def factored_mean(r,k,omega,n,p):
    return [n*n*z for z in centered(normalized_mean,(r,k,omega,p))]


def mean_tail(e):
    """Cauchy bounds for the GL EF jets and a polynomial-weighted tail."""
    R=arb(FAR)
    data=e.data
    k=data.k.real
    omega=data.omega.real
    fmin=1-1/(R-1)
    fmax=1+1/(R-1)
    small=R-1
    dmin=k*k-1/small**3
    dmax=k*k+1/small**3
    assert dmin>0
    # On the unit r disk about a tail point the regular n system is bounded.
    q=(k*k+(6+11/small)/small**2
       +18*fmax/(dmin**2*small**8)
       +6*(4+5/small)*fmax/(dmin*small**4))/fmin
    b=(1/small**2+2*omega)/fmin
    assert q+b<1
    assert (e.tail.gl.p_cap+omega)/e.tail.f_floor<1
    n_growth=arb(1).exp()
    Y=dmax*n_growth
    Yr=(2*k*k/small+1/small**4+dmax)*n_growth
    vr=(3*omega/(dmin*dmin*small)+(k*k+1/(2*small**3))/dmin**2)*Y
    vr+=(1/(2*k*k*dmin*small**2)+3*fmax/(dmin**2*small)
          +omega/(k*k*dmin))*Yr
    ftr=omega*(fmax*Yr+(omega+1/(2*small**2))*Y)/(k*k*dmin)
    sphere=Y/(2*dmin)+(fmax*Yr+omega*Y)/(k*k*dmin*small)
    rr=2*(vr+sphere)/fmin
    # vv, vr, rr multiply |n|/|r'|; sphere multiplies |r'||n|.
    disk_factor=max(z.upper() for z in (ftr+fmax*vr,vr,rr,sphere))
    assert 2*disk_factor*(1+1/R)<64
    # |h|, |h_r|, |h_rr| <= 64 |r| |n| by unit-disk Cauchy.
    # Componentwise majorization of the regular EF Ricci expressions
    # gives |T^v_v|,|T_rr| <=128(64 |r| |n|)^2.
    r64=MATCH+e.tail.ray*R
    n64=upper(e.gl.far()[0]*(-omega*x(r64)).exp())
    rate=e.tail.gl.decay+omega*(e.tail.ray.real-1/(R-1))
    cap=R+MATCH
    decay=2*rate-4/cap
    assert decay>0
    return (64**3*cap**4*n64**2/decay).upper()


def integrate_outer(e,subdivisions=2):
    tail=mean_tail(e)
    result=[box(tail),box(tail)]
    data=e.data
    for i,cell in enumerate(e.gl.cells):
        for j in range(subdivisions):
            t=acb((j+arb(".5",".5"))/subdivisions)
            r=cell.radius(t)
            u,p=e.gl.evaluate(i,t)
            n=u*(-data.omega*x(r)).exp()
            d=factored_mean(r,data.k,data.omega,n,p)
            result=[v+cell.step*q/subdivisions for v,q in zip(result,d)]
    return result,tail


def derivative_value(value,t,order):
    p=value.series
    for _ in range(order):
        p=p.derivative()
    inner=upper(t)
    outer=(inner+value.radius)/2
    error=value.tail(outer)*factorial(order)/(outer-inner)**order
    return poly_at(p,t)+box(error.upper())


def integrate_inner(data,cap=arb(1)/128,near_steps=256,subdivisions=2):
    k,omega,scale=data.k,data.omega,data.gl_scale
    cut=arb(1)/16
    assert 0<cap<cut
    local=gl_horizon(k,omega,scale,60)
    result=[acb(0),acb(0)]
    step=(cut-cap)/near_steps
    for j in range(near_steps):
        t=acb(cap+(j+arb(".5",".5"))*step)
        fields=[[derivative_value(q,t,order) for q in local] for order in range(3)]
        result=[v+step*q for v,q in zip(result,mean_rhs(1+t,k,omega,fields))]

    profile=CompactGL(data)
    for i,cell in enumerate(profile.cells):
        for j in range(subdivisions):
            t=acb((j+arb(".5",".5"))/subdivisions)
            r=cell.radius(t)
            n,p=profile.evaluate(i,t)
            derivative=factored_mean(r,k,omega,n,p)
            result=[v+cell.step*q/subdivisions for v,q in zip(result,derivative)]
    return result


if __name__=="__main__":
    ctx.prec=128
    ctx.cap=96
    data=inputs()
    e=Exterior(data)
    mean_match,tail=integrate_outer(e)
    inner=integrate_inner(data)
    mean_cap=[m-d for m,d in zip(mean_match,inner)]
    print("physical mean tail each component upper",tail)
    print("EF mean at 2.5",mean_match)
    print("EF mean at r=1+1/128",mean_cap)
    print("imaginary part zero by analytic deformation; interval real parts",
          [z.real for z in mean_cap])
    print("real cap endpoints",[(z.real.lower(),z.real.upper()) for z in mean_cap])
