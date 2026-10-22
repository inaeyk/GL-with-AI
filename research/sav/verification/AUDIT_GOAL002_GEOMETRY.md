# GLR-GOAL-002 adversarial audit 1 — geometry, constraints and horizon

2026-10-22. Reviewer: `audit_geometry`, a separate scientific AI agent,
not an external human referee. All checks are non-production symbolic
calculations; no GL root or physical radial boundary-value problem was solved.

**Verdict:** the independently reconstructed covariant quadratic
source, zero-mode Einstein-frame source, EF mass/lapse equations and forced
second-harmonic constraint identities are mathematically consistent.
Full first-order unitary horizon regularity is independently checked at the
component level. These facts do **not** establish Gate B: a fully evaluated,
constraint-compatible n=2 boundary response, its physical norm, boundary
errors and nonlinear remainder are still required. No finite-A energy
interval, first-bulge fraction or exact nonlinear data family is certified.
The primary derivation and actual sector/source scripts were subsequently
inspected and the primary source script independently rerun; see section 7.

## 1. Independent covariant connection route

Use the no-factorial expansion `g=gbar+epsilon h+epsilon²q` and
`R_ab=epsilon R'[h]_ab+epsilon²(R'[q]_ab+Q_ab)+...` on the Ricci-flat
Schwarzschild-string background. The difference of two connections is a
tensor. With all covariant derivatives and index raising taken using gbar,

\[
 C^a{}_{bc}=\frac12\bar g^{ad}
  (\bar\nabla_bh_{dc}+\bar\nabla_ch_{db}-\bar\nabla_dh_{bc}),
 \qquad D^a{}_{bc}=-h^a{}_d C^d{}_{bc},
\]

\[
 Q_{ab}=\bar\nabla_cD^c{}_{ab}-\bar\nabla_bD^c{}_{ac}
       +C^c{}_{cd}C^d{}_{ab}-C^c{}_{bd}C^d{}_{ac}.                 \tag{1}
\]

Equation (1) follows directly by subtracting the coordinate Ricci formulas;
it contains every quadratic connection term. In particular it does not
replace Einstein's quadratic source by a square of selected components.
The associated quadratic Einstein tensor is

\[
 G^{[2]}_{ab}=Q_{ab}-\tfrac12\bar g_{ab}\bar g^{cd}Q_{cd}
\]

**only on an on-shell first-order Ricci-flat perturbation**. Otherwise terms
involving `h R'[h]` remain and the simple conservation statement below is
false. Expanded Bianchi then gives `bar nabla^a G^[2]_ab=0`.
Our executable directly constructs (1) on a physical standing transverse
traceless wave on flat 5D: `h_yy=−h_ww=cos(t)cos(z)`, all other components
zero. Every linear Ricci component and all five quadratic Einstein-source
divergences vanish exactly. Replacing `G^[2]` by `Q` fails that test.
This is an independent full-component algebra control; it is **not** a
machine check of every curved GL source component.

In first-order unitary gauge, `h_zA=0`, the zz contractions of (1) reduce to

\[
 Q_{zz}=\tfrac12\operatorname{tr}(h h_{,zz})
        +\tfrac14\operatorname{tr}(h_{,z}h_{,z}).                \tag{2}
\]

For `h=A H cos(kz)`, therefore,

\[
 Q_{zz,0}=-\frac{k^2 A^2}{8}\operatorname{tr}(H^2),\qquad
 Q_{zz,\cos2kz}=-\frac{3k^2 A^2}{8}\operatorname{tr}(H^2).       \tag{3}
\]

The second entry denotes a **real cosine coefficient**, twice each complex
n=±2 coefficient. The standing-wave test independently recovers both
factors in (3). With `R'[q_0]_zz=−Box chi/2`, `chi=q_zz,0`, one obtains
`Box chi=2 Qzz,0`, hence

\[
 \bar\Box\phi_0^{(2)}=-\frac{\sqrt3 k^2}{16}A^2H_{\mu\nu}H^{\mu\nu},
 \qquad \phi_0^{(2)}=\frac{\sqrt3}{4}\chi .                    \tag{4}
\]

The source sign and missing real-cosine half are deliberate rejected
mutations. Nothing in (1)–(4) proves that the physical outgoing overlap is
nonzero or interval-encloses the numerical `C0`.

## 2. Zero-mode Einstein-frame reconstruction

Set `p_mu nu=q_mu nu,0+chi gbar_mu nu/2`. The infinitesimal four-dimensional
Weyl identity is

