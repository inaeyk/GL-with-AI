#!/usr/bin/env python3
"""Independent GOAL-004 dynamics audit. Non-production manufactured controls.

Does not import primary GOAL-004 code or solve physical perturbation ODEs.
Run from repository root or any directory; outputs are alongside this script.
"""
from __future__ import annotations

import hashlib
import json
import math
from pathlib import Path
import sys

import numpy as np
import scipy
from scipy.integrate import quad
from scipy.optimize import brentq
from scipy.interpolate import CubicSpline
import sympy as S

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent
checks: list[dict] = []


def exact(name, value):
    reduced = S.simplify(value)
    ok = reduced == 0
    checks.append({"name": name, "type": "exact_identity", "pass": bool(ok),
                   "residual": str(reduced)})
    if not ok:
        raise AssertionError(f"{name}: {reduced}")


def reject(name, discrepancy):
    reduced = S.simplify(discrepancy)
    ok = reduced != 0
    checks.append({"name": name, "type": "negative_mutation", "pass": bool(ok),
                   "nonzero_discrepancy": str(reduced)})
    if not ok:
        raise AssertionError(f"mutation escaped: {name}")


def condition(name, ok, **record):
    checks.append({"name": name, "type": "manufactured_control", "pass": bool(ok),
                   **record})
    if not ok:
        raise AssertionError(name)


# I. Energy enclosure: prove through the inner product and test sharp cases.
a, b, c, eta = S.symbols("a b c eta", real=True)
norm_squared = a*a + b*b + 2*a*b*c
exact("upper_energy_slack", (a+b)**2-norm_squared-2*a*b*(1-c))
exact("lower_energy_slack_when_b_less_a", norm_squared-(a-b)**2-2*a*b*(1+c))
exact("parallel_worst_relative_energy", ((1+eta)**2-1)-(2*eta+eta**2))
eta10 = S.sqrt(S.Rational(11, 10))-1
exact("ten_percent_sufficient_threshold", 2*eta10+eta10**2-S.Rational(1, 10))
condition("ten_percent_not_necessary_for_orthogonal_error",
          math.sqrt(.1) > float(eta10), error_norm_ratio=math.sqrt(.1),
          actual_relative_energy_error=.1, sufficient_ratio=float(eta10))
# b is an upper bound, not the actual difference norm. At a=1,b=2,delta=-Nref,
# true E=0; the unclipped (a-b)^2 lower bound would wrongly assert E>=1.
reject("unclipped_lower_bound_when_b_exceeds_a", S.Integer(1))
exact("free_scalar_destructive_interference", (1-1)**2)
reject("adding_generated_and_free_energies_without_interference", 1**2+(-1)**2)
exact("independent_scalar_constructive_interference", (1+1)**2-4)

# II. The field at quartic order and a change of asymptotic clock.
eps = S.symbols("eps")
chi2, chi4 = S.symbols("chi2 chi4")
phi_series = S.series(S.sqrt(3)/4*S.log(1+eps**2*chi2+eps**4*chi4), eps, 0, 6).removeO()
exact("quartic_logarithm", phi_series.coeff(eps, 4)-S.sqrt(3)/4*(chi4-chi2**2/2))
reject("raw_gzz_as_quartic_radion", phi_series.coeff(eps,4)-S.sqrt(3)*chi4/4)
u = S.symbols("u", real=True)
Phi2, Phi4, ashift = (S.Function(q)(u) for q in ("Phi2", "Phi4", "a"))
N2, N4 = S.diff(Phi2,u), S.diff(Phi4,u)
clock_pulled = Phi4-ashift*S.diff(Phi2,u)
exact("quartic_news_clock_pullback", S.diff(clock_pulled,u)-(N4-ashift*S.diff(N2,u)-S.diff(ashift,u)*N2))
reject("clock_pullback_omits_jacobian_term", S.diff(clock_pulled,u)-(N4-ashift*S.diff(N2,u)))
n2,n4,q4 = S.symbols("n2 n4 q4", real=True)
power = S.expand((eps**2*n2+eps**4*(n4+q4))**2)
exact("free_order_four_data_first_enters_energy_order_six", power.coeff(eps,6)-2*n2*(n4+q4))
reject("quartic_news_cannot_change_next_energy_coefficient", power.coeff(eps,6)-2*n2*n4)

