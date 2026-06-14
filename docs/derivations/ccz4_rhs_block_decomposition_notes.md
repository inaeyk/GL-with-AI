# CCZ4 RHS Block Decomposition Notes

Status: Stage 3H planning and review scaffold. This is documentation only. It
does not derive the complete CCZ4 right-hand side, implement C++ source terms,
add variables to `BlackStringToy`, or prove that the current scaffold evolves a
physical 5D black string.

## Purpose

Stages 3C through 3G validated increasingly detailed pieces of the
modified-cartoon geometry and conformal-cartoon algebra. Stage 3H changes the
shape of the work: the future 4+1 CCZ4 / SO(3) modified-cartoon right-hand side
must be decomposed into smaller blocks before any implementation starts.

The goal is to avoid a giant uncheckable RHS. Each block should have an
explicit validation route before it is translated into C++:

- exact symbolic or numerical identities where available;
- diagonal/off-diagonal regression checks;
- flat, Minkowski, or sheared-flat zero checks;
- uniform black-string stationarity checks;
- constraint-preservation checks;
- comparison with Pau's reference implementation;
- convergence-only checks for nonlinear behavior that has no compact exact
  target.

Stage 3H is intentionally less exactly checkable than Stages 3C through 3G.
The geometry and algebra stages had many identities such as determinant
normalization, inverse-metric round trips, Ricci-flat gates, and trace-free
projections. The full nonlinear CCZ4 RHS will not. A green symbolic check for
one block must not be treated as proof of physical correctness for the
complete evolution.

## Conventions And Variables

The project conventions remain:

- physical spatial dimension: `GR_SPACEDIM = 4`;
- Chombo/grid dimension: `CH_SPACEDIM = 2`;
- gridded directions: `x,z`;
- hidden directions: two equivalent `w` directions representing the transverse
  `S^2`;
- hidden multiplicity:

```text
N_hidden = GR_SPACEDIM - CH_SPACEDIM = 4 - 2 = 2.
```

The hidden component `hww` is a Cartesian-like conformal metric component. It
is not `g_theta theta`. The spherical/cartoon reconstruction supplies the
external factor `x^2`, so

```text
g_theta theta = x^2 gamma_ww = x^2 hww / chi.
```

All traces, trace-free projections, and dimension denominators are full
four-spatial-dimensional operations. In particular, the conformal extrinsic
curvature decomposition uses `/4`, not `/2` and not ordinary 3+1 intuition.

The relevant future evolved geometric variables are expected to include:

- `chi`;
- `h_xx`, `h_xz`, `h_zz`, `hww`;
- `A_xx`, `A_xz`, `A_zz`, `Aww`;
- `K`;
- `Theta`;
- hatted conformal connection variables `hat_Gamma^A`, with `A in {x,z}`;
- lapse, shift, and gauge-driver variables as needed later.

The connection-sector variable should follow the GRChombo-facing CCZ4 basis

```text
hat_Gamma^i = tilde_Gamma^i + 2 Z^i.
```

Thus the implementation blueprint should evolve `hat_Gamma^A` on the reduced
grid, with `Z^i` understood as the encoded Z4 constraint vector inside
`hat_Gamma^i`. `Z^i`-based language remains useful for constraint-propagation
analysis, but the implementation plan should use the `hat_Gamma^i` basis to
avoid an extra translation layer.

Stage 3H does not decide final C++ enum names or storage order.

## RHS Block Decomposition

The table below is a planning map, not a final equation list. Each row names a
future implementation block, the term families it must contain, and the
validation route that should exist before coding.

Term-origin tags:

- `[inherited]`: standard CCZ4/GRChombo-like terms;
- `[cartoon-new]`: modified-cartoon hidden-sector additions most likely to be
  absent from public GRChombo for this 5D/SO(3) target;
- `[mixed]`: terms where standard CCZ4 structure and hidden-sector bookkeeping
  interact;
- `[reference/convergence]`: terms expected to need Pau/reference-code
  comparison or convergence evidence.

