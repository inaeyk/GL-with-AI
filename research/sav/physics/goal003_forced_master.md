# GOAL-003: an explicit forced second-harmonic master and reconstruction

2026-10-22; input ef646d56ab7d8bebca578afab53e62da5146ec05 on
research/sav-radiation. All radii and rates in this note are dimensionless
r/r0, kr0, Omega r0: set r0=1. Restore dimensions before using physical
components. Let K=2k, s=2Omega>0, a=K², f=(r−1)/r, D=1+a r³.
Coefficients below are **complex +2 Fourier coefficients** with time
exp(st) removed. No factorial is used in the metric expansion. Thus
Qzz,+2=−3k² H²/16 for h1=exp(Omega t)H cos(kz), not its doubled cosine
coefficient. This master is a massive gravitational singlet; it is not
the zero-mode radion.

## 1. Explicit gauge completion and input source

First make h1 unitary with the unique nonzero-k compensator xi1. For
arbitrary original q2 form

$$\widetilde q=q_2+\mathcal L_{\xi_1}h_1
                  +\tfrac12\mathcal L_{\xi_1}^2\bar g.$$

On the +2 sector set eta_z=−tilde qzz/(2iK) and
eta_mu=−(tilde q_mu z+∇mu eta_z)/(iK), and define
qU=tilde q+L_eta gbar. This is local in t,r, uses only 1/K and its
square, and is smooth in EF coordinates when the input is. It fixes all
nonzero-K gauge freedom. First-order zero-mode coordinate freedoms are
fixed by the existing circle/clock/phase conventions. The omitted half-Lie
term fails the pure-gauge test. Gauge invariance here means the completed
representative under these fixed background conventions, not invariance
under changes of the physical asymptotic clock or circle.

Calculate Q=Pi_2 R^[2][h1U,h1U] by the explicit tensor functional

$$C^A{}_{BC}=\tfrac12\bar g^{AD}
(\bar\nabla_Bh_{DC}+\bar\nabla_Ch_{DB}-\bar\nabla_Dh_{BC}),
\quad D^A{}_{BC}=-h^A{}_D C^D{}_{BC},$$
$$Q_{AB}=\Pi_2\left[\bar\nabla_C D^C{}_{AB}
-\bar\nabla_BD^C{}_{AC}+C^C{}_{CD}C^D{}_{AB}
-C^C{}_{BD}C^D{}_{AC}\right].$$

