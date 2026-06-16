# Stage 3J Unit-Test Fixture Design

Status: design only. No C++ tests, source terms, finite-difference
implementation, or production tolerances are introduced here.

Stage 3J defines the fixture layer that should exist before project-specific
modified-cartoon CCZ4 source terms are implemented. The purpose is to keep the
future implementation from becoming one large RHS block with no local
diagnostics. The tests below separate exact symbolic gates, C++ algebra unit
tests, RHS-block tests, reference-code comparisons, and convergence or
evolution-level checks.

## Conventions

- Physical spatial dimension: `GR_SPACEDIM = 4`.
- Chombo/grid dimension: `CH_SPACEDIM = 2`.
- Gridded directions: `x,z`.
- Hidden directions: two equivalent `w` directions.
- Hidden multiplicity: `N_hidden = GR_SPACEDIM - CH_SPACEDIM = 2`.
- `hww` is a Cartesian-like hidden conformal metric component, not
  `g_theta theta`.
- The spherical/cartoon reconstruction supplies the external `x^2` factor.
- Full traces and trace-free projections are 4D, not 2D.
- The conformal trace denominator is `/4`.
- The evolved connection-sector blueprint is `hat_Gamma^A`, with `Z^A`
  encoded through `hat_Gamma^A = tilde_Gamma^A + 2 Z^A`.

The implementation convention target is GRChombo compatibility plus internal
consistency. External implementations may be used later for validation, but
they are not the source of truth for variable naming, evolved-variable basis,
trace convention, or cartoon-extension bookkeeping. Cartoon-specific
extensions must be documented explicitly when public GRChombo does not already
define them.

## Test-Type Legend

- Symbolic: a Python/SymPy derivation or regression gate.
- C++ unit: a small future C++ test for an isolated algebra/source block.
- RHS-block: a future test of a decomposed CCZ4 RHS term family.
- Evolution: a controlled short evolution or stationary-data run.
- Reference-comparison: later validation against an external implementation,
  either term-by-term or trajectory-level, after the GRChombo-facing
  conventions are fixed.
- Convergence: resolution or self-convergence evidence.

## Tolerance Policy

Stage 3J classifies tests as exact, roundoff-level, tolerance-based,
reference-based, or convergence-based. Numeric tolerances are intentionally not
chosen here. When C++ tests are written, each tolerance must be justified from
the truncation order, grid resolution, floating-point roundoff,
interpolation/ghost-zone treatment, and reference-data uncertainty relevant to
that fixture. Tolerances must not be arbitrary "whatever passes" thresholds.
Exact symbolic identities should remain exact in the symbolic scripts.

## Existing Symbolic Gates

These gates already exist in `docs/derivations/` and should remain part of the
pre-C++ checklist. They are not C++ tests yet, but they define exact expected
values and fixture data that later tests should reuse.

