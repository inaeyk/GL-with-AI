#!/usr/bin/env python3
"""L10 saved-profile scalar Green response and finite-slice energy control.
Three declared 1D scalar configurations only. No GL root or spacetime run.
Floating evidence, not interval enclosures. Imports no primary scalar basis.
"""
from pathlib import Path
import json,platform
import numpy as np
import scipy
import mpmath as mp
from scipy.interpolate import CubicSpline
from scipy.integrate import solve_ivp,cumulative_simpson,simpson,quad
HERE=Path(__file__).resolve().parent
mp.mp.dps=65
data=json.loads((HERE/'gl_linear_results.json').read_text())
k=data['k'];om=data['refinements'][-1]['omega'];s=2*om;scale=data['profile']['scale']
a=np.genfromtxt(HERE/'gl_linear_profile_L10.csv',delimiter=',',names=True)
vv=np.log(a['r']-1)
# Retain reliable coordinate range. Endpoint roundoff is exposed in output.
ps=CubicSpline(vv,a['p']);ells=CubicSpline(vv,a['log_u_relative_match'])
km=mp.mpf(k);ommp=mp.mpf(om)
def vgl(d):
 r=1+d;D=1+km*km*r**3
 return km*km*d/r+(6*r-11)*d/r**4+18*d*d/(r**4*D**2)-6*(4*r-5)*d/(r**4*D)
vc=mp.taylor(vgl,mp.mpf(0),8);pc=[ommp]
for j in range(1,9):
 pc.append((vc[j]-sum(pc[i]*pc[j-i] for i in range(1,j))-sum(i*pc[i]*(-1)**(j-i) for i in range(1,j)))/(j+2*ommp))
def regular_tensor(v):
 d=mp.exp(mp.mpf(float(v)));r=1+d;f=d/r;DD=1+km*km*r**3
 p=sum(c*d**j for j,c in enumerate(pc)) if d<mp.mpf('.005') else mp.mpf(float(ps(v)))
 u=mp.mpf(scale)*mp.exp(mp.mpf(float(ells(v))))
 y=DD/r*u;yp=(2*km*km*r-1/r**2)*u+y*p/f
 T=(d*(mp.mpf('.5')-km*km*r**3)/(r*DD**2)-ommp*ommp/(km*km*DD))*y+(d/(2*km*km*r**3*DD)-3*d*d/DD**2)*yp
 B=-ommp*(yp-y/(2*r*d))/(km*km*DD)
 K=-y/(2*DD)-d*yp/(km*km*DD*r*r)
 RR=T/f**2-2*K/f
 # tau=v_EF-r=t+log(delta), so slice perturbation factor is delta^-Omega.
 mult=mp.exp(-ommp*mp.mpf(float(v)))
 ef=np.array([[float(mult*T),float(mult*(B-T/f))],[float(mult*(B-T/f)),float(mult*(RR-2*B/f+T/f**2))]])
 frame=np.array([[1,(float(f)/2-1)],[1,(float(f)/2+1)]])/np.sqrt(2)
 block=frame@ef@frame.T
 norm=float(mp.sqrt(sum(mp.mpf(float(x))**2 for x in block.flat)+2*(mult*K)**2))
 H2=2*(T/f-K-B)*(T/f-K+B)+4*K*K
 return float(H2),norm

# Source/norm computed once on fixed saved-profile subdivisions, no mode re-solve.
vsrc=np.unique(np.r_[np.linspace(-28,-6,1300),np.linspace(-6,0,1700),np.linspace(0,np.log(64),1700)])
raw=np.array([regular_tensor(v) for v in vsrc])
hs=CubicSpline(vsrc,raw[:,0])
def source(v):
 d=np.exp(v);r=1+d
 return np.where(r<=65,np.sqrt(3)*k*k*d*hs(np.minimum(v,np.log(64)))/16,0.)