Together with the already explicit H(y1,y1') reconstruction this is a
finite differential source functional; it conceals no inverse. The reduced
W=R² version in goal002_second_order.md is an independent route. On shell,
∇^A(Q_AB−gbar_AB Qtrace/2)=0. Ricci Q alone is not conserved. Both the
linear on-shell equation and the Fourier factors are essential.

Write the completed unknown as (T,B,C,S)=(qtt,qtr,qrr,qtheta theta),
qphi phi=S sin²theta, qzA=0. Define known source functions

$$\tau=-2Q_{zz}/a,\qquad j_t=-2Q_{tz}/(iK),\quad
j_r=-2Q_{rz}/(iK).$$

They are affine trace/divergence data. They cannot be set to zero in the
forced problem. All following derivatives are d/dr at fixed static t.

## 2. Master and fully local affine source lift

Define the gauge-completed master

$$\boxed{y=2ar\left[{r-1\over s}B-S\right].} \tag{1}$$

Its equation is

$$\boxed{y''+P y'+(Q_h-s^2/f^2)y=F,} \tag{2}$$
$$P={1\over r-1}-{5\over r}+{6\over rD},\qquad
Q_h={3\over r^2(r-1)}-{ar\over r-1}-{3\over r^2(r-1)D},$$

with the **explicit** source

$$\begin{split}
F={}&-2ar^2j_r-{4ar^2\over r-1}Q_{\theta\theta}
-{ar\over r-1}\tau+{2ar^2\over s}j_t'\\
&+{4ar^2(ar^3-2)\over sD}Q_{tr}
-{2ar[ar^3(2r-3)-4r+3]\over s(r-1)D}j_t .
\end{split} \tag{3}$$

There is no source-only TT replacement. Introduce

$$b={2r^2\over D}(j_t-rQ_{tr}),\qquad
p_T={f\over s}\{fb'+(f'+2f/r)b-j_t-s\tau\}
             -{f^2\over aD}F. \tag{4}$$

The reconstructed tensor is

$$B=-{s\over aD}\left(y'-{y\over2r(r-1)}\right)+b,$$
$$S=-{r^2\over2D}y-{r-1\over aD}y'+{r-1\over s}b,$$
$$T=\left[{(r-1)(1/2-ar^3)\over rD^2}-{s^2\over aD}\right]y
 +\left[{r-1\over2ar^3D}-{3(r-1)^2\over D^2}\right]y'+p_T,$$
$$\boxed{C={T\over f^2}+{\tau\over f}-{2S\over r(r-1)}.} \tag{5}$$

At Q=0 these are exactly the historical homogeneous reconstruction with
k→K, Omega→s. At K≠0,s≠0,r>1 all denominators are nonzero for real K:
D>0, so no interior reconstruction pole is introduced. The divisions by s
make this chart unsuitable at the static threshold; the n=0 and s=0
problems require their own variables, not a claimed uniform limit of (1).

## 3. Derivation and equivalence with all seven equations

Trace and time divergence give, without using a homogeneous equation,

$$C=T/f^2+\tau/f-2S/(fr^2),\quad
T={f\over s}\{fB'+(f'+2f/r)B-j_t-s\tau\}.$$

Use (1) to eliminate S. The tr Ricci equation then becomes algebraic in B
and yields the first line of (5) including b. Let

$$d=\bar\nabla^\mu q_{\mu r}-\tau'-j_r
=-sB/f+fC'+(3f'/2+2f/r)C+f'T/(2f^2)-2S/r^3-\tau'-j_r.$$

A direct component calculation yields

$$E_{\theta\theta}-{r-1\over2}d
={r-1\over4ar^2}\{y''+Py'+(Q_h-s^2/f^2)y-F\}, \tag{6}$$

where E=R'[q]+Q. Before imposing (2), T contains y''; eliminating that
one derivative using (2) gives precisely pT in (4). The source lift uses
only Q and first radial derivatives of Q; differentiating the metric
requires additional source regularity.

For sufficiency, reconstruction already enforces Ezz=Etz=Etr=0.
Equation (2) enforces Etheta theta=(r−1)d/2, while Erz=iK d/2.
Set e=gbar^AB E_AB. The contracted residual Bianchi identities are
∇^A E_AB−∂B e/2=0. The t identity, since s≠0, gives
Ett=−fe/2. The z identity gives e=f d'+(f'+2f/r)d. The trace gives
Err=e/(2f)−d/r. Substituting into the r identity leaves

$$0=-\tfrac12(a+r^{-3})d=-{D\over2r^3}d. \tag{7}$$

Hence d=e=0 and every remaining Ricci equation vanishes. This uses all
three Bianchi identities and the full Einstein source. Necessity follows
by reversing the algebra. Thus (1)–(5) give a bijection between master
solutions and the full sourced SO(3) unitary sector for these nonexceptional
parameters, with homogeneous physical data supplied by the two master
integration constants. No independent missing constraint kernel is hidden.
Boundary regularity is an additional admissibility condition, not a
consequence of merely bounded static-coordinate master values.

The primary executable derives the orbit/sphere linear Ricci tensor from
Christoffels and checks (2)–(7) for **four arbitrary radial functions** by
manufacturing Q=−R'[q]. The proof uses generic K,s, not the old imaginary-k
specialization. A distinct auditor constructs arbitrary EF tensors and
checks the master, all reconstruction components and source Bianchi. The
on-shell physical source is supplied by the tensor functional in section 1;
source conservation follows exactly from the full linear Einstein equations.
Checks do not assert that arbitrary manufactured q is a physical GL mode.

## 4. Regular master, scalar inverse and endpoint problem

Put y=(D/r)u. Equation (2) is equivalent to

