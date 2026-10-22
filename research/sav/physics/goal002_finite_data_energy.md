# GOAL-002: finite data, an energy inequality, and the amplitude obstruction

2026-10-22. Conventions and geometric equations:
[goal002_second_order.md](goal002_second_order.md). The finite-A GL
interval and first-bulge fraction are **not established**. The most useful
new quantitative result is a scalar-linear transient budget on a
horizon-crossing slice, with a three-configuration L10 diagnostic.
The exact free-scalar constraint construction is separate from the still
unproved simultaneous nonlinear GL-plus-scalar construction.

## 1. Finite-time prescription and its exactness boundary

Choose the smooth slice $\Sigma_0:\tau=v-r=0$ crossing the future horizon;
$r_0=1$ below unless dimensions are shown. Prescribe the full first-order
metric and its normal derivative by restricting the genuine on-shell,
ingoing GL eigenmode to this slice. Its growing projection is epsilon
under the r=2 normalization; no decaying GL mode is added. This is
restriction of a solution, not temporal switching. Set all independent
second-harmonic homogeneous data to zero in a specified retarded problem,
after their constraints are solved; their forced values cannot be set
to zero arbitrarily. Fix the order-epsilon-squared ADM shift to zero,
circle length L, asymptotic clock and zero KK momentum.

At second order the following two scalar choices are both allowed:

- **P (particular-matched):** $\psi=r\phi_2=e^{st}q(x)$ and its normal
  derivative restricted to $\Sigma_0$, where $L_sq=S$ is GOAL-001's
  future-regular exponential particular solution, $s=2\Omega$.
- **Z (zero scalar data):** $\psi=0$ and normal derivative zero on
  $\Sigma_0$, evolved by the same on-shell GL quadratic source.

Their difference is the homogeneous scalar solution with initial data
minus those of P. For either scalar choice the zero-mode Einstein-frame
metric obeys the same source constraints through order epsilon squared.
The 5D conformal/radion metric pieces differ. Any extra compactly
supported scalar datum gives another admissible **second-order** choice.
There is no unique selection from a no-incoming condition alone on future
boundaries; outgoing waves on $\Sigma_0$ remain free. P's data encode the
infinite-past particular response, although the problem is posed at finite
time. They are not proof that arbitrary preparation produces that response.

**Unclosed exact-data gate:** equations solved to order epsilon squared
are not an exact nonlinear vacuum constraint solution. This task has not
constructed a bounded right inverse for the z-dependent constraint map,
including its charge/Killing kernel, horizon and asymptotic conditions.
The physical simultaneous GL-plus-scalar data family is therefore a
specified perturbative prescription with an explicit completion problem,
not a finished exact finite-A construction. No field obtained by adding
two exact nonlinear solutions is claimed to solve the constraints.

## 2. A constructive exact scalar freedom control

An exact auxiliary family proves the scalar freedom is physical. It does
not contain the GL harmonic and cannot fill the gap just stated. In the
uncharged consistent KK reduction use $R_{\mu\nu}=2\phi_{,\mu}\phi_{,\nu}$.
On $\Sigma_0$ let

$$\gamma=a^2dr^2+r^2d\Omega^2,\quad
K^i{}_j=\operatorname{diag}(k_R,k_T,k_T),\quad\Pi=n\phi,\quad
K_{ij}=-\tfrac12\mathcal L_n\gamma_{ij},\quad
m=\tfrac r2(1-a^{-2}).$$

Direct Hamiltonian and momentum constraints reduce exactly to

