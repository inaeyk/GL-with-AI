# GOAL-004 adversarial Audit I: source geometry and global constraints

2026-10-23. Reviewer: distinct scientific AI investigator `audit_geometry`,
delegated by the primary GOAL-004 investigator. Sole human branch operator
and publisher: Savdeep Sethi. Input:
`research/sav-radiation@2f4648c379c474ed124e3ef10b62efcd4a783c66`.
Only this audit, its independently authored script and its outputs are
written by the reviewer. No commit/push, physical ODE configuration,
spacetime evolution or production action is performed by this reviewer.

**Current verdict:** the conditional source-aware maximum-principle
estimate and the exterior derivative/KID identities below are exact.
They expose specific unclosed source and global-data constants. They do
not certify a physical L10 profile, a complete numerical M2, a global
fixed-charge GL family, a finite-A interval or a radiation theorem.

## 1. Independence, inspected inputs and chronological boundary

Before reading any new GOAL-004 primary implementation, this reviewer
read the controlling GOAL-004/review/runbook files, scoped AGENTS,
HANDOFF/STATUS/NOTATION, the GOAL-003 forced-master, inverse and local
data derivations, the GOAL-003 report/audit and attributed
[review qualifications](GOAL003_REVIEW_QUALIFICATIONS.md), and the old
source/elimination implementations at the stated input. The following
three estimators were then independently derived and implemented, with
no import of a primary verification module. The independently authored
program ran successfully before any new primary GOAL-004 scientific
code was inspected. This chronological distinction concerns the actual
new estimator/check, not an assertion that the old master, source tensor
or positive supersolution were independently rediscovered.

All investigators share the same language-model family, old research
record and workspace. This is distinct AI scientific scrutiny, not an
external human referee or an entirely independent premise set. In
particular the old supersolution and old source-aware master are inputs;
reusing them does not erase the GOAL-003 shared-authorship qualification.

## 2. A global weighted scalar bound sharper than a bare L2 loss

Use r0=1, x=r+log(r-1), f=1-1/r, a=K², D=1+ar³, and the old
operator H_a=-d_x²+V_a. For a>=3/2 the old positive supersolution is

\[
w=e^{-r/2}\frac rD,\qquad
\frac{H_aw}{w}=\frac{fP_a(r)}{4r^2D}\ge0,
\]

where

\[
P_a=(4a^2-a)r^5-7ar^4+10ar^3-(8a+1)r^2+5r-2.
\]

Set S=-d_x log w and h_s=s²+H_aw/w. The independently derived identity
is

\[
\frac{(H_a+s^2)(wv)}w=-v_{xx}+2Sv_x+h_sv.\tag{I.1}
\]

Thus h_s>=s²>0. If the admissible separated solution satisfies
(H_a+s²)u=mathfrak F, u/w->0 at both ends, and

\[
C_w=\sup_{r>1}\frac{|\mathfrak F(r)|}{w(r)h_s(r)}<\infty,
\]

the maximum principle applied to +/-u/w gives the **conditional exact
bound**

\[
\boxed{|u(r)|\le C_ww(r).}\tag{I.2}
\]

At a positive maximum of u/w the drift vanishes and -v_xx>=0, giving
h_sv<=mathfrak F/w; the negative maximum is identical. Exhaustion by
finite intervals or decay of v justifies the global maximum. No claim
V_a>=0 is used; that false alternative is actually rejected by
V_{3/2}/(r-1)->-11/10.

The compatible EF source class gives mathfrak F=e^{sx}O(r-1), so the
source ratio vanishes at the horizon and u/w~e^{sx}->0. The physical
GL-squared tail has mathfrak F=O(e^{-alpha r}r^{-beta2-2}) with
alpha=sqrt(a+s²)>1/2, so the ratio also vanishes at infinity. This
argument uses the **exact endpoint class**, not a numerical finite
cutoff. The old particular tail and an allowed decaying homogeneous
coefficient both satisfy u/w->0; the horizon-regular/infinity-decaying
inverse fixes that coefficient. Independent Cauchy homogeneous data
are a separate problem and are not bounded by (I.2).

The new weighted source supremum can improve the generic s^-2 L2
estimate when a physical source enclosure is available. It does not
supply that enclosure. A physical numerical value of C_w still needs
uniform L10 eigenfunction/derivative/eigenvalue and endpoint remainder
enclosures. Nor does scalar (I.2) automatically bound the reconstructed
metric: the complete affine source lift, EF Bianchi pole cancellation,
and its derivatives remain required. This is a source-aware conditional
estimate, not a relabeled numerical M2 or a retarded propagator.