results=[]
configs=[(-16.,80.,4001,2e-8,.2),(-22.,120.,8001,2e-10,.1),(-28.,180.,16001,2e-12,.05)]
for lo,R,n,tol,step in configs:
 hi=np.log(R-1)
 def rhs(v,Y):
  r=1+np.exp(v);V=(r-1)/r**4
  return [r*Y[1],r*(s*s+V)*Y[0]]
 xlo=1+np.exp(lo)+lo
 L=solve_ivp(rhs,[lo,hi],[np.exp(s*xlo),(s+np.exp(lo)/(1+2*s))*np.exp(s*xlo)],method='DOP853',rtol=tol,atol=tol*1e-4,max_step=step,dense_output=True)
 xR=R+hi
 E1,err=quad(lambda t:np.exp(-2*s*(t+np.log1p(t/(R-1))))/(R+t)**3,0,np.inf,epsabs=1e-18)
 K1=(1/(2*R**2)-E1)/(2*s);wR=1+K1;dwR=-E1
 RR=solve_ivp(rhs,[hi,lo],[np.exp(-s*xR)*wR,np.exp(-s*xR)*(dwR-s*wR)],method='DOP853',rtol=tol,atol=tol*1e-13,max_step=step,dense_output=True)
 assert L.success and RR.success
 v=np.linspace(lo,hi,n);r=1+np.exp(v);x=r+v;f=(r-1)/r;V=f/r**3
 yl,pl=L.sol(v);yr,pr=RR.sol(v);l0,p0=L.sol(0);r0,pr0=RR.sol(0);W=l0*pr0-p0*r0
 forcing=source(v);il=cumulative_simpson(yl*forcing*r,x=v,initial=0)
 # Integration over increasing -v gives the positive right-hand integral.
 ir=cumulative_simpson((yr*forcing*r)[::-1],x=-v[::-1],initial=0)[::-1]
 q=-(yr*il+yl*ir)/W;qx=-(pr*il+pl*ir)/W
 C=-il[-1]/W
 # Data mismatch: particular exponential versus zero scalar data on tau=0.
 Fslice=np.exp(-s*v)*q;Pt=s*Fslice
 Fder=np.exp(-s*v)*(qx-s/r*q)
 density=.5*((1-1/r**2)*Pt**2+Fder**2+V*Fslice**2)
 Qslice=simpson(density*r,x=v)
 # Compare known C0; convergence does not include saved-profile error.
 analyticJ=1/(4*s*R*R)
 normalized=np.sqrt(max(0,Qslice))
 tolerance=np.sqrt(1.1)-1
 # u1=0,u2=T; this includes ALL possible transient news in the bound.
 T=np.log1p(2*Qslice/(tolerance*tolerance*s*C*C))/(2*s)
 results.append(dict(left_log_delta=lo,outer_r=R,grid=n,tol=tol,C0=C,Q_unit_mismatch=Qslice,
  Q_horizon_omission_leading=float(density[0]*r[0]),Q_infinity_endpoint=float(density[-1]*r[-1]),
  scalar_Volterra_first_Born_w_error_bound=float(np.expm1(analyticJ)-analyticJ),
  scalar_Volterra_derivative_error_bound=float(np.expm1(analyticJ)*E1),
  scalar_E1_quadrature_error_estimate=float(err),
  max_relative_W_drift=float(np.max(np.abs((yl*pr-pl*yr)/W-1))),
  time_window_u1_0_u2_T_for_10percent_transient_only=float(T),
  zero_data_trace_bound_sqrtQ=normalized))
 print(json.dumps(results[-1]),flush=True)
out=dict(python=platform.python_version(),numpy=np.__version__,scipy=scipy.__version__,mpmath=mp.__version__,L=10,k=k,Omega=om,
 first_order_regular_frame_sample_max=float(raw[:,1].max()),first_order_max_at_r=float(1+np.exp(vsrc[np.argmax(raw[:,1])])),
 regular_frame_horizon_sample=float(raw[0,1]),configs=results,
 scope='Empirical saved-profile Green reconstruction and conditional homogeneous-data energy bound; not nonlinear GL data completion, continuum interval or A_star. No other physical k or root solve.')
(HERE/'goal002_scalar_response_results.json').write_text(json.dumps(out,indent=2)+'\n')
