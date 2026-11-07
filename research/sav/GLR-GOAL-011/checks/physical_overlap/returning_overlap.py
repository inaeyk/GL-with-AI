"""Empirical full ordered GLR-011 overlap on the continued outgoing sheet."""
import argparse
import platform

import numpy as np
import scipy
from scipy.integrate import quad_vec
from scipy.special import wrightomega
from numpy.polynomial import Chebyshev, Polynomial

from overlap_gl import GLProfile,tortoise
from overlap_mean import MeanGeometry
from overlap_mixed import MixedSolution
from overlap_rw import Baseline,Jost,bump,pole

LABELS=("H J","H^2 P","B_E P")
LAMBDA=4


def orbit_inverse(r):
    return np.array([[0,1],[1,1-1/r]])


class Experiment:
    def __init__(self,radius,width,length,left_delta,tol,angle,gl_scale=1.):
        self.rmatch=2.5
        self.ray=np.exp(1j*angle)
        self.length=length
        self.left=1+left_delta
        self.tol=tol
        self.gl=GLProfile(self.rmatch,self.ray,length,left_delta,tol)
        self.gl.scale*=gl_scale
        args=(self.rmatch,self.ray,length,left_delta,tol)
        self.s=pole(*args)
        self.test=Jost(self.s,*args)
        self.baseline=Baseline(Jost(self.s-2*self.gl.omega,*args,extend=True),radius,width,tol)
        self.mean=MeanGeometry(self.gl,self.rmatch,self.ray,length,tol)
        self.mixed=MixedSolution(self.mixed_source,self.s-self.gl.omega,self.gl.k,*args)
        step=1e-5
        diff=Jost(self.s+step,*args).w-Jost(self.s-step,*args).w
        self.wprime=diff/(2*step)

    def outgoing_homogeneous(self,radius):
        """Ordinary mean pole from one independent unit canonical packet."""
        start=radius-2*tortoise(radius)
        width=self.baseline.width

        def integrand(y):
            r=1+2*wrightomega(-np.log(2)-(1+start+width*y)/2)
            h,hx=self.test.left(r)
            return np.exp(-self.s*(r-tortoise(r)))*(hx+self.s*h)*bump(y)

        moment,_=quad_vec(integrand,0,1,epsabs=1e-12,epsrel=1e-12)
        return -self.baseline.norm*width**1.5*moment/self.wprime

    def gl_blocks(self,r):
        vv,vr,rr,sphere=np.exp(self.gl.omega*tortoise(r))*self.gl(r)
        inverse=orbit_inverse(r)
        return inverse @ np.array([[vv,vr],[vr,rr]]),sphere/r**2

    def mixed_source(self,r):
        inv=orbit_inverse(r)
        h,sphere=self.gl_blocks(r)
        q,av,ar,_=self.baseline.fields(r)
        density=-LAMBDA*r*r*h@inv@np.array([av,ar])
        ff=1-1/r
        return 3*sphere*q,ff*density[0]-density[1],density[1]

    def densities(self,r,distance=None):
        inv=orbit_inverse(r)
        h,sphere=self.gl_blocks(r)
        q,av,ar,_=self.baseline.fields(r)
        base_raised=inv@np.array([av,ar])
        if distance is None:
            at,ax,ftx,_=self.mixed(r)
        else:
            at,ax,ftx,_=self.mixed.ray_fields(distance)
        f=1-1/r
        first_raised=inv@np.array([at,(ax-at)/f])
        mu,beta=self.mean(r)
        metric=inv@np.array([[2*mu/r-2*f*beta,beta],[beta,0]])
        identity=np.eye(2)
        tr2=np.trace(h@h)
        direct=(h@h-(tr2/4+sphere*sphere/2)*identity)/2
        geometric=beta*identity-metric
        currents=LAMBDA*r*r*np.stack([-h@first_raised/2,direct@base_raised,
                                       geometric@base_raised],axis=1)
        curls=np.array([-3*sphere*r**4*ftx/(2*f),
                         (tr2/4+3.5*sphere*sphere)*q/2,-beta*q])
        return currents,curls

    def weak(self,r,distance=None):
        h,hx=self.test.mode(r)
        f=1-1/r
        av=(f*h+r*hx)/(LAMBDA*r*r)
        ar=(-self.s*r*h-f*h-r*hx)/(LAMBDA*r*r*f)
        d,c=self.densities(r,distance)
        return 2*(av*d[0]+ar*d[1]-h*c/r**3)

    def regular_current(self,r,distance=None):
        f=1-1/r
        d,c=self.densities(r,distance)
        factor=np.exp(-self.s*tortoise(r))
        v=factor*d[0]/r**2
        p=factor*d[1]/r**2
        return p-f*v,v,factor*c

    def collar(self,cap,degree):
        """Integrate the regular strong density through the horizon collar."""
        split=1+cap
        assert self.left<1+cap/2<split<self.baseline.inner
        nodes=np.cos(np.pi*(np.arange(degree+4)+0.5)/(degree+4))
        samples=cap*(1.75+1.25*nodes)
        regular=np.asarray([self.regular_current(1+t) for t in samples])
        fits=[[Chebyshev.fit(samples,regular[:,i,j],degree) for j in range(3)]
              for i in range(3)]
        slopes=[q.deriv() for q in fits[0]]

        t_poly=Polynomial([0.,1.])
        r_poly=1+t_poly
        a_poly=t_poly*r_poly**2
        b_poly=r_poly+2*self.s*r_poly**3
        c_poly=6*r_poly-3
        series=np.zeros(15,dtype=complex)
        series[0]=1
        for n in range(1,len(series)):
            v=Polynomial(series[:n])
            residual=a_poly*v.deriv(2)+b_poly*v.deriv()-c_poly*v
            series[n]=-residual.coef[n-1]/(n*(n+2*self.s))
        horizon_test=Polynomial(series)

        def near(y):
            t=cap*np.exp(-y)
            r=1+t
            derivative=np.array([q(t) for q in slopes])
            v=np.array([q(t) for q in fits[1]])
            c=np.array([q(t) for q in fits[2]])
            weight=r*horizon_test(t)*np.exp(2*self.s*r)*t**(1+2*self.s)
            return 2/LAMBDA*weight*(derivative-self.s*v-LAMBDA*c/r**4)

        qa=abs(self.tol)**0.8
        opts=dict(epsabs=qa,epsrel=qa,limit=400)
        return quad_vec(near,0,90,**opts)

    def evaluate(self,cap,degree):
        split=1+cap
        near_v,near_e=self.collar(cap,degree)
        boundary=split*self.test.left(split)[0]*np.exp(self.s*tortoise(split))
        near_v-=2/LAMBDA*boundary*self.regular_current(split)[0]
        qa=abs(self.tol)**0.8
        opts=dict(epsabs=qa,epsrel=qa,limit=400)
        real_v,real_e=quad_vec(self.weak,split,self.rmatch,
                                points=[self.baseline.inner,self.baseline.radius],**opts)
        ray_v,ray_e=quad_vec(lambda d:self.ray*self.weak(self.rmatch+self.ray*d,d),
                              0,self.length,**opts)
        return near_v+real_v+ray_v,(near_e,real_e,ray_e),ray_v


