# GLR-GOAL-002 report: constrained early-radion analysis and its stopping point

**Was a nonzero finite-A controlled GL interval established? NO.**

**Was an actual first-bulge radiated fraction established? NO.**

**Terminal classification: MEANINGFUL_PARTIAL.**
Task T-20261022-GLR-GOAL-002, executed 2026-10-22 in
inaeyk/GL-with-AI, branch research/sav-radiation. No production simulation,
Michael code/parameter change, manuscript/Overleaf work, merge or subsequent
Goal was performed. All published paths are under research/sav/.

## Scientific result

The independently rechecked GOAL-001 quadratic radion source survives:

$$\bar\Box_4\phi_2=-\frac{\sqrt3k^2}{16}A(t)^2H_{\mu\nu}H^{\mu\nu},
\qquad \phi_2=\frac{\sqrt3}{4}\langle q^U_{zz}\rangle.$$

New work derives the full second-order SO(3) source/equation generator,
including all metric constraints and the n=±2 forced trace/divergence,
and gives zero-mode Einstein-frame mass/lapse quadratures in future-regular
areal EF gauge. It obtains a positive scalar Killing-energy budget on a
finite horizon-crossing slice, reconstructs its L10 value for the
particular-versus-zero-scalar-data mismatch, and derives a quantitative
conditional finite-window energy-error inequality. It also constructs
an exact scalar-only KK-vacuum initial-data family, independently audits
it, and establishes leading energy-data dependence at the **constrained
second-order** level. Extension of that non-universality statement to
exact simultaneous finite-amplitude GL families is conditional on a
constraint-integrability/radiation-map theorem not supplied here.

The complete n=2 boundary response has not been evaluated. An exact
simultaneous GL-seeded initial-data family and a bound on the next even
scalar news have not been constructed. The primary finite-A bridge remains
open; the following results do not meet COMPLETE_VERIFIED.

## Assumptions, gauge and physical observables

Signature (-++++), G4=G5/L, M0=r0/(2G4), real harmonic cos(kz),
k=2pi/L and no factorial in q are preserved. Nonzero harmonics use their
unitary completion hU_zA=0; first-order TT follows from the Einstein
constraints. The amplitude has Htheta theta/(2r²)=1 at r=2r0 and fixed
asymptotic clock/tortoise x=r+r0 log(r/r0−1). It is neither chi nor a
finite horizon-bulge distortion. The regular finite initial slice is
tau=v−r, v=t+x; at the reference radius tau=t. Higher-order comparison
uses an explicitly stated recursive gauge-plus-clock prescription, not
an unfixed raw metric average.

At Bondi infinity phi0=Phi(u)/rho+..., news N=dPhi/du,
P=N²/G4 and E/M0=(2/r0)int N²du. The 4pi sphere and L factors,
real-cosine halves, Lorentz contraction sign and Green Wronskian sign are
checked independently. n=2 forced imaginary-frequency fields are not
propagating massive radiation. Real massive waves need |omega|>2k and
their own timelike flux; no massive null-infinity energy is added here.

## Complete equations versus completed solutions

[The geometric derivation](../physics/goal002_second_order.md) gives
the exact W=R² reduced equations and their explicit linear/quadratic
coefficient formulas, all seven independent components, source Bianchi
identities, regular-frame reconstruction, scalar completion, stationary
charge freedoms and boundary conventions. In particular, with
chi=qzz,0 and pE=qmu nu,0+chi gbar/2,

$$\delta G^{(4)}_{\mu\nu}[p^E]=\mathcal T_{\mu\nu},\quad
\mathcal T_{\mu\nu}=-Q_{\mu\nu,0}
+\tfrac12\bar g_{\mu\nu}(Q^\alpha{}_{\alpha,0}+Q_{zz,0}),$$
$$\beta=-\tfrac12\int_r^\infty R\mathcal T_{rr}dR,\qquad
\mu=\delta m+\tfrac12\int_r^\infty R^2\mathcal T^v{}_v dR.$$

Conserved source, angular/evolution equations and boundary flux are
essential for these quadratures; they are retained explicitly.
Fixed total ADM mass selects delta m=0 at quadratic order. Homogeneous
static scalar c_infinity+c log f is excluded by fixed L and horizon
regularity, so arbitrary independent regular tension hair cannot be added.
Independent scalar Cauchy waves remain allowed and have news.

