# GOAL-003: independent scalar resolvent and endpoint analysis

2026-10-22. Investigator/auditor `forced_master`, separate from the primary
component-elimination derivation. Input research/sav-radiation@
`ef646d56ab7d8bebca578afab53e62da5146ec05`; conventions are those of
NOTATION.md, with dimensionless r0=1 below. This is analytical perturbation
work, not a spacetime simulation. The homogeneous operator used here is
the independently checked operator in `gl_linear_mode.md`. Its use for
the *forced* tensor requires the source-aware equivalence proved in the
primary note; this note never imposes homogeneous TT on a forced tensor.

## 1. A positive supersolution at the actual second harmonic

Let a=K², D=1+a r³, f=(r−1)/r, x=r+log(r−1), and

\[
H_a=-\partial_x^2+V_a,\qquad
V_a=af+\frac{(6r-11)(r-1)}{r^4}
+\frac{18(r-1)^2}{r^4D^2}
-\frac{6(4r-5)(r-1)}{r^4D}.
\]

The elementary positive function

\[
w(r)=e^{-r/2}\frac rD
\]

satisfies the exact identity

\[
\frac{H_aw}{w}=\frac{f\,P_a(r)}{4r^2D},\qquad
P_a=(4a^2-a)r^5-7ar^4+10ar^3-(8a+1)r^2+5r-2.
\tag{1}
\]

Writing r=1+d, the coefficients in increasing powers of d are

\[
4a^2-6a+2,\quad20a^2-19a+3,\quad40a^2-30a-1,
\quad40a^2-28a,\quad20a^2-12a,\quad4a^2-a.
\tag{2}
\]

Every coefficient and its derivative in a are positive at a=3/2;
all are upward quadratics. Therefore (1) is strictly positive for
r>1 and a≥3/2. At L/r0=10, K=2π/5 and a=4π²/25>3/2, with
no numerical GL-threshold or growth-rate input used. This is a sufficient
stability region, not a claim that 3/2 is the exact threshold.

Define the bounded deformation

\[
S=-\partial_x\log w
=f\left(\frac12-\frac1r+\frac{3ar^2}{D}\right)
=f\left(\frac12+\frac{2ar^3-1}{rD}\right),
\qquad 0<S<\frac52.
\]

For compactly supported smooth u,

\[
\int(|u_x|^2+V_a|u|^2)dx
=\int|u_x+Su|^2dx+\int\frac{H_aw}{w}|u|^2dx.
\tag{3}
\]

This identity extends to the closed H¹ form since V and S are bounded.
The real Schrödinger operator with domain H²(R) is self-adjoint, bounded
below, and (3) proves H_a≥0. For every **real** s>0,

\[
\|(H_a+s^2)^{-1}\|_{L^2\to L^2}\le s^{-2},\qquad
\|u_x+Su\|_2\le s^{-1}\|F\|_2,
\]
\[
\|u_x\|_2\le(s^{-1}+\tfrac52s^{-2})\|F\|_2,
\quad
\|u\|_\infty\le
\sqrt{2s^{-2}(s^{-1}+\tfrac52s^{-2})}\,\|F\|_2.
\tag{4}
\]

Here (H_a+s²)u=F, and the last inequality uses the deliberately loose
one-dimensional Sobolev bound. A nonzero mode satisfying ingoing/decaying
Laplace endpoint conditions is in L² for s>0, so it cannot lie in the
kernel. This proves nonresonance at the L10 second harmonic independently
of the empirical GL threshold. The numerical s from L10 may illustrate
(4), but an evaluated interval constant at the physical eigenvalue still
needs its enclosure. More generally, the spectral theorem gives
\(\|(H_a+z^2)^{-1}\|\le\operatorname{dist}(-z^2,[0,\infty))^{-1}\)
off the nonnegative spectrum. This is not a uniform bound on its cut.

The failed simpler estimate must be retained: V_a is **negative** just
outside r=1 at a=3/2, because V_a/(r−1) tends to
a−5+6/(1+a)=−11/10. Thus the assertion V≥0 is false. The first trial
w=r/D yields H_aw/(fw)=a(ar³−2)/D and only proves positivity for
a≥2; it does not cover L10. The factor e^{-r/2} is the distinct successful
repair. No parameter scan or boundary-value solve is used.

