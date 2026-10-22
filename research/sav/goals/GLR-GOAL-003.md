# GLR-GOAL-003 — Forced master, nonlinear closure and early radion radiation

**Task:** T-20261022-GLR-GOAL-003  
**State:** PREPARED / READY FOR CODEX — NOT LAUNCHED  
**Repository:** inaeyk/GL-with-AI  
**Only writable branch:** research/sav-radiation  
**Only writable paths:** research/sav/**  
**Owners:** Savdeep Sethi and Michael Yan; Pau Figueras advises.  
**Preparation baseline:** 554ceb968fb3f2549a80f1b1e953810d9e79744a (2026-10-22).  
**Companions:** [acceptance gates](GLR-GOAL-003_REVIEW_GATES.md),
[launch text](GLR-GOAL-003_LAUNCH.md), [status](../STATUS.md).

## 1. Scientific objective and authority

Push the analytical treatment of the initial Gregory–Laflamme transition
as far as justified, starting with the missing **forced second-harmonic
gauge-invariant master problem and its horizon-regular inverse**. Use that
result to control quadratic backreaction, construct exact admissible
finite-time GL data, and seek a nonlinear error theorem and a nonzero
finite-amplitude interval for early radion news and energy. Pursue cubic
dynamics, fourth-order scalar radiation, and a controlled relation to
initial bulge formation as substantive parts of the program.

The central prize is a theorem or reproducible certified bound for at
least one specified physical data family, not another formal inverse,
source calculation alone, or fitted emitted percentage. Exact analytical
representations and effective bounds count even when elementary closed
forms do not exist. A precise obstruction is valuable but is a partial
result, not completion of the central goal.

The owner's observation that a realistic radion will need a small mass
is a **separate stabilization/EFT extension**, developed in section 10.
The core theory remains five-dimensional vacuum Einstein gravity,
$R_{AB}=0$. No mass term is inserted into those equations. Keep vacuum
results, EFT assumptions and model-dependent corrections visibly separate.

This document prepares future work only. It launches no Codex task,
agent research run, calculation, evolution or simulation. When the owner
explicitly launches the companion prompt, non-production derivations and
the bounded checks below are in scope. Michael's production code,
equations, parameters, branches, workflow and running work are untouched.
No manuscript or Overleaf access, merge, production evolution, MPI/AMR,
CCZ4/GRChombo run, cluster submission, or subsequent Goal is authorized.

## 2. Audited starting point: what GOAL-001/002 actually achieved

Read AGENTS.md, NOTATION.md, STATUS.md, SOURCES.md; both prior reports;
physics/gl_linear_mode.md, quadratic_radion_source.md,
radion_retarded_transfer.md, goal002_second_order.md,
goal002_finite_data_energy.md; the four prior mathematical audits;
verification/INDEX.md and negative_results.md. Preserve their evidence
classes and negative findings. Historical reports are not to be rewritten.

| Input | Established in the records | Still missing |
|---|---|---|
| GOAL-001, core 7110bc2cb3c0496dd62dc97d37b34d11ec780a17, closure ec39904b833e5b7f9f32b80322bee298aaac7fc7 | Reduced action, normalized linear mode, completed quadratic radion source, retarded overlap, independent audits | Complete rigorous mode/overlap enclosure and finite-amplitude radiation theorem |
| GOAL-002, science 6eb617e4d22b1ca8e8641dcf1f80ca5d9c64e681, closure 554ceb968fb3f2549a80f1b1e953810d9e79744a | All quadratic equations/constraints, zero-mode EF quadratures, forced n=2 trace/divergence, exact scalar-only data, finite-slice scalar energy inequality | Evaluated n=2 inverse/global norm, exact simultaneous GL-plus-scalar data, fourth-order radiation remainder, finite GL amplitude interval |
| Finite-data non-universality | Demonstrated at constrained second order; exact scalar-only family exists | Exact GL-family extension needs constraint integrability and radiation-map regularity |
| Numerical diagnostics at L/r0=10 | Omega*r0 approximately 0.06328118898, C0 approximately 12.30729198; unit P/Z mismatch Q approximately 80.8179176 | These are empirical inputs, not interval bounds or a certified settling/bulge window |

Both goals ended **MEANINGFUL_PARTIAL**. No controlled positive GL
amplitude interval or first-bulge emitted fraction is established.
The roughly 32.2843 r0 transient-only comparison window and sampled
first-order regular-frame norm roughly 39.47 are diagnostics, not new
physical limits. Reuse valid calculations; do targeted independent
checks of interfaces, not wholesale repetitions of GOAL-001/002.
The D16 projected constraint failure and M2-C failed GP control remain
historical negatives and supply no physical GL acceptance.

## 3. Locked conventions and exact symmetry

Use signature (-++++), $G_4=G_5/L$, $M_0=r_0/(2G_4)$,
$f=1-r_0/r$, $k_n=2\pi n/L$, and
$x=r+r_0\log(r/r_0-1)$. The expansion has **no factorials**:
$g=\bar g+\epsilon h_1+\epsilon^2q_2+\epsilon^3q_3+\cdots$.
A real $A\cos kz$ has complex coefficients $A/2$ at n=+1 and -1.
The reference mode has $H^U_{\theta\theta}/(2r^2)=1$ at $r=2r_0$,
$A(t)=\epsilon e^{\Omega t}$ after fixing the reference clock/origin.
It is neither the numerical chi seed nor a finite horizon bulge amplitude.
Use $v=t+x$ and the regular initial slice $\tau=v-r$ where appropriate.
Keep fixed asymptotic circle, clock, declared ADM-mass convention and
zero KK momentum; do not impose an independent arbitrary regular tension
hair. Define any nonlinear amplitude through invariant geometry or a
complete recursive gauge/clock prescription.

**Exact SO(3) is an appropriate and retained sector for this program.**
It acts on the transverse round two-sphere, not on the compact circle.
The exact ansatz is

$$ds_5^2=g_{ab}(t,r,z)dx^a dx^b+R(t,r,z)^2d\Omega_2^2,
\qquad a,b\in\{t,r,z\}.$$

Angular $\ell$ and compact Fourier $n$ are different labels. Products
of SO(3) singlets remain singlets at every nonlinear order. Thus the
four-dimensional **massless spin-2 Bondi news is identically zero**
in this sector; spherical metric backreaction and scalar energy loss
are allowed. The zero-mode KK vector has no radiative monopole;
reflection-symmetric uncharged data also fix its charge/momentum freedom.
The n=0 radion is a scalar and can radiate in $\ell=0$, including when
stabilization makes it massive. SO(3) does not forbid scalar monopole
radiation and is not a reason to impose a quadrupole selection rule.
Nonzero-n gravitational singlets can carry the scalar-type/helicity-zero
sector of massive KK gravity; do not set all their components to zero
by importing the massless spin-2 argument. Their flux normalization
must be derived from the constrained gravitational action/current.

For the vacuum pure fundamental seed, Fourier convolution gives n=0,±2
at order two; n=±1,±3 at order three; n=0,±2,±4 at order four.
These are the generated harmonics when other independent free harmonics
are set to zero by the chosen data family; any additional free harmonics
must be included explicitly. Half-period covariance fixes parity
$n\equiv\text{order}\pmod2$, not the bound $|n|\le\text{order}$.
Impose the all-orders family symmetry
$g(-\epsilon,z)=g(\epsilon,z+L/2)$, with compatible free data, to justify
even powers in the zero-mode radion. If a wider data class breaks that
relation, state the modified ordering instead of assuming it. Exact
SO(3) excludes nonspherical instabilities and tensor emission from the
scope; no conclusion about their size in unrestricted evolutions follows.

## 4. A — Derive the genuinely forced gauge-invariant master

Let $K=2k$, $s=2\Omega$. GOAL-002 gives the unitary-completed
second-order sector, in complex Fourier convention,

$$\tau_2=\bar g^{\mu\nu}q^U_{\mu\nu,2}
=-\frac{2Q_{zz,2}}{K^2},\qquad
j_{\mu,2}=\bar\nabla^\nu q^U_{\mu\nu,2}
-\bar\nabla_\mu\tau_2=-\frac{2Q_{\mu z,2}}{iK},$$

$$[\bar\Box_4+2\bar R(\cdot)-K^2]q^U_{\mu\nu,2}
=2Q_{\mu\nu,2}+\bar\nabla_\mu\bar\nabla_\nu\tau_2
+\bar\nabla_\mu j_{\nu,2}+\bar\nabla_\nu j_{\mu,2}.$$

Here $Q=R^{[2]}[h_1,h_1]$, with consistent compensators and Fourier
projection. The conserved quadratic Einstein source is
$Q_{AB}-\bar g_{AB}Q^C{}_C/2$; Ricci Q alone is not conserved.
The forced field is **not homogeneous TT**.

1. Derive an explicit master variable $\Psi_2$ invariant under the
   second-order gauge transformation, including the first-order
   compensator's quadratic Lie terms with the no-factorial convention.
   Relate unitary completion, regular EF variables and the physical
   reduced degree of freedom. Prove that the chosen gauge representative
   exists on the stated domain and respects the boundary/charge data.
2. Eliminate the inhomogeneous trace/divergence and nondynamical fields
   without dropping source terms. Produce explicit coefficients and
   an explicit source differential functional, schematically
   $\mathcal L_{K,s}\Psi_2=\mathcal S_2[H]$ and
   $q^U_2=\mathcal R_{K,s}\Psi_2+\mathcal P_{K,s}[Q]$.
   This schematic notation is a deliverable specification, not a result.
   Recover the existing homogeneous master at k=K and growth parameter s.
   Where possible reduce the on-shell quadratic source to explicit
   coefficients multiplying $y_1^2$, $y_1y_1'$ and $(y_1')^2$. The source
   lift must not conceal another unevaluated inverse.
   Determine whether a regular scalar second-order equation suffices;
   if it does not, identify the precise obstruction and the minimal
   equivalent constrained system instead of asserting a fake scalar master.
3. Give the inverse map and equivalence theorem: every admissible master
   solution reconstructs all seven Einstein components/constraints, and
   all physical solutions are represented modulo declared homogeneous
   data/gauge/charges. Check nonzero-K factors, exceptional denominators,
   apparent interior singularities, pure gauges and missing kernels.
4. Independently obtain the equation from the reduced action/constraint
   elimination or a distinct covariant tensor route. Test complete
   residuals and source Bianchi identities on general on-shell data when
   feasible, not only the old imaginary-k algebra specialization.

**A deliverable:** explicit source-aware master and reconstruction,
regular gauge domain, residual identities and independent derivation.
Naming an inverse or replacing k by 2k in a homogeneous TT equation
does not pass A.

## 5. B — Construct and bound the horizon-regular inverse

The primary reference is $L/r_0=10$, where historical numerical evidence
places $K r_0=4\pi/10$ above the GL threshold. That observation alone
does not prove coercivity, nonresonance or a bound on the inverse at s.

1. Derive horizon Frobenius/Volterra data in a future-regular frame,
   with all reconstructed tensor components and required derivatives
   smooth. Finite $\Psi_2$ alone is insufficient. Treat special indicial
   differences/logarithms explicitly and verify the allowed branch.
2. Derive the full forced infinity expansion, including powers and
   possible logarithms multiplying $e^{-\alpha r}$, where
   $\alpha=\sqrt{K^2+s^2}=2\sqrt{k^2+\Omega^2}$.
   This coincides with the quadratic source exponent; determine the
   actual resonance coefficients. Do not impose a source-free Robin
   tail or claim resonance automatically creates a global kernel.
3. Construct fundamental solutions, a Green kernel/variation-of-constants
   formula and source overlap with a checked Wronskian sign and measure.
   A Sturm–Liouville/Schrödinger form is useful if it exists with controlled
   field redefinition; otherwise use a proven equivalent first-order
   system and its adjoint boundary pairing. Include homogeneous data.
4. Prove absence of a compatible kernel at $(K,s)$, or expose a genuine
   solvability condition, pole and required secular response. Preferred
   approaches: analytic coercivity/S-deformation or comparison estimates;
   constructive Volterra bounds; certified Wronskian/resolvent enclosures.
   A floating nonzero Wronskian, threshold plot or absence of a sampled
   growing eigenmode does not establish the needed lower bound.
5. Define dimensionless source/solution spaces and prove an effective
   bound $\|q_2\|_X\le C_{\rm inv}\|Q_2\|_Y$ with source derivatives,
   reconstruction, endpoint/matching constants and any derivative loss
   explicit. A bound for $\Psi_2$ alone is not a metric bound. Separate
   compact horizon collar, intermediate region and weighted exterior.
6. Distinguish the Laplace-point exponential particular inverse from the
   retarded initial-value propagator. Extend to a controlled resolvent
   region or energy/semigroup estimate if needed for finite-time data;
   one value s does not bound arbitrary transients or nonlinear forcing.
   Identify continuum thresholds, possible quasi-bound/resonant behavior,
   horizon absorption and spectral constants rather than presuming a gap.

**B deliverable:** evaluated analytical representation plus a proven or
certified finite inverse/reconstruction bound, with endpoint remainders.
If only the representation is achieved, label the quantitative gate open.
The inverse is a new central problem; GOAL-002's two equation-level
attempts do not constitute proof it is impossible.

## 6. C — Complete quadratic backreaction and exact finite-slice data

Combine B with the existing n=0 scalar Green response and Einstein-frame
mass/lapse quadratures. Bound the complete regular-frame second-order
metric, derivatives, circle radius, charge shifts and matching data;
control $M_2$ in a specified norm, with certified source/mode input errors
where quantitative certification is claimed. Verify angular/evolution
equations as well as radial constraints. Separate static charge choices,
independent scalar data and independent n=2 gravitational transients.

Construct an **exact simultaneous GL-plus-scalar initial-data family** on
a specified smooth horizon-crossing slice. Begin with the physical
normalized growing mode, not an arbitrary time-switched metric. Specify
the seed's normal derivative, decaying/other mode projections, free data
at every order, asymptotic L/clock, reflection symmetry and charge policy.
Particular-matched P and zero-scalar Z may serve as two second-order
targets, but first complete one full family; do not superpose the existing
exact scalar-only family nonlinearly with a GL solution.

Use the exact SO(3) constraints and a chosen elliptic/conformal or direct
regular-slice formulation to seek a charge-fixed right inverse. Identify
Killing-initial-data/adjoint kernels, boundary terms, horizon conditions,
mass/tension compatibility, and function spaces before invoking an
implicit-function, contraction or tame-inverse theorem. Prove that the
first derivative is the requested GL seed and the second-order data
agree with the chosen response. Treat higher-order scalar free data
explicitly: fixing h1 and q2 alone cannot bound an arbitrary N4.
If fixed ADM mass obstructs a chosen parametrization, first diagnose the
finite-dimensional charge freedom and document an allowed family; never
silently change what mass enters the energy fraction.

**C deliverable:** full $q_2$ control and a reconstructable exact data
prescription, or a precise compatibility/inverse obstruction. Establish
non-universality for exact GL families only after this construction and
radiation-map regularity; the constrained second-order result already exists.

## 7. D — Third/fourth order and nonlinear finite-amplitude closure

This is an ambitious analytical program. **Gate conclusions, not all
independent algebra:** formal higher-order equations and candidate estimates
may be derived while B/C constants are unresolved, with every dependency
explicit. Do not stop useful analytical work solely because a numerical
inverse bound has not yet been obtained; do not present formal work as closure.

1. Derive the n=±1,±3 third-order forcing and the fourth-order n=0
   completed radion equation/current, retaining contributions from
   h1–q3, q2–q2, h1–h1–q2, quartic h1 terms, gauge completion and
   Einstein-frame/asymptotic coordinate shifts. Track all harmonics
   needed for the zero-mode projection. Include free higher-order data,
   charge corrections, horizon flux and the fourth-order Bondi balance.
2. Seek a fixed-family bound on $N_4$ or directly on the full radiation
   remainder, rather than computing huge expressions without estimates.
   An alternate nonlinear energy/majorant/contraction proof may replace
   explicit N4 if it controls the same observable and order.
3. Formulate the exact SO(3) quasilinear initial-boundary/exterior problem
   in a horizon-regular well-posed gauge. Establish constraint propagation,
   redshift control near the horizon, exterior weighted energy and a
   radiation trace bound. Repair derivative loss by a justified energy
   hierarchy/tame argument; do not assume a bounded Banach inverse for
   a quasilinear operator that loses derivatives.
4. Seek finite-time stability about the growing approximation and, if
   possible, control through an instability-time window
   $T\sim\Omega^{-1}\log(A_*/\epsilon)$, not just a fixed time theorem
   whose constants fail before appreciable growth. State uniformity in
   epsilon, horizon regularity, trapping/exterior losses and the exact
   domain on which the comparison holds.
