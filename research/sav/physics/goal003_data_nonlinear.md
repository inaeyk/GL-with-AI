# GOAL-003: simultaneous data, higher orders, memory and the linear bulge map

2026-10-22. Input `research/sav-radiation@ef646d56ab7d8bebca578afab53e62da5146ec05`;
the 79 input blobs are in `verification/goal003_input_tree.json`. All equations
below use five-dimensional vacuum gravity, no factorials in the metric
expansion, exact transverse SO(3), and the normalized growing GL mode of
GOAL-001. The scalar stabilization extension is a separate note. No physical
ODE, spacetime evolution or parameter survey was run for this note.

**Results and limits.** We derive an exact radial constraint system, an
all-orders prescription that admits exact simultaneous GL/scalar data on
finite regular collars, a concrete asymptotic failure of a naive prescription,
and an improved prescription with an exact exterior energy cancellation.
The full asymptotically KK-flat, fixed-ADM-mass family and its bounded right
inverse remain unproved. Third/fourth-order sources and the projected memory
equation are explicit recursive/formal identities, not bounded dynamics.
An explicit *linear* apparent-horizon geometry map is obtained. There is no
quantitative finite-amplitude interval, nonlinear radiation-trace theorem,
or coverage of a finite first-bulge event.

## 1. Exact constraints on a regular slice

The initial slice is the background horizon-crossing slice
$\tau=v-r=0$. Use its radial coordinate as a foliation coordinate, not as
physical evolution time. In a local radial shift-zero spatial chart write

$$\gamma=a^2dr^2+h_{AB}dx^Adx^B,\qquad
h=b^2dz^2+R^2d\Omega_2^2,\qquad n=a^{-1}\partial_r.$$

Here $n$ is the **spatial radial** unit normal; the future spacetime unit
normal to the initial slice will be denoted $N$. Decompose the spacetime
extrinsic curvature, with $K_{ij}=-\frac12\mathcal L_N\gamma_{ij}$, as

$$K=\kappa n^\flat n^\flat+n^\flat\eta+\eta n^\flat
       +\frac q3h+\sigma,\qquad \operatorname{tr}_h\sigma=0.$$

SO(3) leaves only $\eta_z=\eta$, $K^z{}_z=v$ and
$K^\theta{}_\theta=K^\varphi{}_\varphi=w=(q-v)/2$.
Define

$$H_*={b_r\over b}+{2R_r\over R},\quad
A_*={1\over2}h_r,\quad H={H_*\over a},\quad
|A_*|^2=({b_r\over b})^2+2({R_r\over R})^2,$$
$$\mathcal R_h={2\over R^2}
 -{4\over b^2R}(R_{zz}-{b_z\over b}R_z)
 -{2R_z^2\over b^2R^2},\quad
\Delta_h a={1\over bR^2}\partial_z({R^2\over b}a_z).$$

The scalar curvature and Hamiltonian constraint are

$$\mathcal R_\gamma=\mathcal R_h+{2H_*a_r\over a^3}
 -{2\partial_rH_*+H_*^2+|A_*|^2\over a^2}-{2\over a}\Delta_ha,$$
$$\boxed{2H_*a_r=2a^2\Delta_ha-a^3(\mathcal R_h+Z)
                 +a(2\partial_rH_*+H_*^2+|A_*|^2),}\tag{1}$$
$$Z=2\kappa q+{2\over3}q^2-2|\eta|_h^2-|\sigma|_h^2.$$

The two momentum equations are exactly

$$\boxed{q_r=aD^A\eta_A+2\eta^AD_Aa
       +H_*(\kappa-q/3)-\sigma:A_*,}\tag{2}$$
$$\boxed{\partial_r\eta_A=a[D_A\kappa+\tfrac23D_Aq-D^B\sigma_{BA}]
 -H_*\eta_A+(\kappa-q/3)D_Aa-\sigma_A{}^BD_Ba.}\tag{3}$$

In scalar coordinates the last equation is

$$\eta_r=a[\kappa_z+q_z-v_z-2(R_z/R)(v-w)]-H_*\eta+(\kappa-v)a_z,$$

and $D^A\eta_A=(bR^2)^{-1}\partial_z(R^2\eta/b)$.
These equations retain every SO(3) Hamiltonian/momentum component. The
factor $2/3$ is specific to four spatial dimensions. The spatial radial
acceleration is $\nabla_n n=-D\log a$; changing this sign loses the
$2\eta^AD_Aa$ term. Direct four-dimensional coordinate Christoffels and
curvature, independent of a prescribed source, verify (1)--(3) for arbitrary
$a,b,R,\kappa,q,v,\eta$ in the retained executable.

