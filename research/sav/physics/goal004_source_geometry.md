# GOAL-004 physical L10 quadratic source: evaluated diagnostics and the exact enclosure gap

2026-10-23. Primary bounded scientific investigator `source_geometry`, separate
from the GOAL-004 geometry and dynamics auditors. Input
`research/sav-radiation@2f4648c379c474ed124e3ef10b62efcd4a783c66`;
executor/publisher Savdeep Sethi. This work changes only its own research
notes/checks. It is non-production mathematics: one fixed saved physical
L10 profile, local radial/Fourier jet arithmetic, and exactly three new n=2
linear Green configurations. No GL eigenvalue solve, additional k, spacetime
evolution, production code, or manuscript was used. The source implementation
uses the established W=R² equations; it is a primary evaluation, **not an
independent new derivation of those tensor identities**. The attributed
GOAL003 review qualifications apply unchanged.

**Gate A verdict: NO certified physical M2.** The complete seven-component
physical quadratic source has now been evaluated with regular EF components,
checked locally at actual positive physical k, and used in a source-aware n=2
point inverse. This improves the former unevaluated physical-source state.
It does not provide the missing global C5 input enclosure, conserved weighted
source C3 enclosure, horizon divided-difference error, or homogeneous-data
norm. None of the numbers below is a global norm bound or a finite-amplitude
physical radiation estimate. The first missing inequality is stated in §5.

## 1. Definitions and exact formulas reused

Set r0=1, k=2π/10, Ω=0.06328118897590773 as the **empirical saved input**;
s=2Ω, K=2k, a=K², D=1+ar³, f=(r−1)/r, x=r+log(r−1).
The real first-order metric is A(t)H(r)cos(kz), normalized by
Hθθ/(2r²)=1 at r=2. Each complex n=±1 coefficient is H/2. A complex +2
metric coefficient must be doubled, together with its conjugate, to obtain
the real cos(2kz) tensor. There is no factorial in q2.

Write the regular first-order mode as e^{Ωv} Hhat(r), with v=t+x,
Y=e^{-Ωx}y and static derivative e^{-Ωx}y'=Y'+ΩY/f. The regular
components are

$$\widehat H_{vv}=e^{-\Omega x}H_{tt},\quad
\widehat H_{vr}=e^{-\Omega x}(H_{tr}-H_{tt}/f),$$
$$\widehat H_{rr}=e^{-\Omega x}(2H_{tt}/f^2-2H_{tr}/f
 -2H_{\theta\theta}/[r(r-1)]),\quad
\widehat H_{\theta\theta}=e^{-\Omega x}H_{\theta\theta}.$$

Substitute the exact linear reconstruction and its regular master equation
from `gl_linear_mode.md`. In EF components T=Hhat_vv, B=Hhat_vr,
C=Hhat_rr, S=Hhat_θθ, the Lorentz contraction is

$$\widehat{\mathcal H}=2TC+2B^2+4fBC+f^2C^2+2S^2/r^4.$$

This follows directly from tr[(gbar^{-1}Hhat)²], with orbit inverse
[[0,1],[1,f]]. It is not a positive geometric norm. The full physical
sources are generated using equations (4)–(5) of
`goal002_second_order.md`, including all orbit/sphere terms. The scalar
coefficients are checked against

$$\widehat Q_{zz,0}=-k^2\widehat{\mathcal H}/8,\qquad
\widehat Q_{zz,+2}=-3k^2\widehat{\mathcal H}/16.$$

All three Einstein-source Bianchi identities are checked, with
Q_AB−gbar_AB tr Q/2; Ricci Q alone is not conserved.

The forced +2 regular master J is equation (14) of
`goal003_master_independent.md`; its Schrödinger source is

$$\mathfrak F=e^{sx}\widehat{\mathfrak F},\qquad
\widehat{\mathfrak F}=-{fr\over D}J.$$

The metric uses that note's affine Z,Y,τ,j reconstruction, including the
source trace/divergence. This code does **not** explicitly evaluate all seven
inhomogeneous second-order equations on its sampled reconstructed q2; the
prior exact equivalence supports the formulas, while spline/tail/metric
residuals remain unmeasured and unenclosed. Both finite Green integrals are retained with
−1/W and dx=r dlog(r−1). No isolated divergent generic overlap is used.
This n=2 response is an imaginary-frequency, spatially evanescent
particular gravitational singlet; its size is **not** a timelike massive
KK radiation flux and it is not the massless n=0 radion.
The finite-R basis uses a decaying homogeneous endpoint jet, while the
source is retained throughout the computed interval; its uncomputed
inhomogeneous exterior tail is explicitly an error, not silently zero.
The known true particular asymptotic
u2,p=(7/64)c∞² e^{-2qr}r^{-2β1−1}(1+O(1/r)) remains the required
full exterior completion. It is not replaced by a homogeneous Robin law.