## 3. The precise missing differentiated global-data estimate

For the old improved ideal exterior rule b=1, R=r, d=v=0, write
Q=r²q and J=r²eta. The equations are

\[
Q_r=aJ_z+2a_zJ,\qquad
J_r=\tfrac12aQ_z-\tfrac12a_zQ.
\]

Here a is the radial spatial lapse, not the K² parameter of section 2.
For periodic z define E_m=integral[(partial_z^m Q)²/2+
(partial_z^m J)²]dz. Direct calculation, before seeing new primary
data analysis, gives

\[
E_0'=0,\qquad
E_1'=2\int a_zQ_zJ_z\,dz
+\int a_{zz}(2Q_zJ-QJ_z)\,dz.\tag{I.3}
\]

Cauchy-Schwarz gives

\[
|E_1'|\le\sqrt2\|a_z\|_\infty E_1
+3\sqrt2\|a_{zz}\|_\infty\sqrt{E_0E_1},
\]

and, in the regularized sense at E1=0,

\[
\partial_r\sqrt{E_1}
\le\frac{\|a_z\|_\infty}{\sqrt2}\sqrt{E_1}
+\frac3{\sqrt2}\|a_{zz}\|_\infty\sqrt{E_0}.\tag{I.4}
\]

Consequently one sufficient new ingredient is a proof of
integrability of ||a_z||infty and ||a_zz||infty on the exterior,
together with the corresponding differentiated/free-source terms in
the nonideal b,R,d,v problem. Then (I.4) and 1D Sobolev embedding bound
Q,J pointwise, hence q,eta=O(r^-2). Parabolic angular damping may help
prove this, but simply citing its sign does not establish the coupled
integrability or higher-order ADM expansion. Its zero angular mode,
positive lapse, source errors and commutators still need control.

The exact counterexample Q_N=sqrt(2/N)sum_{j=1}^N cos(jz), J_N=0
has E0=pi but Q_N(0)=sqrt(2N) and E1=pi(N+1)(2N+1)/6.
The script checks N=1,2,4 exactly, while Fourier orthogonality proves
the whole family. Thus an E0 bound alone cannot justify the pointwise
quadratic Z bound needed by the Hamiltonian comparison. These are
periodic function controls, not physical GL data. They refute the
inference E0=>a pointwise exterior constant, not global-data existence.

## 4. Independently normalized KID boundary diagnostic

On the static K=0 spatial Schwarzschild-string slice, choose N=sqrt(f)
and the radial variation

\[
h_{rr}=u/f,\quad h_{zz}=w,\quad
h_{\theta\theta}=r^2v,\quad h_{\varphi\varphi}=r^2v\sin^2\theta.
\]

The linear scalar-curvature identity is

\[
NDR[h]=\nabla_i\{N(\nabla_jh^{ij}-\nabla^i\operatorname{tr}h)
-h^{ij}\nabla_jN+(\operatorname{tr}h)\nabla^iN\},
\]

since the static vacuum lapse solves the adjoint KID equation.
Independent warped-curvature expansion yields exactly

\[
r^2DR[h]=\mathcal B'(r),\qquad
\mathcal B=r^2f[-w'-2v'+2(u-v)/r]
+\tfrac12r^2f'(w+2v).\tag{I.5}
\]

Integration over spheres/circle gives 4pi L[B_infty-B_in]. A mass
variation u=delta r0/(rf), v=w=0 gives B=2 delta r0, hence
4pi L B=16pi G5 delta M with M=Lr0/(2G5), checking all factors.
A circle tail w=c/r contributes B_infty=c even though the limiting
circle length is unchanged. A charge claim must control that tail.
For smooth bifurcation-surface data, the horizon lapse-gradient term
is the usual area pairing and is not zero merely because N=0.

This diagnostic is on the bifurcate static slice, **not** a proof for
the future-horizon-crossing GL data. On that slice the full lapse/shift
KID pairing must replace (I.5). Its purpose is to independently test
normalization and expose the unjustified deletion of an inner boundary
term. The script deliberately drops the lapse-gradient term and observes
failure of the curvature/boundary identity.

There is a useful exact refinement to the old statement that even the
mass-tuning derivative is unknown. In the improved radial prescription,
at zero GL amplitude keep b=1, R=r, eta=v=0, q=q0 and d=d0. Vary the
inner lapse by a(Ri)=a0(Ri)exp(lambda). The momentum solution remains
exactly background. With u=a^-2, the Hamiltonian equation is linear,

