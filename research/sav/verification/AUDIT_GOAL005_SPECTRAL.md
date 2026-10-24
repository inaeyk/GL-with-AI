# GOAL-005 adversarial Audit I: spectral input and endpoint certification

Date: 2026-10-23. Distinct AI investigator `audit_spectral`, delegated by
the sole scientific writer. Starting branch `research/sav-radiation` at
`0dd5be7818b2e71d28ebff2e122ac0a318a220f5`; status was clean when inspected.
No commits, pushes, production operations or new physical-domain configurations
were performed by this reviewer. Endpoint parameters below belong to the
primary investigator's declared GL1 configuration. All writes are this audit,
its independent script and outputs under `research/sav/verification/`.

**Current review verdict: primary certified root and scalar nodeless
connection accepted at the scope stated in section 5.** The independent
endpoint bounds above are a separate constructive check. A physical C5
upper envelope is accepted at the explicit coordinate-profile scope in
section 6. Check counts are not counts of proofs.

## Independence and inspected premises

Before seeing any new primary implementation, the reviewer read the scoped
AGENTS contract, complete GOAL-005 and review gates; historical conventions,
handoff/status, source and decision records; the final conclusions and
relevant scientific sections of GOAL-001--004 reports; the linear-mode note,
GOAL-004 source-geometry and its separate geometry audit; relevant historical
verification and negative records; and the attributed GOAL-003 review
qualifications. The exact historical GL scalar potential and reconstruction
are **shared premises**, not independently rederived Einstein equations here.
The review independently derives a linear Wronskian identity and a weighted
Riccati endpoint-error argument, then implements their algebra without
importing any primary or historical verification module. This is a separate
AI analysis, not human peer review or a fully independent premise set.

Chronology: the Wronskian derivation was communicated to the primary before
its implementation was inspected. The independent script and endpoint
certificate ran before any new primary implementation was read. The primary
provided its configuration and trial parameter range, but no computed mode.
Hash-pinned historical inputs and actual package versions are in the JSON.

## 1. Linear Wronskian route to the matching derivative

Use r0=1, a=k², k=pi/5, f=(r-1)/r, x=r+log(r-1), and

\[
u_{xx}=(V+\Omega^2)u,\qquad
V=af+\frac{(6r-11)(r-1)}{r^4}
+\frac{18(r-1)^2}{r^4(1+ar^3)^2}
-\frac{6(4r-5)(r-1)}{r^4(1+ar^3)}.
\]

Differentiate this **linear** equation in Omega. With z=partial_Omega u,

\[
W=u z_x-u_xz,\qquad W_x=2\Omega u^2,\qquad
\partial_\Omega(u_x/u)=W/u^2.
\]

For the horizon branch u~exp(Omega x), W~exp(2Omega x), hence W tends
to zero at minus infinity even though z/u~x. For the decaying infinity
branch u~exp(-qr)r^-beta, q=sqrt(a+Omega²), parameter differentiation
only adds polynomial/logarithmic factors, so W also tends to zero there.
Multiplying a branch by any differentiable nonzero normalization has no
effect on W/u². On a branch with no zeros at the match and no poles in
the continued logarithmic derivatives,

\[
F=p_H-p_I,\quad
F'(\Omega)=2\Omega\left[
\int_{-\infty}^{x_m}(u_H/u_H(x_m))^2dx+
\int_{x_m}^{\infty}(u_I/u_I(x_m))^2dx\right]>0.
\]

This proves simple-root monotonicity on a connected certified branch.
It does not prove the existence of a root, a global unique instability,
or absence of Riccati poles in a numerically sampled interval. A sign
bracket and the nodeless continuation still need rigorous proof.

## 2. Independent endpoint residual and contraction bounds

Let p0 be a reference logarithmic derivative and define

\[
R_0=(p_0)_x-V-\Omega^2+p_0^2,\quad p=p_0+e,
\qquad e_x+2p_0e=-R_0-e^2. \tag{1}
\]

