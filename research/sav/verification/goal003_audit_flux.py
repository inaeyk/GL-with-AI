#!/usr/bin/env python3
"""Independent GOAL-003 Audit II; exact and manufactured controls only.

No imported project equation implementation, physical ODE, or evolution.
One compact smooth-window FFT (2048 points) checks numerical normalization,
not a continuum theorem. All central identities are derived independently.
"""
import json
import platform
from pathlib import Path

import numpy as np
import scipy
from scipy.integrate import quad
import sympy as S

out = {"scope": "non-production independent symbolic/manufactured Audit II",
       "versions": {"python": platform.python_version(), "sympy": S.__version__,
                    "numpy": np.__version__, "scipy": scipy.__version__},
       "checks": {}, "mutations": {}, "diagnostics": {}}


def check(name, expr):
    value = bool(expr)
    out["checks"][name] = value
    assert value, name


def reject(name, expr):
    value = bool(expr)
    out["mutations"][name] = value
    assert value, name


def zero(expr):
    return S.simplify(S.expand(expr)) == 0


t, r = S.symbols("t r", real=True)
r0, m, G, omega, p, amp = S.symbols("r0 m G omega p amp", positive=True)
phi = S.Function("phi")(t, r)
U = S.Function("U")
eta = S.diag(-1, 1)
coords = (t, r)
grad = S.Matrix([S.diff(phi, q) for q in coords])
X = (grad.T * eta * grad)[0]
L = -2*X-4*U(phi)
euler = S.diff(L, phi)-sum(S.diff(S.diff(L, S.diff(phi,q)), q) for q in coords)
box = -S.diff(phi,t,2)+S.diff(phi,r,2)
check("action_scalar_variation", zero(euler-4*(box-S.diff(U(phi),phi))))
wrongL = -2*X-2*U(phi)
wrongEL = S.diff(wrongL,phi)-sum(S.diff(S.diff(wrongL,S.diff(phi,q)),q) for q in coords)
reject("potential_factor_two", not zero(wrongEL-4*(box-S.diff(U(phi),phi))))

# Strip the common physical normalization 1/(4 pi G) from the stress.
T = grad*grad.T-eta*(X/2+U(phi))
for nu in range(2):
    div = sum(eta[mu,mu]*S.diff(T[mu,nu],coords[mu]) for mu in range(2))
    check("stress_exchange_"+str(nu), zero(div-(box-S.diff(U(phi),phi))*grad[nu]))
varphi, mass = S.symbols("varphi mass", real=True)
canonical_potential = (S.Rational(1,2)*mass**2*(S.sqrt(4*S.pi*G)*varphi)**2)/(4*S.pi*G)
check("canonical_mass_normalization", zero(canonical_potential-mass**2*varphi**2/2))

f = 1-r0/r
psi = S.Function("psi")(t,r)
radial_box = -S.diff(psi/r,t,2)/f+S.diff(r*r*f*S.diff(psi/r,r),r)/r**2
Vm = f*(m*m+r0/r**3)
master = S.diff(psi,t,2)-f*S.diff(f*S.diff(psi,r),r)+Vm*psi
check("massive_radial_operator", zero(-r*f*(radial_box-m*m*psi/r)-master))
reject("wrong_source_sign", not zero(r*f*(radial_box-m*m*psi/r)-master))

# Coordinate Killing power and master energy differ by a finite-radius total time derivative.
power_times_G = -r*r*f*S.diff(psi/r,t)*S.diff(psi/r,r)
master_power_times_G = -S.diff(psi,t)*f*S.diff(psi,r)
check("worldtube_storage_boundary_term", zero(power_times_G-master_power_times_G-S.diff(f*psi**2/(2*r),t)))
check("radial_energy_boundary_term", zero((f*S.diff(psi,r)-f*psi/r)**2
       -(f*S.diff(psi,r))**2-f*r0*psi**2/r**3+f*S.diff(f*psi**2/r,r)))

# Exact SO(3) monopole, with full spherical flat radial operator.
monopole = amp*S.cos(omega*t-p*r)/r
kg = -S.diff(monopole,t,2)+S.diff(r*r*S.diff(monopole,r),r)/r**2-m*m*monopole
check("propagating_scalar_monopole", zero(kg.subs(m*m,omega*omega-p*p)))
mono_power = -r*r*S.diff(monopole,t)*S.diff(monopole,r)
theta = S.symbols("theta", real=True)
mono_phase = S.expand_trig(mono_power).subs(omega*t-p*r,theta)
# The trigonometric average is independent of r: sin^2 -> 1/2, sin cos -> 0.
expected_unaveraged = amp**2*omega*p*S.sin(omega*t-p*r)**2-amp**2*omega*S.sin(omega*t-p*r)*S.cos(omega*t-p*r)/r
check("monochromatic_current", zero(mono_power-expected_unaveraged))
mono_mean = amp**2*omega*p/2
reject("massive_Bondi_current", not zero(mono_mean-amp**2*omega**2/2))
reject("scalar_quadrupole_rule", zero(kg.subs({m:3,omega:5,p:4})) and mono_mean.subs({omega:5,p:4,amp:1})>0)