For the zero sector define Tμν=−Qμν+gbarμν tr Q/2, restricting the
full trace to the 4D indices only after forming it. The fixed quadratic
ADM shift is δm=0 and

$$\widehat\beta(r)=-\tfrac12\int_r^\infty R\widehat T_{rr}\,dR,
\qquad \widehat\mu(r)=\tfrac12\int_r^\infty R^2\widehat T_{rv}\,dR.$$

These remove the common e^{sv} factor, not the τ factor. On τ=v−r=0
multiply EF tensor coefficients by e^{sr}; the regular tetrad is the
one in `goal002_second_order.md`. The zero-metric μ,β numbers do not
include the scalar conformal contribution to the full 5D q2.

## 2. What the actual computation verifies

The saved profile is cubic-interpolated in log δ, δ=r−1. Below δ=.01,
the p=u_x/u horizon recurrence is retained through δ^18 and normalized
at δ=.005 using the saved amplitude. Above this collar, y,y' are read
from the saved p,ell interpolation, and successive local derivatives are
assigned by the **on-shell** regular equation. 65-decimal mpmath arithmetic
avoids subtraction of horizon poles. A radial Taylor jet through degree
three, with sufficient y derivatives, computes the quadratic source and
its first derivative. This is adequate for local source/Bianchi checks;
it is not the C3 source derivative norm required by the regular metric
bound. There are 481 fixed source points, 1+e^{-18}≤r≤65.

At r=1+e^{-18}, 1.00673795, 1.04393693, 2, 65 the maximum residuals
of the seven on-shell linear equations are at most 7.7e−34 (at δ=e−18 the residual remains below 9e−51); the three
quadratic Einstein Bianchi residuals are at most 2.0e−30. The scalar
Fourier identity residuals are below 2e−64. These small numbers test
local algebra at actual physical positive k. **They do not test that the
interpolated profile is one exact global solution.**

Indeed a separate actual sampled global interpolation defect on
.01≤δ≤64 gives

$$\max|p_{,\log\delta}-r(V+\Omega^2-p^2)|\simeq4.6343\,10^{-6},\qquad
\max|\ell_{,\log\delta}-rp|\simeq4.7245\,10^{-7}.$$

Those are sampled defects, not suprema or propagated error estimates.
Differentiating an interpolant and imposing the ODE at a point are distinct
operations. Setting local jets on shell can give arbitrarily small local
Ricci residuals even for globally inconsistent point data; thus the tiny
identity residuals cannot serve as input accuracy or source enclosure.

Actual negative controls in postprocessing: changing the Coulomb residual
constant fails exact SymPy simplification; doubling only Qzz,+2 produces
|div_z δEinsteinQ|=0.5542890395 at r=2, since it equals
|iK δQzz/2|. A real primary diagnostic error was caught before the final
source run: 2fBC was used instead of 4fBC in the EF contraction check,
producing a zz mismatch 0.3898909498 at r=2. The full tensor source was
unaffected. The corrected identity passes; the failure is retained here
and in the diagnostics JSON.

## 3. Quantitative empirical results and error visibility

All entries are **EMPIRICAL DIAGNOSTICS**, not enclosures. The last shown
digits identify reproducible output; they are not stated accuracy.

| Quantity | Evaluated diagnostic | Limit/error status |
|---|---:|---|
| a_h in static y~a_h δ^Ω | −38.4974128714 | Saved amplitude plus finite horizon-series matching; no interval normalization |
| Linear AH contrast / A on τ slice | 6.90024548826 | |a_h|/[4(1+k²)]; no nonlinear horizon error |
| βhat at inner source point | 78.83490305 | Integral to r=65, horizon limit/tail not enclosed |
| μhat at inner source point, δm=0 | 7.40924114 | Same; not an exact global family |
| ∫-based absolute β,μ bounds on computed interval | 78.83490305, 7.40924114 | Positivity signs happened on samples; sampled absolute quadratures only |
| ||mathfrak F_+2||L² on source interval | 16.64667045 | Fixed source grid; nested coarse difference 3.03e−5 |
| ||F||/s² diagnostic scalar upper estimate | 1039.2453 | Would be rigorous only from enclosed whole-line source norm |
| Actual truncated ||u_+2||L², finest | 113.147934 | Missing left horizon tail; not the complete norm |
| Sample +2 tetrad maximum on 1.005≤r≤10 | 92.66696 | Real cosine ≤2 times this at sampled points only; source derivatives/interpolation unbounded |
| Sample source-aware supersolution quotient Cw | 1210.2650 at r≈1.04826 | Samples do not enclose supremum |
| Static scalar source transfer integral | 13.27718166 | Frozen spatial profile; not a static GL solution or radiation coefficient C0 |