\[
u_r=(1-u)/r+rZ_0/2,\qquad u_0=r/(r+1).
\]

Hence an **exact global spherical constraint family** is

\[
u_\lambda=u_0+\frac{C(\lambda)}r,\qquad
C(\lambda)=\frac{R_i^2}{R_i+1}(e^{-2\lambda}-1).
\tag{I.7}
\]

It is positive on r>=Ri for every finite lambda: ru0=r²/(r+1) is
strictly increasing, and at Ri its numerator is
Ri² exp(-2lambda)/(Ri+1)>0. With fixed gzz=1, the ADM mass read from
a²=1+(1-C)/r+O(r^-2) is

\[
M(0,\lambda)=M_0(1-C(\lambda)),\qquad
\partial_\lambda M(0,0)=\frac{2M_0R_i^2}{R_i+1}>0.\tag{I.8}
\]

In particular Ri=3/4 gives partial_lambda M/M0=9/14. This is a
nonzero, independently checked background derivative; it should not be
listed as wholly unknown. This family varies the interior data, so it
does not violate the KID boundary pairing. It is a refinement within
the existing improved prescription, not a new global GL construction.
What remains unproved is a global simultaneous z-dependent family,
differentiability of its mass map M(epsilon,lambda), and a uniform lower
bound on partial_lambda M in the required neighborhood. Conditional on
those results, (I.8) gives the base transversality needed for mass
tuning. Neither existence on every finite collar nor the exact spherical
subfamily supplies those missing uniform estimates.

## 5. Nonzero source is not a positive radiation theorem

For the exact flat separated operator L_s=-d_x²+s², take
F(x)=(1-x²)^3 on |x|<=1 and zero outside. F and F' vanish at both
endpoints; S=L_sF is nonzero, but integration by parts gives

\[
\int_{-1}^1e^{sx}S(x)dx=0.\tag{I.6}
\]

The script verifies the zero overlap exactly at s=2/3 and rejects the
wrong sign +F''+s²F. This is an explicit compact-source control, not
the physical GL source. It prohibits concluding positive outgoing
amplitude from a nonzero local source or source norm alone. An actual
complete constrained GL source and its retarded overlap are required.

## 6. Executed operations and negative controls

From research/sav, the actual independent command was

```sh
python3 verification/goal004_audit_geometry.py > verification/goal004_audit_geometry_results.txt
```

The initial pre-implementation run exited 0 with 26 checks. After the
additional independent background-transversality derivation, the same
command was rerun with 33 checks. Subsequent review of the new primary
linear-lapse proposal added four checks. The final execution exited 0:
Python 3.14.8, SymPy 1.14.0, 37 exact checks, including seven deliberately incorrect cases:
momentum connection sign, dropped
H1 commutators, omitted KID lapse-gradient term, wrong ground-state
drift sign, false zero background mass derivative, and wrong source
second-derivative sign, plus an omitted parabolic-kernel factor. The negative
horizon-potential test independently rejects the proposed positivity
shortcut. Actual per-check output, symbolic wrong-case residuals,
deterministic inputs and script SHA256 are in
[results JSON](goal004_audit_geometry_results.json) and
[console output](goal004_audit_geometry_results.txt). The independent
[script](goal004_audit_geometry.py) in its default exact-geometry mode
imports no primary verification module. No physical 1D configuration
allowance was used by this audit.

## 7. Subsequent primary-artifact review

After the independent derivations this reviewer inspected the full
[source note](../physics/goal004_source_geometry.md), source script,
saved results and console output, and the primary
[data/news note](../physics/goal004_data_news.md). The source calculation
uses actual empirical physical L10 k,Omega values and the saved radial
profile. The local on-shell radial jets differ materially from the old
imaginary-k/rational controls. They remain local on-shell algebra,
because assigning jet derivatives by the ODE does not prove that the
interpolated sampled values comprise one exact global solution.

The EF conversion, Lorentz contraction, real-cosine weights, affine
trace/divergence factors, Green signs, dx=r dlog(delta), zero-sector
quadrature signs and regular tetrad agree with the inspected formulas.
The three Green configurations use the same source interpolation, so
source-error convergence cannot be inferred from their agreement. The
different W values are affected by outer-basis normalization; only
within-run relative W drift is comparable. The physical code checks all
seven first-order Ricci equations and full quadratic Bianchi identities
locally; it does **not** independently evaluate the seven forced
second-order equations on the sampled reconstructed response. That
qualification was supplied to the primary investigator and explicitly
inserted in the final source note. The old exact all-equation equivalence
supports the map, but it does not bound its numerical source/derivative,
horizon-division or endpoint residuals.

