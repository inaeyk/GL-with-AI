# GOAL-004 adversarial audit II — data, news, model dynamics and EFT

Independent scientific AI reviewer: `audit_dynamics`, 2026-10-23.
Input research ref: `2f4648c379c474ed124e3ef10b62efcd4a783c66`.
Savdeep Sethi is the designated operator/publisher. This reviewer writes
only this audit, its independently authored verification script and outputs;
it does not commit, push, author the primary model or certify physical data.
No spacetime evolution or physical perturbation ODE configuration is used.

The scoped contract, all GOAL-004 controllers, relevant prior final reports,
data/radiation/EFT notes, prior flux audit, negative findings and attributed
GOAL003 review qualifications were inspected. This is a distinct scientific
AI investigator in the same system, not an external human referee. The
independent controls import no project physics implementation. Central
identities in sections 1–4 and the first 47 executable controls were obtained
and run before any new primary GOAL-004 model/data file existed. Later model
action checks use the proposed formulas communicated by the primary author;
this shared input is disclosed and is not an independent derivation of
those closure assumptions from vacuum gravity.

## 1. Independently derived news, data and fourth-order identities

Put $a=\|N_{\rm ref}\|_{L^2(U)}$ and suppose
$\|\delta N\|_{L^2(U)}\le b$. Expansion of the Hilbert-space norm gives

$$G_4 E_U=a^2+2\langle N_{\rm ref},\delta N\rangle
                      +\|\delta N\|^2,$$
$$\boxed{\max(0,a-b)^2/G_4\le E_U\le(a+b)^2/G_4.}$$

For $b<a$ the lower edge is sharp with anti-parallel error; the upper edge
is sharp with parallel error. If $b\ge a$, the reference can cancel
completely, so squaring $a-b$ without clipping is false. The sufficient
worst-case relative-energy condition is
$2\eta+\eta^2\le0.1$, $\eta=b/a$, hence
$\eta\le\sqrt{1.1}-1=0.04880884817\ldots$. It is not necessary: an
orthogonal error with norm ratio $\sqrt{0.1}$ gives exactly ten percent
energy error. A positive lower energy requires both a genuinely bounded
$b<a$ and a positive reference norm; empirical $C_0$ digits provide neither
for the requested exact global family.

With the stated scalar normalization,
$E/M_0=(2/r_0)\int N^2du$. A generated quadratic news $n$ and independently
prescribed scalar news $\lambda n$ yield $(1+\lambda)^2\|n\|^2/G_4$,
not $(1+\lambda^2)\|n\|^2/G_4$. The two choices $\lambda=0,-1$ are
valid examples of freedom at the homogeneous linear scalar / constrained
second-order level. They do **not** construct two exact global nonlinear
GL-plus-radion families. Their nonlinear admissibility is gated by the
global constraint theorem that remains to be supplied.

For a no-factorial expansion,

$$g_{zz}^U=1+\epsilon^2\chi_2+\epsilon^4\chi_4+\cdots,
\qquad \phi_4={\sqrt3\over4}(\chi_4-\chi_2^2/2).$$

For $u'=u+\epsilon^2 a(u)$ the quartic news changes by
$N_4'=N_4-aN_2'-a'N_2$. The $a'N_2$ term and logarithmic subtraction
are independently checked. If an order-four free datum changes $N_4$ by
$q_4$, then

$$N^2=\epsilon^4N_2^2+2\epsilon^6N_2(N_4+q_4)+O(\epsilon^8).$$

Thus lower-order data cannot determine the first energy correction.
Half-period covariance only requires harmonic number congruent to
perturbative order modulo two: $\epsilon^2\cos6kz$ is an admissible
independent harmonic under that covariance. The narrower generated lists
require an explicit rule excluding independent harmonics. Ordered Lie
completion contains both $\mathcal L_{\xi_1}\mathcal L_{\xi_2}$ and
$\mathcal L_{\xi_2}\mathcal L_{\xi_1}$; an exact noncommuting two-matrix
control rejects replacing their sum by twice one ordering.