The last integral is (sqrt3 k²/16)∫r² mathcal H_static dr. It is useful
for matching a separate low-frequency scalar model; it is not the actual
frequency-dependent transfer. Its nested coarse quadrature difference is
2.07e−6. The corresponding β,μ nested differences are 2.01e−4 and
1.41e−5. These differences do not bound interpolation, mode, source-tail,
or quadrature error.

The three inverse configurations were declared before execution:

| log δ_left | r_right | points | rtol | W drift | truncated ||u||2 | sampled +2 tensor max |
|---:|---:|---:|---:|---:|---:|---:|
| −16 | 30 | 4001 | 2e−8 | 8.52e−7 | 112.361053 | 92.62660 |
| −18 | 45 | 8001 | 2e−10 | 2.37e−8 | 113.147934 | 92.66697 |
| −18 | 65 | 16001 | 2e−12 | 2.01e−10 | 113.147934 | 92.66696 |

The source interpolation is identical in all three; its errors therefore
cannot be measured by this refinement. The differing W values reflect
outer-basis normalization, not physical nonconvergence; only within-run
relative W drift is compared. At r=2, finest τ-slice coefficients are
qvv≈.02730464, qvr≈.06151390, qrr≈.30315150, qθθ≈2.32903619 for
complex +2. Coarse-to-fine differences are about 5.5e−6, 2.1e−5,
2.2e−5, 1e−7; changing the output grid also changes linear evaluation at
r=2. These are not rigorous component error bars.

The left-omitted response tail behaves as ∫e^{2sx}dx and is not zero;
it explains why a source norm with tiny horizon omission can coexist
with a noticeable truncated response-norm difference. Source decay at
infinity does not justify dropping the actual equal-exponent particular
tail in a weighted global metric norm. The reconstruction uses spline
source derivatives, which do not satisfy exact Bianchi identities
between samples; dividing its horizon numerator by f amplifies such
errors. Accordingly no near-horizon numeric tensor supremum is reported.
The analytic divided-difference identity from GOAL-003 is the appropriate
method **only after** its differentiated numerator is enclosed.

## 4. Two bounded strategies: endpoint connection and source-aware inverse

### Strategy 1: endpoint/Volterra control and monotone eigenvalue matching

This derives useful rigorous inequalities without claiming an enclosed
physical eigenpair. For the fundamental homogeneous problem write
u=e^{Ωx}v with v(−∞)=1. The regular horizon Volterra equation is

$$v(x)=1+{1\over2\Omega}\int_{-\infty}^x
(1-e^{-2\Omega(x-y)})V(y)v(y)\,dy.$$

On 0≤δ≤δ_h, |V|≤C_V δ, C_V=k²+47 is a sufficient conservative
choice. Thus its sup-norm operator has norm at most

$$\eta_H={C_V(\delta_h+\delta_h^2/2)\over2\Omega}.$$

If η_H<1, ||v−1||∞≤η_H/(1−η_H), and derivatives follow by
differentiating the integral/ODE with their explicit coefficient bounds.
At illustrative Ω and δ_h=1e−4, η_H≈.03744964. This bounds a
**horizon-normalized** shape, not its connection coefficient a_h to r=2.
The argument is uniform for a genuine enclosed Ω interval with Ω_->0.

At infinity use the Coulomb-corrected reference

$$w=e^{-qr}r^{-\beta},\quad q=\sqrt{k^2+\Omega^2},\quad
\beta=q-k^2/(2q).$$

Set R_w=(H_k+Ω²)w/w. Direct algebra, verified exactly in the script,
gives for t=1/r and D1=1+k²r³

$$R_w/r^{-2}=c_0+c_1t+c_2t^2
 +18(1-t)^2/D_1^2-6(4-5t)(1-t)/D_1,$$
$$c_0=6-q^2+4q\beta-\beta^2-\beta+q,$$
$$c_1=-17-2q\beta+2\beta^2+3\beta-q,\qquad
c_2=11-\beta^2-2\beta.$$

For r≥R>2, the explicit bound

$$C_R=|c_0|+|c_1|/R+|c_2|/R^2
 +18/(k^2R^3)^2+6(4+5/R)/(k^2R^3)$$