| Test name | Stage source | Type | Input data | Expected output | Exactness | Catches | Does not catch | Required before Stage 3K/C++? | Convention / validation note |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Flat cartoon geometry and GP trace | 3C | Symbolic | Flat GP spatial slice in `(x,theta,phi,z)` with natural GP shift | Spherical Christoffels, `K_ww = beta^x/x`, and `K = K^x_x + K^z_z + 2 K^w_w` | Exact identities | Wrong angular contraction, missing hidden multiplicity, GP trace mistakes | Full CCZ4 RHS, nontrivial `hww`, small-axis finite differencing | Yes | Overall `K_ij` sign convention still requires implementation confirmation |
| Diagonal nontrivial `gamma_ww=q` Ricci structure | 3D | Symbolic | Diagonal metric with `a(x,z)`, `c(x,z)`, `q(x,z)` | Hidden Christoffels, angular Ricci equality, flat Ricci limit, constant-`q0` cone regression | Exact identities | Wrong angular `sin^2(theta)` handling, hidden Ricci multiplicity, constant hidden-sector curvature mistakes | Nonconstant derivative terms in the abstract printed expression are only partly guarded | Yes | No for geometry engine; yes before mapping signs into GRChombo code |
| Nonconstant `q(x,z)` derivative-sector Ricci | 3E | Symbolic | `f=x(1+lambda x+mu z)`, `q=(1+lambda x+mu z)^2` | Direct Ricci equals warped-product reference formulas; active `q_x`, `q_z`, `q_xz` | Exact symbolic and substitution identities | Missing hidden derivative terms, wrong warped-product angular Ricci, stale Ricci engine behavior | `R_zz` with nonzero `f_zz`, off-diagonal base metric | Yes | No for symbolic geometry; yes before C++ layout and regularization choices |
| Diagonal conformal-cartoon algebra | 3F | Symbolic | Diagonal `gamma_xx`, `gamma_zz`, `gamma_ww`, `K_xx`, `K_zz`, `K_ww` | Determinant normalization, `q=hww/chi` identities, radius derivatives, `K_ij` round trip, 4D tracelessness, `/4` guard | Exact identities | Treating `hww` as `g_theta theta`, omitting hidden `hww` from determinant/trace, using `/2` or `/3` | Off-diagonal inverse/tracelessness; Stage 3F is diagonal-only | Yes | Document the GRChombo-facing determinant and trace-free enforcement policy |
| Off-diagonal conformal-cartoon algebra | 3G | Symbolic | Nonzero `h_xz`, `gamma_xz`, `A_xz`, `K_xz` | Block determinant/inverse, normalized `det h_4D=1`, `K_xz` reconstruction, full 4D tracelessness, diagonal limit to 3F | Exact identities | Reciprocal inverse mistakes, missing `2 h^xz A_xz`, wrong off-diagonal determinant, wrong `/4` denominator | Full Ricci/source terms and finite-difference behavior | Yes | Confirm implementation storage/order and enforcement policy |
| Sheared-flat off-diagonal Ricci gate | 3G | Symbolic | Flat pullback metrics with constant `g_xz=lambda` and `g_xz=x` | Ricci tensor and scalar vanish exactly | Exact zero | Ricci engine using diagonal-only inverse assumptions, off-diagonal Christoffel wiring mistakes | Non-flat off-diagonal Ricci source formulas, CCZ4 RHS couplings | Yes | No for the local geometry gate; yes for C++ source-term placement |
| Small-`x` regularity and `hat_Gamma^x` guard | 3I | Symbolic | Taylor data with even diagonal fields, odd off-diagonal fields, matching `h_xx-hww=O(x^2)`, and regular `Z^A` parity | Removable limits finite; violated matching exposes `1/x`; `hat_Gamma^x=tilde_Gamma^x+2Z^x` finite under documented convention | Exact limits and divergence checks | Missing axis matching, hidden multiplicity mistakes in the assembled connection, generic parity tests that miss residual `1/x` terms | Ghost-cell filling, finite-difference stencils, final sign convention | Yes | Derive and document the sign in the GRChombo-facing cartoon convention |

## Future C++ Unit-Test Candidates

These are not implemented in Stage 3J. They are the first fixtures that should
be created when the project-specific C++ layer begins. Each test should use a
small analytic fixture and compare only one algebra/source block at a time.

Initial Stage 4A coverage now lives in
`code/BlackStringToy/tests/Stage4AConformalCartoonAlgebraTest.cpp`, using
`code/BlackStringToy/ConformalCartoonAlgebra.hpp`. This fixture is local-value
only: it does not read grid variables, enum slots, `Cell`, `Vars`,
`FArrayBox`, or evolution data. The Stage 4A review follow-up adds
dimension-aware `chi` power bookkeeping, tolerance-based floating-point guards,
and independent hard-coded `K_xz`/`K_ww` reconstruction oracle values.

Initial Stage 4B coverage now lives in
`code/BlackStringToy/tests/Stage4BVariableLayoutTest.cpp`. This fixture checks
the public CCZ4 component order retained as the baseline for comparison and
records the visible helper-input map without editing the enum or reading grid
data. It is a public-layout drift guard only; the real repo-owned hidden
placement check belongs to Stage 4C.

Stage 4C coverage now lives in
`code/BlackStringToy/tests/Stage4CVariablePlacementTest.cpp`, with the real
placement assertions also compiled from `code/BlackStringToy/UserVariables.hpp`.
This confirms that `c_hww` and `c_Aww` exist, that `c_hww == c_K - 1` and
`c_Aww == c_Theta - 1`, and that the Stage 4A helper input map now has real
component slots for `hww` and `Aww`. It still does not read grid data or prove
that a future handoff passes those components correctly.

The Stage 4C scratch Docker/GRChombo build also compiles the full
`BlackStringToy` scaffold with the 27-variable layout. The inherited cheap
smoke run reaches runtime but fails because `hww` and `Aww` are not initialized
or handed off yet. Stage 4D should address only that finite scaffold-value
failure; a Stage 4D smoke pass would still not validate cartoon Ricci terms,
full CCZ4 RHS terms, or physical black-string evolution.