# No factorial: noncommuting generators at order three contain both orderings.
X = S.Matrix([[0,1],[0,0]])
Y = S.Matrix([[0,0],[1,0]])
M = eps*X+eps**2*Y
expM = S.eye(2)+M+M*M/2+M*M*M/6
coef3 = expM.applyfunc(lambda e:S.expand(e).coeff(eps,3))
exact("ordered_gauge_exponential", (coef3-(X*Y+Y*X)/2-X**3/6).norm())
reject("commuting_Lie_generator_mutation", (coef3-X*Y-X**3/6).norm())
z = S.symbols("z", real=True)
exact("quadratic_cosine_zero_mode", S.integrate(S.cos(z)**2,(z,-S.pi,S.pi))/(2*S.pi)-S.Rational(1,2))
exact("quartic_cosine_zero_mode", S.integrate(S.cos(z)**4,(z,-S.pi,S.pi))/(2*S.pi)-S.Rational(3,8))
exact("half_period_even_order_allows_independent_n6", S.cos(6*(z+S.pi))-S.cos(6*z))
reject("parity_alone_implies_quadratic_harmonics_only", S.cos(6*z))

# III. Exact Duhamel integral and logarithmic-time failure modes.
t, O, lam, sig = S.symbols("t Omega lambda sigma", positive=True)
p = S.Integer(4)
tau = S.symbols("tau", nonnegative=True)
integral = S.integrate(S.exp(sig*(t-tau))*S.exp(p*O*tau),(tau,0,t),conds="none")
formula = (S.exp(p*O*t)-S.exp(sig*t))/(p*O-sig)
exact("Duhamel_residual_propagation", integral-formula)
equal_integral = S.integrate(S.exp(p*O*(t-tau))*S.exp(p*O*tau),(tau,0,t))
exact("resonant_exponent_has_time_factor", equal_integral-t*S.exp(p*O*t))
reject("resonant_exponent_no_log_loss", equal_integral-S.exp(p*O*t))
Ast = .05
def logtime_error(seed, sigma):
    T = math.log(Ast/seed)
    if sigma == 4:
        return seed**4*T*math.exp(4*T)
    return seed**4*(math.exp(4*T)-math.exp(sigma*T))/(4-sigma)
log_examples = {str(sigma): [logtime_error(seed,sigma) for seed in (1e-3,1e-6)]
                for sigma in (3,4,5)}
condition("sigma_larger_than_pOmega_not_uniform", log_examples["5"][1]>100*log_examples["5"][0],
          p=4, Omega=1, Astar=Ast, seeds=[1e-3,1e-6], values=log_examples)
condition("sigma_equal_pOmega_log_growth", log_examples["4"][1]>log_examples["4"][0],
          ratio=log_examples["4"][1]/log_examples["4"][0])

# IV. Memory: derive eliminated response by integrating a different toy system.
# A=e^(Omega t), ydot=-lambda y+A^2, y(0)=0, Adot=Omega A+c A y.
yret = S.integrate(S.exp(-lam*(t-tau))*S.exp(2*O*tau),(tau,0,t),conds="none")
expected_y = (S.exp(2*O*t)-S.exp(-lam*t))/(lam+2*O)
exact("stable_mode_retarded_response", yret-expected_y)
exact("growing_history_cubic_denominator", S.limit(yret/S.exp(2*O*t),t,S.oo)-1/(lam+2*O))
reject("static_closure_denominator", (1/(lam+2*O)-1/lam).subs({lam:3,O:1}))
# Deliberately unclosed residual: small equation residual alone has an
# exponentially increasing response with no stability estimate.
condition("small_residual_not_small_solution_error", math.expm1(10)>1000,
          residual=.001, sigma=1, time=10, actual_error=.001*math.expm1(10))