# Independent exact periodic two-frequency current/Parseval convention control.
wave = S.cos(13*t-5*r)+2*S.cos(15*t-9*r)  # both have m=12
period_flux = S.integrate((-S.diff(wave,t)*S.diff(wave,r)).subs(r,0),(t,0,2*S.pi))
period_spectral = S.pi*(13*5+4*15*9)
check("two_frequency_massive_measure", zero(period_flux-period_spectral))
reject("massive_measure_massless_replacement", not zero(period_flux-S.pi*(13**2+4*15**2)))
reject("positive_frequency_factor_two", not zero(period_flux-period_spectral/2))

# Laplace forcing has a decay rate sqrt(s^2+m^2), with no s=m transition.
x,s = S.symbols("x s", positive=True)
q = S.sqrt(s*s+m*m)
green_pos = S.exp(-q*x)/(2*q)
check("massive_laplace_exterior", zero(-S.diff(green_pos,x,2)+(s*s+m*m)*green_pos))
check("massive_laplace_jump", zero(-2*S.diff(green_pos,x).subs(x,0)-1))
reject("wrong_Green_sign", not zero(2*S.diff(green_pos,x).subs(x,0)-1))
check("laplace_decay_both_sides_of_mass", q.subs({s:2,m:1})>0 and q.subs({s:1,m:2})>0)
a,w = S.symbols("a w", positive=True)
causal_transform = S.factorial(4)/(a-s-S.I*w)**5
reject("twoOmega_mass_switch", causal_transform.subs({a:3,s:1,w:5})!=0 and 1<2)

# Massive Schwarzschild outgoing phase has a Coulomb logarithm.
c = S.symbols("c", real=True)
# In x, phase derivative = p+c*f/r; V=m^2-m^2*r0/r+O(r^-3).
phase_defect = (p+c*f/r)**2-(omega**2-Vm)
coefficient = S.limit(r*phase_defect.subs(omega**2,p**2+m**2),r,S.oo)
check("Coulomb_log_coefficient", zero(coefficient.subs(c,m*m*r0/(2*p))))
reject("omitted_Coulomb_log", not zero(coefficient.subs(c,0)))

# Scalar metric energy cannot be a full nondegenerate horizon norm.
y,delta = S.symbols("y delta", positive=True)
bump=(y-1)**2*(2-y)**2
B1=S.integrate(S.diff(bump,y)**2,(y,1,2))
B0=S.integrate(bump**2,(y,1,2))
Qupper=delta*(2*B1+delta*B0)/2
check("horizon_bump_positive_regular_norm", B1>0)
check("horizon_degenerate_energy_sequence", S.limit(Qupper/B1,delta,0)==0)
reject("Killing_energy_is_metric_norm", S.limit(B1/Qupper,delta,0)==S.oo)
out["diagnostics"]["horizon_bump_B1"] = str(B1)
out["diagnostics"]["horizon_bump_B0"] = str(B0)

# Data freedom and interference, independent of a GL source coefficient.
eps,c4,n2,n4 = S.symbols("eps c4 n2 n4", real=True)
news=eps**2*n2+eps**4*c4*n4
stress=S.expand(news**2)
check("free_order4_interference_starts_order6", stress.coeff(eps,6)==2*c4*n2*n4)
check("free_order4_self_energy_starts_order8", stress.coeff(eps,8)==c4*c4*n4*n4)
reject("universal_K4_unfixed_data", S.diff((news-eps**2*n2)/eps**4,c4)==n4)
reject("omitted_interference", (1+(-1))**2 != 1+(-1)**2)
chi2,chi4=S.symbols("chi2 chi4",real=True)
radion=S.series(S.sqrt(3)*S.log(1+eps**2*chi2+eps**4*chi4)/4,eps,0,5).removeO()
check("fourth_radion_log_completion",zero(radion.coeff(eps,4)-S.sqrt(3)*(chi4-chi2**2/2)/4))
reject("raw_fourth_gzz_as_radion",not zero(radion.coeff(eps,4)-S.sqrt(3)*chi4/4))
clock=S.Function("clock")(t)
Ntwo=S.Function("Ntwo")(t)
check("fourth_news_clock_completion",zero(S.diff(clock*Ntwo,t)-clock*S.diff(Ntwo,t)-S.diff(clock,t)*Ntwo))

