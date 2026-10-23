#!/usr/bin/env python3
"""GOAL004 bounded non-production physical L10 source/Green diagnostics.
Uses saved empirical mode; no root solve, no spacetime evolution, no certification.
Three new n=2 scalar inverse configurations at most. Radial jets satisfy the
linear GL equation locally. Source route reuses the GOAL002 W=R^2 identities;
this primary investigation is not an independent audit of those identities.
"""
from pathlib import Path
import json, platform, hashlib, math, sys
import numpy as np
import scipy
from scipy.interpolate import CubicSpline
from scipy.integrate import solve_ivp, cumulative_simpson, simpson
import mpmath as mp
HERE=Path(__file__).resolve().parent
mp.mp.dps=65
N=3
D0=json.loads((HERE/'gl_linear_results.json').read_text())
k=float(D0['k']);om=float(D0['refinements'][-1]['omega']);rate=2*om
km=mp.mpf(str(k));wm=mp.mpf(str(om));k2=km*km
rows=np.genfromtxt(HERE/'gl_linear_profile_L10.csv',delimiter=',',names=True)
ss=np.log(rows['r']-1)
ps=CubicSpline(ss,rows['p']);ells=CubicSpline(ss,rows['log_u_relative_match'])
scale=mp.mpf(str(D0['profile']['scale']))

def potential(r,k):
 d=r-1;D=1+k*k*r**3
 return k*k*d/r+(6*r-11)*d/r**4+18*d*d/(r**4*D**2)-6*(4*r-5)*d/(r**4*D)
# Horizon p series is generated directly from f p_r = V+Omega^2-p^2.
vc=mp.taylor(lambda d:potential(1+d,km),mp.mpf('0'),18)
pc=[wm]
for n in range(1,19):
 pc.append((vc[n]-sum(pc[i]*pc[n-i] for i in range(1,n))-sum(i*pc[i]*(-1)**(n-i) for i in range(1,n)))/(n+2*wm))
# v=e^-Omega*x u, with v'/v=(p-Omega)/f.
logcoef=[pc[i+1]+(pc[i] if i else 0) for i in range(18)]
vcoef=[mp.mpf(1)]
for n in range(1,19):vcoef.append(sum(logcoef[j]*vcoef[n-1-j] for j in range(n))/n)
dmatch=mp.mpf('.005');rmatch=1+dmatch
vexact=scale*mp.exp(mp.mpf(float(ells(float(mp.log(dmatch)))))-wm*(rmatch+mp.log(dmatch)))
vnorm=vexact/sum(c*dmatch**i for i,c in enumerate(vcoef))
ah=vnorm*(1+k2)*mp.exp(wm) # static y~a_h delta^Omega

class J:
 def __init__(self,x=0):
  self.c=list(x) if isinstance(x,(list,tuple,np.ndarray)) else [mp.mpc(x)]+[mp.mpc(0)]*N
  self.c=(self.c+[mp.mpc(0)]*(N+1))[:N+1]
 def __add__(self,x):
  if not isinstance(x,J):x=J(x)
  return J([a+b for a,b in zip(self.c,x.c)])
 __radd__=__add__
 def __neg__(self):return J([-a for a in self.c])
 def __sub__(self,x):return self+-x if isinstance(x,J) else self+(-x)
 def __rsub__(self,x):return -self+x
 def __mul__(self,x):
  if not isinstance(x,J):x=J(x)
  return J([sum(self.c[j]*x.c[i-j] for j in range(i+1)) for i in range(N+1)])
 __rmul__=__mul__
 def inv(self):
  z=[1/self.c[0]]
  for i in range(1,N+1):z.append(-sum(self.c[j]*z[i-j] for j in range(1,i+1))/self.c[0])
  return J(z)
 def __truediv__(self,x):return self*(x.inv() if isinstance(x,J) else 1/x)
 def __rtruediv__(self,x):return self.inv()*x
 def __pow__(self,n):
  if n<0:return self.inv()**(-n)
  out=J(1)
  for i in range(n):out=out*self
  return out
 def der(self):return J([(i+1)*self.c[i+1] for i in range(N)]+[0])

