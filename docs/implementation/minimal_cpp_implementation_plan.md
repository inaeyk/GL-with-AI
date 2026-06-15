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

A broader helper-to-grid handoff test remains required before Stage 4A helpers
are allowed to consume live evolution components.

## Implementation Stages And Gates

| Stage | Candidate repo-owned target | Purpose | Inputs | Outputs | Prior-stage dependency | Required Stage 3J tests | Risk |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Stage 4A local algebra helper | `code/BlackStringToy/ConformalCartoonAlgebra.hpp` | Determinant, inverse, trace-free projection, `/4`, `K_ij` reconstruction | Local scalar components `h_xx`, `h_xz`, `h_zz`, `hww`, `A_ij`, `K` supplied directly by tests, not read from grid data | Algebraic quantities and projections | Stages 3F-3G | determinant, off-diagonal inverse, full-4D trace-free, `/4`, `K_xz`, diagonal-limit fixtures | Medium |
| Stage 4A local algebra fixture target | `code/BlackStringToy/tests/Stage4AConformalCartoonAlgebraTest.cpp` | Verify helper without evolution or grid variables | Hard-coded exact fixtures from Stages 3F-3G | Pass/fail with roundoff-level checks | Stage 3J | all algebra helper tests, negative denominator guards | Medium |
| Stage 4B public CCZ4 baseline layout check | `code/BlackStringToy/tests/Stage4BVariableLayoutTest.cpp` | Prove the public CCZ4 comparison baseline has not drifted unexpectedly | Public CCZ4 enum and names only; no hidden enum symbols | Loud failure on public visible layout/name drift; no claim about real `hww/Aww` placement | Stage 1, Stage 3J | public-layout fixture and visible helper-map check | Medium |
| Stage 4C hidden enum and header-level placement guard | `code/BlackStringToy/UserVariables.hpp`; `code/BlackStringToy/tests/Stage4CVariablePlacementTest.cpp` | Add repo-owned `hww/Aww` enum names and enforce their final placement where the header is included | Actual repo-owned hidden enum definitions | `UserVariables.hpp` static assertions for the final hidden metric/A slots; placement fixture with real helper map | Stage 4B review | real `hww/Aww` placement guard, AH positional hazard, hidden determinant/trace participation setup | High |
| Stage 4D smoke-only hidden-variable freeze | `BlackStringToyLevel` plus smoke parameter file | Give `hww/Aww` finite scaffold values only when the smoke-only parameter is enabled | Repo-owned enum with hidden variables; inherited scaffold state/RHS slots | Cheap smoke run reaches past the current non-finite hidden-variable failure without silently freezing future runs by default | Stage 4C review and explicit approval | Stage 4C placement guard plus smoke-run NaN regression | High |
| Later helper-to-grid handoff | New repo-owned handoff fixture or minimal wrapper, exact harness TBD | Connect Stage 4A helpers to live component slots for the first time | Repo-owned enum with hidden variables, local mock/grid-adjacent component data | Loud failure if helper inputs bind to wrong live components | Stage 4D review plus later explicit approval | helper input map, enum/layout, no-RHS handoff fixture | High |

Deferred later stages, requiring explicit user approval after the layout and
smoke-only scaffold stages pass:

| Later stage | Candidate repo-owned target | Purpose | Inputs | Outputs | Prior-stage dependency | Required Stage 3J tests | Risk |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Stage 4E cartoon Ricci helper interface/implementation | New repo-owned geometry helper | Define and later implement diagonal/off-diagonal cartoon Ricci blocks | Reduced metric, hidden `gamma_ww`, derivatives, off-diagonal block | Interface contract first; implementation only after approval | Stages 3C-3E, 3G | round-`S^2`, Stage 3C-3E Ricci fixtures, sheared-flat gate | High |
| Stage 4F small-axis helper interface/implementation | New repo-owned regularization helper | Isolate regularized small-`x` combinations and connection limits | Taylor-like local fields, `h_xx-hww`, `h_xz`, `Z^A` | Interface contract first; implementation only after approval | Stage 3I | `hat_Gamma^x` assembled guard, regular/irregular Taylor data | High |
| Stage 4G RHS block wiring | Future repo-owned RHS class or wrapper | Connect CCZ4 source blocks to evolution | Grid variables and derivatives | Time derivatives | Stages 3H-3J | RHS block matrix, flat/sheared-flat, uniform-string, reference comparison | Very high |

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
| Grid-variable wiring | Later handoff fixture before any helper reads live grid data; assert intended enum/component slots and positional assumptions |
| Cartoon Ricci helper | Stage 3C flat/cartoon geometry, Stage 3D constant-`q0`, Stage 3E nonconstant `q`, round-`S^2`, sheared-flat Stage 3G Ricci gate |
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