# V. Nonzero forcing with zero outgoing overlap on a positive scalar operator.
x = S.symbols("x", real=True)
fcompact = (1-x*x)**3
source = -S.diff(fcompact,x,2)+fcompact
overlap = S.integrate(S.exp(x)*source,(x,-1,1))
exact("nonzero_source_zero_outgoing_overlap", overlap)
reject("zero_overlap_source_is_nonzero", source.subs(x,0))
exact("compact_control_endpoint_f", fcompact.subs(x,1))
exact("compact_control_endpoint_first_derivative", S.diff(fcompact,x).subs(x,-1))
# Matching C(2Omega) alone does not identify the causal transfer kernel.
delay = S.symbols("delay", positive=True)
s = S.symbols("s", positive=True)
C0 = S.symbols("C0", real=True)
exact("same_Laplace_point_distinct_causal_kernels",
      C0*S.exp(s*delay)*S.exp(s*(t-delay))-C0*S.exp(s*t))
reject("same_Laplace_point_implies_same_constant_response",
       C0*S.exp(s*delay)-C0)

# VI. Derive massive current directly from the action normalization.
r, omega, momentum, G, f = S.symbols("r omega momentum G f", positive=True)
m = S.symbols("m", nonnegative=True)
phase = omega*t-momentum*r
ph = S.cos(phase)/r
P = -r*r*S.diff(ph,t)*S.diff(ph,r)/G
angle = S.symbols("angle", real=True)
# Drop no 1/r term before averaging; its sin*cos integral vanishes exactly.
Pangle = P.subs({S.sin(phase):S.sin(angle),S.cos(phase):S.cos(angle)})
Pavg = S.integrate(S.expand_trig(Pangle),(angle,0,2*S.pi))/(2*S.pi)
exact("massive_monochromatic_outward_current", Pavg-omega*momentum/(2*G))
reject("monochromatic_current_missing_half", Pavg-omega*momentum/G)
reject("massive_current_using_omega_squared", (Pavg-omega**2/(2*G)).subs({omega:2,momentum:1}))
phiarbitrary = S.Function("phi")(t,r)
local_energy = r*r*(S.diff(phiarbitrary,t)**2+S.diff(phiarbitrary,r)**2+m*m*phiarbitrary**2)/2
local_power = -r*r*S.diff(phiarbitrary,t)*S.diff(phiarbitrary,r)
KGleft = S.diff(phiarbitrary,t,2)-S.diff(phiarbitrary,r,2)-2*S.diff(phiarbitrary,r)/r+m*m*phiarbitrary
exact("spherical_scalar_energy_exchange", S.diff(local_energy,t)+S.diff(local_power,r)-r*r*S.diff(phiarbitrary,t)*KGleft)
psi,psit,psix = S.symbols("psi psi_t psi_x", real=True)
Pworldtube = -r*r*f*(psit/r)*(psix/(f*r)-psi/r**2)/G
exact("finite_worldtube_psi_current", Pworldtube+psit*(psix-f*psi/r)/G)
reject("finite_worldtube_storage_omission", Pworldtube+psit*psix/G)
exact("worldtube_endpoint_storage_primitive", S.diff(f*S.Function("psi")(t)**2/(2*G*r),t)-f*S.Function("psi")(t)*S.diff(S.Function("psi")(t),t)/(G*r))
qmass = S.sqrt((2*O)**2+m*m)
exact("imaginary_frequency_no_threshold_at_m_equals_2Omega", qmass.subs(m,2*O)-2*S.sqrt(2)*O)
condition("false_2Omega_mass_threshold_rejected", float(qmass.subs({O:1,m:3}))>0,
          s=2, mass=3, exterior_decay_rate=math.sqrt(13), source_switch=False)
vratio = S.sqrt(1-m*m/omega**2)
exact("massless_current_limit", S.limit(vratio,m,0)-1)
exact("small_mass_flux_weight", S.limit((1-vratio)/(m*m),m,0)-1/(2*omega**2))
exact("small_mass_group_delay", S.limit((1/vratio-1)/(m*m),m,0)-1/(2*omega**2))
# Fixed small mass does not control distant waveform phase.
condition("small_mass_long_distance_not_uniform", .01**2*1e7/(2*1)>100,
          mass=.01, omega=1, distance=1e7, leading_phase_error=500.)

# Gaussian news has memory in Phi=integral(N); Parseval needs news, not a
# square-integrable Phi. All normalization integrals are independent.
q = S.symbols("q", real=True)
Etime = S.integrate(S.exp(-2*q*q),(q,-S.oo,S.oo))
Espectrum = S.integrate(S.exp(-q*q/2),(q,0,S.oo))
exact("smooth_memory_news_Parseval", Etime-Espectrum)
exact("massless_fraction_normalization", 2*Etime-S.sqrt(2*S.pi))
reject("massless_fraction_missing_M0_factor_two", 2*Etime-Etime)