This residual formulation uses a rational function on a compact endpoint
coordinate interval; it does not assume an inverse-radius series converges.

### Horizon

Set delta=r-1 and p0=Omega+c delta, where cancellation of the coefficient
of delta in (1) yields

\[
c=\frac{a-5+6/(1+a)}{1+2\Omega}.
\]

For 0<delta<=deltaH, assume |R0|<=CH delta² and p0>=h>0.
Since d log(delta)/dx=1/(1+delta),

\[
\delta(y)/\delta(x)\le
\exp[-(x-y)/(1+\delta_H)]\quad(y\le x).
\]

Integrating (1) forward from minus infinity therefore maps the ball
|e|<=MH delta² into itself if

\[
CH+MH^2\delta_H^2\le\lambda_H MH,\qquad
\lambda_H=2h+2/(1+\delta_H),
\]

and is a contraction when 2 MH deltaH²/lambdaH<1. The fixed point
constructs the ingoing logarithmic derivative. Integrating p-Omega,
which is integrable at the horizon, constructs a positive u with
u exp(-Omega x)->1, so this is the desired branch and has no horizon
collar node. These scalar bounds do not by themselves enclose physical
tensor derivatives through order five.

### Infinity

Set t=1/r and

\[
p_0=-q+a_1/r+a_2/r^2+a_3/r^3,\quad a_1=a/(2q),
\]
\[
a_2=(-a_1-6+a_1^2)/(2q),\qquad
a_3=(a_1-2a_2+17+2a_1a_2)/(2q).
\]

The script derives these by cancelling the first three powers of t,
independently of importing the historical recurrence. For r>=R suppose
|R0|<=CI/r⁴ and p0<=-cI<0. Integrating (1) **backward from infinity**
gives a kernel bounded by exp[-2cI(y-x)]. Since r(y)>=r(x), the ball
|e|<=MI/r⁴ is invariant and contractive if

\[
CI+MI^2/R^4\le2c_I MI,\qquad MI/(c_I R^4)<1. \tag{2}
\]

This constructs a real logarithmic derivative with the correct decaying
asymptotic. A positive solution reconstructed by integrating it has no
exterior node. In radial coordinates

\[
p_0/f=-q+(a_1-q)/r+O(r^{-2}),\qquad
\beta=q-a_1=(a+2\Omega^2)/(2q)>0.
\]

Thus u~exp(-qr)r^-beta. Positivity/nodelessness here is on the endpoint
exterior only; it says nothing yet about its continuation to r=2.

### Actual outward-rounded bounds

The configuration is deltaH=exp(-28), R=40, exact k=pi/5 and **trial**
Omega in [0.06327,0.06330]. This trial interval is not called a root
enclosure by this audit. Arb uses 160-bit precision, including pi, exp,
sqrt, rational parameter intervals and Horner evaluation of the exact
rational scaled residuals on [0,deltaH] and [0,1/R]. Every denominator
is proved not to contain zero. The actual Arb endpoints are slightly wider
than decimal inputs; no floating coefficient is silently treated as exact.

| Bound, uniform in the trial interval | Safe rounded value |
|---|---:|
| CH | <=3.552 |
| lambdaH | >=2.12653 |
| MH chosen by actual interval computation | <3.341 |
| horizon p error at deltaH | <1.60e-24 |
| horizon contraction factor | <1.502e-24 |
| CI | <43.650 |
| cI | >0.6235176 |
| MI | <70.006 |
| infinity p error at R=40 | <2.74e-5 |
| infinity contraction factor | <4.386e-5 |

These are bounds on the exact endpoint branches relative to the specified
reference p0. They are not eigenvalue errors. For any fixed trial Omega
in the interval, evaluate p0 at that Omega and add the uniform error.
Using the whole-parameter p0 ball as an input at a single trial Omega
would introduce unnecessary parameter width. The JSON records the actual
interval quantities, exact residual expressions and successful inequalities;
the safe decimal table is not substituted for those inequalities in code.

