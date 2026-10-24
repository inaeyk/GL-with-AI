"""Postprocess same physical configuration. No ODE solve or new domain.
Tests independent operator/actual interpolation pipeline, and shared-source
local on-shell jet lift to distinguish interpolation from algebra defects.
"""
from pathlib import Path
import importlib.util,sys,json,hashlib
sys.dont_write_bytecode=True
import numpy as np
import scipy
from scipy.interpolate import CubicSpline,make_interp_spline
import mpmath as mp
HERE=Path(__file__).resolve().parent
def load(name,file):
    sp=importlib.util.spec_from_file_location(name,HERE/file);m=importlib.util.module_from_spec(sp);sp.loader.exec_module(m);return m
audit=load('audit5','audit_goal005_geometry.py');fn=audit.direct_5d_operator()
old=load('shared4','goal004_source_geometry.py')
old.N=5
result=json.loads((HERE/'audit_goal005_geometry_results.json').read_text())
saved={}
def profiler(frame,event,arg):
    if event=='return' and frame.f_code.co_name=='source_at':
        saved.update({key:frame.f_locals[key] for key in ['src','qs','Jj','r','f']})
sys.setprofile(profiler)
old.source_at(2.)
sys.setprofile(None)
J=old.J;r=saved['r'];f=saved['f'];src=saved['src'];qs=saved['qs'];a=4*old.k2;s=2*old.wm;D=1+a*r**3
def jet(v):return J([v.val(2,j)/mp.factorial(j) for j in range(6)])
tau=jet(src[2,2])*(-2/a);j=jet(src[0,2])*(-2/(2j*old.km));jr=jet(src[1,2])*(-2/(2j*old.km));A=jet(src[0,0]);B=jet(src[0,1]);Jforce=jet(saved['Jj'])
q0=result['forced_summary']['qEF_at_r2'];T0,B0,C0,U0=[mp.mpf(str(t)) for t in q0]
Z0=T0+f.c[0]*B0
Y0=2*a*r.c[0]*(r.c[0]*Z0/s-U0)
lift=f*j-r*f*B-r*A
Y1=((-a*D.c[0]/s)*(Z0-2*r.c[0]**2/D.c[0]*lift.c[0])-(s-1/(2*r.c[0]**2))*Y0)/f.c[0]
P=1/(r-1)-5/r+6/(r*D);bb=2*s+f*P;cc=s*(-4+6/D)/r+3/r**3-a-3/(r**3*D)
yc=[Y0,Y1]
for n in range(3):
    rest=sum(f.c[jj]*(n-jj+2)*(n-jj+1)*yc[n-jj+2] for jj in range(1,n+1))
    rest+=sum(bb.c[jj]*(n-jj+1)*yc[n-jj+1] for jj in range(n+1))
    rest+=sum(cc.c[jj]*yc[n-jj] for jj in range(n+1))
    yc.append((Jforce.c[n]-rest)/(f.c[0]*(n+2)*(n+1)))
Y=J(yc);Z=-s/(a*D)*(f*Y.der()+(s-1/(2*r**2))*Y)+2*r*r/D*lift
U=r*Z/s-Y/(2*a*r);qB=-Z.der()/s-2*Z/(s*r)+j/s+tau;qT=Z-f*qB
qC=(2*Z.der()/s+2*Z/(s*r)-2*j/s-tau+Y/(a*r**3))/f
args=[complex(field.c[jj]*mp.factorial(jj)) for field in [qT,qB,qC,U] for jj in range(3)]
lin=np.array(fn(2.,float(s),float(2*old.km),*args),complex)
pairs=[(0,0),(0,1),(0,2),(1,1),(1,2),(2,2)]
source=np.array([complex(src[aa,bb].val(2)) for aa,bb in pairs]+[complex(qs.val(2))])
# Independently check full 5D operator on actual first-order local positive-k jet.
_,_,hT,hB,hC,hU,_=old.regular_jets(mp.mpf(2))
hargs=[complex(field.c[jj]*mp.factorial(jj)) for field in [hT,hB,hC,hU] for jj in range(3)]
hres=np.array(fn(2.,float(old.wm),float(old.km),*hargs),complex)
# Same log-grid and cubic differentiation used for actual response, on an exact
# regular mass perturbation qvv=2/r, all other fields zero, K=s=0.
v=np.linspace(-18.,np.log(64),16001);ev=np.linspace(-18+.137,np.log(64)-.137,701);rr=1+np.exp(ev);dd=rr-1
sp=CubicSpline(v,2/(1+np.exp(v)))
mv=[sp(ev),sp(ev,1)/dd,(sp(ev,2)-sp(ev,1))/dd**2]
mres=np.array([np.broadcast_to(e,rr.shape) for e in fn(rr,0.,0.,*mv,*([0]*9))],complex).T
mid=(rr>=1.1)&(rr<=10);collar=(rr>=1.005)&(rr<=30)
out={'classification':'EMPIRICAL_DIAGNOSTICS_NOT_CERTIFICATION','source_reuse':'Historical GOAL004 source generator captured via profiler; N increased to 5 for local Q through third jet. Independent 5D linear operator, but local master/source jet consistency is NOT global residual proof.',
 'local_r2_first_order_direct5D_residual':abs(hres).tolist(),
 'local_r2_forced_lift_direct5D_residual':abs(lin+source).tolist(),
 'local_qEF':[float(mp.re(q.c[0])) for q in [qT,qB,qC,U]],
 'saved_qEF':q0,
 'mass_kernel_same_pipeline':{'whole_max':float(np.max(abs(mres))),'r1p005_to30_max':float(np.max(abs(mres[collar]))),'r1p1_to10_max':float(np.max(abs(mres[mid])))},
 'new_physical_solves':0,
 'sha256':{n:hashlib.sha256((HERE/n).read_bytes()).hexdigest() for n in ['audit_goal005_geometry.py','audit_goal005_geometry_diagnostics.py','goal004_source_geometry.py','audit_goal005_geometry_results.json']}}