## 2. Independent propagation and memory checks

A residual $C\epsilon^p e^{p\Omega t}$ propagated by
$C_S e^{\sigma(t-s)}$ integrates to

$$CC_S\epsilon^p {e^{p\Omega T}-e^{\sigma T}\over p\Omega-\sigma},$$

or $CC_S\epsilon^p T e^{p\Omega T}$ at equality. At
$T=\Omega^{-1}\log(A_*/\epsilon)$, $\sigma<p\Omega$ is needed for
this particular uniform $O(A_*^p)$ estimate. Equality adds a logarithm;
$\sigma>p\Omega$ produces negative powers of $\epsilon$. The
manufactured counterexample fixes $p=4,\Omega=1,A_*=0.05$ and varies
only the seed from $10^{-3}$ to $10^{-6}$; the greater-exponent error
increases by over two orders of magnitude. A small residual alone also
does not certify a solution approximation: $\dot e=e+10^{-3}$,
$e(0)=0$ has $e(10)=10^{-3}(e^{10}-1)$.

Independently integrate
$\dot y=-\lambda y+A^2$, $A=e^{\Omega t}$, $y(0)=0$:

$$y={e^{2\Omega t}-e^{-\lambda t}\over\lambda+2\Omega}.$$

The growing-history cubic coefficient contains
$(\lambda+2\Omega)^{-1}$, not $\lambda^{-1}$. For a constrained
projection the corresponding object is
$(2\Omega-L_Q)^{-1}QB(e,e)$ plus the independent transient. The
bounded adjoint projector, physical semigroup and kernel moment bounds
are assumptions that a local fit does not prove.

One-point matching of the scalar Laplace response has another exact
nonuniqueness: kernels
$h_1(v)=C_0\delta(v)$ and
$h_2(v)=C_0e^{2\Omega\tau}\delta(v-\tau)$ both return
$C_0e^{2\Omega t}$ on the eternal growing input but disagree for a
nonlinear history. Therefore reproducing $C(2\Omega)=C_0$ alone does
not fix a first-bulge waveform. A delay family is a model sensitivity,
not an enclosure of the physical Green kernel.

The audit also constructs an exactly nonzero compact manufactured source
$S=(-\partial_x^2+1)f$, $f=(1-x^2)^3$ for $|x|<1$, zero outside.
Both $f,f'$ vanish at its endpoints, so
$\int e^x S\,dx=0$ by integration by parts. A nonzero source and positive
Green kernel cannot alone prove outgoing overlap. This manufactured
control has sufficient endpoint regularity for that integral, and claims
no smooth physical GL source.

## 3. Independent massive-EFT transport normalization

For $S_\phi=(16\pi G_4)^{-1}\int\sqrt{-g}
[-2(\nabla\phi)^2-4U(\phi)]$, $U=m^2\phi^2/2+\cdots$,
the scalar equation is $\Box\phi-U'=J$ and

$$T^\phi_{\mu\nu}={1\over4\pi G_4}
 [\phi_{,\mu}\phi_{,\nu}-g_{\mu\nu}((\nabla\phi)^2/2+U)],
\qquad \nabla^\mu T^\phi_{\mu\nu}=J\phi_{,\nu}/(4\pi G_4).$$

The retained source/stabilizer sector must carry opposite exchange.
The scalar potential and its stress are a separate theory; no mass term
is added to the five-dimensional vacuum Einstein equations.

At $r=R$ in Schwarzschild, $\psi=r\phi$,

$$P_R=-{R^2 f\over G_4}\phi_t\phi_r
     =-{1\over G_4}\psi_t(\psi_x-f\psi/R).$$