5. Derive explicit sufficient inequalities using nondegenerate regular
   norms, for example

   $$A_{\max}M_1+A_{\max}^2M_2+R_{\ge3}(A_{\max},T)<\eta_g<1,$$
   $$A_{\max}M_2\le\eta_h m_1,\qquad
   \eta_{\rm trans}+\eta_{\rm nonlinear}+\eta_{\rm input}
   +\eta_{\rm inverse}+\eta_{\rm extraction}\le\sqrt{1.1}-1.$$

   Here $m_1$ is a lower bound, $M_j$ upper bounds, and each eta is
   defined in the same news norm relative to a nonzero reference signal.
   Derive any replacement inequalities. Prove the nonlinear existence/
   contraction/bootstrap and radiation trace estimate behind them;
   small metric entries or a horizon-degenerate scalar Killing norm
   are insufficient. Report a quantitative positive A*, or distinguish
   a qualitative existence theorem, conditional inequalities and an
   unresolved theorem. Never pick A*=0.1 by convention.

**D deliverable:** the strongest exact or certified nonlinear remainder
and finite-amplitude result, with all constants/data dependencies. A
complete core result needs a reproducible nonzero interval and a controlled
observable, not just positive formal powers of A.

## 8. E — Nonlinear dynamics and the initial-bulge bridge