## 3. Physical normalization and adverse cases

From the historical reconstruction y=(1+ar³)u/r and
H_theta theta=-r²y/(2D)-(r-1)y'/(aD), independent substitution gives

\[
\frac{H_{\theta\theta}(2)}{8u(2)}
=-\frac18-\frac{16a-1}{32a(1+8a)}-\frac{p(2)}{8a}.
\tag{3}
\]

The mode must be divided by this functional with a certified nonzero
denominator, not merely set u(2)=1. Its sign may be negative even for
a positive scalar branch; a negative physical amplitude scale does not
create nodes. Any uncertainty in p(2), a and its connected amplitude
must propagate into the full metric normalization.

The script actually rejects these deliberately incorrect conclusions:

1. The outgoing horizon branch is acceptable because it solves the leading
   indicial equation. In EF scaling it behaves as exp(-2Omega x), which
   diverges at the future horizon.
2. Reversing the Coulomb exponent beta. Its residual has nonzero 1/r
   coefficient 2a-4q²=-2a-4Omega².
3. Replacing u_x/u by u_r/u. Their difference for exp(Omega x) is
   Omega(1/f-1), so the omitted radial Jacobian is detected exactly.
4. Finite endpoint samples guarantee a pole-free Riccati denominator.
   u=cos x solves u_xx=(V+Omega²)u with V=-2, Omega=1 and has u=1 at
   x=0,2pi but two interior zeros. Whole-interval Arb cosine contains zero.
5. A small formal truncation term is an actual endpoint error bound.
   The manufactured exact p=Omega+delta/(1+delta)² has truncation
   Omega+delta and error delta²(2+delta)/(1+delta)², exceeding the
   deliberately claimed delta²/2 bound by almost four.
6. A normalization interval containing zero can be inverted safely.

Controls 4 and 5 are manufactured examples, not physical GL runs. None
of the controls consumes a new physical root or domain configuration.

## 4. Reproducibility and limits

From `research/sav`, the executed command is

```powershell
.venv/Scripts/python.exe verification/audit_goal005_spectral.py > verification/audit_goal005_spectral.txt
```

Python 3.13.13, SymPy 1.14.0, python-flint 0.9.0; Arb precision 160 bits.
The JSON carries hashes, all actual checks and endpoint enclosures. The
program uses exact symbolic reduction followed by outward-rounded ball
arithmetic. No SciPy tolerance, interpolated saved profile, asymptotic
series convergence, or observed refinement difference is used as a proof.

The audit's new concrete progress is an independently derived and
machine-evaluated endpoint p certificate, usable by a validated finite
connection solver. It does not certify that connection, the physical root,
the full EF C5 norm, the quadratic source or M2. Derivative bounds for the
normalized physical tensor need the reconstructed regular equations,
coefficient bounds, divided differences and controlled normalization.

## 5. Subsequent primary implementation review

After the independent endpoint audit ran, the primary provided
`goal005_validate_gl.py` and `goal005_gl_results.json`. The full solver
was inspected, including its actual failed first fixed-step Picard test,
retained in `goal005_failures.txt`, and its bounded subdivision repair.
The reviewer did not independently rerun a second full physical root
solver or import the primary implementation into the independent controls.
The new separate checks test the essential Taylor/flow claims on exact
linear fundamental matrices and independently derived Riccati flows.

### Finite-flow enclosure argument

The independent review supports the following implementation logic:

1. In t=log(delta), p_t=r(V+Omega²-p²), r=1+exp(t). For each step,
   strict containment of `p + [0,h] rhs(time_interval,tube)` in the
   candidate tube is a sufficient Picard enclosure. The rhs is smooth
   on this finite coefficient domain and locally Lipschitz in p.
   A solution cannot escape the tube while its full integral image is
   in its interior. Recursive bisection merely refines enclosure cells;
   it does not change the physical domain or boundary problem.
