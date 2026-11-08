from math import pi
import numpy as np
from flint import acb, ctx
from overlap_gl import tortoise
from returning_overlap import Experiment
from return_coefficients import mixed_coefficients, algebraic_coefficients, dot

ctx.prec=128
e=Experiment(13/8,.12,64,1e-6,2e-11,pi/4)
s=e.s
o=e.gl.omega
k=e.gl.k
sig=s-2*o
peak=0.
for r in (1.014,1.04,1.4,1.603,1.614,1.625,1.9,2.5,e.rmatch+e.ray*1,e.rmatch+e.ray*9):
    x=tortoise(r)
    f=1-1/r
    if np.imag(r)==0:
        if r<=e.gl.start:
            n,nr=e.gl.series(r-1),e.gl.series.deriv()(r-1)
        else:
            n,nr=e.gl.left.sol(r)
        u=e.gl.scale*n*np.exp(o*x)
        p=o+f*nr/n
        md=None
        state=e.mixed._unfactor(r,e.mixed._left.sol(np.log(r-1)),left=True)@e.mixed._left_amplitudes
    else:
        md=((r-e.rmatch)/e.ray).real
        p,log_u=e.gl.right.sol(md)
        u=e.gl.scale*e.gl.n_match*np.exp(o*tortoise(e.rmatch)+log_u-e.gl.log_match)
        state=e.mixed._unfactor(r,e.mixed._right_ray.sol(md),left=False)@e.mixed._right_amplitudes
    h,hx=e.test.mode(r)
    q,av,ar,_=e.baseline.fields(r)
    psi=q/(2*r)
    psix=(2*r*r*av-f*psi)/r
    launch=sig*psi-2*r*(av+f*ar)
    alpha=mixed_coefficients(*(acb(complex(v)) for v in (r,k,o,s,p,hx/h)))
    c,yt,yx=e.mixed_source(r)
    # alpha[5] factors X^{tx}/(6rS) = u*Psi.
    hj=u*h*(dot(alpha[:2],state[:2])+dot(alpha[2:4],state[2:]-(s-o)*state[:2])
            +alpha[4]*yx+alpha[5]*u*psi)
    second,mu,beta=algebraic_coefficients(*(acb(complex(v)) for v in (r,k,o,s,p,hx/h,psi,psix,launch)))
    mean=e.mean(r)
    val=np.array([complex(hj),complex(h*u*u*second),complex(h*(mu*mean[0]+beta*mean[1]))])
    ref=f*e.weak(r,md)
    error=np.max(np.abs(val-ref))/max(1,np.max(np.abs(ref)))
    peak=max(peak,error)
    print(r,error)
print('scaled weak coefficient discrepancy',peak)
assert peak<5e-9