satisfies |R_w|≤C_R/r². Write u=w v∞ with v∞(∞)=1. Since
(w²v∞,x),x=R_w w²v∞, its double Volterra kernel has norm at most

$$\eta_\infty={C_R\over2q(1-1/R)^2R}.$$

Here −∂x log w≥q(1−1/R), and dx/dr≤(1−1/R)^{-1} were used.
For η∞<1,

$$\|v_\infty-1\|_\infty\le{\eta_\infty\over1-\eta_\infty},\quad
|v_{\infty,x}|\le {C_R\over2q(1-1/R)R^2(1-\eta_\infty)}.$$

At the illustrative physical input, R=30,45,65 give η∞≈.20318,
.12880,.08644 and relative envelopes .25499,.14784,.09462. These
are conservative endpoint-shape inequalities and show no infinite
Coulomb-tail constant is intrinsically required. They do not certify
the saved finite endpoint jet or any interior interpolation error.

There is also an exact useful monotonicity identity. For positive left
and right homogeneous solutions, p=u_x/u, define F(Ω)=p_H(xm)−p_I(xm),
xm=x(2). Differentiating p_x=V+Ω²−p² gives
(u²∂Ωp)_x=2Ωu². The admissible endpoint terms vanish, hence

$$\boxed{F'(\Omega)=2\Omega\left[
\int_{-\infty}^{x_m}\!\left({u_H(x)\over u_H(x_m)}\right)^2dx
+\int_{x_m}^\infty\!\left({u_I(x)\over u_I(x_m)}\right)^2dx\right]>0.}$$

At a matching root this is 2Ω||u/u(2)||². The saved profile yields
F'≈4.52989465, including only a leading horizon tail approximation.
If validated F error is ε_F and F'≥d_F>0 on a bracket, then
|Ω−Ωapp|≤ε_F/d_F. A sign bracket and validated interior propagation
are still required. Empirical match residual 8e−16 does not replace
ε_F. The amplitude normalization further requires a positive lower
bound on the reference functional Hθθ(2)/(8) for a fixed endpoint-normalized
solution. No such validated connection estimate is asserted.

**Outcome:** explicit endpoint envelopes and simple-root conditioning are
available, but the saved cubic profile has no enclosed interior flow,
normalization or C5 error. This is the first failed gate; it is not a
claim that the true physical eigenfunction is singular or cannot be enclosed.

### Strategy 2: source-aware weighted point inverse

Evaluate the complete physical source before applying the inverse; §3 is
the resulting actual computation. A useful independently derived comparison
by Audit I further avoids a generic L²-to-sup constant. For
w2=e^{-r/2}r/(1+ar³), define h_s=s²+H_a w2/w2≥s². The transformed
equation for u=w2 v is

$$-v_{xx}+2S v_x+h_s v=\mathfrak F/w_2.$$

If u/w2→0 at both endpoints and the source quotient has an enclosed
supremum, the maximum principle gives

$$|u(r)|\le C_w w_2(r),\qquad
C_w=\sup_r{|\mathfrak F(r)|\over w_2(r)h_s(r)}.$$

The actual GL-squared endpoint class permits these endpoint conditions.
Our sample Cw≈1210.265 does not enclose that supremum. It does locate
an interior near-horizon region requiring refinement/enclosure. Even a
certified Cw would control the scalar master only; the complete metric
still needs source derivatives and the N/(r−1) divided difference.
The primary full-source Green evaluation lowers the sampled scalar
response relative to the generic s^{-2} number by about a factor nine,
but no certified geometric M2 follows.

These two strategies exhaust this investigation's bounded source gate.
No third spectral scan, altered k, or claim of a scalar inverse as a
retarded tensor propagator is made.

## 5. Exact first missing inequality and global-data implications

For the GOAL-003 regular-metric norm X_m, its Y_m source norm reserves
m+3 radial source derivatives. Since the complete quadratic Ricci source
contains two derivatives of H, **m=0 already needs a global regular C5
first-order input bound and error** (with the stated exterior weights).
This investigation's source table contains values and first derivatives,
not that C3 source enclosure.

Let E5 be the regular EF collar/middle C5 plus weighted exterior C5 norm
of H−Happ, B5 an enclosed corresponding norm of Happ, and let δΩ be
an enclosed eigenvalue/clock error. The first unevaluated quantitative
step is a bound

$$\boxed{E_5\le\varepsilon_5<\infty\ \text{with explicit numerical }
\varepsilon_5,\qquad
\|Q[H]-Q[H_{app}]\|_{Y_0}
\le C_B(2B_5\varepsilon_5+\varepsilon_5^2)+C_\Omega|\delta\Omega|.}$$

