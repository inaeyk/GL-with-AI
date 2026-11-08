"""Empirical complex-contour L10 GL profile in regular EF components."""
from functools import lru_cache
import numpy as np
import sympy as sp
from numpy.polynomial import Polynomial
from scipy.integrate import solve_ivp

K = np.pi/5
OMEGA = 0.06328118897590773


def tortoise(r):
    return r+np.log(r-1)


def potential(r,k):
    d=1+k*k*r**3
    return (r-1)/r*(k*k+(6*r-11)/r**3+18*(r-1)/(r**3*d*d)
                    -6*(4*r-5)/(r**3*d))


def integrate(rhs, interval, initial, tol, max_step=0.5):
    sol=solve_ivp(rhs,interval,initial,rtol=tol,atol=tol*0.01,
                  method="DOP853",dense_output=True,max_step=max_step)
    if not sol.success:
        raise RuntimeError(sol.message)
    return sol


def horizon_series(k, omega, degree=26):
    """EF regular series; discard only the checked removable Laurent terms."""
    dtype=np.longdouble
    a=dtype(k)**2
    om=dtype(omega)
    one=Polynomial(np.array([1],dtype=dtype))
    t=Polynomial(np.array([0,1],dtype=dtype))
    r=one+t
    d=one+a*r**3
    a_poly=t*r**2*d**2
    b_poly=(r+2*om*r**3)*d**2
    c_poly=a*r**3*d**2+(6*r-11)*d**2+18*t-6*(4*r-5)*d
    coef=np.zeros(degree+1,dtype=dtype)
    coef[0]=1
    for n in range(1,degree+1):
        n_lower=Polynomial(coef[:n])
        resid=a_poly*n_lower.deriv(2)+b_poly*n_lower.deriv()-c_poly*n_lower
        coef[n]=-resid.coef[n-1]/(n*(n+2*om)*d.coef[0]**2)

    def inv(poly):
        q=poly.coef
        result=np.zeros(degree+1,dtype=dtype)
        result[0]=1/q[0]
        for j in range(1,degree+1):
            result[j]=-sum(q[m]*result[j-m] for m in range(1,min(j+1,len(q))))/q[0]
        return Polynomial(result)

    def clip(poly):
        return Polynomial(poly.coef[:degree+1])

    n=Polynomial(coef)
    ir,id_=inv(r),inv(d)
    f=t*ir
    y=clip(d*ir*n)
    yp=y.deriv()
    b=t*ir**3*id_/(2*a)-3*t**2*id_**2
    b_div_f=ir**2*id_/(2*a)-3*r*t*id_**2
    h_vv=clip((f*(dtype(.5)-a*r**3)*id_**2-om*om*id_/a)*y+b*yp+om*b_div_f*y)
    f_tr=clip(-om*id_/a*(f*yp+(om-ir**2/2)*y))
    h_sphere=clip(-r*r*id_*y/2-t*id_*yp/a-om*r*id_*y/a)
    h_vr_num=clip((f_tr-h_vv)*r)
    h_rr_num=clip(2*(h_vv-f_tr-f*h_sphere*ir**2)*r**2)
    assert abs(h_vr_num.coef[0])<1e-14
    assert np.max(np.abs(h_rr_num.coef[:2]))<1e-14
    ef=(h_vv,Polynomial(h_vr_num.coef[1:]),Polynomial(h_rr_num.coef[2:]),h_sphere)
    return n,ef