\[
 R'_4[-\chi\bar g/2]_{\mu\nu}
 =\tfrac12\nabla_\mu\nabla_\nu\chi
  +\tfrac14\bar g_{\mu\nu}\Box\chi.
\]

The 5D linear Ricci contribution from `chi` includes
`−nabla_mu nabla_nu chi/2`. It cancels the Hessian above, leaving

\[
 R'_4[p]_{\mu\nu}=-Q_{\mu\nu,0}
                  -\tfrac12\bar g_{\mu\nu}Q_{zz,0},
\]

\[
 G'_4[p]_{\mu\nu}=\mathcal T_{\mu\nu},\qquad
 \mathcal T_{\mu\nu}=-Q_{\mu\nu,0}
  +\tfrac12\bar g_{\mu\nu}(Q^\alpha{}_{\alpha,0}+Q_{zz,0}).    \tag{5}
\]

The script checks the trace algebra independently. Equation (5) is exactly
`−G^[2]_mu nu,0`, and compact averaging removes the total z derivative
in expanded 5D Bianchi. Thus `nabla^mu T_mu nu=0` for an on-shell h.
**A sign error in the scalar-source equation would also spoil (5).**

Use areal EF gauge

\[
 ds_E^2=-e^{2\beta}(1-2m/r)dv^2+2e^\beta dvdr+r^2d\Omega_2^2,
 \quad m=r_0/2+\epsilon^2\mu,\quad \beta=\epsilon^2 b.
\]

A direct independent coordinate connection on the 2D `(v,r)` base, plus
the exact S² warp formulas, gives

\[
 \delta G_{rr}=2b_r/r,\quad
 \delta G^v{}_v=-2\mu_r/r^2,\quad
 \delta G^r{}_v=2\mu_v/r^2,                                  \tag{6}
\]

\[
 \delta G_{\theta\theta}=r(r-r_0)b_{rr}+r^2b_{rv}
                 +(r+r_0/2)b_r-r\mu_{rr}.                    \tag{7}
\]

The executable checks (6) and (7) for arbitrary smooth `mu(v,r),b(v,r)`.
This is not a static approximation. Wrong mass sign and missing lapse
factor two are rejected. For sufficiently integrable source and fixed
asymptotic lapse, the radial constraints give

\[
 b(v,r)=-\frac12\int_r^\infty R\,\mathcal T_{rr}(v,R)dR,
\]

\[
 \mu(v,r)=\delta m_\infty(v)
       +\frac12\int_r^\infty R^2\mathcal T^v{}_v(v,R)dR.        \tag{8}
\]

`mu_v=r² T^r_v/2`, and (7), must still be checked with the supplied source
and boundary charge. They cannot be erased by calling (8) a solution.
The meaning of the limit at fixed advanced time, the spatial ADM limit,
and any incoming data must be stated separately. A finite-time retarded
problem supplies those limits; arbitrary data cannot be made compatible
by choosing a v-dependent mass constant freely.

The regular stationary homogeneous metric solution contains a mass shift.
The homogeneous static scalar is `c_infinity+c log f`; fixed circle length
excludes the first constant and future-horizon regularity excludes the log.
Thus an arbitrary independent regular scalar/tension hair cannot be added
to cancel radion news. Static KK charge formulas require stationary
asymptotics; instantaneous Bondi scalar news is a different observable.

## 3. Forced n=2 constraints are not homogeneous TT conditions

For a nonzero second harmonic `K=2k`, choose the second-order unitary
representative `q_zA,K=0`, after the known first-order completion. Define

\[
 \tau=\bar g^{\mu\nu}q_{\mu\nu,K},\qquad
 j_\mu=\bar\nabla^\nu q_{\mu\nu,K}-\bar\nabla_\mu\tau.
\]

The zz and mixed Einstein equations imply

\[
 \tau=-\frac{2Q_{zz,K}}{K^2},\qquad
 j_\mu=-\frac{2Q_{\mu z,K}}{iK}.                              \tag{9}
\]

The remaining tensor equation, commuting derivatives on the Ricci-flat
four-dimensional background, is

\[
 (\bar\Box_4+2\bar R_{\mu\alpha\nu\beta}{\cdot}-K^2)
 q_{\mu\nu,K}
 =2Q_{\mu\nu,K}+\bar\nabla_\mu\bar\nabla_\nu\tau
   +\bar\nabla_\mu j_\nu+\bar\nabla_\nu j_\mu.                \tag{10}
\]