class F:
 def __init__(self,d=None,o=0):self.d={n:(a if isinstance(a,J) else J(a)) for n,a in (d or {}).items()};self.o=o
 def __add__(self,x):
  if x==0:return self
  if not isinstance(x,F):x=F({0:x})
  if not self.d:return x
  if not x.d:return self
  assert self.o==x.o,(self.o,x.o)
  return F({n:self.d.get(n,J())+x.d.get(n,J()) for n in self.d.keys()|x.d.keys()},self.o)
 __radd__=__add__
 def __neg__(self):return F({n:-a for n,a in self.d.items()},self.o)
 def __sub__(self,x):return self+-x
 def __rsub__(self,x):return -self+x
 def __mul__(self,x):
  if not isinstance(x,F):return F({n:a*x for n,a in self.d.items()},self.o)
  z={}
  for n,a in self.d.items():
   for m,b in x.d.items():z[n+m]=z.get(n+m,J())+a*b
  return F(z,self.o+x.o)
 __rmul__=__mul__
 def __truediv__(self,x):return self*(1/x)
 def der(self,c):
  return F({n:(a.der() if c==1 else a*(wm*self.o if c==0 else 1j*km*n)) for n,a in self.d.items()},self.o)
 def val(self,n=0,j=0):return self.d.get(n,J()).c[j]*math.factorial(j)

Z=F()
def bg(x):return F({0:x})
def mode(x):return F({-1:x/2,1:x/2},1)

def regular_jets(r0):
 r=J([r0,1]);d=r-1;f=d/r;D=1+k2*r**3
 if r0<mp.mpf('1.01'):
  v=sum(vcoef[i]*d**i for i in range(len(vcoef)))*vnorm
  Y=D/r*v
  Yp=Y.der() # Y last coeff cannot be used after >2 derivatives; replace below.
  # Differentiate exact polynomial before truncating, avoiding lost top coefficient.
  vv=sum((i*vcoef[i])*d**(i-1) for i in range(1,len(vcoef)))*vnorm
  Yp=(2*k2*r-1/r**2)*v+D/r*vv
 else:
  sval=float(mp.log(r0-1));p=mp.mpf(float(ps(sval)))
  Y0=D.c[0]/r0*scale*mp.exp(mp.mpf(float(ells(sval)))-wm*(r0+mp.log(r0-1)))
  Y1=Y0*((2*k2*r0-1/r0**2)/(D.c[0]/r0)+(p-wm)/f.c[0])
  # regular equation f Y''+b Y'+cY=0; Taylor recurrence needs N+1.
  exN=N
  P=1/d-5/r+6/(r*D)
  b=2*wm+f*P;c=wm*(-4+6/D)/r+3/r**3-k2-3/(r**3*D)
  yc=[Y0,Y1]
  for n in range(N):
   rest=sum(f.c[j]*(n-j+2)*(n-j+1)*yc[n-j+2] for j in range(1,n+1))
   rest+=sum(b.c[j]*(n-j+1)*yc[n-j+1] for j in range(n+1))
   rest+=sum(c.c[j]*yc[n-j] for j in range(n+1))
   yc.append(-rest/(f.c[0]*(n+2)*(n+1)))
  Y=J(yc);Yp=J([(i+1)*yc[i+1] for i in range(N+1)])
 staticp=Yp+wm*Y/f
 T=(d*(mp.mpf('.5')-k2*r**3)/(r*D**2)-wm**2/(k2*D))*Y+(d/(2*k2*r**3*D)-3*d*d/D**2)*staticp
 B=-wm*(staticp-Y/(2*r*d))/(k2*D)
 S=-r*r*Y/(2*D)-d*staticp/(k2*D)
 RR=2*T/f**2-2*B/f-2*S/(r*d)
 return r,f,T,B-T/f,RR,S,Y