For K=2k, the second-order unitary sector obeys

$$\tau=-2Q_{zz,2}/K^2,\quad
j_\mu=-2Q_{\mu z,2}/(iK),$$
$$[\bar\Box_4+2\bar R(\cdot)-K^2]q_{\mu\nu,2}
=2Q_{\mu\nu,2}+\bar\nabla_\mu\bar\nabla_\nu\tau
+\bar\nabla_\mu j_\nu+\bar\nabla_\nu j_\mu.$$

Thus the forced field is **not homogeneous TT**. Qzz has the exact
real-cosine form −k²A² H²(1+3cos2kz)/8. The source and homogeneous
asymptotic radial exponent in n=2 coincide; polynomial/resonant tails
need treatment. Neither the reduced radial DAE nor the alternate tensor
inverse route has supplied a fully evaluated constraint-compatible
Green matrix, endpoint error or global q2 norm. A formal inverse is
clearly labeled unevaluated. This is the substantive failure of Gate B,
not a claim that deriving all equations solves them.

The new exact executable checks every linear reduced equation and all
three quadratic Bianchi identities as rational functions of r at a local
on-shell analytic specialization. That control has kappa=ik=2/3 and
k²<0; it is **not** the physical L10 eigenmode, has a reconstruction pole
outside the algebra's domains, and supplies no physical continuum norm.
The separate audit uses an on-shell flat TT standing wave as another
full-source control. Historical all-parameter linear/source checks were
also rerun. No exact or numerical evidence found a substantive source defect.

## Finite data, a new exact control and non-universality

[The finite-data derivation](../physics/goal002_finite_data_energy.md)
specifies P data (particular-matched scalar) and Z data (zero scalar
Cauchy data), with the same on-shell GL first-order restriction on a
smooth horizon-crossing slice. They are constraint compatible through
second order; an exact nonlinear GL-data completion remains unproved.
They are not formed by temporal switching. Their difference is an actual
homogeneous scalar problem, and its interference with the generated
response is retained in energy.