For genuinely prescribed $h,\kappa,\sigma$, (1) is uniformly parabolic in
the radial direction on every compact collar with $H_*>0$, $a>0$;
(2)--(3) are symmetric hyperbolic. This matches the local framework of
[Rácz, Theorem 4.1 and equations (3.1)--(4.7)](https://arxiv.org/html/1508.01810v3).
The theorem supplies local smooth constraints from smooth leaf data; it does
not supply global KK-flat asymptotics or an ADM-charge prescription. Its
Theorem 4.2 concerns the lapse equation with controlled coefficients, not an
unconditional global theorem for our coupled system. We use neither theorem
to suppress those missing estimates.

## 2. Two data prescriptions and their different outcomes

The background has

$$a_0^2=1+r_0/r,\quad b_0=1,\quad R_0=r,\quad
w_0={r_0\over r\sqrt{r(r+r_0)}},\quad q_0=2w_0,\quad v_0=\eta_0=0,
\quad\kappa_0=w_0+rw_0'.$$

Restrict the exact regular growing linear GL solution and its normal
derivative to $\Sigma_0$; decompose its induced $(\gamma_1,K_1)$ into the
variables above. These are definitions of $a_1,R_1,\kappa_1,q_1,\eta_1$,
not independently adjustable velocities. First-order unitary gauge gives
$b_1=v_1=0$. The normal derivative of the complete on-shell mode fixes the
growing tangent and excludes an added decaying mode. Reflection sends
$\eta$ to an odd function of $z$; the remaining variables are even.

**Attempt 1: prescribed $\kappa,\sigma$.** Smooth higher-order free data,
with polynomial/exponential dependence on $\epsilon$, and inner-leaf
values agreeing with the GL first jet generate exact data on finite
collars by the local theorem. Differentiating the well-posed radial problem
at zero and using uniqueness recovers the full GL tangent. A radion can
be inserted at order two through $b$. This is simultaneous exact *local*
data, not nonlinear superposition with the scalar-only family.

There is a concrete obstruction to the naive asymptotic prescription that
holds $\kappa,\sigma$ at their background values outside the free source.
In its zero harmonic with $b=1,R=r,\eta=0$, (2) gives exactly

$$\partial_r(q-q_0)=-{2\over3r}(q-q_0),\qquad
q-q_0=C r^{-2/3}.\tag{4}$$

The forbidden tail is present unless its integration constant is tuned to
zero. It is much larger than the $K=O(r^{-2})$ KK-flat falloff. A compact
free source does not imply compact or adequately decaying constrained
variables. Tuning only the lapse/mass integration constant cannot remove
this independent momentum tail. This is failure of this particular
prescription, not a no-go theorem for GL data.

**Attempt 2: coupled fixed free-data rule.** Prescribe $h$, a scalar $d$,
and $v$, and set

$$\kappa=-q/2+d,\qquad K^z{}_z=v,\qquad
K^\theta{}_\theta=K^\varphi{}_\varphi=(q-v)/2.\tag{5}$$

This alters the hypotheses of the quoted theorem: $\kappa,\sigma$ are
now determined algebraically by the unknown $q$ and the free functions.
It must therefore be checked as its own coupled system. Its principal
momentum part is $q_r=(a/b^2)\eta_z$, $\eta_r=(a/2)q_z$.
The positive symmetrizer is $\operatorname{diag}(1/2,b^{-2})$; the lapse
equation remains parabolic, with $z$-principal coefficient
$\nu=a^2/(H_*b^2)>0$. Here is the needed local derivative estimate,
which is additional to a principal-symbol check. On a compact radial
interval and periodic $z$, fix a smooth background and an $H^s$ ball,
$s\ge4$, in which $a,b,R,H_*$ have positive lower bounds. The free
$h,d,v$ and their required derivatives through order $s+2$ are bounded.
For perturbations $\alpha=a-a_0$ and $W=(q-q_0,\eta-\eta_0)$, set
$E_s=\|\alpha\|_{H^s(S^1)}^2+
\sum_{j\le s}\langle\partial_z^jW,S\partial_z^jW\rangle$ with
$S=\operatorname{diag}(1/2,b^{-2})$.
Commuting the lapse equation gives the parabolic term
$\nu_0\|\alpha\|_{H^{s+1}}^2$, $\nu_0=\inf\nu$.
In the momentum energy the symmetrized principal derivatives of $W$
integrate by parts. The only extra highest derivative is a bounded
coefficient times $\partial_z^{s+1}\alpha$, from the $a_z$ terms.
Its pairing with $\partial_z^sW$ satisfies

$$C_B\|W\|_{H^s}\|\alpha\|_{H^{s+1}}
\le {\nu_0\over4}\|\alpha\|_{H^{s+1}}^2
       +{C_B^2\over\nu_0}\|W\|_{H^s}^2.$$

The other commutators contain at most $s$ derivatives of $a,W$ paired
with bounded first/second derivatives; the one-dimensional $H^s$
product estimate closes them on this ball. The free-source perturbation
is measured in $H^{s+1}$ (and the free leaf metric in $H^{s+2}$).
Consequently, after adjusting finite constants,

$$\boxed{\partial_rE_s+{\nu_0\over2}\|\alpha\|_{H^{s+1}}^2
 \le C_BE_s+C_{\rm free}\|\delta(h,d,v)\|_{s+2}^2.}\tag{5a}$$

The analogous difference estimate is taken one derivative lower to
avoid loss in quasilinear coefficients. Regularization, the bounded-ball
estimate, and this lower-norm difference estimate give the usual local
iteration/limit construction. The constants depend on the finite collar,
the chosen Sobolev ball and its positive coefficient bounds; no explicit
smallness radius is evaluated. Smooth free/leaf data permit every fixed
finite derivative by repeating the hierarchy. This establishes the local
construction for the modified closure, not a global-in-$r$ estimate or a
uniform analytic radius in $\epsilon$.

An explicit all-orders **local** prescription is as follows. Let $F,P$
be fixed smooth radial functions supported in a finite exterior annulus;
one may take a fixed nonzero bump $F$ and $P=0$. Put

$$b=\exp(2\epsilon^2F/\sqrt3),\quad
R=r\exp[\epsilon R_1/r-\epsilon^2F/\sqrt3],$$
$$d=(\kappa_0+q_0/2)+\epsilon(\kappa_1+q_1/2),\qquad
v=-2\epsilon^2P/\sqrt3.\tag{6}$$

To select **one** family from this formula, set r0=1, L=10,
r_in=3/4, the fixed target collar 3/4≤r≤4, P=0, and

$$F(r)=\begin{cases}
\exp[-1/((r-2)(3-r))],&2<r<3,\\
0,&\text{otherwise}.
\end{cases}$$

Use the exact normalized growing fundamental eigenpair on this circle
(the saved digits are illustrative, not its definition). There are no
remaining independently variable higher-order functions in this local
prescription. The smooth ingoing linear solution extends to r_in by its
regular EF equations. Restoring dimensions replaces each r by r/r0 and
L by 10r0. This choice does not solve the global charge-tuning problem.

Fix the inner leaf $r_{\rm in}<r_0$ in the smooth ingoing linear extension,
set $a=a_0\exp(\epsilon a_1/a_0)$,
$q=q_0+\epsilon q_1$, $\eta=\epsilon\eta_1$ on that leaf, and solve
(1)--(3),(5) outward. The free functions and inner values in (6) are the
entire rule: no arbitrary order-four scalar or higher independent
harmonics may be added afterward. A finite interval containing the horizon
and the scalar support is covered for sufficiently small $\epsilon$ by
local existence, continuous dependence and continuation about the regular
background on that compact interval. This supplies an exact local family,
not a numerical discretization. The allowable smallness radius is not
evaluated. The regular ingoing first jet, including its continuation to
the inner leaf, is an assumption about the exact linear eigenfunction;
the empirical saved-profile digits do not certify it.

Define on this initial chart $\phi_{\rm chart}=(\sqrt3/4)\log b^2
=\epsilon^2F$. Choose the spacetime lapse/shift extension so that
$\partial_z\beta^z=0$ on the slice; Gaussian-normal transport of the
spatial chart is one such initial choice. Since $\gamma_{rz}=0$, the
ADM identity is
$N\phi_{\rm chart}=-(\sqrt3/2)K^z{}_z+
(\sqrt3/2\mathcal N)\partial_z\beta^z$, where $\mathcal N$ is the
spacetime lapse. Thus the stated extension gives
$N\phi_{\rm chart}=\epsilon^2P$.
These are exact chart/free-data relations; in a $z$-dependent geometry
they are not a gauge-invariant nonlinear zero-mode truncation. Agreement
with the completed physical radion is through order $\epsilon^2$ in
the specified first-order unitary chart. A global nonlinear unitary/Bondi
completion is still needed for its higher-order physical interpretation.
The factor $e^{-\epsilon^2F/\sqrt3}$ in $R$
implements the leading Einstein-frame scalar scaling, while the remaining
constraint correction is solved jointly. Equations (6) have half-period
covariance; uniqueness preserves it. Fixed finite parameter derivatives
can be obtained by differentiating the local equations with enough
reserved spatial regularity. No uniform all-orders derivative bound or
analytic parameter radius is asserted. This does not prove differentiability
of an infinity radiation map. Nor does this initial rule select the
eternal exponential $n=2$ particular response: it fixes its own constrained
homogeneous data. Matching a globally particular $q_2$ would require
changing the explicitly declared order-two free/leaf coefficients after
that response is constructed.

For the ideal exterior $b=1,R=r$, rule (5) gives exactly

$$q_r=a\eta_z+2\eta a_z+{ -2q+v+2d\over r},$$
$$\eta_r=a(\tfrac12q_z+d_z-v_z)-{2\eta\over r}
                +(-q/2+d-v)a_z.\tag{7}$$

Set $Q=r^2q$, $P_r^{\rm var}=r^2\eta$; to avoid confusion with the
prescribed scalar momentum, denote the latter variable $J$. For $d=v=0$,

$$Q_r=aJ_z+2Ja_z,\qquad J_r={a\over2}Q_z-{Q\over2}a_z,$$
$$\boxed{\frac d{dr}\int_0^L(\tfrac12Q^2+J^2)dz=0.}\tag{8}$$

The identity uses periodic integration by parts and holds even for
$z$-dependent $a$. Thus the $r^{-2/3}$ mode is removed in this exterior
control. With nonzero free $d,v$, the energy derivative is the integral
of $Q\,r(v+2d)+2J[ar^2(d_z-v_z)+r^2(d-v)a_z]$.
The background $d_0=\kappa_0+q_0/2$ decays as $O(r^{-3})$;
its first term is integrable for bounded $Q$. The GL free tails are
exponential. General $b,R$ add explicitly calculable decaying
coefficient errors, not identically zero terms.

The lapse equation in this exterior is

$$a_r={r\over2}a^2a_{zz}+{a-a^3\over2r}-{r\over4}a^3Z,$$
$$Z=-q^2/2+2dq+qv-3v^2/2-2\eta^2.\tag{9}$$

At a minimum of $u=a^{-2}$,
$u_r\ge(1-u)/r+(r/2)\inf_zZ$. If a proved Sobolev bound gave
$Z\ge-C/r^4$ for $r\ge R_m$, comparison would give

$$u_{\min}(r)\ge1+{R_m[u_{\min}(R_m)-1]
                 -(C/2)(R_m^{-1}-r^{-1})\over r}.\tag{10}$$

This is a useful explicit lower comparison, not an established GL bound:
(8) is only a zeroth-order estimate, does not bound pointwise $Z$ in the
needed differentiated system, and (10) does not bound all parabolic
derivatives. A complete exterior proof needs high-order commutator,
parabolic smoothing and asymptotic-expansion estimates with integrable
constants. Those estimates have not been supplied. No third repair or
radial numerical campaign was launched.

**Charges and right-inverse gap.** A full family must continue to infinity,
have fixed circle/clock, and implement $M_{\rm ADM}(\epsilon)=M_0$ and
zero KK momentum. The construction above fixes reflection/KK momentum
parity and the free asymptotic circle, but has not proved the ADM limit.
A possible mass parameter is an even constant $\lambda(\epsilon)$ in
the inner-leaf exponent for $a$. Solving for it requires a differentiable
global map $M(\epsilon,\lambda)$ and a nonzero lower bound on
$|\partial_\lambda M|$; neither is proved. There is no independently
adjustable regular tension hair. This is a declared target policy, not
an accomplished charge prescription.

More invariantly, for the linearized constraint map $D\mathcal C$ and
a background Killing initial datum $(N_K,X_K)$ in its formal adjoint kernel,

$$\int_\Sigma\langle(N_K,X_K),D\mathcal C(\delta\gamma,\delta K)\rangle
 =\mathcal B_\infty-\mathcal B_{\rm in}.\tag{11}$$

At higher orders the quadratic/higher source must equal this boundary
pairing. Fixing both boundaries and all charges arbitrarily can violate
it. The time-translation KID tests mass; circle translation tests momentum.
The horizon-crossing inner leaf is an interior boundary, not a vanishing
boundary term by decree. A right-inverse estimate in weighted spaces
modulo these charge directions is still missing. Consequently **Gate C2
is open**, despite the exact local family and a fixed local all-orders rule.

## 3. Explicit higher-order source recursion

All derivatives and index operations in this paragraph use $\bar g$.
Let $h_1=h$, $h_2=q_2$, $h_3=q_3$, $h_4=q_4$, and
$U_j{}^A{}_B=\bar g^{AC}h_{j,CB}$. Define

$$C_j{}^A{}_{BC}={1\over2}\bar g^{AD}
(\bar\nabla_Bh_{j,DC}+\bar\nabla_Ch_{j,DB}-\bar\nabla_Dh_{j,BC}).$$

The connection-difference coefficients are

$$D_1=C_1,\qquad D_2=C_2-U_1C_1,$$
$$D_3=C_3-U_1C_2+(U_1^2-U_2)C_1,$$
$$D_4=C_4-U_1C_3+(U_1^2-U_2)C_2
 +(-U_3+U_1U_2+U_2U_1-U_1^3)C_1.\tag{12}$$

The matrices do not commute: replacing $U_1U_2+U_2U_1$ by
$2U_1U_2$ fails the retained exact matrix control. Define

$$\mathscr D(D)_{AB}=\bar\nabla_CD^C{}_{AB}-\bar\nabla_BD^C{}_{AC},$$
$$\mathscr B(D,E)_{AB}=D^C{}_{CD}E^D{}_{AB}-D^C{}_{BD}E^D{}_{AC}.$$

With $\widehat D_j=D_j-C_j$, the actual cubic and quartic sources are

$$Q_3=\mathscr D(\widehat D_3)+\mathscr B(D_1,D_2)+\mathscr B(D_2,D_1),$$
$$Q_4=\mathscr D(\widehat D_4)+\mathscr B(D_1,D_3)
                         +\mathscr B(D_2,D_2)+\mathscr B(D_3,D_1),$$
$$R'[q_{3,n}]=-\Pi_nQ_3\ (n=\pm1,\pm3),\qquad
R'[q_{4,n}]=-\Pi_nQ_4\ (n=0,\pm2,\pm4).\tag{13}$$

These are explicit local differential polynomials, rather than a missing
source named by a symbol: (12) fixes every index contraction and ordering.
They include $h_1q_2,h_1^3$ at order three and
$h_1q_3,q_2^2,h_1^2q_2,h_1^4$ at order four. They are not solved ODEs or
bounded retarded integrals. Every Fourier projector uses the complex
convolution $\Pi_n(FG)=\sum_jF_jG_{n-j}$, with $H/2$ at $n=\pm1$.
The displayed harmonic lists assume the independent free harmonics vanish;
half-period covariance alone allows, for example, $\epsilon^2\cos6kz$.

Gauge completion can be generated without omitting ordered Lie terms by
the coefficient rule

$$g^U_j=[\epsilon^j]\exp\!\left(\sum_{m\ge1}\epsilon^m
\mathcal L_{\xi_m}\right)
\left(\bar g+\sum_{m\ge1}\epsilon^mh_m\right).\tag{14}$$

At each order choose the nonzero-$n$ compensator to remove $zA$ components,
then the declared zero-mode gauge/clock/phase conditions. This includes
$\frac12\mathcal L_{\xi_1}^2\bar g$ at order two and both orderings
$\frac12(\mathcal L_{\xi_1}\mathcal L_{\xi_2}+
\mathcal L_{\xi_2}\mathcal L_{\xi_1})\bar g$ at order three.
The coefficient rule is exact algebra; the global small-diffeomorphism
existence and norm estimates are separate requirements.

In this recursively completed chart the nonzero Fourier coefficients of
$g_{zz}$ vanish, and reflection fixes the zero-mode vector. Write
$g^U_{zz}=1+\epsilon^2\chi_2+\epsilon^4\chi_4+\cdots$. Then

$$\phi_2={\sqrt3\over4}\chi_2,\qquad
\phi_4={\sqrt3\over4}(\chi_4-\tfrac12\chi_2^2),$$
$$\boxed{\bar\Box\phi_4={\sqrt3\over2}\Pi_0Q_{4,zz}
                         -{\sqrt3\over8}\bar\Box(\chi_2^2).}\tag{15}$$

The second term is mandatory and contains both $\chi_2\bar\Box\chi_2$
and $(\bar\nabla\chi_2)^2$. At finite radius this is a particular
completed field definition in a $z$-dependent geometry, not a claim that
zero modes alone consistently truncate the tower. At infinity one must
also transform to Einstein-frame Bondi radius and retarded time. If the
old coordinates expressed in Bondi coordinates are
$x^\mu=X_0^\mu(u,\rho)+\epsilon^2X_2^\mu(u,\rho)$, then

$$\Phi_4^B=\lim_{\rho\to\infty}\rho
 [\phi_4(X_0)+X_2^\mu\partial_\mu\phi_2(X_0)],\qquad N_4=\partial_u\Phi_4^B.
\tag{16}$$

Terms exponentially decaying with nonzero $n$ must be shown negligible
in this limit; an asymptotic clock shift is not silently set to zero.
For $u'=u+\epsilon^2a(u)$, (16) gives
$N_4'=N_4-aN_2'-a'N_2$. An arbitrary free order-four scalar changes $N_4$
without changing $h_1,q_2$. A fixed exact family is therefore essential.

The scalar current first carries energy at order four:
$P=\epsilon^4N_2^2/G_4+2\epsilon^6N_2N_4/G_4+\cdots$.
Thus the order-four Bondi balance is $dM_B^{[4]}/du=-N_2^2/G_4$;
$N_4$ first changes its energy at order six. Horizon loss and stored
exterior energy enter the spacetime energy balance; they are not extra
Bondi-news terms. No massive KK null-infinity flux is added.

## 4. What a remainder theorem would have to prove

Use the nondegenerate regular-frame Sobolev/Frobenius and exterior weighted
norms $X$ in GOAL-002, not its horizon-degenerate scalar Killing norm.
An exact evolution requires a horizon-regular hyperbolic gauge and control
of the gauge constraints; if a reduced wave gauge is used, the contracted
Bianchi identity yields a homogeneous wave equation for its constraint
vector only after the full constraints and boundary data vanish. This
propagation identity is not a complete exterior redshift/trapping/trace
estimate. No such nonlinear estimate is proved here.

For an exact half-period-covariant family with a $C^4$ radiation map on
a fixed observation window $U$, Taylor's theorem would give

$$\|N_\epsilon-\epsilon^2N_2\|_{L^2(U)}
 \le {\epsilon^4\over24}\sup_{|a|\le|\epsilon|}
 \|\partial_a^4N_a\|_{L^2(U)}.\tag{17}$$

A $C^6$ map gives the analogous $\epsilon^6/720$ remainder after $N_4$.
The derivatives, their existence at null infinity, and their constants
are open. They cannot be inferred from local data smoothness. If their
bound were $b_U$, the exact safe energy enclosure would be

$${2\over r_0}\max(0,\|N_p\|_U-b_U)^2
\le E_U/M_0\le {2\over r_0}(\|N_p\|_U+b_U)^2.\tag{18}$$

Here $b_U$ must include independent scalar transients/interference, input
mode and overlap errors, inverse/reconstruction errors, higher data,
nonlinear evolution and extraction/clock errors. Strict positive energy
needs $b_U<\|N_p\|_U$, not just an upper bound. Existing empirical L10
$C_0,Q_\Sigma$ numbers are not replacements for certified bounds.

There is an important instability-time constraint on all proposed
constants. A residual bounded by $C\epsilon^pe^{p\Omega t}$, propagated
by $C_Se^{\sigma(t-s)}$, contributes

$$CC_S\epsilon^p{e^{p\Omega T}-e^{\sigma T}\over p\Omega-\sigma}
\quad(p\Omega\ne\sigma),\qquad
CC_S\epsilon^pTe^{p\Omega T}\quad(p\Omega=\sigma).\tag{19}$$

At $T=\Omega^{-1}\log(A_*/\epsilon)$ this is uniformly $O(A_*^p)$
only if $\sigma<p\Omega$ (with the displayed positive denominator).
Equality introduces $\log(1/\epsilon)$; $\sigma>p\Omega$ gives a factor
$\epsilon^{p-\sigma/\Omega}$ that can diverge. Finite-time existence
alone therefore does not close a logarithmic instability window. An
input growth-rate uncertainty $\delta\Omega$ additionally produces
$e^{|\delta\Omega|T}-1$, which must be included in the news budget.

The unclosed sufficient inequalities remain

$$A_*M_1+A_*^2M_2+R_{\ge3}(A_*,T)<\eta_g<1,\qquad
A_*M_2\le\eta_hm_1,$$
$$C_TC_{\rm NL}(A_*M_1+A_*^2M_2+R_{\ge3})<1,$$
$$\eta_{\rm trans}+\eta_{\rm input}+\eta_{\rm inverse}
 +\eta_{\rm nonlinear}+\eta_{\rm extraction}\le\sqrt{1.1}-1.\tag{20}$$

Open constants are the global charge-fixed constraint inverse, $M_2$,
nonlinear evolution $C_T,C_{\rm NL}$ including derivative losses,
redshift/radiation trace, global gauge/coordinate completion, and the
fourth/sixth radiation derivatives. Normalization supplies a lower
$m_1=2$; the sampled first-order maximum is not an upper $M_1$.
No positive numerical $A_*$ or time window is certified by (20).

## 5. Derived memory identity and conditions for a local cubic law

Suppose a regular constraint/gauge-completed evolution has first-order
form $\dot U=LU+B(U,U)+C(U,U,U)+\cdots$, with symmetric multilinear
$B,C$ in this no-factorial convention. Let $e$ be the fixed normalized
growing eigenstate, $Le=\Omega e$, and let an adjoint functional satisfy
$\ell L=\Omega\ell$, $\ell e=1$. Put $P=e\ell$, $Q=1-P$,
$A=\ell U$, $W=QU$. A bounded adjoint projection and the indicated
propagator are hypotheses, not consequences of a fitted eigenfunction.

Parity kills $\ell B(e,e)$. To second order, with $F=QB(e,e)$,

$$W_2(t)=e^{(t-t_0)L_Q}W_2(t_0)
 +\int_{t_0}^t e^{(t-s)L_Q}F A(s)^2ds.$$

Substitution into the amplitude projection derives the cubic memory law

$$\begin{aligned}
\dot A={}&\Omega A+c_3A^3
 +2A\ell B(e,e^{(t-t_0)L_Q}W_2(t_0))\\
 &+2A(t)\int_0^{t-t_0}K(v)A(t-v)^2dv+O_{\rm formal}(A^5),\\
K(v)={}&\ell B(e,e^{vL_Q}F),\qquad c_3=\ell C(e,e,e).
\end{aligned}\tag{21}$$

The symbol $O_{\rm formal}$ is expressly unbounded and denotes perturbation
degree at least five in the fixed family, not a uniform bound by the
instantaneous $A^5$ for arbitrary independent data or times. The quadratic sector
includes zero-mode scalar/metric/charge channels and $n=\pm2$; charge
zero eigenvalues and continuum tails are retained in $e^{vL_Q}$.
Equation (21) is a derived kernel expression with explicit data dependence,
not an evaluated physical kernel or a nonlinear reduction theorem.

On an exactly exponentially growing history, a candidate cubic coefficient
is

$$\gamma=c_3+2\int_0^\infty e^{-2\Omega v}K(v)dv
=c_3+2\ell B(e,(2\Omega-L_Q)^{-1}F),\tag{22}$$

provided the integral/resolvent exists in the required norms and the
initial transient has been bounded. The static replacement
$(-L_Q)^{-1}$ is different and may not exist at charge/continuum zero
frequency. For a checkable scalar model
$\dot y=-\lambda y+bA^2$, $\dot A=\Omega A+cAy$, the exponential
response coefficient is $bc/(\lambda+2\Omega)$, not the static
$bc/\lambda$. Neither number is the GL coefficient.

The local-reduction error can be made precise without assuming it small.
Writing $B_g(t)=e^{-2\Omega t}A(t)^2$ and
$L_g=\sup_{s\in[t_0,t]}|\dot B_g(s)|$, the history replacement obeys

$$\left|\int_0^{t-t_0}K(v)
[A(t-v)^2-e^{-2\Omega v}A(t)^2]dv\right|
\le e^{2\Omega t}L_g\int_0^{t-t_0}v e^{-2\Omega v}|K(v)|dv.\tag{23}$$

The omitted past adds
$A(t)^2\int_{t-t_0}^\infty e^{-2\Omega v}|K(v)|dv$.
Both moments and the independent-data term must be controlled before
claiming (22). A spectral gap is sufficient in some settings but has not
been proved here; the Laplace weight can give convergence without a gap.
Under $B=A+cA^3$, $d\tau'/dt=1+dA^2$,
$\gamma_B=\gamma+2c\Omega-d\Omega$. A fixed clock and amplitude
prescription remain essential. No static nonuniform-string coefficient
is used as a dynamical $\gamma$.

## 6. A new exact linear apparent-horizon bridge

This subsection uses $r_0=1$, $k=2\pi/10$, and the regular
$\tau=v-r$ slice. A marginally outer trapped surface is a graph
$r=1+\epsilon\rho(z)+\cdots$. Its outward expansion is
$\theta_+=H_{\cal S}-\operatorname{tr}_{\cal S}K$ with the above sign
for $K$. For the background,

$$\theta_+(r)={2(r-1)\over r\sqrt{r(r+1)}},\qquad
\delta_{\rho}\theta_+=\sqrt2(-\partial_z^2+1)\rho.\tag{24}$$

The graph displacement has physical normal length $a_0\rho$, producing
the Laplacian term. There is no background radial-tangent $K$ drift. The
coordinate-graph stability operator has positive eigenvalues
$\sqrt2(1+n^2k^2)$ in this SO(3) sector and inverse $L^2$ norm at most
$1/\sqrt2$. Restoring dimensions gives
$\sqrt2(-\partial_z^2+r_0^{-2})$ and bound $r_0^2/\sqrt2$.

Let $T=\delta g_{vv}$ and $S=\delta g_{\theta\theta}$ denote the
regular, unit-$A(\tau)$ coefficients at the horizon. The full ADM
lapse/shift variation, including $\delta\gamma_{rr}$, gives

$$\delta_{\rm data}\theta_+=\sqrt2(\Omega S-T),\qquad
\rho_1={T-\Omega S\over1+k^2}.\tag{25}$$

All radial derivatives and radial-metric terms cancel in this combination;
setting them to zero before the ADM calculation is not the derivation.
The retained exact check verifies the cancellation for independent
$\delta g_{\tau\tau},\delta g_{\tau r},\delta\gamma_{rr},S,S_r$.

For the ingoing master normalized at $r=2$, write
$y\sim a_h(r-1)^\Omega$. The independently audited GOAL-002 horizon
limits are

$$T={a_h\Omega(1-2\Omega)\over2k^2(1+k^2)},\qquad
S=-{a_h(k^2+2\Omega)\over2k^2(1+k^2)}.$$

The physical sphere radius on the displaced surface includes both pieces:

$$\boxed{\delta R_{\rm AH}=S/2+\rho_1
                   =-{a_h\over4(1+k^2)}.}\tag{26}$$

Thus a leading areal bulge/neck contrast
$\mathcal B=(R_{\max}-R_{\min})/(R_{\max}+R_{\min})$ is
$|A(\tau)a_h|/[4(1+k^2)]+O_{\rm formal}(A^2)$.
The master horizon coefficient $a_h$ is fixed by the reference-radius
normalization and requires a certified connection to that radius for a
quantitative enclosure. No numerical value is inferred from the old
sampled norm. This is an apparent-horizon, slice-specific linear map,
not an event-horizon result. A nonlinear MOTS implicit-function estimate
needs metric/derivative control; its linear inverse alone does not supply
the remainder or guarantee outermostness.

One may define a first-bulge event by a fixed contrast threshold
$\mathcal B=\mathcal B_c>0$ together with outermost smooth MOTS conditions,
but (26) reaches no such fixed event with a controlled error. A data-to-MOTS
error, continuation criterion and source-time-to-retarded-window map are
still missing. Therefore no first-bulge flux fraction, early-dominance
denominator or $A\sim1$ extrapolation follows.

## 7. Verification and precise stopping point

Executed, exit 0:

```sh
python3 research/sav/verification/goal003_data_checks.py > research/sav/verification/goal003_data_checks_results.txt
```

Python 3.14.8, SymPy 1.14.0. The script reports 25 exact/manufactured
checks including six deliberately wrong cases. It directly checks full
four-dimensional spatial constraints and background data, the bad tail, modified-rule energy
cancellation and principal symmetrizer, ordered inverse recursion,
harmonic/parity rules, the AH stability potential/cancellations, secular integral and
quartic log completion. These are primary checks; Audit II is separate.
No floating physical solve or simulation was performed.

The strongest achieved data result is exact local existence with a fixed
free-data rule, **not** one global charge-fixed GL family. Formal third/
fourth dynamics and the memory kernel have been advanced through the
open lower bounds without promoting their physical consequences. A concrete
next strategy is to prove weighted high-Sobolev exterior estimates for
(5)--(9), obtain the ADM mass-map derivative and inner-boundary tuning,
then combine the exact global data with the forced-sector inverse and
a nondegenerate hyperbolic evolution/radiation-trace estimate. The two
prescriptions above exhaust this note's two distinct data repair routes;
their failure to close the global theorem is not mathematical impossibility.