Stage 4D should keep that finite scaffold support explicitly smoke-only. The
temporary freeze should be controlled by a default-off parameter such as
`scaffold_freeze_hidden`, enabled only by the cheap smoke parameter file. A
passing Stage 4D smoke run then shows only that the inherited scaffold no
longer dies immediately from hidden-variable NaNs; it must not be interpreted
as a hidden-sector RHS, cartoon Ricci, or physical-evolution test.

Stage 4E adds the first grid-to-helper handoff diagnostic. The default-off
`scaffold_check_hidden_handoff` parameter enables a read-only path that loads
the actual scaffold grid slots for `chi`, `h11`, `h12`, `h22`, `hww`, `A11`,
`A12`, `A22`, `Aww`, and `K`, packages them into the Stage 4A local algebra
helper input structs, and checks finite determinant, inverse, full-4D trace,
and `K_ij` reconstruction outputs. This is a fixture-style handoff check only:
it does not write helper results back to the grid, implement RHS terms, or show
that cartoon evolution is physically meaningful.

The smoke data is intentionally cheap and can be symmetric enough that swapped
slots remain finite. The companion
`code/BlackStringToy/tests/Stage4EGridToHelperMappingTest.cpp` fixture uses
distinct local values for each relevant component and checks the exact helper
input map plus an independent `K_ww` oracle, so it can catch swaps that the
runtime finiteness check would miss.

Stage 4F adds only the future cartoon Ricci helper interface. The header
`code/BlackStringToy/CartoonRicciInterface.hpp` defines the
metric-derivative Ricci contract used by the Stage 3C-3E symbolic checks:
local-value input structures for `x`, `chi`, `h_xx`, `h_xz`, `h_zz`, `hww`,
first derivatives, and second derivatives, plus an output structure for
`R_xx`, `R_xz`, `R_zz`, and `R_ww`. This is not the Gamma-based GRChombo
`CCZ4Geometry` Ricci form. Before RHS wiring, a later stage must confirm how
this metric-derivative output is consumed by the GRChombo-facing RHS path and
resolve any mismatch. The companion interface fixture checks that these types
compile and that hidden multiplicity is two. It does not compute Ricci values,
and the future implementation must still apply the Stage 3I small-`x`
regularity rules.

Stage 4G implements the first local metric-derivative cartoon Ricci helper
against that interface. The fixture
`code/BlackStringToy/tests/Stage4GCartoonRicciMetricDerivativeTest.cpp` checks
flat zero Ricci data, the Stage 3D constant-`q` cone oracle, a Stage 3E
nonconstant-`q` warped-product oracle, the Stage 3G `x`-shear flat gate, and an
away-axis guard that rejects `x = 0`. It remains local-value only and does not
read grid data or wire outputs into the RHS.

Stage 4H is a compatibility-planning gate, not a unit test. It records that the
current inherited GRChombo CCZ4 RHS path computes Ricci through
`CCZ4Geometry::compute_ricci_Z`, which uses the evolved hatted `Gamma` variable
and `Z_over_chi`. The Stage 4G helper is metric-derivative based, so future RHS
tests must not silently mix those two Ricci conventions.

Stage 4I adds a typed Ricci bridge contract. `CartoonRicci::RicciComponents`
remains a distinct input type, and future RHS-facing code must go through
`CartoonRicciBridge.hpp` or a reviewed replacement rather than consuming
Stage 4G output directly. The bridge pins physical lower/lower Ricci
components, full 4D trace factors, hidden multiplicity, and `R_ww` as the
Cartesian-like hidden component. After the review follow-up, the raw cartoon
Ricci component storage is private, and the bridge exposes a distinct
`RhsRicciComponents` view. Ordinary code can no longer use `ricci.xx`-style
field access; that access path is intentionally restricted so the hidden
`R_ww` and off-diagonal multiplicities cannot be bypassed accidentally.

Stage 4J adds the local Ricci-to-RHS contract. It accepts the Stage 4I
bridge-approved `RhsRicciComponents` view and local algebra inputs, then
computes only named Ricci contractions for future RHS blocks. It intentionally
does not accept raw cartoon Ricci, implement RHS formulas, read grid data, or
wire anything into evolution.

The Stage 4J review follow-up keeps the sanctioned bridge path easy to use but
makes the internal `RicciAccess` doorway private. The bridge output remains a
typed by-value view, not a raw pointer, reference, mutable alias, or loose list
of doubles.