The $f\psi\psi_t/R$ term is finite-worldtube storage. Its integral is an
endpoint contribution $f[\psi^2(t_2)-\psi^2(t_1)]/(2RG_4)$ at fixed R.
It cannot be discarded on an arbitrary finite observation window. The
script independently differentiates the local spherical flat-space energy
and current for arbitrary $\phi(t,r)$, obtaining scalar source work with
the above sign, and then checks the exact Schwarzschild current rewrite.

For $\phi=\Re[b e^{-i\omega t+ipr}]/r$,
$p=\sqrt{\omega^2-m^2}$, direct phase averaging gives
$\langle P\rangle=\omega p|b|^2/(2G_4)$. Parseval for a real
finite-energy outgoing pulse with inverse Fourier coefficient $1/(2\pi)$
then gives the all-time scattering measure

$$E_{\rm out}={1\over\pi G_4}\int_m^\infty
       \omega\sqrt{\omega^2-m^2}|b(\omega)|^2d\omega.$$

Its assumptions include no unaccounted net endpoint storage, a separated
outgoing channel and a suitably convergent large-radius scattering limit.
A time taper mixes frequencies, so the diagonal all-time formula does not
become an exact finite-window formula by tapering b. Massless news has
weight $\omega^2$ instead. The independent Gaussian-news Parseval check
uses $N=e^{-u^2}$ and $\Phi=\int Ndu$, which has memory; it confirms
$G_4E=\sqrt{\pi/2}$ and $E/M_0=\sqrt{2\pi}$ for $r_0=1$.

For imaginary frequency $\omega=is$, the far radial decay exponent is
$\sqrt{s^2+m^2}$, smooth at $m=s$ and nonzero on both sides. There is
no $2\Omega>m$ radiation switch. A real-frequency infinity threshold
$|\omega|>m$ does not eliminate below-threshold horizon absorption.
The n=2 vacuum gravitational channel instead has asymptotic mass $2k$;
it is not the stabilized n=0 scalar.

For a fixed above-threshold band with $d=m^2/\omega_{\min}^2<1$,
$p/\omega\in[\sqrt{1-d},1]$ compares **weights for the same b**.
Waveform matching also needs accumulated phase
$|p-\omega|D\sim m^2D/(2\omega)$ and delay
$D(\omega/p-1)\sim m^2D/(2\omega^2)$ controlled, along with
source/curvature/tail errors. Small $mr_0$ alone does not provide these
bounds or fix $m/\Omega$. Fixed-positive-m null-infinity and fixed-distance
massless limits are nonuniform. No new tail amplitude/onset or microscopic
stabilizer matching is computed in this audit; prior literature facts are
read from the attributed source notes rather than claimed as a new direct
paper inspection.

## 4. Independent algebra of the proposed first-bulge closure

The primary author supplied the proposed material map
$z=q+b(t)\sin(kq)/k$, $J=1+b\cos(kq)$ and an isentropic uniform-string
EOS $\varepsilon\propto\sqrt{s}$, $P=-\varepsilon/2$. Entropy per
material label held fixed gives $s\propto J^{-1}$ and
$R/r_0=J^{-1/2}$ at leading nonrelativistic order. Starting independently
from the relativistic fluid action in these variables gives

$$L/E_0=-\langle\sqrt J(1-v^2)^{1/4}\rangle,
\quad v=\dot b\sin(kq)/k,$$
$$M(b)={\langle\sqrt J\sin^2(kq)\rangle\over2k^2},\quad
M={1\over4k^2}-{b^2\over128k^2}+O(b^4).$$

Thus the proposed inertial coefficient uses enthalpy consistently;
substituting an ordinary mass-density kinetic factor twice as large fails
the exact expansion. This does not derive the fluid closure from the
full constrained SO(3) Einstein equations at finite k.

For the proposed gradient potential

$$V=\langle\sqrt J-1\rangle+
 {1\over8k_c^2}\langle J_q^2/J^p\rangle,$$

the independently integrated angular expansion is

$$V={k^2/k_c^2-1\over16}b^2+
 \left[-{15\over1024}+{k^2p(p+1)\over128k_c^2}\right]b^4+O(b^6).$$