Derive the adjoint projection/solvability law for a physically fixed GL
amplitude, retaining zero-mode charge evolution, massive sector response,
horizon dissipation, radiation reaction and independent data. Investigate
a local law $\dot A=\Omega A+\gamma A^3+\cdots$ only with a justified
projection, gauge/clock definition and control of eliminated modes.
Under $B=A+cA^3$ and $d\tau'/dt=1+dA^2$ the historical convention law
is $\gamma_B=\gamma+2c\Omega-d\Omega$; a number gamma without these
choices is not an invariant prediction.

If the massless continuum, charge modes or long-lived transients obstruct
a memoryless reduction, derive a Volterra/memory equation with its kernel
and data terms, and bound any local approximation to it. A slow-time or
near-threshold scaling must declare detuning/order counting and uniform
errors; do not import static nonuniform-string coefficients as dynamical
saturation coefficients. Static branch matching is an independent check
in the appropriate zero-frequency limit, not proof of nonlinear stability.

Seek an invariant mapping between the reference amplitude and horizon
geometry (areal bulge/neck contrast on a specified slice). Define a
first-bulge event before estimating its radiation; distinguish apparent
and event horizons, slicing dependence, source time and retarded arrival.
Attempt analytic continuation/majorants, matched near-threshold expansions
or rigorous upper/lower energy bounds if a controlled route reaches that
event. A resummation without an error theorem is exploratory. A breakdown
criterion before the event is a useful result. Neither A~1 extrapolation,
an AH-area comparison, nor static entropy data supplies a first-bulge
emitted percentage. Early dominance additionally needs control of later
emission and remains unproved if that denominator is unavailable.

