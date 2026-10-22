#!/usr/bin/env python3
"""Bounded non-production Laplace Green quadrature for normalized GL modes.

Uses the same three linear-mode configurations, not new physical k scans.
Horizon tensor contractions are evaluated in 60-digit arithmetic with an
8th-order regular Frobenius logarithmic derivative. These are convergence
estimates, not interval-certified enclosures of C0 or nonlinear amplitude.
"""
from pathlib import Path
import json
import platform
import numpy as np
import scipy
from scipy.integrate import solve_ivp, quad
import mpmath as mp
from gl_linear_mode import pair, potential, CONFIGS

HERE=Path(__file__).resolve().parent
mp.mp.dps=60


def scalar_basis(s,cfg,level):
    def rhs(v,Y):
        d=np.exp(v);r=1+d
        V=d/r**4
        return [r*(s*s+V-Y[0]*Y[0]),r*Y[0]]
    delta=np.exp(cfg['sleft']);R=[80.,120.,180.][level]
    kw=dict(method='DOP853',rtol=cfg['tol'],atol=cfg['tol']*.01,
            max_step=cfg['step'],dense_output=True)
    left=solve_ivp(rhs,[cfg['sleft'],np.log(R-1)],
          [s+delta/(1+2*s),0.],**kw)
    assert left.success
    pH,ellH=left.sol(0.);pR,ellR=left.y[:,-1]
    # Positive Volterra tail, replacing an uncontrolled truncated asymptotic
    # series. v=e^(s*x)y_inf obeys v=1+K v, ||K||_ordered<=J.
    E1,eerr=quad(lambda t:np.exp(-2*s*(t+np.log1p(t/(R-1))))/(R+t)**3,
                0.,np.inf,epsabs=1e-17,epsrel=2e-12)
    J=1/(4*s*R*R)
    K1=(1/(2*R*R)-E1)/(2*s)
    vlo=1+K1;vhi=vlo+np.expm1(J)-J
    elo=E1;ehi=np.exp(J)*E1
    factor=np.exp(ellR-ellH-s*(R+np.log(R-1)))
    minusWlo=factor*((s+pR)*vlo+elo)
    minusWhi=factor*((s+pR)*vhi+ehi)
    assert 0<minusWlo<=minusWhi
    return left,float(ellH),float(minusWlo),float(minusWhi),R,float(J),float(eerr)


def horizon_coeffs(k,om,n=8):
    k=mp.mpf(float(k));om=mp.mpf(float(om))
    def V(d):
        r=1+d;DD=1+k*k*r**3
        return k*k*d/r+(6*r-11)*d/r**4+18*d*d/(r**4*DD**2)-6*(4*r-5)*d/(r**4*DD)
    vv=mp.taylor(V,mp.mpf(0),n)
    c=[om]
    for j in range(1,n+1):
        pairs=sum(c[i]*c[j-i] for i in range(1,j))
        lower=sum(i*c[i]*(-1)**(j-i) for i in range(1,j))
        c.append((vv[j]-pairs-lower)/(j+2*om))
    return c


def tensor_norm(v,k,om,p,coeff):
    # Reconstruct with u=1; actual norm is u^2 times this expression.
    d=mp.exp(mp.mpf(float(v)));r=1+d;f=d/r
    k=mp.mpf(float(k));om=mp.mpf(float(om));DD=1+k*k*r**3
    pp=sum(cc*d**j for j,cc in enumerate(coeff)) if d<mp.mpf('.005') else mp.mpf(float(p))
    y=DD/r;yp=2*k*k*r-1/r**2+y*pp/f
    th=-r*r*y/(2*DD)-d*yp/(k*k*DD)
    tt=(d*(mp.mpf('.5')-k*k*r**3)/(r*DD**2)-om*om/(k*k*DD))*y
    tt+=(d/(2*k*k*r**3*DD)-3*d*d/DD**2)*yp
    tr=-om*(yp-y/(2*r*d))/(k*k*DD)
    KK=th/r**2;aa=tt/f
    norm=2*(aa-KK-tr)*(aa-KK+tr)+4*KK*KK
    return float(norm)