Consequently $\Gamma^2=k^2(1-k^2/k_c^2)/2$, and with
$d=(\nu/2)\langle\cos^2/J^2\rangle$, $d(0)=\nu/4$, the
growing linear root satisfies
$\Omega^2+\nu k^2\Omega-\Gamma^2=0$. Choosing
$\nu=(\Gamma^2-\Omega^2)/(k^2\Omega)$ reproduces the previously
measured GL rate by construction; this is a calibration, not a fresh
independent prediction of that rate.

Since $R_z=-(r_0/2)J_q/J^{5/2}$, Eulerian gradient energy
$\int dz\,\varepsilon(R)(R_z)^2$ has material weight $J^{-9/2}$;
freezing its energy-density prefactor instead gives $J^{-4}$. These
derive the stated exponents from the two assumptions, but the positive
gradient coefficient itself is threshold matched, and higher derivative
operators are omitted. At $kr_0=0.6283\ldots$ no small parameter bounds
these omissions. The two exponents are a sensitivity comparison, not
upper/lower bounds on the exact vacuum dynamics.

Independent 256-node angular quadrature, with empirical
$k_cr_0=0.876160402034$, finds static potential minima:

| Model exponent | Minimum b | Radius-contrast proxy $b/(1+\sqrt{1-b^2})$ |
|---:|---:|---:|
| 4 | 0.378712057364104 | 0.196680963983723 |
| 9/2 | 0.341047662605719 | 0.175793595459759 |

The derivative residuals are below $3.2\times10^{-17}$ in this floating
quadrature. They are not certified intervals. An inertial trajectory may
overshoot these minima; an equilibrium is not by itself an upper bound on
the transient. Their difference gives a concrete closure sensitivity.

Two physically material gaps were reported to the primary author:

1. Bulk-viscous heating is quadratic in b on the growing branch, so the
   omitted change of entropy profile influences cubic amplitude feedback
   at the same order as retained quartic energy terms. A separately
   accumulated heat integral without thermodynamic feedback is an error
   diagnostic. It does not make an isentropic closure self-consistent.
2. Postprocessing scalar flux without its source-work backreaction leaves
   a mass residual equal to the emitted energy at that approximation
   order. Far-zone power and near-zone work occur at different times and
   include storage; subtracting far power at the same source time is not
   an exact repair. The model must retain this residual and declare a
   weak-backreaction regime or derive the missing coupling.

The model contrast is a thermodynamic cross-section proxy. Its match to
the audited *linear* apparent-horizon radius map does not prove a nonlinear
MOTS, outermostness or a finite first-bulge event in the vacuum solution.

## 5. Execution and artifact review record

Executed from `research/sav`:

```text
python3 verification/goal004_audit_dynamics.py > verification/goal004_audit_dynamics_results.txt
```

The final independent script reports **109 checks, 18 rejected deliberate
mutations, all passed**. Python 3.14.8, SymPy 1.14.0, NumPy 2.4.6 and
SciPy 1.18.0. Exact source SHA256 and old input-file hashes are in
`goal004_audit_dynamics_results.json`; the stdout transcript retains actual
symbolic residuals and manufactured numerical inputs/outputs. One failed
extended run due to coefficient extraction before expanding a rational
SymPy expression is retained in `goal004_audit_dynamics_failures.txt`.
Expansion fixed the implementation; no scientific coefficient was changed.
The independent massive pulse integral is a single manufactured compact
frequency-band weight check, not a physical radion-mass survey.

### Separate check of the new pulse-dependent EFT result

After inspecting `physics/goal004_stabilized_transport.md`, this auditor
independently changed variables $y=\sqrt{z^2+\mu^2}$ in its kinematic
spectral integrals. For Gaussian news with nonzero memory this gives

$$\mathcal R_{\rm mem}={2e^{-\mu^2}\over\sqrt\pi}
 \int_0^\infty {z^2e^{-z^2}\over z^2+\mu^2}dz
