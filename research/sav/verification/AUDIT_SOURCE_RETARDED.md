# Adversarial audit II: quadratic radion source and retarded projection

Date: 2026-10-22. Independent scientific reviewer: `audit_source` child
agent, working separately from the primary derivation and linear-mode solver.
Scope: exact quadratic scalar equation, gauge completion, independent cubic
action route, boundary/retarded prescription, and cancellation tests. No
physical GL ODE or numerical transfer coefficient was evaluated by this reviewer.

## Verdict and limitations

**PASS, with stated hypotheses:** the proposed scalar source
\[
 \Box_4\phi_0^{(2)}=-\frac{\sqrt3 k^2}{16}A(t)^2
 H_{\mu\nu}H^{\mu\nu},\qquad
 S_0=\frac{\sqrt3 k^2}{16}rf A(t)^2H_{\mu\nu}H^{\mu\nu}
\tag{1}
\]
is the complete quadratic source for the physical zero-mode radion in a
first-order unitary gauge, **provided the reconstructed first-order mode
satisfies every linear vacuum equation and is horizon regular and spatially
decaying**. The normalization is \(g=\bar g+\epsilon h+\epsilon^2q\),
with no factorial multiplying \(q\),
\(h_{\mu\nu}=A(t)H_{\mu\nu}(r)\cos kz\), and
\(\phi_0^{(2)}=\sqrt3\langle q_{zz}^{U}\rangle/4\).
The amplitude must still be normalized by an invariantly defined mode.

**PASS:** with \(W=y_Hy_\infty'-y_H'y_\infty\), the Green kernel has a
**minus sign**, \(-y_H(x_<)y_\infty(x_>)/W\), and the source-to-infinity
coefficient is (11) below.

**NOT CERTIFIED by this audit:** the physical overlap at any of the four
requested finite \(kr_0\), its numerical normalization/error, a numerical
amplitude window, or a radiated percentage. The contraction in (1) is
indefinite. A positive scalar Green kernel is insufficient to prove its
weighted integral nonzero. A manufactured exact cancellation is retained
below to expose this gap. This audit is a mathematical review of the
equations, not a substitute for a qualified first-order GL mode or second-order
metric reconstruction.

## 1. Gauge fixing is a physical completion, not a raw metric average

For each nonzero compact harmonic use the pullback convention
\[
 h^U=h+\mathcal L_{\xi_1}\bar g,\qquad
 q^U=q+\mathcal L_{\xi_1}h+
 \tfrac12\mathcal L_{\xi_1}^2\bar g+\mathcal L_{\xi_2}\bar g.
\tag{2}
\]
In the product background, lower-index gauge vectors satisfying
\[
 \xi_z=-\frac{h_{zz}}{2ik},\qquad
 \xi_\mu=-\frac{h_{\mu z}+\partial_\mu\xi_z}{ik}
\tag{3}
\]
set \(h^U_{zA}=0\). These are algebraic in \(1/k\) and use smooth background
covariant operations; in ingoing regular coordinates they preserve horizon
regularity for a smooth mode. The \(k=0\) sector is exceptional and cannot
be fixed by (3). Fixed asymptotic circle length excludes a large circle
rescaling. Gauge vectors must be periodic in \(z\), regular, and preserve the
specified asymptotic frame and decaying first-order sector.

Subsequent second-order transformations obey
\(\langle (\mathcal L_{\xi_2}\bar g)_{zz}\rangle
=2\langle\partial_z\xi_{2z}\rangle=0\). Residual first-order zero-mode
coordinate transformations do not change this scalar when \(h^U_{zA}=0\)
and the background \(g_{zz}=1\) is constant. Thus
\(\langle q_{zz}^{U}\rangle\) is a second-order gauge-invariant completion.
At this order \(\log g_{zz}^{U}=\epsilon^2q_{zz}^{U}\), so the given KK
ansatz fixes the factor \(\sqrt3/4\). At infinity the exponentially decaying
first-order completion terms cannot contribute a \(1/r\) scalar coefficient.
This does **not** justify using raw \(\langle g_{zz}\rangle\) at finite
radius in an arbitrary gauge.

