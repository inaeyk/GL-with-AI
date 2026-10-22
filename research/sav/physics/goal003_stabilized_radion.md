# GOAL-003: separate stabilized-radion EFT and massive monopole transport

2026-10-22. This is a **conditional EFT specification**, not a modification
of the five-dimensional vacuum equations or a matched microscopic model.
Input: research/sav-radiation@ef646d56ab7d8bebca578afab53e62da5146ec05.
The vacuum conventions in NOTATION.md remain unchanged. All formulas below
use signature −+++ and the same dimensionless radion phi and fixed G4.

## 1. Action, stress, source and exchange

A consistent uncharged four-dimensional specification is

$$S={1\over16\pi G_4}\int\sqrt{-g}
 [R-2(\nabla\phi)^2-4U(\phi)]+S_K[g,\phi,\chi_n]+S_{higher},$$
$$U(0)=U'(0)=0,\qquad U''(0)=m^2>0,\qquad
 U={m^2\over2}\phi^2+{u_3\over6}\phi^3+{u_4\over24}\phi^4+\cdots.$$

The symbols chi_n denote retained GL/KK fields, or the degrees of freedom
whose boundary/source matching has actually been derived. They cannot be
removed while retaining a vacuum z-dependent GL instability as an exact
solution of a zero-mode theory. A potential requires additional physics.
Define, with its fields on shell,

$$J=-{4\pi G_4\over\sqrt{-g}}{\delta S_K\over\delta\phi},\qquad
T^K_{\mu\nu}=-{2\over\sqrt{-g}}{\delta S_K\over\delta g^{\mu\nu}}.$$

Variation and the diffeomorphism identity give

$$\Box\phi-U'(\phi)=J,\qquad
G_{\mu\nu}=2\phi_{,\mu}\phi_{,\nu}
-g_{\mu\nu}[(\nabla\phi)^2+2U]+8\pi G_4T^K_{\mu\nu},$$
$$T^\phi_{\mu\nu}={1\over4\pi G_4}
 \left[\phi_{,\mu}\phi_{,\nu}-g_{\mu\nu}
 (\tfrac12(\nabla\phi)^2+U)\right],$$
$$\nabla^\mu T^\phi_{\mu\nu}={J\phi_{,\nu}\over4\pi G_4},\qquad
\nabla^\mu T^K_{\mu\nu}=-{J\phi_{,\nu}\over4\pi G_4}.$$

Higher-derivative terms add their own variations to both equations and the
exchange identity. Prescribing J but discarding the corresponding stress
is a test-field transport approximation, not a self-consistent gravitating
solution. The canonical field is varphi=phi/sqrt(4pi G4), with potential
V(varphi)=U(sqrt(4pi G4)varphi)/(4pi G4); its quadratic term is
m² varphi²/2. A nonzero U(0) gives cosmological constant 2U(0), changing
the asymptotics. U'(0)≠0 makes phi=0 fail even the background equation.
These alternatives are excluded from the KK-flat specification.

## 2. What matching must supply

Keep the near-zone vacuum calculation as its already qualified baseline.
An EFT interpretation requires a stabilizer that preserves transverse SO(3)
and compact translations, an equilibrium radius, and a matching surface
r=Rm. Supply boundary values/normal derivatives or a response functional,
including energy exchange, from the retained KK/stabilizer system. The
vacuum source may enter only as

$$J_{match}=J_{vac}^{(2)}+\delta J,
\quad J_{vac}^{(2)}=-{\sqrt3 k^2\over16}A^2 H_{\mu\nu}H^{\mu\nu}.$$

The background and H are then leading approximations, not exact stabilized
solutions. Record separate bounds on δg_background, δOmega, δH, δJ,
charge changes, interface mismatch and omitted operators. In particular a
small growth-rate error accumulates as exp(δOmega t)−1; for a uniform
instability-time comparison one needs |δOmega|T small or a resummed rate
with a proved error. Source errors include the square of the mode error,
normalization changes, and new stabilizer stress. No values for these errors
are available in the repository. They are not zero by convention.