# A smooth finite-band manufactured massive pulse: b=(w-1)^2(2-w)^2,
# 1<w<2, m=1/2. This is a transport-weight check, no GL mass survey.
def spectrum(w): return (w-1)**2*(2-w)**2
flat_energy,err0 = quad(lambda w:w*w*spectrum(w)**2/math.pi,1,2,epsabs=1e-14)
mass_energy,errm = quad(lambda w:w*math.sqrt(w*w-.25)*spectrum(w)**2/math.pi,1,2,epsabs=1e-14)
ratio = mass_energy/flat_energy
condition("massive_pulse_weight_band_bound", math.sqrt(.75)<=ratio<=1,
          mass=.5, band=[1,2], massless_energy_times_G4=flat_energy,
          massive_energy_times_G4=mass_energy, ratio=ratio,
          quadrature_reported_absolute_errors=[err0,errm],
          certificate=False)

# VII. Independently check the proposed material-fluid reduced action.
# These are model identities, not a derivation of its closure from vacuum GR.
B,k,kc,nu,pp = S.symbols("B k kc nu p", positive=True)
theta = S.symbols("theta", real=True)
ct,st = S.cos(theta),S.sin(theta)
def avg(expr): return S.integrate(S.expand_trig(S.expand(expr)),(theta,-S.pi,S.pi))/(2*S.pi)
sqrtJ = sum(S.binomial(S.Rational(1,2),j)*(B*ct)**j for j in range(5))
massseries = S.expand(avg(sqrtJ*st*st)/(2*k*k))
exact("material_mass_zero_coefficient", massseries.coeff(B,0)-1/(4*k*k))
exact("material_mass_quadratic_coefficient", massseries.coeff(B,2)+1/(128*k*k))
gradseries = B*B*k*k*st*st*(1-pp*B*ct+pp*(pp+1)*B*B*ct*ct/2)
Vseries = S.expand(avg(sqrtJ-1)+avg(gradseries)/(8*kc*kc))
exact("reduced_potential_quadratic", Vseries.coeff(B,2)-(k*k/kc**2-1)/16)
exact("reduced_potential_quartic", Vseries.coeff(B,4)-(-S.Rational(15,1024)+k*k*pp*(pp+1)/(128*kc*kc)))
Gamma2 = k*k*(1-k*k/kc**2)/2
exact("reduced_linear_growth", -2*Vseries.coeff(B,2)/massseries.coeff(B,0)-Gamma2)
exact("matched_viscosity_growth", (O*O+nu*k*k*O-Gamma2).subs(nu,(Gamma2-O*O)/(k*k*O)))
JJ = S.symbols("J",positive=True)
# R_z=-(r0/2) J_q/J^(5/2); epsilon/epsilon0=J^(-1/2), dz=J dq.
exact("local_EOS_gradient_power_nine_halves", JJ*JJ**(-S.Rational(1,2))*JJ**(-5)-JJ**(-S.Rational(9,2)))
exact("frozen_EOS_gradient_power_four", JJ*JJ**(-5)-JJ**(-4))
v = S.symbols("velocity",real=True)
fluidL = -S.sqrt(JJ)*S.series((1-v*v)**S.Rational(1,4),v,0,4).removeO()
exact("fluid_kinetic_coefficient", S.expand(fluidL).coeff(v,2)-S.sqrt(JJ)/4)
reject("ordinary_mass_density_in_place_of_enthalpy", S.expand(fluidL).coeff(v,2)-S.sqrt(JJ)/2)
qcoord = S.symbols("qcoord",real=True)
bfun = S.Function("b")(t)
sn,cn = S.sin(k*qcoord),S.cos(k*qcoord)
Jfun=1+bfun*cn
velocity=S.diff(bfun,t)*sn/k
ELfluid=S.diff(S.sqrt(Jfun)*velocity/2,t)+S.diff(-1/(2*S.sqrt(Jfun))+velocity**2/(8*S.sqrt(Jfun)),qcoord)
ELexpanded=S.sqrt(Jfun)*S.diff(bfun,t,2)*sn/(2*k)+S.diff(bfun,t)**2*sn*cn/(2*k*S.sqrt(Jfun))+bfun*S.diff(bfun,t)**2*sn**3/(16*k*Jfun**S.Rational(3,2))-bfun*k*sn/(4*Jfun**S.Rational(3,2))
exact("ideal_fluid_unprojected_EL_sign_and_velocity_terms", ELfluid-ELexpanded)
reject("ideal_fluid_wrong_pressure_gradient_sign", (ELexpanded.subs({S.diff(bfun,t):0,S.diff(bfun,t,2):0})-bfun*k*sn/(4*Jfun**S.Rational(3,2))).subs({bfun:S.Rational(1,4),k:1,qcoord:S.pi/2}))
Rfun=Jfun**(-S.Rational(1,2))
exact("Eulerian_area_radius_gradient", S.diff(Rfun,qcoord)/Jfun-bfun*k*sn/(2*Jfun**S.Rational(5,2)))
exact("Eulerian_log_radius_gradient", S.diff(S.log(Rfun),qcoord)/Jfun-bfun*k*sn/(2*Jfun**2))
Jarb=S.Function("J")(qcoord)
varb=S.Function("v")(qcoord)
Jq=S.diff(Jarb,qcoord)
gradient_density=Jq**2/Jarb**pp/(8*kc**2)
gradient_force=-S.diff(S.diff(gradient_density,Jarb),qcoord)+S.diff(S.diff(gradient_density,Jq),qcoord,2)
expected_grad=(2*S.diff(Jarb,qcoord,3)/Jarb**pp-4*pp*Jq*S.diff(Jarb,qcoord,2)/Jarb**(pp+1)+pp*(pp+1)*Jq**3/Jarb**(pp+2))/(8*kc**2)
exact("complete_gradient_variational_force",gradient_force-expected_grad)
rayleigh=nu*S.diff(varb,qcoord)**2/(4*Jarb**2)
viscforce=-S.diff(S.diff(rayleigh,S.diff(varb,qcoord)),qcoord)
exact("complete_viscous_variational_force",viscforce+nu*S.diff(S.diff(varb,qcoord)/Jarb**2,qcoord)/2)
# Viscous heating is already quadratic on an exponential history and hence
# can alter cubic amplitude feedback in a two-way thermodynamic closure.
exact("heat_is_quadratic_on_growing_history", (nu/(16*O)*S.exp(2*O*t)).diff(t)-nu*S.exp(2*O*t)/8)