The first-order \(zz\) and \(\mu z\) Einstein equations themselves give
\[
 \delta R_{zz}=\frac{k^2}{2}h^\mu{}_{\mu}=0,\quad
 \delta R_{\mu z}=\frac{ik}{2}
 (\nabla^\nu h_{\mu\nu}-\nabla_\mu h^\nu{}_{\nu})=0.
\tag{4}
\]
Consequently the nonzero-mode unitary metric is 4D transverse and traceless;
these are constraints, not extra assumptions that can be imposed on an
unqualified candidate. The remaining equation is the massive spin-2
Lichnerowicz equation with mass \(|k|\).

## 2. Direct connection expansion

For the first-order block metric \(ds^2=dz^2+g_{\mu\nu}(x,z)dx^\mu dx^\nu\),
\(\Gamma^\mu{}_{z\nu}=\frac12(g^{-1}g_{,z})^\mu{}_\nu\), so
\[
 R_{zz}=-\partial_z\Gamma^\mu{}_{z\mu}
       -\Gamma^\mu{}_{z\nu}\Gamma^\nu{}_{z\mu}
 =-\tfrac12\operatorname{tr}(g^{-1}g_{,zz})
  +\tfrac14\operatorname{tr}(g^{-1}g_{,z}g^{-1}g_{,z}).
\tag{5}
\]
Expansion and compact averaging give
\[
 \langle R_{zz}^{(2)}[h,h]\rangle
 =\tfrac12\langle\operatorname{tr}(h h_{,zz})\rangle
   +\tfrac14\langle\operatorname{tr}(h_{,z}h_{,z})\rangle
 =-\frac{k^2 A^2}{8}\operatorname{tr}(H^2).
\tag{6}
\]
Both \(\langle\cos^2\rangle=1/2\) and
\(\langle\sin^2\rangle=1/2\) matter. The linearized zero-mode equation
is \(\langle\delta R_{zz}[q]\rangle=-\Box_4\langle q_{zz}\rangle/2\),
independent of \(q_{\mu\nu}\) and \(q_{\mu z}\), since background
\(g_{zz}\) is constant and \(\partial_zq_0=0\). Combining (6) with it gives (1).

For a real SO(3) mode, write
\(H_{tt}=fa, H_{tr}=b,H_{rr}=c/f,H_{\theta\theta}=r^2d\),
\(H_{\varphi\varphi}=r^2\sin^2\theta\,d\). Then
\[
 \operatorname{tr}(H^2)=a^2+c^2+2d^2-2b^2,
 \qquad -a+c+2d=0.
\tag{7}
\]
Replacing the negative \(b^2\) term by a positive one is an invalid Euclidean
norm argument for the growing Lorentzian mode.

## 3. Independent cubic-action derivation

Use a spacelike \(z\)-ADM split with no shift through first order,
\(N=e^{2\phi/\sqrt3}\),
\(g_{\mu\nu}=e^{-2\phi/\sqrt3}\widetilde g_{\mu\nu}(x,z)\), and
\(\phi=\phi_0(x)=O(A^2)\). The Einstein-Hilbert bulk, after the usual
extrinsic-curvature boundary divergence, is
\[
 N\sqrt{-g}\,[R[g]+K^2-K_{\mu\nu}K^{\mu\nu}],\qquad
 K_{\mu\nu}=\frac{g_{\mu\nu,z}}{2N}.
\]
The 4D Weyl transformation gives
\(\sqrt{-\widetilde g}[R[\widetilde g]-2(\nabla\phi)^2]\),
plus \(2\sqrt3\sqrt{-\widetilde g}\,\widetilde\Box\phi\), an
\(x\)-boundary divergence. Dropping it is legitimate only with corresponding
boundary treatment or compactly supported variations. There is no additional
bulk linear \(\phi h^2\) term from the 4D curvature sector.