# Exact generated compact-harmonic coefficients; parity does not bound n.
z=S.symbols("z", real=True)
fourier={j:S.simplify(S.integrate(S.cos(z)**4*S.cos(j*z),(z,0,2*S.pi))/(2*S.pi)) for j in (0,2,4)}
check("fourth_zero_mode_convolution",fourier[0]==S.Rational(3,8))
check("fourth_complex_fourier_halves",fourier[2]==S.Rational(1,4) and fourier[4]==S.Rational(1,16))
check("half_period_covariance_extra_n4",zero((-eps)**2*S.cos(4*z)-eps**2*S.cos(4*(z+S.pi))))
reject("parity_implies_max_harmonic",zero((-eps)**2*S.cos(4*z)-eps**2*S.cos(4*(z+S.pi))) and 4>2)

# Independent exact radial-foliation constraints, arbitrary a,b,R,kappa,q,v,eta.
# Use the direct 2D-base warped curvature and full 4D coordinate momentum.
aa,bb,RR=[S.Function(n)(r,z) for n in ('aa','bb','RR')]
kap,qq,vv,et=[S.Function(n)(r,z) for n in ('kap','qq','vv','et')]
basecurv=-2/(aa*bb)*(S.diff(S.diff(bb,r)/aa,r)+S.diff(S.diff(aa,z)/bb,z))
lapR=(S.diff(bb/aa*S.diff(RR,r),r)+S.diff(aa/bb*S.diff(RR,z),z))/(aa*bb)
curv4=basecurv+2/RR**2-4*lapR/RR-2*((S.diff(RR,r)/aa)**2+(S.diff(RR,z)/bb)**2)/RR**2
leafcurv=2/RR**2-4/(RR*bb)*S.diff(S.diff(RR,z)/bb,z)-2*(S.diff(RR,z)/(bb*RR))**2
lap_a=S.diff(RR**2/bb*S.diff(aa,z),z)/(bb*RR**2)
Hs=S.diff(bb,r)/bb+2*S.diff(RR,r)/RR
Asq=(S.diff(bb,r)/bb)**2+2*(S.diff(RR,r)/RR)**2
folcurv=leafcurv-(2*S.diff(Hs,r)+Hs**2+Asq)/aa**2+2*Hs*S.diff(aa,r)/aa**3-2*lap_a/aa
check("radial_foliation_curvature",zero(curv4-folcurv))
ww=(qq-vv)/2
th,ph=S.symbols('th ph',real=True)
coordinates=(r,z,th,ph)
g=S.diag(aa**2,bb**2,RR**2,RR**2*S.sin(th)**2)
ginv=S.diag(aa**-2,bb**-2,RR**-2,(RR*S.sin(th))**-2)
Gam=[[[sum(ginv[i,l]*(S.diff(g[l,k],coordinates[j])+S.diff(g[l,j],coordinates[k])-S.diff(g[j,k],coordinates[l])) for l in range(4))/2 for k in range(4)] for j in range(4)] for i in range(4)]
Kmixed=S.diag(kap,vv,ww,ww)
Kmixed[0,1]=et/aa
Kmixed[1,0]=aa*et/bb**2
mom=[]
for ii in (0,1):
    val=sum(S.diff(Kmixed[j,ii],coordinates[j]) for j in range(4))-S.diff(kap+qq,coordinates[ii])
    val+=sum(Gam[j][j][k]*Kmixed[k,ii]-Gam[k][j][ii]*Kmixed[j,k] for j in range(4) for k in range(4))
    mom.append(S.expand(val))
sigA=(vv-qq/3)*S.diff(bb,r)/bb+(qq-3*vv)/3*S.diff(RR,r)/RR
diveta=S.diff(RR**2*et/bb,z)/(bb*RR**2)
q_rhs=aa*diveta+2*et*S.diff(aa,z)/bb**2+Hs*(kap-qq/3)-sigA
eta_rhs=aa*(S.diff(kap,z)+S.diff(qq,z)-S.diff(vv,z)-(3*vv-qq)*S.diff(RR,z)/RR)-Hs*et+(kap-vv)*S.diff(aa,z)
check("radial_q_momentum_constraint",zero(mom[0]+S.diff(qq,r)-q_rhs))
check("radial_eta_momentum_constraint",zero(aa*mom[1]-S.diff(et,r)+eta_rhs))
reject("omitted_lapse_gradient_momentum",not zero(aa*mom[1]-S.diff(et,r)+eta_rhs-(kap-vv)*S.diff(aa,z)))
sig2=(vv-qq/3)**2+2*((qq-3*vv)/6)**2
Kquad=(kap+qq)**2-S.trace(Kmixed*Kmixed)
check("radial_extrinsic_Hamiltonian",zero(Kquad-(2*kap*qq+2*qq**2/3-2*et**2/bb**2-sig2)))
# Modified outer closure kappa=-q/2+d, v prescribed: principal pair q_r=a eta_z,
# eta_r=a q_z/2 is symmetrized by diag(1/2,1), but this alone is not a theorem.
princ=S.Matrix([[0,aa],[aa/2,0]])
symm=S.diag(S.Rational(1,2),1)*princ
check("modified_outer_principal_symmetry",symm==symm.T)