=e^{-\mu^2}-\sqrt\pi\mu\operatorname{erfc}\mu.$$

One independent derivation of the denominator integral uses
$(z^2+\mu^2)^{-1}=\int_0^\infty e^{-u(z^2+\mu^2)}du$:
performing its Gaussian z integral and then setting $v=\sqrt{1+u}$
gives $\pi e^{\mu^2}\operatorname{erfc}\mu/(2\mu)$.
For derivative-Gaussian news the extra $y^2$ cancels the denominator
under the same substitution, leaving $\mathcal R_{0}=e^{-\mu^2}$.
Independent direct above-threshold quadratures at $\mu=0.1,0.5,1$
agree with both functions. The finite linear right derivative
$\mathcal R_{\rm mem}'(0)=-\sqrt\pi$ rejects a universal quadratic
small-mass relative suppression when massless memory supplies power
down to zero frequency. The comparison holds the scattering amplitude
fixed and does not determine physical GL/stabilizer matching. This is
an independently checked kinematic mathematical result, not a massive
Bondi-energy law or a physical suppression percentage.

### Actual primary-script/data-note review and repairs

The complete `verification/goal004_model.py`, its numerical JSON, both
saved waveform/spectrum tables and `physics/goal004_data_news.md` were
inspected after the independent identities. The independent script checks
the saved-news energy integral without importing the primary ODE code,
the two settled shapes against separately computed potential minima,
the initial weak $C_0$ news and the one-sided spectral RMS normalization.
Input/output/source hashes are saved in its JSON.

Three specific diagnostic defects were found in the initial model script
and reported before acceptance:

1. Its unprojected ideal-fluid residual used the wrong sign of the pressure
   gradient and omitted a retained-order spatial kinetic-stress term.
   Direct variation of $L_2=-\sqrt J+\sqrt Jv^2/4$ instead gives
   $\partial_t(\sqrt Jv/2)+\partial_q[-1/(2\sqrt J)+v^2/(8\sqrt J)]=0$.
   The corrected expanded formula and subsequently added gradient/viscous
   forces agree with independent symbolic variations in this audit script.
   The projected amplitude ODE itself already used the correct potential.
2. Its quantity named radius gradient was $|\partial_z\log R|$, with
   weight $J^{-2}$, while $|\partial_zR|$ has weight $J^{-5/2}$.
   It is now explicitly named a log-radius gradient.
3. The spectral RMS denominator included the full zero-frequency bin,
   while its Parseval energy correctly used half that bin. The corrected
   RMS values are $0.0673685861394390$ and $0.0688522489730500$ in
   $r_0^{-1}$, rather than $0.0635994$ and $0.0650786$. Waveform and
   integrated-energy results are unaffected by that diagnostic correction.

The repaired script records a complete-fluid-closure unprojected relative
residual of **0.630817 and 0.634563**, although its Galerkin projection
error is only $4.2\times10^{-17}$ or smaller. These are compatible: an
accurately integrated projected ODE can leave substantial discarded modes.
The residual is not small, and no stability estimate makes it a physical
error certificate. The primary author retained this explicit closure
failure rather than interpreting projection convergence as vacuum GR
acceptance. The small low-velocity diagnostic does not cure finite-k,
entropy, discarded-mode or radiation-reaction errors.

The actual model uses static frozen-profile transfer
$T(0)=13.2771811732$ and growing transfer $T(2\Omega)=C_0=12.3072919724$
to set a one-relaxation kernel and a pure-delay kernel. Their parameters
$\tau=0.6226657731r_0$, $\tau_d=0.5993481767r_0$ follow algebraically
from those two calibrations. This is stronger than calibrating $C_0$ alone,
but these are not measured first moments of the full physical Green
kernel. The static input source still freezes the physical growing-mode
radial shape beyond its derived regime. The nonzero horizon/infinity
source errors are empirical, not interval-certified.

