"""Bound a local RW pole and both shifted pencils in r0=1 units.

SciPy produces candidate Hermite coefficients. The enclosures use only
the coefficients, outward-rounded residuals, and analytic tail estimates.
"""
from math import pi, sqrt
import platform

import flint
import numpy as np
import scipy
from flint import acb, acb_poly, arb, ctx
from scipy.integrate import solve_ivp

ctx.prec = 128
I = acb(0, 1)
W = (1 + I)/arb(2).sqrt()
K = arb.pi()/5
T = acb(arb("0.5", "0.5"))
R_MATCH = arb("2.5")
INNER = 1e-22
OUTER = 144
CENTER = acb("-0.177924631", "-0.747343369")
CELLS = 4200
OMEGA = arb("0.06328119", "0.00000001")


def shift(q, value):
    return q + acb(arb(0, value), arb(0, value))


def cap(z):
    return abs(z).upper()


def matrix_norm(a):
    return sum(cap(z)**2 for z in a).sqrt().upper()


def coefficients(r, r_minus, s, massive, right, k, ell0, alpha):
    """Denominator of R=V+s²-ell²-ell_x and of ell. Works with polynomials."""
    r2 = r*r
    r3 = r*r2
    r4 = r2*r2
    if not massive:
        return r4, [(r_minus*(6*r-3))], ell0*r4
    k2 = k*k
    d = 4+k2*r2
    d2 = d*d
    da = d+k2*r2
    db = 2*d-k2*r2
    a = r_minus*da
    b = r_minus*db
    ap = da+4*k2*r*r_minus
    bp = db+2*k2*r*r_minus
    common = 2*d+2*k2*r2
    ax = r_minus*(ap*r*d-a*common)
    bx = r_minus*(bp*r*d-b*common)
    off = (-4j)*s*k*r_minus*r3*d
    den = r4*d2
    if right:
        base = (4*r_minus*r-k2*r2-alpha**2*r_minus**2-alpha*r_minus*(2-r))*d2
        ell = ell0*den+alpha*r_minus*r2*d2
    else:
        base = r_minus*(k2*r3+4*r)*d2
        ell = ell0*den
    return den, [base+a*a-ax, off, off, base+b*b+bx], ell


def branch(s, massive, right, k):
    if not right:
        return s, s*0
    if not massive:
        return -s, s*0
    radicand = s*s + k*k
    if isinstance(s, acb):
        # On the selected lower-left continuation the square root is the
        # negative principal root, joined to the retarded branch across Re s=0.
        assert radicand.imag > 0 and s.imag < -k.real
        kappa = -radicand.sqrt()
        assert kappa.imag < 0
    else:
        kappa = -np.sqrt(radicand)
    return -kappa, k*k/(2*kappa)


def candidate(center, massive, right, cells, knot=None):
    n = 2 if massive else 1
    k = pi/5
    ell0, alpha = branch(center,massive,right,k)
    w = (1+1j)/sqrt(2)
    if right:
        grid = OUTER*np.linspace(1,0,cells+1)**3
        if knot is not None:
            grid = np.unique(np.r_[grid,knot])[::-1]
        initial = np.zeros(n*n,dtype=complex)
    else:
        grid = np.log(1.5)-(np.log(1.5)-np.log(INNER))*np.linspace(1,0,cells+1)**2
        den, numerator, _ = coefficients(1,1e-150,center,massive,right,k,ell0,alpha)
        initial = np.asarray(numerator,dtype=complex)/(1e-150*den*(1+2*center))

    def geometry(q):
        if right:
            dr = w*q
            return 2.5+dr,1.5+dr
        t = np.exp(q)
        return 1+t,t

    def data(q):
        r,t = geometry(q)
        den, numerator, ell = coefficients(r,t,center,massive,right,k,ell0,alpha)
        return r,t,np.asarray(numerator).reshape(n,n)/den,ell/den

    def rhs(q,state):
        r,t,Q,ell=data(q)
        p=state.reshape(n,n)
        if right:
            return (w*r/t*(Q-2*ell*p-p@p)).ravel()
        return (r*(Q/t-2*ell*p-t*(p@p))-p).ravel()

    sol=solve_ivp(rhs,(grid[0],grid[-1]),initial,t_eval=grid,rtol=4e-14,atol=1e-15,
                  method="DOP853")
    assert sol.success, sol.message
    result=[]
    for a,p in zip(grid,sol.y.T):
        r,t,Q,ell=data(a)
        if not right:
            p=p*t
        p=p.reshape(n,n)
        slope=r/t*(Q-2*ell*p-p@p)
        result.append((float(a) if right else float(t),p.ravel(),slope.ravel()))
    return result