C_B,CΩ must be evaluated in the same regular derivative/weight spaces,
uniformly over the enclosed eigenvalue and normalization. The endpoint
inequalities above do not supply this interior/connection bound. There
is no legitimate replacement of E5 by an ODE tolerance, a matching
residual, W drift, source-grid difference, or the local on-shell identity
residuals. On 0≤τ≤T a rate error also incurs e^{|δΩ|T}−1.

Only after this step could the derivative-loss inverse yield

$$M_2(T)\le e^{sT}[C_{0,0}\|Q_0\|_{Y_0}
+2C_{2,0}\|Q_{+2}\|_{Y_0}]
+\|q_{2,hom}\|_{X_0([0,T])}+C_{charge}|\delta m|.$$

The scalar regular high-derivative constant, full physical source errors,
regular reconstruction residual and homogeneous family norm remain
unclosed. The factor two is compulsory. The sampled zero-mode μ,β
suggest sizable quadratic backreaction under the reference amplitude;
they justify neither a selected A_* nor a perturbative smallness claim
at A=.1 or at a finite first-bulge contrast.

Setting δm=0 in the separated zero-mode quadrature fixes the quadratic
spatial asymptotic charge **within that perturbative solution**. It does
not construct one exact global all-orders initial-data family. For the
constraint map, every admissible source still must satisfy the KID
boundary pairing, including the nonvanishing horizon-crossing inner
boundary. Fixing an independent inner correction and all charges
arbitrarily can violate that pairing. The μhat value is a concrete
sourced interior correction at fixed quadratic ADM charge, not an
independently removable regular tension hair. Neither local collar
existence nor a perturbative particular/homogeneous split supplies a
bounded global fixed-charge right inverse or the derivative of a global
ADM mass tuning map. These are separate Gate B obstacles. A chosen
all-orders data rule may also select different second-harmonic Cauchy
transients from the separated exponential inverse used here.

## 6. Reproducibility and accepted stopping point

Executed from research/sav (exit 0 for the final two commands):

```sh
python3 verification/goal004_source_geometry.py --probe
python3 verification/goal004_source_geometry.py > verification/goal004_source_geometry_output.txt
python3 verification/goal004_source_geometry.py --post > verification/goal004_source_geometry_diagnostics_output.txt
```

The first probe exposed the contraction diagnostic error stated above;
its correction preceded the full source run. An earlier probe also failed
with a Python J/F multiplication type error; replacing J*F by F*J fixed
operand dispatch without changing mathematics. There was no failed
physical evolution or suppressed numerical solve. The three configured
linear inverse calculations all completed; their finite-domain and
interpolation limitations are retained. The postprocess was rerun only
to add the actual saved-profile flow-defect diagnostic; it performs no
ODE. Python 3.14.8, NumPy 2.4.6, SciPy 1.18.0, mpmath 1.3.0,
SymPy 1.14.0. Input SHA-256s and the source-run script hash are in
`goal004_source_geometry_results.json`; the final script and sample
hashes are in `goal004_source_geometry_diagnostics.json`. The later
postprocessing addition accounts for their differing script hashes.
The compressed 481-point source table is retained for reconstruction,
not promoted into certified continuum data.

Read scientific inputs: the full current AGENTS/GOAL-004/review-gates/runbook,
notation, the prior first-order reconstruction and quadratic-source note,
`goal002_second_order.md`, `goal003_forced_master.md`,
`goal003_master_independent.md`, `goal003_data_nonlinear.md`,
`GOAL003_REVIEW_QUALIFICATIONS.md`, and prior geometry audit/source
implementations. The analysis reuses their exact identities under their
hypotheses; it does not replay or upgrade completed Goals 001–003.
Geometry Audit I independently derived the supersolution comparison
before inspecting this implementation, then reviewed its algebra,
coordinate factors, Green/quadrature signs and explicit limitations.
Its separate audit record controls the scope of that independence.

**YES:** physical-source local algebra at positive L10 k, useful endpoint
inequalities, reproducible source-aware diagnostics and a sharp first
missing enclosure inequality. **NO:** enclosed physical M2, an exact
fixed-charge global GL family, a retarded nonlinear regular-metric bound,
a numerical positive A_*, or a physical first-bulge radiation fraction.
A natural next mathematical idea is a validated two-ended nodeless
Riccati connection using the monotone F' formula and the Coulomb
Volterra remainder, followed by interval regular jets through C5 and
the divided-difference reconstruction. It requires new authorized
bounded work; no follow-on is launched here.
