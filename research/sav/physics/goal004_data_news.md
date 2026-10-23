# GOAL-004: global-data tests and the missing nonlinear news estimate

2026-10-23. Input `2f4648c379c474ed124e3ef10b62efcd4a783c66`;
executor Savdeep Sethi. Exact SO(3), 5D vacuum, conventions in NOTATION.
This is mathematical analysis, not spacetime evolution. Prior Goals retain
MEANINGFUL_PARTIAL. [Audit I](../verification/AUDIT_GOAL004_GEOMETRY.md)
and [Audit II](../verification/AUDIT_GOAL004_DYNAMICS.md) distinguish
independently derived identities from the unproved implications below.

**Result:** a previously unevaluated *background* charge derivative can
be calculated exactly. A differentiated exterior estimate and a linear
parabolic kernel identify a concrete route to a global family. Neither
establishes the simultaneous nonlinear global GL family, its right inverse,
or a radiation trace. There is no numerical positive GL amplitude interval.

## 1. All-orders data and a new exact mass-tuning subfamily

Use the radial constraints (1)--(3),(5) of
[the prior data derivation](goal003_data_nonlinear.md), not a superposition
of exact metrics. Let r0=1, L=10, Ri=3/4, and use the regular
tau=v-r initial slice. For each fixed smooth compact F,P define

\[
b=e^{2\epsilon^2F/\sqrt3},\quad
R=r e^{\epsilon R_1/r-\epsilon^2F/\sqrt3},\quad
d=d_0+\epsilon(\kappa_1+q_1/2),\quad
v=-2\epsilon^2P/\sqrt3,\qquad\kappa=-q/2+d.
\tag{1}
\]

At Ri prescribe a=a0 exp(epsilon a1/a0+lambda),
q=q0+epsilon q1, eta=epsilon eta1. Here the first jets are the restriction
of the *exact growing GL solution*, with its velocity, not separately
chosen shapes. Fix the normal-transport chart used in the old derivation.
These formulas, radial constraints and leaf values specify every order;
no later order-four scalar can be silently appended. Reflection enforces
zero KK momentum when a global charge exists; half-period covariance
requires lambda(epsilon) even. Circle length/clock/phase are fixed.

Two explicitly specified **local** choices are F=P=0 and P=0 with
F=exp[-1/((r-2)(3-r))] on 2<r<3, zero elsewhere. Both are admitted by
the prior local mixed estimate on each fixed finite collar. This does not
prove either is global. Particular-matched P versus zero-radion Z data
remain a comparison at perturbative order two; their simultaneous exact
global extension is conditional. The scalar-only exact family is not a
proof of that extension.

There is useful exact charge information without GL. Keep epsilon=0,
q=q0=2/[r sqrt(r(r+1))], eta=v=0, b=1,R=r,
d=d0=1/[2r^(3/2)(r+1)^(3/2)]. Then Z0=-2/[r²(r+1)²], and the
Hamiltonian equation for u=a^-2 is

\[
u_r=(1-u)/r+rZ_0/2,\qquad u_0=r/(r+1).
\]

Changing the inner lapse by exp(lambda) therefore gives the exact family

\[
u_\lambda=u_0+C(\lambda)/r,\qquad
C(\lambda)={R_i^2\over R_i+1}(e^{-2\lambda}-1).
\tag{2}
\]

The momentum constraints are unchanged. Since r²/(r+1) increases,
u_lambda>0 for all r>=Ri and finite lambda. For sufficiently small
lambda the mass is positive and the same exterior interpretation applies.
Reading a²=1+(1-C)/r+O(r^-2), with gzz=1, gives

\[
M(0,\lambda)=M_0(1-C(\lambda)),\qquad
{\partial_\lambda M(0,0)\over M_0}
={2R_i^2\over R_i+1}={9\over14}.
\tag{3}
\]

Reviewer I independently obtained (2)--(3); the primary author checked
them directly through the linear u equation and ADM asymptotics. This
closes **background transversality**, not the global implicit-function
theorem. A C1 global map M(epsilon,lambda) and a neighborhood where
|partial_lambda M|>=9M0/28 would suffice to tune the mass locally by IFT;
neither uniform global continuation nor that C1 map has been proved.
The inner boundary changes, as required by the KID pairing. ADM tension
is an asymptotic stress datum requiring its own spacetime/asymptotic
prescription, not an independent conserved Cauchy charge associated with
time translation. No extra freely adjustable regular tension hair is used.

