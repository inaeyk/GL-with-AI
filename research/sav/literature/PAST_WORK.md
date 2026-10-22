# Historical literature and radiation gap — selective source-checked review
Review date: 2026-10-22. Scope: 5D neutral uniform black string on R^{1,3} x S^1,
exact SO(3) symmetry, initial bulge and radiation; not a systematic census
of every black-string paper.

## Complement to Michael's existing notes

The parent repository already contains a detailed horizon-reproduction plan,
Stage 3 derivations, GRChombo code comparisons and the Milestone 1/2 evidence;
see `research_plan/reproduction_plan.md`, `docs/derivations/`,
`research_plan/stage_checklists.md`. Its `research_plan/radiation_profile_plan.md`
is explicitly a placeholder. In the research-plan/derivation material reviewed,
there was no separate bibliographic survey of SO(3)-invariant radiation or
second-order radion sourcing. This file fills that specific gap, and does
**not** replace or edit Michael's scientific history.

## Primary foundational papers

1. **Gregory and Laflamme (1993, 1994).**
   "Black Strings and p-Branes are Unstable" (PRL 70, 2837,
   https://doi.org/10.1103/PhysRevLett.70.2837);
   "The Instability of Charged Black Strings and p-Branes"
   (Nucl. Phys. B428, 399, https://arxiv.org/abs/hep-th/9404071).
   Physical exponentially growing, long-wavelength compact modes; provides
   linear perturbation and critical-wavenumber foundation. **Not** a
   nonlinear radiated-energy computation.

2. **Gubser (2002),** "On non-uniform black branes"
   (CQG 19, 4825, https://arxiv.org/abs/hep-th/0110193).
   Perturbative static nonuniform solutions near threshold, evidence for a
   first-order transition in D=5. Distinguish static backreaction from
   time-dependent outgoing radion radiation: neither source nor flux follows
   merely from a static second-order solution.

3. **Sorkin (2004),** "A Critical Dimension in the Black-String Phase
   Transition" (PRL 93, 031601,
   https://doi.org/10.1103/PhysRevLett.93.031601).
   Critical-dimension/phase-structure analysis. Motivates caution about
   applying threshold Landau expansions deep into D=5 dynamics.

4. **Kudoh (2006),** "Origin of black string instability"
   (PRD 73, 104034, https://doi.org/10.1103/PhysRevD.73.104034).
   Tensor/vector/scalar spherical perturbations: exceptional s-wave GL
   instability. Useful cross-check that SO(3) does not make the GL
   deformation pure gauge or remove all dynamical degrees of freedom.

5. **Collingbourne (2021),** "The Gregory–Laflamme Instability of the
   Schwarzschild Black String Exterior" (J. Math. Phys. 62, 032502,
   https://doi.org/10.1063/5.0043059; arXiv:2007.08441).
   Rigorous linear instability using a reduced ODE/Schrodinger functional;
   includes a non-pure-gauge criterion. High-priority independent source for
   admissible GL eigenmode normalization and horizon regularity.

## Nonlinear evolution and first-bulge conjecture

6. **Lehner and Pretorius (2010),** "Black Strings, Low Viscosity Fluids,
   and Violation of Cosmic Censorship" (PRL 105, 101102,
   https://arxiv.org/abs/1006.5960).
   Fully nonlinear 5D horizon evolves into blobs connected by thinner
   strings and later satellites; finite-time pinch-off indicated by their
   simulation. Geometry/cascade evidence is not a gauge-invariant flux.

7. **Figueras, França, Gu and Andrade (2023),**
   "The endpoint of the Gregory-Laflamme instability of black strings
   revisited" (PRD 107, 044028;
   https://arxiv.org/html/2210.13501v2).
   Independent GRChombo/CCZ4 reproduction at L/r0=8,10,12,16; different
   late-generation timelines and *no global generation timescale*.
   Crucially, section III.1 explicitly **conjectures** a strong initial
   bulge radiation burst and diminishing late-generation radiation.
   It did **not** extract SO(3)-invariant radion news or a KK-mode energy
   spectrum. The head-on collision energy comparison there is a heuristic
   analogy, not a derived KK scalar bound.

   Their Table 2 first-generation `(t_p,t_n)/r0`:
   L=8: (163.1,174.7); L=10: (93,107); L=12: (78,96.4);
   L=16: (91,112.7). Those are geometric formation diagnostics, **not**
   emitted-waveform duration or arrival time.
   Their L=10 Table 1 blob radii: first 2.04 r0, second 0.299 r0.
   A model with radiative energy proportional to radius-squared predicts a
   smaller second contribution **only if** efficiencies and multiplicities
   do not compensate; it is not a measurement.

## Asymptotic radiation, KK energy, effective models

8. **Lü, Mao and Wu (2019),** "Asymptotic Structure of
   Einstein-Maxwell-Dilaton Theory and Its Five Dimensional Origin"
   (JHEP 11 (2019) 005, https://arxiv.org/abs/1909.00970).
   Explicit Bondi gravitational, electromagnetic, and scalar news in 4D
   EMD/KK theory and 5D lift. Most directly relevant external source to
   establish massless SO(3)-invariant scalar radiation with normalization.
   Independently re-derive its flux in our action conventions.

9. **Harmark and Obers (2004),** "General Definition of Gravitational
   Tension" (JHEP 05 (2004) 043,
   https://arxiv.org/abs/hep-th/0403103).
   ADM tension and charge definitions for KK asymptotics. Stationary
   mass/tension/radion-charge relations do not by themselves give radiation
   from dynamical bulges.

10. **Emparan, Harmark, Niarchos, Obers (2010),** "New Horizons for Black
    Holes and Branes" (JHEP 04 (2010) 046,
    https://doi.org/10.1007/JHEP04(2010)046).
    Blackfold effective theory, a candidate long-wavelength description.
    Leading neutral string fluid sound speed squared is negative; accuracy
    in the D=5, k r0~O(1) growth regime is not guaranteed.

11. **Suzuki and Tanabe (2015),** "Non-uniform black strings and the
    critical dimension in the 1/D expansion",
    https://arxiv.org/abs/1506.01890.
    Effective near-horizon/far-zone matched large-D treatment of nonuniform
    strings. Useful methodology, but 1/D extrapolation to D=5 needs checks.

## Specific unresolved literature question

A targeted keyword and reference search did not identify an established
published numerical value for the **retarded, second-order n=0 radion
radiation transfer coefficient sourced by one normalized growing D=5 GL
mode**. This is a research lead, *not a verified novelty or priority claim*.
GOAL-001 must perform a deeper literature audit (including cited works and
alternative terminology) before claiming originality.

## Working citations / warnings

- Do not relabel `SO(3)` as `SO(4)`; rotations are of the transverse S^2.
- Do not equate horizon-area change with radiated energy.
- Do not equate conserved KK momentum k_n with outgoing radial momentum
  q_n or conclude that a growing mode itself is a radiative oscillation.
- Distinguish fixed-time Schwarzschild and horizon-penetrating coordinates.
- Do not quote first-bulge radiated mass fractions from these papers:
  they do not give such a measurement for this setup.