An exact auxiliary family is constructed in the consistent z-independent
Einstein-scalar reduction, using compact F,P outside the horizon,
phi=delta F and normal derivative Pi=delta P. The Hamiltonian/momentum
constraints reduce to a linear mass ODE and integrating-factor solution.
The pulse source is positive,
S2=r²[(P+r kT0 F')²+f F'²]/2, and with B=int S2dr,

$$0\le\Delta m\le\delta^2B,\qquad
\delta^2<\frac{R_1^2}{2B(R_1+r_0)}$$

is a sufficient nondegeneracy interval. Its exact KK lift supplies smooth
vacuum exterior/horizon-crossing data, with a compensable ADM shift.
This is a scalar-only construction: it proves physical independent scalar
freedom, **not a GL A interval**. Nonlinear superposition with a GL
solution is not used.

At fixed first-order GL projection, adding any homogeneous scalar to q2
changes the leading energy coefficient to

$$\frac{2\epsilon^4}{r_0}
[\|n_{GL}\|_U^2+2b\langle n_{GL},n_f\rangle_U+b^2\|n_f\|_U^2].$$

This proves second-order non-universality; its exact-family extension
requires the missing z-dependent constraint right inverse and radiation
map differentiability. Even fixing all q2 scalar data leaves an arbitrary
order-four homogeneous scalar datum, so no universal bound on N4 follows
from h1/q2 alone over unspecified higher-order families.

## Quantitative result and its precise limits

For a homogeneous scalar difference psi=r phi on Sigma0,

$$Q_\Sigma=\tfrac12\int[(1-r_0^2/r^2)P_\Sigma^2
+(\partial_xF_\Sigma)^2+V F_\Sigma^2]dx,\quad
\|N_f\|_{L^2(\mathscr I^+)}^2\le Q_\Sigma.$$

The inequality is an exact linear scalar energy statement with nonnegative
horizon flux. It needs no pointwise Price law. The Killing norm degenerates
at the horizon and cannot control the full nonlinear regular-frame metric.
For the settled particular template,

$$\frac{E_{p,U}}{M_0}=2\hat\Omega C_0^2(A_2^4-A_1^4),\qquad
\frac{|E_U-E_{p,U}|}{E_{p,U}}\le2\eta+\eta^2,
\quad\eta=\|\Delta N\|_U/\|N_p\|_U.$$

A sufficient ten-percent energy criterion is eta≤sqrt(1.1)−1,
approximately 0.04880885. The exact transient contribution is bounded by
its initial Q; mode, ODE, extraction and nonlinear errors must be added.

| L10 quantity | New computed value / implication | Evidence and limit |
|---|---:|---|
| Reused Omega r0 | 0.06328118897590773 | Existing GOAL-001, empirical mode uncertainty retained |
| Reconstructed C0 | 12.3072919789 | Saved-profile/direct scalar Green check; no continuum interval |
| Q for unit P/Z mismatch on tau=0 | 80.8179175854 | Three floating scalar configurations; not a rigorous upper bound |
| sqrt Q | 8.9898786191 | Conditional integrated transient scale |
| U=[0,T] transient-only 10% criterion | T about 32.2843 r0 | Uses empirical Q/C0, ignores unresolved nonlinear errors |
| Linear growth over that window | amplitude factor about 7.71 | Must still fit below unknown A_* |
| Regular-frame first-order sampled norm | about 39.47 at the future horizon | Not a global/derivative enclosure or a smallness threshold |
| Admissible finite GL A_* | NOT ESTABLISHED | M2, N4 and exact GL-data completion missing |
| First-bulge fraction / full physical spectrum | NOT ESTABLISHED | No justified bulge map or nonlinear waveform |

The coarse/medium/fine Q estimates are
80.8178880546, 80.8179175178, 80.8179175854; C0 estimates are
12.3072932071, 12.3072920853, 12.3072919789. The finite-data note separates
analytic scalar-tail inequalities from floating basis/GL/interpolation/
quadrature and Frobenius errors. W drift and endpoint densities are
diagnostics, not complete error enclosures. Only L10 was used: one scalar
sector with exactly three refinements, no new GL eigenvalue solves, no
other physical k, no n2 physical BVP or spacetime time evolution.

The strongest spectrum statement is the smooth observation-window
functional |Fourier[wN]|²/(pi G4) for w in Cc-infinity(U), with Parseval
and an absolute spectral-error bound from the news L2 error. Actual Z
scattering news, a physical full burst spectrum and rollover are not
computed. Audit II's smooth memory/interference examples are manufactured
controls, clearly separated from a GL waveform.

## Explicit open inequalities and finite stopping budget

An explicit near-region regular Sobolev/Frobenius norm, exterior weighted
energy norm and radiation trace space are defined in the finite-data note.
A sufficient validity/accuracy attempt needs at least

$$A_{max}M_1+A_{max}^2M_2+K_3A_{max}^3<\eta_g<1,
\quad A_{max}M_2\le\eta_h m_1,$$
$$\eta_{trans}+K_4A_{max}^2+\eta_{mode}+\eta_{ODE}
+\eta_{extract}\le\sqrt{1.1}-1,$$

with a lower bound m1 on the first-order norm, an upper bound M2 on the
completed metric, and an actual fourth-order news constant K4 for a fixed
data family. The first-order normalization supplies a lower bound, while
the required upper/higher-order constants remain unbounded. A nonlinear
constraint/evolution inverse and radiation trace estimate are also absent.

Two methods were examined per central obstruction, not an open-ended
campaign: regular reduced DAE versus covariant tensor reconstruction for
n2, and regular-frame hierarchy versus positive scalar energy for the
amplitude gate. The first pair stops before an evaluated inverse; the
second controls first-order size/transients without M2/N4. There is no
proof these estimates are impossible, only precise unclosed requirements.
No third repair, control wavelength or cubic/bulge campaign was launched.

The convention law gamma_B=gamma+2c Omega−d Omega under
B=A+cA³, d tau'/dt=1+dA² is independently checked. No dynamical cubic
coefficient is inferred from a static branch, and no saturation is claimed.
As lower gates failed, third-order forcing/projection and a first-bulge
bridge were not pursued. A nonlocal projected memory equation is a future
organizing option, not a calculated kernel or a proved normal-form failure.

## Two adversarial audits and acceptance matrix

[Geometry audit](../verification/AUDIT_GOAL002_GEOMETRY.md): a separate
scientific AI agent ran 17 independently implemented exact/control checks,
including direct EF curvature, full flat on-shell quadratic Bianchi,
forced n2 identities, component Frobenius limits and periodic EF pure
gauge. Eight deliberately erroneous cases in six groups were rejected.
It also inspected the actual primary note/scripts and independently
reran the sector check. It withholds full Gate B and finite-A claims.

[Energy audit](../verification/AUDIT_GOAL002_ENERGY.md): a second
scientific AI agent ran 18 checks and rejected 11 mutations. Direct
constraints, exact scalar data, finite-slice energy/trace, factors,
interference, smooth Parseval and amplitude conventions were checked.
It corrected an upper-bound denominator in a hierarchy ratio, the use
of componentwise rather than operator smallness, and wording that had
mistaken a sufficient energy condition for a necessary one. Those
corrections are incorporated. These AI audits are not human referees.

| Gate | Terminal assessment |
|---|---|
| A source, normalization, regularity, conventions | Source/regular-frame identities rechecked; nonlinear gauge global/exact-data conditions still open |
| B complete response | FAIL/INCOMPLETE: n0 quadratures and complete n2 equations, unevaluated n2 boundary inverse/norm |
| C finite-A validity | OBSTRUCTION DOCUMENTED: explicit inequalities, no numerical A_* |
| D finite data/transients | Exact scalar-only control; constrained second-order GL non-universality and transient inequality; exact simultaneous GL completion OPEN |
| E cubic/bulge | Not advanced through failed lower gates; convention ambiguity checked; neither gamma nor bulge energy claimed |
| F verification/handoff | Two independent AI audits, executed checks/mutations, negative records and science-only Michael interface delivered |

Exact nonzero-source/zero-overlap controls are retained; the physical
source shows no new cancellation. D16 non-invariant projected constraints
and M2-C failed GP control remain unchanged historical negatives and are
not promoted to physics acceptance. No early-dominance or A~1 extrapolation.

## Artifacts, provenance and a single concrete next strategy

Input: research/sav-radiation@**8d4163cd876b33752f7fb6938bb7861fe221b7c0**.
Controlling goal blob b57fec85b3e801ac4f9ba79f9266f0b18bd746cb;
independent review-gates blob 89f11d758170b8a93d6b35a383af6dc277580f5c.
Historical GOAL-001 core 7110bc2cb3c0496dd62dc97d37b34d11ec780a17 and
closing report ec39904b833e5b7f9f32b80322bee298aaac7fc7 were preserved.
The input tree inventory pins all 54 research blobs and the existing
L10 profile (7c854177b86e44e5733e1e9b989ebcdcfdd98ea0).
The new scientific commit is pinned in the closing provenance amendment.
The closing commit's own SHA is provided in final chat delivery.

Read [the full verification record](../verification/GLR_GOAL002_CHECKS.md),
both physics notes, both audits and machine-readable outputs.
[Michael's updated handoff](../handoffs/MICHAEL_START_HERE.md) requests
no simulation/code action. The local project remains an uncommitted
desktop mirror with no Git remote; authoritative result commits are
created using the connected Git-tree API on the named branch, with a
base-tree scope comparison and expected-head lease. No other branch is
checked out or modified.

Literature inspected anew: [Collingbourne, section 3.4](https://arxiv.org/html/2007.08441)
on all-component future-horizon regularity;
[Gubser, sections 3.4–3.5 and discussion](https://arxiv.org/pdf/hep-th/0110193)
on static harmonic backreaction, charge/scheme freedom and cubic static
equations; [Sorkin, static ansatz and perturbation discussion](https://arxiv.org/html/hep-th/0402216)
on phase structure; [Figueras et al., section III.1](https://arxiv.org/html/2210.13501v2)
on geometric evolution and radiation discussion. These provide no
dynamical gamma or measured first-bulge scalar fraction. No novelty claim.

Concrete next strategy, **proposed only**: derive a constraint-preserving
inhomogeneous n2 scalar master, evaluate its regular horizon/infinity
fundamental system with source-tail bounds, and establish its inverse
constant. Use the charge-fixed constraint framework to complete one
specified GL-plus-scalar finite-slice family, then bound its fourth-order
scalar radiation trace. This is a mathematical continuation proposal;
it is not a newly launched Goal or Michael production request.