def transfer(k,om,cfg,level):
    left,right=pair(k,om,cfg,True)
    p0=float(left.y[0,-1]);DD=1+8*k*k
    y0=DD/2;yp0=4*k*k-.25+y0*p0/.5
    th0=-4*y0/(2*DD)-yp0/(k*k*DD)
    scale=8/th0
    cs=horizon_coeffs(k,om)
    sh,ellhm,Wlo,Whi,R,J,Eerr=scalar_basis(2*om,cfg,level)
    def components(v):
        sol=left if v<=0 else right
        p,ell=sol.sol(v);ell-=sol.y[1,-1]
        norm=tensor_norm(v,k,om,p,cs)*np.exp(2*ell)*scale*scale
        return norm,float(sh.sol(v)[1]-ellhm)
    def integrand(v):
        norm,ellh=components(v)
        d=np.exp(v);r=1+d
        return np.exp(ellh)*r*norm*d
    vmax=np.log(cfg['rmax']-1)
    splits=sorted(set([cfg['sleft'], -12.,-8.,-6.,-5.3,-4.,-2.,0.,1.,vmax]))
    splits=[v for v in splits if cfg['sleft']<=v<=vmax]
    pieces=[quad(integrand,lo,hi,epsabs=cfg['tol']*.01,epsrel=cfg['tol'],limit=100)
            for lo,hi in zip(splits[:-1],splits[1:])]
    integral=sum(q[0] for q in pieces);qerr=sum(q[1] for q in pieces)
    factor=np.sqrt(3)*k*k/16
    pref=factor/((Wlo+Whi)/2)
    probes=np.linspace(cfg['sleft'],vmax,1600)
    norms=np.array([components(v)[0] for v in probes])
    # Estimated missing horizon tail. Leading integrand in d is d^(4Omega).
    # Compare local slopes; this is an asymptotic estimate, not an interval bound.
    endpoint=integrand(cfg['sleft'])
    htail=endpoint/(1+4*om)
    result=dict(k=float(k),omega=float(om),C0=float(pref*integral),
      quadrature_error_estimate=float(abs(pref)*qerr),
      horizon_tail_leading_estimate=float(pref*htail),
      infinity_integrand_endpoint=float(pref*integrand(vmax)),
      min_H_squared_sample=float(np.min(norms)),max_H_squared_sample=float(np.max(norms)),
      normalization=float(scale*th0/8),
      C0_scalar_tail_bracket=sorted([float(factor*integral/Whi),float(factor*integral/Wlo)]),
      scalar_minus_W_bracket=[Wlo,Whi],scalar_tail_J=J,scalar_E1_quadrature_error=Eerr,
      scalar_rmax=R,config=cfg,
      scope='Floating convergence and asymptotic tail estimates, not rigorous interval certification')
    return result


def main():
    data=json.loads((HERE/'gl_linear_results.json').read_text())
    cases=[dict(L=10,k=data['k'],refinements=data['refinements'])]
    extension=HERE/'gl_linear_extended_results.json'
    if extension.exists():
        cases+=json.loads(extension.read_text())
    cases.sort(key=lambda case:case['L'])
    allresults=[]
    for case in cases:
        results=[]
        for level,mode in enumerate(case['refinements']):
            cfg={key:mode[key] for key in ['sleft','rmax','tol','step']}
            result=transfer(case['k'],mode['omega'],cfg,level)
            results.append(result);print(json.dumps(result),flush=True)
        allresults.append(dict(L=case['L'],k=case['k'],refinements=results))
    report=dict(python=platform.python_version(),numpy=np.__version__,scipy=scipy.__version__,
                mpmath=mp.__version__,cases=allresults)
    (HERE/'radion_transfer_results.json').write_text(json.dumps(report,indent=2)+'\n')


if __name__=='__main__':main()
