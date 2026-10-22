# GOAL-003 adversarial audit II — data, nonlinear control and radion EFT

Independent scientific AI reviewer: `audit_flux_eft`, 2026-10-22. This is
a separate derivation and implementation within the same AI system, not
an external human referee. Input branch is
`research/sav-radiation@ef646d56ab7d8bebca578afab53e62da5146ec05`.
Only `research/sav/**` audit artifacts are written. No physical ODE,
spacetime evolution, production code, or mass-parameter survey is used.

The controlling goal/gates, scoped contract, notation, prior finite-data
note, all four earlier audits and negative-results record were read.
This audit first derives the action/current, massive radial operator and
error counterexamples independently, then reviews the new primary notes.
The independent script imports no project equation implementation.

## 1. Independent EFT variation and energy exchange

The separately postulated four-dimensional action is

$$S={1\over16\pi G_4}\int\sqrt{-g}
 [R-2(\nabla\phi)^2-4U(\phi)]+S_K+S_{\rm higher}.$$

Compactly supported variations give

$$\Box\phi-U'(\phi)=J,\qquad
J=-{4\pi G_4\over\sqrt{-g}}{\delta S_K\over\delta\phi},$$
$$T^{\phi}_{\mu\nu}={1\over4\pi G_4}
 \left[\phi_{,\mu}\phi_{,\nu}
 -g_{\mu\nu}\left({(\nabla\phi)^2\over2}+U\right)\right],$$
$$G_{\mu\nu}=2\phi_{,\mu}\phi_{,\nu}
 -g_{\mu\nu}(\nabla\phi)^2-2Ug_{\mu\nu}+8\pi G_4T^K_{\mu\nu}.$$

The matter metric variation is
`delta(sqrt(-g) Lphi)/sqrt(-g) =
[-2 phi_mu phi_nu + g_mu nu ((grad phi)^2+2U)] delta g^mu nu`.
This fixes both the potential coefficient in Einstein's equation and the
physical stress normalization. With `varphi=phi/sqrt(4 pi G4)`,
`U=m² phi²/2+...` gives the canonical potential `m² varphi²/2`.
Thus `U''(0)=m²`, not `2m²`. Flat asymptotics require both `U(0)=0`
and `U'(0)=0`. A constant nonzero U changes the background.

Commuting scalar covariant derivatives gives the exact exchange identity

$$\nabla^\mu T^\phi_{\mu\nu}={J\over4\pi G_4}\phi_{,\nu},
\qquad \nabla^\mu T^K_{\mu\nu}=-{J\over4\pi G_4}\phi_{,\nu}.$$

The second equation follows from diffeomorphism invariance with the KK
equations imposed. An externally prescribed J without its accompanying
stress/charge exchange is a transport model, not a closed gravity theory.
The independent implementation checks the first equation for arbitrary
`phi(t,r)` and `U(phi)` in flat two-coordinate dependence. The curved
identity follows from the displayed covariant commutator, not from a
claim that this restricted executable tests arbitrary curved geometry.

For the Schwarzschild Killing vector `partial_t`, its energy current is
`E^mu=-T^mu_t`. The outward coordinate-time power at a timelike sphere is

$$P_R=-{R^2f(R)\over G_4}\phi_t\phi_r\big|_R.$$

The factor f is required away from infinity. In a static exterior
annulus the energy balance is

$$E(t_2)-E(t_1)+\int_{t_1}^{t_2}(P_R+P_H)dt
 =-{1\over G_4}\int_{t_1}^{t_2}\!dt\int r^2J\phi_t\,dr,$$

with `P_H` defined outward from the exterior into the future horizon;
the horizon limit is nonnegative for a regular free scalar. Source work
has the indicated sign under the convention `Box phi-U'=J`. Storage and
horizon absorption must not be discarded when interpreting finite-R flux.

## 2. Radial transport, pulse measure and the horizon/infinity distinction

At the linear transport level, put `psi=r phi` and `dx/dr=1/f`. Direct
substitution, including every radial derivative of r, yields

$$[\partial_t^2-\partial_x^2+V_m]\psi=-rfJ,
\qquad V_m=f\left(m^2+{r_0\over r^3}\right).$$

