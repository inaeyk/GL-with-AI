# Stage 3K Minimal C++ Implementation Plan

Status: planning only. This document does not implement C++, finite
differences, source terms, or production runs.

## Implementation Boundary

- Do not modify `external/GRChombo`.
- Future implementation should live in repo-owned code, likely under
  `code/BlackStringToy/` or a new repo-owned implementation layer that is
  copied into the scratch GRChombo tree by the existing Stage 2 workflow.
- Scratch builds, tests, and raw outputs should stay under ignored `runs/`.
- Compilation, executable creation, or HDF5 output is not evidence of physical
  correctness.
- Algebra-helper fixtures passing is not integration correctness. Green helper
  tests do not prove that variables are read from the correct grid slots.
- No result from Stage 4A should be described as "cartoon evolution works" or
  "CCZ4 works."
- Every implementation stage must have a corresponding validation gate from
  Stage 3J or an explicitly documented new gate.
- The current `BlackStringToy` source is still a non-physical inherited
  scaffold. Stage 3K does not change that.
- Stage 4A C++ work required explicit user approval after Stage 3K review.
  Stage 4B and later stages also require explicit approval before source edits.

## Convention Policy

The implementation convention target is GRChombo compatibility plus internal
consistency. External implementations may be used later for validation, but
they are not the source of truth for variable naming, evolved-variable basis,
trace convention, or cartoon-extension bookkeeping. Where public GRChombo does
not already define the reduced-cartoon extension, the project must document the
cartoon-specific convention explicitly and keep it consistent across helpers,
tests, diagnostics, and notes.

## Stage 4A Local Algebra Implementation

Recommended first implementation stage: **Stage 4A, repo-owned algebraic conformal-cartoon helper layer
plus local/non-grid fixture tests, without grid-variable wiring and without
connecting it to time evolution**.

This is lower risk than adding RHS terms or changing evolution variables first
because it exercises the load-bearing bookkeeping from Stages 3F-3G while
keeping the public GRChombo RHS untouched. It should use small local fixtures
and avoid claiming any evolved black-string dynamics.

Stage 4A should include:

1. A read-only variable-layout audit.
   - Confirm what public `CCZ4UserVariables.hpp` already provides.
   - Decide the exact placement of `hww`, `Aww`, `hat_Gamma^x`, and
     `hat_Gamma^z` before editing `UserVariables.hpp`.
   - Recheck the AH positional hazard around `hww` before adding variables.
   - No enum edit should happen until the audit result is reviewed.
2. A repo-owned local algebra helper module.
   - Determinant block:
     `(h_xx h_zz - h_xz^2) hww^2`.
   - Reduced inverse conformal metric block:
     `h^{xx}`, `h^{xz}`, `h^{zz}`, `h^{ww}`.
   - Full-4D trace and trace-free projection with hidden multiplicity
     `N_hidden = 2`.
   - `/4` trace denominator guard for `GR_SPACEDIM = 4`.
   - `K_ij` reconstruction, including `K_xz` and `K_ww`.
   - Diagonal-limit path back to Stage 3F formulas.
3. A minimal local/non-grid fixture-test target for the algebra helper.
   - Use hard-coded local analytic fixtures first.
   - Do not require AMR, HDF5, AH finding, or long runs.
   - Tests should fail if `/2` or `/3` denominators are substituted for `/4`.
4. Documentation of the next blocked stages.
   - Variable-enum grid wiring, cartoon Ricci helpers, small-axis helpers, and
     full RHS wiring remain separate follow-up stages requiring explicit user
     approval after Stage 4A passes.

Stage 4A should not implement:

- grid-variable wiring;
- cartoon Ricci helper implementation;
- small-axis helper implementation;
- full CCZ4 RHS terms;
- gauge or Gamma-driver equations;
- constraint damping;
- finite-difference stencils;
- black-string initial data;
- turduckening;
- AH finding;
- radiation extraction;
- production evolutions.

Passing Stage 4A algebra-helper fixtures is not evidence that cartoon evolution
works; it only shows that local algebra formulas were implemented correctly on
supplied local values.

## Stage 4A Local Helper Implementation

Stage 4A has started as a local-value-only implementation:

- Helper: `code/BlackStringToy/ConformalCartoonAlgebra.hpp`.
- Fixture: `code/BlackStringToy/tests/Stage4AConformalCartoonAlgebraTest.cpp`.

The helper implements the determinant block, reduced block inverse, full 4D
trace with hidden multiplicity, trace-free projection, `/4` denominator
bookkeeping, `K_ij` reconstruction including `K_xz` and `K_ww`, diagonal-limit
checks, independent hard-coded `K_xz`/`K_ww` oracle values, and a local
normalized-determinant fixture. After Stage 4A review, the conformal-factor
power is expressed using `physical_spatial_dim = 4`, floating-point zero guards
use a local tolerance, and the fixture includes independently derived
`K_xz = 41/8` and `K_ww = 43/4` oracle checks. The fixture is compiled
standalone with hard-coded scalar values:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -I code/BlackStringToy \
  code/BlackStringToy/tests/Stage4AConformalCartoonAlgebraTest.cpp \
  -o /tmp/blackstringtoy_stage4a_algebra_test