## 2. Correct Green representation, beyond a formal Wronskian test

Let u_H be the positive homogeneous solution with u_H~e^{sx} as x→−∞,
and u_I the positive decaying solution at +∞. Positivity follows by
disconjugacy: a first zero with the decaying endpoint would contradict
the positive quadratic form on the corresponding half-line. Their
Wronskian W=u_H u_I'−u_H' u_I is constant and strictly negative, since
the Green kernel of a strictly positive one-dimensional scalar operator
is positive and its derivative jump is −1. Equivalently its sign follows
from the ordered logarithmic derivatives of the two positive solutions.

\[
u(x)=-\frac{u_I(x)}W\int_{-\infty}^x u_H(y)F(y)dy
-\frac{u_H(x)}W\int_x^\infty u_I(y)F(y)dy .                 \tag{5}
\]

Equation (4) supplies an actual bounded inverse for F∈L², so (5) is no
longer justified merely by a floating nonzero W. Homogeneous endpoint
data must still be added when specifying a different Cauchy or boundary
problem. A separated growth-point inverse is not itself a retarded
metric propagator for arbitrary data. In particular H_a has no positive
spectral gap: the horizon potential tends to zero, yielding a continuum
down to zero. For the scalar master wave equation its conserved energy
uses (3), and Duhamel implies a useful energy estimate, but that energy
alone does not control the physical tensor in a horizon-regular norm.

## 3. Effective collar and exterior bounds for the master

For a compatible smooth future-EF source assume
F=e^{sx}F_H with |F_H|≤C_F d on 0<d≤d_c≤1. Write u=e^{sx}v.
The regular solution is represented by

\[
v(x)=c+\frac1{2s}\int_{-\infty}^x
(1-e^{-2s(x-y)})(V_a(y)v(y)-F_H(y))dy .                    \tag{6}
\]

Let C_V bound |V_a|/d on the collar, and
M=C_V(d_c+d_c²/2)/(2s). A sufficient explicit, conservative choice is
C_V=a+47 and d_c=min(1/12,s/[4(a+47)]), giving M<1/4.
Gronwall yields |v_H−1|≤e^M−1 for the homogeneous solution with c=1,
and |v_P|≤C_F(d_c+d_c²/2)e^M/(2s) for c=0.
Consequently v_H≥2−e^M>0 and

\[
|c|\le
\frac{e^{-sx_c}\|u\|_\infty+
C_F(d_c+d_c²/2)e^M/(2s)}{2-e^M}.                         \tag{7}
\]

Equations (4), (6), and (7) bound the **regular master**, including its
otherwise invisible horizon coefficient, using source norms and explicit
constants. Differentiating (6) gives v_x=O(d); higher regular derivatives
require the corresponding regular source derivatives. Frobenius
recurrences have denominators j(j+2s), j≥1, so there are no logarithms
on the ingoing branch for analytic compatible forcing. The excluded
homogeneous branch e^{-sx} is singular in future EF coordinates.

For r≥4 put σ=√(s²+a/4) and h=e^{-σ(r−4)}. Since
V_a≥a(1−1/r)−24/(a r⁵),

\[
(H_a+s²)h/h=V_a+s²-σ²f²+σff'\ge a/3\quad(a\ge3/2).
\]

Writing F_∞=sup_{r≥4}e^{σ(r−4)}|F|, exterior comparison therefore gives

\[
|u(r)|\le (|u(4)|+3F_\infty/a)e^{-σ(r-4)}.               \tag{8}
\]

Here s²−σ²f²=s²(1−f²)−af²/4≥−a/4; the lower bound is valid for
every s>0. Also s<σ<α, so the chosen source weight is strictly below the
physical forced exponential. There is no unproved numerical endpoint
remainder in these inequalities.
On a middle compact interval, (4) and u_xx=(V_a+s²)u−F give derivative
bounds; higher derivatives follow by differentiating this equation with
the required source regularity. The weak weight in (8) deliberately does
not pretend that a resonant forcing tail has the homogeneous asymptotic
power. These estimates still need conversion through the actual forced
reconstruction to provide the Goal's full metric X←Y norm.