## 9. F — Early massless radion news, energy and spectrum

Preserve the vacuum second-order source and Bondi normalization,

$$\bar\Box_4\phi_2=-\frac{\sqrt3 k^2}{16}A(t)^2
H_{\mu\nu}H^{\mu\nu},\qquad
\phi_0=\frac{\Phi(u)}{\rho}+O(\rho^{-2}),\quad
N_\phi=\partial_u\Phi,\quad P_0=\frac{N_\phi^2}{G_4}.$$

For a specified exact family compute/bound the causal finite-data response,
its independent homogeneous news, interference, and nonlinear correction.
An eternal exponential is a reference particular solution; a no-incoming
condition does not remove independent outgoing Cauchy data. A nonzero
source need not have nonzero outgoing overlap; retain cancellation tests.

For a finite observation interval U, use

$$\frac{E_U}{M_0}=\frac{2}{r_0}\int_U N_\phi^2du,\qquad
\frac{|E_U-E_{p,U}|}{E_{p,U}}\le2\eta+\eta^2,
\quad\eta=\frac{\|N_\phi-N_p\|_{L^2(U)}}{\|N_p\|_{L^2(U)}}.$$

The inequality requires a positive reference denominator. The comparison
also admits an absolute enclosure: if $b_U\ge\|N_\phi-N_p\|_U$,