## 2. A differentiated exterior obstruction, and a bounded linear repair

The old zeroth-order identity does not bound pointwise Z. In the ideal
exterior put Q=r²q, J=r²eta and first take d=v=0. The exact equations are

\[
Q_r=aJ_z+2a_zJ,\qquad J_r={a\over2}Q_z-{a_z\over2}Q.
\]

With E_m=int[(partial_z^m Q)²/2+(partial_z^m J)²] dz,
E0'=0, but the independently audited differentiated identity is

\[
E_1'=2\int a_zQ_zJ_z dz+\int a_{zz}(2Q_zJ-QJ_z)dz,
\]
\[
\partial_r\sqrt{E_1}\le {\|a_z\|_\infty\over\sqrt2}\sqrt{E_1}
 +{3\over\sqrt2}\|a_{zz}\|_\infty\sqrt{E_0}.
\tag{4}
\]

Thus a specific missing inequality is integrability of angular lapse
derivatives, plus higher commutators and actual free-tail terms:

\[
\int_{R_m}^\infty(\|a_z\|_\infty+\|a_{zz}\|_\infty)dr<\infty.
\tag{5}
\]

Audit I gives a bounded-E0 periodic sequence with unbounded point values,
so omitting (5) is a false proof. This is failure of an inference, not
nonexistence of global GL data.

A second, explicitly bounded linear route comes from angular parabolic
damping. Linearize the actual ideal exterior around a0=sqrt((r+1)/r),
q0,d0, with alpha=delta a and W=r² delta q. Direct differentiation gives

\[
\alpha_r={r+1\over2}\alpha_{zz}
-{2r+5\over2r(r+1)}\alpha+{2r+1\over4r^4}W.
\tag{6}
\]

For harmonic k_n=2pi n/L its exact scalar evolution kernel is

\[
G_n(r,\rho)=
e^{-k_n^2[(r^2-\rho^2)/4+(r-\rho)/2]}
(\rho/r)^{5/2}[(r+1)/(\rho+1)]^{3/2},
\tag{7}
\]

alpha_n(r)=G_n(r,Rm)alpha_n(Rm)+int_Rm^r G_n(r,rho)
(2rho+1)W_n(rho)/(4rho^4) drho. For n!=0, r>=2Rm>=2,
and |W_n|<=Bn, splitting at rho=r/2 gives the explicit loose bound

\[
|\alpha_n(r)|\le G_n(r,R_m)|\alpha_n(R_m)|
 +{3B_n\over8R_m^2}e^{-3k_n^2r^2/16}
 +{24B_n\over k_n^2r^4}.
\tag{8}
\]

Here G_n<=exp[-k_n²(r²-rho²)/4], and (2rho+1)/(4rho^4)<=3/(4rho³).
The zero mode instead has a 1/r homogeneous tail, which is precisely a
charge direction. Equations (6)--(8) demonstrate integrable angular-lapse
tails *for bounded linear momentum modes*. They do not bound the full
coupled nonlinear Fourier tower: a uniform Sobolev/maximal-regularity
estimate and high-derivative source control are still required. Exchanging
a radial supremum and an infinite Fourier sum without an estimate would
be another gap. Two routes here are exhausted: the nonlinear H1 energy
plus conditional coefficients, and this exact linear parabolic kernel.
No third global-data repair or radial PDE experiment is attempted.

## 3. Exact hyperbolic variables do not supply the missing trace constant

The exact SO(3) action is (4G5)^-1 int sqrt(-g3)
[R² Ric3+2(grad R)²+2]. In R>0 put
g3E=(R/r0)^4g3 and chi=sqrt(12) log(R/r0). Including the corresponding
boundary terms, its bulk becomes

\[
S={r_0^2\over4G_5}\int\sqrt{-g_{3E}}
[\mathcal R_{3E}-{1\over2}(\nabla\chi)^2-U_3(\chi)],\qquad
U_3=-{2\over r_0^2}e^{-\sqrt3\chi}.
\tag{9}
\]

Consequently R_ab[g3E]=partial_a chi partial_b chi/2+U3 g_ab,
Box3E chi=U3'. A fixed generalized-harmonic gauge converts the principal
metric part to -g3E^cd partial_c partial_d g3E_ab/2, and the scalar part
to its wave operator. Smooth future-horizon charts give a regular local
quasilinear hyperbolic system, with homogeneous propagation of harmonic
constraints when the full constraints/boundary data vanish. This is an
analytic gauge choice for this calculation, not a change to production.