$$(H_K+s^2)u=\mathfrak F,\quad
H_K=-\partial_x^2+V_K,\quad \mathfrak F=-{f^2r\over D}F,$$
$$V_K=af+{(6r-11)(r-1)\over r^4}
 +{18(r-1)^2\over r^4D^2}-{6(4r-5)(r-1)\over r^4D}.$$

The source sign is verified off shell. The independent note
[master/inverse analysis](goal003_master_independent.md) proves H_K≥0
for a≥3/2 by an explicit positive supersolution. L10 has
K²=(2pi/5)²>3/2. Thus ||(H_K+s²)^−1||L²≤s^−2 and no compatible
growing kernel exists for any s>0. This is a genuine analytical
nonresonance result, not an inference from the numerical GL threshold.
It is a Laplace-point bound; arbitrary retarded data require the separate
energy/propagator estimates and their reconstruction/gauge completion.

In EF coordinates write y=e^{sx}Y and transform the source as

$$j_t=e^{sx}\widehat j_v,\quad
j_r=e^{sx}(\widehat j_r+\widehat j_v/f),\quad
Q_{tr}=e^{sx}(\widehat Q_{vr}+\widehat Q_{vv}/f),$$
$$\tau=e^{sx}\widehat\tau,\qquad
Q_{\theta\theta}=e^{sx}\widehat Q_{\theta\theta}.$$

The combination J=f e^−sx F is smooth for a smooth EF source: the
apparently divergent jt derivative and jr terms cancel. The regular
master is

$$fY''+(2s+fP)Y'+
\left[{s(-4+6/D)\over r}+{3\over r^3}-a-{3\over r^3D}\right]Y=J. \tag{8}$$

At r=1 the derivative coefficient is 2s+1. The ingoing Frobenius
recurrence has no positive-integer resonances for s>0. The excluded branch
is delta^−2s in Y; a finite arbitrary Y not satisfying (8)'s boundary
relation can still give a singular reconstructed tensor. In EF the four
components are e^−sx(T, B−T/f, C−2B/f+T/f², S). Their pole numerators
must cancel, with the source identities included. The independent note
makes the regular-source norm and the reconstruction estimates explicit.

At infinity alpha=sqrt(K²+s²)=2sqrt(k²+Omega²) coincides with the squared
GL exponent. Both parts of the variation-of-constants solution must be
kept. One cannot define a constant 'outgoing overlap' by extending a
divergent isolated growing-basis integral and discard the compensating
particular tail. For a source term e^−alpha r r^p, the response power is
generically p+1, with a logarithm if p=−beta2−1,
beta2=(K²+2s²)/(2alpha). This local resonance is not a spectral pole.
The actual quadratic-source tail, rather than this generic rule alone,
is assessed in the verification record.

### Actual quadratic tail and the absence of a formal logarithmic resonance

Let \(q_\infty=\sqrt{k^2+\Omega^2}\),
\(\beta_1=(k^2+2\Omega^2)/(2q_\infty)\), and let the decaying exact
first-order solution have the differentiated asymptotic expansion
\(u_1=c_\infty e^{-q_\infty r}r^{-\beta_1}(1+O(r^{-1}))\).
The local Lorentz-frame leading tensor, divided by u1/r, is

$$H_{tt}=-q_\infty^2/k^2,\quad H_{tr}=\Omega q_\infty/k^2,
\quad H_{rr}=-\Omega^2/k^2,\quad
H_{\hat\theta\hat\theta}=H_{\hat\varphi\hat\varphi}=-1/2.$$

Thus its Lorentz contraction is \(3u_1^2/(2r^2)\); the phase covector
\(p=(\Omega,-q_\infty,ik)\) is null and transverse. The quadratic
connection gives, for the complex +2 coefficient,

$$Q_{AB,+2}={3\over16}p_Ap_B H_{\mu\nu}H^{\mu\nu}
 +O(u_1^2r^{-3})\quad(A,B\in\{t,r,z\}).$$

The angular leading constant cancels. Expanding the independent exact
sphere equation \(R_{\theta\theta}=1-\Box_3 W/2\), including its
metric determinant density at second order, gives