def tail(s,massive,right,ell0,alpha):
    if not right:
        t=arb(INNER)*arb("1.0001")
        assert cap(s)<1 and arb("-.32")<s.real<0 and t<arb("1e-8")
        # On this horizon interval |Q|_F <= 100(r-1) and |dx/dlog(r-1)|=r.
        # The weighted Riccati integral is a contraction in |P|_F <=1000(r-1).
        k=(1+t)/(1+2*s.real.lower()*(1+t))
        assert k*(100+1000**2*t)<1000 and 2*k*1000*t<1
        return (1000*t).upper(), (1000*t*(1+t)).upper()
    R=arb(OUTER)
    fbound=1+1/R
    if not massive:
        q=fbound*(6/R**2+3/R**3)
    else:
        k2=K**2
        ratio=1/(1-4/(k2*R**2))
        diag=fbound*4/R**2+k2/R**2
        diag+= (fbound*(2+ratio)/R)**2
        diag+= fbound*((2+ratio)/R**3+fbound*(2+ratio+2*ratio**2)/R**2)
        diag+= (cap(alpha)*fbound/R)**2 + cap(alpha)*fbound*(1+2/R)/R**2
        off=4*cap(s)*K*fbound/(k2*R**2-4)
        q=2*(diag+off)
    attenuation=((-ell0*W).real-cap(ell0)/(R-1)-cap(alpha)/R).lower()
    assert attenuation>0
    kernel=(1+1/(R-1))/(2*attenuation)
    radius=(2*kernel*q).upper()
    assert 2*kernel*radius<1
    return radius, (radius*R*(1+1/(R-1))).upper()


def compact_bound(s,massive,right,cells,normalization,capture=None,knot=None):
    n=2 if massive else 1
    center=complex(float(s.real.mid()),float(s.imag.mid()))
    samples=candidate(center,massive,right,cells,knot)
    ell0,alpha=branch(s,massive,right,acb(K))
    if right:
        assert samples[0][0] == OUTER and samples[-1][0] == 0
    else:
        assert arb(samples[0][0]) <= arb(INNER)*arb("1.0001")
        assert samples[-1][0] == 1.5
    initial, tail_integral=tail(s,massive,right,ell0,alpha)

    def local_coefficients(r,r_minus):
        den,q,ell=coefficients(r,r_minus,s,massive,right,acb(K),ell0,alpha)
        fden=r_minus*r**3
        if massive:
            fden *= (4+acb(K)**2*r*r)**2
        return den,q,ell,fden

    return directed_flow(samples,n,right,ell0,alpha,local_coefficients,
                         initial,tail_integral,normalization,capture)