## 4. Equal exponents at infinity: the necessary tail calculation

With α=√(a+s²), β=(a+2s²)/(2α), a direct substitution gives

\[
(H_a+s²)[e^{-αr}r^ν]
=2α(ν+β)e^{-αr}r^{ν-1}[1+O(r^{-1})]
\tag{9}
\]

when ν+β≠0. Thus for F~F_0 e^{-αr}r^p the leading particular term is
F_0 e^{-αr}r^{p+1}/[2α(p+1+β)]. If p=−β−1, replace that term by
F_0 e^{-αr}r^{-β}log r/(2α). Further terms follow by substitution;
there may be additional logarithms at resonant powers. Equal exponential
rates alone do not establish a spectral pole: (3) excludes such a pole
here, whereas (9) explains local asymptotic resonance.

One must insert the **actual derived F[H1]**, including trace/divergence
and gauge terms, before assigning p or F0. A divergent isolated overlap
\(\int^{\infty}u_HF\) cannot be substituted for (5): the growing integral
is multiplied by the decaying u_I at finite x, and its tail may be exactly
the required polynomial/logarithmic particular solution. Equation (8)
remains valid for such a source. It does not determine its leading
coefficient or justify omitting the forced tail from outer data.

### 4a. Actual quadratic GL tail: an independent leading coefficient

This bounded final calculation uses the actual first-harmonic GL
reconstruction, not a generic equal-exponent manufactured source. Let
b=k², q=√(b+Ω²)>Ω>0, β₁=(b+2Ω²)/(2q), and take the decaying first-order
master u₁=c₁e^{−qr}r^{−β₁}(1+O(r⁻¹)). The remainder hypothesis here is a
differentiable GL asymptotic expansion: after removing e^{−qr}r^{−β₁},
the expansion through 1/r has an O(r⁻²) remainder whose j-th radial
derivative is O(r⁻²⁻ʲ), 0≤j≤4. The on-shell reconstruction and ODE then
give the differentiated metric remainders used below. This hypothesis
does **not** supply a certified remainder constant or matching radius
for the saved physical L10 input.

Writing the static first-order metric as (T,B,C,Sθ), the required terms,
after factoring the exact u₁, are