qnodes,qweights = np.polynomial.legendre.leggauss(256)
angles = math.pi*qnodes
weights = qweights/2
cosines,sines = np.cos(angles),np.sin(angles)
kn=2*math.pi/10
kcn=.876160402034
On=.06328119
model_audit = {}
for pvalue in (4.,4.5):
    def dV(bvalue):
        Jn=1+bvalue*cosines
        return float(np.dot(weights,.5*cosines/np.sqrt(Jn)+kn*kn/(8*kcn*kcn)*sines*sines*(2*bvalue*Jn**(-pvalue)-pvalue*bvalue*bvalue*cosines*Jn**(-pvalue-1))))
    eq=brentq(dV,.000001,.95,xtol=1e-13)
    contrast=eq/(1+math.sqrt(1-eq*eq))
    model_audit[str(pvalue)]={"static_potential_minimum_b":eq,
                             "static_contrast_proxy":contrast,
                             "derivative_residual":dV(eq)}
condition("model_static_minima_distinct",model_audit["4.0"]["static_potential_minimum_b"]>model_audit["4.5"]["static_potential_minimum_b"],
          source="independent 256-point angular quadrature, not root model code",
          inputs={"L_over_r0":10,"kc_r0_empirical":kcn,"Omega_r0_empirical":On},
          models=model_audit,
          certificate=False,
          interpretation="Potential minima only; inertial transients may overshoot, and fluid geometry is a model proxy.")