The independent optional point command was also actually run, exit 0:

```sh
python3 verification/goal004_audit_geometry.py --inspect-primary > verification/goal004_audit_geometry_physical_point.txt
```

It imports the primary module only to obtain its empirical regular H
at r=2 and full-source tau. The reviewer independently implements the
5D block-metric identity

\[
R_{zz}=-\tfrac12\operatorname{tr}(g^{-1}g_{,zz})
+\tfrac14\operatorname{tr}(g^{-1}g_{,z}g^{-1}g_{,z})
\]

and its quadratic coefficient and eight-phase Fourier projection. The
five additional point checks include two rejected mutations (factor two
and Euclidean rather than Lorentzian contraction). Actual values are

| Physical empirical point, r=2 | Value |
|---|---:|
| Hhat_AB Hhat^AB from independent 5D matrix contraction | 11.917781361548352 |
| Qzz,0 from independent quadratic/Fourier calculation | -0.5881189368857916 |
| Qzz,+2 from independent calculation | -0.8821784053286874 |
| Qzz,+2 from primary complete Ricci source | -0.8821784053286874 |

The machine-readable [physical point record](goal004_audit_geometry_physical_point.json)
and [actual output](goal004_audit_geometry_physical_point.txt) pin the
primary script hash and local diagnostics. This is an independent
tensor/Fourier calculation with **shared empirical radial input**. It
does not independently enclose the eigenfunction, all seven source
components, a full forced metric residual or any continuum source norm.

The primary endpoint Coulomb residual coefficients were separately
checked by substituting w_r/w=-q-beta/r in
-f²w_rr/w-ff'w_r/w+V+Omega². They agree. The stated exterior Volterra
norm follows from -partial_x log w>=q f(R), |Rw|<=CR/r² and
dx/dr<=1/f(R); the derivative envelope has the stated factors.
The eigenvalue-match identity F'(Omega)>0 follows by integrating
(u² partial_Omega p)_x=2Omega u² separately on node-free half-lines.
It requires positive solutions and vanishing endpoint terms, as stated.
Neither identity validates the actual saved profile connection,
normalization or eigenvalue bracket. The explicit missing C5 input and
C3 source inequalities are therefore appropriate stopping boundaries.

The primary data note's equations (2)--(4) agree with the independent
spherical mass and exterior-energy calculations above. For its linear
lapse equation, independent differentiation of the original Hamiltonian
gives

\[
\frac{1-3a_0^2}{2r}-\frac{3r}4a_0^2Z_0
=-\frac{2r+5}{2r(r+1)},\qquad
-\frac{a_0^3(-q_0+2d_0)}{4r}=\frac{2r+1}{4r^4}.
\]

The exact G_n has the correct logarithmic derivative and initial value.
The stated bound for r>=2Rm>=2 is valid: on rho<=r/2 the source
integral is <=3B/(8Rm²) times exp(-3k_n²r²/16); on rho>=r/2 the
source is <=6B/r³ and the exponential integral is <=4/(k_n²r),
giving 24B/(k_n²r⁴). These three identities and an omitted-kernel-factor
mutation are actually checked in the final exact suite. Bounded linear
momentum W is an explicit hypothesis. Integrable angular tails in this
linear forced equation do not establish the coupled nonlinear Fourier
tower, global ADM map or its C1 regularity. The primary note preserves
that distinction and the nonzero background derivative (9/14) without
claiming the missing global IFT.

**Final scientific verdict:** the reviewed identities, conditional
endpoint/point-inverse estimates and empirical physical-source
diagnostics survive this bounded adversarial review. Gate A remains open
for a certified physical source-evaluated regular metric M2; Gate B
remains open for the exact global charge-fixed GL family. No numerical
positive A_star, physical finite-window energy enclosure, full nonlinear
radiation trace or first-bulge fraction is certified by this audit. The
most concrete source next step is a validated nodeless Riccati connection
and normalization with C5 regular jets. The most concrete data next step
is a high-order coupled exterior estimate proving the coefficient
integrability in (I.4) and C1 mass map, then exploiting the exact
background transversality rather than treating it as an unknown.