$$\frac{2}{r_0}\max(0,\|N_p\|_U-b_U)^2
\le\frac{E_U}{M_0}\le\frac{2}{r_0}(\|N_p\|_U+b_U)^2.$$

This form remains useful near cancellations. The settled
quadratic template gives
$E_{p,U}/M_0=2(\Omega r_0)C_0^2(A_2^4-A_1^4)$ only under its stated
particular-response conventions. Combine certified input/transfer errors,
the finite-slice transient bound and D's nonlinear error in one budget;
do not spend the entire tolerance on transients. Seek a strictly positive
lower bound on emitted energy if possible, not only an upper bound.
If the signal cancels or is dominated by chosen data, report that outcome.

Derive the spectrum from actual finite-energy news with a smooth window
$w\in C_c^\infty(U)$:
$dE_w/d\omega=|\widetilde{wN_\phi}(\omega)|^2/(\pi G_4)$ for positive
frequency. Check Parseval and absolute errors at spectral zeros. Do not
interpret a hard temporal cutoff or a switched GL metric as a physical
source spectrum. Give the strongest energy/window/spectrum that the
data, causal mapping and error theorem actually support.

## 10. S — Separate realistic small-mass stabilization/EFT extension

This extension is required as a consistent analytical specification and
as a radiation-transport analysis. A microscopic stabilization model or
numerical phenomenological prediction is conditional on additional input.
Do not choose an observational mass value or claim a universal mechanism.

