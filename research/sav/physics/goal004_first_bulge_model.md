# GOAL-004: two matched effective bulge closures and a scalar waveform

2026-10-23; input `2f4648c379c474ed124e3ef10b62efcd4a783c66`.
**Evidence class: calibrated semi-analytic heuristic, with uncontrolled
nonlinear constitutive, entropy and radial-mode errors.** This is a
computed reduced alternative to the unclosed exact first-bulge problem,
not an exact-data vacuum evolution or a physical error enclosure.
Only two small effective ODEs and profile quadratures are used. No
spacetime numerical evolution, production code or parameter survey.

## 1. Exact starting point and the reduction that loses control

The exact SO(3) orbit action is

\[
S={1\over4G_5}\int\sqrt{-g_3}
[R^2\mathcal R_3+2(\nabla R)^2+2].
\tag{1}
\]

It retains one local physical configuration degree and the entire circle
tower. A long-wavelength, locally uniform near-string expansion of (1)
has the neutral-string equation of state. This can be derived without a
dynamic fit: energy density e=R/(2G5), entropy density s=pi R²/G5 and
temperature T=1/(4pi R) obey Ts=e/2 and P=Ts-e=-e/2. Thus e(s) is
proportional to sqrt(s), and cs²=dP/de=-1/2. The derivative expansion
and viscosity comparison agree with
[Camps--Emparan--Haddad, (1.1)--(1.4), sections 2.3 and 3](https://arxiv.org/html/1003.3636).
This literature statement is not an accuracy theorem at kr0=0.628.

Let q be a material entropy coordinate and use the single displacement

\[
z=q+{b(t)\over k}\sin(kq),\quad J=\partial_qz=1+b\cos(kq)>0,
\quad v={\dot b\over k}\sin(kq).
\tag{2}
\]

Isentropic transport gives s=s0/(gamma J). Integrating the relativistic
fluid action -int dz e(s) yields, divided by E0=e0 L,

\[
\mathcal L_{ideal}=-\langle\sqrt J(1-v^2)^{1/4}\rangle
=-\langle\sqrt J\rangle+{1\over4}\langle\sqrt J v^2\rangle+O(v^4).
\tag{3}
\]

Angle brackets mean uniform average over kq. At low velocity the local
radius proxy is R/r0=J^-1/2, and the contrast is

\[
\mathcal B={R_{max}-R_{min}\over R_{max}+R_{min}}
={b\over1+\sqrt{1-b^2}},\qquad b={2\mathcal B\over1+\mathcal B^2}.
\tag{4}
\]

This is a thermodynamic proxy, not a theorem identifying nonlinear MOTS
or event-horizon radii. Matching the *linear*, slice-specific GL horizon
map gives b=2 beta A, beta=|a_h|/[4(1+k²)]≈6.90024549. The saved L10
profile gives a_h≈-38.49741287 by three small-horizon-window polynomial
extrapolations. The source investigator obtains the same value separately.
Neither extrapolation is an interval-certified horizon connection.

## 2. Two explicit constitutive closures, derived from their actions

The ideal fluid alone has growth k/sqrt(2), which is badly inaccurate at
L10. Instead of an arbitrary prescribed saturation history, retain a
positive two-gradient energy and match its quadratic coefficient to the
old empirical threshold kc=0.876160402034/r0. Define

\[
M(b)={\langle\sqrt J\sin^2(kq)\rangle\over2k^2},\qquad
V_p(b)=\langle\sqrt J-1\rangle+
{\langle J_q^2/J^p\rangle\over8k_c^2}.
\tag{5}
\]

Closure F uses p=4: the Eulerian gradient energy is proportional to
e0 (partial_z R)², with its energy-density prefactor frozen at the
background. Closure L uses p=9/2: replace that prefactor by the local
e(R)=e0/sqrt(J). Transforming dz=Jdq and R=J^-1/2 derives both powers.
The coefficient follows from imposing the threshold on the quadratic
dispersion; the two nonlinear completions are explicit **constitutive
assumptions**, not coefficients derived from the full constrained 5D
action. No stronger claim about their origin is made. The competing
completions expose precisely the radial matching information still missing.

Every retained nonlinear coefficient is fixed by variation of the stated
effective action M dot b²/2-Vp, rather than chosen as a fitted cubic law.
For example independent expansion gives

\[
M={1\over4k^2}-{b^2\over128k^2}+\cdots,\quad
V_p={k^2/k_c^2-1\over16}b^2+
\left[-{15\over1024}+{k^2p(p+1)\over128k_c^2}\right]b^4+\cdots.
\tag{6}
\]

Use positive Rayleigh dissipation
D(b) dot b², with D=(nu/2)<cos²(kq)/J²>. The J^-2 follows from a
local bulk viscosity proportional to R² and a velocity gradient
partial_z v=dot b cos(kq)/J. The leading black-brane value is nu=3r0.
For both closures use the *same* linearly matched coefficient

\[
\Gamma^2={k^2\over2}(1-k^2/k_c^2),\quad
\nu={\Gamma^2-\Omega^2\over k^2\Omega}=3.67756505r_0,
\]
\[
\boxed{M\ddot b+\tfrac12 M'\dot b^2+V_p'=-D\dot b.}
\tag{7}
\]

Then Omega²+nu k² Omega-Gamma²=0 exactly. This is matching to the
empirical physical GL growth, not an independent prediction of it. The
23-percent discrepancy between matched and leading viscosity is already
evidence of finite-wavelength corrections. All other constants in (7)
are fixed by (3),(5), with no separate nonlinear fit. Reflection fixes
zero net longitudinal momentum. Circle L is fixed. Effective stress/
tension is tied to the EOS/gradient functional, not independently tuned.

The exact reduced energy identity is

\[
H_{shape}=\tfrac12M\dot b^2+V_p,\quad
\dot H_{shape}=-D\dot b^2,\quad
Q(t)=\int_0^tD\dot b^2dt',\quad H_{shape}+Q=constant.
\tag{8}
\]

Viscous entropy production changes the entropy profile; keeping J as
the only shape while storing Q in a reservoir omits that feedback.
Q starts at order b² and feeds the same order-b³ dynamics as the quartic
potential. It is **not** a higher-order controlled omission. Equations
(7)--(8) are a reproducible dissipative model, not a closed asymptotic
expansion through cubic order in vacuum GR.

## 3. Weak source matching, and exactly what q2 is not recovered

The genuine quadratic radion source from (1) is the previously audited
S(r)=sqrt(3)k²rf H_mu nu H^mu nu/16. The source history f(t)=A(t)²
with the exact growing linear mode yields Phi=r0 C0 A², C0≈12.30729197.
The physical n=2 companion is the old affine forced-master response,
with trace/divergence and all constraints. Its new empirical evaluation
is in [the source note](goal004_source_geometry.md).

For a formal weak near-zone lift one must use
gbar+A H cos(kz)+A²[q20+2 Re(q22 exp(2ikz))], with q20,q22 obtained
from that source and the selected homogeneous data. A fluid radius
ansatz is not a substitute for this lift. Its generated second harmonic
does **not** equal the full q22 tensor. We have not constructed a
nonlinear matching that makes the fluid metric recover all seven q2
equations or the homogeneous data of a global fixed-charge family.
Consequently the fully constrained first-bulge reduction requested in
Gate E remains obstructed; the present model is the explicit computed
reduced alternative. It recovers Omega and scalar C0, not an uncomputed
physical cubic self-coupling or the full q2 metric. This failure is
retained, not hidden by calling the fluid n=2 harmonic a GL solution.

## 4. Two source-to-waveform kernels, with two calibration points

Freezing the normalized radial tensor defines the scalar transfer

\[
T(s)=-W_s^{-1}\int y_{H,s}(x)S(x)dx.
\tag{9}
\]

Its static value is known without a new scalar ODE because
yH,0=r, yI,0=-r log f and W0=-1 (r0=1):

\[
T(0)={\sqrt3k^2\over16}\int_1^\infty r^2H_{\mu\nu}H^{\mu\nu}dr
\simeq13.27718117.
\tag{10}
\]

This is the static response of a **frozen growing-mode radial source**,
not a static GL mode or a second radiation result. Saved-profile
PCHIP plus the prior regular horizon contraction gives the displayed
number; the source investigator's separate source-grid quadrature gives
13.27718166. The difference ~4.9e-7 is diagnostic, not a certified error.
Both share the empirical physical profile. No GL/root solve is repeated.

Two minimal causal rational/delay interpolants now match BOTH T(0) and
T(2Omega)=C0:

\[
T_R(s)={T(0)\over1+\tau s},\quad
\tau={T(0)/C_0-1\over2\Omega}=0.62266577r_0,
\]
\[
T_D(s)=T(0)e^{-d s},\quad
d={\log[T(0)/C_0]\over2\Omega}=0.59934818r_0.
\tag{11}
\]

Thus tau dot Phi+Phi=r0 T(0) A², or Phi=r0 T(0)A(t-d)². On an
exponential growing history both reproduce Phi=r0 C0 A² and the old
weak energy law 2 Omega r0 C0²(A2^4-A1^4). The relaxation time is a
matched **secant parameter**, not an evaluated Green-kernel moment.
Neither interpolant encloses the actual distributed radial Green kernel.
Even fixing two transfer values permits arbitrary signed three-delay
perturbations with zero transfer at both points; the exact primary test
retains that counterexample. Their small mutual spread cannot be sold
as a physical kernel error bound.

Equations (2)--(11) specify the waveform for any initial b,dot b and
auxiliary scalar filter datum. The baseline is epsilon=A(0)=10^-4,
b(0)=2 beta epsilon, dot b(0)=Omega b(0), and Phi(0)=r0 C0 epsilon².
These are **model inputs**, not a certified exact vacuum Cauchy family.
The second filter preparation sets Phi(0)=0, changing the homogeneous
filter transient only. It is called Zfilter, not the physical P/Z scalar
problem. An independent smooth scalar news nfree can also be added;
energy always uses int(Nmodel+nfree)²/G4 with interference. There is no
nonlinear metric superposition or switching of a GL solution in this step.

At u=t in this effective asymptotic clock convention, compute
N=dot Phi, d(E/M0)/du=2N²/r0. The delays in (11) are the model clock
matching; actual near-zone-to-Bondi timing and its error are unclosed.
For spectra transform wN with a C-infinity observation taper, not the
derivative of wPhi. Stored CSV spectra specify that window explicitly.

## 5. Computed quantitative predictions, not physical bounds

Length/time units r0; epsilon=10^-4. R and D denote the two kernels.

| Closure | Bproxy | time/r0 | E_R/M0 up to event | E_D/M0 up to event |
|---|---:|---:|---:|---:|
| F (p=4) | .05 | 68.1125 | 5.08422e-8 | 5.08794e-8 |
| L (p=4.5) | .05 | 68.2082 | 5.05447e-8 | 5.05887e-8 |
| F | .10 | 80.5612 | 7.16131e-7 | 7.17916e-7 |
| L | .10 | 81.0720 | 6.94626e-7 | 6.96618e-7 |
| F | .15 | 90.6574 | 2.77631e-6 | 2.78612e-6 |
| L | .15 | 93.1559 | 2.46370e-6 | 2.47149e-6 |

No closure reaches Bproxy=.2 or .3 by t=320r0; their limiting
contrasts are .196681 and .175794. These are spurious candidate
equilibria of the constitutive closures, not evidence that the vacuum
GL instability saturates. The reduced model therefore fails as a
predictor of a large first bulge or a cascade, and it is stopped there.

The computed initial pulse news peaks at t/r0=89.10 and 87.42, with
peak N=.00034745 and .00028639, and half-height NEWS widths 25.36 and
24.96r0. Cumulative model pulse energies through 320r0 are 4.63396e-6
and 3.09792e-6 M0. These numbers are model outputs, **not estimates with
controlled errors for physical first-bulge radiation**. The specified
observation taper on 10<u/r0<250 gives rms angular frequencies
omega_rms r0≈.06737 and .06885. This width/frequency is window-dependent;
it is not a measured carrier oscillation of an exponentially growing GL
mode. Full deterministic waveforms and spectra are in the verification CSVs.

For another small seed, the leading event-time shift is
Delta t=-log(epsilon_new/epsilon_old)/Omega; a tenfold reduction delays
events by 36.3866r0. The nonlinear ODE defines corrections for all seeds
without fitting a new history. The initial shape energy/heat reservoir
is adjusted as a bookkeeping reference to the same total model mass.

The resemblance of the .15 event times to the geometrical t_p≈93r0 in
[Figueras et al., III.1/Table 2](https://arxiv.org/html/2210.13501v2)
is not validation: the seed normalization, initial data, radius criterion,
slice and radiation arrival map have not been matched. That paper and
[Lehner--Pretorius](https://arxiv.org/html/1006.5960) supply geometry and
cascade comparisons, not scalar-news energy benchmarks.

## 6. Residuals, error inventory and falsification

The exact model energy identity (8) has numerical drift below 3e-17
in units E0; smooth-window spectral and time energies agree at floating
precision. These are implementation checks. Physical errors remain:

- kr0=.628 is not a parametrically small derivative expansion. The
  kc-matched gradient energy and its two nonlinear completions have no
  rigorous truncation bound. No static Gubser coefficient is interpreted
  as a dynamical cubic coefficient.
- Heat is ~.00250/.00200 M0 in the two closures and has omitted local
  entropy feedback at the retained nonlinear order. One-way scalar
  postprocessing has no reciprocal near-zone source work. The total
  energy residual, after adding heat and outgoing energy, equals the
  postprocessed E_rad, ~4.6e-6/3.1e-6 M0, rather than zero. Subtracting
  the asymptotic flux instantaneously from a near-zone mass would not fix
  this without accounting for delay, exterior storage and horizon flux.
- The modal truncation leaves a nonzero second-harmonic displacement
  Euler--Lagrange residual. The executable evaluates the *complete*
  retained gradient/viscous residual, checks its first-harmonic projection,
  and records discarded relative residuals .630817 and .634563. A small Galerkin projection
  residual does not imply a small physical field error without stability.
- max(|dot b|/k)<.016 is a useful low-velocity check; the independent
  regular-frame diagnostic 39.47 A reaches ~1, so this is not a weak
  metric perturbation theorem. The logarithmic radius gradient is also
  reported with its correct label.
- Source-rate drift |dot b/(Omega b)-1| is already .24--.29 at B=.1 and
  .55--.69 at B=.15. A frozen linear radial tensor no longer solves
  the constraints with that history. This is a leading model-error proxy,
  not a computed nonlinear radion source or a switch of a vacuum metric.
- The full q2 response, q3 feedback, fourth-order radion/Bondi completion,
  true initial scalar data and actual Green kernel remain unmatched.
  Constitutive/kernel spread and floating error are not bounds on these.

The waveform is falsifiable: specified preparation and contrast must
produce the stated time/shape relation and scalar response for this model
to survive a later independently authorized test. A growing second-mode
residual, failure of the EOS radius map, or a different radial transfer
rejects it. The failure to reach .2 already rejects its strong-bulge use.
It still supplies a concrete early-pulse alternative for comparison,
rather than a free sigmoid or a proposed unexecuted fit.

Neither early nor late dominance follows. If later generation i has a
characteristic radius rho_i, efficiency eta_i and multiplicity mi,
even a dimensional energy law E_i~mi eta_i rho_i²/G5 requires
  sum mi eta_i rho_i² relative to the first term. Those efficiencies,
  multiplicities and above-threshold high-frequency channels are unknown.
Early-dominant and cascade-dominant histories are both allowed. The
separate light-radion transport note changes none of this vacuum model.

## 7. Reproduction and independent review

From research/sav:
`python3 verification/goal004_model.py > verification/goal004_model_output.txt`.
This imports only prior scalar contraction/Frobenius utilities, whose
hashes are recorded, and does not call their GL solver. The original
profile/eigenvalue/C0 uncertainty remains empirical. Results JSON pins
versions and input/script hashes. Two effective ODEs, 256-point angular
quadrature, deterministic time quadrature and FFT are non-production.

[Reviewer II](../verification/AUDIT_GOAL004_DYNAMICS.md) independently
derived (3),(6)--(8), the current and energy inequality before inspecting
this implementation. It found a pressure sign error in the discarded-mode
diagnostic, a logarithmic-gradient label error and an rms-spectrum DC-bin
weight error. The ODE/waveform energy was unaffected; all diagnostics
were corrected and rerun. Those failures are retained in negative results.
The independent review accepts the arithmetic within the stated closures
and withholds physical nonlinear/first-bulge certification.

## 8. Second and final repair: retain the stable shape mode and its memory

The .63 discarded-mode residual is too large to leave untested. The final
bounded repair retains a second material displacement,
z=q+b1 sin(kq)/k+b2 sin(2kq)/(2k), so J=1+b1 cos(kq)+b2 cos(2kq).
Keep the same two constitutive potentials and viscosity, with no new
free nonlinear coefficient. Define ei=sin(ki q)/ki, ci=cos(ki q),
ki=ik for i=1,2. Variation of exactly the same low-velocity action gives

\[
M_{ij}=\tfrac12\langle\sqrt J e_i e_j\rangle,\quad
D_{ij}=\tfrac\nu2\langle c_i c_j/J^2\rangle,\quad
M_{ij}\ddot b_j+I_i+\partial_iV_p=-D_{ij}\dot b_j,
\]
\[
I_i=\left\langle{\dot J v e_i\over4\sqrt J}
-{c_i v^2\over8\sqrt J}\right\rangle,\quad
v=\sum_j\dot b_je_j.
\tag{12}
\]

The shape-plus-heat identity remains exact in this projected model.
The growing weak response b2=c_p b1² is determined dynamically,

\[
c_p=-{f_p\over M_{22}(0)[4\Omega^2+8\nu k^2\Omega-\Gamma_2^2]},\quad
M_{22}(0)={1\over16k^2},\quad
\Gamma_2^2=2k^2(1-4k^2/k_c^2),
\]
\[
f_p={3\Omega^2\over32k^2}+{3\over64}
-{3pk^2\over32k_c^2}-{\nu\Omega\over4}.
\tag{13}
\]

The four terms follow respectively from kinetic connection, EOS cubic
energy, gradient cubic energy and viscous mode coupling. Reviewer II
independently obtained them before inspecting this implementation.
For p=4,4.5 the coefficients are .809483 and .905512. Replacing the
dynamic denominator by the static one gives 1.53786 and 1.72030, a large
error. Initial b2=c_p b1², dot b2=2Omega b2 selects this forced fluid
response; independent stable homogeneous data would be extra model inputs.
This tests a real effective-model memory effect, not the complete
constrained vacuum q22 tensor or its horizon data.

Radius contrast is now calculated from the extrema of
J(x)=1+b1 x+b2(2x²-1), -1<=x<=1, including the vertex
x=-b1/(4b2) when inside. Applying the one-mode contrast formula to b1
would be wrong. The scalar kernel still sees Aproxy=b1/(2beta), with the
same explicitly uncontrolled source-shape matching. Changing that source
at order four requires the missing actual q3/q4 calculation, not a
fit to fluid b2.

| Retained shape modes | p | t(B=.10)/r0 | E(B=.10)/M0 | t(B=.20)/r0 | E(B=.20)/M0 |
|---|---:|---:|---:|---:|---:|
| 1 | 4 | 80.56 | 7.16e-7 | not reached | not available |
| 1 | 4.5 | 81.07 | 6.95e-7 | not reached | not available |
| 2 | 4 | 79.12 | 9.07e-7 | 92.35 | 1.52e-5 |
| 2 | 4.5 | 79.15 | 9.18e-7 | 92.96 | 1.48e-5 |

All entries are calibrated model outputs. Exact deterministic precision is
in `goal004_two_mode_results.json`, not an accuracy claim. With two modes
the pulse news peaks at 94.96/93.30r0, NEWS half-height widths
20.84/20.08r0, rms windowed angular frequencies .07993/.08361 r0^-1,
and cumulative E(320)/M0≈4.11e-5/2.89e-5. The limiting contrasts are
.2912/.2641, still below .3. Compared to the one-mode totals, this is
an order-of-magnitude sensitivity that the small two-kernel spread missed.

The complete next discarded-mode relative residual is now .795/.800,
with a nonzero third harmonic, even though the two projected equations
are satisfied to ~1e-16 and energy balance to ~5e-17. Thus the repair
does **not** make the effective PDE approximation converge. It isolates
one missing stable response, improves weak-history structure and produces
new explicit predictions, while exposing continued failure of modal
truncation. Entropy feedback, gradient constitutive errors, physical q2
matching, scalar reaction and finite-A news remain uncontrolled. No third
mode, closure repair, spectral scan or implicit budget extension is made.

Reproduction:
`python3 verification/goal004_two_mode.py > verification/goal004_two_mode_output.txt`.
Two additional small effective ODEs use 384 angular quadrature points;
they consume no physical Einstein/master-sector configuration budget.
The code preserves all earlier one-mode negative findings. See the
separate audit's final two-mode inspection for its independent formula
and arithmetic checks. This is the stopping point of the model track.
