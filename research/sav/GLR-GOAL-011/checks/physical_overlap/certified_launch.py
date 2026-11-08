"""Validate the horizon GL normalization and the compact baseline launch moment."""
from dataclasses import dataclass
from importlib.util import module_from_spec, spec_from_file_location
from pathlib import Path

from flint import acb,acb_mat,acb_series,arb,ctx

from horizon_series import (AnalyticSeries,box,coefficients,gl_regular,
                            norm_matrix,norm_vector,rw_horizon,upper)
certificate_path=Path(__file__).resolve().parents[1]/"spectral_certificate.py"
certificate_spec=spec_from_file_location("goal011_spectral_certificate",certificate_path)
spec=module_from_spec(certificate_spec)
certificate_spec.loader.exec_module(spec)


def advance(operator,center,step,initial,degree=24):
    """Taylor flow on a disk of radius three times the real step."""
    radius=3*abs(step)
    count=len(initial)
    t=acb_series([center,1],prec=degree)
    entries=operator(t)
    coeff=[[coefficients(entries[i][j],degree) for j in range(count)] for i in range(count)]
    matrices=[acb_mat([[coeff[i][j][n] for j in range(count)] for i in range(count)])
              for n in range(degree)]
    state=[acb_mat([[v] for v in initial])]
    for n in range(degree):
        rhs=acb_mat(count,1)
        for j in range(n+1):
            rhs+=matrices[j]*state[n-j]
        state.append(rhs/(n+1))
    box_operator=acb_mat(operator(acb(center)+box(radius)))
    norm=(norm_vector(state[0])*(radius*norm_matrix(box_operator)).exp()).upper()
    return [AnalyticSeries(acb_series([v[i,0] for v in state],prec=degree+1),radius,norm)
            for i in range(count)]


def transport(operator,left,right,state,steps):
    step=(right-left)/steps
    for j in range(steps):
        local=advance(operator,left+j*step,step,state)
        state=[v.value(acb(step)) for v in local]
    return state


def gl_scale(k,omega):
    left=arb("1.125")
    right=arb(2)
    start=gl_regular(k,omega,acb(1),40)
    initial=[v.value(acb(left-1)) for v in start]

    def operator(r):
        t=r-1
        d=1+k*k*r**3
        c=k*k*r**3+6*r-11+18*t/d**2-6*(4*r-5)/d
        return [[0,1],[c/(r**2*t),-(1/r+2*omega*r)/t]]

    n,dn=transport(operator,left,right,initial,64)
    t=right-1
    a=k*k
    d=1+a*right**3
    y=d*n/right
    yp=(2*a*right-1/right**2)*n+d*dn/right
    angular=-right**2*y/(2*d)-t*yp/(a*d)-omega*right*y/(a*d)
    scale=8/(angular*(omega*right).exp())
    match=transport(operator,right,arb("2.5"),[scale*n,scale*dn],32)
    return scale,match


def massless(s):
    def operator(r):
        f=(r-1)/r
        potential=f*(6/r**2-3/r**3)
        return [[0,1/f],[(s*s+potential)/f,0]]
    return operator


def normalized_bump():
    # Symmetric exact derivative-square integral. The omitted flat tail
    # at y<=1/128 is enclosed by monotonicity of y^-4 exp(-2/y).
    a=arb(1)/128
    length=(arb(1)/2-a)/8192
    total=arb(0)
    for j in range(8192):
        y=a+(j+arb(".5",".5"))*length
        f=y*(1-y)
        derivative=(1-2*y)*(4-1/f).exp()/f**2
        total+=derivative**2*length
    error=(a**-3*(8-2/a).exp()/(1-a)**4).upper()
    total+=arb(0).union(error)
    return 1/(2*total).sqrt()


def bump(y):
    low,high=y.lower(),y.upper()
    if high<=0 or low>=1:
        return arb(0)
    if 0<low and high<1:
        return (4-1/(y*(1-y))).exp()
    lo,hi=max(arb(0),low),min(arb(1),high)
    maximizer=min(max(lo,arb(1)/2),hi)
    m=maximizer*(1-maximizer)
    return arb(0).union((4-1/m).exp().upper())


def baseline_horizon(sigma,b_norm):
    op=massless(sigma)
    match=arb("2.5")
    left=arb("1.125")
    radius=arb(13)/8
    n,nr=[v.value(acb(left-1)) for v in rw_horizon(sigma,40)]
    x=left+(left-1).log()
    e=(sigma*x).exp()
    h_launch=transport(op,left,radius,[e*n,e*(sigma*n+(1-1/left)*nr)],40)
    h,hx=transport(op,radius,match,h_launch,64)
    g_slope=spec.compact_bound(sigma,False,True,spec.CELLS,False)[0][0]
    wronskian=h*g_slope-hx
    assert wronskian.abs_lower()>0

    # The factor g(rmatch) is set to one and cancels in the left coefficient.
    # The right coefficient multiplies this normalized g.
    delta=arb(3)/25
    lower=arb("1.595")
    u_start=-radius-2*(radius-1).log()
    assert (-lower-2*(lower-1).log()-u_start)>delta
    g_launch=transport(op,match,radius,[acb(1),g_slope],64)
    step=(lower-radius)/4
    moments=[acb(0),acb(0)]
    states=[h_launch,g_launch]
    for j in range(4):
        start=radius+j*step
        fields=[advance(op,start,step,state) for state in states]
        for i in range(1024):
            t=(i+arb(".5",".5"))*step/1024
            r=start+t
            f=(r-1)/r
            clock=-r-2*(r-1).log()
            T=-(r-1).log()
            b=bump((clock-u_start)/delta)*delta.sqrt()*b_norm
            weight=(-sigma*T).exp()*(1+1/r)*b/f*(-step/1024)
            for k,local in enumerate(fields):
                field,derivative=[v.value(acb(t)) for v in local]
                moments[k]+=weight*(derivative+sigma*field)
        states=[[v.value(acb(step)) for v in local] for local in fields]
    return -moments[1]/wronskian,-moments[0]/wronskian,wronskian


@dataclass
class Inputs:
    s: acb
    omega: acb
    k: acb
    bump_normalization: arb
    gl_scale: acb
    gl_match: list[acb]
    launch_left: acb
    launch_right: acb
    baseline_w: acb


def inputs():
    s=spec.shift(spec.CENTER,arb("5e-8"))
    omega=acb(spec.OMEGA)
    k=acb(spec.K)
    b_norm=normalized_bump()
    scale,match=gl_scale(k,omega)
    launch_left,launch_right,w=baseline_horizon(s-2*omega,b_norm)
    return Inputs(s,omega,k,b_norm,scale,match,launch_left,launch_right,w)


if __name__=="__main__":
    ctx.prec=128
    ctx.cap=80
    data=inputs()
    assert arb("-25.909") < data.gl_scale.real < arb("-25.907")
    assert arb("-.00718") < data.launch_left.real < arb("-.00702")
    assert arb(".00661") < data.launch_left.imag < arb(".00679")
    assert arb("-.00659") < data.launch_right.real < arb("-.00641")
    assert arb(".01083") < data.launch_right.imag < arb(".01097")
    print("bump normalization for exp(4-1/[y(1-y)]) =",data.bump_normalization)
    print("physical GL horizon scale =",data.gl_scale)
    print("GL regular N,N_r at r=2.5 =",data.gl_match)
    print("baseline W with g(2.5)=1 =",data.baseline_w)
    print("physical compact-launch horizon amplitude =",data.launch_left)
    print("physical compact-launch outer amplitude for g(2.5)=1 =",data.launch_right)