def directed_flow(samples,n,right,ell0,alpha,local_coefficients,
                  initial,tail_integral,normalization,capture=None,initial_is_error=False):
    """Certify candidate Riccati cells, optionally saving the local enclosures."""
    integral=acb(arb(0,tail_integral),arb(0,tail_integral))
    error=initial if initial_is_error else (initial+matrix_norm([acb(complex(x)) for x in samples[0][1]])).upper()
    peak=error
    for (a,pl,dl),(b,pr,dr) in zip(samples,samples[1:]):
        left=acb(a); step=acb(b)-left
        if right:
            left=arb("1.5")+W*left
            step=W*step
        r_minus=acb_poly([left,step])
        r=r_minus+1
        den,q,ell,fden=local_coefficients(r,r_minus)
        p=[]
        for l,u,v,w in zip(pl,pr,dl,dr):
            l,u,v,w=[acb(complex(q)) for q in (l,u,v,w)]
            diff=u-l
            p.append(acb_poly([l,step*v,3*diff-step*(2*v+w),-2*diff+step*(v+w)]))
        residual=[]
        for i in range(n):
            for j in range(n):
                k=i*n+j
                value=fden*p[k].derivative()*(1/step) + 2*ell*p[k]-q[k]
                value+=den*sum(p[i*n+a]*p[a*n+j] for a in range(n))
                residual.append(value(T)/den(T))
        # Derivative of tortoise position in the directed cell parameter.
        step_x=step*r(T)/r_minus(T)
        scale=cap(step_x)
        force=(scale*matrix_norm(residual)).upper()
        # Hermitian numerical abscissa via a Gershgorin bound.
        norm_bound=[]
        for i in range(n):
            bound=(-step_x*p[i*n+i](T)).real
            for j in range(n):
                if j!=i:
                    bound+=cap((-step_x*p[i*n+j](T)+(-step_x*p[j*n+i](T)).conjugate())/2)
            norm_bound.append(bound.upper())
        linear=(-2*step_x*ell(T)/den(T)).real.upper()+2*max(norm_bound)
        budget=(2*(error+force)*max(arb(1),linear.exp().upper())).upper()
        rate=(linear+scale*budget).upper()
        growth=rate.exp()
        if rate != 0:
            h=rate.expm1()/rate
        else:
            h=arb(1)
        # The scalar comparison solution is monotone in this cell.
        error=(growth*error+h*force).upper()
        assert error<budget, (right, n, len(samples)-1, budget,error,pl,pr)
        if capture is not None:
            capture.append((a,b,left,step,p,budget))
        if normalization:
            integral_error=(scale*budget).upper()
            integral+=step_x*p[0](T)+acb(arb(0,integral_error),arb(0,integral_error))
        peak=max(peak,error)
    end=[acb(complex(x)) for x in samples[-1][1]]
    ell=ell0+alpha*(1-1/R_MATCH)/R_MATCH
    end=[p+acb(arb(0,error),arb(0,error))+(ell if k//n==k%n else 0)
         for k,p in enumerate(end)]
    side = "right" if right else "left"
    print(f"  {side} endpoint error: {float(error):.7g}, peak: {float(peak):.7g}", flush=True)
    return end, integral


def matching(s,massive=False,normalization=False):
    left, left_int=compact_bound(s,massive,False,CELLS,normalization)
    right, right_int=compact_bound(s,massive,True,CELLS,normalization)
    return [a-b for a,b in zip(left,right)], left_int+right_int


def rw_determinant(s):
    return matching(s)[0][0]


def main():
    print("Python",platform.python_version(),"python-flint",flint.__version__,
          "NumPy",np.__version__,"SciPy",scipy.__version__)
    print("r0=1; cells per side:",CELLS,"; match radius:",R_MATCH,
          "; horizon offset:",INNER,"; far ray length:",OUTER)
    radius=arb("0.00000005")
    cauchy_radius=arb("0.005")
    step=arb("0.0004")
    print("Fundamental center",CENTER)
    center_value=rw_determinant(CENTER)
    print("  D(center) =",center_value)
    outer_bound=cap(rw_determinant(shift(CENTER,cauchy_radius)))
    print("  bound on |D| on Cauchy disk:",outer_bound)
    plus=rw_determinant(CENTER+step)
    minus=rw_determinant(CENTER-step)
    derivative_box=(plus-minus)/(2*step)
    derivative=acb(derivative_box.real.mid(),derivative_box.imag.mid())
    slope_error=(cap(derivative_box-derivative)+outer_bound*step**2/
                 (cauchy_radius**3*(1-(step/cauchy_radius)**2))).upper()
    small=radius/cauchy_radius
    remainder=(cap(center_value)+slope_error*radius+outer_bound*small**2/(1-small)).upper()
    comparison=abs(derivative)*radius
    assert remainder<comparison
    print("  comparison slope:",derivative,"slope error:",slope_error)
    print("  Rouche: remainder upper",remainder,"< linear modulus",comparison)
    print("  one simple RW zero inside radius",radius)
    disk=shift(CENTER,radius)
    print("Fundamental normalization throughout this disk")
    _, logarithm=matching(disk,normalization=True)
    slope_error+=outer_bound/cauchy_radius*((1-small)**-2-1)
    derivative_enclosure=derivative+acb(arb(0,slope_error),arb(0,slope_error))
    wronskian_derivative=-logarithm.exp()*derivative_enclosure
    wronskian_modulus = abs(wronskian_derivative)
    assert arb("3.64") < wronskian_modulus < arb("10.57")
    print("  W' at the simple zero =",wronskian_derivative)
    print("  |W'| lower",wronskian_modulus.lower(),"upper",wronskian_modulus.upper())

    print("Massless shift s - 2 Omega")
    shifted=rw_determinant(disk-2*OMEGA)
    assert shifted.imag>arb("0.28")
    print("  D(s - 2 Omega) =",shifted)
    print("Massive shift s - Omega")
    matrix,_=matching(disk-OMEGA,massive=True)
    determinant=matrix[0]*matrix[3]-matrix[1]*matrix[2]
    assert determinant.imag>arb("0.40")
    print("  det D(s - Omega) =",determinant)
    print("PASS isolated simple RW pole and noncoincident shifted pencils")


if __name__=="__main__":
    main()