2. The central Taylor polynomial uses degrees zero through eleven.
   The coefficient numbered twelve computed with *all intermediate
   times and all states in the tube* encloses the twelfth derivative
   divided by 12!, by the differentiated ODE recurrence. Adding its
   absolute upper bound times |h|^12 is a valid remainder. It is not
   an empirical difference or an asymptotic-series error assumption.
3. For each fixed same k/Omega, compare the interval-start trajectory
   to the center-start trajectory. Their difference obeys
   d_t=-r(p1+p2)d. Both trajectories lie in the tube. Its growth is
   bounded by exp(sup[-2r*tube*h]), retaining the sign of h. This
   controls the initial radius without repeatedly wrapping all initial
   uncertainty through nonlinear Taylor coefficients. Coefficients
   still contain the full parameter interval, so parameter dependence
   is included; double counting parameter width only enlarges bounds.
4. The infinity starting time log(39) is an Arb enclosure. Its exact
   value and its exact fraction h=-log(39)/N lie in all interval
   arguments used. Lost correlations can widen the enclosure, but do
   not remove the exact time path. The integer step count is a chosen
   numerical partition; its construction from a floating approximation
   is not itself a mathematical coefficient or claimed accuracy.
5. Bounded real Riccati continuation constructs positive u by integrating
   exp(int p dx). Together with the independently checked nonzero
   endpoint branches, this proves scalar nodelessness over both full
   half-lines for every Omega in the tested parameter interval.

The primary uses the older, looser horizon/Coulomb Volterra endpoint
bounds, not the new sharper estimates in section 2. Their formulas were
separately checked. The horizon estimates divide the bound on v_x by
the positive lower bound v>=(1-2eta)/(1-eta). At infinity the same
division changes 1-eta to 1-2eta in the p error denominator. Both eta
values are less than one half, including for the whole Omega interval.

### Actual root evidence inspected

At the two endpoints of the tighter bracket, the saved outward F values
are `[-4.1e-8 +/- 5.38e-10]` and `[5.0e-8 +/- 2.60e-10]`, respectively.
The same-domain whole-parameter run has finite Picard tubes throughout
and a normalization functional `[-0.0771376 +/- 5.54e-8]`, which excludes
zero. Hence the sign change, continuity, independently derived strict
F' positivity and scalar nodelessness establish a unique root **within
this nodeless bracket**:

\[
\boxed{0.06328118\le \Omega r_0\le0.06328120},
\qquad \text{width}=2\times10^{-8}.
\]

This does not prove a unique instability in the entire spectrum.
The printed Arb balls above deliberately retain their conservative
display widths; full lower/upper bounds are requested in the primary
final output. The physical scale functional agrees with (3), and its
inverse is justified because the whole bracket excludes zero.

The first inspected root-results source hash was
`8be4340164caad04f42de05397048e79fc37878ebc69b5d66b20fdea8947b1ad`.
The reviewer noticed subsequent output-bound/cell-logging additions made
the live source differ from that saved hash, and requested a final rerun.
This is a provenance refresh requirement, not a newly discovered equation
defect. The final primary ledger must identify the rerun's actual hash.

**Refresh verified:** the later root output and live source both hash to
`e91bd964f81ca7d387aa082926948fb9fe9f994328e94e5af46e459497a5d788`.
The endpoint F bounds, rounded outward for this table, are

| Omega | F lower | F upper | Enclosure width |
|---|---:|---:|---:|
| 0.06328118 | -4.085700e-8 | -4.046279e-8 | <3.9420e-10 |
| 0.06328120 | 4.974090e-8 | 5.013511e-8 | <3.9420e-10 |

The whole tight-bracket normalization lies inside
[-0.077137656,-0.077137624], excluding zero. Full outward Arb endpoint
strings remain in the primary JSON.

### Independent manufactured flow checks actually run

