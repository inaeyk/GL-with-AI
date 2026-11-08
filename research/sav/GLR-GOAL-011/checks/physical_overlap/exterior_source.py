"""Validated outgoing affine mixed datum and right source profiles."""
import numpy as np
from flint import acb,acb_poly,arb,ctx
from scipy.integrate import solve_ivp
from scipy.interpolate import CubicSpline

from certified_launch import inputs
from exterior_profiles import FAR, profiles
from horizon_series import box,endomorphism,matvec,inverse,upper
from outer_gljet import njet
from enclosure_math import centered
from outer_tail import OuterTail

I=acb(0,1)
LAMBDA=4


def midpoint(z):
    return complex(float(z.real.mid()),float(z.imag.mid()))


def forcing_coefficients(r,k,omega,s,p,p_psi):
    """Physical first insertion F,G in U_x=J+F, J_x=(s1^2+V)U+G."""
    f=1-1/r
    s1=s-omega
    sigma=s-2*omega
    # The exact common amplitude u*Psi is removed before enclosure.
    h,_,_=njet(r,k,omega,1,(p-omega)/f)
    av=(f+r*p_psi)/(2*r*r)
    ar=(-1/r**2+(sigma-p_psi)/(f*r))/2
    return from_fields(r,k,s1,h,1,av,ar)


def from_fields(r,k,s1,h,psi,av,ar):
    f=1-1/r
    x,sphere=endomorphism(r,h)
    yv,yr=[-LAMBDA*r*r*a for a in matvec(x,matvec(inverse(r),[av,ar]))]
    yt=f*yv-yr
    yx=yr
    curl=6*r*sphere*psi
    d=LAMBDA+k*k*r*r
    root_d=r*d.sqrt()
    c=I*k/(2*d.sqrt())
    a=f*(1/r+k*k*r/d)
    b=f*(2/r-k*k*r/d)
    F=[yt/root_d,-c*yx]
    G=[(s1*yx-a*yt)/root_d+f*root_d*curl/r**4,
       -c*(s1*yt+b*yx)]
    return F+G


def forcing(r,k,omega,s,u,p,psi,p_psi):
    result=[u*psi*z for z in centered(forcing_coefficients,(r,k,omega,s,p,p_psi))]
    return result[:2],result[2:]


def matrix(values):
    return [values[:2],values[2:]]


def norm2(values):
    return sum(upper(z)**2 for z in values).sqrt().upper()


def numerical_abscissa(p,dx):
    bounds=[]
    for i,j in ((0,1),(1,0)):
        a=(-dx*p[i][i]).real
        a+=upper((-dx*p[i][j]+(-dx*p[j][i]).conjugate())/2)
        bounds.append(a.upper())
    return max(bounds)


class Exterior:
    def __init__(self,data):
        self.data=data
        self.gl,self.baseline,self.test,self.massive,self.ell,self.power=profiles(data)
        self.tail=OuterTail(k=data.k.real,omega=data.omega.real,s=data.s)
        geometry=[(c.a,c.b) for c in self.massive]
        for scalar in (self.gl,self.baseline,self.test):
            assert geometry==[(c.a,c.b) for c in reversed(scalar.cells)]

    def stationary_data(self,index,t):
        r=self.massive[index].radius(t)
        j=len(self.massive)-1-index
        u,p=self.gl.evaluate(j,t)
        psi,g=self.baseline.evaluate(j,t)
        F,G=forcing(r,self.data.k,self.data.omega,self.data.s,u,p,psi,g)
        P=matrix(self.massive[index].slopes(t,self.ell,self.power))
        return F,G,P

    def affine(self,subdivisions=2):
        """Enclose the exact affine source contribution at the real match."""
        tail=self.tail.affine_bound(self.gl.far()[0],self.baseline.far()[0])
        initial=[box(tail),box(tail)]
        endpoint,error,peak=validate_affine(
            self.massive,self.stationary_data,initial,subdivisions,initial_norm=tail)
        return endpoint,error,tail,peak


def validate_affine(cells,stationary,initial,subdivisions,initial_norm=None):
    """Certify an inhomogeneous affine first order flow in either direction."""
    knots=[c.a for c in cells]+[cells[-1].b]
    assert knots[1:]==[c.b for c in cells]
    a=[]
    b=[]
    unit=(cells[0].step/(acb(knots[1])-acb(knots[0])))
    for j in range(len(knots)):
        index=min(j,len(cells)-1)
        t=acb(j!=index)
        F,G,P=stationary(index,t)
        r=cells[index].radius(t)
        dx=unit*r/(r-1)
        a.append([[midpoint(-dx*z) for z in row] for row in P])
        b.append([midpoint(dx*(g-p)) for g,p in zip(G,matvec(P,F))])
    order=np.argsort(knots)
    coefficient=CubicSpline(np.asarray(knots)[order],np.asarray(a)[order])
    inhomogeneous=CubicSpline(np.asarray(knots)[order],np.asarray(b)[order])

    def rhs(q,y):
        return coefficient(q)@y+inhomogeneous(q)

    center=np.asarray([midpoint(z) for z in initial])
    solution=solve_ivp(rhs,(knots[0],knots[-1]),center,dense_output=True,
                       method="DOP853",rtol=2e-11,atol=2e-13)
    assert solution.success
    candidate=solution.sol
    if initial_norm is not None:
        assert all(z==0 for z in candidate(knots[0]))
        error=initial_norm
    else:
        error=norm2([z-acb(complex(c)) for z,c in zip(initial,candidate(knots[0]))])
    peak=error
    for index,cell in enumerate(cells):
        a,b=cell.a,cell.b
        left,right=candidate(a),candidate(b)
        slope_left=(b-a)*rhs(a,left)
        slope_right=(b-a)*rhs(b,right)
        polynomial=[]
        for l,r,p,q in zip(left,right,slope_left,slope_right):
            l,r,p,q=[acb(complex(z)) for z in (l,r,p,q)]
            delta=r-l
            polynomial.append(acb_poly([l,p,3*delta-2*p-q,-2*delta+p+q]))
        for j in range(subdivisions):
            t=acb((j+arb(".5",".5"))/subdivisions)
            dx=cell.dx(t)
            F,G,P=stationary(index,t)
            pp=[v(t) for v in polynomial]
            residual=[polynomial[i].derivative()(t)+dx*(w-g)
                      for i,(w,g) in enumerate(zip(matvec(P,[p+f for p,f in zip(pp,F)]),G))]
            rate=numerical_abscissa(P,dx)/subdivisions
            force=norm2(residual)/subdivisions
            growth=rate.exp()
            integral=rate.expm1()/rate if rate!=0 else arb(1)
            error=(growth*error+integral*force).upper()
            peak=max(peak,error)
    endpoint=[acb(complex(z))+box(error) for z in candidate(knots[-1])]
    return endpoint,error,peak


if __name__=="__main__":
    ctx.prec=128
    ctx.cap=80
    e=Exterior(inputs())
    value,error,tail,peak=e.affine()
    print("physical GL |u(64)| upper",upper(e.gl.far()[0]))
    print("baseline |Psi(64)| upper",upper(e.baseline.far()[0]))
    print("returning adjoint |h(64)| upper",upper(e.test.far()[0]))
    print("outgoing affine initial tail norm upper",tail)
    print("outgoing affine match error norm upper",error,"peak",peak)
    print("outgoing affine match enclosure",value)
