"""Certified GL and RW profiles on the continued outgoing radial ray."""
from dataclasses import dataclass
from math import pi, sqrt

import numpy as np
from flint import acb, acb_poly, arb, ctx
from scipy.integrate import solve_ivp

from certified_launch import inputs, spec, transport, massless
from horizon_series import box, gl_regular, rw_horizon, upper

MATCH = spec.R_MATCH
RAY = spec.W
FAR = 64
T = spec.T


def x(r):
    return r+(r-1).log()


def gl_coefficients(r,t,k,ell0,alpha):
    """Remove the constant and Coulomb terms algebraically."""
    k2=k*k
    d=1+k2*r**3
    d2=d*d
    den=r**4*d2
    base=-k2*r*r+(6*r-11)*t-alpha**2*t*t-alpha*t*(2-r)
    q=base*d2+18*t*t-6*(4*r-5)*t*d
    ell=ell0*den+alpha*t*r*r*d2
    return den,[q],ell,t*r**3*d2


def gl_tail(k,q,alpha,radius):
    fmax=1+1/radius
    dmin=k**2*radius**3-1
    assert dmin>0
    q_bound=(k**2+(6+11/radius)*fmax+alpha**2*fmax**2
             +alpha*fmax*(1+2/radius)+18*fmax**2/dmin**2
             +6*(4+5/radius)*fmax/dmin)/radius**2
    attenuation=(RAY.real*q-q/(radius-1)-alpha/radius).lower()
    assert attenuation>0
    kernel=(1+1/(radius-1))/(2*attenuation)
    tail=(2*kernel*q_bound).upper()
    assert 2*kernel*tail<1
    return tail


def gl_cells(data):
    k=data.k.real
    om=data.omega.real
    q=(k*k+om*om).sqrt()
    alpha=k*k/(2*q)
    w=(1+1j)/sqrt(2)
    k_mid=pi/5
    q_mid=sqrt(k_mid**2+float(om.mid())**2)
    alpha_mid=k_mid**2/(2*q_mid)
    grid=np.unique(np.r_[spec.OUTER*np.linspace(1,0,spec.CELLS+1)**3,FAR])[::-1]

    def rhs(a,state):
        r=2.5+w*a
        den,force,ell,_=gl_coefficients(r,r-1,k_mid,-q_mid,alpha_mid)
        p=state[0]
        return [w*r/(r-1)*(force[0]/den-2*ell/den*p-p*p)]

    sol=solve_ivp(rhs,(grid[0],grid[-1]),[0j],t_eval=grid,method="DOP853",
                  rtol=4e-14,atol=1e-15)
    assert sol.success
    samples=[(float(a),np.array([p]),np.array([rhs(a,[p])[0]/w]))
             for a,p in zip(grid,sol.y[0])]
    capture=[]
    spec.directed_flow(samples,1,True,-q,alpha,
                       lambda r,t:gl_coefficients(r,t,acb(k),-q,alpha),
                       gl_tail(k,q,alpha,arb(spec.OUTER)),arb(0),False,capture)
    return capture,acb(-q),acb(alpha)


