# GLR-GOAL-001 final scientific report

Task `T-20261022-GLR-GOAL-001`, executed 2026-10-22.
Repository `inaeyk/GL-with-AI`, branch `research/sav-radiation`.

**Terminal classification: MEANINGFUL_PARTIAL.** The complete quadratic
physical zero-mode scalar equation, invariant mode normalization and
retarded overlap formula have been derived and passed two adversarial
mathematical reviews. Bounded calculations give strong independent
numerical evidence for nonzero outgoing overlap at all four wavelengths.
They do not supply a rigorous complete error enclosure, a controlled
finite-A interval, or a first-bulge emitted-energy percentage. The
finite-k nonzero-news question is substantially advanced, not fully
proved to the requested rigorous standard.

## Exact scalar source and normalized mode

Use \(f=1-r_0/r\), \(k=2\pi/L\), \(G_4=G_5/L\),
\(M_0=r_0/(2G_4)\), signature(-++++), and
\(g=\bar g+\epsilon h+\epsilon^2q+\cdots\), without a factorial in q.
For nonzero k the unique unitary representative has \(h^U_{zA}=0\), with
\[
h^U_{\mu\nu}=h_{\mu\nu}
+\frac{i}{k}(\bar\nabla_\mu h_{\nu z}+\bar\nabla_\nu h_{\mu z})
-\frac1{k^2}\bar\nabla_\mu\bar\nabla_\nu h_{zz}.
\]
Its four-dimensional TT conditions follow from the linear zA equations.
Write \(h^U=A(t)H(r)\cos kz\), \(A=\epsilon p_1e^{\Omega(t-t_0)}\).
Normalize \(H_{\theta\theta}/(2r^2)=1\) at background areal radius \(2r_0\).
This is an invariant first-order fractional sphere-radius amplitude in
the completed frame, not a chi or unspecified horizon-bulge amplitude.
[The linear derivation](../physics/gl_linear_mode.md) gives the master,
two-gauge conversion and every component. All seven independent SO(3)
linear Ricci equations and the TT conditions pass exact rational checks.

With pullback convention
\(q^U=q+\mathcal L_\xi h+\tfrac12\mathcal L_\xi^2\bar g+\mathcal L_\eta\bar g\),
periodicity makes the last term's zero-mode zz component vanish.
The physical scalar is \(\phi_0^{(2)}=\sqrt3\langle q^U_{zz}\rangle/4\).
Regular periodic gauges, fixed circle/clock and absence of an independent
physical first-order zero mode are required. At infinity the nonzero-k
tail and its local quadratic gauge completion decay, so this scalar
matches the Einstein-frame Bondi coefficient
\(\phi_0=\Phi(u)/\rho+O(\rho^{-2})\).

The complete closed scalar equation is
\[
\boxed{\bar\Box_4\phi_0^{(2)}
=-\frac{\sqrt3 k^2}{16}A(t)^2\mathcal H(r)},\qquad
\mathcal H=\frac{H_{tt}^2}{f^2}+f^2H_{rr}^2
+\frac{2H_{\theta\theta}^2}{r^4}-2H_{tr}^2.
\tag{1}
\]
For \(\psi=r\phi_0\), \(x=r_*=r+r_0\log(r/r_0-1)\), without an additive constant,
\[
[\partial_t^2-\partial_x^2+fr_0/r^3]\psi=A^2\mathcal S,\qquad
\mathcal S=\frac{\sqrt3 k^2rf}{16}\mathcal H. \tag{2}
\]

