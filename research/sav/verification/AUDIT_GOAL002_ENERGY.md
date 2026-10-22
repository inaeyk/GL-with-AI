# GLR-GOAL-002 adversarial audit II — data, energy and nonlinear control

Independent scientific AI auditor: `audit_energy`, 2026-10-22. This is a
separate derivation/code path within the same AI system, not an external
human referee. Scope is finite Cauchy data, scalar energy, nonlinear
amplitude conventions and what can actually follow from them.

**Verdict:** the scalar transient energy estimate, the conditional energy
error inequality, the amplitude-redefinition law and second-order data
dependence below are mathematically sound under their stated hypotheses.
An exact scalar-only constraint family demonstrates that free scalar data
are physical. A simultaneously completed finite-amplitude GL-seeded data
family and its nonlinear remainder have **not** been proved here. Thus
neither a nonzero controlled GL amplitude interval nor a first-bulge energy
fraction is certified by this audit. A conditional constraint-map statement
must not be relabeled an unconditional nonlinear existence theorem.

## 1. Exact independent scalar data on a horizon-crossing slice

The consistent, uncharged, z-independent KK theory has
\(R_{\mu\nu}=2\partial_\mu\phi\partial_\nu\phi\), \(\Box\phi=0\),
with the metric and normalization in NOTATION.md. On an initial slice set
\[
\gamma_{ij}dx^idx^j=a(r)^2dr^2+r^2d\Omega_2^2,\quad
K^i{}_j=\mathrm{diag}(k_R,k_T,k_T),\quad \Pi=n\phi,
\]
where \(K_{ij}=-\tfrac12\mathcal L_n\gamma_{ij}\). The constraints are
\[
{}^{(3)}R+K^2-K^i{}_jK^j{}_i=2(\Pi^2+a^{-2}\phi'^2),\qquad
D_jK^j{}_r-D_rK=-2\Pi\phi'. \tag{1}
\]
Direct three-dimensional Christoffels give
\[
m=\frac r2(1-a^{-2}),\quad {}^{(3)}R=\frac{4m'}{r^2},\qquad
k_R=k_T+rk_T'-r\Pi\phi'. \tag{2}
\]
Consequently the remaining constraint is the **linear radial ODE**
\[
m'+r\phi'^2m=\frac{r^2}{2}
 [\Pi^2+\phi'^2-2k_Rk_T-k_T^2]. \tag{3}
\]
It is linear in m because k_R is already given by (2). This observation
does not solve the z-dependent GL constraint problem.

For a concrete horizon-crossing background use \(\tau=v-r\),
\(v=t+r_*\), with background data
\[
a_0^2=1+r_0/r,\quad
k_{T0}=\frac{r_0}{r\sqrt{r(r+r_0)}},\quad
k_{R0}=k_{T0}+rk_{T0}',\quad
m_0=\frac{rr_0}{2(r+r_0)}. \tag{4}
\]
These data satisfy (1) exactly and are smooth across r=r_0. Choose any
real \(F,P\in C_c^\infty((R_1,R_2))\), \(r_0<R_1<R_2\), and set
\(\phi=\delta F\), \(\Pi=\delta P\), \(k_T=k_{T0}\).
For inner condition m=m_0 below R_1, (3) gives the exact solution
\[
\Delta m(r)=\delta^2e^{-I(r)}\int_{R_1}^{r}e^{I(s)}S_2(s)ds,
\quad I(r)=\delta^2\int_{R_1}^{r}sF'(s)^2ds, \tag{5}
\]
with integrals continued using the zero extension of F,P and
\[
S_2=\frac{r^2}{2}
 [P^2+a_0^{-2}F'^2+2rk_{T0}PF']. \tag{6}
\]
The source is positive for a nontrivial pulse outside the horizon:
\[
P^2+2rk_{T0}PF'+a_0^{-2}F'^2
=(P+rk_{T0}F')^2+fF'^2,
\quad a_0^{-2}-r^2k_{T0}^2=f>0. \tag{7}
\]
Writing \(B=\int_{R_1}^{R_2}S_2dr\), one has
\(0\le\Delta m\le\delta^2B\), and
\[
a^{-2}\ge\frac{R_1}{R_1+r_0}-\frac{2\delta^2B}{R_1}>0
\quad\hbox{if}\quad
\delta^2<\frac{R_1^2}{2B(R_1+r_0)}. \tag{8}
\]
Near the horizon the data are exactly the original data, and at infinity
the pulse vanishes while m tends to a finite shifted ADM mass. The KK lift
therefore supplies exact vacuum 5D initial data, periodic in z, on this
black-hole horizon-crossing region. This is an exterior/horizon-crossing
construction; no theorem about a globally complete nonsingular black-hole
interior is implied. In the lift,
\[
\gamma^{(5)}_{ij}=e^{-2\phi/\sqrt3}\gamma_{ij},\quad
\gamma^{(5)}_{zz}=e^{4\phi/\sqrt3},\quad
K^{(5)}_{ij}=e^{-\phi/\sqrt3}
 (K_{ij}+\Pi\gamma_{ij}/\sqrt3),\quad
K^{(5)}_{zz}=-2e^{5\phi/\sqrt3}\Pi/\sqrt3.
\]
The scalar-only interval (8) is **not a GL A_* or a radiation error bound**.
It only demonstrates physical, independently selectable scalar data.
If total ADM mass is fixed, the homogeneous mass integration constant
must be adjusted. Equation (3) has derivative
\(\partial m_\infty/\partial m_{\rm inner}=e^{-I(\infty)}>0\), so such
a compensating small shift exists; it is of order \(\delta^2\).
That variant alters the near-horizon mass by a small regular amount.

## 2. What is and is not a non-universality theorem

Suppose one has a constraint-compatible expansion
\(g=\bar g+\epsilon h_1+\epsilon^2q_2+\cdots\), with the fixed nonzero
first-order GL mode and no first-order radion. Let \(\varphi\) be any
smooth homogeneous zero-mode scalar solution. Adding the linear KK lift
\[
\Delta q_{\mu\nu}=-2\varphi\bar g_{\mu\nu}/\sqrt3,
\qquad \Delta q_{zz}=4\varphi/\sqrt3 \tag{9}
\]
changes the radion by \(\epsilon^2\varphi\) and leaves every second-order
Einstein equation and constraint unchanged. The 4D Einstein-frame metric
addition at that order is zero. This follows because scalar stress is
quadratic, so for an O(\(\epsilon^2\)) scalar it starts at O(\(\epsilon^4\)).
The exact scalar construction above with \(\delta=\epsilon^2\) verifies
that this tangent freedom is not a pure-gauge artifact.

If \(n_f\) is its nonzero radiation news on a window U, then
\[
N_{\epsilon,b}=\epsilon^2(n_{\rm GL}+b n_f)+O(\epsilon^4),
\quad
\frac{E_U}{M_0}=\frac{2\epsilon^4}{r_0}
 [a+2bc+b^2d]+O(\epsilon^6), \tag{10}
\]
where \(a=\|n_{\rm GL}\|_U^2\), \(c=\langle n_{\rm GL},n_f\rangle_U\),
\(d=\|n_f\|_U^2>0\). Hence the coefficient of early energy is not
determined by the fundamental GL amplitude, k and Omega alone in this
second-order solution class. Translating by half a compact period preserves
this even scalar choice. No cancellation or failure of the GL source is
being asserted. The energy has a genuine interference term; there is no
unique positive split into a GL-generated and a free part unless a data
prescription fixes that split.

Nontrivial outgoing scalar solutions exist independently of the GL source:
the scalar equation is a wave equation with smooth integrable potential;
its outgoing radiation map is nonzero (for example its real-frequency
transmission coefficient is nonzero for nonzero frequency). One may choose
smooth finite-energy data with nonzero radiation in a prescribed suitable
causal window. The bound below does not assert that every free datum
radiates; some energy can fall through the horizon.

For a theorem about **exact simultaneous GL-seeded finite-amplitude
families**, (10) additionally requires a smooth solution of the nonlinear
vacuum constraints with these prescribed GL and scalar tangent data,
compatible charges/boundary conditions, and differentiability of the
radiation map. This audit supplies no right inverse/implicit-function
estimate for that z-dependent constraint map. Thus (10) is an unconditional
statement at second perturbative order, and a **conditional** statement
about exact GL-seeded families. It is invalid to turn the scalar-only
construction into proof of the missing simultaneous family by linear
superposition of nonlinear data.

## 3. A quantified scalar transient bound with a regular initial slice

For \(\psi=r\varphi\), \(x=r_*\), the homogeneous scalar equation is
\[
\psi_{tt}-\psi_{xx}+V\psi=0,\qquad V=fr_0/r^3\ge0. \tag{11}
\]
On a constant-t exterior slice, with endpoint boundary terms vanishing,
\[
Q_t=\frac12\int(\psi_t^2+\psi_x^2+V\psi^2)dx,
\qquad E_{\rm scalar}=Q_t/G_4. \tag{12}
\]
The exact integration-by-parts term is
\[
(\psi_x-f\psi/r)^2-\psi_x^2-V\psi^2
=-\partial_x(f\psi^2/r). \tag{13}
\]
For the horizon-crossing \(\tau=t+x-r\) slice, let
\(F_\psi(x)=\psi|_\Sigma\), \(P_\psi(x)=\partial_t\psi|_\Sigma\),
\(h'=r_0/r\). The flux of the same Killing current is
\[
Q_\Sigma=\frac12\int
 [(1-h'^2)P_\psi^2+(F_\psi')^2+VF_\psi^2]dx\ge0. \tag{14}
\]
It is finite for smooth compact pulse data, and also for suitable regular
decays at the future horizon. Its degeneracy at the horizon is physical:
the Schwarzschild Killing field becomes null there. It is not a coercive
norm controlling all regular derivatives, so (14) alone cannot close the
nonlinear metric hierarchy.

Positive horizon flux and conservation give the exact scalar-linear bound
\[
\int_{\mathscr I^+}|N_f|^2du\le Q_\Sigma,
\qquad \|N_f\|_{L^2(U)}\le\sqrt{Q_\Sigma}. \tag{15}
\]
This gives an integrated transient budget, without invoking a quantitative
Price law or claiming a pointwise late-time tail estimate. On a finite
weak-amplitude window it can be compared directly with the particular
exponential response:
\[
\|N_p\|_U^2=\Omega r_0^2C_0^2(A_2^4-A_1^4). \tag{16}
\]
If initial mismatch data are known and \(Q_\Sigma\) is bounded, then
\(\sqrt{Q_\Sigma}/\|N_p\|_U\) bounds their relative integrated influence.
This is a sufficient comparison condition, not proof that every finite-time
family settles before perturbation theory fails. Any error in the leading
mode/source, extraction and higher scalar orders must be added separately.

## 4. The actual ten-percent gate

For \(N=N_p+\Delta N\) on U, Cauchy–Schwarz gives
\[
\frac{|E_U-E_{p,U}|}{E_{p,U}}
\le2\eta+\eta^2,\qquad
\eta=\frac{\|\Delta N\|_{L^2(U)}}{\|N_p\|_{L^2(U)}}. \tag{17}
\]
Thus a sufficient condition for ten-percent energy accuracy, measured
relative to the predicted energy, is
\[
\eta\le\sqrt{1.1}-1=0.04880884817015155\ldots . \tag{18}
\]
A ten-percent news-norm error instead permits a 21-percent energy error.
If a bound \(\eta\le\eta_{\rm trans}+K_4A_{\max}^2+\eta_{\rm num}
+\eta_{\rm extract}\) is proposed, each term must actually be evaluated.
Unknown \(K_4\), a merely empirical C0 error or an unspecified data energy
cannot be set to zero. No numerical GL A_* follows from (18) here.

There is a further data-level obstruction, before any estimate of a
particular fourth-order forced response: one may add an independent
homogeneous zero-mode scalar at order \(\epsilon^4\). Its linear KK lift
solves the homogeneous fourth-order equations and leaves h1 and q2
unchanged. Its self stress starts at \(\epsilon^8\), its stress cross term
with an existing \(\epsilon^2\) scalar starts at \(\epsilon^6\), and
coupling to the first-order GL field starts at \(\epsilon^5\). Its finite
coefficient is arbitrary in the formal hierarchy. Consequently no
uniform K4 over unspecified higher-order initial-data families follows
from h1 and q2 alone, even if q2 were fully bounded. Fixing an exact
data family, or imposing an explicit fourth-order scalar-data norm,
is essential. The executable checks these stress orders independently.

## 5. Cubic amplitude convention and smooth spectra

For \(\dot A=\Omega A+\gamma A^3+\cdots\),
\(B=A+cA^3\), \(d\tau/dt=1+dA^2\), direct expansion yields
\[
\frac{dB}{d\tau}=\Omega B+
 [\gamma+2c\Omega-d\Omega]B^3+O(B^5). \tag{19}
\]
At fixed nonzero Omega the cubic coefficient is coordinate dependent.
An amplitude-dependent global clock change is excluded when the physical
asymptotic clock is fixed; its algebra nevertheless shows what an
uncontrolled clock choice would do. Near threshold Omega tending to zero
is a distinct limit. Static branch coefficients do not determine (19).
This audit does not attempt a dynamical cubic coefficient through failed
lower-order gates.

For finite-energy news the positive-frequency spectrum is
\(dE/d\omega=|\widetilde N(\omega)|^2/(\pi G_4)\). As an independent
smooth memory control take
\[
N(u)=\frac{B}{\sqrt\pi\tau}e^{-u^2/\tau^2},\quad
\widetilde N=B e^{-\tau^2\omega^2/4},\quad
\int N^2du=\frac{B^2}{\sqrt{2\pi}\tau}. \tag{20}
\]
The frequency integral gives the same result. This tests a real smooth
finite-energy waveform and avoids differentiating a sharply cut waveform.
It is a manufactured flat outgoing-wave control, **not a calculated
Schwarzschild GL spectrum**. Smooth manufactured baseline/free news also
give nonzero interference and two different finite energies. These checks
support the algebra, not an unperformed physical scattering calculation.

## 6. Executed checks and rejected mutations

Command executed (exit zero):

```text
python3 research/sav/verification/goal002_audit_energy.py > research/sav/verification/goal002_audit_energy_results.txt
```

Environment: Python 3.14.8, SymPy 1.14.0, NumPy 2.4.6, SciPy 1.18.0.
Exact stdout is in `goal002_audit_energy_results.txt`; machine-readable
results are in `goal002_audit_energy_results.json`. The independent script
imports no central GOAL-002 or GOAL-001 equation implementation.

Eighteen checks pass, including direct curvature/momentum constraints,
the scalar-pulse mass ODE, horizon-crossing energy, flux normalization,
the actual 10-percent criterion, cubic convention and smooth Parseval.
Eleven deliberate errors are rejected: momentum source sign, scalar
Hamiltonian factor two, omitted momentum backreaction, scalar potential
sign, extra null-flux half, 10-percent-news/energy confusion, invariant
finite-Omega gamma, omitted interference, and a two-sided/positive-frequency
factor-two error, a hierarchy ratio with an uncertified upper-bound
denominator, and componentwise smallness used as an operator-norm bound.

The smooth manufactured integrals are
\(a=4.1599877247378405\), \(d=0.2659615202676218\),
\(c=0.9533048699511361\). Choices b=0 and b=1 give
4.1599877247378405 and 6.332558984907735 respectively. Exact Gaussian
expressions and analytic endpoint-tail bounds accompany the floating
quadrature error estimates in JSON. These are not emitted GL fractions.

## 7. Acceptance status

| Gate | Audit conclusion |
|---|---|
| Independent scalar data are physical | Exact scalar-only constraints/lift; regular nondegenerate exterior interval (8) |
| Non-universality at fixed first-order GL mode | Proved at second perturbative order, conditional for exact simultaneous families |
| Scalar transient integrated bound | Exact scalar-linear Killing-energy inequality; initial mismatch norm must be supplied |
| Controlled finite GL A interval | NOT CERTIFIED: simultaneous data family, full q2 and first omitted scalar bound unavailable |
| Physical full burst spectrum | NOT CERTIFIED: only linear formula and manufactured smooth controls |
| First-bulge fraction | NOT CERTIFIED; no extrapolation allowed |

Concrete next strategy: establish a bounded right inverse for the
horizon-crossing z-dependent vacuum constraint map after fixing its charge
kernel, then construct the selected GL-plus-scalar family and bound the
even scalar remainder in an energy space with a controlled radiation
trace. Merely refining C0 or reusing scalar stability does not replace
those missing estimates.

## 8. Review of the actual central scalar-response calculation

This auditor inspected `goal002_scalar_response.py` and its three-configuration
JSON output after deriving (14)–(18) independently. The central code was
not imported into the independent check and no extra physical radial solve
was run by this auditor. The coordinate called v in the script is
\(\log(r-1)\), distinct from EF advanced time. In r0=1 units,
\(\tau=t+\log(r-1)\), so its data transformations
\[
F_\Sigma=e^{-s\log(r-1)}q,\quad P_\Sigma=sF_\Sigma,\quad
\partial_x F_\Sigma=e^{-s\log(r-1)}(q_x-sq/r)
\]
are correct. The EF tetrad is orthonormal by an additional exact check in
this auditor's script. The reverse cumulative Green integral has a
positive final orientation. The initially reviewed version used two
cancelling minus signs; the final version directly assigns the positive
`cumulative_simpson(..., x=-v[::-1])[::-1]` expression. They are algebraically
identical. The C coefficient retains the required minus Wronskian sign.

The saved results approach
\(Q_\Sigma\simeq80.8179176\) for a unit particular-versus-zero-data mismatch,
and the formula
\[
T=\frac1{2s}\log\left(1+\frac{2Q_\Sigma}
 {\eta^2sC_0^2}\right),\qquad \eta=\sqrt{1.1}-1,
\]
gives T approximately 32.2842853 for the window u in [0,T]. Direct symbolic
substitution verifies this formula's **transient-only** ten-percent energy
criterion. It is not a bound on the physical nonlinear energy because
Q is floating evidence and the data completion/remainder are unbounded.
The first-order EF norm near 39.4663 is a sampled regular-frame diagnostic;
it supplies no uniform maximum or finite-A error theorem. Inner-tail
endpoint values are leading omission diagnostics; outer basis W drift and
ODE tolerances must not be promoted to total quadrature/source enclosures.

The subsequent `physics/goal002_finite_data_energy.md` was also read in
full. Its P/Z scalar-data prescription correctly declares the missing
simultaneous nonlinear GL constraint completion. Its equation (11) has
consistent units: Q has length and \(s r_0^2\) has length. This auditor
identified two presentation/mathematical issues for correction:

1. The news-norm threshold (18) is sufficient for ten-percent energy
   accuracy, not a necessary condition on every waveform error; interference
   can reduce the actual energy error even at larger news norm.
2. For a hierarchy ratio, an upper bound M1 on \(\|h_1\|_X\) cannot be used
   in the denominator to guarantee smallness. Use a certified lower bound
   \(0<m_1\le\|h_1\|_X\), so the sufficient condition is
   \(A_{\max}M_2\le\eta_hm_1\). The nondegeneracy norm must dominate the
   matrix operator norm (a Frobenius supremum suffices). Bounds on each
   component separately below one are insufficient.

Both failures have explicit counterexamples in the independent executable:
upper bounds M1=100, M2=10 with actual first-order norm one and A=0.1
give a misleading proxy ratio 0.01 but actual ratio one; a four-dimensional
spatial block \(I-0.3\mathbf 1\mathbf 1^T\) has a negative eigenvalue,
despite each metric perturbation component being less than one. No
finite-amplitude theorem is inferred from these simple counterexamples.

Final bookkeeping review: the central finite-data note now uses the
sufficient-condition wording, the lower bound
\(0<m_1\le\|h_1\|_X\le M_1\), and an explicit Euclidean Frobenius
supremum dominating the matrix operator norm. The proposed conservative
lower bound m1=2 is valid: at the normalization sphere and maximal cosine,
the two sphere tetrad entries alone each have magnitude two, giving a
Frobenius magnitude at least \(\sqrt8>2\). These changes resolve the
identified inequality defects without establishing the missing M2/K4.

The scalar script additionally records the E1 quadrature estimate and
the derivative endpoint remainder \((e^J-1)E_1\). The latter follows
directly from \(E_1\le-w_x(R)\le e^J E_1\) and is consistent with its
first-Born derivative datum. These are separate error components, not a
total Q or C0 enclosure. The primary author reports rerunning the same
three configurations after simplifying the cancelling signs; this auditor
checked expression equivalence and the preserved Q/C0 output, and ran no
additional physical solve.