$$Q_{\theta\theta,+2}=-{q_\infty\over4r}u_1^2+O(u_1^2r^{-2}).$$

Audit I derives this coefficient independently for generic
q_infty>Omega>0, with k²=q_infty²−Omega², before inspecting the primary
rational source result. Substitution into (3) shows that its jr and jt'
leading terms cancel, its Qtr term is −9a q_infty u1²/16, and its
angular term is +a q_infty u1². Consequently

$$\boxed{\mathfrak F=-{7q_\infty\over16r^2}u_1^2
                  +O(u_1^2r^{-3}).} \tag{8a}$$

This is an actual source coefficient, not the generic tail rule alone.
It is nonzero for a nontrivial decaying mode. Since alpha=2q_infty and
beta2=2beta1, a canonical particular tail with zero leading homogeneous
coefficient is

$$u_{2,p}={7c_\infty^2\over64}
 e^{-\alpha r}r^{-\beta_2-1}(1+O(r^{-1})). \tag{8b}$$

The full decaying solution adds C_I u_I; its constant C_I is fixed by
the regular-horizon inverse, not by this asymptotic calculation. In fact
this actual GL tail gives \(u_H\mathfrak F=O(r^{-2})\), so its full
overlap converges and, with the normalized decaying uI,

$$C_I=-W^{-1}\int_{-\infty}^{\infty}u_H(x)\mathfrak F(x)\,dx.$$

The horizon end converges by the compatible EF source behavior. The
remaining two finite tail integrals generate (8b). Thus the earlier
warning about divergent isolated overlaps is a warning for generic
sources, not an assertion that the actual GL overlap diverges. At every
formal inverse-power step j≥1 the coefficient is −2alpha j, so the
actual quadratic source has no logarithmic resonance in this formal
expansion. A source-free outer condition misses (8b), even though there
is no global spectral pole. The primary exact rational control at
k²=9/25, Omega=4/5 gives
\(\mathfrak F/u_1^2=-7/(16r^2)-20173/(7200r^3)+O(r^{-4})\).
It is a local positive-k control, not a physical eigenmode or L10 sample.
Its raw output also prints an r^-4 term for a truncated logarithmic-
derivative jet; that coefficient is not used as a full asymptotic result.

The O symbols here refer to the differentiated decaying asymptotic class.
They do not supply a numerical remainder constant or radius for the
normalized L10 mode. Those require a bounded GL endpoint remainder and
its source substitution. For any global smooth approximate scalar response
with the correct endpoint class, an actually bounded residual
\(\mathcal R=(H_K+s^2)u_{app}-\mathfrak F\) gives the rigorous error
\(\|u-u_{app}\|_2\le s^{-2}\|\mathcal R\|_2\), followed by the
regular derivative/reconstruction estimates with the residual derivatives.
No such physical residual enclosure is asserted here. Effective weak
endpoint envelopes are already proven in the inverse note; a fully
source-evaluated L10 response with sharp endpoint remainders remains open.

## 5. Error and completion boundary

The source functional and reconstruction are explicit, local and
constraint preserving. The scalar nonresonance bound is analytic for the
entire a≥3/2 range. Neither result turns the empirical L10 mode, C0 or
its derivatives into certified inputs. Source errors depend on H and its
needed derivatives, endpoint remainder and normalization; for a bounded
bilinear source B(H,H), ||δQ||≤||B||(2||H||||δH||+||δH||²), with the
same derivative/weight spaces. The operator coefficient error needs a
resolvent perturbation bound, not a local residual alone.

The quantitative full-geometry/data/radiation status is stated separately
in the report: an algebraic master, a scalar inverse, an effective regular
metric bound, a certified physical-input M2 and a nonlinear radiation
remainder are distinct claims. A failed bound at a later level does not
undo equations (1)–(8), and these equations do not authorize an A~1 or
first-bulge extrapolation.

## 6. Complete quadratic geometry as an explicit conditional estimate