@dataclass
class Cell:
    a: float
    b: float
    left: acb
    step: acb
    p: list
    error: arb

    def radius(self,t):
        return 1+self.left+self.step*t

    def dx(self,t):
        return self.step*self.radius(t)/(self.radius(t)-1)

    def slopes(self,t,ell0,alpha):
        r=self.radius(t)
        ell=ell0+alpha*(1-1/r)/r
        size=len(self.p)
        dim=1 if size==1 else 2
        return [p(t)+box(self.error)+(ell if i//dim==i%dim else 0)
                for i,p in enumerate(self.p)]

    def log_primitive(self):
        """Polynomial primitive of the central P times dx/dt, with uniform error."""
        ratio=upper(self.step/self.left)
        assert ratio<1
        correction=acb_poly([(-self.step/self.left)**j/self.left for j in range(9)])
        derivative=(1+correction)*self.p[0]*self.step
        remainder=upper(self.step)*sum(upper(q) for q in self.p[0].coeffs())
        remainder*=ratio**9*upper(1/self.left)/(1-ratio)
        remainder+=self.error*upper(self.dx(T))
        return derivative.integral(),remainder.upper()


class ScalarProfile:
    def __init__(self,capture,ell0,alpha,at_match):
        self.ell0,self.alpha,self.at_match=ell0,alpha,at_match
        self.cells=[Cell(*cell) for cell in reversed(capture) if cell[0]<=FAR]
        self.integrals=[]
        current=acb(0)
        for cell in self.cells:
            primitive,error=cell.log_primitive()
            self.integrals.append((current,primitive,error))
            current+=primitive(0)-primitive(1)+box(error)

    def evaluate(self,index,t):
        cell=self.cells[index]
        prefix,primitive,error=self.integrals[index]
        r=cell.radius(t)
        phase=self.ell0*(x(r)-x(acb(MATCH)))+self.alpha*(r/MATCH).log()
        logarithm=prefix+primitive(t)-primitive(1)+box(error*upper(t-1))
        return self.at_match*(phase+logarithm).exp(),cell.slopes(t,self.ell0,self.alpha)[0]

    def far(self):
        return self.evaluate(len(self.cells)-1,acb(0))


class CompactGL:
    """Validated normalized N=e^{-Omega x}u on the real compact leg."""
    def __init__(self,data,cells=2048,grid=None):
        start=arb(1)/16 if grid is None else arb(grid[0])
        k,om=data.k,data.omega
        n,nr=[v.value(acb(start)) for v in gl_regular(k,om,data.gl_scale,60)]
        p_initial=start/(1+start)*nr/n
        km,om0=pi/5,float(om.real.mid())

        def local(r,t,k,omega):
            d=1+k*k*r**3
            d2=d*d
            den=r**4*d2
            q=t*((k*k*r**3+6*r-11)*d2+18*t-6*(4*r-5)*d)
            return den,[q],omega*den,t*r**3*d2

        def rhs(t,state):
            r=1+t
            den,q,_,_=local(r,t,km,om0)
            p=state[0]
            return [r/t*(q[0]/den-2*om0*p-p*p)]

        if grid is None:
            grid=np.geomspace(float(start),1.5,cells+1)
            grid[0],grid[-1]=float(start),1.5
        sol=solve_ivp(rhs,(grid[0],grid[-1]),[float(p_initial.real.mid())],
                      t_eval=grid,method="DOP853",rtol=4e-14,atol=1e-15)
        assert sol.success
        samples=[(float(t),np.array([p]),np.array(rhs(t,[p])))
                 for t,p in zip(grid,sol.y[0])]
        initial=upper(p_initial-acb(samples[0][1][0]))
        capture=[]
        spec.directed_flow(samples,1,False,om,acb(0),lambda r,t:local(r,t,k,om),
                           initial,arb(0),False,capture,initial_is_error=True)
        self.cells=[Cell(*cell) for cell in capture]
        self.omega=om
        self.initial=n
        self.integrals=[]
        current=acb(0)
        for cell in self.cells:
            primitive,error=cell.log_primitive()
            self.integrals.append((current,primitive,error))
            current+=primitive(1)-primitive(0)+box(error)
        n,p=self.evaluate(len(self.cells)-1,acb(1))
        assert n.overlaps(data.gl_match[0])
        assert (n*(p-om)/(1-1/arb(MATCH))).overlaps(data.gl_match[1])

    def evaluate(self,index,t):
        cell=self.cells[index]
        prefix,primitive,error=self.integrals[index]
        logarithm=prefix+primitive(t)-primitive(0)+box(error*upper(t))
        return self.initial*logarithm.exp(),cell.slopes(t,self.omega,acb(0))[0]


def rw_profile(s,at_match):
    capture=[]
    spec.compact_bound(s,False,True,spec.CELLS,False,capture=capture,knot=FAR)
    return ScalarProfile(capture,-s,acb(0),at_match)


def horizon_rw_match(s):
    left=arb("1.125")
    n,nr=[v.value(acb(left-1)) for v in rw_horizon(s,40)]
    e=(s*x(acb(left))).exp()
    return transport(massless(s),left,MATCH,[e*n,e*(s*n+(1-1/left)*nr)],96)[0]


def profiles(data,returning=True):
    gl_capture,ell0,alpha=gl_cells(data)
    gl=ScalarProfile(gl_capture,ell0,alpha,data.gl_match[0]*(data.omega*x(acb(MATCH))).exp())
    baseline=rw_profile(data.s-2*data.omega,data.launch_right)
    test=rw_profile(data.s,horizon_rw_match(data.s)) if returning else None
    massive_capture=[]
    s1=data.s-data.omega
    spec.compact_bound(s1,True,True,spec.CELLS,False,capture=massive_capture,knot=FAR)
    massive=[Cell(*cell) for cell in massive_capture if cell[0]<=FAR]
    mell,mpower=spec.branch(s1,True,True,data.k)
    return gl,baseline,test,massive,mell,mpower


if __name__=="__main__":
    ctx.prec=128
    ctx.cap=80
    data=inputs()
    gl,base,test,massive,*_=profiles(data)
    assert len(gl.cells)==len(base.cells)==len(test.cells)==len(massive)
    print("GL far |u| upper",upper(gl.far()[0]))
    print("baseline far |Psi| upper",upper(base.far()[0]))
    print("returning adjoint far |h| upper",upper(test.far()[0]))