$$k_R=k_T+rk_T'-r\Pi\phi',\qquad
m'+r\phi'^2m=\tfrac{r^2}{2}(\Pi^2+\phi'^2-2k_Rk_T-k_T^2). \tag{1}$$

Schwarzschild background data in this slicing are

$$a_0^2=1+r_0/r,\quad k_{T0}=\frac{r_0}{r\sqrt{r(r+r_0)}},\quad
m_0=\frac{rr_0}{2(r+r_0)}.$$

Choose $F,P\in C_c^\infty((R_1,R_2))$, $r_0<R_1<R_2$,
$\phi=\delta F$, $\Pi=\delta P$, $k_T=k_{T0}$.
Writing $I(r)=\delta^2\int_{R_1}^r sF'^2ds$,

$$\Delta m(r)=\delta^2e^{-I(r)}\int_{R_1}^r e^{I(s)}S_2(s)ds,
\quad S_2=\tfrac{r^2}{2}[(P+rk_{T0}F')^2+fF'^2]\ge0. \tag{2}$$

With $B=\int S_2dr$, $0\le\Delta m\le\delta^2B$, and the explicit
sufficient nondegeneracy condition is

$$\delta^2<\frac{R_1^2}{2B(R_1+r_0)}. \tag{3}$$

The data coincide with Schwarzschild near the future horizon and are
asymptotically KK flat after the exact 5D lift. They describe a regular
exterior/horizon-crossing region; no nonsingular black-hole interior is
asserted. Scalar pulse stress and ADM shift are O(delta squared).
Taking $\delta=\epsilon^2$ puts that shift at order epsilon to the fourth.
Fixed ADM mass can instead be imposed by a compensating regular inner mass
shift: the radial ODE has positive derivative
$\partial m_\infty/\partial m_{inner}=e^{-I(\infty)}$.
This is an **exact scalar-only** interval, not a GL A_* or radiation bound.
Audit II gives the lift including extrinsic curvature and independently
checks (1)-(3) from Christoffels rather than importing this note's equations.

## 3. Non-universality theorem, with its hypotheses exposed

For every constraint-compatible second-order GL solution, add any smooth
homogeneous zero-mode scalar $\varphi_b=b\varphi_f$ by

$$\Delta q_{\mu\nu}=-2b\varphi_f\bar g_{\mu\nu}/\sqrt3,
\qquad\Delta q_{zz}=4b\varphi_f/\sqrt3,\qquad\bar\Box\varphi_f=0. \tag{4}$$

This solves **all** homogeneous linearized 5D equations, changes no
first-order GL field and adds no second-order Einstein-frame stress.
The scalar is physical: (2) realizes its independent tangent in exact
vacuum KK data. The completed asymptotic news consequently has the
formal second-order form

$$N_{\epsilon,b}=\epsilon^2(n_{GL}+bn_f)+O(\epsilon^4),$$
$$\frac{E_U}{M_0}=\frac{2\epsilon^4}{r_0}
\left[\|n_{GL}\|_U^2+2b\langle n_{GL},n_f\rangle_U
 +b^2\|n_f\|_U^2\right]+O(\epsilon^6). \tag{5}$$

Take $n_f$ with nonzero outgoing trace on U. Such homogeneous waves exist;
the exterior scalar scattering/radiation map is nonzero. A pulse can be
placed so its causal outgoing signal intersects the window. Equation (5)
proves leading-energy **non-universality within the constrained
second-order solution class** at fixed first-order GL projection. It is
conditional for exact simultaneous nonlinear GL families on constraint
integrability and differentiability of the finite-window radiation map.
Those hypotheses were not proved here. It is not a theorem of GL-source
cancellation, nor an unconditional exact GL finite-A energy prediction.

The cross term in (5) prohibits a unique positive split of measured energy
into a generated part and a free part. A data prescription defines the
baseline waveform; source linearity defines a waveform split, with
interference retained in the energy. A stationary mass/tension adjustment
has zero news and cannot remove this freedom. Half-period translation
$z\mapsto z+L/2$ maps epsilon to minus epsilon; (4) can be chosen even in
epsilon and all zero-mode quantities remain even. If the independent data
family breaks this covariance, the even-power assertion must be revisited.

## 4. Integrated transient control on the regular slice

For a homogeneous difference $\psi_f=r\varphi_f$,
$\psi_{tt}-\psi_{xx}+V\psi=0$, $V=fr_0/r^3\ge0$.
Let $F_\Sigma(x)=\psi_f|_{\Sigma_0}$ and
$P_\Sigma(x)=\partial_t\psi_f|_{\Sigma_0}$. The positive Killing-energy
flux through the sloping slice is

$$Q_\Sigma[F_\Sigma,P_\Sigma]=\tfrac12\int_{-\infty}^\infty
\left[(1-r_0^2/r^2)P_\Sigma^2+(\partial_xF_\Sigma)^2+VF_\Sigma^2\right]dx.
\tag{6}$$

To derive it write the slice as $t=-h(x)$, $h'=r_0/r$.
The energy current is $(e,-\psi_t\psi_x)$ with
$e=(\psi_t^2+\psi_x^2+V\psi^2)/2$ and
$F_\Sigma'=\psi_x-h'\psi_t$; its flux is $e-h'\psi_t\psi_x$.
Positive horizon flux and the null-infinity limit imply

$$\boxed{\|N_f\|_{L^2(\mathscr I^+)}^2\le Q_\Sigma,\quad
\|N_f\|_{L^2(U)}\le\sqrt{Q_\Sigma}.} \tag{7}$$

Units: Q has length, $Q/G_4$ is physical scalar energy, $N$ is
dimensionless. There is no additional null-flux half. On t=constant slices
the equivalent scalar-energy reduction has boundary term
$-\partial_x(f\psi^2/r)$, which vanishes under the stated decay/regularity.
Equation (6) is degenerate at the future horizon; it controls integrated
news but not all regular-frame derivatives. No pointwise Price-law decay
or nonlinear coercive estimate is asserted. A retarded window at infinity
must use the fixed $u=t-x$ clock, not tau or a near-horizon formation time.
For localized initial support the first causal arrival is obtained from
$u_\Sigma=t_\Sigma-x$; backscattering adds tails. P/Z mismatch is not
compactly supported, so it has no single sharp onset time.

For P versus Z in r0=1 units,

$$F_\Sigma=e^{-s\log\delta}q(x),\quad P_\Sigma=sF_\Sigma,
\quad F_\Sigma'=e^{-s\log\delta}(q_x-sq/r). \tag{8}$$

Both are regular at the future horizon. These actual data, rather than a
time-switched on-shell metric, define the transient difference.

## 5. Bounded L10 reconstruction and its error classes

`verification/goal002_scalar_response.py` uses the saved final GL profile
without a new GL root solve. It reconstructs the contraction in 65-digit
arithmetic with an eighth-order near-horizon recurrence, interpolates the
saved p/log-u functions, integrates two direct **linear scalar** basis
equations, and accumulates both terms of the Green solution. It imports
neither the historical scalar basis routine nor its tensor contraction.
It uses exactly three scalar configurations:

| log delta left | scalar outer r | samples | relative ODE tolerance | C0 | Q for unit P/Z mismatch |
|---:|---:|---:|---:|---:|---:|
| -16 | 80 | 4001 | 2e-8 | 12.3072932071 | 80.8178880546 |
| -22 | 120 | 8001 | 2e-10 | 12.3072920853 | 80.8179175178 |
| -28 | 180 | 16001 | 2e-12 | 12.3072919789 | 80.8179175854 |

The finest C0 agrees with the historical primary midpoint to about 7e-9;
this is another saved-profile consistency check, not new independent
continuum digits. The source beyond saved GL r=65 is set to zero and its
exponential tail has not been rigorously enclosed. The right scalar
endpoint is the first positive Volterra/Born iterate. The analytic bound
on its w value remainder is $e^J-1-J$, $J=1/(4sR^2)$, about 1.86e-9 at
R=180. This is **not** the full C0 or Q error.

The matching Volterra derivative remainder is bounded by
$(e^J-1)E_1$ for exact E1, about 3.86e-11 at the finest endpoint.
The separate E1 numerical quadrature error estimate there is 3.66e-17;
it is recorded separately rather than suppressed by the tail inequality.

The finest W drift is 4.46e-10; inner Q omission leading diagnostic is
1.65e-10; outer density endpoint is 5.19e-19. These monitor floating ODE,
domain and quadrature behavior. Saved-profile interpolation, GL mode
error, horizon recurrence remainder, integrated derivative/tail errors
and outward rounding are not rigorously bounded. No fourth configuration
or other physical k was used. The machine-readable JSON and stdout record
every configuration. The Q estimate is deliberately quoted as an estimate,
not an upper bound or a nonlinear physical energy.

## 6. Strongest conditional finite-window news, energy and spectrum

For P, at second order only, $N_p=sr_0C_0 A(u)^2$ and

$$\|N_p\|_U^2=\Omega r_0^2 C_0^2(A_2^4-A_1^4),\quad
\frac{E_{p,U}}{M_0}=2\hat\Omega C_0^2(A_2^4-A_1^4). \tag{9}$$

For Z the same forced response adds the homogeneous solution from (8).
For any full-news error $\Delta N$, exact Cauchy-Schwarz gives

$$\frac{|E_U-E_{p,U}|}{E_{p,U}}\le2\eta+\eta^2,\qquad
\eta=\frac{\|\Delta N\|_U}{\|N_p\|_U}. \tag{10}$$

A sufficient condition for ten-percent accuracy relative to the predicted energy is
$\eta\le\sqrt{1.1}-1\simeq0.04880885$. An epsilon-squared mismatch
contributes at most $\epsilon^2\sqrt{Q_\Sigma}$ to the numerator.
For $A(u)=\epsilon e^{\Omega u}$, $U=[0,T]$, imposing this transient-only
criterion yields

$$ T\ge\frac1{2s}\log\left(1+
\frac{2Q_\Sigma}{\eta^2s r_0^2 C_0^2}\right). \tag{11}$$

In r0=1 units the empirical Q and C0 give T about **32.2843 r0**;
the amplitude grows by about 7.71 across this window. Equation (11) uses
the global transient budget, so it is conservative and needs no tail
decay theorem. If A_* were known, $\epsilon e^{\Omega T}\le A_*$ would
be an additional essential condition. It is **not known**. Equation (11)
neither certifies a physical settling interval nor leaves any error
budget for nonlinear/reconstruction errors at equality. P avoids this
particular scalar transient through order two but does not remove the
nonlinear/initial-data completion gates.

For actual finite-energy news and smooth observation taper
$w\in C_c^\infty(U)$, the mathematically meaningful spectrum is

$$\frac{dE_w}{d\omega}=\frac{|\int e^{i\omega u}w(u)N(u)du|^2}{\pi G_4},
\quad\int_0^\infty d\omega\frac{dE_w}{d\omega}
=\frac1{G_4}\int w^2N^2du. \tag{12}$$

Use news for scalar memory. P's leading template is the smooth transform
of $w sr_0C_0\epsilon^2e^{su}$; Z also needs its genuine scattering
transient. No full Z waveform or physical spectrum has been computed here.
A smooth Gaussian-news memory control in Audit II checks Parseval and
the interference term, with analytic tails; it is explicitly manufactured.
No sharp window or artificial time switch is used as a physical spectrum.
For each frequency, $|\widetilde{w\Delta N}|\le\|w\|_2\|\Delta N\|_2$;
this supplies an absolute spectral-error functional when that norm is
bounded, not a relative bound at spectral zeros.

## 7. Quantified norm and the exact missing inequalities

Fix the regular tetrad in the geometric note, a matching radius
$R_m=20r_0$, a finite tau interval and integer $m\ge6$.
Let $D$ denote all regular tetrad spatial and normal derivatives, and
$d\widehat\Sigma=d\Sigma/r_0^4$ (the slice includes the circle). An explicit
positive near-region norm is
$$\|X\|_{near}=\sup_{\tau\in[0,T]}\left[
\sum_{j=0}^m\|(r_0D)^jX_{\hat A\hat B}\|_{L^2(d\widehat\Sigma,r\le R_m)}
+\sup_{r\le R_m,z}\bigl(|X|_F+|r_0DX|_F\bigr)\right],$$
where F denotes the Euclidean Frobenius norm of all tetrad entries,
including both sphere directions. Derivative multi-indices are summed.
For the exterior define $Z_{\hat A\hat B}=(r/r_0)X_{\hat A\hat B}$ and
$$\|X\|_{ext}^2=\sup_{\tau\in[0,T]}\sum_{j=0}^{m-1}
\int_{x(R_m)}^\infty\frac{dx}{r_0}\int_0^L\frac{dz}{L}
\left[|r_0\partial_t(r_0D)^jZ|_F^2
+|r_0\partial_x(r_0D)^jZ|_F^2+|r_0\partial_z(r_0D)^jZ|_F^2
+(1+r/r_0)^{-2}|(r_0D)^jZ|_F^2\right].$$
The constant 1/r mass field is allowed by the last integrable weight.
These positive component norms define a bookkeeping Banach/Sobolev
space, not a claimed conserved gravitational energy. Matching traces at
Rm are included with their usual Sobolev norms. Denote their sum X and
the scalar news space $Y=L^2(U,du/r_0)$. The near-region Sobolev constants
are finite on this compact collar, but their values, the exterior
constraint inverse and the trace constants have not been supplied.
The degenerate Killing energy (6) alone is not X.

A proposed controlled single-amplitude family must satisfy, for example,

$$\begin{aligned}
& A_{max}M_1+A_{max}^2M_2+K_3A_{max}^3<\eta_g<1,\\
& A_{max}M_2\le\eta_h m_1,\\
&\eta_{trans}+K_4A_{max}^2+\eta_{mode}+\eta_{ODE}
 +\eta_{extract}\le\sqrt{1.1}-1, \tag{13}
\end{aligned}$$

where $M_j$ are upper bounds on completed order-j fields in X,
$0<m_1\le\|h_1\|_X\le M_1$, and $K_4$ bounds the
first omitted even scalar **news relative to the leading news on U**.
For a unit reference profile the exact sphere normalization gives the
safe lower bound $m_1=2$ from the included Frobenius supremum at r=2r0;
the numerical near-horizon maximum is not needed for that lower bound.
The metric inequality ensures signature/nondegeneracy by the operator
bound relative to the background tetrad metric; it is not a theorem about
the event horizon's location. An actual contraction proof also requires
a constraint-compatible evolution inverse bound $C_T$ and a nonlinear
Lipschitz estimate, e.g. $C_TC_{NL}(A_{max}M_1+A_{max}^2M_2+R)<1$.
Neither unknown constant can be silently absorbed into the symbol O(A^4).
Mixed data scales and secular terms must be included if present.

Two attempts to close this gate are explicitly exhausted here:
(i) the regular-frame hierarchy controls first-order size and gives
zero-mode quadratures, but supplies no bound on the constrained n=2
inverse, hence no M2; (ii) the positive scalar Killing-energy route
controls homogeneous transients, but is horizon degenerate and does
not bound the fourth-order scalar source/evolution trace or full metric
derivatives, hence no K4 or nonlinear C_T. Refinement of C0 cannot repair
either missing estimate. Moreover, even fixing h1 and q2 leaves an
independent homogeneous scalar datum at order epsilon to the fourth.
Its coefficient can be any finite number without changing the lower
hierarchy. Its self stress starts at order eight, interference with the
order-two scalar starts at order six, and coupling to h1 starts at order
five. Therefore no bound on K4 follows from h1/q2 alone over unspecified
higher-order data families. One must fix the exact initial-data family
or impose explicit bounds on those higher-order scalar data.
No unbounded derivative or continuum estimate
is asserted to be impossible; they are precise unproved requirements.

## 8. Cubic/bulge gate and next strategy

The lower hierarchy has not passed Gate B/C or exact simultaneous-data
completion. Therefore no cubic forcing computation, spectral/adjoint
projection, gamma, saturation or bulge-energy extrapolation is performed.
The independently checked convention law is useful for preventing a
false claim: if $B=A+cA^3$ and $d\tau'/dt=1+dA^2$, then
$\gamma_B=\gamma+2c\Omega-d\Omega$. A fixed asymptotic clock excludes
the arbitrary d transformation as a physical convention. At finite
Omega even the amplitude-only c transformation changes gamma. The
near-static threshold limit and a static nonuniform branch are different
problems. Gubser's static branch and Sorkin's phase comparison do not
give this dynamical coefficient.

A safer future representation before a spectral-gap theorem is a
projected memory equation with retained homogeneous data, schematically
$\dot A=\Omega A+\mathcal F[A,h_{hom}]+\int_{t_0}^tK(t,t')\mathcal S[A(t')]dt'$.
It is an organizing formula, not a calculated kernel or a proved
obstruction to every local normal form. Static mass/tension kernels,
horizon absorption and the real-frequency continuum require analysis.
No map from the reference-radius A to a finite horizon bulge event is
certified. Log-epsilon translation survives only conditionally within
the linear growth regime; FFG&A geometric formation windows are not
retarded waveform windows or flux measurements.

One concrete next **proposed** strategy is to construct a constraint-
preserving inhomogeneous scalar master for n=2 from (6)/(11)-(12) of the
geometric note, prove a weighted horizon-regular inverse bound including
its matched source tail, and use the same charge-fixed operator framework
for the finite-slice nonlinear constraint completion. Only then estimate
the fourth-order scalar radiation trace. No subsequent Goal is launched.
