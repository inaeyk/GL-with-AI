#!/usr/bin/env python3
"""GOAL004 non-production reduced-fluid ODE/quadrature, NOT Einstein evolution.

Two matched constitutive closures; all dimensional lengths in r0. The
nonlinear closure spread and quadrature convergence are NOT physical bounds.
No GL eigenvalue/root solve, k scan, spacetime PDE or production import.
"""
from pathlib import Path
import hashlib
import json
import platform
import numpy as np
import scipy
from scipy.integrate import solve_ivp, quad, simpson, cumulative_trapezoid
from scipy.interpolate import PchipInterpolator
from scipy.optimize import brentq
from radion_transfer import horizon_coeffs, tensor_norm

HERE = Path(__file__).resolve().parent
PROFILE = HERE / 'gl_linear_profile_L10.csv'
LINEAR = HERE / 'gl_linear_results.json'
TRANSFER = HERE / 'radion_transfer_results.json'


def inputs():
    lin = json.loads(LINEAR.read_text())
    k = lin['k']; om = lin['refinements'][-1]['omega']
    case = next(c for c in json.loads(TRANSFER.read_text())['cases'] if c['L'] == 10)
    c0 = case['refinements'][-1]['C0']
    tab = np.genfromtxt(PROFILE, delimiter=',', names=True)
    r = tab['r']; d = r-1
    # Independent diagnostic extrapolations of y/d^Omega. No interval claim.
    ahs = []
    for upper in (1e-4, 3e-4, 1e-3):
        good = (d >= 1e-6) & (d <= upper)
        ahs.append(float(np.polynomial.polynomial.polyfit(d[good],
                    tab['y'][good]/d[good]**om, 3)[0]))
    ah = ahs[-1]
    beta = abs(ah)/(4*(1+k*k))
    u = tab['y']*r/(1+k*k*r**3)
    vv = np.log(d)
    logu = PchipInterpolator(vv, np.log(abs(u)))
    pp = PchipInterpolator(vv, tab['p'])
    coeff = horizon_coeffs(k, om)
    def fun(v):
        rr = 1+np.exp(v)
        h2 = tensor_norm(v, k, om, pp(v), coeff)*np.exp(2*logu(v))
        return np.sqrt(3)*k*k/16*rr*rr*h2*np.exp(v)
    splits = [vv[0], -12., -8., -6., -5.3, -4., -2., 0., 1., vv[-1]]
    qs = [quad(fun, a, b, epsabs=1e-8, epsrel=2e-8, limit=120)
          for a,b in zip(splits[:-1], splits[1:])]
    tstatic = sum(q[0] for q in qs)
    tau = (tstatic/c0-1)/(2*om)
    delay = np.log(tstatic/c0)/(2*om)
    assert tstatic > c0 and tau > 0
    return dict(k=k, omega=om, kcritical=lin['kcritical'], C0=c0,
                ah_extrapolations=ahs, beta_AH_empirical=beta,
                static_frozen_shape_transfer=tstatic,
                static_quad_error_estimate=sum(q[1] for q in qs),
                static_omitted_horizon_leading=fun(vv[0])/(1+2*om),
                static_outer_endpoint=fun(vv[-1]),
                transfer_relaxation_time=tau, transfer_delay=delay)


def closure(p, inp, nquad=256):
    k=inp['k']; om=inp['omega']; kc=inp['kcritical']
    gamma2=k*k/2*(1-k*k/kc**2)
    nu=(gamma2-om*om)/(k*k*om)
    ang=(np.arange(nquad)+.5)*2*np.pi/nquad
    cc=np.cos(ang); ss=np.sin(ang)**2
    def coefficients(b):
        J=1+b*cc
        assert np.all(J>0)
        mass=np.mean(np.sqrt(J)*ss)/(2*k*k)
        massp=np.mean(cc*ss/(2*np.sqrt(J)))/(2*k*k)
        pot=np.mean(np.sqrt(J)-1)+k*k*b*b/(8*kc*kc)*np.mean(ss/J**p)
        potp=np.mean(cc/(2*np.sqrt(J)))+k*k/(8*kc*kc)*(
            2*b*np.mean(ss/J**p)-p*b*b*np.mean(ss*cc/J**(p+1)))
        damp=nu/2*np.mean(cc*cc/J**2)
        return mass,massp,pot,potp,damp
    return coefficients,nu