\[
\frac T{u_1}=-\frac{q^2}{br}+\frac{1+3q/b}{r^2}+O(r^{-3}),\quad
\frac B{u_1}=\frac{\Omega q}{br}-\frac{\Omega(2-\beta_1)}{br^2}
+O(r^{-3}),
\]
\[
\frac C{u_1}=-\frac{\Omega^2}{br}+\frac{q-2\Omega^2}{br^2}
+O(r^{-3}),\quad
\frac{S_\theta}{u_1}=-\frac r2+\frac qb+O(r^{-1}),\quad
\frac{u_1'}{u_1}=-q-\frac{\beta_1}r+O(r^{-2}).             \tag{9a}
\]

The leading orthonormal polarization is transverse to the complex null
covector p=(Ω,−q,0,0,ik), and H_AB H^AB=3u₁²/(2r²). Direct quadratic
connection algebra for one complex plane wave gives
Q_AB=(3/4)p_Ap_B H_CD H^CD: the derivative of the second-order connection
contributes 1 and the connection product subtracts 1/4. The real-cosine
+2 harmonic carries another factor 1/4. Consequently

\[
Q_{tr,+2}=-\frac{9\Omega q}{32}\frac{u_1^2}{r^2}
+O(u_1^2r^{-3}),\quad
j_t=-\frac{9\Omega}{32}\frac{u_1^2}{r^2}+O(u_1^2r^{-3}),
\]
\[
j_r=\frac{9q}{32}\frac{u_1^2}{r^2}+O(u_1^2r^{-3}),\qquad
\tau=\frac9{64}\frac{u_1^2}{r^2}+O(u_1^2r^{-3}).          \tag{9b}
\]

The plane-wave cancellation alone is insufficient for Qθθ, whose first
nonzero term is needed. Independently expand the orbit determinant
density with W=r²+εSθ, g₃=g₃⁽⁰⁾+εh, and

\[
d_1=\tfrac12\operatorname{tr}(g_3^{-1}h),\quad
d_2=\tfrac18[\operatorname{tr}(g_3^{-1}h)]^2
-\tfrac14\operatorname{tr}[(g_3^{-1}h)^2],
\]
\[
M_1^{ab}=d_1g_3^{ab}-h^{ab},\qquad
M_2^{ab}=d_2g_3^{ab}-d_1h^{ab}+h^a{}_ch^{cb}.
\]

Since Rθθ=1−□₃W/2, the background angular equation and the on-shell
linear angular equation imply the exact quadratic identity

\[
Q_{\theta\theta}^{\rm complex}
=d_2-\frac12\partial_a
\left(M_1^{ab}\partial_b S_\theta+2rM_2^{ar}\right).
\]

Substituting (9a), the coefficient of u₁²r⁰ cancels and that of u₁²/r
is −q. Thus the actual real-cosine source satisfies

\[
Q_{\theta\theta,+2}=-\frac q4\frac{u_1^2}{r}
+O(u_1^2r^{-2}).                                        \tag{9c}
\]

This is checked symbolically for generic q>Ω>0, before comparison with
the primary source generator. For a₂=4b, s₂=2Ω, substitution into the
forced master shows cancellation of the leading j_r and j_t′ terms.
The angular term contributes 4bq u₁² and the Qtr term −9bq u₁²/4;
the other terms are lower order. With the primary sign convention,

\[
\frac{F_y}{u_1^2}=\frac{7bq}{4}+O(r^{-1}),\qquad
\frac{\mathfrak F}{u_1^2}
=-\frac{f^2r}{1+4br^3}\frac{F_y}{u_1^2}
=-\frac{7q}{16r^2}+O(r^{-3}).                            \tag{9d}
\]

Here α₂=2q and β₂=2β₁. The actual source starts at the power
−β₂−2, below the resonant power −β₂−1 in (9). A particular solution
normalized to have no leading decaying homogeneous term therefore has

\[
u_{2,P}=\frac7{64}c_1^2e^{-2qr}r^{-\beta_2-1}
\bigl(1+O(r^{-1})\bigr),\qquad
u_2=c_2u_I+u_{2,P},\quad
u_I=e^{-2qr}r^{-\beta_2}(1+O(r^{-1})).                    \tag{9e}
\]

The coefficient c₂ is fixed by the global source/endpoint problem;
it is not zero by the local asymptotic calculation. Its homogeneous
series also contributes to the total subleading coefficients. For a
pure inverse-power GL source expansion, subsequent particular recurrence
denominators are −2α₂j, j≥1, so no logarithm is forced. The actual
growing-basis overlap has u_H mathfrak F=O(r⁻²) and converges; the
generic divergent-overlap warning above remains relevant to other
sources, but does not describe this leading GL tail.

The separate tail-only execution passed six exact checks, including
the wrong case omitting (9c), which changes the answer to +9q/16.
Only after this derivation was the primary positive-k source diagnostic
read. Its supported coefficient −7/16 at q=1 agrees with (9d).
Its printed r⁻⁴ coefficient used a truncated logarithmic-derivative jet
and is unsupported: the omitted O(r⁻³) derivative term can enter that
coefficient. The primary raw output is preserved as diagnostic evidence;
this audit does not promote that printed coefficient to a result.
No physical ODE was rerun. These formal coefficients close the leading
tail algebra, while the physical L10 source/remainder enclosure and a
quantitative endpoint radius/constant remain open. They are not a
massive-radiation flux calculation.

## 5. Source-aware EF reconstruction and a constructive collar norm

The primary investigator subsequently supplied the explicit forced master
and reconstruction. This section independently simplifies that map in EF
coordinates and checks the potentially singular term. Write y=e^{sx}Y,
and remove the common e^{sv+iKz} from EF source components. Denote these
components by A=Qvv, B=Qvr, C=Qrr, QΩ=Qθθ, and let τ, j, j_r be the
regular EF trace/divergence data, so Qzz=−aτ/2,
Qvz=−iK j/2, Qrz=−iK j_r/2. These letters denote **source** components
in this subsection, not the physical GL amplitude.

With the primary source sign R'[q]=−Q, define

\[
Z=-\frac{s}{aD}\left[fY'+(s-\tfrac1{2r^2})Y\right]
+\frac{2r^2}{D}(f j-rfB-rA).
\tag{10}
\]

The entire tensor reconstructs as

\[
\widehat q_{\theta\theta}=\frac{rZ}{s}-\frac{Y}{2ar},\quad
\widehat q_{vr}=-\frac{Z'}s-\frac{2Z}{sr}+\frac js+\tau,
\quad \widehat q_{vv}=Z-f\widehat q_{vr},
\tag{11}
\]
\[
\widehat q_{rr}=\frac Nf,\qquad
N=\frac{2Z'}s+\frac{2Z}{sr}-\frac{2j}s-\tau+\frac{Y}{ar^3}.
\tag{12}
\]

Thus there is only one apparent simple horizon pole, not a genuine
double-pole norm loss. The regular master equation is

\[
fY''+bY'+cY=J,\quad b=2s+fP,
\quad c=\frac{s(-4+6/D)}r+\frac3{r^3}-a-\frac3{r^3D},
\tag{13}
\]
\[
J=-2ar^2f j_r-4arQ_\Omega-a\tau+
\frac{2ar^2f}{s}j'
+\frac{4ar^2(ar^3-2)}{sD}(fB+A)
-\frac{2a[ar^3(2r-3)-4r+3]}{sD}j.
\tag{14}
\]

At r=1, b=2s+1 and Y'=(J−cY)/(2s+1). Direct substitution into (12)
gives

\[
N(1)=-\frac{4}{s(a+1)}
\left[A'+B+2A+\frac{sa}{4}\tau-sQ_\Omega+\frac a2j\right]_{r=1}=0.
\tag{15}
\]

The bracket is exactly the v-component of conservation of
Q_AB−g_AB tr(Q)/2 at the horizon. It is **not** conservation of Ricci Q.
This proves smooth reconstruction for smooth compatible source and a
smooth regular Y. The absence of further hidden tensor endpoint data
uses the primary all-seven-equation equivalence, independently checked
below. An arbitrary finite master without (13) fails (15).

Here is an effective derivative estimate, without a hidden radial inverse.
On the chosen collar b≥2s+1/2 and f'>0. Let B_n bound ||Y^(n)||∞,
J_n bound ||J^(n)||∞, and F_j, b_j, c_j bound the derivatives of the
explicit rational coefficients f,b,c. The base B_0 follows from (4),
(6), (7) and Y=(D/r)v. Differentiating (13) n times and using the regular
solution of the first-order equation for Y^(n+1) gives

\[
B_{n+1}\le\frac{1}{2s+1/2}\left[
J_n+\sum_{j=2}^n {n\choose j}F_j B_{n+2-j}
+\sum_{j=1}^n {n\choose j}b_j B_{n+1-j}
+\sum_{j=0}^n {n\choose j}c_j B_{n-j}\right].             \tag{16}
\]

The positive coefficient is b+n f'; there is no unspecified elliptic
constant in (16). Regularity at d=0 excludes its divergent homogeneous
solution. All coefficient bounds are effective rational bounds on
1≤r≤1+d_c, D≥1+a: expand the finite derivative numerator polynomial,
sum the absolute monomial bounds, and use the displayed positive
denominator lower bounds. No eigenvalue calculation is needed.

Finally N(1)=0 implies the exact divided-difference identity

\[
\frac{N(r)}{r-1}=\int_0^1 N'(1+t(r-1))dt,
\quad
\left\|\partial_r^m\frac{N}{r-1}\right\|_\infty
\le\frac{\|N^{(m+1)}\|_\infty}{m+1}.                    \tag{17}
\]

Apply Leibniz to qrr=rN/(r−1) and (10)–(11). The C^m regular tensor
therefore needs Y through m+3 and the source components through m+2;
using (14),(16), a conservative **m+3 source-derivative budget** suffices.
The coefficient map (10)–(17) is a constructive finite collar constant.
Regular tetrad conversion has bounded coefficients there. On a τ=v−r
slice multiply the EF coefficient by e^{sr}; its derivatives must be
included, so the bound explicitly depends on s and the chosen clock.

For definiteness, define dimensionless Y_m as the sum of:

1. ||mathfrak F||L²(dx), where mathfrak F=−f²rF/D;
2. all C^{m+3} sup norms of the seven **regular EF** source components
   on 1≤r≤4, with the e^{sv+iKz} factor removed;
3. all sup norms of e^{σ(r−4)} ∂r^j Q_AB in **static** coordinates on
   r≥4, and of e^{σ(r−4)} ∂r^j mathfrak F there, for 0≤j≤m+3.

This is a deliberately strong source norm; it involves only explicit
source differentiation and coordinate conversion, not an inverse. It
includes Qθθ itself, so no hidden angular-coordinate factor is dropped.
Compatibility is conservation of the Einstein combination and smooth
EF behavior. Define X_m as C^m sup norms of all regular tetrad components
on the slice τ=v−r=0 for 1≤r≤4, plus the corresponding norms multiplied
by e^{λ(r−4)} for r≥4, where λ=(σ−s)/2>0. The tetrad is the regular
one in goal002_second_order.md. Include radial derivatives up to m;
time/compact derivatives only add explicit factors s and K.

The middle interval is compact and contains no D or f zero; the primary
algebraic reconstruction and the differentiated master give an effective
constant by the same rational-coefficient bound. Exterior (8), the ODE
and source derivative weights supply the exterior estimate. A smaller
exponential output weight absorbs polynomial reconstruction coefficients.
Explicitly, use u_xx=(V+s²)u−mathfrak F, the unit-x Taylor bound
|u_x(x)|≤2 sup_[x,x+1]|u|+(1/2)sup_[x,x+1]|u_xx|, and its differentiated
version. Convert to r using f≥3/4. A factor r^d is bounded after reserving
any c>0 of the available exponential margin by
\(\sum_{j=0}^d {d\choose j}4^{d-j}(j/(ec))^j\), with the j=0 term
understood as 4^d. All rational denominators have stated positive lower
bounds. On the τ slice e^{sr}qhat=(r−1)^{-s}q_static, so there is no
unaccounted exponential growth from converting the exterior profile.

Equations (4), (7), (8), and (10)–(17), these finite coefficient bounds,
and Leibniz therefore specify a finite algorithmic constant C_alg(a,s,m)
such that ||q||X_m≤C_alg ||Q||Y_m. This is an **effective operator bound
with three-source-derivative loss**, not a claimed optimal constant.
The fixed profile has time growth e^{sτ}; on 0≤τ≤T multiply its bound
by e^{sT}. This point-resolvent construction does not provide a uniform
retarded estimate on general time-dependent sources or the continuum cut.

These steps do **not** evaluate
the physical quadratic source norm from the L10 profile, its input
uncertainty, or an optimal numerical C_inv. A reported physical M2 still
needs those bounds; writing down this norm algorithm does not supply M2.

## 6. What this contribution proves and does not prove

This establishes exact nonresonance and effective L²/H¹/master collar and
weak-exterior bounds for a≥3/2, real s>0, conditional only on the stated
scalar operator and source space. The successful coercivity mechanism is
new analytical work in this Goal; no novelty beyond this repository is
claimed. It improves the prior purely numerical threshold rationale.

The EF reconstruction (10)–(17) additionally closes the smooth-horizon
map and specifies an effective finite derivative-loss bound. Section 4a
also derives the actual leading forced infinity coefficient. Its
quantitative endpoint remainder and the physical source/input bounds
remain open. No evaluated complete physical q2 norm, exact GL data,
nonlinear theorem, finite amplitude or first-bulge radiation claim is
inferred from this note. The primary report must distinguish a bounded
operator construction from an enclosed physical source-to-metric number.

The primary source comparator inspected online was
[Collingbourne, §§3.2–3.4, 4.1 and Appendix E](https://arxiv.org/html/2007.08441).
Appendix E gives an a≥2 sufficient exclusion by another argument; it
does not supply the forced reconstruction or our scalar-to-metric bound.
Our formulas (1)–(9) are derived here, not copied from that reference.
Executed identities and deliberately wrong cases are recorded in
`../verification/goal003_audit_master_results.json` and
`../verification/AUDIT_GOAL003_MASTER.md`.
