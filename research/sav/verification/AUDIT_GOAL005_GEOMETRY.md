# GOAL-005 Audit II: sourced geometry and scalar data

Reviewer: distinct AI subagent `audit_geometry`, delegated by the sole
scientific writer, 2026-10-23. This is not human referee certification.
Input checkout HEAD at launch: `0dd5be7818b2e71d28ebff2e122ac0a318a220f5`.
No commits, publication, collaborator contact, production work or new GL root
are performed by this reviewer. The historical GOAL-003 independence
qualifications remain applicable to the historical record.

## Preimplementation derivation and declared calculation

Before reading new GOAL-005 primary implementations, I inspected the scoped
contract, Goal/review gates, historical reports and the geometry/source,
scalar transfer, data/news and GOAL-003 review-qualification records.
The following derivation uses the full five-dimensional connection and
contracted Bianchi identities. The numerical residual implementation below
will use direct five-dimensional coordinate Christoffels (including the
angular derivatives), not the historical reduced W=R^2 Ricci generator.
The old master and affine reconstruction remain shared inputs, not claimed
independent discoveries. Source samples and eigenpair are historical,
empirical inputs; no residual calculation certifies them.

For a Ricci-flat product and a nonzero unitary harmonic with q_zA=0,
the linear connection formula gives, directly,

\[
R'_{zz}=K^2\tau/2,\qquad
R'_{\mu z}=iK(\nabla^\nu q_{\nu\mu}-\nabla_\mu\tau)/2,
\quad \tau=\bar g^{\mu\nu}q_{\mu\nu}.
\]

Thus R'[q]=-Q forces tau=-2Qzz/K^2 and
j_mu=div(q)_mu-d_mu tau=-2Q_mu z/(iK). Homogeneous TT is not a
consistent replacement. Expanding the exact contracted Bianchi identity
on a vacuum background with R'[h]=0 yields
div(Q-gbar tr(Q)/2)=0. For an off-shell h there are also connection times
linear-Einstein-residual terms; they cannot be discarded when estimating
the error of an interpolated profile.

The z-Gauss identity follows directly from Gamma^z_mu nu=-gamma_mu nu,z/2
and Gamma^mu_z nu=gamma^mu rho gamma_rho nu,z/2:
Rzz=-tr(gamma^-1 gamma_zz)/2+tr[(gamma^-1 gamma_z)^2]/4.
For h=H cos(kz), its quadratic term is
-k^2 tr[(gbar^-1 H)^2](1+3cos(2kz))/8. Hence the complex +2 coefficient
is -3k^2 H^2/16, while the zero coefficient is -k^2 H^2/8.
The inverse Lorentzian metric gives -2Htr^2 in static coordinates.

In the zero mode R'zz=-Box(qzz)/2 and phi=sqrt(3)qzz/4, giving
Box(phi)=-sqrt(3)k^2 A^2 H^2/16. Varying
S_phi=-(8pi G4)^-1 integral sqrt(-g)(d phi)^2 gives
T_uu=(4pi G4)^-1 N^2/r^2 and, after the sphere integral,
P=N^2/G4. This is an action/current route for the normalization,
independent of evaluating a numerical Green overlap.

For any actual particular news n and independently prescribed free news h,
E/M0=(2/r0)(||n||^2+2<n,h>+||h||^2). An omitted cross term fails
already for h=-n. This is an algebraic test of an inference, not a proof
that arbitrary prescribed null news extends to the exact global GL data
family. Static homogeneous phi=c0+c1 log(f) is excluded by fixed circle
and horizon regularity. Dynamical homogeneous scalar data are not excluded.

The parent allocated **forced n2 configuration 1 of the Goal-wide maximum
three**, before execution: historical finest domain log(r-1)=-18 through
r=65, 16001 output points, DOP853 rtol=2e-12, max step .05 in log(r-1),
same first-order outer homogeneous jet as historical computation and
same 481-point source table. This repeats one historical configuration
for an independent actual-metric residual; it is not a new refinement
or a rigorous endpoint prescription. No physical GL root configuration
is used. Missing exterior forced tail and horizon source omission are
explicit errors. The seven residuals will differentiate actual metric
interpolants, without assigning their jets to satisfy the field equations.

