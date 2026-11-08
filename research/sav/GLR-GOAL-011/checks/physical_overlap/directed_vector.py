"""A posteriori enclosures for two-component directed linear flows."""
from dataclasses import dataclass

import numpy as np
from flint import acb,acb_poly,acb_series,arb
from scipy.integrate import solve_ivp
from scipy.interpolate import CubicSpline

from exterior_source import midpoint,norm2,numerical_abscissa
from horizon_series import box,matvec


@dataclass
class VectorPiece:
    polynomial: list
    errors: list
    reverse: bool

    def jet(self,t,degree):
        tau=1-t if self.reverse else t
        sign=-1 if self.reverse else 1
        jets=[]
        for p in self.polynomial:
            coefficients=[]
            for n in range(degree):
                coefficients.append(p(tau)*sign**n/(2 if n==2 else 1))
                p=p.derivative()
            jets.append(acb_series(coefficients,prec=degree))
        return jets

    def value(self,t,subcell):
        tau=1-t if self.reverse else t
        j=len(self.errors)-1-subcell if self.reverse else subcell
        return [p(tau)+box(self.errors[j]) for p in self.polynomial]


def indices(length,reverse):
    return range(length-1,-1,-1) if reverse else range(length)


def local_t(j,subdivisions,reverse=False):
    t=acb((j+arb(".5",".5"))/subdivisions)
    return 1-t if reverse else t


def advance(error,rate,force):
    rate=rate.upper()
    gain=rate.expm1()/rate if rate else arb(1)
    return (rate.exp()*error+gain*force).upper()


def directed_vector(cells,coefficients,initial,subdivisions,reverse=False,candidate_only=False):
    """Enclose dy/dx=M y+b, retaining the whole-subcell error norm."""
    order=indices(len(cells),reverse)
    start=int(reverse)
    knots=[cells[i].b if reverse else cells[i].a for i in order]
    knots.append(cells[order[-1]].a if reverse else cells[order[-1]].b)
    unit=(cells[0].step/(acb(cells[0].b)-acb(cells[0].a)))
    matrix=[]
    offset=[]
    for j,q in enumerate(knots):
        index=order[min(j,len(cells)-1)]
        t=acb(start if j<len(cells) else 1-start)
        M,b=coefficients(index,t,None)
        r=cells[index].radius(t)
        gamma=unit*r/(r-1)
        matrix.append([[midpoint(gamma*p) for p in row] for row in M])
        offset.append([midpoint(gamma*p) for p in b])
    sorted_knots=np.argsort(knots)
    matrix=CubicSpline(np.asarray(knots)[sorted_knots],np.asarray(matrix)[sorted_knots])
    offset=CubicSpline(np.asarray(knots)[sorted_knots],np.asarray(offset)[sorted_knots])

    def rhs(q,y):
        return matrix(q)@y+offset(q)

    center=np.asarray([midpoint(q) for q in initial])
    sol=solve_ivp(rhs,(knots[0],knots[-1]),center,method="DOP853",
                  dense_output=True,rtol=2e-11,atol=2e-13)
    assert sol.success
    values_at_knots=[sol.sol(q) for q in knots]
    error=norm2([q-acb(complex(v)) for q,v in zip(initial,values_at_knots[0])])
    if candidate_only:
        assert error==0
    result={}
    for step,index in enumerate(order):
        cell=cells[index]
        a,b=knots[step:step+2]
        p0,p1=values_at_knots[step:step+2]
        d0,d1=(b-a)*rhs(a,p0),(b-a)*rhs(b,p1)
        polynomial=[]
        for l,r,p,q in zip(p0,p1,d0,d1):
            l,r,p,q=[acb(complex(z)) for z in (l,r,p,q)]
            delta=r-l
            polynomial.append(acb_poly([l,p,3*delta-2*p-q,-2*delta+p+q]))
        errors=[]
        if candidate_only:
            result[index]=VectorPiece(polynomial,[arb(0)]*subdivisions,reverse)
            continue
        for j in range(subdivisions):
            tau=local_t(j,subdivisions)
            t=1-tau if reverse else tau
            signed_dx=cell.dx(t)*(-1 if reverse else 1)
            subcell=subdivisions-1-j if reverse else j
            M,b=coefficients(index,t,subcell)
            values=[p(tau) for p in polynomial]
            deriv=[p.derivative()(tau) for p in polynomial]
            defect=[d-signed_dx*(v+w) for d,v,w in zip(deriv,matvec(M,values),b)]
            rate=numerical_abscissa(M,-signed_dx)/subdivisions
            next_error=advance(error,rate,norm2(defect)/subdivisions)
            errors.append(max(error,next_error))
            error=next_error
        result[index]=VectorPiece(polynomial,errors,reverse)
    return [acb(complex(v))+box(error) for v in values_at_knots[-1]],error,result