Stage 4K adds a local RHS source-block skeleton. It consumes the Stage 4J
contract type and exposes named future source output fields, but only through
an explicit inert contract-test path. It does not implement physical RHS
formulas or evolution wiring.

Stage 4L implements the first local formula block: the full-4D trace-free
projection of bridge-approved Ricci. The source block delegates the projection
arithmetic to the shared conformal-cartoon algebra helper and rejects
nonpositive `x` through the existing away-axis contract validation. It is still
local only and does not wire the projected pieces into evolution or claim a
full CCZ4 RHS.

| Test name | Stage source | Type | Input data | Expected output | Exactness | Catches | Does not catch | Required before Stage 3K/C++? | Convention / validation note |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Conformal determinant enforcement | 3F/3G | C++ unit | Positive diagonal and off-diagonal metric samples, including nonzero `h_xz` | `(h_xx h_zz - h_xz^2) hww^2 = 1` after the chosen enforcement path | Exact identity to roundoff | Missing hidden `hww`, wrong block determinant, accidental spherical Jacobian factors | Ricci/source-term correctness | Yes | Define project enforcement in the GRChombo-facing variable set |
| Full 4D trace-free projection | 3F/3G | C++ unit | Random symmetric reduced `A_AB`, `Aww`, positive `h_AB`, `hww` | `h^{xx}A_xx + 2h^{xz}A_xz + h^{zz}A_zz + 2Aww/hww = 0` after projection | Exact identity to roundoff | Using 2D trace, missing hidden factor, omitting off-diagonal contraction | Evolution stability or damping signs | Yes | Yes |
| `/4` trace denominator guard | 3F/3G/3H | C++ unit | Known `K_ij`, `K`, `h_ij` fixtures | `/4` reconstructs `K_ij`; `/2` and `/3` fail in a deliberate negative test | Exact identity and expected nonzero failures | Accidentally using `CH_SPACEDIM` or 3+1 intuition | Sign convention of `K_ij` | Yes | Yes for final convention naming |
| Hidden `hww` determinant/trace participation | 3F/3G | C++ unit | Same visible metric with varied `hww` | Determinant and trace tests change with `hww` as expected | Exact identity to roundoff | Treating hidden sector as passive or diagnostic-only | Ricci hidden derivatives | Yes | Yes |
| Off-diagonal inverse and determinant handling | 3G | C++ unit | Nonzero `h_xz` values near but safely below positive-definite limit | Matrix inverse satisfies `h h^{-1}=I`; determinant uses `h_xx h_zz - h_xz^2` | Exact identity to roundoff | Reciprocal inverse, wrong sign on `h^{xz}`, missing positive-definiteness checks | Nonlinear RHS correctness | Yes | No beyond storage convention |
| `K_ij` reconstruction including `K_xz` | 3G | C++ unit | Off-diagonal physical `K_xz` and conformal `A_xz` fixtures | `K_xz = chi^{-1}(A_xz + h_xz K/4)` | Exact identity to roundoff | Dropped off-diagonal extrinsic-curvature component, wrong denominator | RHS signs and gauge coupling | Yes | Yes for `K_ij` sign convention |
| Round two-sphere positive-curvature anchor | 3C/3D | Symbolic geometry / C++ geometry unit | Round `S^2` angular sector with radius `a`, or the equivalent extracted hidden angular-sector fixture | `R_{S^2} = 2/a^2` for the two-sphere scalar curvature, with the fixture convention stated explicitly | Exact symbolic identity or roundoff-level C++ check | Wrong Ricci sign, angular-index contraction error, missing angular multiplicity | Black-string source terms, full CCZ4 RHS, gauge or damping behavior | Yes before trusting cartoon Ricci signs | No for the local geometry anchor; yes before source-term placement |
| Cartoon Ricci source block fixtures | 3D/3E/3G | RHS-block | Analytic diagonal and sheared-flat metrics from symbolic scripts | C++ Ricci/source block matches symbolic expected values | Tolerance-based against symbolic exact values | Missing `1/x`, `1/x^2`, hidden derivative, or off-diagonal geometry terms | Full CCZ4 evolution and damping behavior | Yes before trusting source terms | Use GRChombo-facing term placement; compare externally later as validation |
| Small-`x` regularized combinations | 3I | C++ unit | Regular and deliberately irregular Taylor data near the axis | Regular data gives finite limits; irregular data triggers expected diagnostic/failure | Exact limit fixtures plus tolerance near finite `x` | Unsafe raw `1/x` forms, missing matching condition, over-aggressive regularization | Correct ghost-zone strategy | Yes | Yes for axis-fill and stencil conventions |
| Assembled `hat_Gamma^x` axis cancellation | 3I | C++ unit | Taylor data with `h_xx-hww=O(x^2)`, odd `h_xz`, regular `Z^x` | Assembled `tilde_Gamma^x` singular part is finite and `hat_Gamma^x` remains axis-regular | Exact symbolic target or tight tolerance | Wrong sign/multiplicity in highest-risk connection expression | Full Gamma-driver evolution | Yes | Yes, required |
| `hat_Gamma^A = tilde_Gamma^A + 2Z^A` convention | 3H/3I | C++ unit | Controlled `tilde_Gamma^A` and `Z^A` samples | Encoded `hat_Gamma^A` and recovered constraint quantity match the chosen convention | Exact identity to roundoff | Wrong factor of two, confusing lowered and raised `Z` components | Damping signs in evolution | Yes | Yes, required |
| Stage 4B public CCZ4 baseline-layout check | 3J/4B | C++ layout fixture | Public CCZ4 enum retained as a comparison baseline; visible helper input map | Public symbols bind to the expected names/indices; current slots before `K` and `Theta` are identified as public `h33` and `A33` | Compile-time assertions plus runtime string/name checks | Public CCZ4 name/order drift before adding repo-owned variables; stale visible helper input map | Real grid reads, hidden enum implementation, real `hww/Aww` placement, helper integration, source terms | Yes before Stage 4C/4D | GRChombo-facing public layout is the baseline; real hidden placement guard is Stage 4C |
| Stage 4C hidden enum placement guard | 3J/4C | Header-level compile-time check plus updated layout fixture | Repo-owned `hww/Aww` enum entries in `BlackStringToy/UserVariables.hpp` | `UserVariables.hpp` static assertions prove `c_hww == c_K - 1` and `c_Aww == c_Theta - 1`; the placement fixture verifies names and helper-map slots | Compile-time assertions plus runtime name checks | Wrong `hww/Aww` placement, missed AH positional hazard, hidden helper input mismatch | Evolution, source terms, finite-difference correctness, real grid handoff correctness | Yes before Stage 4D grid handoff | Uses actual Stage 4C enum names |
| Stage 4E grid-to-helper handoff diagnostic | 3J/4E | Scaffold diagnostic / grid-read fixture plus standalone mapping fixture | Cheap smoke scaffold grid state with Stage 4D finite hidden-variable support enabled; distinct local values `chi=2`, `h11=3`, `h12=5`, `h22=37`, `hww=11`, `A11=13`, `A12=17`, `A22=19`, `Aww=23`, `K=29` | Stage 4A helper receives finite values from the intended slots; the standalone fixture checks exact component values and `K_ww = 51.375` | Runtime finite-value checks plus exact local value mapping to roundoff | Wrong live component handoff, swapped helper-map slots, non-finite hidden slots, invalid local conformal determinant before helper use | Cartoon Ricci, RHS correctness, physical hidden-sector evolution, finite-difference correctness | Yes before any helper output is used by evolution | Check-only; helper results are not written back |
| Stage 4F cartoon Ricci interface fixture | 3J/4F | C++ interface/type fixture | Local-value metric-derivative input and output structures for future cartoon Ricci helper | Interface compiles; hidden multiplicity is two; after Stage 4G, `ricci_formulas_implemented` is true | Compile/type check only | Interface drift, accidental hidden multiplicity change, ambiguous metric-derivative versus Gamma-based Ricci contract | Source term, RHS, evolution, small-`x` safety, or physical correctness | Yes before Ricci implementation | Later RHS wiring must resolve compatibility with any Gamma-based GRChombo Ricci expectation |
| Stage 4G metric-derivative cartoon Ricci fixture | 3J/4G | C++ local geometry unit | Local Stage 4F inputs for flat data, constant `q0=4` cone at `x=2`, nonconstant `q=(1+x)^2` at `x=1`, and Stage 3G `x`-shear flat metric | Flat and shear cases give zero Ricci; constant cone gives `R_ww=-3/4`; nonconstant `q` gives `R_xx=-2`, `R_ww=-12`; `x=0` is rejected | Roundoff-level local exact oracles | Wrong angular sign, confusing `hww` with `x^2 hww`, missing metric derivatives, broken off-diagonal inverse path, unsafe axis acceptance | RHS wiring, Gamma-based Ricci compatibility, full small-`x` regularization, physical evolution | Yes before any Ricci output is used outside local tests | Metric-derivative form only; not Gamma-based `CCZ4Geometry` Ricci |
| Stage 4I typed Ricci bridge contract fixture | 3H/3J/4H/4I | C++ local contract fixture | `R_xx=2`, `R_xz=3`, `R_zz=5`, `R_ww=7`; `h^{xx}=11`, `h^{xz}=13`, `h^{zz}=17`, `h^{ww}=19`; `chi=0.5` | Raw cartoon Ricci fields are not directly accessible; bridge exposes the intended RHS-facing values; full 4D conformal-inverse contraction is `451`; physical scalar is `225.5`; missing off-diagonal or hidden factors fail; nonpositive `chi` rejected | Exact local identities to roundoff plus compile-time access restriction | Wrong Ricci component placement, missing hidden `R_ww`, missing off-diagonal factor, missing hidden multiplicity, accidental direct-use convention drift | Live RHS correctness, Gamma-driver/damping signs, physical evolution | Required before any Ricci output is wired into RHS | No evolution wiring; future RHS-facing code must cross the bridge |
| Stage 4J local Ricci-to-RHS contract fixture | 3H/3J/4H/4I/4J | C++ local contract fixture | Bridge-approved `RhsRicciComponents` with `R_xx=2`, `R_xz=3`, `R_zz=5`, `R_ww=7`; `h^{xx}=11`, `h^{xz}=13`, `h^{zz}=17`, `h^{ww}=19`; `chi=0.5`; local `x=2` | Contract accepts bridge Ricci, not raw cartoon Ricci; full 4D Ricci trace is `451`; physical scalar is `225.5`; dropping `Rww` or the off-diagonal factor fails; `x=0` rejected | Exact local identities to roundoff plus type-boundary check | Future RHS grabbing raw Ricci, omitting `Rww`, missing hidden multiplicity, missing off-diagonal factor, accepting unsafe axis input | Real RHS formula correctness, evolution correctness, small-axis regularization, Gamma-based compatibility | Required before any repo-owned RHS source block consumes Ricci | No evolution wiring; local contract only |
| Stage 4K local RHS source-block skeleton fixture | 3H/3J/4J/4K | C++ local skeleton fixture | Stage 4J `RhsLocalInputs` contract with bridge-approved Ricci; hard-coded `451` trace data | Source-block skeleton accepts the contract type, not raw cartoon Ricci; named inert fields `rhs_chi`, `rhs_hww`, `rhs_Aww`, `rhs_K` are finite only in the explicit test path; carried Ricci trace remains `451` | Compile/type check plus roundoff-level local identities | Future source block bypassing the contract, unnamed source outputs, accidental non-finite placeholders, mistaking inert zeros for implemented formulas | Real source-term formulas, evolution wiring, grid reads, small-axis regularization | Required before implementing RHS source formulas | Skeleton/container only; no physical RHS terms |
| Stage 4L trace-free Ricci projection fixture | 3F/3G/3J/4L | C++ local formula fixture | Bridge-approved Ricci values with lower metric and conformal inverse metric; hard-coded `451` trace case plus a consistent diagonal metric case | `R_xx^TF=-110.75`, `R_xz^TF=3`, `R_zz^TF=-107.75`, `R_ww^TF=-105.75` for the hard-coded case; consistent case has zero full 4D trace after projection; `x=0` is rejected | Exact local identities to roundoff | Missing hidden `ww` factor, dropping `R_ww`, using `/3` instead of `/4`, wrong trace-free contraction, unsafe away-axis acceptance | Full RHS correctness, live evolution, grid reads, small-axis regularization | Required before Ricci projection is used in RHS source formulas | Local formula block only |
| Gamma-driver ownership boundary | 3H | C++ unit/design check | Mock RHS block inputs for gauge and `hat_Gamma^A` terms | Gauge block owns lapse/shift/auxiliary evolution; `hat_Gamma^A` block owns their appearances in `partial_t hat_Gamma^A` | Structural review plus targeted unit checks | Double-counting gauge terms, split ownership drift | Physical correctness of chosen gauge | Yes as a design review gate | Yes |