def model(p, inp):
    cf,nu=closure(p,inp)
    eps=1e-4; beta=inp['beta_AH_empirical']; om=inp['omega']
    b0=2*beta*eps; db0=om*b0
    tau=inp['transfer_relaxation_time']; ts=inp['static_frozen_shape_transfer']
    c0=inp['C0']
    # Particular matched auxiliary scalar data; an independent scalar is
    # added below at the news level, not by superposing nonlinear metrics.
    y0=c0*eps**2
    def rhs(t,z):
        b,db,heat,y=z
        M,Mp,V,Vp,D=cf(b)
        return [db,(-Mp*db*db/2-Vp-D*db)/M,D*db*db,
                (ts*(b/(2*beta))**2-y)/tau]
    sol=solve_ivp(rhs,(0,320),(b0,db0,0,y0),method='DOP853',
                  rtol=2e-10,atol=1e-13,max_step=.2,dense_output=True)
    assert sol.success
    t=np.linspace(0,320,16001); b,db,heat,y=sol.sol(t)
    amplitude=b/(2*beta)
    news=(ts*amplitude**2-y)/tau
    energy=cumulative_trapezoid(2*news**2,t,initial=0)
    # Alternative scalar transfer: exactly matches same T(0), T(2Omega).
    td=t-inp['transfer_delay']
    past=np.maximum(td,0)
    bd,dbd,_,_=sol.sol(past)
    pastmask=td<0
    bd[pastmask]=b0*np.exp(om*td[pastmask])
    dbd[pastmask]=om*bd[pastmask]
    delayed_phi=ts*(bd/(2*beta))**2
    delayed_news=2*ts*bd*dbd/(2*beta)**2
    delayed_energy=cumulative_trapezoid(2*delayed_news**2,t,initial=0)
    # No-time-switch control: Z-like difference here is auxiliary filter
    # homogeneous data ONLY, not an exact GL/scalar Cauchy prescription.
    z_phi=y-y0*np.exp(-t/tau)
    z_news=news+y0/tau*np.exp(-t/tau)
    # Explicit independent outgoing pulse, smoothly small at endpoints.
    pulse_center=float(t[np.argmax(news)])
    width=1/(2*om)
    pulse=np.exp(-.5*((t-pulse_center)/width)**2)
    pulse_news=-(t-pulse_center)/width**2*pulse
    pulse_scale=np.sqrt(simpson(news**2,x=t)/simpson(pulse_news**2,x=t))
    pulse_news*=pulse_scale
    alternatives={str(c):float(2*simpson((news+c*pulse_news)**2,x=t))
                  for c in (-1.,0.,1.)}
    contrasts=b/(1+np.sqrt(1-b*b))
    events=[]
    for B in (.05,.10,.15,.20,.30):
        target=2*B/(1+B*B)
        found=np.flatnonzero(b>=target)
        if not len(found):
            events.append(dict(contrast=B,reached=False));continue
        j=int(found[0]);te=brentq(lambda tt:sol.sol(tt)[0]-target,t[j-1],t[j])
        vals=sol.sol(te);Ae=vals[0]/(2*beta)
        events.append(dict(contrast=B,reached=True,time=te,A_proxy=float(Ae),
             energy_relaxation=float(np.interp(te,t,energy)),
             energy_delay=float(np.interp(te,t,delayed_energy)),
             linear_time=float(np.log(target/b0)/om),
             heat=float(vals[2]),
             source_rate_drift=float(abs(vals[1]/vals[0]/om-1))))
    balance=np.array([cf(bb)[0]*vv**2/2+cf(bb)[2]
                      for bb,vv in zip(b,db)])+heat
    peak=int(np.argmax(news)); half=news[peak]/2
    above=np.flatnonzero(news>=half)
    # C-infinity observation taper: transforms w*N, never d(w*Phi).
    w=np.zeros_like(t); a,bwindow=10.,250.
    z=(t-a)/(bwindow-a); inside=(z>0)&(z<1)
    w[inside]=np.exp(4-1/(z[inside]*(1-z[inside])))
    dt=t[1]-t[0]; spec=np.fft.rfft(w*news)*dt
    freq=2*np.pi*np.fft.rfftfreq(len(t),dt)
    density=2/np.pi*abs(spec)**2
    df=freq[1]-freq[0]
    espec=float(df*(.5*density[0]+np.sum(density[1:])))
    etime=float(2*simpson((w*news)**2,x=t))
    rms_freq=float(np.sqrt(np.sum(freq[1:]**2*abs(spec[1:])**2)/
                    (.5*abs(spec[0])**2+np.sum(abs(spec[1:])**2))))
    # Model residual diagnostics: derivative expansion, velocity and
    # unprojected second-harmonic conservative Euler equation.
    # Conservative residual from L2=-sqrt(J)+sqrt(J)*v^2/4:
    # d_t[sqrt(J)*v/2]+d_q[-1/(2sqrt(J))+v^2/(8sqrt(J))]=0.
    # At each time remove the chosen sin(kq) projection; remaining norm
    # does NOT include omitted higher-gradient stress or entropy response.
    angles=(np.arange(256)+.5)*2*np.pi/256
    ss=np.sin(angles);cc=np.cos(angles)
    residual2=[];fullres2=[];projection_errors=[];relative_res=[];gradient=[];vel=[]
    for tt in t[::80]:
        bb,vv,hh,yy=sol.sol(tt); acc=rhs(tt,sol.sol(tt))[1]
        J=1+bb*cc
        res=np.sqrt(J)*acc*ss/(2*inp['k'])+vv*vv*cc*ss/(2*inp['k']*np.sqrt(J))
        res+=bb*vv*vv*ss**3/(16*inp['k']*J**1.5)
        res-=bb*inp['k']*ss/(4*J**1.5)
        ideal=res.copy()
        Jq=-bb*inp['k']*ss;Jqq=-bb*inp['k']**2*cc;Jqqq=bb*inp['k']**3*ss
        gradforce=(2*Jqqq/J**p-4*p*Jq*Jqq/J**(p+1)+
              p*(p+1)*Jq**3/J**(p+2))/(8*inp['kcritical']**2)
        viscforce=nu/2*(vv*inp['k']*ss/J**2-2*vv*bb*inp['k']*ss*cc/J**3)
        full=res+gradforce+viscforce
        projection_errors.append(float(abs(np.mean(full*ss)/inp['k'])))
        full-=2*np.mean(full*ss)*ss
        fullres2.append(float(2*np.mean(full*np.sin(2*angles))))
        relative_res.append(float(np.linalg.norm(full)/
               max(np.linalg.norm(ideal)+np.linalg.norm(gradforce)+np.linalg.norm(viscforce),1e-30)))
        res-=2*np.mean(res*ss)*ss
        residual2.append(float(2*np.mean(res*np.sin(2*angles))))
        gradient.append(float(np.max(abs(bb*inp['k']*ss/(2*J**2)))))
        vel.append(float(abs(vv/inp['k'])))
    data=np.c_[t,sol.sol(t)[0],contrasts,amplitude,y,news,energy,
               delayed_phi,delayed_news,delayed_energy,z_phi,z_news,heat]
    out=HERE/f'goal004_model_p{str(p).replace(".","_")}.csv'
    # Modest decimation, full reproduction from deterministic source.
    np.savetxt(out,data[::20],delimiter=',',header=
        't,b,contrast,A_proxy,Phi_relax,N_relax,E_relax_over_M0,Phi_delay,N_delay,E_delay_over_M0,Phi_Zfilter,N_Zfilter,heat_over_M0',comments='')
    np.savetxt(HERE/f'goal004_spectrum_p{str(p).replace(".","_")}.csv',
        np.c_[freq[:321],density[:321]],delimiter=',',header='omega,dE_window_over_M0_domega',comments='')
    return dict(p=p,epsilon=eps,nu_matched=nu,nu_longwave=3.,
        events=events,settled_contrast=float(contrasts[-1]),
        peak_time=float(t[peak]),peak_news=float(news[peak]),
        half_height_news_width=float(t[above[-1]]-t[above[0]]),
        cumulative_energy_relaxation=float(energy[-1]),
        cumulative_energy_delay=float(delayed_energy[-1]),
        filter_Z_energy=float(2*simpson(z_news*z_news,x=t)),
        independent_pulse_energies=alternatives,
        energy_balance_max_drift=float(np.max(abs(balance-balance[0]))),
        omitted_emission_mass_residual=float(energy[-1]),
        final_heat=float(heat[-1]),windowed_energy_time=etime,
        windowed_energy_spectrum=espec,windowed_rms_omega=rms_freq,
        max_velocity=float(max(vel)),max_log_radius_gradient=float(max(gradient)),
        max_ideal_second_harmonic_residual=float(max(abs(np.array(residual2)))),
        max_complete_closure_second_harmonic_residual=float(max(abs(np.array(fullres2)))),
        max_complete_closure_relative_residual=float(max(relative_res)),
        max_Galerkin_projection_error=float(max(projection_errors)),
        max_input_amplitude_frame_proxy=float(max(amplitude)*39.47),
        scope='EOS/linear-matched heuristic; no physical truncation error bound')


def main():
    inp=inputs();results=[model(p,inp) for p in (4.,4.5)]
    report=dict(task='T-20261023-GLR-GOAL-004',
        scope='Non-production effective model, NOT 2+1/5D evolution',
        versions=dict(python=platform.python_version(),numpy=np.__version__,scipy=scipy.__version__),
        input_commit='2f4648c379c474ed124e3ef10b62efcd4a783c66',
        sha256={p.name:hashlib.sha256(p.read_bytes()).hexdigest()
                for p in (Path(__file__),PROFILE,LINEAR,TRANSFER,HERE/'radion_transfer.py')},
        inputs=inp,models=results)
    (HERE/'goal004_model_results.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))


if __name__=='__main__':
    main()
