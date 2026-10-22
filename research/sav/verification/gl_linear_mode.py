#!/usr/bin/env python3
"""Bounded nonproduction GL ODE: one physical k, 3 refinements, threshold.

No time-domain spacetime simulation. Deterministic double-precision ODE
and scalar root solving. Outputs are exploratory convergence estimates,
not certified interval enclosures. Length unit r0=1.
"""
from pathlib import Path
import json
import platform
import numpy as np
import scipy
from scipy.integrate import solve_ivp
from scipy.optimize import brentq

HERE=Path(__file__).resolve().parent
K=2*np.pi/10
CONFIGS=[dict(sleft=-16.0,rmax=30.0,tol=2e-8,step=.2),
         dict(sleft=-22.0,rmax=45.0,tol=2e-10,step=.1),
         dict(sleft=-28.0,rmax=65.0,tol=2e-12,step=.05)]

def potential(r,k):
    D=1+k*k*r**3
    return k*k*(r-1)/r+(6*r-11)*(r-1)/r**4+18*(r-1)**2/(r**4*D**2)-6*(4*r-5)*(r-1)/(r**4*D)

def pair(k,om,cfg,dense=False):
    # p=u_{r*}/u, ell=log|u|, s=log(r-1). A nodeless mode is sought.
    def rhs(s,x):
        r=1+np.exp(s)
        return [r*(potential(r,k)+om*om-x[0]**2),r*x[0]]
    q=np.hypot(k,om);a1=k*k/(2*q)
    a2=(-a1-6+a1*a1)/(2*q)
    a3=(a1-2*a2+17+2*a1*a2)/(2*q)
    R=cfg['rmax'];e=np.exp(cfg['sleft'])
    h1=(k*k-5+6/(1+k*k))/(1+2*om)
    pleft=om+h1*e
    pright=-q+a1/R+a2/R**2+a3/R**3
    opts=dict(method='DOP853',rtol=cfg['tol'],atol=cfg['tol']*.01,
              max_step=cfg['step'],dense_output=dense)
    left=solve_ivp(rhs,[cfg['sleft'],0],[pleft,0],**opts)
    right=solve_ivp(rhs,[np.log(R-1),0],[pright,0],**opts)
    if not (left.success and right.success):
        raise RuntimeError((left.message,right.message,k,om))
    return left,right

def mismatch(k,om,cfg):
    left,right=pair(k,om,cfg)
    return left.y[0,-1]-right.y[0,-1]

def solve_reference(cfg,k=K):
    om=brentq(lambda x:mismatch(k,x,cfg),0,.1,xtol=5e-14,rtol=1e-13)
    return dict(cfg,omega=om,matching_residual=mismatch(k,om,cfg))

def profile(om,cfg,k=K,label=10):
    left,right=pair(k,om,cfg,True)
    ss=np.unique(np.r_[np.linspace(cfg['sleft'],-8,180),np.linspace(-8,0,320),np.linspace(0,np.log(cfg['rmax']-1),500)])
    rr=1+np.exp(ss)
    vals=np.empty((2,len(ss)))
    for mask,sol in [(ss<=0,left),(ss>0,right)]:
        vals[:,mask]=sol.sol(ss[mask])
        vals[1,mask]-=sol.y[1,-1]
    pp,ell=vals
    uu=np.exp(ell)
    D=1+k*k*rr**3;f=(rr-1)/rr;w=D/rr
    wwprime=2*k*k*rr-1/rr**2
    yy=w*uu;yp=(wwprime+w*pp/f)*uu
    htheta=-rr*rr*yy/(2*D)-(rr-1)*yp/(k*k*D)
    # Invariant normalization delta R^U/r=Htheta/(2r^2)=1 at r=2.
    idx=np.flatnonzero(ss==0)[0]
    scale=1/(htheta[idx]/(2*rr[idx]**2))
    yy*=scale;yp*=scale;htheta*=scale
    a=(rr-1)*(.5-k*k*rr**3)/(rr*D*D)-om*om/(k*k*D)
    b=(rr-1)/(2*k*k*rr**3*D)-3*(rr-1)**2/(D*D)
    htt=a*yy+b*yp
    htr=-om*(yp-yy/(2*rr*(rr-1)))/(k*k*D)
    hrr=htt/f**2-2*htheta/(rr*(rr-1))
    contraction=htt**2/f**2+f**2*hrr**2+2*htheta**2/rr**4-2*htr**2
    # The raw Lorentz contraction loses digits very near the horizon. Flag it.
    good=rr-1>=1e-5
    contraction[~good]=np.nan
    source=np.sqrt(3)*k*k*rr*f*contraction/16
    table=np.c_[rr,rr+np.log(rr-1),yy,yp,htt,htr,hrr,htheta,pp,ell,contraction,source]
    np.savetxt(HERE/('gl_linear_profile_L'+str(label)+'.csv'),table,delimiter=',',
       header='r,rstar,y,yp,Htt,Htr,Hrr,Htheta,p,log_u_relative_match,H_squared_raw,S_raw',comments='')
    return dict(scale=scale,normalization=float(htheta[idx]/(2*rr[idx]**2)),
                p_match=float(pp[idx]),min_H_squared_reliable=float(np.min(contraction[good])),
                max_H_squared_reliable=float(np.max(contraction[good])),
                note='Raw contraction is deliberately NaN for r-1<1e-5; near-horizon analytic cancellation required.')

def main():
    import sys
    if '--extend' in sys.argv:
        extended=[]
        for label in [8,12,16]:
            k=2*np.pi/label
            refinements=[]
            for cfg in CONFIGS:
                result=solve_reference(cfg,k);refinements.append(result)
                print(json.dumps(dict(kind='extension',L=label,k=k,**result)),flush=True)
            extended.append(dict(L=label,k=k,refinements=refinements,
                            profile=profile(refinements[-1]['omega'],CONFIGS[-1],k,label)))
        (HERE/'gl_linear_extended_results.json').write_text(json.dumps(extended,indent=2)+'\n')
        return
    results=[]
    for cfg in CONFIGS:
        result=solve_reference(cfg);results.append(result)
        print(json.dumps(dict(kind='reference',k=K,**result)),flush=True)
    # One independent continuum-threshold check. Not a spectral scan.
    cfg=CONFIGS[1]
    critical=brentq(lambda kk:mismatch(kk,0,cfg),.85,.9,xtol=5e-14,rtol=1e-13)
    report=dict(python=platform.python_version(),numpy=np.__version__,scipy=scipy.__version__,
                k=K,refinements=results,kcritical=critical,
                critical_residual=mismatch(critical,0,cfg),
                profile=profile(results[-1]['omega'],CONFIGS[-1]))
    (HERE/'gl_linear_results.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(dict(kind='threshold',kcritical=critical,residual=report['critical_residual'])),flush=True)
    print(json.dumps(report['profile']),flush=True)

if __name__=='__main__':main()