**Theory distinction.** Pure five-dimensional vacuum reduction has a
massless modulus. A radion potential requires additional fields, quantum
effects or other stabilization physics, with its own stress tensor and
matching. Use a separately labeled four-dimensional Einstein-frame EFT,
for example in the uncharged sector,

$$S_{\rm EFT}=\frac1{16\pi G_4}\int d^4x\sqrt{-g_E}
\left[R_E-2(\nabla\phi)^2-4U(\phi)\right]
+S_{\rm KK,match}+S_{\rm higher},$$
$$U(0)=U'(0)=0,\qquad U''(0)=m_\phi^2>0.$$

The conventions imply $\Box\phi-U'(\phi)=J_{\rm match}$ and
$U=\tfrac12m_\phi^2\phi^2+\cdots$ near the minimum; the canonical field
is $\varphi=\phi/\sqrt{4\pi G_4}$. Derive the stress tensor, metric
equations and energy exchange consistently. If asymptotic U(0) is not
tuned to zero, the asymptotic geometry and flux definitions must change;
that is outside the KK-flat baseline. $S_{\rm KK,match}$ denotes explicit
KK degrees of freedom or a justified matched source, not an assumption
that a zero-mode EFT by itself describes the z-dependent GL instability.

**Matching and control.** State a hierarchy for $m_\phi r_0$,
$m_\phi/k$, $m_\phi/\Omega$, source bandwidth, propagation distance/time
and the EFT cutoff/stabilization scale. Small $m_\phi r_0$ alone does not
make the transport massless: $m_\phi/\Omega$ may be order one or larger.
Integrating out KK fields requires a different regime from keeping the
unstable n=1 mode; use a near-zone five-dimensional description matched
to the far-zone EFT, or an explicitly retained tower with a justified
cutoff. Bound corrections to the background, mode/growth rate, quadratic
source, constraints and charges. Reusing the vacuum source as
$J_{\rm match}$ is a named leading approximation with a matching error,
not an exact equation of a stabilized five-dimensional vacuum theory.
Preserving Fourier-n selection requires z-translation-invariant
stabilization. Translation-breaking stabilizers and their altered n mixing
are outside this Goal and require a separately scoped extension. SO(3)
selection persists whenever the stabilizer and its data preserve SO(3).

**Massive scalar transport under exact SO(3).** Derive the retarded
massive radial operator and its Green function, threshold, branch cut,
tails, evanescence, horizon absorption and any quasi-bound contributions.
For a free minimally coupled far-zone scalar on the Schwarzschild
background the candidate potential is
$V_m=f(r)[r_0/r^3+m_\phi^2]$ for $\psi=r\phi$ at $\ell=0$;
verify it and delimit corrections. Real asymptotic frequencies obey