def source_at(r0,checks=False):
 r,f,T,B,Cc,S,Y=regular_jets(mp.mpf(str(r0)))
 g=[[bg(-f),Z,Z],[Z,Z,Z],[Z,Z,bg(1)]];g[0][1]=g[1][0]=bg(1)
 gi=[[Z,bg(1),Z],[bg(1),bg(f),Z],[Z,Z,bg(1)]]
 h=[[mode(T),mode(B),Z],[mode(B),mode(Cc),Z],[Z,Z,Z]];w=mode(S)
 G={}
 for c in range(3):
  for a in range(3):
   for b in range(3):G[c,a,b]=sum(gi[c][e]*(g[e][b].der(a)+g[e][a].der(b)-g[a][b].der(e))/2 for e in range(3))
 hm=[[sum(gi[a][c]*h[c][b] for c in range(3)) for b in range(3)] for a in range(3)]
 hu=[[sum(hm[a][c]*gi[c][b] for c in range(3)) for b in range(3)] for a in range(3)]
 def nab(d,b,a):return h[d][b].der(a)-sum(h[e][b]*G[e,a,d]+h[d][e]*G[e,a,b] for e in range(3))
 CC={(c,a,b):sum(gi[c][e]*(nab(e,b,a)+nab(e,a,b)-nab(a,b,e))/2 for e in range(3)) for c in range(3) for a in range(3) for b in range(3)}
 DD={(c,a,b):-sum(hm[c][e]*CC[e,a,b] for e in range(3)) for c in range(3) for a in range(3) for b in range(3)}
 def riclin(a,b,X):
  v=sum(X[c,a,b].der(c)-X[c,a,c].der(b) for c in range(3))
  return v+sum(X[c,c,e]*G[e,a,b]+X[e,a,b]*G[c,c,e]-X[c,b,e]*G[e,a,c]-X[e,a,c]*G[c,b,e] for c in range(3) for e in range(3))
 W=bg(r*r);dw=[Z,bg(2*r),Z]
 H0={(a,b):bg(2 if a==b==1 else 0)-G[1,a,b]*2*r for a in range(3) for b in range(3)}
 H1={(a,b):w.der(b).der(a)-sum(w.der(c)*G[c,a,b]+CC[c,a,b]*dw[c] for c in range(3)) for a in range(3) for b in range(3)}
 H2={(a,b):-sum(DD[c,a,b]*dw[c]+CC[c,a,b]*w.der(c) for c in range(3)) for a in range(3) for b in range(3)}
 src={};lins=[]
 for a in range(3):
  for b in range(a,3):
   lin=riclin(a,b,CC)-H1[a,b]/r**2+w*H0[a,b]/r**4+(w.der(a)*dw[b]+w.der(b)*dw[a])/(2*r**4)-w*dw[a]*dw[b]/r**6
   lins.append(abs(lin.val(1)))
   vv=riclin(a,b,DD)+sum(CC[c,c,e]*CC[e,a,b]-CC[c,b,e]*CC[e,a,c] for c in range(3) for e in range(3))
   vv+=-H2[a,b]/r**2+w*H1[a,b]/r**4-w*w*H0[a,b]/r**6
   vv+=w.der(a)*w.der(b)/(2*r**4)-w*(w.der(a)*dw[b]+w.der(b)*dw[a])/r**6+w*w*dw[a]*dw[b]*3/(2*r**8)
   src[a,b]=src[b,a]=vv
 qs=-sum(H2[a,b]*gi[a][b]-hu[a][b]*H1[a,b]+sum(hm[a][c]*hu[c][b] for c in range(3))*H0[a,b] for a in range(3) for b in range(3))/2
 lins.append(abs((-sum(H1[a,b]*gi[a][b]-hu[a][b]*H0[a,b] for a in range(3) for b in range(3))/2).val(1)))
 Hsq=2*T*Cc+2*B*B+4*f*B*Cc+f*f*Cc*Cc+2*S*S/r**4
 zz0=src[2,2].val();zz2=src[2,2].val(2)
 scalar_error=max(abs(zz0+k2*Hsq.c[0]/8),abs(zz2+3*k2*Hsq.c[0]/16))
 assert scalar_error < mp.mpf('1e-25'), ('scalar',r0,scalar_error)
 trace=sum(src[a,b]*gi[a][b] for a in range(3) for b in range(3))+qs*2/r**2
 E={(a,b):src[a,b]-trace*g[a][b]/2 for a in range(3) for b in range(3)}
 Es=qs-trace*r*r/2
 Em={(a,b):sum(gi[a][c]*E[c,b] for c in range(3)) for a in range(3) for b in range(3)}
 bian=[]
 for b in range(3):
  div=sum(Em[a,b].der(a) for a in range(3))+sum(Em[e,b]*G[a,a,e]-Em[a,e]*G[e,a,b] for a in range(3) for e in range(3))+Em[1,b]*2/r-(Es*2/r**3 if b==1 else Z)
  bian.extend([abs(div.val(0)),abs(div.val(2))])
 a2=4*k2;s2=2*wm;D2=1+a2*r**3
 tau=src[2,2]*(-2/a2);j=src[0,2]*(-1/(1j*km));jr=src[1,2]*(-1/(1j*km))
 Jj=-jr*(2*a2*r*r*f)-qs*(4*a2*r)-tau*a2+j.der(1)*(2*a2*r*r*f/s2)
 Jj+=(src[0,1]*f+src[0,0])*(4*a2*r*r*(a2*r**3-2)/(s2*D2))
 Jj-=j*(2*a2*(a2*r**3*(2*r-3)-4*r+3)/(s2*D2))
 x=float(r0+np.log(r0-1));Fhat=Jj*(-f*r/D2)
 # Einstein zero-sector T=-Q+g trace4+zz /2.
 # Q^alpha_alpha+Qzz equals full trace, so T=-E restricted to 4D.
 Trr=-E[1,1].val();Tvr=-E[0,1].val()
 # Qrr^EF, Qvr^EF; T^v_v=T_rv in background EF.
 out=[float(mp.re(Fhat.val(2))),float(mp.re(Jj.val(2))),float(mp.re(tau.val(2))),float(mp.re(j.val(2))),float(mp.re(jr.val(2))),float(mp.re(src[0,0].val(2))),float(mp.re(src[0,1].val(2))),float(mp.re(src[1,1].val(2))),float(mp.re(qs.val(2))),float(mp.re(Trr)),float(mp.re(Tvr)),float(mp.re(Hsq.c[0]))]
 return out,dict(r=float(r0),linear_max=float(max(lins)),scalar_source_error=float(scalar_error),Bianchi_max=float(max(bian)),ah=float(ah))