A possible hierarchy to *test*, not an established model, is

$$m r_0\ll1,\quad m/k\ll1,\quad
r_0^{-1},k,\Omega,\Delta\omega\ll\Lambda_{EFT},$$

with the unstable n=1 field retained near the string. The ratio m/Omega is
independent: at L10, Omega r0≈0.06328 is much smaller than one. A small
m r0 therefore need not imply m/Omega≪1. Integrating out stable KK fields
also requires control of their near-horizon redshifted continuum and their
response on the source bandwidth; their asymptotic mass alone supplies no
uniform gap for the exterior evolution. A far-zone zero-mode description
can instead be matched after exponentially localized GL fields have become
small, with a bound on any propagating KK leakage. No such leakage bound
or matched stabilizer is supplied here. Compact-translation-breaking
stabilization changes harmonic mixing and is outside this specification.

## 3. Derived radial operator and resolvent

Linearize about Schwarzschild with f=1−r0/r, x=r+r0 log(r/r0−1), and
psi=r phi. Direct substitution of the covariant scalar d'Alembertian gives

$$(\partial_t^2-\partial_x^2+V_m)\psi=F,\qquad
V_m=f(m^2+r_0/r^3),\quad F=-rfJ.$$

This is exact for a minimally coupled test scalar on that background;
δg and higher operators belong to matching errors. The derivation agrees
with [Koyama–Tomimatsu, II, (4)–(10)](https://arxiv.org/html/gr-qc/0103086v2).
It does not identify that scalar's source with a matched GL calculation.

Let H_m=−d_x²+V_m on L²(dx). V_m≥0, so its Friedrichs operator is
nonnegative. At real s>0,

$$R_m(s)=(H_m+s^2)^{-1},\quad \|R_m(s)\|\le s^{-2},$$
$$R_m-R_0=-R_m(m^2 f)R_0,\qquad
\|R_m-R_0\|\le m^2/s^4.$$

This is an absolute operator estimate; a relative waveform error requires
a nonzero reference lower bound. At s=2Omega, the useful small parameter
for this point-resolvent comparison is m/s, not m r0. Positivity rules out
upper-half-plane growing modes in this **scalar test theory**. It says
nothing about the vacuum gravitational GL eigenvalue.

For frequency convention exp(−i omega t), define u_H~exp(−i omega x)
at the future horizon and u_out as the outgoing solution continued from
Im omega>0. With W=u_H u_out'−u_H' u_out,

$$G_\omega(x,x')=-{u_H(x_<)u_{out}(x_>)\over W(\omega)},
\qquad G_{ret}(t)={1\over2\pi}\int_{\mathbb R+ic}
 e^{-i\omega t}G_\omega\,d\omega,\quad c>0.$$

The derivative jump is −1. Fundamental solutions are defined by the stated
ODE/boundaries; no evaluated Schwarzschild transmission coefficient is
claimed. Initial data add cos(t sqrt H_m)psi0 plus
sin(t sqrt H_m)/sqrt H_m times pi0. For forcing, integrate the latter
operator against F over its causal past. In a horizon-crossing formulation
use the corresponding regular-slice propagator and flux, not a switch
multiplying the GL metric.

At infinity p=sqrt(omega²−m²), with the branch analytic in the upper
half-plane and Im p>0 there (p has the sign of omega on the propagating
real axis). For omega>m,

$$u_{out}\sim e^{i\Theta_\omega},\qquad
\Theta_\omega=p x+{m^2r_0\over2p}\log(r/r_0)+O(r^{-1}\log r).$$

The logarithm follows by inserting exp[i(px+c log r)] into the radial
ODE: omega²−V_m=p²+m²r0/r+O(r^−3), hence 2pc=m²r0.
This long-range phase must be included in near-threshold matching. The
leading power/current does not depend on its convention. For |omega|<m,
p=i sqrt(m²−omega²): the infinity branch is evanescent, with a Coulomb
power multiplying its exponential. The horizon remains an open absorption
channel since V_m→0 there; the full operator has continuum starting at
zero, not at m². Infinity-channel thresholds are ±m. Resonances and
quasi-bound contributions are poles of the continued Wronskian, generally
with decay into the horizon. No lifetime or uniform resonance-free strip
is asserted without its spectral proof.

## 4. Tails, causal fronts and the false exponential threshold

The retarded propagator has branch points at ±m and an interior light-cone
tail. In flat 3+1 dimensions one sees the distinction directly:

$$G^{flat}_{ret}(t,R)={\delta(t-R)\over4\pi R}
-\theta(t-R){m J_1(m\sqrt{t^2-R^2})\over4\pi\sqrt{t^2-R^2}}.$$

The front is causal at light speed; the group speed of a narrow propagating
band is v_g=p/omega<1. A finite-band envelope travels at that speed, not
its sharp front. Near-threshold packets are delayed and dispersed. On
Schwarzschild, branch-cut backscatter adds long tails. In the small-mass
fixed-radius regime the established intermediate monopole tail is
oscillatory t^−3/2; at sufficiently late times it crosses to an oscillatory
t^−5/6 tail with a curvature-dependent phase, for generic nonzero relevant
data overlaps. These are literature asymptotics, not uniform error bounds
for our forced GL source. The crossover depends on mM, M=r0/2; see
[Koyama–Tomimatsu, III.2–III.4](https://arxiv.org/html/gr-qc/0103086v2).
Special data can remove leading coefficients; quasi-bound terms can also
dominate a finite observation window.

For exp(st), omega=is and the infinity exponent is sqrt(s²+m²) for
*every* positive m. There is no change at s=m. The inequality 2Omega>m
is therefore not an emission criterion. A finite-time causal history has
a real-frequency transform and must be passed through G_omega, including
its data term. Even a nonzero above-threshold local source need not have
a nonzero outgoing overlap. For compact radial source the Laplace
particular response is more rapidly evanescent as m increases; this is a
statement about an imaginary-frequency response, not a radiated fraction.

## 5. Timelike energy current and the pulse spectral measure

For the Schwarzschild Killing vector xi=partial_t set
J_E^mu=−Tphi^mu{}_nu xi^nu. Its divergence is
−J phi_t/(4pi G4). At the timelike worldtube r=R,

$$P_R(t)=-{R^2 f(R)\over G_4}\phi_t\phi_r
=-{1\over G_4}\psi_t(\psi_x-f\psi/R).$$

This is energy per asymptotic Killing time. The proper-time power measured
by a static observer is P_R/f; the two redshift factors come from energy
and clock. The Schwarzschild expression applies for R>r0. A regular
horizon flux is obtained in EF coordinates, with positive absorbed flux
r0²(phi_v)²/G4. For a shell the exact balance includes the change in scalar
stored energy, outward outer flux, inward horizon/inner flux, and work
−(1/G4)int r² J phi_t dr. Total stress including the retained system has
zero exchange. At finite R, P_R may change sign and is not pure outgoing
energy: near-zone storage and interference matter.

For a real monochromatic far-zone field phi=Re[b exp(−i omega t+iTheta)]/R,

$$\langle P_R\rangle\longrightarrow{\omega p|b|^2\over2G_4},
\qquad \omega>m.$$

The 1/2 is only this real-amplitude averaging convention. For a real finite
outgoing pulse use a **two-sided** Fourier coefficient b with inverse
1/(2pi), b(−omega)=b(omega)* and the conjugate outgoing phase. After
integrating all time, cross frequencies vanish by Parseval and

$$E_{out}={1\over\pi G_4}\int_m^\infty
 \omega\sqrt{\omega^2-m^2}\,|b(\omega)|^2d\omega.$$

This assumes suitable finite-energy scattering data and the all-time
large-worldtube limit; nonpropagating pieces must leave no net storage
contribution in that limit. For finite time integrate the actual P_R and
retain endpoint storage. Multiplying by a time window mixes frequencies;
one cannot simply replace b by a windowed transform in the diagonal
infinite-time measure. An incoming pulse adds its signed current and
interference. The measure differs from massless news, which uses omega²
and a function of u=t−x alone.

## 6. Controlled and nonuniform massless limits

On a band omega≥omega_min>m let d=m²/omega_min²<1. If the same b is
held fixed, the flux-weight ratio is sqrt(1−m²/omega²), between sqrt(1−d)
and one. Its fractional reduction is at most

$$1-\sqrt{1-d}={d\over1+\sqrt{1-d}}.$$

This compares weights only. The actual b_m−b_0 also needs a matching and
propagator bound; destructive source/data interference invalidates a
universal relative suppression of the signal. In a weakly curved far
region, over distance D, the flat dispersive delay is
D(omega/p−1), and the phase change obeys

$$|p-\omega|D\le {m^2D\over\omega_{min}(1+\sqrt{1-d})}.$$

A massless waveform approximation additionally needs that phase change
and the delay relative to the envelope time scale small, plus control of
the Coulomb phase m²r0 log(R/Rm)/(2p), near-threshold weight, data tails,
curvature scattering and matching errors. m r0≪1 alone supplies none of
these propagation-distance conditions.

A separate finite-time absolute estimate is available without a frequency
gap. For equal Cauchy data on a constant-t exterior slice and equal F,
delta psi=psi_m−psi_0 obeys the massive wave equation with forcing
−m² f psi_0 and zero data. Its positive energy E_m^lin gives

$$\sqrt{2E_m^{lin}[\delta\psi](T)}
\le m^2\int_0^T\|\psi_0(t)\|_2dt.$$

Since H0≥0, the functional calculus bound sin(t sqrt H0)/sqrt H0≤t
in operator norm yields

$$\int_0^T\|\psi_0(t)\|_2dt\le
T\|\psi_0(0)\|_2+\tfrac12T^2\|\dot\psi_0(0)\|_2
+\tfrac12\int_0^T(T-t)^2\|F(t)\|_2dt.$$

Add int||δF|| to the first bound for unequal forcing, and sqrt(2 E_m^lin[delta psi](0)), the energy norm of the initial
data difference, for unequal data. These require L² data/source and do
not substitute for a horizon-nondegenerate metric estimate or a uniform
radiation trace at arbitrarily large R,T. They exhibit an explicit fixed-T
massless convergence domain and its growing constants.

At any fixed positive m, smooth localized finite-energy massive packets
have no usual nonzero massless 1/R news transporting energy through
future null infinity. For a band away from infinite frequency the phase
at t=x+u has no fixed-frequency stationary point as R→infinity. With
sufficient spectral decay the limit vanishes; the energy is recovered on
timelike/scattering limits instead. Finite energy alone need not give a
pointwise radiation-field limit, so pointwise statements require this
regularity. Sending m→0 before large propagation distance recovers massless
transport; keeping m>0 while taking R→infinity at fixed u does not. The
limits are nonuniform, as the m²D/omega estimate explicitly shows.

## 7. Symmetry and remaining inputs

SO(3)-invariant data and stabilizer keep angular ell=0. The only invariant
sphere rank-two tensor is proportional to its metric, with no trace-free
spin-2 news. The massless KK vector has no radiative monopole. Scalar
monopoles remain allowed, both massless and massive; kinematic permission
is not an overlap proof. Fourier n labels the circle and retains the
vacuum convolution rules only with translation-invariant stabilization.
The vacuum n=2 massive gravitational singlet has asymptotic mass 2k and
its own constrained current. Its mass is not sqrt(4k²+m²) without a
specified mixing calculation. No scalar mass restores forbidden massless
tensor Bondi news.

Open inputs: stabilizer field content and stress, u3/u4 and cutoff,
equilibrium/background correction, GL/KK mixing and growth correction,
all-orders data/charge prescription in the stabilized theory, interface
response/current, continuum/quasi-bound constants and errors. S is a
consistent conditional action/transport result, not a matched prediction.
No observational mass, suppression percentage, emission switch or physical
mass survey is chosen. Verification and independent audit are recorded in
AUDIT_GOAL003_FLUX_EFT.md; no spacetime evolution was run.