The executable checks every component of the uncommuted flat-space
linear Ricci identity, with arbitrary off-shell symmetric
`q_mu nu(t,x) exp(iKz)`, including zz and all mixed constraints. The curved
Riemann commutator in (10) is obtained analytically. Imposing `tau=j=0`
on the forced problem is a deliberately rejected mutation: these
conditions hold only for an independent homogeneous massive mode.

Equations (9)–(10) are a closed differential-algebraic formulation, **not**
an evaluated Green representation, not a completed n=2 solution and not
Gate B. One must solve with ingoing/regular future-horizon data and the
appropriate asymptotic decaying branch for exponential forcing. Such
forcing has `omega=i 2Omega`, not real on-shell massive propagation.
Only independent real-frequency data with `|omega|>|K|` admit outgoing
massive waves, with `q_radial²=omega²−K²`, `v_group=q_radial/omega`.
No massive Bondi flux is thereby produced at generic 4D null infinity.

## 4. Full first-order horizon component check

Set `r0=1`, `delta=r−1`, `K=k²` **only in this subsection**. Write the
ingoing Frobenius branch as

\[
 y=\delta^\Omega(1+c_1\delta+c_2\delta^2+\cdots).
\]

Rather than use the primary Riccati recurrence, the audit substitutes this
series directly into the second-order y master. Denominators are cleared
before extracting coefficients. The two coefficients are

\[
 c_1=\frac{K^2+2K\Omega^2+5K\Omega-2K+2\Omega^2-\Omega}
             {(K+1)(2\Omega+1)},
\]

\[
 c_2=\frac{K^3+4K^2\Omega^2+12K^2\Omega+K^2
       +4K\Omega^4+22K\Omega^3+18K\Omega^2+2K\Omega
       +4\Omega^4-2\Omega^3+2\Omega^2+2\Omega}
       {4(K+1)(\Omega+1)(2\Omega+1)}.
\]

With `v=t+r_*`, the EF components before the common time factor are
`Hvv=T`, `Hvr=B−T/f`, `Hrr=2T/f²−2B/f−2S/[r(r−1)]`, and
`Htheta theta=S`. Factoring the overall
`e^(Omega v)e^(−Omega r)` and leading y coefficient, the exact horizon
limits are

\[
 (H_{vv},H_{vr},H_{rr},H_{\theta\theta}/r^2)\to
 \left(
 \frac{\Omega(1-2\Omega)}{2K(K+1)},
 \frac{K+2\Omega}{2K(K+1)},
 -\frac{K^2+8K\Omega+8\Omega^2+2\Omega}
       {2K(K+1)(\Omega+1)(2\Omega+1)},
 -\frac{K+2\Omega}{2K(K+1)}\right).                            \tag{11}
\]

All four are finite for the growing physical mode. Replacing `c1` by
`c1+1` creates an uncancelled `Hrr` simple pole; the test detects it.
These are analytic endpoint limits. They supply no finite-cutoff series
remainder bound and no global metric-norm enclosure.

A regular null frame is
`l=partial_v+(f/2)partial_r`, `n=−partial_r`, with `g(l,n)=−1`;
`e0=(l+n)/sqrt(2)`, `e1=(l−n)/sqrt(2)` is a regular orthonormal pair.
Together with sphere unit vectors and `partial_z`, this gives a legitimate
component norm. Static Schwarzschild coordinate components cannot be
used as that norm.

## 5. Independent regular periodic pure-gauge control

On the EF orbit metric, take
`xi^v=d(r)cos(kz)`, `xi^r=b(r)cos(kz)`, `xi^z=c(r)sin(kz)`.
Smooth compactly supported profiles give an admissible regular periodic
gauge, and `h=L_xi gbar`, `q=L_xi² gbar/2`. The full completion with
compensator `−xi` vanishes identically. The raw zero-mode zz component is

\[
 \langle q_{zz}\rangle
  =\frac{k^2}{2}\{c^2+2bd-fd^2\},                             \tag{12}
\]

which is generically nonzero. Omitting the half-Lie-squared term leaves
a spurious value of the opposite sign. This independently tests the
completion in horizon-regular coordinates, rather than copying the
previous static-coordinate pure-gauge example. Sphere/orbit terms do not
alter this zz identity; they must be included when testing the full
metric's curvature. A raw `log gzz` average remains unsuitable as the
physical scalar at finite radius.

## 6. Execution record, mutations and limits

