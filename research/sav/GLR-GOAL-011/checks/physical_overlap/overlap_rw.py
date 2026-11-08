"""Jost and compact launch profiles for the empirical GOAL-011 overlap."""
import numpy as np
from scipy.integrate import quad
from scipy.optimize import root
from scipy.special import wrightomega

from overlap_gl import integrate,tortoise

CENTER=-0.177924631-0.747343369j


def f(r):
    return 1-1/r


def potential(r):
    return f(r)*(6/r**2-3/r**3)


class Jost:
    def __init__(self,s,rmatch,ray,length,left_delta,tol,extend=False):
        self.s,self.rmatch,self.ray=s,rmatch,ray

        def left_eq(y,state):
            r=1+np.exp(y)
            return r*np.array([state[1],potential(r)*state[0]-2*s*state[1]])

        a1=3/(1+2*s)
        a2=((2-6*s)*a1+6)/(4*(1+s))
        d=left_delta
        initial=np.array([1+a1*d+a2*d*d,d/(1+d)*(a1+2*a2*d)])
        self.h=integrate(left_eq,(np.log(d),np.log(rmatch-1)),initial,tol)
        c=[1+0j]
        for n in range(1,11):
            c.append(-((n*(n-1)-6)*c[-1]+(3-n*(n-2))*(c[-2] if n>1 else 0))/(2*s*n))
        end=rmatch+ray*length
        start=np.array([sum(v/end**n for n,v in enumerate(c)),
                        -f(end)*sum(n*v/end**(n+1) for n,v in enumerate(c))])

        def right_r_eq(r,state):
            return np.array([state[1],potential(r)*state[0]+2*s*state[1]])/f(r)

        def right_eq(q,state):
            return ray*right_r_eq(rmatch+ray*q,state)

        self.g=integrate(right_eq,(length,0),start,tol)
        self.g_inner=None
        if extend:
            self.g_inner=integrate(right_r_eq,(rmatch,1.35),self.g.sol(0),tol)
        hl,hd=self.left(rmatch)
        gl,gd=self.right(rmatch)
        self.w=hl*gd-hd*gl
        self.log_difference=hd/hl-gd/gl
        self.ray_scale=hl/gl

    def left_factored(self,r):
        return self.h.sol(np.log(r-1))

    def left(self,r):
        n,d=self.left_factored(r)
        return np.exp(self.s*tortoise(r))*np.array([n,d+self.s*n])

    def right(self,r):
        if np.imag(r)==0 and r.real<self.rmatch:
            n,d=self.g_inner.sol(r.real)
        else:
            n,d=self.g.sol(((r-self.rmatch)/self.ray).real)
        return np.exp(-self.s*tortoise(r))*np.array([n,d-self.s*n])

    def mode(self,r):
        if np.imag(r)==0 and r.real<=self.rmatch:
            return self.left(r.real)
        return self.ray_scale*self.right(r)


def pole(rmatch,ray,length,left_delta,tol):
    def objective(z):
        p=Jost(complex(*z),rmatch,ray,length,left_delta,tol)
        return [p.log_difference.real,p.log_difference.imag]

    rt=root(objective,[CENTER.real,CENTER.imag],tol=5e-10)
    s=complex(*rt.x)
    assert abs(s-CENTER)<5e-8 and np.linalg.norm(objective(rt.x))<2e-9
    return s


def bump(y):
    return np.exp(4-1/(y*(1-y))) if 0<y<1 else 0.


def bump_derivative(y):
    return bump(y)*(1-2*y)/(y*(1-y))**2 if 0<y<1 else 0.


class Baseline:
    def __init__(self,jost,radius,width,tol):
        self.jost,self.radius,self.width=jost,radius,width
        self.start_u=radius-2*tortoise(radius)
        self.inner=1+2*wrightomega(-np.log(2)-(1+self.start_u+width)/2)
        if not 1.35 < self.inner < radius < jost.rmatch:
            raise ValueError("Compact packet must lie in 1.35 < r < rmatch")
        self.norm=1/np.sqrt(quad(lambda y:bump_derivative(y)**2,0,1,epsabs=1e-12)[0])

        def rhs(y,_):
            r=1+2*wrightomega(-np.log(2)-(1+self.start_u+width*y)/2)
            t=r-tortoise(r)
            h,hx=jost.left(r)
            g,gx=jost.right(r)
            factor=np.exp(-jost.s*t)*self.norm*width**1.5*bump(y)
            return factor*np.array([hx+jost.s*h,gx+jost.s*g])

        self.moments=integrate(rhs,(0,1),np.zeros(2,dtype=complex),tol,max_step=.04)
        self.full=self.moments.sol(1)

    def fields(self,r):
        """Return (Q_z,A_v,A_r,F_vr) in EF chart, transformed in t."""
        p=self.jost
        ff=f(r)
        if np.imag(r)==0 and r.real<=self.inner:
            n,nx=p.left_factored(r.real)
            factor=-self.full[1]/p.w*np.exp(p.s*tortoise(r))
            psi=factor*n
            psi_x=factor*(p.s*n+nx)
            av=(ff*psi+r*psi_x)/(2*r**2)
            ar=-psi/(2*r**2)-factor*nx/(2*r*ff)
        elif np.imag(r)!=0 or r.real>=self.radius:
            psi,psi_x=-self.full[0]/p.w*p.right(r)
            av=(ff*psi+r*psi_x)/(2*r**2)
            ar=(p.s*psi/(2*r)-av)/ff
        else:
            t=r-tortoise(r)
            y=(t-tortoise(r)-self.start_u)/self.width
            partial=self.moments.sol(y)
            jh,jg=self.full[0]-partial[0],partial[1]
            h,hx=p.left(r)
            g,gx=p.right(r)
            psi=-(g*jh+h*jg)/p.w
            launch=np.exp(-p.s*t)*np.sqrt(self.width)*self.norm*bump(y)
            psi_x=-(gx*jh+hx*jg)/p.w+launch
            av=(ff*psi+r*psi_x)/(2*r**2)
            ar=((p.s*psi-launch)/(2*r)-av)/ff
        q=2*r*psi
        return q,av,ar,-q/r**4