For the +2 sector use the source and regular-metric norms and effective
constant constructed in the independent note, not a scalar-only norm.
Here Y_m already includes m+3 source derivatives, as defined there.
On a fixed tau interval, the exponential particular metric obeys

$$\|q_{2,2}\|_{X_m([0,T])}\le e^{sT}C_{2,m}(a,s)\|Q_{2,2}\|_{Y_m}.$$

Complex +2 and −2 reconstruct a real tensor with an upper-bound factor
two. Independent homogeneous second-harmonic Cauchy data add their own
regular evolution norm. The point-resolvent estimate does not bound those
data, and setting them to zero is a declared family choice, not a source
identity. The affine trace/divergence are part of C2,m.

At n=0 let chi=qzz, phi2=sqrt(3)chi/4 and
pE=qmu nu+chi gbar/2. In EF coordinates, the exact quadratic source is

$$\mathcal T_{\mu\nu}=-Q_{\mu\nu,0}
 +\tfrac12\bar g_{\mu\nu}(Q^\alpha{}_{\alpha,0}+Q_{zz,0}).$$

Its conserved-source quadratures at fixed advanced time are

$$\beta=-\tfrac12\int_r^\infty R\mathcal T_{rr}dR,\qquad
\mu=\delta m+\tfrac12\int_r^\infty R^2\mathcal T^v{}_v dR.$$

For the separated source, strip e^{sv}; then for r≥1

$$|\widehat\beta(r)|\le\tfrac12\int_r^\infty R|\widehat{\mathcal T}_{rr}|dR,
\quad |\widehat\mu(r)|\le\tfrac12\int_r^\infty R^2
|\widehat{\mathcal T}^{v}{}_{v}|dR$$

with fixed quadratic ADM shift delta m=0. Derivatives j≥1 are bounded
by one half the absolute value of the (j−1)st derivative of the respective
integrand; conversion to tau adds e^{sr} and its derivatives. For an
explicit envelope C R^p exp(−lambda R), each integral is the finite
incomplete-gamma expression C lambda^(−p−1) Gamma(p+1,lambda r),
with the extra R or R² absorbed into p. These are effective bounds,
not a repeated assertion that a radial constraint alone solves Einstein.
Source conservation and boundary flux enforce mu_v=r² T^r_v/2 and

$$r(r-1)\beta_{rr}+r^2\beta_{rv}+(r+1/2)\beta_r-r\mu_{rr}
=\mathcal T_{\theta\theta};$$

the independent source/Bianchi checks verify their compatibility. The
remaining scalar response is the previously audited Green problem with
V0=f/r³, strictly positive Laplace shift s², plus specified homogeneous
scalar data. Its regular derivatives require the corresponding source
and initial-data derivatives, not merely its degenerate Killing energy.
There is no independent arbitrary regular static tension hair at fixed L.

In particular, if C0,m denotes the explicit scalar-response/quadrature
bound in the same regular norm, a full bound has the structure

$$M_2(T)\le e^{sT}
 [C_{0,m}\|Q_{2,0}\|_{Y_0}+2C_{2,m}\|Q_{2,2}\|_{Y_m}]
 +\|q_{2,hom}\|_{X_m([0,T])}+C_{charge}|\delta m|. \tag{9}$$

Every term is needed. This is a conditional complete-geometry estimate;
the actual L10 weighted source/input derivative norms, scalar high-order
constant, and chosen family homogeneous-data norm have not been enclosed.
Thus (9) is not an evaluated certified M2. The source uses the unchanged
L10 profile only as historical empirical input, not an interval oracle.

For a fixed operator, source uncertainty propagates as
C2,m||δQ|| in this norm. If a bounded perturbation of the master operator
has ||R δH||<1, the Neumann identity gives
||δR||≤||R||²||δH||/(1−||R δH||); changes in s, endpoint conventions
and reconstruction add their own coefficient errors. For unbounded
coefficient/derivative perturbations, use a relative graph/form estimate
instead; a pointwise residual cannot justify this formula. Time-dependent
mode errors include exp(|δOmega|T)−1. None of these open input or transient
errors is silently spent as zero in a news budget.