## Executed evidence and the actual failed physical residual

Commands, from research/sav, using the isolated interpreter:

```
.venv/Scripts/python.exe verification/audit_goal005_geometry.py > verification/audit_goal005_geometry_output.txt 2>&1
.venv/Scripts/python.exe verification/audit_goal005_geometry_diagnostics.py > verification/audit_goal005_geometry_diagnostics_output.txt 2>&1
```

The first script authors the full 5D linear Ricci operator directly from
coordinate Christoffels on (v,r,z,theta,phi), retaining angular derivatives
before setting theta=pi/2. It imports no historical verification module.
Its seven explicit equations, file SHA256s, all check results and a modest
101-point actual metric/residual table are in
[results](audit_goal005_geometry_results.json). Dependencies actually used:
Python 3.13.13, NumPy 2.4.6, SciPy 1.18.0, SymPy 1.14.0.
The second script additionally uses mpmath 1.3.0 through the historical
local source generator. No interval arithmetic is used or claimed in
this audit; exact SymPy identities and floating diagnostics are separated.

The single declared forced configuration was executed twice with identical
inputs: the first execution stopped at an overstrong mutation diagnostic;
the replay preserved the complete results. An earlier syntax error happened
before any science. Both are retained in
[failures](audit_goal005_geometry_failures.txt), not hidden as physical
successes. Postprocessing executes zero physical ODEs. This is one domain/
resolution configuration, not a refinement survey.

The historical result reproduces u(r=2)=-1.30798368033 and negative
W=-.292440184878 (relative floating drift 2.01e-10). These establish
reproduction of that approximation, not a certified inverse. EF coefficients
have the common exp(s v) removed; the historical tau-slice values include
exp(2s) at r=2, explaining their different printed numbers.

Here R'+Q is the independent **Ricci** equation residual. Vanishing all
seven Ricci equations is equivalent to vanishing Einstein equations, but
the following component maxima are explicitly coordinate Ricci residuals,
not an invariant Einstein norm. The scaled residual divides the largest
component by max(1,max_component |Q|) at each point.

| Actual sampled region | max absolute R'+Q | max scaled residual | exact mass-kernel pipeline error |
|---|---:|---:|---:|
| 1.1 <= r <= 10 | 30.72869 | .1894123 | 1.91e-7 |
| 1.005 <= r <= 30 | 3688.7901 | 6.898631 | 2.23e-5 |
| whole cutoff domain, excluding .137 in log(delta) at ends | 4.966e9 | 8.887e6 | 4.797e5 |

The first two rows demonstrate that the actual differentiated sampled
response does not have a small all-equation residual, well above the same
pipeline's smooth manufactured error. The whole-domain row is **not
usable evidence of a physical singularity or defect magnitude**: even the
exact regular mass kernel qvv=2/r gives huge floating differentiation
error on the extreme horizon collar. A regular analytic divided difference
is required there. None of the table entries is a supremum or error bound.

The algebraic zz trace and v-z constraint remain tiny (interior max
1.51e-10 and 2.11e-7), while vr fails badly. Thus checking those convenient
constraints alone misses the error. Direct divergence of the interpolated
**Einstein** source has interior sampled maxima
(5.95e-5,8.49e-5,1.74e-5) for v,r,z; the tiny historical local Bianchi
jets were not the actual interpolants' conservation residuals.

## Two diagnostic strategies distinguish interpolation from the equations

1. **Local compatible-jet comparison.** The independent 5D operator
   annihilates the physical positive-k first-order on-shell metric jet at
   r=2 to 5.28e-16. With consistent complete source derivatives through
   order three and the forced master recurrence, its seven forced
   residuals are <=2.23e-16. This local test reuses the old source generator
   (captured at function return, with its radial degree increased to five),
   and it imposes the local master recurrence. It is an independent
   equation/lift check with shared empirical radial input, **not** an
   independent source derivation or a global solution proof. The old
   on-shell source jets versus the actual source cubic already change
   qrr(r=2) by about 1.73e-4.