`audit_goal005_spectral_flow.py` does not import the primary solver and
implements the Taylor coefficients of p'=4-p² by an explicit convolution
recurrence. Its separate exact route propagates (u,u_x) by the 2x2
cosh/sinh fundamental matrix. Both h=1/32 and h=-1/32 and four initial
values were tested, followed by a full initial interval and monotone
Möbius endpoint test. All exact values lie in the independently computed
Taylor enclosures. Deliberate omissions of the remainder, initial radius
and signed backward growth are rejected. The command was

```powershell
.venv/Scripts/python.exe verification/audit_goal005_spectral_flow.py > verification/audit_goal005_spectral_flow.txt
```

These are meaningful exact-solution checks of the validator mechanism,
not a second physical root computation or a separate derivation of the
Einstein-to-master reduction. The audit concludes that the inspected
primary root proof is constructive and passes at A1; it does not promote
that conclusion into A2, the source norm or full second-order geometry.

## 6. Coarse physical C5 bound: subsequent independent review

The primary then supplied `goal005_mode_bounds.py` and its actual JSON.
This is a bound on the normalized exact mode, **not** a small difference
from the historical interpolated profile. The enormous constant is
mathematically useful as a finite envelope and practically useless as
an accuracy estimate. The following components were inspected in full.

### Horizon derivatives and division

Independent substitution Y=exp(-Omega x)y into the historical scalar
equation gives

\[
fY''+bY'+cY=0,
\]
\[
b=2\Omega+1/r-5f/r+6f/(rD),\quad
c=3/r^3-a-3/(r^3D)+\Omega[-4/r+6/(rD)].
\]

These two coefficients and b(1)=1+2Omega were checked symbolically in
the independent audit script. On the collar delta<=1/4096, the primary
bounds the horizon-normalized scalar coefficient by outward rectangle
quadrature of the full Riccati tubes, then the horizon Volterra bound
gives an explicit B0 for |Y|. The positive ingoing Frobenius branch is
analytic after removal of exp(Omega x): its recurrence divisors n(n+2Omega)
are nonzero for positive n. Thus the regular derivatives used next exist.

After differentiating n times, z=Y^(n+1) satisfies

\[
fz'+(b+nf')z=-\sum_{j=2}^n{n\choose j}f^{(j)}Y^{(n+2-j)}
-\sum_{j=1}^n{n\choose j}b^{(j)}Y^{(n+1-j)}
-\sum_{j=0}^n{n\choose j}c^{(j)}Y^{(n-j)}.
\]