Because \(h\) is traceless, the only cubic bulk term relevant to its
zero-mode scalar source is
\[
 \mathcal L_z^{(2)}=-\tfrac14e^{-2\sqrt3\phi}
                   \operatorname{tr}(h_{,z}h_{,z}).
\tag{8}
\]
Varying \(-2(\partial\phi)^2+\mathcal L_z^{(2)}\) with respect to \(\phi\)
and keeping \(O(A^2)\) gives
\[
 4\Box\phi+\frac{\sqrt3}{2}
 \langle\operatorname{tr}(h_{,z}h_{,z})\rangle=0,
\]
which reproduces (1) including sign and factor. The Fourier average is not a
second independent factor to be inserted after (8) has already been averaged.
Boundary terms change the variational boundary data, not the bulk scalar
equation; source-to-infinity comparisons in another gauge must retain their
induced boundary contributions and field redefinition (2).

## 4. Why the other equations cannot add another scalar source

At second order the Einstein-frame metric is
\(q^E_{\mu\nu}=q^U_{\mu\nu}+\frac12q^U_{zz}\bar g_{\mu\nu}\).
The independent zero-mode radion equation follows from varying \(\phi\)
while holding this metric fixed. Remaining spherical Einstein equations
determine sourced mass/metric constraints and homogeneous charges. They do
not re-enter (1) because the background scalar vanishes. The quadratic
Einstein source is covariantly conserved on the first-order vacuum
equations, by the expanded Bianchi identity. This compatibility statement
fails for an arbitrary trial profile or time-switched metric that violates
the linear equations. This reviewer did not integrate the remaining
second-order metric equations explicitly.

