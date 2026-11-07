# Literature comparison and source audit

Search date 2026-11-07. Spacetime dimension $D$ counts the compact
direction where present. Numerical values in this record are reported
literature results. No spectrum or nonlinear evolution was rerun.

## Five-dimensional Kerr string

**East and Siemonsen, _Instability and backreaction of massive spin-2
fields around black holes_.** Equation (1) and §II specify a transverse,
traceless massive spin-2 field on a Ricci-flat Kerr background. Section III
and Fig. 1 compare the axisymmetric GL-connected branch with the
$m=1,2,3$ superradiant branches using time-domain growth rates. The
axisymmetric threshold in $\alpha=M\mu$ increases from about $0.44$
at zero spin to $1.73$ at $\chi=0.998$. At small $\alpha$ they
report $\operatorname{Im}\omega\simeq0.62\mu$ for the GL-connected
branch. The axisymmetric mode grows faster across most of the explored
region. Their dominance estimates were $\chi>0.95$ for $m=1$, slightly
below $0.95$ for $m=2$, and about $0.7$ for $m=3$. The last
estimate uses a real-frequency extrapolation as described in §III
footnote 2. Section IV and Figs. 2–3 evolve axisymmetric four-dimensional
Einstein–Weyl gravity, with the Weyl-squared action in Eq. (3). Its massive
variable is the trace-free Ricci tensor, so the effective metric backreaction
discussed there is linear in that variable. In a vacuum KK string, the linear
massive mode has zero circle average and a neutral response begins
quadratically in its amplitude. The Einstein–Weyl mass evolution and
ghost-cloud saturation apply to their model.
[PRD 108, 124048 (2023)](https://doi.org/10.1103/PhysRevD.108.124048),
[inspected arXiv:2309.05096v2 PDF](https://arxiv.org/pdf/2309.05096v2).

**Dias, Lingetti, Pani and Santos, _Black hole superradiant instability
for massive spin-2 fields_.** The original Kerr calculation solves
coupled elliptic equations. Main Eq. (1), the growth-rate comparison and
Appendices A–B document the massive spin-2 boundary problem, including
the rapidly growing special dipole. The spectral calculation supplies
a linear comparison for the KK modes. Its radiation applications concern
massive boson clouds.
[PRD 108, L041502 (2023)](https://doi.org/10.1103/PhysRevD.108.L041502),
[inspected arXiv:2304.01265v2](https://arxiv.org/html/2304.01265v2).

**Dias and Santos, _Separation of variables for massive spin-2 fields
on the Kerr black hole_.** The 2026 preprint explicitly gives the KK
equivalence following Eq. (1). The joint angular problem and physical
radial equation are Eqs. (6) and (13), with a reconstruction theorem
and qualifications in supplemental §S7. Their Results and Figs. 1–2
compare the separated equations with earlier two-dimensional calculations
for the stationary threshold and 261 complex quasibound frequencies.
Fig. 3 compares the GL branch with $\ell=m=1,2,3$ as a function of
$(\chi,\alpha)$. The GL branch dominates at small $\alpha$. Dominance
regions first appear at $\chi\gtrsim0.62$ for $m=3$, about $0.90$
for $m=2$, and about $0.9895$ for the dipole. These thresholds
refer to the branches compared in that calculation and differ from the
earlier estimates. Supplemental §S1 and Figs. S1–S2 give their frequency
and growth-rate surfaces, including the purely growing GL branch and
the slower peak rates of the higher-$m$ branches. They also report
that the Kerr branches remain bound and unstable towards $\alpha\to0$
at fixed nonzero spin, with growth rates tending to zero. The six-dimensional
equal-spin strings have a finite marginal-binding edge. At a fixed point
on the map, a branch with a smaller peak rate can still win if the
other branches are stable there. The displayed upper boundary of
the $m=3$ region leaves $m>3$ and uncomputed branches open. The
ODE construction supersedes the 2023 expectation that a linear Kerr-string
spectrum requires a coupled PDE solve. The preprint supplies a spectral
framework for the homogeneous massive modes. The sourced mixed response
and the nonlinear tensor waveform are open. The ancillary reconstruction
code was not executed.
[Version record](https://arxiv.org/abs/2610.40780v1),
[inspected HTML](https://arxiv.org/html/2610.40780v1),
[inspected PDF](https://arxiv.org/pdf/2610.40780v1).

For a circle of proper asymptotic circumference $L$, sample these
Kerr results at $\alpha_n=2\pi |n|M/L$ for $n\ne0$.
The identification holds on any Ricci-flat product. In an $e^{ikz}$
sector one can set $h_{zz}=h_{\mu z}=0$ by first solving
$2ik\xi_z=-h_{zz}$ and then $ik\xi_\mu=-h_{\mu z}-\nabla_\mu\xi_z$.
The $zz$ and $\mu z$ vacuum equations impose the trace and divergence
constraints, and $R_{\mu\nu}^{[1]}=0$ becomes
$\delta R_{\mu\nu}^{(4)}[h]+\tfrac12 k^2h_{\mu\nu}=0$.
The $k=0$ system has separate gauge freedom. For unboosted Kerr,
positive-frequency superradiant amplification requires
$0<\operatorname{Re}\omega<m\Omega_H$. A growing bound mode additionally
requires horizon regularity and the decaying asymptotic branch of
$\sqrt{\mu^2-\omega^2}$. The GL-connected branch has $m=0$ and
persists in Schwarzschild. Both $n$ and $m$ are conserved by the
linear stationary product operator. A finite-data comparison must
account for the initial amplitudes and the discrete set of allowed
$n$. This is an inference from the cited linear equations and their
boundary conditions.
[Dias–Santos, Eq. (1), Results and supplemental §S1](https://arxiv.org/html/2610.40780v1),
[Dias–Lingetti–Pani–Santos, Appendix A](https://arxiv.org/html/2304.01265v2).

## Six-dimensional equal-spin Myers–Perry string

- **Dias, Ishii, Murata, Santos and Way, _Gregory–Laflamme encounters
  Superradiance_.** The base black hole has $D=5$ and the string
  $D=6$. Sections 3.1–3.2 and Figs. 1–2 compute the GL threshold and
  growth rates. Section 4.3 and Figs. 6–8 exhibit the finite-$m$
  gravitational superradiant windows and regions of overlap or separation
  from the GL region. Their Eq. (24) describes the confining cutoff
  and §5 distinguishes it from the superradiant onset. Their
  Hopf-angle convention gives $\omega=2m\Omega_H$ at the superradiant
  onset and $\omega=k$ at the confining cutoff. This two-spin convention
  differs from the Kerr single-azimuth condition. The argument at the
  end of §5 about arbitrarily high $m$ in Kerr is a physical expectation.
  [JHEP 01 (2023) 147](https://doi.org/10.1007/JHEP01%282023%29147),
  [inspected arXiv:2211.02672v2](https://arxiv.org/html/2211.02672v2).
- **Same authors, _Gregory–Laflamme and Superradiance encounter Black
  Resonator Strings_.** Sections 3–4 construct a helically symmetric
  black resonator branch from the $D=6$ superradiant onset. Section 7
  and Figs. 5–6 compare its nonlinear solutions with the perturbative
  solution and the uniform string, finding greater entropy where the
  resonator and uniform solutions coexist at the same charges.
  Section 5 constructs separate horizonless KK geons. Section 8
  discusses possible dynamics and competing instabilities, and leaves
  open how other perturbations affect resonators. Its mention of
  waves escaping to null infinity is a general observation about such
  dynamics. The proposed $D=5$ resonator branch is an extrapolation.
  [JHEP 02 (2023) 069](https://doi.org/10.1007/JHEP02%282023%29069),
  [inspected arXiv:2212.01400v2](https://arxiv.org/html/2212.01400v2).
- **Same authors, _Superradiance and black resonator strings encounter
  helical black strings_.** Sections 4–5 construct the distinct
  $D=6$ cohomogeneity-one helical branch. Sections 6.2–6.3,
  Fig. 8 and Eqs. (94)–(99) compare it with the uniform and resonator
  strings and explain the asymptotically timelike Killing field.
  At matched charges where all coexist, the helical entropy lies above
  the uniform string and below the resonator. This is a stationary
  solution comparison.
  [JHEP 05 (2023) 041](https://doi.org/10.1007/JHEP05%282023%29041),
  [inspected arXiv:2302.09085v2](https://arxiv.org/html/2302.09085v2).

## Tensor wave extraction and causal perturbation theory

- **Martel and Poisson, _Gravitational perturbations of the Schwarzschild
  spacetime: A practical covariant and gauge-invariant formalism_.**
  The conserved-source projections and field equations are in §§IV.B
  and V.B, the even master and sourced equation in Eqs. (4.23)–(4.27),
  and the odd Cunningham–Price–Moncrief master and sourced equation
  in Eqs. (5.13)–(5.16). These identities apply to the linear Schwarzschild
  operator acting on a higher-order coefficient once its effective
  source and gauge completion are derived. Their covariant equations
  use the orbit metric d'Alembertian. In tortoise form, both the
  potential and the source acquire a factor $f=1-2M/r$.
  The linear radiation field follows from Eq. (6.13), with harmonic
  norms from Eqs. (3.8)–(3.10). Equation (6.16) presents an averaged
  power in $G=1$ units. With $N_{AB}=\partial_u C_{AB}$ and
  $G_4=G_5/L$, the unaveraged tensor Bondi flux in this normalization
  is $\frac{1}{32\pi G_4}\int N^{AB}N_{AB}\,d\Omega$. Its
  $\ell=2$ master coefficient is $24/(64\pi G_4)$ multiplying
  the sum over $m$ and parity of $|\partial_u\Psi|^2$, since
  the squared norm of each tensor harmonic is $12$. Section V.D
  distinguishes the wave-extraction odd master from the traditional
  Regge–Wheeler variable.
  [PRD 71, 104003 (2005)](https://doi.org/10.1103/PhysRevD.71.104003),
  [inspected arXiv:gr-qc/0502028v1](https://arxiv.org/pdf/gr-qc/0502028).
- **Brizuela, Martín-García and Tiglio, _A complete gauge-invariant
  formalism for arbitrary second-order perturbations of a Schwarzschild
  black hole_.** Sections II–III and VI give nonlinear gauge-invariant and
  sourced RWZ constructions in four-dimensional GR. Eqs. (86)–(88)
  and (91)–(93) illustrate adding local lower-order terms to regulate
  the master and consistently change its source. Eqs. (95)–(98) show
  that higher-order contributions interfere with the lower-order news
  in the radiated power. Their use of order derivatives with factorial
  weights and their master normalizations must be converted before
  importing numerical prefactors. Their source formulas cover
  four-dimensional vacuum perturbations.
  [PRD 80, 024021 (2009)](https://doi.org/10.1103/PhysRevD.80.024021),
  [inspected arXiv:0903.1134v1](https://arxiv.org/pdf/0903.1134v1).
- **Lagos and Hui, _Generation and propagation of nonlinear quasi-normal
  modes of a Schwarzschild black hole_.** Eq. (12) is the parity
  product rule. Section III.1 constructs the retarded Green function
  from the ingoing/outgoing solutions, and §III.3 distinguishes the
  initial-data and source terms in the second-order response. Their
  asymptotic light-ring description and Eq. (34) neglect the branch cut.
  For a mode convention $e^{-i\omega t}$, the outgoing radial
  $e^{+i\omega r_*}$ of a damped QNM grows on a constant $t$
  slice at large $r_*$. A finite-data problem has its own regular
  initial data and the residue gives one contribution to its response.
  [PRD 107, 044040 (2023)](https://doi.org/10.1103/PhysRevD.107.044040),
  [inspected arXiv:2208.07379v2](https://arxiv.org/html/2208.07379v2).
- **Ferko, Satishchandran and Sethi, _Gravitational Memory and Compact
  Extra Dimensions_.** Section 2, Eqs. (2.8)–(2.11), distinguishes
  massive KK spin-2 from the circle-independent massless degrees of
  freedom. Eqs. (4.18)–(4.22) and (4.46) decompose the generalized
  asymptotic news into a trace-free $S^2$ tensor, mixed vector
  components, and scalars under the assumed asymptotic expansion.
  Eqs. (6.16)–(6.18) separate their fluxes. Section 6.4 and Eq. (6.48)
  show how the trace-free news agrees with the four-dimensional
  Einstein-frame tensor news. This supplies the asymptotic normalization
  and field identification. Establishing the falloff and trace for the
  specific GOAL-007 mixed finite-data solution is a separate problem.
  [PRD 105, 024072 (2022)](https://doi.org/10.1103/PhysRevD.105.024072),
  [arXiv:2109.11599v3 record](https://arxiv.org/abs/2109.11599v3),
  [inspected arXiv:2109.11599v3 HTML](https://arxiv.org/html/2109.11599v3).

## Merger and neck precedents

- **Andrade, Emparan, Licht and Luna, _Cosmic censorship violation in
  black hole collisions in higher dimensions_.** A leading large-$D$
  effective evolution forms transient rotating bars that can pinch.
  The Discussion and Eq. (4) compare the growth time of a stationary
  string proxy with an estimated radiative spin-down time. The regime
  is large $D$ with noncompact asymptotics.
  [JHEP 04 (2019) 121](https://doi.org/10.1007/JHEP04%282019%29121),
  [inspected arXiv:1812.05017v2 PDF](https://arxiv.org/pdf/1812.05017).
- **Andrade, Figueras and Sperhake, _Evidence for violations of Weak
  Cosmic Censorship in black hole collisions in higher dimensions_.**
  Sections 2 and 4 show numerical evidence from nonzero-impact-parameter
  collisions of boosted rotating black holes in noncompact $D=6,7$
  vacuum GR. The collision can create a dumbbell-shaped horizon whose
  elongated segments develop local GL-like satellites. Section 4.1.2
  and Fig. 7 attribute the first two prominent waveform peaks to merger
  and the later shape/rotation change of the dumbbell, respectively.
  Smaller subsequent features are associated with neck evolution.
  Equation (4.1) is a higher-dimensional Weyl-flux formula with
  $S^{D-2}$ angular integration. Figure 7 is a $D=7$ finite-radius
  extraction. Section 4.2 reports no $D=6$ waveform extraction.
  These collisions offer a precedent for a dynamically formed neck
  with initially stable holes. Their neck direction is noncompact.
  In an exact $g_{\rm merge}^{(4)}+dz^2$ uplift, the circle
  direction stays an isometry and the unperturbed product evolution
  has no $n\ne0$ seed. Observed growth of seeded modes on that uplift
  could be inherited from an unstable progenitor or remnant.
  Demonstrating instability created by the merger would require
  measuring the transient propagator and comparing with stable
  reference phases.
  [JHEP 03 (2022) 111](https://doi.org/10.1007/JHEP03%282022%29111),
  [inspected arXiv:2011.03049v2 PDF](https://arxiv.org/pdf/2011.03049).
- **Figueras, França, Gu and Andrade, _The endpoint of the
  Gregory–Laflamme instability of black strings revisited_.**
  Section II.1 imposes transverse $SO(3)$ in $4+1$ vacuum
  evolution, which excludes conventional trace-free $S^2$ tensor
  news. Sections III–IV document evolving thin segments and local
  satellites, and report no common timescale for higher generations.
  This is a precedent for evolution of an initially unstable KK
  string. It motivates checking local scale separation and finite
  growth time before using an instantaneous dispersion relation in
  a transient neck.
  [PRD 107, 044028 (2023)](https://doi.org/10.1103/PhysRevD.107.044028),
  [inspected arXiv:2210.13501v2](https://arxiv.org/html/2210.13501v2).

## Scoped novelty search

Queries combined Gregory–Laflamme or nonuniform black strings with
ringdown, mode coupling, massless tensor, Bondi news, radion, compact
extra dimensions, and merger necks. The closest alternatives include
Seahra–Clarkson–Maartens brane-world black-string spectroscopy, which
probes massive KK oscillations in a two-brane model, and the
four-dimensional nonlinear Schwarzschild coupling and compactified
asymptotic analyses above. The six-dimensional resonator/helical
bifurcations are nonlinear massive-mode backreaction in vacuum and
constitute a separate comparison class. The reviewed sources did
not yield a computed finite-data $n=0,\ell=2$ Bondi news projection
of the $(n=\pm1,\ell=0)^2\times(n=0,\ell=2)$ coupling in pure
five-dimensional Einstein gravity. This bounded search leaves priority
beyond the inspected material open.
[Seahra–Clarkson–Maartens, PRL 94, 121302 (2005), abstract](https://doi.org/10.1103/PhysRevLett.94.121302).