| RHS block | Term-origin tags | Term families to separate | Planned validation route |
| --- | --- | --- | --- |
| `partial_t h_ij` | `[inherited]`, `[cartoon-new]`, `[mixed]` | Standard conformal-metric advection/stretching terms; determinant-preserving projection; hidden `hww` evolution; off-diagonal `h_xz` terms | Flat/Minkowski zero or pure-gauge check; diagonal Stage 3F limit; off-diagonal Stage 3G algebra; reference-code comparison for exact projection/enforcement choices |
| `partial_t chi` | `[inherited]`, `[mixed]` | Standard conformal-factor trace/gauge terms; full 4D trace dependence; hidden multiplicity through `K` and divergence terms | Flat/Minkowski check; uniform-string stationarity in chosen gauge; comparison with Pau's implementation for gauge and damping conventions |
| `partial_t K` | `[inherited]`, `[cartoon-new]`, `[reference/convergence]` | Standard trace evolution; lapse Laplacian; 4D Ricci scalar; CCZ4 damping and matter-free terms; hidden Ricci source terms | Exact flat and sheared-flat geometry-source gates; uniform-string stationarity; Hamiltonian-constraint consistency; reference-code comparison for damping/source conventions |
| `partial_t A_ij` | `[inherited]`, `[cartoon-new]`, `[mixed]`, `[reference/convergence]` | Trace-free Ricci/lapse-Hessian block; `K A_ij` and quadratic `A` terms; hidden `Aww` block; off-diagonal `A_xz` terms; full 4D trace-free projection | Stage 3F/3G trace-free algebra; flat/sheared-flat zero checks; diagonal/off-diagonal regression; reference-code comparison for nonlinear trace-free blocks |
| `partial_t Theta` | `[inherited]`, `[cartoon-new]`, `[mixed]`, `[reference/convergence]` | CCZ4 constraint-propagation terms; Hamiltonian-constraint coupling; damping; lapse/gauge factors; hidden-sector scalar curvature contributions | Constraint-violation injection tests; uniform-string constraint preservation outside turduckening; reference-code comparison for damping signs and coefficients |
| `partial_t hat_Gamma^A` | `[inherited]`, `[cartoon-new]`, `[mixed]`, `[reference/convergence]` | Hatted conformal connection evolution; encoded `Z^i` constraint-vector contributions; momentum-constraint coupling; connection/Gamma terms; hidden-direction divergences; off-diagonal `x,z` terms; lapse/shift/Gamma-driver occurrences inside this RHS | Momentum-constraint checks; diagonal limit; sheared-flat geometry guard for connection handling; symbolic dimensional extraction for `Theta`, `Z^i`, and `hat_Gamma^i` terms; reference-code comparison |
| Gauge/lapse/shift RHS | `[inherited]`, `[reference/convergence]` | Lapse slicing, shift driver, `B` variables if used, advection choices, possible dependence on `K`, `Theta`, and connection functions | Flat/pure-gauge sanity checks; uniform-string stationarity or documented gauge startup behavior; reference-code comparison required |
| Algebraic enforcement | `[mixed]` | `det h_4D = 1`; trace-free `A`; hidden multiplicity; denominator `/4`; post-step or RHS-level projection choices | Stage 3F/3G determinant, round-trip, and tracelessness scripts; explicit Pau comparison for enforcement timing and hidden-sector participation |
| Regularity/small-`x` placeholders | `[cartoon-new]`, `[reference/convergence]` | `1/x`, `1/x^2`, hidden-sector regular limits, turduckening cutoff behavior, axis/origin treatment | Deferred to Stage 3I; exact regular limits where possible; numerical regularity indicators before production runs |

Ownership rule for gauge coupling: the gauge block owns the evolution equations
for lapse, shift, and any Gamma-driver auxiliary variables. The
`hat_Gamma^A` block owns the occurrences of lapse, shift, and Gamma-driver
quantities inside the `partial_t hat_Gamma^A` RHS. Keeping this ownership split
explicit should prevent double-counting when the two blocks are implemented or
validated against reference code.

## Validation Matrix

The validation matrix makes clear which checks are exact anchors and which
blocks will require reference or convergence evidence.

| RHS block | Exact anchor available? | Diagonal/off-diagonal limit | Flat or sheared-flat check | Uniform string stationarity | Constraint relevance | Pau/reference comparison | Risk |
| --- | --- | --- | --- | --- | --- | --- | --- |
| `partial_t h_ij` | Partial: determinant and trace-free algebra only | Yes: Stage 3F diagonal and Stage 3G off-diagonal | Flat/pure-gauge only | Gauge dependent | Indirect through determinant/trace health | Required for enforcement timing | Medium |
| `partial_t chi` | Limited | Diagonal/off-diagonal trace consistency | Flat/pure-gauge | Yes, gauge dependent | Hamiltonian-sensitive | Required for gauge/sign conventions | Medium |
| `partial_t K` | Partial: Ricci scalar in special geometries | Diagonal and sheared-flat geometry gates | Strong geometry-source guard | Yes, if gauge fixed | Direct Hamiltonian coupling | Required for nonlinear CCZ4 terms | High |
| `partial_t A_ij` | Partial: trace-free projection identities | Yes, including `A_xz` and `Aww` | Flat/sheared-flat trace-free zero checks | Yes, but nonlinear | Momentum and Hamiltonian sensitive | Required term-by-term | High |
| `partial_t Theta` | Limited exact flat checks | Diagonal/off-diagonal constraints | Flat data is partly vacuous for damping | Yes, outside cutoff | Direct constraint-propagation block | Required for damping/signs | High |
| `partial_t hat_Gamma^A` | Limited exact flat checks plus symbolic dimensional extraction | Yes | Sheared-flat connection relevance | Momentum-constraint dependent | Encodes `Z^i` through `hat_Gamma^i` | Required | High |
| Gauge RHS | Mostly convention checks | Diagonal/off-diagonal advection consistency | Pure-gauge sanity only | Depends on chosen startup gauge | Indirect | Required | Medium |
| Algebraic enforcement | Yes for determinant/tracelessness formulas | Yes: Stage 3F/3G scripts | Not sufficient alone | Not a stationarity proof | Helps control constraints | Required for enforcement policy | Medium |
| Regularity/small-`x` terms | Local regular-limit checks only | Both diagonal and off-diagonal required | Flat limit near cutoff useful | Interior handling dependent | Can dominate constraint errors | Required after Stage 3I design | High |