## Integration, Reference, And Convergence Tests

These are not substitutes for unit tests. They catch coupled failures that
small algebra tests cannot catch, and several require the future implementation
or external/reference-code comparison.

| Test name | Stage source | Type | Input data | Expected output | Exactness | Catches | Does not catch | Required before Stage 3K/C++? | Convention / validation note |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Flat/Minkowski zero RHS | 3H | RHS-block/evolution | `chi=1`, flat `h`, zero `A_ij`, `K=0`, zero shift or documented gauge | RHS vanishes or reduces to known pure-gauge terms | Exact zero or gauge-specified tolerance | Gross source-term errors, nonzero hidden flat residues | Constraint-damping signs; many nonlinear terms are inactive | No before planning; required before long runs | Gauge convention yes |
| Sheared-flat zero Ricci/source check | 3G/3H | RHS-block | Sheared-flat metrics from Stage 3G Ricci gate | Geometry source terms vanish in flat pullback cases | Exact zero or tight tolerance | Off-diagonal geometry mistakes in source assembly | Non-flat Ricci and physical black-string behavior | No before planning; required before implementation trust | No for geometry, yes for source block placement |
| Gauge-fixed uniform GP string stationarity | 3A/3H | Evolution/RHS-block | Unperturbed uniform black string outside turduckening region with chosen gauge | RHS vanishes or is pure gauge, and constraints remain controlled outside cutoff | Tolerance-based | Coupled metric, `K`, `A_ij`, gauge, and hidden-source errors | Interior turduckening correctness, GL growth spectrum | No before planning; required before production | Yes: gauge, branch, sign, and reference comparison |
| Uniform Schwarzschild/black-string anchor - slicing-dependent | 3A/3H | Symbolic geometry / evolution / future spacetime check | For a GP-type slice, the spatial metric is the flat GP spatial metric times `S^1` and the nontrivial black-hole geometry is carried by `K_ij`; alternatively, a future spacetime-Ricci engine may check Schwarzschild_4 x `S^1` | GP spatial Ricci vanishes for the GP spatial metric times `S^1`; full spacetime Ricci flatness is out of scope for the current spatial-cartoon fixture layer until a spacetime engine exists | Exact zero for the GP spatial geometry; future reference/spacetime check for full Ricci flatness | Confusing spatial-slice Ricci with spacetime Ricci, careless Schwarzschild x `S^1` assumptions, GP geometry regressions | Does not validate arbitrary Schwarzschild slicings, full CCZ4 RHS, or turduckening | No before planning; required before trusting uniform-string anchors | Yes: slicing, sign, gauge, and variable conventions |
| Physical-sector linear GL dispersion/growth milestone | 3A/3H | Evolution/reference-comparison | Linear perturbations around uniform string after matching radius convention, `z` periodicity, perturbation sector, gauge choice, extraction variable, and slicing/conformal-variable convention | Threshold/growth spectrum matches the literature or a reference spectral calculation for the SO(3)-symmetric sector | Semi-analytic/reference-based | Physical linearized RHS mistakes in metric, `K`, `A_ij`, gauge/source coupling, and cartoon-sector terms | Full nonlinear dynamics; because the GL mode is constraint-satisfying in the continuum, it is necessary but not sufficient for damping signs | No before planning; required before scientific claims | Yes, required |
| Constraint-violation damping injection | 3H | RHS-block/evolution | Linearized flat or uniform-string data with controlled Hamiltonian, momentum, `Theta`, and encoded-`Z^A` violations through `hat_Gamma^A = tilde_Gamma^A + 2Z^A` | Prefer derived/documented linearized CCZ4 constraint-subsystem decay rates and signs for the chosen `kappa_1`, `kappa_2`, gauge, and background; otherwise mark as high-risk reference/formulation-derived | Formulation-derived, reference-based, and tolerance-based | Wrong damping sign, wrong `kappa_1`/`kappa_2`, missing hidden multiplicity, wrong `/4` bookkeeping, wrong `hat_Gamma^A` coupling to encoded `Z^A` | Physical GL growth correctness and nonlinear source-term completeness | No before planning; required before trusting the constraint-damping block | Yes, required |
| External term-by-term comparison | 3H/Stage 4 | Reference-comparison | Shared local fields and gauge data for each decomposed RHS block | Project RHS terms match an external implementation after mapping from the GRChombo-facing project conventions | Reference-based | Term omissions, sign mistakes, hidden-sector source drift | Bugs shared with reference or mismatched setup | No before planning; required for high-risk blocks | Validation only, not convention authority |
| External trajectory-level comparison | 3H/Stage 4+ | Reference-comparison/evolution | Short controlled runs with shared parameters | Diagnostics follow an external/reference trajectory within documented tolerance | Reference/tolerance-based | Integration-level coupling errors | Isolated source-term attribution | No before planning; required before physics interpretation | Validation only, not convention authority |
| Resolution self-convergence | 3H/Stage 6 | Convergence | At least three resolutions for controlled fixtures and later black-string runs | Expected convergence rate for constraints, RHS residuals, horizons, and modes | Convergence-based | Discretization and implementation coupling errors | Convergence to wrong continuum equations if source terms are wrong | No before planning; required before claims | No for method, yes for acceptance thresholds |