This is the uncharged SO(3) scalar sector; the angular term would be
`f ell(ell+1)/r²`. For `m²>=0`, `V_m>=0` on the exterior. Consequently
the Laplace-point scalar operator `-d_x²+V_m+s²`, `s>0`, is coercive in
the scalar L² space with `||inverse||<=s^-2`. This fact does not bound
the constrained n=2 gravitational inverse or the regular full metric.

With `W=y_H y_out'-y_H' y_out`, the frequency Green kernel for
`-d_x²+V_m-omega²` is `-y_H(x_<)y_out(x_>)/W`.
The horizon branch is `y_H~exp(-i omega x)`. For real `omega>m`,
`p=sqrt(omega²-m²)>0`; for `0<omega<m`, the physical exterior branch
decays with `sqrt(m²-omega²)`. Below-threshold infinity evanescence does
not remove horizon absorption or finite-time source work. Branch points
occur at `omega=+-m`; lower-half-plane poles and the cut can contribute
long-lived or tail responses. Scalar positivity alone does not exclude
quasi-bound resonances or prove a useful uniform real-axis gap.

The massive Schwarzschild exterior is long range. In tortoise coordinate,

$$V_m=m^2-{m^2r_0\over r}+O(r^{-3}),\qquad
y_{out}\sim \exp\left[i\left(px+{m^2r_0\over2p}
 \log(r/r_0)\right)\right](1+O(r^{-1})).$$

The leading 1/r residual fixes the log coefficient; setting it to zero
fails the independent symbolic test. Its growth near p=0 is one reason
not to impose plane-wave matching uniformly down to threshold. In r
coordinates the log coefficient is `r0(2omega²-m²)/(2p)` because x
already contains `r0 log r`.

At infinity the real monochromatic convention
`phi=Re[b exp(-i omega t+i pR)]/R` gives

$$\langle P_R\rangle={\omega p|b|^2\over2G_4}+o(1).$$

For a finite-energy outgoing pulse use the *two-sided Fourier convention*

$$\psi(t,R)={1\over2\pi}\int_{|\omega|>m}
 b(\omega)e^{-i\omega t+i\Theta(\omega,R)}d\omega,
\quad b(-\omega)=b(\omega)^*,
\quad p(-\omega)=-p(\omega).$$

For sufficiently regular packet amplitudes, Parseval and the large-R
phase derivative `partial_x Theta -> p` give the all-time asymptotic
worldtube transport measure

$$E_{out}={1\over\pi G_4}\int_m^\infty
 \omega\sqrt{\omega^2-m^2}\,|b(\omega)|^2d\omega.$$

This is an all-time/outgoing scattering statement after the indicated
limit and decay assumptions. A finite observation interval is not
diagonal in frequency; windowing mixes frequencies and storage terms.
The coefficient b includes the source overlap and independent Cauchy data,
including their interference. It is not automatically the vacuum C0.
The independent executable checks the current and an exact two-tone
periodic control with `m=12`, `(omega,p)=(13,5),(15,9)`. That test checks
normalization/orthogonality, not the existence or GL amplitude of a pulse.

For `psi=r phi`, the exact finite-radius identity is

$$G_4P_R=-\psi_t\psi_x+{f\over r}\psi\psi_t
 =-\psi_t\psi_x+\partial_t{f\psi^2\over2r}.$$

The derivative term integrates to endpoint storage, explaining why the
master current and physical finite-sphere current agree only after
appropriate endpoints/limits. Copying the massless Bondi integrand
`omega²|b|²` misses the factor `p/omega` and is rejected.

## 3. Threshold, tails and controlled massless limits

`exp(s t)` has frequency `omega=i s`; its flat radial Green kernel is
`exp(-sqrt(s²+m²)|x|)/(2sqrt(s²+m²))`. The same decaying branch exists
whether s is above or below m. No emission switch occurs at s=m.
A causal scalar test source `t^4 exp[-(a-s)t] Theta(t)`, `a>s`, has
Fourier transform `24/(a-s-i omega)^5`, nonzero above any finite mass.
This is a manufactured scalar source, not a switched GL metric. For a
nonzero smooth compact-time source the entire Fourier transform likewise
cannot vanish on a whole open above-threshold frequency interval.
Neither assertion proves nonzero physical GL source projection: spatial
overlap cancellation is still possible.