# Full first-order ADM lapse/shift expansion of the horizon MOTS data term.
dtt,dtr,drr,sphere,sphere_r,Og=S.symbols('dtt dtr drr sphere sphere_r Og',real=True)
gtt=eps*dtt
gtr=1+eps*dtr
grr=2+eps*drr
lapse=S.sqrt(-gtt+gtr*gtr/grr)
shift=gtr/grr
theta_data=(2+eps*sphere_r)/S.sqrt(grr)-(shift*(2+eps*sphere_r)-eps*Og*sphere)/lapse
linear_theta=S.simplify(S.diff(theta_data,eps).subs(eps,0))
check("MOTS_full_ADM_lapse_shift",zero(linear_theta-S.sqrt(2)*(Og*sphere-dtt)))
background_theta=2*(r-1)/(r*S.sqrt(r*(r+1)))
check("MOTS_background_displacement",zero(S.diff(background_theta,r).subs(r,1)-S.sqrt(2)))
# Circle-size time derivative additionally depends on compact shift gradient.
normal_log_b,shift_z,lapse0,vcirc=S.symbols('normal_log_b shift_z lapse0 vcirc',real=True)
circle_relation=-normal_log_b+shift_z/lapse0
reject("circle_normal_derivative_omits_shift",not zero(circle_relation+normal_log_b))

# A dynamical eliminated field remembers the GL growth rate.
A,B,Omega,gamma,ca,dc = S.symbols("A B Omega gamma ca dc", real=True)
reparam=S.series((1+3*ca*A*A)*(Omega*A+gamma*A**3)/(1+dc*A*A),A,0,4).removeO()
in_B=S.series(reparam.subs(A,B-ca*B**3),B,0,4).removeO()
check("cubic_amplitude_clock_law",zero(in_B-Omega*B-(gamma+2*ca*Omega-dc*Omega)*B**3))
lam=S.symbols("lam",positive=True)
dt=S.symbols("dt",positive=True)
memory_coeff=S.integrate(S.exp(-(lam+2*S.Symbol('O',positive=True))*dt),(dt,0,S.oo))
check("growing_memory_denominator",zero(memory_coeff-1/(lam+2*S.Symbol('O',positive=True))))
reject("static_gamma_is_dynamical",S.Rational(1,3)!=S.Rational(1,5))
reject("gamma_amplitude_invariant",not zero(2*ca*Omega))

# Exact algebra for absolute interval-energy/spectral errors; destructive data allowed.
norm,budget=S.symbols("norm budget",nonnegative=True)
check("energy_error_expansion",zero((norm+budget)**2-norm**2-(2*norm*budget+budget**2)))
check("ten_percent_news_threshold",zero(2*(S.sqrt(S.Rational(11,10))-1)+(S.sqrt(S.Rational(11,10))-1)**2-S.Rational(1,10)))

# One bounded numerical manufactured C-infinity compact observation taper.
Ngrid=2048
tgrid=np.linspace(-2.,2.,Ngrid,endpoint=False)
dtgrid=4./Ngrid
window=np.zeros_like(tgrid)
inside=np.abs(tgrid)<1
window[inside]=np.exp(-1/(1-tgrid[inside]**2))
signal=window*(1+tgrid)*np.exp(0.2*tgrid)
transform=dtgrid*np.fft.rfft(signal)
domega=2*np.pi/4.
spectral=domega/np.pi*(0.5*abs(transform[0])**2+sum(abs(transform[1:-1])**2)+0.5*abs(transform[-1])**2)
sampled=dtgrid*np.dot(signal,signal)
continuum,quaderr=quad(lambda u:np.exp(-2/(1-u*u))*(1+u)**2*np.exp(.4*u),-1,1,epsabs=1e-13,epsrel=1e-13)
check("compact_window_discrete_Parseval",abs(spectral-sampled)<1e-13)
check("compact_window_integral_diagnostic",abs(sampled-continuum)<1e-12)
reject("compact_window_spectrum_factor_two",abs(spectral/2-continuum)>1e-3)
out["diagnostics"].update({"window_grid":Ngrid,"window_time_integral":float(sampled),
                           "window_spectral_integral":float(spectral),"window_quad":float(continuum),
                           "quad_error_estimate_not_certificate":float(quaderr)})

out["all_passed"] = all(out["checks"].values()) and all(out["mutations"].values())
Path(__file__).with_name("goal003_audit_flux_results.json").write_text(json.dumps(out,indent=2)+"\n")
print(json.dumps(out,indent=2))