Its input extrapolation gives $\beta_{\rm AH}=6.9002454883$ for the
audited *linear* radius map; this is not a new nonlinear horizon theorem.
The kinematic second harmonic of $J^{-1/2}$ has not been matched to
GOAL003's full forced n=2 metric/master and the exact data family's selected
homogeneous response. The model recovers the empirical growing rate and
quadratic scalar transfer by calibration; it does not recover that whole
constrained q2 sector.

The full data/news note's conformal action was independently checked:
the original $R^2\mathcal R_3$ term contributes
$-8(\nabla\log R)^2$ after its boundary divergence, while its original
kinetic term contributes $+2(\nabla\log R)^2$. Hence
$\chi=\sqrt{12}\log(R/r_0)$ and
$U_3=-2r_0^{-2}e^{-\sqrt3\chi}$, as displayed there. Its distinction
between sphere-orbit dilaton and physical KK radion is essential. The
note does not infer an exterior radiation theorem from compact-region
hyperbolicity. Its quartic completion, conditional $\epsilon^2K_4<a_2$
criterion and logarithmic-time condition agree with this audit. The
background mass transversality is expressly not promoted to a global
GL-family map. Reviewer I owns its independent constraint derivation;
this audit does not count that shared result as another independent proof.

### Quantitative model disposition

| Model output for the explicit $A_0=10^{-4}$ preparation | p=4 | p=9/2 |
|---|---:|---:|
| Model time to radius-contrast proxy 0.15, $t/r_0$ | 90.6574 | 93.1559 |
| Postprocessed $E/M_0$ by that event | $2.77631\times10^{-6}$ | $2.46370\times10^{-6}$ |
| Postprocessed $E/M_0$ by model time 320 | $4.63396\times10^{-6}$ | $3.09792\times10^{-6}$ |
| Complete closure relative residual | 0.630817 | 0.634563 |
| Sampled first-order frame proxy $39.47\max A$ | 1.08313 | 0.975411 |

Every number in this table is a **calibrated heuristic model result**,
with no physical truncation-error bound. Closure spread is neither a
confidence interval nor an enclosure of vacuum radiation. The omitted
emission mass residual equals the postprocessed energy at this order;
heat and scalar free-data variations remain explicit. The numerical times
cannot be calibrated to the published first-bulge times without relating
the very different seed normalizations and horizon criteria. A timing
coincidence is not a radiation measurement.

This is a substantial, falsifiable reduced alternative with an explicit
closure obstruction, rather than a complete projection of the exact SO(3)
system. It is scientifically usable as such. It supplies no controlled
nonzero physical $A_*$, no physical two-sided finite-window energy, and no
certified actual first-bulge radiation fraction. The central remaining
dependencies are global charge-fixed exact data, physical source-enclosed
regular metric norms, a nonlinear redshift/exterior radiation trace, and
a bounded completed fourth-order news. Stabilized transport remains a
separate matched-EFT hypothesis. The final report must preserve these
**NO / NO / NO / CONDITIONAL MODEL** distinctions.

### Final explanatory-note inspection and bounded modal repair

The complete primary model and separate EFT notes were inspected. They
explicitly retain nonlinear matching, entropy, reaction and physical-data
gaps; the pulse-ratio identities above are consistent. A small heat-value
typo and the request to state the large discarded-mode residual numerically
were sent to the primary author for correction. These do not alter the
derived waveform or energy.

The primary author then chose the second/final substantive model repair:
add a displacement $b_2\sin(2kq)/(2k)$ while retaining the same effective
action, viscosity and transfer assumptions. This auditor independently
expanded it before viewing its implementation. With
$J=1+b_1\cos kq+b_2\cos2kq$, the relevant weak coefficients are

$$M_{22}(0)={1\over16k^2},\quad M_{21}={b_1\over32k^2}+O(b^2),
\quad \partial_2M_{11}(0)=-{1\over16k^2},$$
$$[b_1^2b_2]V={3\over64}-{3pk^2\over32k_c^2},\qquad
D_{21}=-{\nu b_1\over4}+O(b^2).$$