/tmp/blackstringtoy_stage4a_algebra_test
```

This helper does not include GRChombo grid headers, does not read enum slots,
does not touch `UserVariables.hpp`, and does not wire anything to evolution.
Passing this fixture does not show that grid variables are wired correctly or
that modified-cartoon CCZ4 evolution works.

## Stage 4B Public CCZ4 Baseline Layout Check

Stage 4A helpers operate on supplied local values only. Stage 4B adds an
automated public-layout baseline check before any helper is wired to grid
variables. Manual review is useful, but it is not sufficient.

Stage 4B proves one limited point:

- the public GRChombo CCZ4 symbols currently used by this project still have
  the expected order;
- public-name drift or public-layout drift is caught when the Stage 4B test is
  built and run;
- the visible part of the future helper input map is documented before adding
  repo-owned hidden variables.

Stage 4B does **not** yet prove that `hww` or `Aww` are correctly placed,
because the repo-owned `c_hww` and `c_Aww` symbols do not exist yet. It also
does not prove that future grid reads pass the right values to Stage 4A
helpers, or that evolution, CCZ4 source terms, or cartoon terms work.

Stage 4B was completed with a standalone non-grid layout fixture:

- Fixture: `code/BlackStringToy/tests/Stage4BVariableLayoutTest.cpp`.
- It checks the underlying public `CCZ4UserVariables.hpp` directly, so it
  remains a baseline for the public layout even after BlackStringToy adds
  repo-owned variables.
- Before Stage 4C, `code/BlackStringToy/UserVariables.hpp` aliased public
  `CCZ4UserVariables.hpp` through `NUM_VARS = NUM_CCZ4_VARS` and
  `variable_names = ccz4_variable_names`.
- Stage 4B cannot enforce hidden placement by itself. It records that the
  public slots before `K` and `Theta` are `h33` and `A33`, respectively.

The current public CCZ4 component layout found before hidden-variable
extension is:

| Component | Index | Notes |
| --- | ---: | --- |
| `c_chi` | 0 | conformal factor |
| `c_h11`, `c_h12`, `c_h13`, `c_h22`, `c_h23`, `c_h33` | 1-6 | visible public CCZ4 metric block |
| `c_K` | 7 | trace; current `c_K - 1` slot is `h33`, not a named `hww` |
| `c_A11`, `c_A12`, `c_A13`, `c_A22`, `c_A23`, `c_A33` | 8-13 | visible public CCZ4 trace-free block |
| `c_Theta` | 14 | Z4 scalar; current `c_Theta - 1` slot is `A33`, not a named `Aww` |
| `c_Gamma1`, `c_Gamma2`, `c_Gamma3` | 15-17 | hatted connection-sector components in the GRChombo-facing basis |
| `c_lapse`, `c_shift1..3`, `c_B1..3` | 18-24 | gauge variables in the public layout |

The Stage 4B helper-input map currently checked for the visible public
components is:

| Future helper input | Current component convention checked |
| --- | --- |
| `helper.chi` | `c_chi` |
| `helper.h_xx` | `c_h11` |
| `helper.h_xz` | `c_h12` |
| `helper.h_zz` | `c_h22` |
| `helper.A_xx` | `c_A11` |
| `helper.A_xz` | `c_A12` |
| `helper.A_zz` | `c_A22` |
| `helper.K` | `c_K` |
| `helper.Theta` | `c_Theta` |
| `helper.hat_Gamma^x` | `c_Gamma1` |
| `helper.hat_Gamma^z` | `c_Gamma2` |

## Stage 4C Hidden Enum And Header-Level Placement Guard

Stage 4C adds the real repo-owned hidden-sector variables:

- `c_hww`, named `hww`, immediately before `c_K`;
- `c_Aww`, named `Aww`, immediately before `c_Theta`.

The real guard now lives in `code/BlackStringToy/UserVariables.hpp`, not only
in a test file:

```cpp
static_assert(c_hww == c_K - 1, "hww must be immediately before K");
static_assert(c_Aww == c_Theta - 1, "Aww must be immediately before Theta");
```

This makes the placement check hard to skip: every normal translation unit
that includes `UserVariables.hpp` compiles the assertions. The standalone
Stage 4C placement fixture uses a layout-test-only include path to compile the
header checks without pulling in Chombo grid infrastructure.

The Stage 4C helper-input map now has real hidden component slots:

| Helper input | Component convention |
| --- | --- |
| `helper.chi` | `c_chi` |
| `helper.h_xx` | `c_h11` |
| `helper.h_xz` | `c_h12` |
| `helper.h_zz` | `c_h22` |
| `helper.hww` | `c_hww` |
| `helper.A_xx` | `c_A11` |
| `helper.A_xz` | `c_A12` |
| `helper.A_zz` | `c_A22` |
| `helper.Aww` | `c_Aww` |
| `helper.K` | `c_K` |
| `helper.Theta` | `c_Theta` |
| `helper.hat_Gamma^x` | `c_Gamma1` |
| `helper.hat_Gamma^z` | `c_Gamma2` |

The Stage 4C layout after editing is:

| Component | Index | Notes |
| --- | ---: | --- |
| `c_chi` | 0 | conformal factor |
| `c_h11`, `c_h12`, `c_h13`, `c_h22`, `c_h23`, `c_h33` | 1-6 | visible metric block |
| `c_hww` | 7 | hidden Cartesian-like conformal metric component |
| `c_K` | 8 | trace; header asserts `c_hww == c_K - 1` |
| `c_A11`, `c_A12`, `c_A13`, `c_A22`, `c_A23`, `c_A33` | 9-14 | visible trace-free block |
| `c_Aww` | 15 | hidden trace-free extrinsic-curvature component |
| `c_Theta` | 16 | Z4 scalar; header asserts `c_Aww == c_Theta - 1` |
| `c_Gamma1`, `c_Gamma2`, `c_Gamma3` | 17-19 | hatted connection-sector components |
| `c_lapse`, `c_shift1..3`, `c_B1..3` | 20-26 | gauge variables |
| `NUM_VARS` | 27 | total variable count |

Stage 4C also passed the full scratch Docker/GRChombo compile with the
27-variable layout. The inherited cheap smoke run reaches runtime but aborts
because `hww` and `Aww` are still non-finite/uninitialized in the scaffold.

Stage 4D is the next narrow implementation stage: provide finite scaffold
initialization or handoff for `hww` and `Aww` so the cheap smoke run no longer
dies immediately from NaNs. Stage 4D must remain a runtime-sanity cleanup, not a
claim of physical evolution correctness and not an implementation of cartoon
Ricci, full CCZ4 RHS, gauge/damping, finite differences, initial data, AH
finding, or radiation extraction.

Stage 4D uses an explicit smoke-only parameter,
`scaffold_freeze_hidden`, for this temporary support. The default is off; the
cheap smoke parameter file turns it on. When enabled, the scaffold freezes
`hww = 1.0`, `Aww = 0.0`, and the corresponding RHS slots at zero so the
inherited scaffold can run without hidden-variable NaNs. This is not physical
hidden-sector evolution and must be disabled or replaced before any real
cartoon hidden-sector RHS is enabled.

## Stage 4E First Grid-To-Helper Handoff Diagnostic

Stage 4E adds the first guarded grid-to-helper diagnostic. It is controlled by
the default-off `scaffold_check_hidden_handoff` parameter; the cheap smoke
parameter file enables it together with the Stage 4D hidden freeze. When
enabled, the diagnostic reads the real grid slots for `chi`, `h11`, `h12`,
`h22`, `hww`, `A11`, `A12`, `A22`, `Aww`, and `K`, packages them into the
Stage 4A local algebra helper structs, and checks the determinant, inverse,
full 4D trace, and `K_ij` reconstruction outputs for finite values.

This is a check-only path. The helper output is not written back to the grid
and is not used to evolve the system. Passing Stage 4E shows only that the
current scaffold can read the intended component slots and hand finite local
values to the Stage 4A helper during the cheap smoke run. It does not validate
cartoon Ricci terms, CCZ4 RHS terms, hidden-sector dynamics, or physical
black-string evolution.

After review, Stage 4E also includes a standalone distinct-value mapping
fixture:

- Fixture: `code/BlackStringToy/tests/Stage4EGridToHelperMappingTest.cpp`.
- It uses deliberately different local values for `chi`, `h11`, `h12`, `h22`,
  `hww`, `A11`, `A12`, `A22`, `Aww`, and `K`.
- It checks that the future handoff map populates the Stage 4A helper input
  structs with the intended component values and verifies an independently
  derived `K_ww = 51.375` oracle.

This fixture catches swapped slots that could remain finite in the symmetric
cheap smoke data. It still does not read `Cell`, `Vars`, `FArrayBox`, AMR data,
or evolution data.

## Stage 4F Cartoon Ricci Helper Interface

Stage 4F adds only the repo-owned interface for the future cartoon Ricci
helper:

- Interface: `code/BlackStringToy/CartoonRicciInterface.hpp`.
- Fixture: `code/BlackStringToy/tests/Stage4FCartoonRicciInterfaceTest.cpp`.

The interface is local-value based and explicitly targets the
metric-derivative cartoon Ricci form validated by Stages 3C-3E. A later Ricci
implementation using this interface is expected to compute Ricci from
coordinate `x`, `chi`, conformal metric components `h_xx`, `h_xz`, `h_zz`,
`hww`, their first derivatives, their second derivatives, and hidden
multiplicity. It also defines the future output shape `R_xx`, `R_xz`, `R_zz`,
and `R_ww`.

This is not the Gamma-based GRChombo `CCZ4Geometry` Ricci form. Before this
helper is wired into a CCZ4 RHS, a later stage must check how the
metric-derivative Ricci output will be consumed by the GRChombo-facing RHS
code. If that code path expects a Gamma-based Ricci form, the mismatch must be
resolved before wiring.

As committed, Stage 4F deliberately did not implement Ricci formulas, did not
return fake zero curvature, did not read grid data, and did not affect
evolution. The interface records that `hww` is the hidden Cartesian-like
conformal metric component, not `g_theta theta`, and that the external
spherical/cartoon `x^2` factor is not stored inside `hww`. Hidden multiplicity
remains fixed to two for this project. Later implementation stages must apply
the Stage 3I small-`x` regularity rules; including coordinate `x` in the
interface does not by itself make the `x -> 0` behavior safe.

## Stage 4G Local Metric-Derivative Cartoon Ricci Helper

Stage 4G implements the first local-value cartoon Ricci helper against the
Stage 4F interface:

- Helper: `compute_metric_derivative_ricci` in
  `code/BlackStringToy/CartoonRicciInterface.hpp`.
- Fixture:
  `code/BlackStringToy/tests/Stage4GCartoonRicciMetricDerivativeTest.cpp`.

The helper computes physical Ricci components from the metric-derivative data
already represented in the Stage 4F inputs: `h`, `chi`, first derivatives,
second derivatives, the reduced coordinate `x`, and hidden multiplicity. It
converts `h/chi` to the physical metric locally, assembles the cartoon
four-dimensional metric at `theta = pi/2`, and evaluates the Ricci tensor from
metric first and second derivatives. The returned `R_ww` is the
Cartesian-like hidden component, obtained from the angular component as
`R_theta theta / x^2`.

Stage 4G is away-axis only. It rejects `x <= 0` and does not implement the
Stage 3I small-`x` regularization. It also does not resolve how the
metric-derivative Ricci output should feed a GRChombo-facing RHS path if that
path expects Gamma-based Ricci data. That compatibility decision remains a
later wiring-stage requirement.

Stage 4G deliberately does not read grid data, wire into the RHS, write helper
outputs back to evolution variables, implement hidden-sector RHS terms, or
prove physical evolution correctness.

## Stage 4H Ricci-To-RHS Compatibility Decision

Stage 4H is documented in
`docs/implementation/stage4H_ricci_rhs_compatibility.md`. It inspects the
current BlackStringToy and public GRChombo CCZ4 paths and records the
compatibility issue before any RHS wiring:

- BlackStringToy currently calls the inherited public `CCZ4RHS` from
  `specificEvalRHS`.
- Public `CCZ4RHS` obtains Ricci through
  `CCZ4Geometry::compute_ricci_Z`.
- That GRChombo-facing path uses evolved `Gamma`, `d1.Gamma`, Christoffels,
  conformal metric derivatives, and `Z_over_chi`; it is the Gamma-based path,
  not the pure Stage 4G metric-derivative helper contract.

The Stage 4H recommendation is to keep Stage 4G as the checked
metric-derivative Ricci source for local tests and future repo-owned RHS
experiments, but not to wire it into evolution until a local Ricci-to-RHS
contract test defines the expected data shape.

## Stage 4I Typed Ricci Bridge Contract

Stage 4I adds the first code-level convention bridge:

- Bridge: `code/BlackStringToy/CartoonRicciBridge.hpp`.
- Fixture: `code/BlackStringToy/tests/Stage4IRicciBridgeContractTest.cpp`.

The bridge keeps `CartoonRicci::RicciComponents` as a distinct named input
type. After the Stage 4I review follow-up, the raw cartoon Ricci components are
private rather than public doubles. The bridge exposes a distinct
`RhsRicciComponents` type as the reviewed RHS-facing component view. It does
not expose Ricci as a generic array and does not add implicit conversion to any
GRChombo tensor or RHS type. Future RHS-facing code must go through this bridge
or a reviewed replacement.

The bridge pins the local convention:

- Ricci components are physical lower/lower components `R_xx`, `R_xz`, `R_zz`,
  and `R_ww`.
- `R_ww` is the Cartesian-like hidden component, not `R_theta theta`.
- The signs follow the Stage 4G metric-derivative helper.
- Full 4D contractions include the off-diagonal factor `2 h^{xz} R_xz` and
  the hidden multiplicity term `2 h^{ww} R_ww`.
- Given conformal inverse components `h^{ij}`, the bridge computes
  `h^{ij} R_ij`; the physical scalar is then `chi h^{ij} R_ij`.

Stage 4I still does not wire Ricci into the RHS, call evolution code, read grid
data, or prove RHS correctness. It only makes the convention boundary explicit
and testable.

## Stage 4J Local Ricci-To-RHS Contract

Stage 4J defines the first local contract for how bridge-approved Ricci data
may be consumed by a future repo-owned RHS source block:

- Contract: `code/BlackStringToy/CartoonRhsContract.hpp`.
- Fixture: `code/BlackStringToy/tests/Stage4JRicciRhsContractTest.cpp`.

The contract accepts `CartoonRicciBridge::RhsRicciComponents`, not raw
`CartoonRicci::RicciComponents`. It records that the Ricci input is physical
lower/lower metric-derivative cartoon Ricci, that `R_ww` and `hww` are
Cartesian-like hidden components rather than angular `theta theta` components,
and that the external spherical/cartoon `x^2` factor is not stored inside
`hww`.

Stage 4J is still local only. It may compute named Ricci contractions such as
`h^{ij} R_ij` and `chi h^{ij} R_ij` from already-bridged data, but it does not
implement real RHS source formulas, does not call evolution code, does not read
grid data, and does not implement small-axis regularization.

After review, the internal `RicciAccess` doorway in the Stage 4I bridge is
private. The intended public path remains `to_rhs_ricci_components(...)`,
which returns a typed by-value `RhsRicciComponents` view. This internal doorway
must not be changed to expose raw doubles, references, pointers, or mutable
aliases to the hidden cartoon Ricci storage.

## Stage 4K Local RHS Source-Block Skeleton

Stage 4K adds the future local source-block container:

- Skeleton: `code/BlackStringToy/CartoonRhsSourceBlock.hpp`.
- Fixture: `code/BlackStringToy/tests/Stage4KLocalRhsSourceBlockTest.cpp`.

The skeleton consumes `CartoonRhsContract::RhsLocalInputs`, not raw cartoon
Ricci data. Its output structure names future RHS pieces such as `rhs_chi`,
`rhs_hww`, `rhs_Aww`, and `rhs_K`, but Stage 4K provides only an explicit inert
contract-test path. Those zero outputs are placeholders for checking the local
container and must not be interpreted as physical source terms.

Stage 4K does not implement hidden-sector evolution, full CCZ4 RHS formulas,
gauge or damping terms, finite differences, small-axis regularization, or any
live evolution wiring.

## Stage 4L Local Trace-Free Ricci Projection Block

Stage 4L implements the first local formula block inside the RHS source-block
skeleton: the 4D trace-free projection of bridge-approved Ricci data.

- Formula location: `code/BlackStringToy/CartoonRhsSourceBlock.hpp`.
- Fixture: `code/BlackStringToy/tests/Stage4LRicciTraceFreeSourceTest.cpp`.

The block consumes the Stage 4K local source-block input, which itself carries
the Stage 4J contract and Stage 4I bridge-approved Ricci view. It is a thin
wrapper around `ConformalCartoonAlgebra::trace_free_part`, so the `/4`
projection denominator comes from the shared Stage 4A convention constant
rather than a local literal. The trace uses the full 4D rule with both the
off-diagonal `2 h^{xz} R_xz` term and the hidden `2 h^{ww} R_ww` contribution.
The Stage 4L boundary reuses the existing contract validation and rejects
`x <= 0`; full small-axis regularization remains deferred.

Stage 4L still does not implement the full CCZ4 RHS, does not add live
evolution wiring, does not read grid data, does not implement gauge or damping
terms, and does not implement small-axis regularization.

## Stage 4M Away-Axis Policy

Stage 4M adds one named local policy for deciding whether a cartoon/RHS
expression may evaluate at a supplied reduced coordinate `x`:

- Policy: `code/BlackStringToy/CartoonAxisPolicy.hpp`.
- Fixture: `code/BlackStringToy/tests/Stage4MAxisPolicyTest.cpp`.

The implemented mode is away-axis only. The policy accepts only finite
positive `x`, rejects `x = 0`, negative `x`, NaN, and infinity, and provides
guarded `1/x` and `1/x^2` helper entry points. Stage 4J's local contract
boundary now uses this policy instead of a local ad hoc `x > 0` check, so Stage
4K and Stage 4L continue to inherit the same boundary.

Stage 4M is deliberately not Stage 3I regularization. It does not clamp `x`,
replace the axis with an epsilon, provide a finite axis limit, or make
cartoon-axis evaluation physically valid. The Stage 3I parity and removable
singularity logic remains deferred to a later implementation stage.

## Stage 4N Guarded Singular Combinations

Stage 4N adds local away-axis helpers for singular-looking cartoon
combinations:

- Helper: `code/BlackStringToy/CartoonSingularCombinations.hpp`.
- Fixture: `code/BlackStringToy/tests/Stage4NSingularCombinationsTest.cpp`.

The helpers form combinations such as `d_x f / x` and `(f - g) / x^2` by
routing through `CartoonAxisPolicy` instead of raw division. They require all
input values to be finite, reject zero/negative/nonfinite `x`, and do not
claim regularized-axis support.

Stage 4N prepares the local building blocks future RHS/Ricci source terms will
need, but it does not add a new physical source term, does not read grid data,
does not touch evolution, and does not implement Stage 3I small-axis
regularization. Future source code should avoid raw `1/x` and `1/x^2` and use
the policy/helper path instead.

## Stage 4O Axis-Regime Semantics

Stage 4O settles the current axis-regime semantics before real singular source
terms use the Stage 4M/4N helpers:

- Policy location: `code/BlackStringToy/CartoonAxisPolicy.hpp`.
- Fixture:
  `code/BlackStringToy/tests/Stage4OAxisRegimeSemanticsTest.cpp`.

The only implemented regime is `AwayAxisOnly`. The policy accepts finite
positive `x` and rejects `x = 0`, negative `x`, NaN, and infinity. It does not
clamp `x`, substitute an epsilon, or claim that evaluation on the cartoon axis
is physically valid.

The guarded `1/x^2` helper is a separately guarded away-axis primitive, not a
future definition by squaring a possibly clamped `1/x`. If a later
regularized or clamped regime is introduced, it must decide the `1/x` and
`1/x^2` semantics independently. True Stage 3I finite-limit regularization
remains a later stage.

## Stage 4P Cartoon Geometry Primitives

Stage 4P adds the first named local primitives for singular-looking
cartoon-geometry combinations:

- Helper: `code/BlackStringToy/CartoonGeometryPrimitives.hpp`.
- Fixture:
  `code/BlackStringToy/tests/Stage4PCartoonGeometryPrimitivesTest.cpp`.

The primitive block provides the low-risk away-axis route for `(d_x hww) / x`
through the Stage 4N singular-combination helper, which in turn uses the
Stage 4O away-axis semantics. After the Stage 4R review patch, raw Stage 4P no
longer exposes `(hxx - hww) / x^2` as an ordinary public output. The
source-facing route for that risky value is the Stage 4R guarded sub-block.

Stage 4P is not a full Ricci tensor, not a full CCZ4 RHS source term, and not
Stage 3I small-axis regularization. It does not clamp `x`, introduce
`eps_cut`, substitute epsilon for the axis, read grid data, compute finite
differences, or touch evolution.

The `(hxx - hww) / x^2` source ingredient has an additional regularity precondition:
finite axis behavior requires the Stage 3I matching condition
`hxx - hww = O(x^2)`. Stage 4P does not enforce that condition on grid data,
and a large finite away-axis value is not a regularity validation. A later
regularity/matching guard must check or construct the matching behavior before
this primitive is used in a real source block near the axis.

## Stage 4Q Local Regularity Matching Guard

Stage 4Q adds the first local guard for the Stage 3I matching condition needed
by `(hxx - hww) / x^2`:

- Helper: `code/BlackStringToy/CartoonRegularityChecks.hpp`.
- Fixture: `code/BlackStringToy/tests/Stage4QRegularityMatchingTest.cpp`.

The guard checks finite inputs, applies the Stage 4O away-axis policy to `x`,
and rejects clear pointwise violations of `hxx - hww = O(x^2)` using the named
residual tolerance `hxx_hww_matching_tolerance`. The local residual routes
through the guarded `CartoonAxisPolicy::inverse_x2_away_axis(x)` helper and a
simple finite metric scale, so obvious `O(1)` mismatches near the axis fail
before future source blocks trust the Stage 4R guarded ingredient.

Stage 4Q is still not Stage 3I small-axis regularization. A pointwise guard
cannot prove analytic regularity, does not construct finite axis limits, does
not repair mismatched data, and does not read grid data, compute finite
differences, add a physical Ricci/RHS source term, or touch evolution.

## Stage 4R Regularity-Guarded Source Sub-Block

Stage 4R adds the first local source-style boundary around the regularity
sensitive cartoon geometry ingredients:

- Helper: `code/BlackStringToy/CartoonRegularityGuardedSources.hpp`.
- Fixture:
  `code/BlackStringToy/tests/Stage4RRegularityGuardedSourceBlockTest.cpp`.

The helper calls the Stage 4Q `hxx - hww = O(x^2)` matching guard before
returning the Stage 4P low-risk primitive `(d_x hww) / x` and the guarded
source-facing `(hxx - hww) / x^2`. It also returns the local matching residual
so future tests can inspect what the guard accepted. The calculation remains
away-axis-only through the Stage 4O policy and uses the Stage 4P primitive path
rather than duplicating singular-looking formulas.

Stage 4R is not a full Ricci tensor, not a full CCZ4 RHS formula, not Stage 3I
small-axis regularization, and not evolution wiring. It does not read grid
data, compute finite differences, repair or clamp inputs, or construct finite
axis limits.

## Stage 4S Local RHS Guarded-Geometry Integration

Stage 4S connects the Stage 4R guarded geometry package to the existing local
RHS source-block skeleton:

- Integration point: `code/BlackStringToy/CartoonRhsSourceBlock.hpp`.
- Fixture:
  `code/BlackStringToy/tests/Stage4SLocalRhsGuardedGeometryIntegrationTest.cpp`.

The source-block layer now exposes a narrow local function that accepts local
doubles through the Stage 4R input shape, calls the Stage 4R helper, and
carries the resulting guarded geometry package as a nested source-block
output. The regularity-sensitive `(hxx - hww) / x^2` value is source-facing
only through that guarded package, after the Stage 4Q matching guard has
accepted the inputs.

Stage 4S does not duplicate the Stage 4Q guard, does not duplicate the Stage
4R formulas, and does not expose the raw Stage 4P metric-difference path. It
does not implement full Ricci, full CCZ4 RHS, small-axis regularization,
finite differences, grid reads, parameter switches, or live evolution wiring.

After review, the Stage 4R guarded package is no longer an open aggregate.
Its values are private and exposed through const accessors, with construction
limited to the guarded compute path. The Stage 4S carry output is likewise not
brace-initializable by arbitrary callers. The former Stage 4P `detail` helper
for `(hxx - hww) / x^2` was removed, so ordinary source-block code has no
named metric-difference bypass around the Stage 4Q guard.

## Stage 4T Guarded Source-Consumer Probe

Stage 4T adds the first local consumer of the checked guarded geometry
package:

- Consumer:
  `code/BlackStringToy/CartoonGuardedSourceConsumers.hpp`.
- Fixture:
  `code/BlackStringToy/tests/Stage4TGuardedSourceConsumerTest.cpp`.

This stage deliberately chooses a diagnostic/probe consumer rather than a
physics source formula. No small Ricci or RHS sub-expression was introduced
because no isolated, already-validated physical formula was extracted in this
stage. The probe accepts only the non-forgeable Stage 4R guarded package,
mirrors the checked `(d_x hww) / x`, `(hxx - hww) / x^2`, and matching
residual values through accessor methods, and reports whether the residual is
within the Stage 4Q tolerance.

Stage 4T therefore proves the consumer boundary can require the checked
package type, but it does not implement full Ricci, a Ricci sub-block, full
CCZ4 RHS, small-axis regularization, grid reads, finite differences, parameter
switches, or live evolution wiring. A later physics consumer must still point
to a specific validated Stage 3/Stage 4 derivation or local expression before
using these ingredients in a real formula.

## Stage 4U Source-Formula Authoring Gate

Stage 4U adds a typed authoring gate for future source formulas that need
regularity-sensitive cartoon geometry:

- Gate: `code/BlackStringToy/CartoonSourceFormulaAuthoringGate.hpp`.
- Fixture:
  `code/BlackStringToy/tests/Stage4USourceFormulaAuthoringGateTest.cpp`.

The gate defines `RegularitySensitiveSourceInputs`, a non-aggregate input type
that carries the checked Stage 4R geometry package. Future source formulas
that need `(hxx - hww) / x^2` should take this type, or the checked
`RegularityGuardedGeometrySources` package directly, rather than raw
`h_xx`, `h_ww`, and `x` values.

The authoring rule is explicit: direct use of
`CartoonSingularCombinations::difference_over_x2(h_xx, h_ww, x)` for this
metric matching-sensitive ingredient is forbidden in source formulas. The
generic helper remains available for generic away-axis combinations, but not
as the source-facing route for `(hxx - hww) / x^2`. This generic-helper bypass
is not mechanically blocked by Stage 4U; the typed path is enforced, while
hand-written or generic-helper recomputation remains a review responsibility
unless a later lint/CI gate is added.

Stage 4U does not implement a real Ricci formula, a CCZ4 RHS term, small-axis
regularization, grid reads, finite differences, parameter switches, or live
evolution wiring.

## Stage 4W Hidden-Sphere CCZ4 Rww Derivation Lock

Stage 4W is a Markdown derivation-lock stage:

- Note:
  `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md`.

The note maps how hidden-sphere quantities enter the local CCZ4 source blocks:
the full cartoon shift divergence in the `chi` equation, direct hidden
Lie/cartoon terms in the `h_ww` equation, full Ricci scalar traces in `K` and
`Theta`, and the trace-free curvature/lapse block in `A_IJ`. It identifies
the physical CCZ4 input `R_ww[gamma]` as the first serious hidden Ricci target,
separates the conformal warped-product target `tilde{R}_ww[h]` from the
`d = 4` conformal-factor correction `R^chi_ww`, and records the future
varying-`chi` consistency oracle.

Stage 4W does not add formula code. It also blocks full `R_ww` implementation
until a missing off-diagonal regularity guard exists: the gradient expansion
contains both the Stage 4Q/4R/4U-protected `(hxx - hww) / x^2` path and an
unguarded `h^xz h_xz / x^2` contribution. Stage 4W derives `h_xz = O(x)` from
reflection parity about the cartoon axis and records that the curvature/lapse
RHS sign must be pinned against the published CCZ4/BSSN formulation and the
Stage 3A `K_IJ` convention caveat. The next narrow coding stage should
therefore be Stage 4X, an off-diagonal axis/parity guard for `h_xz = O(x)`,
not an `R_ww[gamma]` implementation.

## Implementation Stages And Gates

| Stage | Candidate repo-owned target | Purpose | Inputs | Outputs | Prior-stage dependency | Required Stage 3J tests | Risk |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Stage 4A local algebra helper | `code/BlackStringToy/ConformalCartoonAlgebra.hpp` | Determinant, inverse, trace-free projection, `/4`, `K_ij` reconstruction | Local scalar components `h_xx`, `h_xz`, `h_zz`, `hww`, `A_ij`, `K` supplied directly by tests, not read from grid data | Algebraic quantities and projections | Stages 3F-3G | determinant, off-diagonal inverse, full-4D trace-free, `/4`, `K_xz`, diagonal-limit fixtures | Medium |
| Stage 4A local algebra fixture target | `code/BlackStringToy/tests/Stage4AConformalCartoonAlgebraTest.cpp` | Verify helper without evolution or grid variables | Hard-coded exact fixtures from Stages 3F-3G | Pass/fail with roundoff-level checks | Stage 3J | all algebra helper tests, negative denominator guards | Medium |
| Stage 4B public CCZ4 baseline layout check | `code/BlackStringToy/tests/Stage4BVariableLayoutTest.cpp` | Prove the public CCZ4 comparison baseline has not drifted unexpectedly | Public CCZ4 enum and names only; no hidden enum symbols | Loud failure on public visible layout/name drift; no claim about real `hww/Aww` placement | Stage 1, Stage 3J | public-layout fixture and visible helper-map check | Medium |
| Stage 4C hidden enum and header-level placement guard | `code/BlackStringToy/UserVariables.hpp`; `code/BlackStringToy/tests/Stage4CVariablePlacementTest.cpp` | Add repo-owned `hww/Aww` enum names and enforce their final placement where the header is included | Actual repo-owned hidden enum definitions | `UserVariables.hpp` static assertions for the final hidden metric/A slots; placement fixture with real helper map | Stage 4B review | real `hww/Aww` placement guard, AH positional hazard, hidden determinant/trace participation setup | High |
| Stage 4D smoke-only hidden-variable freeze | `BlackStringToyLevel` plus smoke parameter file | Give `hww/Aww` finite scaffold values only when the smoke-only parameter is enabled | Repo-owned enum with hidden variables; inherited scaffold state/RHS slots | Cheap smoke run reaches past the current non-finite hidden-variable failure without silently freezing future runs by default | Stage 4C review and explicit approval | Stage 4C placement guard plus smoke-run NaN regression | High |
| Stage 4E first grid-to-helper handoff diagnostic | `BlackStringToyLevel` guarded scaffold diagnostic plus `Stage4EGridToHelperMappingTest.cpp` | Connect Stage 4A helpers to live component slots for the first time in a check-only path and guard the map with distinct local values | Repo-owned enum with hidden variables and inherited scaffold grid state; standalone distinct local fixture values | Loud failure if helper inputs are non-finite, the local conformal determinant is invalid, or the intended helper input map is swapped; no writes or evolution use | Stage 4D review plus explicit approval | helper input map, enum/layout, no-RHS handoff fixture | High |
| Stage 4F cartoon Ricci helper interface | `CartoonRicciInterface.hpp`; `Stage4FCartoonRicciInterfaceTest.cpp` | Define the metric-derivative local-value input/output contract for future cartoon Ricci calculations | Local `x`, `chi`, reduced conformal metric components, first derivatives, and second derivatives | Type/interface contract | Stages 3C-3E, 3G, 4E | round-`S^2`, Stage 3C-3E Ricci fixtures, sheared-flat gate as later implementation tests; later GRChombo-facing Ricci-form compatibility check | High |
| Stage 4G local metric-derivative cartoon Ricci helper | `CartoonRicciInterface.hpp`; `Stage4GCartoonRicciMetricDerivativeTest.cpp` | Compute local physical Ricci components from metric-derivative cartoon data without grid/RHS wiring | Stage 4F local-value inputs | `R_xx`, `R_xz`, `R_zz`, `R_ww` for away-axis inputs | Stage 4F review and explicit approval | flat zero, constant-`q` cone, nonconstant-`q` warped-product oracle, sheared-flat off-diagonal gate, away-axis guard | High |
| Stage 4H Ricci/RHS compatibility note | `docs/implementation/stage4H_ricci_rhs_compatibility.md` | Decide how the Stage 4G metric-derivative Ricci helper can later feed CCZ4 RHS work without mixing Ricci conventions | Current BlackStringToy RHS path and public GRChombo CCZ4 geometry headers | Recommendation and future contract requirements only | Stage 4G review | Ricci-form compatibility review; no code execution | High |
| Stage 4I typed Ricci bridge contract | `CartoonRicciBridge.hpp`; `Stage4IRicciBridgeContractTest.cpp` | Make the Ricci convention boundary explicit before any RHS use | Opaque `CartoonRicci::RicciComponents`, conformal inverse metric, `chi` | Distinct `RhsRicciComponents` bridge view, full 4D contraction, and physical scalar contract only; no RHS writes | Stage 4H | `451` hard-coded trace oracle, hidden/off-diagonal multiplicity negative checks, private raw-component storage, no-direct-use rule | High |
| Stage 4J local Ricci-to-RHS contract | `CartoonRhsContract.hpp`; `Stage4JRicciRhsContractTest.cpp` | Define the local future RHS entry shape for bridge-approved Ricci data without implementing RHS formulas | `CartoonRicciBridge::RhsRicciComponents`, conformal inverse metric, `chi`, local `x` | Named Ricci contractions and inert local output struct only; no RHS writes | Stage 4I | bridge-only input type, `451` trace oracle, Rww/off-diagonal omission negative checks, away-axis guard | High |
| Stage 4K local RHS source-block skeleton | `CartoonRhsSourceBlock.hpp`; `Stage4KLocalRhsSourceBlockTest.cpp` | Define the future local source-block container without implementing physics RHS formulas | Stage 4J `RhsLocalInputs` contract | Named inert output fields for future source pieces; no RHS writes | Stage 4J | accepts Stage 4J contract type, rejects raw Ricci by type, finite inert placeholder fields, no-real-RHS guard | High |
| Stage 4L local trace-free Ricci projection block | `CartoonRhsSourceBlock.hpp`; `Stage4LRicciTraceFreeSourceTest.cpp` | Compute only the local 4D trace-free projection of bridge-approved Ricci | Stage 4K source-block input with lower/inverse conformal metric and Ricci | `R_xx^TF`, `R_xz^TF`, `R_zz^TF`, `R_ww^TF`; no RHS writes | Stage 4K | hard-coded projection oracle, zero trace in consistent metric case, `/4` denominator and hidden `ww` sensitivity checks, `x = 0` rejection | High |
| Stage 4M away-axis policy | `CartoonAxisPolicy.hpp`; `Stage4MAxisPolicyTest.cpp` | Centralize the finite `x > 0` decision before adding explicit `1/x` or `1/x^2` source terms | Local reduced coordinate `x` | Away-axis validation and guarded inverse helpers only; no small-axis regularization | Stage 4L | finite-positive acceptance, zero/negative/NaN/infinity rejection, guarded inverse helpers, Stage 4L boundary still rejects `x = 0` | High |
| Stage 4N guarded singular combinations | `CartoonSingularCombinations.hpp`; `Stage4NSingularCombinationsTest.cpp` | Provide guarded away-axis local combinations such as `d_x f / x` and `(f - g) / x^2` | Finite local values and reduced coordinate `x` | Singular-looking local combinations through `CartoonAxisPolicy`; no RHS writes | Stage 4M | finite-value checks, zero/negative/NaN/infinity rejection, agreement with Stage 4M inverse helpers | High |
| Stage 4O axis-regime semantics | `CartoonAxisPolicy.hpp`; `Stage4OAxisRegimeSemanticsTest.cpp` | Lock current support as away-axis-only and prevent future clamp drift in `1/x^2` semantics | Local reduced coordinate `x` | Explicit `AwayAxisOnly` regime, no regularized/clamped mode, separately guarded `1/x^2` primitive | Stage 4N | implemented-regime check, inverse-helper checks, invalid-axis rejection, no-regularized-mode guard | High |
| Stage 4P cartoon geometry primitives | `CartoonGeometryPrimitives.hpp`; `Stage4PCartoonGeometryPrimitivesTest.cpp` | Name first away-axis singular-geometry primitive before source-term assembly | Local `x` and `d_x hww` values | Public `(d_x hww) / x` through Stage 4N helpers; no public risky metric-difference output | Stage 4O | finite-value checks, zero/negative/NaN/infinity rejection, agreement with Stage 4N helpers, no-full-RHS guard, no-public-risky-field check | High |
| Stage 4Q local regularity matching guard | `CartoonRegularityChecks.hpp`; `Stage4QRegularityMatchingTest.cpp` | Catch obvious local violations of `hxx - hww = O(x^2)` before near-axis source-block use | Local `x`, `h_xx`, `h_ww`, and tolerance | Pointwise matching residual and throwing guard only; no finite axis limit construction | Stage 4P | matching example passes, `O(1)` mismatch fails, zero/negative/NaN/infinity rejection, no-regularization guard | High |
| Stage 4R regularity-guarded source sub-block | `CartoonRegularityGuardedSources.hpp`; `Stage4RRegularityGuardedSourceBlockTest.cpp` | Package regularity-sensitive cartoon ingredients behind the Stage 4Q guard | Local `x`, `h_xx`, `h_ww`, and `d_x hww` values | Guarded `(d_x hww) / x`, source-facing `(hxx - hww) / x^2`, and matching residual; no RHS writes | Stage 4Q | matching case agrees with Stage 4P for low-risk primitive, mismatch rejection, invalid input rejection, no-full-RHS/no-regularization guards | High |
| Stage 4S local RHS guarded-geometry integration | `CartoonRhsSourceBlock.hpp`; `Stage4SLocalRhsGuardedGeometryIntegrationTest.cpp` | Carry the Stage 4R guarded geometry package through the local RHS source-block skeleton | Local `x`, `h_xx`, `h_ww`, and `d_x hww` values | Non-forgeable guarded geometry package in the local source-block output; no RHS writes | Stage 4R | matching pass-through, mismatch rejection, invalid input rejection, agreement with Stage 4R guarded path, no-open-aggregate checks, no-full-RHS/no-evolution guards | High |
| Stage 4T guarded source-consumer probe | `CartoonGuardedSourceConsumers.hpp`; `Stage4TGuardedSourceConsumerTest.cpp` | Add the first local consumer that requires the checked guarded geometry package | Non-forgeable Stage 4R guarded geometry package, possibly carried by Stage 4S | Diagnostic/probe view of checked values and Stage 4Q residual status; no RHS writes | Stage 4S | accepts checked package, rejects raw-double call shape by type, matching example oracle, mismatch fails before consumer, no-full-RHS/no-evolution guards | High |
| Stage 4U source-formula authoring gate | `CartoonSourceFormulaAuthoringGate.hpp`; `Stage4USourceFormulaAuthoringGateTest.cpp` | Define the typed authoring entry point for future formulas needing regularity-sensitive geometry | Checked Stage 4R package or Stage 4S carried package | Non-aggregate `RegularitySensitiveSourceInputs` for future formula signatures; no RHS writes | Stage 4T | sanctioned Stage 4S/4R construction, no raw-double consumer call shape, mismatch fails before gate, no-full-RHS/no-evolution guards | High |
| Stage 4V source-formula derivation-lock gate | Documentation only unless a tiny expression is already derived | Decide whether the first real authoring-gate consumer is derivation-locked | Stage 4U authoring gate plus Stage 4G/4L validated helper context | Outcome B for now: no formula code until a precise sub-expression, coefficient/sign convention, and oracle are extracted | Stage 4U | docs name the derivation gap; no speculative formula, no RHS writes | High |
| Stage 4W hidden-sphere Rww derivation lock | `docs/derivations/stage4W_hidden_sphere_CCZ4_Rww_derivation.md` | Lock hidden-sphere CCZ4 contribution map and first physical `R_ww[gamma]` target | Stage 4G-compatible Ricci oracles, Stage 4U guarded-path context, Stage 3I regularity expectations, Stage 3A sign caveat | Documentation only; blocks `R_ww[gamma]` code until `h_xz = O(x)` guard exists | Stage 4V | conformal/physical Ricci split, hidden trace map, varying-`chi` oracle, flat/cone/nonconstant oracles, sign lock, no code | High |

Deferred later stages, requiring explicit user approval after the layout and
smoke-only scaffold stages pass:

| Later stage | Candidate repo-owned target | Purpose | Inputs | Outputs | Prior-stage dependency | Required Stage 3J tests | Risk |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Future small-axis regularization interface/implementation | New repo-owned regularization helper | Isolate regularized small-`x` combinations and connection limits | Taylor-like local fields, `h_xx-hww`, `h_xz`, `Z^A` | Interface contract first; implementation only after approval | Stage 3I and Stage 4U | `hat_Gamma^x` assembled guard, regular/irregular Taylor data | High |
| Stage 4X off-diagonal axis/parity guard | New repo-owned regularity-check helper | Add the missing `h_xz = O(x)` guard before implementing `R_ww` | Local `x`, `h_xz`, and scale/tolerance data | Pointwise guard only; no `R_ww` code | Stage 4W | regular/irregular off-diagonal Taylor data, no-full-RHS guard | High |
| Future additional RHS formula implementation | Future repo-owned RHS source-block implementation | Fill selected source-block terms only after local skeleton and regularity gates pass | Stage 4K source-block input plus reviewed local formulas | Computed source terms for reviewed blocks only | Stages 3H-3J and Stage 4X | RHS block matrix, flat/sheared-flat, uniform-string, reference comparison | Very high |
| Future RHS block wiring | Future repo-owned RHS class or wrapper | Connect source blocks to evolution only after local contracts pass | Grid variables and derivatives | Time derivatives | Stages 3H-3J and future formula stages | RHS block matrix, flat/sheared-flat, uniform-string, reference comparison | Very high |

Unknown exact class and file names should be resolved by a separate code
inspection pass immediately before implementation. Do not invent a new
abstraction if a local GRChombo pattern already exists.

## Likely Repo-Owned Files To Inspect Before Coding

- `code/BlackStringToy/UserVariables.hpp`
  - Purpose: repo-owned enum and variable-name layout.
  - Current state: extends public CCZ4 naming with real `hww` and `Aww`
    entries.
  - Validation gate: header-level hidden `hww/Aww` placement assertions, AH
    positional hazard, determinant/trace participation tests.
  - Risk: high.
- `code/BlackStringToy/BlackStringToyLevel.hpp` and
  `code/BlackStringToy/BlackStringToyLevel.cpp`
  - Purpose: future integration point for level setup, initial data, RHS, and
    diagnostics.
  - Inputs: GRChombo level data, simulation parameters, future helper outputs.
  - Outputs: initialized/evolved fields and diagnostics.
  - Validation gate: no RHS wiring until helper tests pass.
  - Risk: high.
- `code/BlackStringToy/SimulationParameters.hpp`
  - Purpose: future parameter parsing for black-string, gauge, and fixture
    choices.
  - Inputs: parameter files.
  - Outputs: typed runtime parameters.
  - Validation gate: parameter smoke tests and no physical-claim warnings.
  - Risk: medium.
- New repo-owned helper headers/sources, names TBD.
  - Purpose: isolate algebra, geometry, and axis regularity from driver code.
  - Inputs: local field components and derivative data.
  - Outputs: deterministic algebra/source quantities.
  - Validation gate: Stage 3J fixtures before evolution integration.
  - Risk: medium to high depending on block.

## Test Mapping

| Implementation stage | Required tests before or alongside implementation |
| --- | --- |
| Determinant helper | Stage 3F/3G determinant and normalized `det h_4D = 1` fixtures; hidden `hww` participation check |
| Reduced inverse helper | Stage 3G block-inverse identity, including nonzero `h_xz` and diagonal limit |
| Trace-free projection | Full 4D trace-free projection with hidden multiplicity; `/4` denominator negative tests |
| `K_ij` reconstruction | Stage 3F/3G `K_xx`, `K_xz`, `K_zz`, `K_ww` reconstruction fixtures |
| Public layout baseline | Stage 4B public CCZ4 layout fixture; visible helper-map drift check |
| Hidden enum placement | Stage 4C header-level `UserVariables.hpp` assertions for real `hww/Aww` symbols; AH positional hazard check |
| Smoke-only hidden freeze | Stage 4D smoke parameter and runtime guard; verify default-off behavior is documented and the cheap smoke file opts in explicitly |
| Grid-variable handoff diagnostic | Stage 4E `scaffold_check_hidden_handoff` path plus the distinct-value mapping fixture; read intended enum/component slots into Stage 4A helper structs and check finite determinant/inverse/trace/`K_ij` outputs without writing helper results |
| Cartoon Ricci interface | Stage 4F compile-only/type fixture for the metric-derivative Ricci form |
| Cartoon Ricci implementation | Stage 4G fixture with Stage 3C flat/cartoon geometry, Stage 3D constant-`q0`, Stage 3E nonconstant `q`, and sheared-flat Stage 3G Ricci gate; round-`S^2` and additional axis-regularized fixtures remain future extensions |
| Away-axis policy | Stage 4M fixture proving finite-positive acceptance, zero/negative/NaN/infinity rejection, guarded `1/x` and `1/x^2`, and continued Stage 4L `x = 0` rejection |
| Singular combinations | Stage 4N fixture proving guarded `d_x f / x`, guarded `(f - g) / x^2`, finite-input checks, axis rejection, and agreement with Stage 4M inverse helpers |
| Axis-regime semantics | Stage 4O fixture proving `AwayAxisOnly` is the only implemented regime, regularized/clamped axis support is not exposed as working, and `1/x^2` remains a separately guarded primitive |
| Cartoon geometry primitives | Stage 4P fixture proving named `(d_x hww) / x` routes through Stage 4N, rejects invalid inputs, and raw Stage 4P does not expose a public risky `(hxx - hww) / x^2` field |
| Local regularity matching guard | Stage 4Q fixture proving a local `hxx - hww = O(x^2)`-style sample passes, an obvious `O(1)` mismatch fails, invalid inputs reject, and the guard does not claim analytic regularity |
| Regularity-guarded source sub-block | Stage 4R fixture proving the guarded package agrees with Stage 4P for `(d_x hww) / x`, exposes the risky metric-difference ingredient only after Stage 4Q accepts matching data, rejects mismatches and invalid inputs, and does not claim full Ricci/RHS or regularization |
| Source-formula derivation-lock gate | Stage 4V documentation-only gate proving no formula is added until a precise Stage 3/4 sub-expression, coefficient/sign convention, and oracle are derived or extracted for the Stage 4U checked package |
| Hidden-sphere Rww derivation lock | Stage 4W documentation-only gate locking the hidden-sphere contribution map, `tilde{R}_ww[h]`, `R^chi_ww`, physical `R_ww[gamma]`, Stage 4G-compatible oracles, the varying-`chi` oracle, sign-convention check, and missing `h_xz = O(x)` guard before implementation |
| Small-axis helper | Stage 3I regular and irregular Taylor fixtures; assembled `tilde_Gamma^x` / `hat_Gamma^x` limit guard |
| Constraint damping | Not a Stage 4A task; requires Stage 3H/3J linearized constraint-violation injection milestone |
| Gauge/Gamma driver | Not a Stage 4A task; requires ownership and convention confirmation |

## Deferred Work

Defer all of the following beyond Stage 4A:

- full CCZ4 RHS implementation;
- Gamma-driver and gauge equation implementation;
- constraint-damping block implementation;
- linear GL dispersion numerical milestone;
- turduckening/interior implementation;
- finite-difference stencil, ghost-cell, and axis-boundary implementation;
- black-string initial-data implementation;
- apparent horizon finding;
- waveform/radiation extraction;
- production evolution runs;
- comparison with external/reference implementations, except as a later
  validation gate.

## Risk Table

| Risk | Where it can enter | Stage 3 gate | Implementation mitigation |
| --- | --- | --- | --- |
| Wrong hidden multiplicity `N_hidden=2` | Trace, Ricci, `Aww`, `hat_Gamma^x`, diagnostics | 3C, 3F, 3G, 3I, 3J | Centralize multiplicity constant and test negative cases |
| Using 2D trace instead of 4D trace | Trace-free projection, `K_ij` split, RHS terms | 3F, 3G, 3J | Unit-test `/4` denominator and full hidden trace |
| Missing `hww` in determinant or trace | Conformal normalization and projection | 3F, 3G, 3J | Determinant and trace fixtures vary `hww` independently |
| Treating `hww` as `g_theta theta` | Initial data, Ricci, diagnostics | 3A, 3B, 3F, 3J | Keep external `x^2` out of `hww`; add explicit fixture docs |
| Reciprocal inverse with nonzero `h_xz` | Off-diagonal algebra and RHS | 3G, 3J | Block-inverse helper and matrix-product identity tests |
| Residual `1/x` in `hat_Gamma^x` | Axis regularity and Gamma-driver coupling | 3I, 3J | Assembled connection limit guard with regular and irregular Taylor data |
| Treating flat-zero checks as enough for damping | Constraint-damping block | 3H, 3J | Require separate constraint-violation injection milestone |
| Modifying `external/GRChombo` | Any implementation stage | Stage 2 workflow | Use scratch copy under `runs/` and repo-owned code only |
| Trusting compilation alone | Any implementation stage | All Stage gates | Require fixture tests and documented validation before claims |
| Enum layout breaks AH/source assumptions | `UserVariables.hpp`, AH access, diagnostics | Stage 1 and Stage 3J | Review enum order and add layout tests before implementation |
| Stage 4B overclaimed as hidden-placement protection | Planning docs or review notes | Stage 4B review | State clearly that Stage 4B checks public CCZ4 baseline layout only; real `hww/Aww` placement waits for Stage 4C |
| Helper formulas correct but wired to wrong grid component | Enum/component layout, AHFunctions positional assumptions, `hww/K` adjacency assumptions | Stage 3J and Stage 4B-4D layout gates | Keep Stage 4A helpers local-value-only until Stage 4C header assertions and a later explicit handoff test exist |
| Temporary `hww/Aww` smoke freeze silently masks future physics | Stage 4D scaffold support if left unconditional | Stage 4D review | Keep `scaffold_freeze_hidden` default-off, enable it only in the cheap smoke parameter file, and require future real hidden-sector RHS support to disable or replace it |

## Acceptance Criteria

Stage 3K is complete when:

- this minimal C++ implementation plan exists;
- the first implementation stage is explicitly chosen;
- Stage 4A is limited to local algebra-helper functions and local/non-grid
  fixtures;
- automated enum/layout gating is required before any grid-variable wiring;
- all deferred blocks are listed;
- implementation targets are mapped without editing code;
- every proposed implementation stage links to Stage 3J tests;
- protected paths are untouched;
- Stage 4A does not wire helpers to grid variables or evolution.