$$p(\omega)=\sqrt{\omega^2-m_\phi^2},\qquad
|\omega|>m_\phi,\qquad v_g=p/|\omega|<1.$$

Above-threshold monopoles are allowed; below-threshold monochromatic
fields are evanescent at spatial infinity, though horizon absorption
and transient/near-zone energy remain possible. $e^{2\Omega t}$ is an
imaginary-frequency particular forcing, not a real radiative frequency;
do not apply the criterion $2\Omega>m_\phi$ as an emission switch.
A finite-time causal source has a real-frequency spectrum that must be
projected through the massive retarded propagator.

Derive outward energy from the action through large **timelike** worldtubes,
with outward sign and redshift fixed. Asymptotically the scalar contribution
is $P_R=-R^2\partial_t\phi\,\partial_R\phi/G_4$ in asymptotically
Minkowski coordinates; include finite-radius storage and horizon flux
in energy balance. For $\phi=\Re[b(\omega)e^{-i\omega t+ipR}]/R$,
the asymptotic monochromatic average is
$\langle P\rangle=\omega p|b|^2/(2G_4)$ for positive omega, subject
to the stated real-amplitude convention. Derive the corresponding
finite-pulse spectral measure instead of copying the massless news law.
At fixed positive mass, finite-energy massive waves have no usual
massless $1/R$ news carrying energy through future null infinity;
use timelike propagation/scattering limits. Examine the noncommuting
limits $m_\phi\to0$, $R,t\to\infty$ and fixed retarded time, and
state where the massless result is a controlled approximation.

The vacuum n=2 massive **gravitational** channel is separate: its
asymptotic threshold is $|\omega|>2k$, with its own constrained canonical
current. Do not automatically replace that mass by
$\sqrt{(2k)^2+m_\phi^2}$; such a spectrum requires a specified EFT
field content and matching calculation. Neither scalar mass nor KK mass
restores SO(3)-forbidden massless tensor news.

**S deliverable:** separately labeled action/assumptions, matching/error
inventory, scalar propagation and flux derivation, regime/limit map,
and explicit unknown stabilization inputs. Provide conditional analytic
suppression/delay estimates where possible. Do not present an EFT
phenomenology number as a vacuum result or silently alter NOTATION.md's
baseline action.

## 11. Method, scope and finite stopping discipline

- Prioritize analytical identities, exact Green/Volterra representations,
  coercivity/comparison estimates, asymptotic bounds and nonlinear energy
  arguments. Numerical fitting cannot replace a missing proof.
- Reuse L/r0=10 profiles/results with their provenance. No new physical
  wavelength survey or independent multi-unstable-mode campaign. Other
  k may enter symbolic limits; L16's independently unstable n=2 requires
  a separate multimode problem and is not the default calculation.
- A future launch may use exact symbolic scripts and manufactured tests;
  retain Python/Mathematica text sources. If needed, use at most three
  bounded 1D refinements per genuinely new operator/sector. Declare the
  finite list and error target before running them. Permit a targeted
  certified re-enclosure of existing L10 inputs when essential; no new
  production or spacetime time-evolution simulations. The EFT track is
  primarily symbolic/asymptotic; no mass-parameter survey.
- For each central failed gate use at most two substantive, distinct
  repair approaches. Record the failed estimate, hypotheses and evidence.
  Continue other already-scoped independent analytical branches with
  conditional labels; stop dependent physical claims. This is not a
  license to repeat parameter/refinement sweeps or abandon necessary
  reasoning to save model calls.
- Delegate bounded scientific questions selectively. Require two distinct
  adversarial audits: geometry/master/inverse, and data/nonlinear/flux
  including EFT/symmetry. Audits must check equations and intentionally
  incorrect variants, not just prose. Document if independent audit is
  unavailable; do not call self-review independent verification.
- Preserve assumptions, negative results, source provenance, uncertainty,
  exact commands/versions when run, residuals and reproducible derivations
  in Markdown with LaTeX math. No result promotion from a test PASS alone.

## 12. Deliverables, gates and honest completion