It yields standard fixed-compact-region local energy estimates under
positive-lapse and smooth-coefficient hypotheses. It supplies no numerical
radius or global radiation map. In particular g3E grows like r^4 in the
asymptotic region, so an ordinary asymptotically Minkowski 2+1 theorem
cannot simply be imported. Also chi is the sphere-orbit dilaton, not the
completed n=0 KK radion. Extracting the latter requires the constrained
tower and Einstein-frame Bondi coordinates. A horizon-degenerate scalar
Killing norm is insufficient; its counterexample is in the prior audit.

An actual estimate would need an r-adapted regular-gauge Sobolev energy
X_s, an exterior/radiation space Y, redshift flux and constraint bounds
such that, schematically but with finite constants to be supplied,

\[
\|\mathcal T_0U\|_{L^2(U_u)}\le C_{tr}\|U\|_Y,\qquad
\|\mathcal S_Q(t)\|_{X_s\to X_s}\le C_S e^{\sigma t}.
\tag{10}
\]

The old three-source-derivative metric loss must be absorbed with an
explicit tame hierarchy or a no-loss evolution variable. A single
real-Laplace resolvent value proves neither estimate. No nonlinear
redshift/exterior/trace constant is evaluated here.

## 4. Quartic source, data freedom and a conditional amplitude interval

Use the full ordered Q3,Q4 and Lie recursion (12)--(16) of the old data
note. In particular Q4 contains h1 q3, q2², h1² q2 and h1^4, with q3 at
n=1,3. The scalar completion is

\[
\phi_4={\sqrt3\over4}(\chi_4-\chi_2^2/2),\quad
\bar\Box\phi_4={\sqrt3\over2}\Pi_0Q_{4,zz}
-{\sqrt3\over8}\bar\Box(\chi_2^2),
\]

followed by the order-two Bondi time/radius pullback. The ordered
polynomials are available, but no solved/bounded physical q3 is available.
Even a certified q2 cannot determine N4 without the all-orders free-data
rule and its evolution. A tame product estimate would have to give

\[
\|S_{4,0}\|_Y\le C_{prod}
(M_1M_3+M_2^2+M_1^2M_2+M_1^4)+C_{gauge},
\tag{11}
\]

and a retarded trace estimate must propagate this with the selected
order-four homogeneous data. Every M_j here is a horizon-regular
derivative norm in compatible spaces, not a sample maximum. M3,
Cprod,Cgauge,Ctr and the actual source error remain open.

For a fixed exact even family with a C4 news map, let
K4(U)=sup_{|a|<=epsilon}||partial_a^4 N_a||_U/24. Taylor gives
||N_epsilon-epsilon² n2||_U<=K4 epsilon^4. Write
n2=nGL+nfree and a2=||n2||_U. Conditional on a2>0 and finite K4,

\[
\epsilon^2 K_4<a_2
\tag{12}
\]

is the strict positive-energy condition after this remainder alone;
epsilon² K4/a2<=sqrt(1.1)-1 is sufficient for ten-percent reference
energy accuracy. Other input/trace/transient errors add in news norm.
This is an explicit symbolic interval, **not a numerical epsilon_star**:
K4 has not been proved finite for the global data/news problem.
Free scalar interference can make a2 small or zero even at a nonzero
local source. No universal strictly positive lower bound over unrestricted
scalar preparations follows. The order-four homogeneous scalar variation
leaves lower jets fixed and changes N4; the audits check this negative case.

For a propagated order-p residual the exact secular integral is
epsilon^p(e^{pOmega T}-e^{sigma T})/(pOmega-sigma), with the continuous
limit epsilon^p T e^{pOmega T}. At T=log(Astar/epsilon)/Omega a uniform
O(Astar^p) bound needs sigma<pOmega. Equality adds log(1/epsilon), and
larger sigma produces epsilon^{p-sigma/Omega}. These distinctions are
tested independently; no fixed-time constant is relabeled logarithmic
control.

Finally, with b_U an *actual* total news error, the exact inequality is

\[
{2\over r_0}\max(0,\|N_{ref}\|_U-b_U)^2
\le {E(U)\over M_0}\le {2\over r_0}(\|N_{ref}\|_U+b_U)^2.
\tag{13}
\]

No unbounded constant in (10)--(12) is assigned zero. Equations (2)--(8)
sharpen the constraint gate; (9)--(13) identify why its remaining
closure, an exact physical interval and a first-bulge energy remain open.