[Koyama–Tomimatsu, II and III](https://arxiv.org/html/gr-qc/0103086v2)
were inspected directly. Their equations (4)–(10) agree with the free
operator above, with `r0=2M`. Their small-mass analysis distinguishes an
intermediate oscillatory monopole tail from a still later curvature-
dominated tail. These are literature asymptotic regimes, not quantitative
uniform remainder estimates for this forced/matched problem. No tail
coefficient, onset time, or quasi-bound amplitude is imported here.
The Green sign, flux normalization and source work above are independently
derived; no potentially inconsistent rendering of the paper's Green
ordering/sign convention is copied.

For a smooth packet of compact frequency support
`m<omega_min<=|omega|<=omega_max`, at fixed `u=t-R` the phase derivative
is `R(omega/p-1)-u` to leading order. For fixed m>0 its coefficient is
bounded away from zero. Repeated frequency integration by parts makes
`R phi` and its time derivatives tend to zero faster than inverse powers
of R in this packet class. There is therefore no ordinary massless
Bondi news carrying that packet's energy at null infinity; the energy
arrives at timelike speed `v_g=p/omega<1`. General finite-energy data
require their regularity/decay assumptions before asserting pointwise
null asymptotics. The packet argument suffices to reject the universal
use of massless news for massive transport.

Let `xi=m²/omega_min²<=eta<1`, on a fixed above-threshold band. Then

$$0\le1-{p\over\omega}\le
 {m^2\over\omega_{min}^2(1+\sqrt{1-\eta})},$$
$$|\omega-p|D\le
 {m^2D\over\omega_{min}(1+\sqrt{1-\eta})},\qquad
\Delta t\le D\left({1\over\sqrt{1-\eta}}-1\right).$$

These respectively bound the kinematic flux-factor error, propagation
phase error and group delay of a fixed spectral packet. Schwarzschild's
extra mass-dependent Coulomb phase must also be small or retained.
Source/transfer/matching changes in b are additional errors. The estimates
are not uniform in propagation distance, time, or threshold approach;
`m r0 <<1` alone establishes none of them. Taking m to zero first at fixed
distance differs from fixed-positive-m null-infinity limits.

## 4. Independent nonlinear/data rejection controls

An arbitrary free fourth-order scalar produces
`N=epsilon² n2+epsilon⁴ c n4+...`. The current then contains
`2 epsilon⁶ c n2 n4` and `epsilon⁸ c² n4²`. The fourth-order coefficient
can be arbitrarily large while h1 and q2 stay fixed. Thus no uniform
remainder constant follows without an all-orders family or explicit free
data norm. The script checks those orders and rejects omitted interference
using exact destructive cancellation. It does not claim an exact GL
family from this formal freedom.

Half-period covariance only enforces `n=order mod 2`.
`epsilon² cos(4kz)` obeys the covariance but is outside the generated
quadratic list. The usual lists require other independent harmonics to
be set to zero in the data. The fourth power of a real cosine has complex
coefficients `3/8,1/4,1/16` at n=0,+-2,+-4. Exact SO(3) leaves an
invariant sphere rank-two tensor proportional to the sphere metric;
requiring trace-free Bondi news sets it to zero. The radial KG monopole
`cos(omega t-pr)/r`, `omega²=p²+m²`, instead solves the scalar equation
and has positive outward average power. These are different statements.
The vacuum n=2 massive gravitational singlet has mass 2k and its own
current; adding radion mass in quadrature has no derivation here.

For the horizon-energy counterexample set r0=1 and, on tau=v-r,
`F_delta(r)=sqrt(delta) b((r-1)/delta)`, `P_Sigma=0`, with support
`1+delta<r<1+2delta`. Take a smooth bump b, or the C1 polynomial
`b(y)=(y-1)²(2-y)²` used in the executable. The regular radial derivative
norm is `int |F_delta,r|² dr=B1>0`, independent of delta, but

$$Q_\Sigma\le{\delta\over2}(2B_1+\delta B_0)\longrightarrow0,
\quad B_1=\int_1^2(b')^2dy,\quad B_0=\int_1^2b^2dy.$$

The scalar Killing energy cannot uniformly dominate a regular derivative
norm, much less the full constrained metric norm. This is an explicit
failure of the proposed coercivity inequality, not merely a warning.

The independent cubic calculation gives
`gamma_B=gamma+2c Omega-d Omega` under `B=A+c A³`,
`d tau'/dt=1+d A²`. A toy eliminated stable variable
`y_dot=-lambda y+b A²`, `A_dot=Omega A+c A y`, has retarded response
`y=e^-lambda t y0+b int_0^t e^-lambda(t-s) A(s)² ds`.
On an eternal growing reference its cubic coefficient is
`bc/(lambda+2Omega)`, whereas the static substitution gives `bc/lambda`.
The exact values `1/5` versus `1/3` at lambda=3, Omega=1 reject the
static-to-dynamic inference. This manufactured model supplies no GL
coefficient; its purpose is to detect an invalid elimination.

For any actually bounded news error b on U, triangle inequalities give
the absolute energy enclosure in the controlling Goal. Relative estimates
require a positive reference denominator; the sufficient ten-percent
news-norm condition is `eta<=sqrt(1.1)-1`. Smooth-window spectrum errors
must remain absolute at zeros. A single 2048-point manufactured C-infinity
compact taper checks discrete positive-frequency Parseval against a direct
scalar integral. No physical waveform or continuum certificate follows
from that floating check.

## 5. Execution record and actual primary-artifact review

The reproducible command is

```text
python3 research/sav/verification/goal003_audit_flux.py > research/sav/verification/goal003_audit_flux_results.txt
```

Versions and exact output are retained in that transcript and
`goal003_audit_flux_results.json`. The first execution stopped at a Python
structural-expression equality in the memory integral: SymPy had returned
an algebraically equivalent rational expression. Replacing structural
comparison with exact simplification fixes the check; the mathematical
formula was not altered. This implementation failure is retained here.

The final execution passes 36 exact/manufactured checks and rejects 19
deliberately incorrect variants. The compact-window time and spectral
integrals both give 0.16224972764708148; direct adaptive quadrature gives
0.1622497276470815, with an estimated error 4.45e-14. These are floating
normalization diagnostics, not a continuum error certificate. All other
algebraic tests use exact SymPy simplification. Two additional checks
retain the fourth-order radion logarithm and the product rule required
by a second-order asymptotic clock transformation; raw fourth-order gzz
is explicitly rejected as the completed radion.

### Actual EFT note reviewed

After the independent derivation, the complete
`physics/goal003_stabilized_radion.md` was read. Its variations, source-work
sign, finite-radius f factor, proper-time static-observer power `P_R/f`,
Coulomb phase, all-time pulse measure and conditional matching inventory
agree with this audit. The scalar resolvent identity
`R_m-R_0=-R_m(m²f)R_0` and bound `m²/s⁴` follow from nonnegative H0,Hm
and `0<=f<=1`; they are absolute scalar operator estimates only.

For the note's finite-time massless comparison, if
`u=psi_m-psi_0` with equal data and forcing, then
`u_tt+H_m u=-m² f psi_0`. The energy identity yields
`d sqrt(2E_m[u])/dt <= m² ||psi_0||_2`, verifying its estimate. Its
subsequent L² bound follows from the spectral-theorem bounds on cosine
and `sin(t sqrt(H0))/sqrt(H0)`. The audit requested precise wording for
unequal data: add `sqrt(2 E_m[u](0))`, not the difference of energies of
two initial states. The parent accepted this clarification. It leaves
the same-data result unchanged.

### Independent exact constraint-system check

The data investigator supplied the candidate equations before its final
note. This auditor independently used the spatial metric

$$\gamma=a(r,z)^2dr^2+b(r,z)^2dz^2+R(r,z)^2d\Omega_2^2$$

and arbitrary extrinsic-curvature fields
`K^r_r=kappa`, `K^z_z=v`, `K^theta_theta=K^varphi_varphi=(q-v)/2`,
`K^r_z=eta/a`, `K^z_r=a eta/b²`. Direct two-dimensional warped-product
curvature gives

$$\mathcal R_4=\mathcal R_h-
 {2H_*'+H_*^2+|A_*|^2\over a^2}
 +{2H_*a_r\over a^3}-{2\Delta_h a\over a},
\quad H_*={b_r\over b}+{2R_r\over R}.$$

The extrinsic part is exactly
`2 kappa q+2q²/3-2eta²/b²-|sigma|²` with
`sigma^z_z=v-q/3` and `sigma^theta_theta=(q-3v)/6`.
Full four-dimensional coordinate Christoffels verify both momentum
equations for arbitrary a,b,R,kappa,q,v,eta, not a single specialized
background. In particular their tangential form is

$$\eta_r=a\left[\kappa_z+q_z-v_z
 -(3v-q){R_z\over R}\right]-H_*\eta+(\kappa-v)a_z.$$

Omitting its last lapse-gradient term is rejected. The equivalent
trace/sigma equation supplied by the investigator agrees exactly.

[Rácz, theorem 4.1](https://arxiv.org/html/1508.01810v3) was inspected
directly. Smooth prescribed leaf metric, radial shift, kappa and tangential
trace-free curvature, positive H*, and smooth positive initial a support
a unique local radial collar solution of the full constraints. The compact
leaf topology here is S² times S¹, and the theorem permits dimension n=4.
It does not supply the required global asymptotic completion, fixed ADM
mass or a quantitative radius. A closure making kappa depend on unknown q
changes the specified-free-data hypothesis; it cannot be accepted by
simply citing that theorem.

For the proposed outer repair `kappa=-q/2+d`, prescribed `K^z_z=v`,
`b=1,R=r`, the principal pair is `q_r=a eta_z`,
`eta_r=(a/2)q_z`, with positive symmetrizer diag(1/2,1). This algebra is
independently verified. It is useful for a possible global estimate but
does not itself prove coupled parabolic/hyperbolic continuation, weighted
Sobolev estimates, asymptotic charges or a parameter-smooth full data map.

### Actual data/higher-order note and AH map reviewed

The complete `physics/goal003_data_nonlinear.md` and
`goal003_data_checks.py` were subsequently inspected. The independent
constraint calculation above agrees with the primary equations (1)–(3).
The fixed-kappa/sigma bad tail `C r^(-2/3)` follows from subtracting the
background radial momentum equation. The modified exterior weighted
energy cancellation is exact, but the actual Schwarzschild free function
is `d0=r0²/[2 r^(3/2)(r+r0)^(3/2)]`, rather than zero. The note keeps
that integrable-background-source distinction, and leaves the necessary
higher Sobolev/asymptotic estimates open.

The ordered connection recursion (12) is obtained by multiplying the
metric-inverse series by the linear connection numerator. At order four
the term `U1 U2+U2 U1` is required. The Ricci bilinear sums in (13)
contain every ordered pair with total order three/four. The logarithmic
radion completion and coordinate pullback in (15)–(16) supply additional
terms beyond `Q4,zz`. The quartic Bondi energy uses `N2²`, whereas the
first `N2 N4` correction occurs at order six. These are formal identities
with the note's explicitly unbounded higher fields and asymptotic map;
this audit does not promote them to a solved evolution or a bound on N4.

The Duhamel estimate (19), cubic memory representation (21), exponentially
weighted candidate coefficient (22), and moment error (23) have consistent
orders/signs under their stated semigroup/projection hypotheses. The
independent static-versus-growing elimination test above verifies why
the `2Omega` in the resolvent cannot be dropped. The unresolved adjoint
functional, propagator and kernel moments prevent a local GL amplitude
law from following merely from these representations.

For the AH map, this auditor independently varied the ADM lapse and shift
at the horizon, using

$$N^2=-g_{\tau\tau}+{g_{\tau r}^2\over\gamma_{rr}},
\qquad \beta^r={g_{\tau r}\over\gamma_{rr}},
\qquad (g_{\tau\tau},g_{\tau r},\gamma_{rr})_0=(0,1,2).$$

For arbitrary variations `(T,D,E)`, one obtains
`delta N/N=-T+D-E/4` and `delta beta=D/2-E/4`.
The tangential expansion then gives
`delta theta_data=sqrt(2)(Omega S-T)`; all Sr,D,E terms cancel.
The independent executable differentiates these exact ADM expressions,
rather than inserting the already reduced primary result. The background
expansion derivative is sqrt(2), and the normal graph length is
sqrt(2)rho; thus the graph operator is
`sqrt(2)(-partial_z²+1)`. These agree with (24)–(25).
Substitution of the previously audited ingoing T,S yields
`delta R_AH=S/2+rho=-a_h/[4(1+k²)]`, as in (26).
This is a linear, slice-specific MOTS map. Its positive linear inverse
does not bound nonlinear geometry, prove outermostness, or reach a fixed
finite-contrast first-bulge event.

Two precision repairs were requested during actual note review:

1. The modified `kappa=-q/2+d` local family must be supported by a mixed
   parabolic/hyperbolic Sobolev estimate, not the principal symmetrizer
   alone or a direct invocation of theorem 4.1 with changed hypotheses.
   The terms containing the lapse derivative require its parabolic
   dissipation, with sufficiently smooth free coefficients.
2. The finite-radius circle-size derivative is a chart statement. From
   the exact ADM identity with `gamma_rz=0`,
   `K^z_z=-N(log b)+(partial_z beta^z)/lapse`. Therefore
   `N[(sqrt(3)/2)log b]=-sqrt(3)K^z_z/2` requires a normally transported
   compact coordinate on the initial slice (`partial_z beta^z=0`).
   This chart-defined quantity agrees with the physical perturbative
   radion at the specified order; it cannot silently substitute for the
   full finite-radius nonlinear gauge completion. A deliberately omitted
   compact-shift term is now rejected by the audit executable.

The revised note was then inspected. Equation (5a) supplies the mixed
energy estimate on a compact periodic collar with a,b,R,H* bounded away
from zero and sufficiently smooth fixed coefficients. Symmetrizing the
momentum principal part removes derivatives of W from its energy; the
additional top derivative of a is absorbed into the positive lapse
parabolic dissipation by Young's inequality. Lower-norm difference
estimates avoid quasilinear coefficient loss. Together with smooth
regularization and compact-interval continuation near the known background,
this supplies the stated qualitative finite-collar construction and local
parameter smoothness. It supplies no global weighted estimate or explicit
amplitude radius. The revised chart paragraph includes the shift term and
identifies the physical radion only through the justified perturbative
order. Both requested precision repairs are resolved at that local scope.

## 6. Final acceptance and withheld conclusions

| Item | Audit disposition |
|---|---|
| Exact radial constraints | Accepted by independent arbitrary-function curvature/momentum checks |
| Fixed all-orders simultaneous data | Qualitative exact finite-collar family accepted under smooth input and positive-coefficient assumptions; global asymptotic/ADM family remains OPEN |
| Differentiability | Local parameter dependence supported by mixed estimates; global charge and radiation maps not established |
| Cubic/quartic source and completion | Formal ordered differential identities accepted; forced solutions and N4 estimates remain OPEN |
| Memory dynamics | Derived conditional Duhamel/projection identity accepted; physical kernel, moments and local gamma not computed/bounded |
| AH geometry bridge | Explicit linear slice-specific map accepted; nonlinear remainder, outermostness and finite first-bulge coverage OPEN |
| Massless news/energy/spectrum | Normalization and conditional absolute-error algebra accepted; exact-family finite-A waveform and positive lower energy bound not certified |
| Separate EFT | Action, exchange, scalar operator/current, pulse measure and conditional limits accepted; microscopic stabilizer and GL/KK matching OPEN |
| Quantitative GL amplitude interval | NO; no positive numerical A*, logarithmic time window or complete radiation-trace error is established |

The exact missing inequalities include: a uniform weighted exterior
constraint bound with positive lapse; a nonzero lower bound on the ADM
mass-map derivative for the chosen inner charge parameter; full regular
metric/inverse bounds and higher-data control; a nonlinear nondegenerate
energy/radiation trace estimate; and a total news error strictly below a
certified positive reference norm when a positive energy lower bound is
claimed. The local collar result and scalar positivity solve none of those
global estimates automatically. The next concrete data strategy is the
high-Sobolev extension of the modified outer system with its integrable
background d0 and free tails, followed by charge-map tuning and a separate
exterior evolution/radiation estimate. The retained bad tail invalidates
one prescription; two unsuccessful global routes do not prove impossibility.

This audit supports **MEANINGFUL_PARTIAL** at the overall Goal level if
the remaining master/metric/data/remainder gates stay open. It does not
weaken `COMPLETE_VERIFIED`, certify a first-bulge fraction, or authorize
any subsequent Goal. Publication/tree scope is the parent agent's separate
final check; this auditor's writes are confined to the four named audit
artifacts under `research/sav/verification/`.