def main():
 if '--probe' in sys.argv:
  for r in [1.000001,1.005,1.1,2.,4.,20.]:print(r,source_at(r),flush=True)
  return
 # Fixed source quadrature only; changes of source interpolation are diagnosed
 # by nested Simpson subsamples, not extra boundary-value configurations.
 v=np.unique(np.r_[np.linspace(-18,-5,81),np.linspace(-5,0,161),np.linspace(0,np.log(64),241)])
 src=[];checks=[]
 for i,vv in enumerate(v):
  row,ch=source_at(1+np.exp(vv));src.append(row)
  if i in [0,80,140,240,len(v)-1]:checks.append(ch)
  if i%80==0:print('SOURCE_POINT',i,len(v),flush=True)
 arr=np.array(src);r=1+np.exp(v);x=r+v;f=(r-1)/r
 np.savez_compressed(HERE/'goal004_source_geometry_samples.npz',v=v,r=r,source=arr)
 spl=[CubicSpline(v,arr[:,j]) for j in range(arr.shape[1])]
 # n=0 fixed-charge quadratures, common e^{s v_EF} stripped.
 beta=.5*cumulative_simpson((r*arr[:,9])[::-1],x=-r[::-1],initial=0)[::-1]*-1
 mu=.5*cumulative_simpson((r*r*arr[:,10])[::-1],x=-r[::-1],initial=0)[::-1]
 # Full physical n=2 F in static time after EF factor restoration.
 Fs=arr[:,0]*np.exp(rate*x)
 Fnorm=np.sqrt(simpson(Fs*Fs*r,x=v))
 a2=(2*k)**2;alpha=np.hypot(2*k,rate);beta2=(a2+2*rate*rate)/(2*alpha)
 configs=[(-16.,30.,4001,2e-8,.2),(-18.,45.,8001,2e-10,.1),(-18.,65.,16001,2e-12,.05)]
 reports=[]
 for lo,R,ng,tol,step in configs:
  hi=np.log(R-1)
  def rhs(vv,z):
   rr=1+np.exp(vv)
   return [rr*z[1],rr*(potential(rr,2*k)+rate*rate)*z[0]]
  d=np.exp(lo);xl=1+d+lo;v1=a2-5+6/(1+a2);p1=v1/(1+2*rate)
  left=solve_ivp(rhs,[lo,hi],[np.exp(rate*xl),(rate+p1*d)*np.exp(rate*xl)],method='DOP853',rtol=tol,atol=tol*1e-6,max_step=step,dense_output=True)
  # Decaying infinity jet p=-alpha+a1/r+a2/r²+a3/r³.
  b1=a2/(2*alpha);b2=(-b1-6+b1*b1)/(2*alpha);b3=(b1-2*b2+17+2*b1*b2)/(2*alpha)
  pR=-alpha+b1/R+b2/R**2+b3/R**3
  uR=np.exp(-alpha*R)*R**(-beta2)
  right=solve_ivp(rhs,[hi,lo],[uR,pR*uR],method='DOP853',rtol=tol,atol=tol*1e-40,max_step=step,dense_output=True)
  assert left.success and right.success
  vv=np.linspace(lo,hi,ng);rr=1+np.exp(vv);xx=rr+vv;ff=(rr-1)/rr
  uH,pH=left.sol(vv);uI,pI=right.sol(vv)
  h0,ph0=left.sol(0);i0,pi0=right.sol(0);W=h0*pi0-ph0*i0
  forcing=spl[0](vv)*np.exp(rate*xx)
  intL=cumulative_simpson(uH*forcing*rr,x=vv,initial=0)
  intR=cumulative_simpson((uI*forcing*rr)[::-1],x=-vv[::-1],initial=0)[::-1]
  u=-(uI*intL+uH*intR)/W;ux=-(pI*intL+pH*intR)/W
  Y=(1+a2*rr**3)/rr*u*np.exp(-rate*xx)
  Yprime=((2*a2*rr-1/rr**2)*u+(1+a2*rr**3)/rr*(ux-rate*u)/ff)*np.exp(-rate*xx)
  J=spl[1](vv);tau=spl[2](vv);j=spl[3](vv);jr=spl[4](vv);A=spl[5](vv);B=spl[6](vv);Qrr=spl[7](vv);Qs=spl[8](vv)
  DD=1+a2*rr**3;P=1/(rr-1)-5/rr+6/(rr*DD);bb=2*rate+ff*P;cc=rate*(-4+6/DD)/rr+3/rr**3-a2-3/(rr**3*DD)
  Ypp=(J-bb*Yprime-cc*Y)/ff
  term=ff*Yprime+(rate-1/(2*rr**2))*Y
  lift=ff*j-rr*ff*B-rr*A
  Z=-rate*term/(a2*DD)+2*rr*rr/DD*lift
  def drcol(col):return spl[col].derivative()(vv)/(rr-1)
  termp=Ypp*ff+(1/rr**2+rate-1/(2*rr**2))*Yprime+Y/rr**3
  liftp=drcol(3)*ff+j/rr**2-(ff+1/rr)*B-rr*ff*drcol(6)-A-rr*drcol(5)
  DDp=3*a2*rr*rr
  Zp=-rate/a2*(termp/DD-term*DDp/DD**2)+(4*rr/DD-2*rr*rr*DDp/DD**2)*lift+2*rr*rr/DD*liftp
  S=rr*Z/rate-Y/(2*a2*rr);vr=-Zp/rate-2*Z/(rate*rr)+j/rate+tau
  T=Z-ff*vr;Nn=2*Zp/rate+2*Z/(rate*rr)-2*j/rate-tau+Y/(a2*rr**3)
  RR=Nn/ff
  # Avoid endpoint division contamination; no global norm claim. Physical
  # tetrad on tau=0, with e^{s r} multiplying regular EF coefficients.
  norms=[]
  for aa,bbv,ccv,ssv,rrv,ffv in zip(T,vr,RR,S,rr,ff):
   ef=np.array([[aa,bbv],[bbv,ccv]])*np.exp(rate*rrv)
   frame=np.array([[1,ffv/2-1],[1,ffv/2+1]])/np.sqrt(2)
   block=frame@ef@frame.T
   norms.append(np.sqrt(np.sum(block*block)+2*(ssv*np.exp(rate*rrv)/rrv**2)**2))
  norms=np.array(norms);good=(rr>=1.005)&(rr<=10)
  reports.append(dict(log_left=lo,outer_r=R,grid=ng,rtol=tol,max_W_drift=float(np.max(np.abs((uH*pI-pH*uI)/W-1))),Wronskian=float(W),
   source_L2_truncated=float(np.sqrt(simpson(forcing*forcing*rr,x=vv))),response_L2_truncated=float(np.sqrt(simpson(u*u*rr,x=vv))),
   resolvent_bound_from_sampled_input=float(np.sqrt(simpson(forcing*forcing*rr,x=vv))/rate**2),
   complex_plus2_tetrad_sample_max_r1p005_to10=float(norms[good].max()),u_at_r2=float(np.interp(0,vv,u)),
   qvv_at_r2=float(np.interp(0,vv,T)*np.exp(2*rate)),qvr_at_r2=float(np.interp(0,vv,vr)*np.exp(2*rate)),qrr_at_r2=float(np.interp(0,vv,RR)*np.exp(2*rate)),qS_at_r2=float(np.interp(0,vv,S)*np.exp(2*rate))))
  print('GREEN',json.dumps(reports[-1]),flush=True)
 out=dict(python=platform.python_version(),numpy=np.__version__,scipy=scipy.__version__,mpmath=mp.__version__,L=10,k=k,Omega=om,precision=65,
  source_samples=len(v),input_sha256={name:hashlib.sha256((HERE/name).read_bytes()).hexdigest() for name in ['gl_linear_results.json','gl_linear_profile_L10.csv','goal004_source_geometry.py']},
  checks=checks,source_norm_truncated=Fnorm,ah=float(ah),linear_AH_contrast_per_A=float(abs(ah)/(4*(1+k*k))),
  horizon_frobenius_order=18,normalization_delta=.005,
  n0_beta_at_r1=float(beta[0]),n0_mu_at_r1=float(mu[0]),n0_beta_abs_integral=float(.5*simpson(r*abs(arr[:,9]),x=r)),n0_mu_abs_integral=float(.5*simpson(r*r*abs(arr[:,10]),x=r)),
  configs=reports,
  source_columns=['Fhat_plus2','J_plus2','tau_plus2','jv_plus2','jrEF_plus2','Qvv_plus2','Qvr_plus2','Qrr_plus2','Qtheta_plus2','Trr_zero','Tvr_zero','H_contraction_EF'],
  scope='EMPIRICAL physical saved-profile source/point inverse diagnostics only; no interval input, global norm, all-order data, retarded radiation or physical finite-amplitude enclosure.')
 (HERE/'goal004_source_geometry_results.json').write_text(json.dumps(out,indent=2)+'\n')
 print('DONE',json.dumps({key:out[key] for key in ['ah','linear_AH_contrast_per_A','n0_beta_at_r1','n0_mu_at_r1','source_norm_truncated']}),flush=True)