# VIII. Verify the new closed kinematic pulse functions by a separate integral.
mu=S.symbols("mu",positive=True)
Rmemory=S.exp(-mu*mu)-S.sqrt(S.pi)*mu*S.erfc(mu)
Rzeromemory=S.exp(-mu*mu)
exact("Gaussian_memory_linear_mass_term",S.limit(S.diff(Rmemory,mu),mu,0)+S.sqrt(S.pi))
exact("Gaussian_zero_memory_no_linear_mass_term",S.limit(S.diff(Rzeromemory,mu),mu,0))
reject("universal_quadratic_small_mass_error_with_memory",S.limit(S.diff(Rmemory,mu),mu,0))
for muv in (.1,.5,1.):
    memint,memerr=quad(lambda w:math.sqrt(max(0,1-muv*muv/(w*w)))*math.exp(-w*w),muv,np.inf,epsabs=2e-13)
    zeroint,zeroerr=quad(lambda w:w*w*math.sqrt(max(0,1-muv*muv/(w*w)))*math.exp(-w*w),muv,np.inf,epsabs=2e-13)
    memratio=memint/(math.sqrt(math.pi)/2)
    zeroratio=zeroint/(math.sqrt(math.pi)/4)
    memclosed=math.exp(-muv*muv)-math.sqrt(math.pi)*muv*math.erfc(muv)
    zeroclosed=math.exp(-muv*muv)
    condition(f"closed_memory_pulse_ratio_mu{muv}",abs(memratio-memclosed)<2e-11,
              direct_spectral_quadrature=memratio,closed_value=memclosed,quadrature_reported_error=memerr)
    condition(f"closed_zero_memory_pulse_ratio_mu{muv}",abs(zeroratio-zeroclosed)<2e-11,
              direct_spectral_quadrature=zeroratio,closed_value=zeroclosed,quadrature_reported_error=zeroerr)

# IX. Independent weak expansion for the second/final 2-mode model repair.
B1,B2=S.symbols("b1 b2",real=True)
delta=B1*ct+B2*S.cos(2*theta)
Jqmodel=-k*B1*st-2*k*B2*S.sin(2*theta)
sqrt2mode=1+delta/2-delta**2/8+delta**3/16
V2mode=S.expand(avg(sqrt2mode-1+Jqmodel**2*(1-pp*delta)/(8*kc*kc)))
V3=V2mode.coeff(B1,2).coeff(B2,1)
exact("two_mode_cubic_potential_coupling",V3-(S.Rational(3,64)-3*pp*k*k/(32*kc*kc)))
M12linear=S.expand(avg((1+delta/2)*st*S.sin(2*theta)/(2*k*k))/2)
M11linear=S.expand(avg((1+delta/2)*st*st/k**2)/2)
exact("two_mode_offdiagonal_inertia",M12linear.coeff(B1,1)-1/(32*k*k))
exact("two_mode_kinematic_quadratic_forcing",(M12linear.coeff(B1,1)+(M12linear.coeff(B1,1)-M11linear.coeff(B2,1)/2))*O*O-3*O*O/(32*k*k))
D21linear=S.expand(nu*avg(S.cos(2*theta)*ct*(1-2*delta))/2)
exact("two_mode_dissipation_cross_term",D21linear.coeff(B1,1)+nu/4)
M22=S.Rational(1,16)/k**2
Gamma22=2*k*k*(1-4*k*k/kc**2)
exact("two_mode_forced_denominator",4*O*O*M22+nu*O/2+(4*k*k/kc**2-1)/8-M22*(4*O*O+8*nu*k*k*O-Gamma22))