# Second and final derivative diagnostic: exact derivatives of the piecewise
# cubic source in log(delta), composed with its analytic radial Jacobian. This
# eliminates q-grid spline differentiation without replacing source derivatives
# by their physical on-shell values. No ODE or domain change is made.
tab=np.load(HERE/'goal004_source_geometry_samples.npz')
sps=[CubicSpline(tab['v'],tab['source'][:,i]) for i in range(12)]
dv=J([0]+[mp.mpf((-1)**(n+1))/n for n in range(1,6)])
def sourcejet(col,side):
    spline=sps[col]
    # At v=0 select either adjacent cubic. The third derivative is discontinuous.
    vals=[mp.mpf(str(float(spline(0,n)))) for n in range(3)]
    vals.append(mp.mpf(str(float(spline(side*1e-10,3)))))
    return sum(vals[n]/mp.factorial(n)*dv**n for n in range(4))
alt=[]
for side in [-1,1]:
    Jf=sourcejet(1,side);tt=sourcejet(2,side);jj=sourcejet(3,side);jrr=sourcejet(4,side)
    AA=sourcejet(5,side);BB=sourcejet(6,side);QQ=sourcejet(8,side)
    lif=f*jj-r*f*BB-r*AA
    y1=((-a*D.c[0]/s)*(Z0-2*r.c[0]**2/D.c[0]*lif.c[0])-(s-1/(2*r.c[0]**2))*Y0)/f.c[0]
    ycoeff=[Y0,y1]
    for n in range(3):
        rest=sum(f.c[jj0]*(n-jj0+2)*(n-jj0+1)*ycoeff[n-jj0+2] for jj0 in range(1,n+1))
        rest+=sum(bb.c[jj0]*(n-jj0+1)*ycoeff[n-jj0+1] for jj0 in range(n+1))
        rest+=sum(cc.c[jj0]*ycoeff[n-jj0] for jj0 in range(n+1))
        ycoeff.append((Jf.c[n]-rest)/(f.c[0]*(n+2)*(n+1)))
    YY=J(ycoeff);ZZ=-s/(a*D)*(f*YY.der()+(s-1/(2*r**2))*YY)+2*r*r/D*lif
    UU=r*ZZ/s-YY/(2*a*r);bfield=-ZZ.der()/s-2*ZZ/(s*r)+jj/s+tt;tfield=ZZ-f*bfield
    cfield=(2*ZZ.der()/s+2*ZZ/(s*r)-2*jj/s-tt+YY/(a*r**3))/f
    localargs=[complex(field.c[n]*mp.factorial(n)) for field in [tfield,bfield,cfield,UU] for n in range(3)]
    rrres=np.array(fn(2.,float(s),float(2*old.km),*localargs),complex)+source
    recomputed=-2*a*r*r*f*jrr-4*a*r*QQ-a*tt+2*a*r*r*f/s*jj.der()+4*a*r*r*(a*r**3-2)/(s*D)*(f*BB+AA)-2*a*(a*r**3*(2*r-3)-4*r+3)/(s*D)*jj
    alt.append({'side':side,'actual_cubic_source_analytic_jet_residual':abs(rrres).tolist(),
     'stored_vs_recomputed_J_jets':[float(abs((Jf-recomputed).c[n]*mp.factorial(n))) for n in range(3)],
     'qEF':[float(mp.re(field.c[0])) for field in [tfield,bfield,cfield,UU]]})