2. **Analytic derivatives of the actual source cubic.** Using the same
   physical response value/master derivative at r=2, analytically compose
   each actual cubic in log(delta) with radial derivatives. This avoids
   differentiating the metric output spline and retains the actual source
   interpolation, rather than assigning physical source jets. The forced
   residual is .8756302 from the left source cell and .6157712 from the
   right; the third source derivatives jump at the knot. More decisively,
   separately interpolating J and Q fails the identity J=J[Q,Q']:

   | radial derivative order | absolute J_stored - J[Q_cubic,Q'_cubic] at r=2 |
   |---|---:|
   | 0 | 7.35902e-5 |
   | 1 | .02462866 |
   | 2 | 6.32196 (left), 4.53609 (right) |

   These are the two sides of the same saved source knot, not new physical
   configurations. The independent operator therefore identifies a concrete
   compatibility/high-derivative defect of the **sampled approximation**.
   The exact master/source map passes the local test; a structural error
   in that exact map is not established by the sampled failures.

Both strategies and the manufactured mass-kernel control are recorded in
[diagnostics](audit_goal005_geometry_diagnostics.json). No third repair,
source-grid survey or new forced domain is attempted. The source has only
piecewise cubic smoothness, whereas the requested source norm reserves
three derivatives, and arbitrary separate interpolants do not preserve
the derivative identities. Agreement of master Green values across grids
cannot repair that defect.

## Independent complete physical source point and a derivative-count refinement

The diagnostics were subsequently extended with a genuinely distinct
complete quadratic source implementation. At r=2, construct the full 5D
metric, its first/second coordinate derivatives (including theta derivatives
before evaluating the equator), and inverse derivatives. Form Gamma,
C=delta Gamma, D=-gbar^{-1}h C and partial D directly, then

\[
Q_{AB}=\partial_C D^C_{AB}-\partial_B D^C_{AC}
 +D^C_{CD}\bar\Gamma^D_{AB}+\bar\Gamma^C_{CD}D^D_{AB}
 -D^C_{BD}\bar\Gamma^D_{AC}-\bar\Gamma^C_{BD}D^D_{AC}
 +C^C_{CD}C^D_{AB}-C^C_{BD}C^D_{AC}.
\]

Sixteen compact phases separately project n=0,+2. This implementation
uses only the old physical local H,H',H'' as input, **not** its reduced
source formula. All seven n=0 source components agree with that formula
within 8.89e-16 and all seven +2 components within 2.23e-15. Shared
empirical mode data are still not an enclosed global eigenfunction.
The new code, phase results and all component comparisons are preserved
in the diagnostics artifact. This strengthens the independent source
equation check without purporting to make the local-jet test global.

The primary investigator correctly raised a subtle derivative-count
question: the original Y0 contains F through three derivatives, but
J contains j_v'. For **generic** sources a mere Q C3 bound does not
bound F C3. Bianchi alone trades j_v' for f j_r' and lower terms, so it
does not remove the extra derivative.

The actual unitary physical quadratic source has additional structure.
In Gaussian z coordinates set
K^nu_mu=gamma^{nu rho} gamma_{rho mu,z}/2. Direct Codazzi gives
R_{mu z}=nabla_nu K^nu_mu-partial_mu tr K. Consequently

\[
Q_{\mu z}=-\tfrac12\bar\nabla_\nu
 (h^{\nu\rho}\partial_z h_{\rho\mu})
 +\tfrac12\partial_\mu\operatorname{tr}(h\partial_z h)
 +\tfrac12 C^\nu_{\nu\lambda}\partial_z h^\lambda{}_\mu
 -\tfrac12 C^\lambda_{\nu\mu}\partial_z h^\nu{}_\lambda.
\]

Every term has only **one noncompact derivative of H**. For TT input
C^nu_{nu lambda}=0, but the derivative count does not require deleting
that term. At fixed k, four radial derivatives of Q_mu z therefore
need H through five derivatives. In contrast the ordinary orbit/sphere
source requires two derivatives of H and its C3 norm also needs H5.
Thus C5 physical input can control the original F C3 reserve **if one
explicitly uses Codazzi for the mixed-source C4 bound**. The correct
intermediate norm is anisotropic: ordinary source C3 plus Q_mu z C4,
or a directly bounded F C3 norm; it is not generic Q C3 alone.
Background coefficients, weights and time factors still need their
evaluated bounds. A numerical direct-5D control multiplies H'' by 17:
Qvz/Qrz remain unchanged while the ordinary sources change, corroborating
the exact derivative-count argument. No new physical solve was made.

I inspected the primary `goal005_mode_bounds.py` and saved JSON after
these independent derivations. The stated mixed norm bounds raw EF
tau-slice components through five radial derivatives on [1,4], and
weighted static profiles outside, with a coarse maximum below 4e29.
Its collar divided-difference coefficient convention is consistent:
the j-th Taylor coefficient of N/(r-1) is bounded by the (j+1)-st
Taylor coefficient enclosure of N. This is an absolute physical-field
envelope, not an error enclosure around the historical cubic profile.
This geometry reviewer has not independently reexecuted or proved every
root/Arb/endpoint step; that is the separate spectral review's remit.
No full primary Y0 bound is accepted solely from the printed C5 number.

### Bounded review of the subsequent primary C3 coefficient envelope

At the primary investigator's request I then inspected
`goal005_source_bounds.py/json`, independently recounted its products and
ran [a separate exact rational check](audit_goal005_geometry_bound_review.py),
with [saved results](audit_goal005_geometry_bound_review.json) and console
output. The following are analytical counts, not merely a rerun of the
primary inequalities. A C^m derivative of p products has multinomial sum
p^m. Three-dimensional inverse coefficients have 9 and 81 summands:
I1_C4 <=9*3^4*K^2*B and I2_C4 <=81*5^4*K^3*B^2. The Christoffel
coefficients have nine half-weight terms, hence factor72 at derivative
order four. Quadratic Ricci has six derivative terms, 36 background/
second-connection products and 18 first-connection-square products.
These reproduce the proposed generous bounds.

I independently rewrote the warped orbit contribution as
-Hess(log W)-(dlog W)^2/2. With W=r^2(1+epsilon w), L=dlog(r^2),
its quadratic coefficient is

\[
w w_{,ab}+\tfrac12w_{,a}w_{,b}
-\bar\Gamma^d_{ab}w w_{,d}+\Gamma_1^d{}_{ab}w_{,d}
+\Gamma_2^d{}_{ab}L_d
+\tfrac12w(L_a w_{,b}+L_b w_{,a}).
\]

The separate script verifies this formal expansion exactly and bounds
it by 12 Wb^2+24 Gamma2 K+24 Gamma1 Wb+81 Gamma0 Wb^2+27 K Wb^2,
where Wb=1e5 is the relative-sphere coefficient bound per B. It is
smaller than the primary warped envelope. The normalized sphere Hessian
counts and contraction by inverse coefficients also fit the proposed
caps, including the r^2 multiplication factor16.

A precise trace caveat was sent to the primary: the spherical trace must
either use the already bounded Qtheta/r^2 or include derivatives of
r^-2 (through order three, at most24). The check uses the more conservative
bracket (9K+48) instead of the initial (9K+2); the resulting coefficient
still lies below 1e44. Thus the caveat does not invalidate the final
large bound. K=1000 also safely covers fifth background derivatives
needed by Gamma0_C4. Fourier projection introduces no enlargement of
these pointwise phase bounds. The squared input exponential and
sigma<.65 leave a decay margin>.35; r^2 exp(-.35(r-4))<100, and the
interior r^2<=16 is covered by the same final allowance.

**Review decision:** conditional on the independently validated physical
mixed C5 envelope, the supplied all-seven-component C3 Ricci/Einstein
bound **1e110** in its declared interior/exterior weights is supported
by the explicit product counts. This upgrades the source-component
verdict compared with the earlier historical-profile assessment. It is
not an error bar around historical source samples and does not alone
prove the full original Y0 norm, mixed-source C4/F_C3, horizon-forcing
norm, scalar response or full M2. Those require their separate bounds.

### Subsequent exact-source scalar-master bound

The primary then supplied `goal005_master_bounds.py/json`. I inspected
its rational source coefficient lists against the static forced master
and regular EF J formula, its interval polynomial denominators and
subdomain cover. The replacement of i by a unit real phase is legitimate
for these **individual coefficient absolute bounds**; it is not a new
real-valued forced equation. The signs and common factor
F=-f^2 r F_y/D, equivalently F=-f r exp(sx)J/D, are consistent with
the earlier Green convention. No Fourier doubling is inserted: this is
the complex +2 source-selected solution.

On [1,4], each of seven J coefficients is bounded by 1e10, giving
|J|<=Cj=7e10 Q with Q=1e110. Therefore
|F|<=4 exp(4s) Cj delta^(s+1), and dx<=4 ddelta/delta gives the
primary horizon/interior integral bound. Outside r=4, six static source
coefficients are bounded by 1e10 r^6; the stronger underlying source
envelope Q r^2 exp(-(r-4)) yields |F|<=CF r^8 exp(-(r-4)), CF=1e12 Q.
The exact exterior moment is

\[
\int_4^\infty r^{16}e^{-2(r-4)}dr
=\sum_{j=0}^{16}{16\choose j}4^{16-j}\frac{j!}{2^{j+1}},
\qquad dx/dr\le4/3.
\]

The independent bound-review script was extended to check this moment
and the whole norm using exact rational arithmetic with looser factors:
0.12<s<0.14 implies exp(8s)<9, 3^(2s+2)<27 and 2s+2>2. These alone
give ||F||2<1e128. The previously established positive-supersolution
form for a=K^2>3/2 and s>0 then gives

\[
\|u\|_2\le s^{-2}\|F\|_2<5\,10^{129},\qquad
\|u\|_\infty\le
\sqrt{2s^{-2}(s^{-1}+\tfrac52s^{-2})}\|F\|_2<2\,10^{130}.
\]

**Updated master verdict:** these very coarse **exact particular scalar
master** bounds are supported, conditional on the verified physical
input/source prerequisite. This is a successful route around the defective
historical source interpolant. It is not an approximation-error certificate
for that interpolant, a full regular metric norm or a general retarded
propagator. Only source value/first derivative and compact J derivatives
through two were used; the generic F_C3 issue above is not needed for
this scalar L2/sup result. The first missing full-geometry inequality is
an explicitly evaluated compatibility-preserving regular affine-lift
constant together with the zero-mode scalar/mass/lapse and chosen
homogeneous-data bounds. Large sampled residuals do not prove these
constants cannot be obtained.

The final [bound-review JSON](audit_goal005_geometry_bound_review.json)
pins SHA256s for the reviewed source/master scripts and results, including
the primary's applied +48 trace correction. No additional physical ODE
configuration or root calculation was run by this review.

Final bounded refresh: the primary replaced exactly representable float
subdomain endpoints with explicit rational Arb endpoints and tightened
the reported inequalities. The independent rational check now uses
s>0.1265 and compares the **full rational squared source estimate** to
the squared u bounds above. Merely combining ||F||2<1e128 with s>0.1265
would not establish the sharper 5e129 L2 bound; retaining that more
precise squared estimate does. The exact check passed without any ODE
rerun. I also read the completed `physics/goal005_enclosures.md`; no
substantive geometry/source/master or physical-scope misstatement was
identified in this bounded review. Its SHA256 is recorded with the
refreshed source/master hashes. Spectral certification remains supported
by its separately identified spectral audit, not a new proof by this
geometry reviewer.

## Explicit error inequalities and the horizon compatibility requirement

There is a useful exact scalar-source estimate with all local constants
specified. In any fixed basis let A=gbar^{-1}H and B=gbar^{-1}Happ be
four-by-four matrices, with an induced Euclidean operator norm in that
basis. If ||B||<=b and ||A-B||<=epsilon, then

\[
|\operatorname{tr}(A^2-B^2)|\le4\epsilon(2b+\epsilon),\qquad
|\delta Q_{zz,0}|\le {k^2\over2}\epsilon(2b+\epsilon).
\]

The proof is tr(A^2-B^2)=tr[(A-B)B+B(A-B)+(A-B)^2] and
|tr M|<=4||M||. The +2 coefficient's corresponding constant is
3k^2/4. If k is also uncertain, add
|delta(k^2)| b^2/2 (zero) or 3|delta(k^2)| b^2/4 (+2), taking
the first term's k as the exact/enclosed target. This bounds the
Lorentz contraction's **absolute error**, without making it positive.
The norm is frame dependent and must be bounded in a regular fixed frame;
these formulas do not provide a global physical b or epsilon or the
other six source components' C3 bounds.

The independent coordinate divergence at the horizon reobtains

\[
\mathcal B_v(1)=A'+B+2A+sa\tau/4-sQ_\Omega+aj/2,
\quad Q_{zz}=-a\tau/2,\quad Q_{vz}=-iKj/2.
\]

Together with the historical affine lift's independently checked local
equivalence, N(1)=-4 B_v(1)/[s(a+1)]. A nonzero compatibility defect
there leaves a term r N(1)/(r-1) in qrr. A small residual number alone
cannot bound its horizon supremum. For two compatible regular numerators
N,Napp with N(1)=Napp(1)=0, however,

\[
|\delta q_{rr}|\le(1+d_c)\|\delta N'\|_{L^\infty([1,1+d_c])},
\quad
\partial_r^m\frac{\delta N}{r-1}
=\int_0^1 t^m\delta N^{(m+1)}(1+t(r-1))dt.
\]

Thus compatibility must be enforced as an exact analytic identity, and
the differentiated numerator error must be enclosed. Separate floating
source interpolants with uncontrolled derivatives do neither. The
first missing inequality for a full physical M2 remains a globally
weighted regular source-C3 error, derived from the physical C5 metric
input, carried through a compatibility-preserving lift and endpoints.
An independently enclosed spectral root alone does not supply it.

## Mutation coverage, scalar radiation and final audit verdict

The main script executes 21 checks, including the required intentionally
bad cases: missing Fourier half; Euclidean Htr sign; reversed W sign;
lost EF division; dropped Bianchi compatibility; static tension regarded
as regular scalar hair; omitted half Lie-square gauge completion; and
discarded homogeneous scalar interference. The Fourier and W mutations
also have actual physical-input witnesses. Test counts are not proofs
of the global physical geometry. Exact pure-gauge pullback supplies the
Lie-square null test: omission yields a spurious zero scalar -1/2 in the
manufactured circle coordinate example.

This audit independently derives and checks the scalar source/flux
normalization, but does **not** recompute a verified physical C0 overlap.
No error interval for C0 or a finite-time physical energy is certified.
The zero metric requires beta/mu charge quadratures plus the scalar
conformal contribution; the complex +2 metric must be combined with its
conjugate. Independent n2/scalar Cauchy data remain outside the separated
particular response. A general-data M2 cannot omit their norm, and a
first-bulge energy cannot follow from a particular response or arbitrary
static tension. The action-current derivation and interference controls
support these distinctions. No full exact global GL/scalar family,
nonlinear news trace or fourth-order remainder is established here.

**Audit classification: MEANINGFUL_PARTIAL.** Exact local equations and
the existing source-aware map survive this independent 5D implementation
check. The historical physical approximation fails a newly executed
all-seven-equation residual test, with the specific source interpolation
and horizon-error mechanisms isolated. The audit accepts the subsequent
coarse all-component C3 bound subject to its audited physical C5
prerequisite, and the subsequent exact particular scalar-master L2/sup
bounds, as specified above. It does not accept those bounds as the full
Y0 norm, physical full M2, finite-time radiation or actual first-bulge
fraction. Those verdicts require separate demonstrations, not this
audit's local PASSes.