def main():
    p=argparse.ArgumentParser()
    p.add_argument("--length",type=float,default=48)
    p.add_argument("--left-delta",type=float,default=1e-5)
    p.add_argument("--tol",type=float,default=2e-10)
    p.add_argument("--angle",type=float,default=np.pi/4)
    p.add_argument("--radius",type=float,default=13/8)
    p.add_argument("--width",type=float,default=.12)
    p.add_argument("--cap",type=float,default=.006)
    p.add_argument("--degree",type=int,default=6)
    p.add_argument("--repeat-cap",type=float)
    p.add_argument("--gl-scale",type=float,default=1.)
    a=p.parse_args()
    print(f"Python {platform.python_version()}, NumPy {np.__version__}, SciPy {scipy.__version__}",flush=True)
    print(a,flush=True)
    e=Experiment(a.radius,a.width,a.length,a.left_delta,a.tol,a.angle,a.gl_scale)
    print("s =",e.s,"Wprime =",e.wprime,"baseline W =",e.baseline.jost.w,flush=True)
    print("GL matching error",e.gl.match_error,"mixed match",e.mixed.match_error,
          "mixed sigma_min",e.mixed.sigma_min,"EF mean at match",e.mean.match_ef,flush=True)
    i,err,tail=e.evaluate(a.cap,a.degree)
    print("quadrature estimates",err,"ray contribution",tail,flush=True)
    for label,value in zip(LABELS,i):
        print(label,value,"residue term",value/e.wprime,flush=True)
    residue=sum(i)/e.wprime
    print("I =",sum(i),"residue =",residue,flush=True)
    baseline=e.outgoing_homogeneous(a.radius)
    direction=e.s*baseline
    remainder=residue-(residue*direction.conjugate()).real*direction/abs(direction)**2
    print("baseline residue =",baseline,"real-time-shift orthogonal component =",remainder,flush=True)
    print("independent later unit upper packet residue change =",e.outgoing_homogeneous(1.46),flush=True)
    print("far endpoint norm",np.linalg.norm(e.weak(e.rmatch+e.ray*e.length,e.length)),flush=True)
    if a.repeat_cap is not None:
        j,_,_=e.evaluate(a.repeat_cap,a.degree)
        print("regular collar difference",j-i,flush=True)


if __name__=="__main__":
    main()