Derivation1 expands the exact z-Gauss Ricci equation, obtaining
\(\langle R_{zz}^{[2]}\rangle=-k^2 A^2\mathcal H/8\), including both
cosine/sine averages1/2. The full linear zero-mode equation
\(R'_{zz}[q]=-\bar\Box_4q_{zz}/2\) contains no other zero-mode field.
Derivation2 varies the cubic radion coupling in the spacelike z-ADM
action, giving the same sign/coefficient. The remaining spherical
metric equations have compatible sources by the on-shell expanded
Bianchi identity; they do not subtract a scalar from (1).
This is a closed scalar/integrability argument, not a claim to have
integrated or bounded the full metric backreaction.
Details: [quadratic source](../physics/quadratic_radion_source.md).

The independent foundation audit also checks the exact SO(3) action,
GHY cancellation and one-local-degree Hamiltonian count. At n=0 a
regular mass shift is separate; fixed circle and horizon regularity
exclude the homogeneous static scalar \(c_0+c_1\log f\).
Arbitrary homogeneous tension changes cannot silently cancel scalar news.
The prior photon coupling sign was corrected: the stated KK metric
ansatz requires \(e^{+2\sqrt3\phi}F^2\). The uncharged scalar is unaffected.

## Retarded functional and exact cancellations

For \(s=2\Omega>0\), \(L_s=-\partial_x^2+V+s^2\), \(V=fr_0/r^3\ge0\).
Use \(y_H\sim e^{sx}\) at the horizon, \(y_\infty\sim e^{-sx}\) at infinity,
and \(W=y_Hy_\infty'-y_H'y_\infty<0\). Then
\[
G_s=-y_H(x_<)y_\infty(x_>)/W,\qquad
\boxed{C_0=-\frac1{r_0W}\int_{-\infty}^{\infty}y_H(x)\mathcal S(r)\,dx}.
\tag{3}
\]
The scalar has no unstable resolvent pole, and the integral converges.
The opposite W sign fails the Green-function jump test.

The horizon contraction's apparently divergent powers
\(\delta^{2\Omega-2}\) and \(\delta^{2\Omega-1}\) cancel exactly.
The remaining leading coefficient of \(\mathcal H/y^2\) is an explicitly
positive rational polynomial, verified symbolically. The source remains
integrable and is not discarded; raw double component-square subtraction
near the horizon is unreliable.

At the static GL threshold, \(H_{tr}=0\),
\(y_H=r/r_0\), \(y_\infty=-(r/r_0)\log f\), \(W=-1/r_0\). Thus
\[
C_0(k_{\rm cr}r_0,0)
=\frac{\sqrt3 k_{\rm cr}^2}{16r_0}\int_{r_0}^{\infty}r^2\mathcal H\,dr>0
\]
for a nontrivial static mode. Its news is zero because \(\Omega=0\).
Continuation to an unstable interval requires continuous normalized
branch/tail control; this does not itself prove the four finite-k cases.
AuditII retains an exact manufactured nonzero source \(S=L_sF\) with zero
outgoing overlap. The general Lorentz contraction is indefinite:
neither a local source nor positive samples alone prove radiation.

For compatible finite-time Cauchy data use the full retarded convolution.
Arbitrarily switching a GL metric in time violates its first-order vacuum
constraints. Infinite-past exponential forcing is a valid Laplace
particular solution; a physical finite-time state adds a homogeneous
scalar transient. “No incoming radiation” alone does not fix independent
outgoing scalar Cauchy data. Details: [retarded and flux derivation](../physics/radion_retarded_transfer.md).

## Bounded quantitative findings

| L/r0 | kr0 | Omega*r0 estimate | C0 estimate | estimated absolute C0 error |
|---:|---:|---:|---:|---:|
| 8 | 0.7853981634 | 0.02645870 | 28.2964 | 0.0003 |
| 10 | 0.6283185307 | 0.06328119 | 12.30729 | 0.00003 |
| 12 | 0.5235987756 | 0.08044276 | 7.02554 | 0.00003 |
| 16 | 0.3926990817 | 0.09161923 | 3.32104 | 0.00003 |

Errors are conservative empirical estimates, not rigorous enclosures or
statistical confidence intervals. The Omega estimated absolute error1e-8
is also empirical. C0 scales quadratically with mode normalization and
depends on the stated tortoise/clock convention; its size is not an
emission fraction. Full numerical precision in JSON is diagnostic.

One separate threshold solve gives \(k_{\rm cr}r_0=0.876160402034\),
consistent with the literature ~0.876. Growth coarse–fine differences
are below1.7e-11 and matching residuals below7e-15. All rates satisfy
Collingbourne's rigorous literature lower bound. A precise published
numerical growth point was not independently digitized; that part of A4
is incomplete. Exact Ricci verification supplies an independent equation
check, not a substitute for numerical error enclosure.

Reference L10 was completed before the other three wavelengths.
Exactly four physical k times three declared ODE configurations were
used, plus the threshold solve. No spectral scan or time evolution.
The initial scalar asymptotic-series boundary prescription understated
its error at small s. The second/final method uses positive Volterra
tail inequalities, with scalar radii80,120,180 on the same configurations.
The finest L10 scalar-tail-only bracket is
[12.30729195998,12.30729198473]; it does not include errors in floating
GL/basis inputs, quadrature or horizon-series remainders.
An independent saved-metric/direct-linear-scalar calculation gives
12.30729190866, within6.4e-8 of the final primary midpoint.
All sampled physical contractions were positive.

**Nonzero outgoing finite-k overlap is strongly supported numerically;
a strict continuum sign/error proof remains open.**
[The numerical record](../physics/radion_transfer_numerics.md) retains
refinements, tail analysis, normalization and failure history.

## Flux, interval energy and spectrum

Independent 4D scalar-action and 5D EH transverse-TT current calculations
agree on
\[
\frac{dE_0}{du}=\frac{\dot\Phi^2}{G_4},\quad
\frac{E_0}{M_0}=\frac2{r_0}\int\dot\Phi^2du,\quad
\frac{dE_0}{d\omega}=\frac{|\widetilde N_\phi(\omega)|^2}{\pi G_4}
\quad(\omega>0). \tag{4}
\]
The 5D scalar polarization has transverse norm8; sphere area4pi and
circle lengthL recover1/G4. The leading symplectic flux needs no
high-frequency averaging. Transform news for scalar memory.

For an isolated settled exponential response, the conditional laws are
\[
\frac{d(E_0/M_0)}{d(u/r_0)}
=8\hat\Omega^2C_0^2A^4+O(A^6),\qquad
\frac{\Delta E_0}{M_0}
=2\hat\Omega C_0^2(A_2^4-A_1^4)+O(A_2^6). \tag{5}
\]
The first prefactors are4.48425,4.85249,2.55519,0.740646; the second are
42.3703,19.1703,7.94103,2.02099 in L=8,10,12,16 order.
Reducing epsilon by ten shifts a fixed weak-amplitude event by
87.0256,36.3866,28.6239,25.1321 in r0 units.

Windowed-news spectra are explicit:
\[
\widetilde N_{[u_1,u_2]}
=sr_0C_0\frac{A_2^2e^{i\omega u_2}-A_1^2e^{i\omega u_1}}{s+i\omega}.
\]
This obeys Parseval with interval energy but sharp windows create
artificial tails. A full growing exponential has no finite total
Fourier spectrum. Actual initial data and nonlinear rollover are needed
for a physical burst spectrum; none is guessed here.

**Unclosed amplitude gate:** no evaluated regular-frame bound on the full
q2 metric/n=2 response or next even-scalar order was obtained, and no
finite-data transient error bound was supplied. Therefore no explicit
finite-A interval is certified, despite a meaningful asymptotic law.
Substituting A~1 or an arbitrary A=0.1 is unjustified.
[FFG&A2023 III.1/Table2](https://arxiv.org/html/2210.13501v2) first-bulge
geometric (tp,tn)/r0 windows are (163.1,174.7),(93,107),(78,96.4),(91,112.7).
They calibrate neither A nor waveform arrival/energy. No first-bulge
percentage, late-emission bound or cascade spectrum follows.

## Milestones and adversarial reviews

| Milestone | Reached | Unresolved |
|---|---|---|
| A | Action/EOM/GHY, canonical count, two-gauge mode, exact Ricci checks, threshold/four modes | Precise independent numerical growth point and rigorous mode enclosures |
| B | Complete closed scalar source, quadratic gauge completion, constraints compatibility, horizon cancellations | Integrated metric response/norm for nonlinear control |
| C | Exact normalized integral, causal prescription, independent action equality, four estimates | Full finite-k sign/error enclosure and finite-data transient bounds |
| D | Independent4D/5D flux; conditional energy/windowed spectrum/onset laws; geometric comparison | Controlled finite-A interval and physical full burst prediction |
| E | Two adversarial AI reviews, mutations, failures, reproducible scripts/data, Michael interface | External human review not supplied or claimed |

[AuditI](../verification/AUDIT_FOUNDATION.md) accepts action/gauge/uncharged
flux/source under stated hypotheses and found the corrected photon/count
faults. [AuditII](../verification/AUDIT_SOURCE_RETARDED.md) accepts the
independent cubic source, retarded sign, static threshold and positive
Volterra inequalities. Its independent reference calculation supports
nonzero numerical overlap while withholding finite-k interval and finite-A
certification. Gauge/sign/Fourier/normalization/potential mutations are
rejected. Separate scientific AI agents performed these reviews; they
are not external human referees.

## Commit provenance, reproduction and stop

- Input: `0b0b2e1ee1df64a80e4474d6909173d4682564f3`.
- Scientific derivations, source, checks, audits, profiles and results:
  [`7110bc2cb3c0496dd62dc97d37b34d11ec780a17`](https://github.com/inaeyk/GL-with-AI/commit/7110bc2cb3c0496dd62dc97d37b34d11ec780a17).
- This report, handoff and closing pointers form the subsequent closing
  commit; its exact SHA is supplied in final chat delivery. A commit
  cannot embed its own SHA; the scientific result is independently pinned.

The scientific tree comparison verified33 changed/added blobs, all under
research/sav/**, no deletions or outside changes. Only the designated
research branch advanced, with an expected-head lease. The desktop began
with an empty local project; no checkout of main/Michael branches was
made. No root/code/production change, merge, manuscript or Overleaf work.

Commands, versions and exact output references:
[GLR_GOAL001_CHECKS.md](../verification/GLR_GOAL001_CHECKS.md).
Five exact-check commands passed. Complete numerical configurations,
outputs and CSV profiles accompany executable sources. Raw horizon NaNs
are intentional and documented. Michael's concrete science-only interface:
[MICHAEL_START_HERE.md](../handoffs/MICHAEL_START_HERE.md).

The four-k/three-refinement allocation and two scalar-boundary approaches
have been used. The next ambition is a bounded validated overlap proof
including horizon-series remainders, followed by regular-frame second-order
metric/n=2 and next even-scalar/transient bounds. It requires a newly scoped
task, not an automatic wider scan or Michael/GRChombo change.
The stop is meaningful partial completion, not a gauge-invariance failure,
a proved cancellation, or a fully verified radiation fraction.
