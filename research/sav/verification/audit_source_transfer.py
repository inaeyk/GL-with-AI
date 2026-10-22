#!/usr/bin/env python3
"""Independent scalar basis/profile quadrature; no physical GL ODE solves.

Uses the saved final L10 GL profile. Direct linear scalar ODEs replace the
main Riccati algorithm; the infinity normalization uses a first-Born Volterra
tail with an analytic remainder bound. A fixed saved mesh Simpson integral
does not constitute a physical-mode refinement run or interval enclosure.
"""
from pathlib import Path
import json
import platform
import numpy as np
import scipy
from scipy.integrate import solve_ivp, quad, simpson
import mpmath as mp

HERE=Path(__file__).resolve().parent


def main():
    mode=json.loads((HERE/'gl_linear_results.json').read_text())
    k=float(mode['k']); om=float(mode['refinements'][-1]['omega']); s=2*om
    tab=np.genfromtxt(HERE/'gl_linear_profile_L10.csv',delimiter=',',names=True)
    # Omit the cancellation-sensitive near-horizon segment rather than import
    # the main evaluator's Frobenius/norm algorithm.
    keep=(tab['r']-1>=1e-3)
    r=tab['r'][keep];v=np.log(r-1);x=r+v
    mp.mp.dps=60
    norms=[]
    for row in tab[keep]:
        rr=mp.mpf(float(row['r']));f=(rr-1)/rr
        tt,tr,rrc,th=[mp.mpf(float(row[n])) for n in ['Htt','Htr','Hrr','Htheta']]
        norms.append(float(tt*tt/f**2+f*f*rrc*rrc+2*th*th/rr**4-2*tr*tr))
    norms=np.array(norms)

    # Direct yH and yH,x, no Riccati/log-amplitude variables.
    vlo=-28.;dlo=np.exp(vlo)
    def rhs_h(v,Y):
        rr=1+np.exp(v);VV=(rr-1)/rr**4
        return [rr*Y[1],rr*(s*s+VV)*Y[0]]
    h=solve_ivp(rhs_h,[vlo,float(v[-1])],[1.,s+dlo/(1+2*s)],
                method='DOP853',rtol=2e-12,atol=2e-14,max_step=.04,dense_output=True)
    assert h.success
    hm,hpm=h.sol(0.)
    yh=h.sol(v)[0]/hm;ph=hpm/hm

    # yI=e^(-s*x)w; w_xx-2s*w_x-Vw=0, backwards stable.
    # Volterra w=1+T w, ||T|| <= eta=1/(4s R^2).
    # First-Born endpoint error <= eta^2/(1-eta), rather than an
    # imported inverse-r asymptotic-series normalization.
    R=1000.;xR=R+np.log(R-1)
    def kern_q(rr):
        dx=(rr-R)+np.log((rr-1)/(R-1))
        return -np.expm1(-2*s*dx)/(2*s*rr**3)
    def kern_p(rr):
        dx=(rr-R)+np.log((rr-1)/(R-1))
        return -np.exp(-2*s*dx)/rr**3
    bornq=quad(kern_q,R,np.inf,epsabs=1e-15,epsrel=2e-12)[0]
    bornp=quad(kern_p,R,np.inf,epsabs=1e-17,epsrel=2e-12)[0]
    eta=1/(4*s*R*R)
    def rhs_i(v,Y):
        rr=1+np.exp(v);VV=(rr-1)/rr**4
        return [rr*Y[1],rr*(2*s*Y[1]+VV*Y[0])]
    ii=solve_ivp(rhs_i,[np.log(R-1),0.],[1+bornq,bornp],
                 method='DOP853',rtol=2e-12,atol=2e-14,max_step=.04)
    assert ii.success
    w,wx=ii.y[:,-1];xm=2.
    yi=np.exp(-s*xm)*w;yip=np.exp(-s*xm)*(wx-s*w)
    W=yi*(yip/yi-ph) # yH normalized to one at matching radius r=2.
    pref=-np.sqrt(3)*k*k/(16*W)
    vals=yh*r*norms*(r-1)  # dr=(r-1)dv
    integral=simpson(vals,x=v)
    trap=np.trapezoid(vals,x=v)
    # Leading endpoint power estimate only; no uniform remainder bound here.
    beta=1+4*om
    horizon_est=float(vals[0]/beta)
    # Independent local analytic-power continuation estimated on this saved
    # mesh. It is diagnostic; no rigorous remainder bound is asserted.
    near=(r-1<.005)
    ds=r[near]-1
    fit=np.polynomial.polynomial.polyfit(ds,vals[near]/ds**beta,2)
    dcut=r[0]-1
    fitted_tail=sum(cc*dcut**(beta+j)/(beta+j) for j,cc in enumerate(fit))
    fit_resid=float(np.max(abs(np.polynomial.polynomial.polyval(ds,fit)
                              -vals[near]/ds**beta)))
    out=dict(python=platform.python_version(),numpy=np.__version__,scipy=scipy.__version__,
      mpmath=mp.__version__,k=k,omega=om,normalization_at_r2=float(
      tab['Htheta'][np.argmin(abs(tab['r']-2))]/8),
      algorithm='Saved GL profile; direct linear scalar yH and scaled yI; first-Born Volterra infinity tail; fixed-mesh Simpson',
      scalar_infinity_R=R,scalar_volterra_eta=eta,
      scalar_endpoint_w_first_born_error_bound=eta*eta/(1-eta),
      scalar_yinf_r2=float(yi),scalar_log_yinf_r2=float(np.log(yi)),
      scalar_pH_minus_pI=float(ph-yip/yi),W_r2_normalized_yH=float(W),
      min_profile_H2=float(norms.min()),max_profile_H2=float(norms.max()),
      omitted_horizon_delta=float(r[0]-1),
      C0_truncated_simpson=float(pref*integral),
      C0_truncated_trapezoid=float(pref*trap),
      C0_horizon_leading_estimate=float(pref*horizon_est),
      C0_simpson_plus_leading_horizon=float(pref*(integral+horizon_est)),
      C0_simpson_plus_fitted_horizon=float(pref*(integral+fitted_tail)),
      horizon_fit_absolute_residual_on_retained_mesh=fit_resid,
      main_C0_reference=12.3072919626396,
      scope='Independent floating-point check, not interval-certified GL mode, transfer, endpoint, or amplitude bound')
    print(json.dumps(out,indent=2))
    (HERE/'audit_source_transfer_results.json').write_text(json.dumps(out,indent=2)+'\n')


if __name__=='__main__':main()