For a standing real cosine all averaged mixed \(\mu z\) quadratic terms
have an odd compact derivative and vanish as \(\langle\sin kz\cos kz\rangle\).
No independent graviphoton wave is forced in this sector. Stationary
homogeneous scalar solutions satisfy
\((r^2f\phi')'=0\), so \(\phi=c_0+c_1\log f\). Fixed asymptotic circle
length removes \(c_0\); horizon regularity removes \(c_1\). Arbitrary
asymptotic tension shifts therefore cannot be appended as independent
regular horizon solutions. Sourced backreaction may carry tension. A
regular homogeneous Schwarzschild mass shift belongs to a separate
zero-mode metric sector and cannot cancel scalar news.

## 5. Retarded prescription and exact overlap

Set \(x=r_*\), \(\psi=r\phi\), \(s=2\Omega>0\),
\(V=f r_0/r^3\). The transformation of (1) gives
\[
 (\partial_t^2-\partial_x^2+V)\psi=A(t)^2 F(r),
 \qquad F(r)=\frac{\sqrt3 k^2}{16}rf\operatorname{tr}(H^2).
\tag{9}
\]
For the exponential particular response, solve
\(L_sq=F\), \(L_s=-\partial_x^2+V+s^2\). Define
\[
 y_H\sim e^{sx}\ (x\to-\infty),\qquad
 y_\infty\sim e^{-sx}\ (x\to+\infty),\qquad
 W=y_Hy_\infty'-y_H'y_\infty.
\]
Then
\[
 G_s(x,x')=-\frac{y_H(x_<)y_\infty(x_>)}{W},\qquad
 [\partial_xG_s]_{x'^-}^{x'^+}=-1.
\tag{10}
\]
The sign in (10) follows from \(-\partial_x^2G=\delta\); a positive
\(1/W\) is incorrect with this specified Wronskian convention.
Since \(V\ge0\), \(L_s\) is strictly positive for real \(s>0\), has no
bound-state pole, and its decaying Green kernel is positive. With positive
normalizations the homogeneous basis functions are positive and \(W<0\).
This is the retarded frequency continuation \(\omega=is\) for time
dependence \(e^{-i\omega t}\).

With the asymptotic basis normalized as above and the stated origin of
\(r_*\),
\[
 \boxed{\displaystyle
 C_0=-\frac{1}{r_0W}\int_{-\infty}^{\infty}y_H(x)F(r(x))\,dx
 =-\frac{\sqrt3 k^2}{16r_0W}
   \int_{r_0}^{\infty}y_H(r)r\operatorname{tr}(H^2)\,dr .}
\tag{11}
\]
It is dimensionless: \(W\) has inverse-length dimension and the first
integral is dimensionless. Then
\(\Phi(u)=r_0 C_0 A(u)^2\) for this settled exponential particular response.
For a future-horizon regular first-order mode, its invariant contraction
has the form \(\operatorname{tr}(H^2)=e^{2\Omega x}\times\text{bounded}\)
near the horizon in a regular ingoing basis. At infinity it decays with
\(e^{-2\sqrt{k^2+\Omega^2}\,r}\) times powers. Thus the integral converges
at both ends for \(k>0,\Omega>0\).

An eternal exponential is integrable towards the past in the retarded
convolution, but it diverges towards the future, outside perturbation theory.
A finite-data problem equals its exponential particular solution plus
homogeneous transients; its late-time interpretation requires a genuine
interval where those transients are small while \(A\ll1\). Multiplying
the first-order GL metric by an arbitrary switch \(\chi(t)\) generally
violates the linear constraints and is not a physical vacuum solution.
Such switching can regulate the scalar Green problem, but must be labeled
as a regulator or supplied with admissible constrained initial data on a
regular horizon-crossing slice. No quantitative transient bound or
controlled numerical amplitude interval is certified in this audit.

## 6. Cancellations: a retained counterexample and a conditional threshold result

The inference "nonzero local source implies nonzero outgoing coefficient"
is false. For any compactly supported smooth \(q(x)\), set \(F=L_sq\).
Self-adjoint integration by parts and \(L_sy_H=0\) give
\(\int y_HF\,dx=0\), although \(F\) can be nonzero. This is an exact
zero-overlap construction even for Schwarzschild \(V\).
The executable check uses the elementary flat case
\[
 q=e^{-a x^2},\quad
 F=(s^2+2a-4a^2x^2)e^{-a x^2},\quad
 \int_{-\infty}^{\infty}e^{sx}F\,dx=0.
\tag{12}
\]
Equation (12) is a manufactured adversarial control, **not** a cancellation
claimed for the GL source.

There is a useful analytic check at the static GL threshold. The
time-reflection-even, spatially decaying static mode has \(H_{tr}=0\),
so \(\operatorname{tr}(H^2)>0\) wherever its nontrivial diagonal
components are nonzero. (A stationary off-diagonal TT solution has a
non-exponential conserved radial tail and is excluded by the \(k>0\)
decay condition.) The exact scalar \(s=0\) basis is
\[
 y_H=r/r_0,\qquad y_\infty=-(r/r_0)\log f,\qquad W=-1/r_0.
\]
Consequently
\[
 C_0(k_{\rm cr}r_0,0)=\frac{\sqrt3 k_{\rm cr}^2}{16r_0}
   \int_{r_0}^{\infty}r^2\operatorname{tr}(H^2)\,dr>0.
\tag{13}
\]
This is a static scalar charge; its news is zero because \(\Omega=0\).
**Conditional continuation statement:** if the normalized, future-horizon
regular growing GL branch approaches this static mode continuously with a
uniformly integrable radial envelope, then (11) is continuous and remains
positive for some unquantified interval immediately below threshold.
This excludes an all-branch exact-zero theorem under those assumptions.
This reviewer did not independently prove that branch-continuity hypothesis
or find an interval size; (13) alone does not certify \(C_0\ne0\) at
\(kr_0=2\pi/(8,10,12,16)\).

## 7. Executed reproducible checks and mutations

Command:
`python3 research/sav/verification/audit_source_checks.py`

Executed with Python 3.14.8, SymPy 1.14.0. Exact output:

```text
Python 3.14.8 SymPy 1.14.0
LORENTZ_CONTRACTION: PASS; H2 = a**2 - 2*b**2 + c**2 + 2*d**2
DIRECT_CONNECTION_RZZ_ZERO_MODE: PASS; Qzz = -k^2 H2/8
CUBIC_ACTION_RADION_SOURCE: PASS; box phi = -sqrt(3) k^2 H2/16
SIGN_FOURIER_AND_LORENTZ_MUTATIONS: REJECTED (4)
RETARDED_GREEN_JUMP: PASS; wrong Wronskian sign rejected
STATIC_SCHWARZSCHILD_BASIS: PASS; W = -1/r0
MANUFACTURED_NONZERO_SOURCE_ZERO_OUTGOING_OVERLAP: PASS
SCOPE: no physical GL overlap, coefficient, or energy fraction certified
```

The four source mutations are wrong Ricci sign, missing cosine/sine
Fourier half, doubled scalar source coefficient, and positive Lorentzian
off-diagonal norm. A fifth wrong Wronskian sign is rejected by the delta
jump. These tests verify the stated algebra and a manufactured cancellation;
they do not solve or qualify a physical GL eigenfunction.

## Required next gate

Supply the invariantly normalized, constraint-checked horizon-regular GL
metric, evaluate (7) without losing cancellations near the horizon, and
evaluate (11) with endpoint/error control. If its source changes sign,
an interval/refinement estimate must resolve whether cancellation leaves a
nonzero coefficient. An explicit second-order Einstein-frame metric
constraint reconstruction remains a desirable additional consistency check.
Neither symmetry nor positivity of the scalar kernel substitutes for this
gate.

## 8. Numerical addendum: one independent L/r0=10 transfer check

This addendum follows the later availability of
`gl_linear_profile_L10.csv`, `gl_linear_results.json`, and the primary
`radion_transfer.py`. It does not retract the distinction between numerical
evidence and a mathematical interval proof. No GL eigenvalue equation was
re-solved by this reviewer and no physical wavenumber was added.

### Algorithms and normalization inspected

The primary transfer code uses
\(H_{\theta\theta}/(2r^2)=1\) at \(r=2r_0\), with \(r_0=1\).
This is the first-order *unitary gauge completion* of the sphere-radius
perturbation, not raw coordinate horizon displacement. Its scaling
`scale=8/Htheta(r=2)` is consistent with that convention; the source and
coefficient scale as `scale**2`. Its tensor contraction
\[
 2(a-d-b)(a-d+b)+4d^2
\]
is algebraically (7) after the trace condition. Evaluating the two nearly
canceling factors in high precision and using an eighth-order horizon
Frobenius series avoids a serious false divergence from directly subtracting
\(O((r-r_0)^{-2})\) Schwarzschild component squares. The regular series
coefficients solve the GL Riccati equation with denominators
\(j+2\Omega\); the implemented coefficient recurrence has the correct sign.
The scalar left-end correction
\(p_H=s+(r-r_0)/(1+2s)+O((r-r_0)^2)\) is also correct in \(r_0=1\) units.

The initial scalar infinity inverse-radius Riccati/logarithm recurrence was
algebraically correct, but is an *asymptotic series*. Small ODE solver
tolerance alone does not bound its finite-radius truncation error, especially
when \(s\) is small. The primary author identified a material L/r0=8
coarse-to-fine discrepancy and replaced that scalar normalization by the
positive Volterra bound below. This is an appropriate bounded repair, not
evidence that a physical source cancels.

### Independent calculation executed

Command:
`python3 research/sav/verification/audit_source_transfer.py`

Environment: Python 3.14.8, NumPy 2.4.6, SciPy 1.18.0, mpmath 1.3.0.
Machine-readable output: `audit_source_transfer_results.json`.
The script imports neither the primary scalar-basis routine nor its
Frobenius tensor-contraction routine. It uses the single saved final mode
profile, recomputes the Lorentz contraction from its saved metric components
in 60-digit arithmetic on \(r-r_0\ge0.00101126655\), integrates *linear*
scalar basis equations directly, and evaluates a fixed-mesh Simpson overlap.
The omitted horizon segment is explicitly exposed rather than silently
discarded. A quadratic regular-power continuation on that same saved mesh
is an additional diagnostic, with no asserted interval remainder bound.

The independent infinity normalization instead writes
\(y_\infty=e^{-sx}w\) and solves
\(w_{xx}-2sw_x-Vw=0\) backwards from \(R=1000r_0\). Its endpoint is the
first Born iterate of the positive Volterra equation. In \(r_0=1\) units,
\(\eta=1/(4sR^2)=1.97531054683\times10^{-6}\) bounds the tail integral
operator. The first-Born endpoint error for \(w(R)\) is at most
\(\eta^2/(1-\eta)=3.90185946380\times10^{-12}\); this is an analytic
tail estimate, **not** an enclosure of the subsequent floating ODE solution.

Results for \(kr_0=2\pi/10\),
\(\Omega r_0=0.06328118897590773\):

| Quantity | Independent result |
|---|---:|
| invariant normalization at r=2 | 1.0 |
| log y_infinity at r=2 | -0.07926044679746594 |
| p_H-p_infinity at r=2 | 0.5025456616692139 |
| W, with y_H(r=2)=1 | -0.4642513267207237 |
| C0, retained profile segment only (Simpson) | 12.298694970217497 |
| C0, retained segment only (trapezoid diagnostic) | 12.29854749263057 |
| estimated omitted leading horizon contribution | 0.008580540109026478 |
| C0, Simpson plus leading horizon estimate | 12.307275510326525 |
| C0, Simpson plus quadratic fitted horizon continuation | 12.307291908660186 |
| initial primary reference value | 12.3072919626396 |

The independent scalar log normalization agrees with the initial reference
within \(2.7\times10^{-13}\), and its Riccati difference agrees within
\(1.1\times10^{-16}\). The fitted-horizon transfer agrees within
\(5.4\times10^{-8}\); the leading-only horizon estimate differs by
\(1.65\times10^{-5}\), as expected when its next regular power is omitted.
These are comparisons of computed values, not certified error bars.
The independently recomputed source contraction is positive at every
retained saved-profile point, with maximum approximately 685.2413 and
minimum approximately \(1.27805\times10^{-38}\). Sparse positivity does
not prove positivity between points or in the omitted near-horizon segment.

### Adversarial check of the replacement scalar-tail bracket

For \(V=f/r^3\), the exact positive Volterra equation is
\[
 w(x)=1+\int_x^\infty
 \frac{1-e^{-2s(t-x)}}{2s}V(t)w(t)\,dt.
\]
Let \(J=1/(4sR^2)\) and
\[
 E_1=\int_R^\infty
 \frac{e^{-2s[x(r)-x(R)]}}{r^3}\,dr,
 \qquad K_1=\frac{1/(2R^2)-E_1}{2s}.
\]
Ordered positive iterates are bounded by \(J^n/n!\). Therefore the primary
replacement formulas
\[
 w(R)\in[1+K_1,\;1+K_1+e^J-1-J],\qquad
 -w_x(R)\in[E_1,e^J E_1]
\]
are valid analytic inequalities. With \(y_H\) positive,
\[
 -W=e^{\ell_H(R)-s x(R)}
 \{[s+p_H(R)]w(R)-w_x(R)\}
\]
has the correct normalization and sign. The primary code normalizes
\(y_H(r=2)=1\) by subtracting \(\ell_H(2)\), which cancels consistently
between its source integral and Wronskian. Its `E1` numerical quadrature,
left-basis ODE and GL profile are floating computations; hence its printed
bracket is a scalar-tail analytic bracket *conditional on those inputs*,
not a rigorous full-C0 interval enclosure. The `E1` quadrature estimate
should be retained as numerical uncertainty rather than suppressed.

**Updated numerical verdict:** the one reference calculation supplies strong,
independently reproduced numerical evidence for a positive quadratic outgoing
coefficient near 12.30729 in the stated invariant normalization. There is no
observed exact cancellation. It does not constitute an interval proof for
the exact GL solution, nor a controlled nonlinear finite-amplitude theorem.
No second-order metric/remainder bound has been produced here, so this audit
does not certify a numerical amplitude interval or extrapolation to A~1.
The exact-source/conditional-flux result and this numerical transfer estimate
should remain distinct evidence classes. `MEANINGFUL_PARTIAL` is an honest
terminal classification when those stronger gates remain open.