# Optional artifact inspection: independently integrate the saved primary
# waveform tables, without importing or rerunning its ODE implementation.
primary_output=HERE/"goal004_model_results.json"
if primary_output.exists():
    primary=json.loads(primary_output.read_text())
    for item in primary["models"]:
        pvalue=item["p"]
        saved=np.genfromtxt(HERE/f'goal004_model_p{str(pvalue).replace(".","_")}.csv',delimiter=",",names=True)
        tt=saved["t"]
        nin=saved["N_relax"]
        independent_E=float(scipy.integrate.simpson(2*nin*nin,x=tt))
        rel=abs(independent_E/item["cumulative_energy_relaxation"]-1)
        condition(f"primary_saved_news_energy_p{pvalue}",rel<1e-7,
                  independent_decimated_table_energy=independent_E,
                  primary_full_grid_energy=item["cumulative_energy_relaxation"],relative_difference=rel)
        predicted_b=model_audit[str(pvalue)]["static_potential_minimum_b"]
        condition(f"primary_settled_shape_matches_independent_potential_p{pvalue}",abs(saved["b"][-1]-predicted_b)<1e-9,
                  primary_final_b=float(saved["b"][-1]),independent_equilibrium_b=predicted_b)
        oldomega=primary["inputs"]["omega"]
        c0val=primary["inputs"]["C0"]
        weaknews=2*oldomega*c0val*item["epsilon"]**2
        condition(f"primary_weak_C0_limit_p{pvalue}",abs(nin[0]/weaknews-1)<1e-12,
                  initial_news=float(nin[0]),matched_exponential_news=weaknews)
        spectrum_table=np.genfromtxt(HERE/f'goal004_spectrum_p{str(pvalue).replace(".","_")}.csv',delimiter=",",names=True)
        ww=spectrum_table["omega"]
        dd=spectrum_table["dE_window_over_M0_domega"]
        independent_rms=float(np.sqrt(np.sum(ww[1:]**2*dd[1:])/(dd[0]/2+np.sum(dd[1:]))))
        condition(f"primary_spectral_RMS_half_DC_p{pvalue}",abs(independent_rms-item["windowed_rms_omega"])<1e-12,
                  independent_spectral_RMS=independent_rms,primary_RMS=item["windowed_rms_omega"])
        condition(f"primary_unprojected_residual_not_error_certificate_p{pvalue}",item["max_complete_closure_relative_residual"]>.5,
                  full_relative_residual=item["max_complete_closure_relative_residual"],
                  projected_equation_error=item["max_Galerkin_projection_error"],
                  interpretation="Both values retained; no stability/error bound inferred.")

twomode_output=HERE/"goal004_two_mode_results.json"
if twomode_output.exists():
    two=json.loads(twomode_output.read_text())
    inp=json.loads(primary_output.read_text())["inputs"]
    kn,On,kcn=inp["k"],inp["omega"],inp["kcritical"]
    nun=(kn*kn/2*(1-kn*kn/kcn**2)-On*On)/(kn*kn*On)
    modes=np.array([1.,2.])
    wav=modes*kn
    c2=np.cos(modes[:,None]*angles)
    s2=np.sin(modes[:,None]*angles)
    basis2=s2/wav[:,None]
    Jbasis=-wav[:,None]*s2
    for item in two["models"]:
        pvalue=item["p"]
        tab=np.genfromtxt(HERE/f'goal004_two_mode_p{str(pvalue).replace(".","_")}.csv',delimiter=",",names=True)
        bb=np.array([tab["b1"],tab["b2"]])
        vv=np.array([tab["db1"],tab["db2"]])
        tt=tab["t"]
        dyn=4*On*On+8*nun*kn*kn*On-2*kn*kn*(1-4*kn*kn/kcn**2)
        forc=3*On*On/(32*kn*kn)+3/64-3*pvalue*kn*kn/(32*kcn*kcn)-nun*On/4
        cn=-forc/(dyn/(16*kn*kn))
        condition(f"two_mode_weak_coefficient_p{pvalue}",abs(cn-item["weak_b2_over_b1_squared"])<1e-13,
                  independent=cn,primary=item["weak_b2_over_b1_squared"],
                  static_instead=item["static_instead_coefficient"])
        condition(f"two_mode_initial_particular_p{pvalue}",abs(bb[1,0]/bb[0,0]**2-cn)<1e-12 and abs(vv[1,0]/bb[1,0]-2*On)<1e-12)
        eint=float(scipy.integrate.simpson(2*tab["N"]**2,x=tt))
        condition(f"two_mode_saved_news_energy_p{pvalue}",abs(eint/item["energy"]-1)<2e-7,
                  independent=eint,primary=item["energy"])
        geometry_error=0.
        balance=[]
        vder=CubicSpline(tt,vv,axis=1).derivative()(tt)
        acceleration_error=0.
        for j in (0,100,200,250,400,800):
            J=1+bb[:,j]@c2
            Jq=bb[:,j]@Jbasis
            vel=vv[:,j]@basis2
            vq=vv[:,j]@c2
            # Distinct Gauss angular integration, no primary-function calls.
            mass=np.einsum('ia,ja,a->ij',basis2,basis2,weights*np.sqrt(J))/2
            iner=np.sum(weights*(vq*vel*basis2/(4*np.sqrt(J))-c2*vel*vel/(8*np.sqrt(J))),axis=1)
            pot=np.dot(weights,np.sqrt(J)-1+Jq*Jq/(8*kcn*kcn*J**pvalue))
            vp=np.sum(weights*(c2/(2*np.sqrt(J))+(2*Jq*Jbasis/J**pvalue-pvalue*Jq*Jq*c2/J**(pvalue+1))/(8*kcn*kcn)),axis=1)
            damp=nun*np.einsum('ia,ja,a->ij',c2,c2,weights/J**2)/2
            acc=np.linalg.solve(mass,-iner-vp-damp@vv[:,j])
            if j not in (0,800): acceleration_error=max(acceleration_error,float(np.max(abs(acc-vder[:,j]))))
            balance.append(float(vv[:,j]@mass@vv[:,j]/2+pot+tab["heat"][j]))
            # Exact polynomial critical points independently of root's formula.
            poly=np.polynomial.Polynomial([1-bb[1,j],bb[0,j],2*bb[1,j]])
            pts=[-1.,1.]+[float(q.real) for q in poly.deriv().roots() if abs(q.imag)<1e-12 and -1<q.real<1]
            radii=1/np.sqrt(poly(np.array(pts)))
            con=(max(radii)-min(radii))/(max(radii)+min(radii))
            geometry_error=max(geometry_error,abs(float(con)-tab["contrast"][j]))
        condition(f"two_mode_independent_EL_output_p{pvalue}",acceleration_error<1e-8,
                  max_acceleration_difference_from_saved_velocity_spline=acceleration_error,
                  interpretation="Finite table consistency, not a continuum solution enclosure.")
        condition(f"two_mode_independent_mass_heat_balance_p{pvalue}",max(balance)-min(balance)<1e-12,
                  sampled_balance_range=max(balance)-min(balance))
        condition(f"two_mode_independent_contrast_extrema_p{pvalue}",geometry_error<1e-12,
                  max_contrast_difference=geometry_error)
        condition(f"two_mode_nonconvergence_retained_p{pvalue}",item["max_discarded_relative_residual"]>.75,
                  discarded_relative_residual=item["max_discarded_relative_residual"],
                  interpretation="Large residual remains despite projected ODE accuracy.")