## Pre-C++ Gate List

The following gates should be true before Stage 3K turns this design into an
implementation plan or before C++ source-term work begins:

- All existing symbolic scripts from Stages 3C-3I pass.
- No stale stage-label or old conformal-cartoon filename references remain.
- The determinant condition, hidden `hww` participation, full 4D trace, and
  `/4` denominator conventions are documented.
- The `hat_Gamma^A = tilde_Gamma^A + 2Z^A` convention is documented, including
  the fact that `Z^A` is encoded rather than the primary evolved connection
  variable.
- Small-`x` risks, cartoon-axis parity, the co-located physical singularity,
  and turduckening/interior regularization are documented as distinct issues.
- The unresolved GRChombo-facing convention choices below are listed
  explicitly and carried into Stage 3K.
- Each future C++ source block has a proposed exact, reference, convergence, or
  evolution-level validation route.

## C++ Unit-Test Design Principles

- Test small algebra/source blocks, not the entire CCZ4 RHS at once.
- Isolate hidden-multiplicity factors in tests where possible.
- Pair every off-diagonal implementation with a diagonal-limit regression.
- Use both regular and deliberately irregular Taylor data for axis-limit tests.
- Treat flat data as necessary but insufficient, especially for
  constraint-damping signs.
- Compare source blocks against symbolic fixtures before comparing full
  trajectories.