Command (exit 0):

```sh
python3 research/sav/verification/goal002_audit_geometry.py > research/sav/verification/goal002_audit_geometry_results.txt
```

Python 3.14.8; SymPy 1.14.0. Exact full output is retained in
`goal002_audit_geometry_results.txt`, and detailed expressions/scopes in
`goal002_audit_geometry_results.json`. There are 17 exact PASS records,
including eight deliberate-error cases in six groups. The source code imports no new
primary GOAL-002 implementation and performs no ODE discretization.

Two early whole-script executions were manually interrupted (exit 130)
during avoidably expensive generic `cancel()` expansion of the horizon
rational expressions, after the source/constraint checks had passed.
The final implementation requests only linear curvature in the generic
n=2 identity and takes the direct rational Frobenius limits without the
redundant expansion. The independent small derivation had already given
(11). These were symbolic performance failures, not failed physical gates
or additional mathematical repair strategies. A scratch attempt to use
`coeff(delta,-1)` before clearing denominators omitted terms of the form
`1/(K delta+delta)`; it was discarded and is explicitly prevented in the
retained code. No divergent physical mode was inferred from that algebra
extraction error.

No transfer coefficient was recomputed, no exact full nonlinear Cauchy
family was certified, and no full n=2 boundary solution was obtained by
this reviewer. Audit acceptance of equations must not be promoted to a
finite-A theorem. The existing GOAL-001 independent C0 evidence and its
unclosed continuum enclosure retain their prior classification.

## 7. Review of actual primary artifacts and independent rerun

After completing the independent derivation and 17 checks above, this
reviewer read `physics/goal002_second_order.md`, the complete
`goal002_sector_checks.py`, its machine-readable result and transcript,
and the source/norm reconstruction in `goal002_scalar_response.py`.

Equations (5)–(6) of the primary geometry note correctly expand
`Ric5_ab=Ric3_ab−Hess W/W+dW dW/(2W²)` and
`Ric_theta theta=1−Box W/2`. The inverse-metric second-order sphere term
and the `3w²/(2W0⁴)` orbit term are present. Equations (7)–(12) agree with
(5)–(10) above, including the forced real-cosine trace `3 A²H²/16`.
The radial quadratures use the correct signs; full source conservation
and asymptotic charge conditions remain essential. No computed q2
supremum or solved n=2 inverse is present, and the note explicitly says
so. Its Gate B failure is scientifically appropriate.

Independently executed command, exit 0:

```sh
python3 research/sav/verification/goal002_sector_checks.py
```

Exact observed output:

```text
LINEAR_CONNECTION_BUILT
ALL_SIX_ON_SHELL_LINEAR_ORBIT_EQUATIONS: PASS
QUADRATIC_SEVEN_COMPONENT_SOURCE_BUILT
QZZ_ZERO_AND_SECOND_HARMONIC_SOURCE: PASS
FULL_QUADRATIC_BIANCHI_0: PASS
FULL_QUADRATIC_BIANCHI_1: PASS
FULL_QUADRATIC_BIANCHI_2: PASS
DOUBLED_ZZ_WITH_UNCHANGED_METRIC_SOURCE_MUTATION: REJECTED
```

This rerun is replication of the primary implementation; the independent
pathway is the separate 17-check script above. The exact specialization
`kappa=2/3,Omega=1/7` has `k²=−4/9`: it is an analytic-continuation
**local** algebra control, not the L10 physical mode. In fact `D=1+k²r³`
has a positive-r pole for this specialization. Rational identity
cancellation remains legitimate wherever the formulas are defined;
no global physical solution, endpoint bound or continuum norm follows.
The primary note labels these limitations correctly.

One notation clarification was sent to the primary author: on a tau slice,
the tensor profile is `exp(Omega r) widehat H`, equivalently
`delta^(−Omega) H^(raw EF)`, where the latter means the EF bracket
combinations **without** the `exp(−Omega r_*)` factor. Confusing the raw
transformed components with widehat H from equation (13) would double-count
that factor. The actual scalar-response script implements the correct
raw-component expression. This is a notation clarification, not a
GOAL-001 source defect.

**Acceptance:** the new complete equation/source generator, zero-mode
quadratures and forced trace/divergence constraints survive this audit at
the stated mathematical scope. **Withheld:** complete second-harmonic
boundary reconstruction, numerical global q2 bounds, exact finite-time
nonlinear constraint solution, and any finite-A/first-bulge energy claim.