@lru_cache(maxsize=1)
def ef_evaluator():
    r,a,om,n0,n1,n2,n3=sp.symbols("r a om n0 n1 n2 n3")
    d=1+a*r**3
    f=1-1/r
    y=d*n0/r
    yp=sp.diff(y,r)+sp.diff(y,n0)*n1
    b=(r-1)/(2*a*r**3*d)-3*(r-1)**2/d**2
    b_div_f=1/(2*a*r**2*d)-3*r*(r-1)/d**2
    vv=(f*(sp.Rational(1,2)-a*r**3)/d**2-om**2/(a*d))*y+b*yp+om*b_div_f*y
    f_tr=-om/(a*d)*(f*yp+(om-1/(2*r*r))*y)
    sphere=-r*r*y/(2*d)-(r-1)*yp/(a*d)-om*r*y/(a*d)
    amplitudes=[vv,(f_tr-vv)/f,2*(vv-f_tr-f*sphere/r**2)/f**2,sphere]

    def deriv(expr):
        return (sp.diff(expr,r)+sp.diff(expr,n0)*n1+
                sp.diff(expr,n1)*n2+sp.diff(expr,n2)*n3)

    terms=[amplitudes,[deriv(q) for q in amplitudes],
           [deriv(deriv(q)) for q in amplitudes]]
    vp=f*(a+(6*r-11)/r**3+18*(r-1)/(r**3*d**2)-6*(4*r-5)/(r**3*d))
    coeff=[vp/f**2,-1/(r*r*f)-2*om/f]
    ode=sp.lambdify((r,a,om),coeff+[sp.diff(q,r) for q in coeff],"numpy",cse=True)
    ef=sp.lambdify((r,a,om,n0,n1,n2,n3),terms,"numpy",cse=True)
    return ef,ode


class GLProfile:
    def __init__(self,rmatch,ray,length,left_delta,tol,omega=OMEGA):
        self.k,self.omega=K,omega
        self.rmin=1+left_delta
        self.rmatch,self.ray=rmatch,ray
        self.start=1.11
        self.series,self.series_ef=horizon_series(self.k,omega)
        self.ef,self.ode=ef_evaluator()

        def left_eq(r,y):
            q,p,*_=self.ode(r,self.k**2,omega)
            return [y[1],q*y[0]+p*y[1]]

        initial=[self.series(self.start-1),self.series.deriv()(self.start-1)]
        self.left=integrate(left_eq,(self.start,rmatch),np.asarray(initial,dtype=float),tol)
        left_n,left_deriv=self.left.sol(rmatch)
        self.n_match=left_n
        p_left=omega+(1-1/rmatch)*left_deriv/left_n
        q=np.hypot(self.k,omega)
        a1=self.k**2/(2*q)
        a2=(-a1-6+a1*a1)/(2*q)
        a3=(a1-2*a2+17+2*a1*a2)/(2*q)
        end=rmatch+ray*length

        def right_eq(distance,y):
            r=rmatch+ray*distance
            return ray/(1-1/r)*np.array([potential(r,self.k)+omega**2-y[0]**2,y[0]])

        self.right=integrate(right_eq,(length,0),np.array(
            [-q+a1/end+a2/end**2+a3/end**3,0j]),tol)
        self.log_match=self.right.sol(0)[1]
        self.match_error=abs(p_left-self.right.sol(0)[0])
        self.scale=1.
        self.scale=8/(np.exp(omega*tortoise(2.))*self(2.)[3])

    def jet(self,r):
        if np.imag(r)==0 and r.real<=self.start:
            t=r.real-1
            return self.scale*np.asarray([[p.deriv(j)(t) for p in self.series_ef]
                                          for j in range(3)],dtype=float)
        if np.imag(r)==0 and r.real<=self.rmatch:
            n,nr=self.left.sol(r.real)
        else:
            distance=((r-self.rmatch)/self.ray).real
            p,log_u=self.right.sol(distance)
            n=self.n_match*np.exp(log_u-self.log_match-self.omega*(tortoise(r)-tortoise(self.rmatch)))
            nr=(p-self.omega)*n/(1-1/r)
        q,p,dq,dp=self.ode(r,self.k**2,self.omega)
        nrr=q*n+p*nr
        nrrr=dq*n+(q+dp)*nr+p*nrr
        return self.scale*np.asarray(self.ef(r,self.k**2,self.omega,n,nr,nrr,nrrr))

    def __call__(self,r):
        return self.jet(r)[0]


if __name__=="__main__":
    gl=GLProfile(2.5,np.exp(1j*np.pi/4),80,1e-6,1e-11)
    print("left/right GL p mismatch",gl.match_error)
    for r in (1.000001,1.11,1.110000001,2.,2.5,2.5+1j**.5):
        print(r,gl(r))