Consequently the forced growing fluid response $b_2=cb_1^2$ has

$$c=-{3\Omega^2/(32k^2)+3/64-3pk^2/(32k_c^2)-\nu\Omega/4
\over M_{22}(4\Omega^2+8\nu k^2\Omega-\Gamma_2^2)},\qquad
\Gamma_2^2=2k^2(1-4k^2/k_c^2).$$

The first kinematic numerator includes both $M_{21}\ddot b_1$ and
the Christoffel contribution; omitting the first would change its
coefficient. The independent exact script verifies all these factors.
Choosing $\dot b_2(0)=2\Omega cb_1(0)^2$ fixes the growing-history
fluid particular at that order, not the physical forced gravitational
q22. Contrast for this repair must be computed from actual extrema of
$J=1+b_1x+b_2(2x^2-1)$ on $x\in[-1,1]$, including its interior vertex
where present. This modal repair does not repair constitutive finite-k,
entropy or radiation-matching errors. Its actual final implementation
and output disposition follows.

The complete `verification/goal004_two_mode.py`, results JSON, saved
two-mode waveform tables and final section 8 of the model note were
inspected. Its matrix inertia, kinetic connection, potential derivatives,
positive Rayleigh matrix and full Euler--Lagrange residual agree with the
independent variations above. The initial velocities have the required
factor two, and the actual contrast includes the interior J vertex.
The audit independently integrates its saved scalar news, evaluates
its mass/heat functional using a distinct 256-node Gauss angular rule,
recomputes its weak coefficient and geometry, and compares its independently
computed acceleration with the derivative of a spline of the saved
velocity table. Maximum acceleration differences are
$3.1\times10^{-10}$ and $7.9\times10^{-11}$; sampled model energy
variations are below $5.9\times10^{-16}$; contrast differences are below
$1.2\times10^{-16}$. These are finite table/model consistency diagnostics,
not continuum error certificates or physical norm estimates.

| Two-mode model output | p=4 | p=9/2 |
|---|---:|---:|
| Dynamic $b_2/b_1^2$ coefficient | 0.809483 | 0.905512 |
| Incorrect static replacement | 1.53786 | 1.72030 |
| Time to contrast proxy .20, $t/r_0$ | 92.3470 | 92.9630 |
| Postprocessed $E(.20)/M_0$ | $1.51556\times10^{-5}$ | $1.48444\times10^{-5}$ |
| Postprocessed $E(320)/M_0$ | $4.10952\times10^{-5}$ | $2.89287\times10^{-5}$ |
| Complete discarded-mode relative residual | 0.794902 | 0.799682 |

Both retained equations and heat balance are implemented consistently.
The repair reaches model contrast .2, changes scalar energy by nearly
an order of magnitude compared with one mode, and leaves a still larger
next-mode residual. It does **not** demonstrate modal convergence, a
stable nonlinear reduced solution, physical vacuum saturation or a
controlled first-bulge estimate. No third repair was attempted. These
negative findings strengthen the need to retain the model qualification;
they must not be compressed into a small two-kernel error band.

The final model/EFT notes incorporate the three diagnostic repairs and
retain the physical failures. This reviewer accepts the independent
identities, the conditional data/news inequalities, the new kinematic
pulse ratios and the arithmetic of the four explicit effective ODEs.
The overall scientific conclusion remains **MEANINGFUL_PARTIAL** if the
other source/geometry audit finds no central defect: **NO** certified
physical amplitude interval, **NO** physical finite-window two-sided
energy, **NO** certified actual first-bulge fraction, and **CONDITIONAL**
falsifiable calibrated models with demonstrated nonconvergence. Audit
scope is mathematical review and manufactured verification; all source
and global spacetime closure gates retain their independent open status.
This completes the bounded review and its single substantive repair
response. No further Goal, simulation, publication or collaborator
message is launched by this audit.