- Do not claim agreement from compilation, executable creation, or HDF5 output
  alone.
- Keep reference-code comparisons explicit about convention maps and expected
  tolerances.

## Required Linearized Milestones

Two linearized milestones must remain separate:

1. Physical-sector GL dispersion: the linearized RHS around the uniform 5D
   black string should reproduce the Gregory-Laflamme threshold/growth spectrum
   in the SO(3)-symmetric sector after matching radius convention, periodic
   boundary condition in `z`, perturbation sector, gauge choice, extraction
   variable, and slicing/conformal-variable convention. This validates the
   physical linearized RHS coupling of metric, `K`, `A_ij`, gauge, and
   cartoon-source terms.
2. Constraint-violation damping injection: controlled linearized violations
   should be injected around flat or uniform-string data, then `Theta`,
   encoded `Z^A`, the Hamiltonian constraint, and the momentum constraints
   should be tracked. This is the non-vacuous damping-sector test. If feasible,
   derive or document the expected linearized CCZ4 constraint-subsystem decay
   rates/signs for the chosen `kappa_1`, `kappa_2`, gauge, and background;
   otherwise this fixture remains high risk and reference/formulation-derived.

The GL mode is expected to be constraint-satisfying in the continuum. It is
therefore necessary for the physical sector but not sufficient for validating
constraint-damping signs or `kappa_1`, `kappa_2` behavior.

