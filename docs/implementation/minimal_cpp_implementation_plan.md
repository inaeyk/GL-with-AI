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

## Stage 4B Automated Variable-Layout Gate Before Grid Wiring

Stage 4A helpers operate on supplied local values only. Before any helper is
wired to grid variables, Stage 4B must add an automated layout test or compile-time
assertion layer. Manual review is useful, but it is not sufficient.

The Stage 4B layout gate should:

- verify that each helper input binds to the intended enum/component slot;
- assert the project/GRChombo positional assumptions that remain intentional,
  including `c_hww == c_K - 1` if that remains the intended
  `AHFunctions`/variable-layout convention;
- verify that `c_chi`, `c_hxx`, `c_hxz`, `c_hzz`, `c_hww`, `c_Axx`,
  `c_Axz`, `c_Azz`, `c_Aww`, `c_K`, `c_Theta`, `c_hat_Gamma^x`, and
  `c_hat_Gamma^z` map to their intended components, using the actual enum
  names present in the implementation;
- fail loudly if enum names, component ordering, `hww/K` adjacency, or
  AH/source positional assumptions change.

If exact enum names differ from the names above, implementation must inspect
the repo-owned enum definitions and assert the actual names/indices. The
helper-green state is not enough to permit grid wiring.

## Implementation Stages And Gates

| Stage | Candidate repo-owned target | Purpose | Inputs | Outputs | Prior-stage dependency | Required Stage 3J tests | Risk |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Stage 4A local algebra helper | `code/BlackStringToy/ConformalCartoonAlgebra.hpp` | Determinant, inverse, trace-free projection, `/4`, `K_ij` reconstruction | Local scalar components `h_xx`, `h_xz`, `h_zz`, `hww`, `A_ij`, `K` supplied directly by tests, not read from grid data | Algebraic quantities and projections | Stages 3F-3G | determinant, off-diagonal inverse, full-4D trace-free, `/4`, `K_xz`, diagonal-limit fixtures | Medium |
| Stage 4A local algebra fixture target | `code/BlackStringToy/tests/Stage4AConformalCartoonAlgebraTest.cpp` | Verify helper without evolution or grid variables | Hard-coded exact fixtures from Stages 3F-3G | Pass/fail with roundoff-level checks | Stage 3J | all algebra helper tests, negative denominator guards | Medium |
| Stage 4B grid layout gate | Automated `static_assert` or unit-layout test, exact harness TBD | Prove helper inputs map to intended enum/component slots before grid wiring | Actual repo-owned enum definitions and public assumptions | Loud failure on layout drift or wrong component binding | Stage 1, Stage 3J | enum/layout fixture, `c_hww == c_K - 1` if retained, `hat_Gamma^A` convention | High |

Deferred later stages, requiring explicit user approval after Stage 4A passes:

| Later stage | Candidate repo-owned target | Purpose | Inputs | Outputs | Prior-stage dependency | Required Stage 3J tests | Risk |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Stage 4C cartoon Ricci helper interface/implementation | New repo-owned geometry helper | Define and later implement diagonal/off-diagonal cartoon Ricci blocks | Reduced metric, hidden `gamma_ww`, derivatives, off-diagonal block | Interface contract first; implementation only after approval | Stages 3C-3E, 3G | round-`S^2`, Stage 3C-3E Ricci fixtures, sheared-flat gate | High |
| Stage 4D small-axis helper interface/implementation | New repo-owned regularization helper | Isolate regularized small-`x` combinations and connection limits | Taylor-like local fields, `h_xx-hww`, `h_xz`, `Z^A` | Interface contract first; implementation only after approval | Stage 3I | `hat_Gamma^x` assembled guard, regular/irregular Taylor data | High |
| Stage 4E RHS block wiring | Future repo-owned RHS class or wrapper | Connect CCZ4 source blocks to evolution | Grid variables and derivatives | Time derivatives | Stages 3H-3J | RHS block matrix, flat/sheared-flat, uniform-string, reference comparison | Very high |

Unknown exact class and file names should be resolved by a separate code
inspection pass immediately before implementation. Do not invent a new
abstraction if a local GRChombo pattern already exists.

## Likely Repo-Owned Files To Inspect Before Coding

- `code/BlackStringToy/UserVariables.hpp`
  - Purpose: future enum and variable-name extension.
  - Current state: aliases public CCZ4 variables only.
  - Validation gate: enum layout review, hidden `hww/Aww` placement, AH
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
| Variable layout | Enum layout review; `hww/Aww` placement; `hat_Gamma^A` convention; AH positional hazard check |
| Grid-variable wiring | Automated layout/static-assert gate before any helper reads grid data; assert intended enum/component slots and positional assumptions |
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
| Helper formulas correct but wired to wrong grid component | Enum/component layout, AHFunctions positional assumptions, `hww/K` adjacency assumptions | Stage 3J and Stage 4B layout gate | Keep Stage 4A helpers local-value-only until automated `static_assert` or unit-layout tests exist |

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
