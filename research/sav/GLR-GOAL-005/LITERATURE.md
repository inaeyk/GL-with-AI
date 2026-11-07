# Primary literature and scope of comparison

GOAL-005 literature review, 2026-11-07. The scientific input is the
`research/sav-radiation` tree at `96339836a1281c219917f900224e7c98cf62f3f8`.
The convention comparisons use [NOTATION.md](../NOTATION.md) and the
[prior source register](../SOURCES.md). The source notes below use the
indicated parts of the versioned primary texts. The earlier benchmark
profiles and numerical growth rate retain their evidence status.

## Linear mode and finite-amplitude context

1. **Ruth Gregory and Raymond Laflamme**, *Black Strings and p-Branes
   are Unstable*, PRL 70, 2837 (1993). Equations (1)–(4) give the
   product background and linear TT Lichnerowicz system. The discussion
   before Fig. 2 chooses the future-horizon-regular growing branch and
   the decaying spatial branch. Figure 2 reports numerical dispersion
   curves. The compactification argument quantizes the wavenumber. The
   figure supplies historical numerical evidence for instability, with
   no new digitization or error bound used here.
   [arXiv:hep-th/9301052v2](https://arxiv.org/html/hep-th/9301052v2),
   [journal](https://doi.org/10.1103/PhysRevLett.70.2837).

2. **Sam C. Collingbourne**, *The Gregory–Laflamme Instability of the
   Schwarzschild Black String Exterior*, JMP 62, 032502 (2021).
   Theorem 1.1 and Propositions 3.7, 4.1 and 4.6 give a non-pure-gauge
   growing mode, smooth on a slice reaching the future horizon and
   exponentially decaying toward spatial infinity. With $r_0=2M$,
   the paper's $|\omega|r_0\in[0.3,0.8]$ includes $kr_0=2\pi/10$.
   Its $\mu$ is our $\Omega$ and the result gives
   $\Omega r_0>1/(20\sqrt{10})$. Periodization at $k=2\pi/L$ is
   described after Proposition 3.7. Sections 3.2–3.4 supply the ODE,
   reconstruction, ingoing EF regularity and asymptotics. Equations
   (127)–(133) specify the scaled master and potential, and Proposition
   4.2 gives the two endpoint classes. These establish qualitative
   existence and the asymptotic mode structure at L10. A tight growth
   bracket, reconstruction error in a regular weighted derivative
   norm, and a normed source error require additional estimates.
   [arXiv:2007.08441v1](https://arxiv.org/html/2007.08441v1),
   [journal](https://doi.org/10.1063/5.0043059).

3. **Steven S. Gubser**, *On non-uniform black branes*, CQG 19, 4825
   (2002). Section 3.1 and Eq. (3) set a static ansatz at the
   threshold. Section 3.4, Eqs. (17)–(18), treats the quadratic
   zero harmonic with charge and asymptotic conditions. Section 3.5
   treats the second harmonic and higher orders. The hierarchy
   compares with the harmonic content of the dynamic calculation.
   The stationary branch supplies no causal radion waveform or
   dynamical amplitude equation at L10.
   [arXiv:hep-th/0110193v2](https://arxiv.org/pdf/hep-th/0110193v2).

4. **Luis Lehner and Frans Pretorius**, *Black Strings, Low Viscosity
   Fluids, and Violation of Cosmic Censorship*, PRL 105, 101102 (2010).
   The results and Figs. 1–3 give numerical horizon geometry and
   shrinking satellite generations in five-dimensional vacuum. Their
   cascade interpretation and pinch-off estimate concern a developed
   instability. The paper supplies horizon morphology and timescale
   context for GOAL-005.
   [arXiv:1006.5960v3](https://arxiv.org/html/1006.5960v3),
   [journal](https://doi.org/10.1103/PhysRevLett.105.101102).

5. **Pau Figueras, Tiago França, Chenxia Gu and Tomas Andrade**, *The
   endpoint of the Gregory-Laflamme instability of black strings
   revisited*, PRD 107, 044028 (2023). Section II.4, Eqs. (6)–(10),
   gives the apparent-horizon extraction and areal radius. Section
   III and Table 1 include $r_0=1,L=10$ and discuss seed sensitivity
   of first formation. Section III.1 discusses radiation by comparing
   horizon areas and by analogy with other systems. It supplies no
   radion news or energy measurement. Its sub-$0.1\%$ example concerns
   five-dimensional asymptotically flat head-on collisions. The
   simulation scale and horizon contrasts are relevant comparison
   observables. A common seed normalization and Bondi clock would be
   required for a quantitative waveform comparison.
   [arXiv:2210.13501v2](https://arxiv.org/html/2210.13501v2),
   [journal](https://doi.org/10.1103/PhysRevD.107.044028).

## Exact constraint data, asymptotic charges and exterior evolution

6. **István Rácz**, *Constraints as evolutionary systems*, CQG 33,
   015014 (2016). Section 3 splits the constraints, and Section 4.1,
   Eqs. (4.1)–(4.9) and Theorem 4.1, gives a short radial-interval
   existence result for the symmetric-hyperbolic/parabolic system.
   Strict parabolicity and a positive lapse are part of the construction.
   Immediately after Theorem 4.1 the paper raises global continuation
   as a separate question. This is the local comparison for a radial
   constraint method. Global lapse control, weighted momentum estimates,
   and a mass parameter for the GL family require their own argument.
   [arXiv:1508.01810v3](https://arxiv.org/html/1508.01810v3),
   [journal](https://doi.org/10.1088/0264-9381/33/1/015014).

7. **Florian Beyer, Jörg Frauendiener and Joshua Ritchie**,
   *Asymptotically flat vacuum initial data sets from a modified
   parabolic-hyperbolic formulation of the Einstein vacuum constraint
   equations*, PRD 101, 084013 (2020). Sections 3–5 and Result 2
   analyze modified free data and radial expansions. Sections 6–7
   add numerical evidence from Schwarzschild-like and binary-like
   examples. The exterior slices in Section 4 are three-dimensional,
   foliated by $S^2$. The GL slice here has dimension four and leaves
   $S^2\times S^1$ with a compact derivative at a fixed length scale.
   The paper provides a comparison for radial constraints with
   asymptotic targets. Its evidence concerns a different end and
   cannot establish global existence or charge control on the GL slice.
   [arXiv:2002.06759v1](https://arxiv.org/pdf/2002.06759v1),
   [journal](https://doi.org/10.1103/PhysRevD.101.084013).

8. **Hamed Barzegar, Piotr T. Chruściel and Michael Hörzinger**,
   *Energy in higher-dimensional spacetimes*, PRD 96, 124002 (2017).
   Equations (II.1)–(II.4) define their spatial KK end and decay, and
   Section III.1 gives Hamiltonian charges. The decay condition includes
   all coordinate derivatives, including the compact one. In our three
   noncompact spatial dimensions it requires
   $\partial_\mu g_{\nu\rho}=o(r^{-3/2})$. Pointwise $O(r^{-1})$
   metric falloff alone permits $z$-dependent leading tails that fail
   this condition, even when the averaged boundary integral has a
   limit. The paper separates Hamiltonian, ADM-type and Komar
   definitions. Applying a charge formula requires checking the end,
   generator and decay hypotheses in the chosen frame.
   [arXiv:1708.03122v2](https://arxiv.org/html/1708.03122v2),
   [journal](https://doi.org/10.1103/PhysRevD.96.124002).

9. **Cécile Huneau, Annalaura Stingo and Zoe Wyatt**, *The global
   stability of the Kaluza–Klein spacetime*, JEMS (2025, online first).
   Theorems 1.1 and 1.2
   establish nonlinear small-data evolution about flat KK spacetime on
   $\mathbb R^3\times S^1$. Section 1.2 discusses exact zero-mode
   reduction. Equations (1.7)–(1.10) separate a prescribed
   Schwarzschild-shaped mass tail from a small, faster decaying
   perturbation. Theorem 1.1 states $g_0^1=O(r^{-1-\kappa})$ and
   $K_0=O(r^{-2-\kappa})$ with $\kappa>0$. The precise weighted
   estimates in (1.10) include compact derivatives. Section 3 and
   Proposition 3.1 treat the exterior independently of the interior
   bootstrap, which makes their exterior method relevant for early
   cuts. The result propagates weighted energies with time growth.
   A direct application would need the stronger weighted initial
   bounds and control along actual Bondi cuts, including the
   positive-mass logarithmic shift relative to $t-r$. Neither
   theorem states differentiability of the nonlinear solution-to-news
   map or a uniform remainder for a horizon-crossing GL family.
   [arXiv:2307.15267v1](https://arxiv.org/html/2307.15267v1),
   [journal](https://ems.press/journals/jems/articles/14298963).

## Radion news, causal propagation and tails

10. **Christian Ferko, Gautam Satishchandran and Savdeep Sethi**,
    *Gravitational Memory and Compact Extra Dimensions*, PRD 105,
    024072 (2022). Section 3 assumes a Bondi expansion (3.5).
    Lemmas 3–4 and Eqs. (4.18)–(4.23), (4.46) express the radiative
    curvature in terms of generalized news and identify its
    volume-mode component. In five dimensions, the internal trace
    coefficient in their convention is $4\Phi/\sqrt{3}$ for the
    [project radion](../NOTATION.md). Use the generalized-news norm
    in (5.28), its decomposition (4.20)–(4.22), and the local
    constraint (5.33) for the geometric flux comparison. Section 6.4,
    Eqs. (6.43)–(6.49), distinguishes the Einstein-frame tensor
    news from the breathing contribution. The integrated memory theorem
    uses stationary endpoint conditions. The asymptotic analysis
    identifies a radiative degree of freedom given the expansion.
    It does not prove that an initial-data family develops that
    expansion or fix its free scalar data.
    [arXiv:2109.11599v3](https://arxiv.org/html/2109.11599v3),
    [journal](https://doi.org/10.1103/PhysRevD.105.024072).

    For the circle, the two-sphere trace and internal component give
    $\mathcal N^{ab}\mathcal N_{ab}=\tfrac32(\partial_u g_{zz}^{(1)})^2$
    in the pure volume channel. With the normalization above, (5.28)
    and the solid-angle integral reproduce the reduced-action factor
    $1/G_4$ for $(\partial_u\Phi)^2$. The isolated scalar split (6.18)
    as printed in arXiv v3 differs from the contraction in (5.28)
    with the news of (4.20)–(4.22). The comparison uses the full
    geometric norm. The publisher's rendering of (6.18) was unavailable.

11. **Dean Baskin and Fang Wang**, *Radiation fields on Schwarzschild
    spacetime*, CMP 331, 477–506 (2014). Theorem 1.1 and Section 3
    relate endpoint expansion of the scalar data on a Schwarzschild
    time slice to radiation-field regularity. Theorem 1.3 and Section
    4 give a finite-energy identity for the pair of homogeneous
    scalar traces at the horizon and null infinity. Theorem 1.4
    and Section 5 concern support of homogeneous traces. These results
    give a fixed-background benchmark for the energy and news spaces.
    The GL calculation separately needs a source-driven trace on its
    selected slice. The homogeneous theorem contains no sign result
    for the resulting source integral and no nonlinear KK remainder.
    [arXiv:1305.5273v3](https://arxiv.org/html/1305.5273v3),
    [journal](https://doi.org/10.1007/s00220-014-2047-4).

12. **Hiroko Koyama and Akira Tomimatsu**, *Asymptotic tails of massive
    scalar fields in Schwarzschild background*, PRD 64, 044014 (2001).
    Section II, Eqs. (4)–(20), gives the free massive scalar radial
    equation, horizon/infinity solutions, Wronskian Green function
    and the threshold branch cut. Sections III–IV distinguish
    mass and time regimes for oscillatory tails. Their field is a
    test scalar, useful for massive-radion EFT comparisons. The
    paper supplies neither a sourced massive spin-2 inverse nor
    a timelike-energy coefficient for the $n=2$ GL harmonic.
    [arXiv:gr-qc/0103086v2](https://arxiv.org/html/gr-qc/0103086v2),
    [journal](https://doi.org/10.1103/PhysRevD.64.044014).

## Boundaries of the source search

The focused searches paired Gregory–Laflamme with radion radiation,
zero-mode waveform, nonlinear instability and exterior Cauchy estimates.
The constraint searches targeted radial/parabolic formulations and
KK-asymptotic charges. The radiation searches targeted compactified
Bondi news and Schwarzschild traces. Within the primary texts above,
no result supplies a uniform differentiable family-to-news map for
horizon-crossing vacuum GL data specified only by $h=O(r^{-1})$ and
$K=O(r^{-2})$ with no gain from compact differentiation. Stronger
end conditions might permit a new argument. Compact-region Cauchy
control alone supplies no estimate on the unbounded fixed-cut domain.

The literature already supplies the linear instability, radial
constraint framework, kinematic scalar news and homogeneous
Schwarzschild scattering. The specific prospective contributions are
a global weighted lapse/momentum estimate for the GL-tangent constraint
family and a sign/lower bound for an early, zero-scalar-data **forced**
news coefficient from the exponentially decaying GL source. The
inspected sources contain neither calculation. The strength of either
claim depends on the derivation and verification in this dossier.
In particular, early-cut positivity of the quadratic trace concerns
a finite-data perturbative signal and does not establish a nonlinear
Bondi-news error or first-bulge energy. The search was selective and
establishes no priority claim.