## Known Unresolved Convention And Validation Items

- Document how the project enforces conformal determinant and trace-free
  conditions in the GRChombo-facing variable set when hidden `hww`
  participates.
- Derive and document the exact `tilde_Gamma^x` sign and full
  `hat_Gamma^A` convention in the GRChombo-facing cartoon extension, including
  raised/lowered `Z` handling.
- Document Gamma-driver and `hat_Gamma^A` coupling ownership in the
  GRChombo-facing implementation.
- Gauge choice for uniform-string stationarity checks.
- Radius, periodicity, perturbation-sector, gauge, and extraction-variable
  conventions for the GL dispersion comparison.
- Which external/reference comparisons can be term-by-term and which are only
  reliable at the trajectory or diagnostic level.

## Non-Goals

Stage 3J does not:

- implement C++ tests;
- implement CCZ4 source terms;
- implement finite-difference stencils or axis ghost filling;
- choose final production tolerances;
- settle all GRChombo-facing convention details or external validation
  mappings;
- replace the Stage 3K minimal C++ implementation plan.

## Acceptance Criteria

Stage 3J is complete when:

- this unit-test fixture design document exists;
- every validated symbolic gate from Stages 3C-3I is mapped to future test
  usage;
- required pre-C++ gates are identified;
- symbolic, C++ unit, RHS-block, reference, evolution, and convergence tests
  are clearly separated;
- unresolved convention confirmations are tracked;
- no implementation code has changed.