paths = [Path(__file__), ROOT/"physics/goal003_data_nonlinear.md",
         ROOT/"physics/goal003_stabilized_radion.md",
         ROOT/"verification/GOAL003_REVIEW_QUALIFICATIONS.md"]
for optional in ("physics/goal004_first_bulge_model.md", "physics/goal004_data_news.md",
                 "physics/goal004_stabilized_transport.md", "verification/goal004_model.py",
                 "verification/goal004_model_results.json", "verification/goal004_model_p4_0.csv",
                 "verification/goal004_model_p4_5.csv", "verification/goal004_spectrum_p4_0.csv",
                 "verification/goal004_spectrum_p4_5.csv", "verification/goal004_two_mode.py",
                 "verification/goal004_two_mode_results.json", "verification/goal004_two_mode_p4_0.csv",
                 "verification/goal004_two_mode_p4_5.csv"):
    if (ROOT/optional).exists(): paths.append(ROOT/optional)
hashes = {str(path.relative_to(ROOT)): hashlib.sha256(path.read_bytes()).hexdigest()
          for path in paths}
record = {"kind":"independent non-production dynamics audit",
          "input_commit":"2f4648c379c474ed124e3ef10b62efcd4a783c66",
          "python":sys.version,
          "sympy":S.__version__,"numpy":np.__version__,"scipy":scipy.__version__,
          "source_sha256":hashes,"checks":checks,
          "summary":{"checks":len(checks),"negative_mutations":sum(c["type"]=="negative_mutation" for c in checks),
                     "all_passed":all(c["pass"] for c in checks)},
          "physical_ODE_configurations":0,"spacetime_evolutions":0,
          "interpretation":"Identities and manufactured controls only; no physical finite-A certification."}
(HERE/"goal004_audit_dynamics_results.json").write_text(json.dumps(record,indent=2)+"\n")
print(json.dumps(record,indent=2))