### Anchors To Use

- Flat/Minkowski: RHS blocks should vanish or reduce to known pure-gauge
  behavior.
- Sheared-flat off-diagonal metric: Ricci geometry should remain zero. This is
  a geometry-source guard, not a full CCZ4 validation.
- Diagonal limit: setting `h_xz = A_xz = gamma_xz = K_xz = 0` must recover the
  diagonal cartoon formulas.
- Uniform black string: exact unperturbed initial data should produce a
  vanishing or pure-gauge RHS in the chosen gauge, outside any turduckening
  region.
- Gauge-fixed uniform GP string: stationarity and constraint preservation
  outside the turduckening region are supporting anchors once GP branch, gauge,
  and sign conventions are fixed.
- Constraint preservation: Hamiltonian, momentum, `Theta`, and the `Z^i`
  constraint vector encoded in `hat_Gamma^i` should remain controlled for
  exact data.
- Symbolic dimensional extraction: the `Theta`, `Z^i`, and `hat_Gamma^i` terms
  should be isolated symbolically enough to guard hidden multiplicity and `/4`
  trace bookkeeping before C++ translation.
- Linear GL spectrum: the eventual linearized RHS should reproduce the
  Gregory-Laflamme threshold/growth spectrum for the SO(3)-symmetric sector
  before nonlinear conclusions are trusted.
- Reference implementation: nonlinear source blocks should be compared
  term-by-term or at trajectory level against Pau's implementation.
- Convergence: full nonlinear behavior requires convergence testing; a
  symbolic identity on a sub-block is not enough.

## Required Future Validation Milestones

The following milestones are required before trusting the future RHS
implementation. They are not completed by Stage 3H.

| Milestone | What it checks | Nuance |
| --- | --- | --- |
| Linear GL spectrum | The linearized RHS around a uniform 5D black string should reproduce the Gregory-Laflamme threshold/growth spectrum for the SO(3)-symmetric sector after matching radius convention, `z`-periodic boundary condition, perturbation sector, gauge choice, and extraction variable. | This is a semi-analytic or literature/numerical spectral anchor from the linear GL eigenvalue problem, not a closed-form elementary identity. It is necessary for the physical linearized RHS but not sufficient for constraint damping. |
| Constraint-violation damping injection | Inject linearized constraint violations around flat or uniform-string data and track `Theta`, `Z^i`, Hamiltonian constraint, and momentum constraint response. Verify damping signs, `kappa_1`, `kappa_2`, hidden multiplicity, and `/4` dimensional bookkeeping against the chosen CCZ4 formulation and Pau/reference implementation. | Flat exact data is vacuous for damping signs; deliberate constraint-violation data is required before trusting the `Theta` and encoded-`Z^i` damping sector. |

The physical GL mode is expected to be constraint-satisfying in the continuum.
It is therefore a strong check of the physical linearized RHS, `K`, `A_ij`,
metric, gauge, and source-sector couplings, but it may not fully exercise the
`Theta` / `Z^i` damping signs. The GL spectrum and constraint-injection tests
are complementary, not interchangeable.

## Exact-Checkable Versus Reference-Only Pieces

Exact-checkable or partly exact-checkable:

- determinant and inverse-metric algebra;
- conformal tracelessness and `/4` denominator guards;
- diagonal/off-diagonal limits;
- flat and sheared-flat Ricci geometry gates;
- selected uniform-string stationary checks after gauge conventions are fixed;
- linear GL spectral checks after convention matching;
- constraint-violation damping-injection checks for `Theta` and encoded `Z^i`;
- local regular limits, after Stage 3I.

Reference-code or convergence-dependent:

- full nonlinear CCZ4 damping and constraint-propagation signs;
- complete hidden-sector Ricci/source terms in production form;
- gauge-driver conventions;
- enforcement timing for determinant and trace-free projections;
- nonlinear GL dynamics and late-time behavior.

## Non-Goals

Stage 3H does not:

- implement C++ source terms;
- derive every CCZ4 RHS term explicitly;
- prove physical correctness of the current scaffold;
- solve small-`x` regularity or turduckening behavior;
- decide final gauge choices;
- replace comparison with Pau's implementation;
- define final `UserVariables.hpp` layout or enum order.

## Acceptance Criteria

Stage 3H is complete when:

- this RHS block decomposition exists;
- each future RHS block has a proposed validation route;
- exact-checkable blocks are separated from reference-code or convergence-only
  blocks;
- diagonal and off-diagonal dependencies are explicitly marked;
- small-`x` regularity is deferred to Stage 3I;
- unit-test fixture design is deferred to Stage 3J;
- no C++ implementation has started.