Create coherent physics notes under research/sav/physics for the master/
inverse, nonlinear/data closure and the separate stabilized-radion EFT;
retain corresponding verification sources/records and two audits.
Write GLR-GOAL-003_REPORT.md only after execution, with a gate-by-gate
assessment, quantitative intervals/errors if obtained, explicit open
constants and one concrete next mathematical strategy if incomplete.
Update STATUS.md, verification/INDEX.md, SOURCES.md and the existing
science-only handoff at meaningful milestones, not on every tool call.
Use existing record structure; do not invent duplicate HANDOFF/NEXT files
where none exist. Publish coherent research-only commits with exact
provenance and expected-head/fast-forward checks. Never merge or force push.

**Core COMPLETE_VERIFIED** requires A/B master/inverse equivalence and
effective bound, C exact fixed-family data and full quadratic control,
D a nonzero quantified finite-amplitude domain, F controlled early news/
energy, S a consistent separate EFT specification/transport analysis,
and both independent audits. Report E's cubic/nonlinear-bulge reach
separately; first-bulge coverage is a stretch result, never implied by
core completion. A microscopic stabilization model is not required for
the vacuum theorem, and absent matching input the EFT output remains
conditional. A correct inverse without nonlinear closure earns
MEANINGFUL_PARTIAL, not a weakened definition of complete.

Other terminal labels: BLOCKED_BY_MATHEMATICAL_OBSTRUCTION (identify the
actual obstruction, not mere difficulty), NOT_REPRODUCIBLE, or
OPEN_NEEDS_APPROVAL only for genuinely out-of-scope work. Do not claim
impossibility from two failed methods. Report no first-bulge percentage
unless the data-to-geometry-to-flux bridge and errors have actually closed.

## 13. References and preparation ledger

Repository equations and proof status are pinned to the baseline above;
see [SOURCES.md](../SOURCES.md) for detailed inspected scope.
[Collingbourne, sections 3.2–3.4 and 4.1](https://arxiv.org/html/2007.08441)
provides a linear master/regularity benchmark; it does not solve our forced
inverse. [Gubser, sections 3.4–3.5](https://arxiv.org/pdf/hep-th/0110193)
provides static harmonic/charge comparisons, not a dynamical gamma.
[Figueras et al., III.1](https://arxiv.org/html/2210.13501v2) supplies
geometric and radiation-motivation context, not a measured early radion
fraction. The stabilization action above is a proposed normalized EFT
specification; its model-dependent matching remains to be supplied.
[Brito, Cardoso and Pani, III–IV, equations (26)–(30)](https://arxiv.org/html/1304.6725v3)
offers a homogeneous massive spin-2 monopole comparison; its TT constraints
do not solve the forced system. [Koyama and Tomimatsu, II,
equations (4)–(9)](https://arxiv.org/html/gr-qc/0103086v2) supplies the
massive Schwarzschild scalar operator/Green-function benchmark; the
source, flux normalization and matching here need their own derivation.

| State | Evidence / next action |
|---|---|
| Prepared 2026-10-22 | Prior committed reports/derivations reviewed; bounded independent planning reviews; documentation only |
| Execution | NOT STARTED; no new scientific checks, simulations, or physical result claimed by this preparation |
| Next action | Owner may paste GLR-GOAL-003_LAUNCH.md into a chosen Codex chat; the ready pointer itself authorizes no execution |

Append execution milestones here only after an explicit launch, recording
actual evidence, paths, errors, audit status and exact commit references.


## 14. Execution ledger — explicitly launched 2026-10-22

The owner launched T-20261022-GLR-GOAL-003 in this chat on the requested
branch. The preparation statements above remain the historical launch
contract, not the current task state. Actual input head:
`ef646d56ab7d8bebca578afab53e62da5146ec05`, tree
`97ddd8265a3b961d3999867ac800de29388df2f2`.

Execution delivered the explicit forced master/all-constraint reconstruction,
analytic nonresonance/effective regular inverse, exact qualitative local
data, formal higher sources/memory, linear AH map and separate consistent
massive-radion EFT. Two distinct adversarial scientific audits executed
checks and wrong cases. Full global data, physical M2 and nonlinear
radiation/A* remain open. Terminal **MEANINGFUL_PARTIAL**, under the
unchanged definition in section 12. See [report](GLR-GOAL-003_REPORT.md),
[checks](../verification/GLR_GOAL003_CHECKS.md), [input/publication
provenance](../SOURCES.md) and the existing science handoff. No physical
ODE/production simulation, manuscript, Overleaf or subsequent Goal.