All terms appear in the implementation. Because b>1 and f'>0 throughout
the collar, dividing the source bound by inf(b), instead of the larger
inf(b+nf'), is conservative. The integrating-factor/maximum-principle
bound includes the regular endpoint value z(1)=source(1)/(b+nf')(1).
It supplies derivatives through Y^(8), with interval coefficient jets.

The reviewer independently checked the primary Z,B,T,S,N reconstruction
against all four historical static components after coordinate/time
conversion and use of the scalar equation. Every difference vanishes
exactly. Independently, substitution of Y'(1)=-c(1)Y(1)/b(1) gives N(1)=0.
For C=N/f=r N/delta,

\[
\partial_\delta^j(N/\delta)=\int_0^1t^jN^{(j+1)}(t\delta)dt.
\]

Hence its jth Taylor coefficient is bounded by the absolute (j+1)th
Taylor coefficient of N. The primary `dd[j]=abs(NN[j+1])` has the
correct factorial convention; an extra factor of j+1 would be wrong.

### Middle and infinity

Finite cells use the *entire* validated state tube and interval rectangle
integral for log u, followed by ODE-determined local derivative jets over
the whole cell. This is a continuum enclosure, not sampling. Correlations
are discarded conservatively. The terrible middle bound comes largely
from bounding singular-looking static terms separately just outside the
tiny collar, where cancellations are lost; it is not evidence the exact
mode is enormous or singular.

On r>=40, z=1/r is compactified and local radial translation is
z(h)=z/(1+zh). Factoring each static component as r*u*E(z,p), the series
(1+zh)*(u(r+h)/u(r))*E(z(h),p(r+h)) correctly generates its derivatives
divided by r*u. The compact potential uses the correct powers z^8 and
z^5 in its two rational terms. Derivatives are generated from the exact
ODE at each possible point, not by differentiating a coarse interval
bound as if it were a function.

For w=exp(-qr)r^-beta, beta>0, the Volterra bound implies

\[
|u(r)|\le |u(40)|\frac{e^{-q(r-40)}}{1-2\eta_I}.
\]

Thus the exterior weight exp((r-4)/2) satisfies

\[
\sup_{r\ge40}e^{(r-4)/2}r|u(r)|
\le\frac{|u(40)|e^{18}}{1-2\eta_I}
\left(40+\frac1{q-1/2}\right),
\]

which is the primary radial factor. Here q>k=pi/5>1/2 is explicit.
The inequality (40+s)exp(-(q-1/2)s)<=40+1/(q-1/2) is deliberately loose.
The reviewer requested replacement of a final floating `max` selection
by pure outward arithmetic or a direct all-components comparison against
the proposed rounded common bound. That selection is not part of the
underlying per-component enclosures.

**Fix verified:** the refreshed code asserts every one of the 24 full
component/derivative bounds is strictly below exact Arb `4e29`, and the
saved mode-results source hash agrees with the current code:
`fbab9e5ed3ee81e7a4b5221268109518f8823a746443dc4fb818079a221ad34a`.
This removes the floating maximum-selection concern. The bound M<4e29
used in the conversion below is therefore an actual checked common bound.

### Common slice and precise norm

The primary initially bounds EF components at tau=v-r=0 for r<=4 and
static t=0 radial profiles for r>=4, with raw sphere coefficient S=Htheta
theta and radial derivatives through five. This mixed profile norm must
not silently be called a common-slice tetrad norm. The reviewer supplied
the following explicit conversion to a common tau=0 EF coordinate norm.

On r>=4, delta>=3 and 0<Omega<1. With g_m=delta^-Omega f^-m,

\[
|g_0^{(j)}|\le j!/3^j,
\]
\[
|g_1^{(j)}|\le j!/3^j+(j+1)!/3^{j+1},
\]
\[
|g_2^{(j)}|\le j!/3^j+2(j+1)!/3^{j+1}
+(j+2)!/(2\,3^{j+2}).
\]

These follow by expanding f^-1=1+delta^-1, f^-2=1+2delta^-1+delta^-2
and bounding the rising factorials with Omega<=1. For EF rr the worst
Leibniz sum over orders n<=5 is exactly

\[
\max_{0\le n\le5}\sum_{j=0}^n{n\choose j}
(G_{0,j}+2G_{1,j}+G_{2,j})=20638/243<85.
\]

The independent script checks all six rational sums. Thus a rigorously
verified mixed bound M<4e29 implies a common tau=0 **EF coordinate-component**
bound below 3.4e31, for the piecewise weight 1 inside r=4 and exp((r-4)/2)
outside, applied to each component derivative (the weight is not itself
differentiated). Time and compact-coordinate derivatives add Omega^a k^b,
which are <=1; this covers mixed derivatives of total order <=5 at this
fixed mode. The background inverse is nonsingular in EF coordinates.
Angular structure is the fixed SO(3) sphere tensor; sphere/tetrad/frame
conversion requires its own explicit coefficient bounds if a tetrad norm
is quoted. No such bound is silently folded into 3.4e31.

This finite constant bounds the exact normalized mode in a defined
regular coordinate norm. It is not a useful error estimate around a
computed H_app, not a physical quadratic source bound, and not M2.
Its applicability to a chosen Y0 source/metric space still requires
explicit product, connection and derivative estimates in that space.