out['alternative_exact_cubic_derivative_at_r2']=alt
# A distinct complete quadratic source route: five-dimensional coordinate
# inverse/connection expansion evaluated on all sixteen compact phases.
# This uses ONLY old H,H',H'' point data, never the old source formula.
def independent_Q_phase(phase,second_derivative_multiplier=1):
    radius=2.;growth=float(old.wm);wave=float(old.km)
    g=np.zeros((5,5));g[0,0]=-.5;g[0,1]=g[1,0]=1;g[2,2]=1;g[3,3]=g[4,4]=4
    dg=np.zeros((5,5,5));ddg=np.zeros((5,5,5,5))
    dg[1,0,0]=-1/4;ddg[1,1,0,0]=1/4
    for ii in [3,4]:dg[1,ii,ii]=4;ddg[1,1,ii,ii]=2
    ddg[3,3,4,4]=-8
    hi=[]
    for der in range(3):
        hh=np.zeros((5,5));values=[float(mp.re(field.c[der]*mp.factorial(der))) for field in [hT,hB,hC,hU]]
        hh[0,0]=values[0];hh[0,1]=hh[1,0]=values[1];hh[1,1]=values[2];hh[3,3]=hh[4,4]=values[3];hi.append(hh)
    hi[2]*=second_derivative_multiplier
    co=np.cos(phase);si=np.sin(phase);h=hi[0]*co
    dh=np.zeros_like(dg);ddh=np.zeros_like(ddg)
    dh[0]=growth*h;dh[1]=hi[1]*co;dh[2]=-wave*hi[0]*si
    ddh[0,0]=growth*growth*h;ddh[1,1]=hi[2]*co;ddh[2,2]=-wave*wave*h
    ddh[0,1]=ddh[1,0]=growth*dh[1];ddh[0,2]=ddh[2,0]=growth*dh[2]
    ddh[1,2]=ddh[2,1]=-wave*hi[1]*si;ddh[3,3,4,4]=-2*h[4,4]
    inv=np.linalg.inv(g);dinv=np.array([-inv@dg[e]@inv for e in range(5)])
    hu=inv@h@inv;dhu=np.array([dinv[e]@h@inv+inv@dh[e]@inv+inv@h@dinv[e] for e in range(5)])
    G=np.zeros((5,5,5));C=np.zeros_like(G);dC=np.zeros((5,5,5,5))
    for c in range(5):
      for aa in range(5):
       for bb0 in range(5):
        for e in range(5):
         Lg=dg[aa,e,bb0]+dg[bb0,e,aa]-dg[e,aa,bb0]
         Lh=dh[aa,e,bb0]+dh[bb0,e,aa]-dh[e,aa,bb0]
         G[c,aa,bb0]+=inv[c,e]*Lg/2
         C[c,aa,bb0]+=(inv[c,e]*Lh-hu[c,e]*Lg)/2
         for d0 in range(5):
          dLg=ddg[d0,aa,e,bb0]+ddg[d0,bb0,e,aa]-ddg[d0,e,aa,bb0]
          dLh=ddh[d0,aa,e,bb0]+ddh[d0,bb0,e,aa]-ddh[d0,e,aa,bb0]
          dC[d0,c,aa,bb0]+=(dinv[d0,c,e]*Lh+inv[c,e]*dLh-dhu[d0,c,e]*Lg-hu[c,e]*dLg)/2
    hm=inv@h;dhm=np.array([dinv[e]@h+inv@dh[e] for e in range(5)])
    D=-np.einsum('ce,eab->cab',hm,C)
    dD=-np.einsum('dce,eab->dcab',dhm,C)-np.einsum('ce,deab->dcab',hm,dC)
    QQ=np.zeros((5,5))
    for aa in range(5):
     for bb0 in range(5):
      QQ[aa,bb0]=sum(dD[c,c,aa,bb0]-dD[bb0,c,aa,c] for c in range(5))
      QQ[aa,bb0]+=sum(D[c,c,e]*G[e,aa,bb0]+G[c,c,e]*D[e,aa,bb0]+C[c,c,e]*C[e,aa,bb0]-D[c,bb0,e]*G[e,aa,c]-G[c,bb0,e]*D[e,aa,c]-C[c,bb0,e]*C[e,aa,c] for c in range(5) for e in range(5))
    return np.array([QQ[aa,bb0] for aa,bb0 in pairs]+[QQ[3,3]])
phases=np.arange(16)*2*np.pi/16
Qphases=np.array([independent_Q_phase(phase) for phase in phases])
Q0=Qphases.mean(axis=0);Q2=(Qphases*np.exp(-2j*phases[:,None])).mean(axis=0)
ref0=np.array([complex(src[aa,bb].val(0)) for aa,bb in pairs]+[complex(qs.val(0))])
out['direct5D_quadratic_source_r2']={'phase_count':16,'fourier0_abs_difference':abs(Q0-ref0).tolist(),
 'fourier2_abs_difference':abs(Q2-source).tolist(),'fourier0':Q0.tolist(),
 'fourier2_real':Q2.real.tolist(),'fourier2_imag':Q2.imag.tolist(),
 'independence':'Complete new direct 5D connection expansion and Fourier projection; shared local physical H,Hprime,Hsecond only; comparison source generated by old reduced W route.'}
base=independent_Q_phase(.37);mut=independent_Q_phase(.37,17)
out['Codazzi_derivative_count_control']={'change_second_radial_derivative_factor':17,
 'mixed_Qvz_Qrz_change':abs((mut-base)[[2,4]]).tolist(),
 'ordinary_Qvv_Qvr_Qrr_change':abs((mut-base)[[0,1,3]]).tolist(),
 'scope':'Numerical direct5D control accompanies exact Gaussian Codazzi derivation; not a physical off-shell mode or a norm bound.'}
(HERE/'audit_goal005_geometry_diagnostics.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps(out,indent=2))