# --post is deliberately handled separately below so it reuses the saved source
# table and never repeats any physical ODE or adds another configuration.

def post():
 import sympy as sy
 z=np.load(HERE/'goal004_source_geometry_samples.npz')
 r=z['r'];v=z['v'];src=z['source'];x=r+v;f=(r-1)/r
 a=(2*k)**2;D=1+a*r**3;w=np.exp(-r/2)*r/D
 poly=(4*a*a-a)*r**5-7*a*r**4+10*a*r**3-(8*a+1)*r*r+5*r-2
 hs=rate*rate+f*poly/(4*r*r*D)
 quotient=abs(src[:,0]*np.exp(rate*x))/(w*hs)
 def diagnostics(mask):
  rr=r[mask];xx=x[mask];aa=src[mask]
  return dict(n0_beta_abs=float(.5*simpson(rr*abs(aa[:,9]),x=rr)),n0_mu_abs=float(.5*simpson(rr*rr*abs(aa[:,10]),x=rr)),
   source_L2=float(np.sqrt(simpson((aa[:,0]*np.exp(rate*xx))**2*rr,x=v[mask]))),
   static_scalar_transfer=float(np.sqrt(3)*k*k/16*simpson(rr*rr*aa[:,-1]*np.exp(rate*xx),x=rr)))
 full=diagnostics(np.arange(len(r)));coarse=diagnostics(np.unique(np.r_[np.arange(0,len(r),2),len(r)-1]))
 # Independent algebra simplification of the Coulomb comparison residual.
 t,A,Q=sy.symbols('t A Q',positive=True);B=Q-A/(2*Q);ff=1-t;DD=1+A/t**3
 V=A*ff+(6-17*t+11*t*t)*t*t+18*t*t*ff*ff/DD**2-6*t*t*(4-5*t)*ff/DD
 wxx=ff*ff*(Q*Q+2*Q*B*t+(B*B+B)*t*t)-ff*t*t*(Q+B*t)
 c0=6-Q*Q+4*Q*B-B*B-B+Q
 c1=-17-2*Q*B+2*B*B+3*B-Q
 c2=11-B*B-2*B
 rhs=c0+c1*t+c2*t*t+18*ff*ff/DD**2-6*(4-5*t)*ff/DD
 assert sy.cancel((V+Q*Q-A-wxx)/t**2-rhs)==0
 assert sy.cancel((V+Q*Q-A-wxx)/t**2-(rhs+1))!=0
 q=np.hypot(k,om);Bv=(k*k+2*om*om)/(2*q)
 ends=[]
 for R in [30.,45.,65.]:
  cc0=6-q*q+4*q*Bv-Bv*Bv-Bv+q
  cc1=-17-2*q*Bv+2*Bv*Bv+3*Bv-q
  cc2=11-Bv*Bv-2*Bv
  CR=abs(cc0)+abs(cc1)/R+abs(cc2)/R**2+18/(k*k*R**3)**2+6*(4+5/R)/(k*k*R**3)
  eta=CR/(2*q*(1-1/R)**2*R)
  ends.append(dict(R=R,C_R=CR,eta=eta,relative_envelope=eta/(1-eta)))
 # Global interpolation defects are distinct from imposed local on-shell jets.
 vvflow=np.linspace(np.log(.01),np.log(64),3001);rrflow=1+np.exp(vvflow)
 pdefect=float(np.max(abs(ps.derivative()(vvflow)-rrflow*(potential(rrflow,k)+om*om-ps(vvflow)**2))))
 elldefect=float(np.max(abs(ells.derivative()(vvflow)-rrflow*ps(vvflow))))
 # Saved profile match derivative: includes leading horizon integral omission.
 integral=simpson(np.exp(2*rows['log_u_relative_match']),x=rows['rstar'])+np.exp(2*rows['log_u_relative_match'][0])/(2*om)
 # Intentionally double only the +2 Qzz source: its z Einstein-divergence
 # mismatch is i K deltaQzz/2, independent of other components.
 at2=src[np.argmin(abs(r-2))];Qzz2=-a*at2[2]/2
 mutation=abs(1j*(2*k)*Qzz2/2)
 assert mutation>.1
 # The EF contraction has coefficient 4 f B C; replacing by 2 f B C
 # was an actual primary diagnostic bug, caught by the scalar source check.
 # Retain the earlier observed value in the negative record, not a fresh run.
 out=dict(python=platform.python_version(),sympy=sy.__version__,source_sample_sha256=hashlib.sha256((HERE/'goal004_source_geometry_samples.npz').read_bytes()).hexdigest(),
 final_script_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
 full_source_quadrature=full,nested_coarse_quadrature=coarse,absolute_nested_differences={key:abs(full[key]-coarse[key]) for key in full},
 supersolution_sample_Cw=float(quotient.max()),supersolution_sample_radius=float(r[np.argmax(quotient)]),
 empirical_match_function_derivative=float(2*om*integral),Coulomb_Volterra_endpoints=ends,
 sampled_saved_p_flow_defect=pdefect,sampled_saved_log_amplitude_flow_defect=elldefect,
 checks=['exact Coulomb residual polynomial identity PASS','wrong Coulomb residual constant REJECTED','doubled Qzz without metric source REJECTED'],
 doubled_Qzz_Bianchi_mismatch_at_r2=mutation,
 actual_corrected_diagnostic_bug='Initial EF H contraction used 2 f B C instead of 4 f B C. At r=2 zz identity mismatch 0.3898909498. Corrected before source/Green run; Q itself unaffected.',
 scope='Postprocessing only: no new physical ODE. Nested differences and sample maxima are diagnostics, not certified errors or global suprema.')
 (HERE/'goal004_source_geometry_diagnostics.json').write_text(json.dumps(out,indent=2)+'\n')
 print(json.dumps(out,indent=2))
if __name__=='__main__':
 if '--post' in sys.argv:post()
 else:main()
