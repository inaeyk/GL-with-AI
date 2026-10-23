#!/usr/bin/env python3
"""Second/final GOAL004 model repair: two-mode material-fluid Galerkin ODE.

Non-production effective model only. No Einstein evolution or physical
source certification. Resolves one stable shape response with its actual
2*Omega dynamic denominator, not an assumed static equilibrium.
"""
from pathlib import Path
import hashlib,json,platform
import numpy as np
import scipy
from scipy.integrate import solve_ivp,cumulative_trapezoid,simpson
from scipy.optimize import brentq

HERE=Path(__file__).resolve().parent
INPUT=HERE/'goal004_model_results.json'

def run(p,inp):
    k=inp['k'];om=inp['omega'];kc=inp['kcritical'];beta=inp['beta_AH_empirical']
    nu=(k*k/2*(1-k*k/kc**2)-om*om)/(k*k*om)
    ang=(np.arange(384)+.5)*2*np.pi/384
    nn=np.array([1.,2.]);kk=nn*k
    cc=np.cos(nn[:,None]*ang);ss=np.sin(nn[:,None]*ang)
    basis=ss/kk[:,None];Jqb=-kk[:,None]*ss
    tau=inp['transfer_relaxation_time'];ts=inp['static_frozen_shape_transfer'];c0=inp['C0']
    b10=2*beta*1e-4
    v3=3/64-3*p*k*k/(32*kc*kc)
    force=3*om*om/(32*k*k)+v3-nu*om/4
    gamma22=2*k*k*(1-4*k*k/kc**2)
    dynamic=4*om*om+8*nu*k*k*om-gamma22
    c=-force/((1/(16*k*k))*dynamic)
    b0=np.array([b10,c*b10*b10]);v0=np.array([om*b10,2*om*c*b10*b10])
    def coefficients(b,vel):
        J=1+b@cc
        if np.min(J)<=.03: raise RuntimeError('model J cutoff .03 reached')
        Jq=b@Jqb; v=vel@basis;Jdot=vel@cc
        M=np.einsum('ia,ja,a->ij',basis,basis,np.sqrt(J))/(2*len(ang))
        inert=np.mean(Jdot*v*basis/(4*np.sqrt(J))-cc*v*v/(8*np.sqrt(J)),axis=1)
        V=np.mean(np.sqrt(J)-1)+np.mean(Jq*Jq/J**p)/(8*kc*kc)
        Vp=np.mean(cc/(2*np.sqrt(J)),axis=1)+np.mean(
             2*Jq*Jqb/J**p-p*Jq*Jq*cc/J**(p+1),axis=1)/(8*kc*kc)
        D=nu/2*np.einsum('ia,ja,a->ij',cc,cc,1/J**2)/len(ang)
        return J,M,inert,V,Vp,D
    def rhs(t,z):
        b=z[:2];vel=z[2:4];y=z[5]
        J,M,I,V,Vp,D=coefficients(b,vel)
        acc=np.linalg.solve(M,-I-Vp-D@vel)
        return np.r_[vel,acc,vel@D@vel,(ts*(b[0]/(2*beta))**2-y)/tau]
    sol=solve_ivp(rhs,(0,320),np.r_[b0,v0,0,c0*1e-8],method='DOP853',
                  rtol=3e-10,atol=1e-13,max_step=.2,dense_output=True)
    assert sol.success
    t=np.linspace(0,320,16001);z=sol.sol(t);b=z[:2];vel=z[2:4];heat=z[4];phi=z[5]
    news=(ts*(b[0]/(2*beta))**2-phi)/tau
    energy=cumulative_trapezoid(2*news**2,t,initial=0)
    # Near/far match still uses the first GL projection: generated fluid b2
    # does not become the complete vacuum q22 source/tensor.
    def contrast_at(bvec):
        b1,b2=bvec
        js=[1+b1+b2,1-b1+b2]
        if abs(b2)>1e-30 and abs(b1/(4*b2))<=1:
            js.append(1-b2-b1*b1/(8*b2))
        rr=1/np.sqrt(np.array(js))
        return float((max(rr)-min(rr))/(max(rr)+min(rr)))
    contrasts=np.array([contrast_at(bb) for bb in b.T])
    events=[]
    for B in (.05,.10,.15,.20,.30):
        found=np.flatnonzero(contrasts>=B)
        if not len(found): events.append(dict(contrast=B,reached=False));continue
        j=int(found[0]);te=brentq(lambda tt:contrast_at(sol.sol(tt)[:2])-B,t[j-1],t[j])
        zz=sol.sol(te)
        events.append(dict(contrast=B,reached=True,time=float(te),b1=float(zz[0]),b2=float(zz[1]),
             energy=float(np.interp(te,t,energy)),heat=float(zz[4]),
             source_rate_drift=float(abs(zz[2]/zz[0]/om-1))))
    balances=[];res_rat=[];projerr=[];res3=[];modes_ratio=[]
    for tt in t[::80]:
        zz=sol.sol(tt);bb=zz[:2];vv=zz[2:4];acc=rhs(tt,zz)[2:4]
        J,M,I,V,Vp,D=coefficients(bb,vv)
        balances.append(float(vv@M@vv/2+V+zz[4]))
        Jq=bb@Jqb;Jqq=-(bb*kk**2)@cc;Jqqq=(bb*kk**3)@ss
        v=vv@basis;vq=vv@cc;vqq=-(vv*kk)@ss;accfield=acc@basis
        # Same full displacement EL as in the one-mode audit.
        ideal=np.sqrt(J)*accfield/2+vq*v/(2*np.sqrt(J))-v*v*Jq/(16*J**1.5)+Jq/(4*J**1.5)
        grad=(2*Jqqq/J**p-4*p*Jq*Jqq/J**(p+1)+p*(p+1)*Jq**3/J**(p+2))/(8*kc*kc)
        visc=-nu/2*(vqq/J**2-2*vq*Jq/J**3)
        full=ideal+grad+visc
        projerr.append(float(max(abs(np.mean(full*basis,axis=1)))))
        residual=full-(2*np.mean(full*ss,axis=1))@ss
        res_rat.append(float(np.linalg.norm(residual)/max(np.linalg.norm(ideal)+np.linalg.norm(grad)+np.linalg.norm(visc),1e-30)))
        res3.append(float(2*np.mean(residual*np.sin(3*ang))))
        modes_ratio.append(float(abs(bb[1])/max(abs(bb[0]),1e-30)))
    w=np.zeros_like(t);q=(t-10)/240;inside=(q>0)&(q<1);w[inside]=np.exp(4-1/(q[inside]*(1-q[inside])))
    spec=np.fft.rfft(w*news)*(t[1]-t[0]);omega=2*np.pi*np.fft.rfftfreq(len(t),t[1]-t[0])
    dens=2/np.pi*abs(spec)**2
    rms=np.sqrt(np.sum(omega[1:]**2*abs(spec[1:])**2)/(.5*abs(spec[0])**2+np.sum(abs(spec[1:])**2)))
    peak=np.argmax(news);above=np.flatnonzero(news>=news[peak]/2)
    name=str(p).replace('.','_')
    np.savetxt(HERE/f'goal004_two_mode_p{name}.csv',np.c_[t,b.T,vel.T,contrasts,phi,news,energy,heat][::20],
        delimiter=',',header='t,b1,b2,db1,db2,contrast,Phi,N,E_over_M0,heat',comments='')
    np.savetxt(HERE/f'goal004_two_mode_spectrum_p{name}.csv',np.c_[omega[:321],dens[:321]],delimiter=',',
        header='omega,dE_window_over_M0_domega',comments='')
    return dict(p=p,nu=nu,weak_b2_over_b1_squared=c,static_instead_coefficient=-force/((1/(16*k*k))*(-gamma22)),
       dynamic_denominator=dynamic,events=events,settled_b=b[:,-1].tolist(),
       settled_contrast=float(contrasts[-1]),max_b2_over_b1=max(modes_ratio),
       peak_time=float(t[peak]),peak_news=float(news[peak]),half_height_news_width=float(t[above[-1]]-t[above[0]]),
       energy=float(energy[-1]),heat=float(heat[-1]),energy_balance_drift=max(abs(np.array(balances)-balances[0])),
       max_Galerkin_projection_error=max(projerr),max_discarded_relative_residual=max(res_rat),
       max_third_harmonic_residual=max(abs(np.array(res3))),windowed_rms_omega=float(rms),
       windowed_energy_time=float(2*simpson((w*news)**2,x=t)),
       windowed_energy_spectral=float((omega[1]-omega[0])*(.5*dens[0]+sum(dens[1:]))),
       scope='Two-mode effective closure; q22/gravity/entropy/kernel errors uncontrolled')

def main():
    inp=json.loads(INPUT.read_text())['inputs']
    result=dict(versions=dict(python=platform.python_version(),numpy=np.__version__,scipy=scipy.__version__),
       source_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
       input_sha256=hashlib.sha256(INPUT.read_bytes()).hexdigest(),models=[run(p,inp) for p in (4.,4.5)])
    (HERE/'goal004_two_mode_results.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result,indent=2))

if __name__=='__main__':main()
