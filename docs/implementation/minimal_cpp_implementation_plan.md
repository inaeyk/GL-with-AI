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
- No result from Slice 1 should be described as "cartoon evolution works" or
  "CCZ4 works."
- Every implementation slice must have a corresponding validation gate from
  Stage 3J or an explicitly documented new gate.
- The current `BlackStringToy` source is still a non-physical inherited
  scaffold. Stage 3K does not change that.
- Even the safe Slice 1 C++ work requires a separate, explicitly
  user-approved stage after Stage 3K is reviewed and committed. No source edits
  should begin automatically from this plan.

## Convention Policy

The implementation convention target is GRChombo compatibility plus internal
consistency. External implementations may be used later for validation, but
they are not the source of truth for variable naming, evolved-variable basis,
trace convention, or cartoon-extension bookkeeping. Where public GRChombo does
not already define the reduced-cartoon extension, the project must document the
cartoon-specific convention explicitly and keep it consistent across helpers,
tests, diagnostics, and notes.

## Recommended First Implementation Slice

Recommended first slice: **repo-owned algebraic conformal-cartoon helper layer
plus local/non-grid fixture tests, without grid-variable wiring and without
connecting it to time evolution**.

This is lower risk than adding RHS terms or changing evolution variables first
because it exercises the load-bearing bookkeeping from Stages 3F-3G while
keeping the public GRChombo RHS untouched. It should use small local fixtures
and avoid claiming any evolved black-string dynamics.

The first implementation slice should include:

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
4. Documentation of the next blocked slices.
   - Variable-enum grid wiring, cartoon Ricci helpers, small-axis helpers, and
     full RHS wiring remain separate follow-up slices requiring explicit user
     approval after Slice 1 passes.

The first slice should not implement:

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

Passing Slice 1 algebra-helper fixtures is not evidence that cartoon evolution
works; it only shows that local algebra formulas were implemented correctly on
supplied local values.

## Automated Variable-Layout Gate Before Grid Wiring

Slice 1 helpers should operate on supplied local values only. Before any helper
is wired to grid variables, add an automated layout test or compile-time
assertion layer. Manual review is useful, but it is not sufficient.

The layout gate should:

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

## Implementation Slices And Gates

| Slice | Candidate repo-owned target | Purpose | Inputs | Outputs | Prior-stage dependency | Required Stage 3J tests | Risk |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Slice 0 layout audit | `code/BlackStringToy/UserVariables.hpp`, public `CCZ4UserVariables.hpp`, AH variable assumptions | Decide enum layout before adding variables | Existing public CCZ4 enum, desired `hww/Aww/hat_Gamma^A` list | Reviewed layout decision, no code change until approved | Stages 1, 3F, 3G, 3H | `hww` determinant/trace participation, `hat_Gamma^A` convention, AH positional hazard review | High |
| Slice 1 local algebra helper | New repo-owned helper under `code/BlackStringToy/` or a small library namespace, exact file name TBD | Determinant, inverse, trace-free projection, `/4`, `K_ij` reconstruction | Local scalar components `h_xx`, `h_xz`, `h_zz`, `hww`, `A_ij`, `K` supplied directly by tests, not read from grid data | Algebraic quantities and projections | Stages 3F-3G | determinant, off-diagonal inverse, full-4D trace-free, `/4`, `K_xz`, diagonal-limit fixtures | Medium |
| Slice 1 local algebra fixture target | New repo-owned test or smoke fixture, exact harness TBD after GRChombo build inspection | Verify helper without evolution or grid variables | Hard-coded exact fixtures from Stages 3F-3G | Pass/fail with roundoff-level checks | Stage 3J | all algebra helper tests, negative denominator guards | Medium |
| Post-Slice-1 grid layout gate | Automated `static_assert` or unit-layout test, exact harness TBD | Prove helper inputs map to intended enum/component slots before grid wiring | Actual repo-owned enum definitions and public assumptions | Loud failure on layout drift or wrong component binding | Stage 1, Stage 3J | enum/layout fixture, `c_hww == c_K - 1` if retained, `hat_Gamma^A` convention | High |

Deferred later slices, requiring explicit user approval after Slice 1 passes:

| Later slice | Candidate repo-owned target | Purpose | Inputs | Outputs | Prior-stage dependency | Required Stage 3J tests | Risk |
| --- | --- | --- | --- | --- | --- | --- | --- |
| Cartoon Ricci helper interface/implementation | New repo-owned geometry helper | Define and later implement diagonal/off-diagonal cartoon Ricci blocks | Reduced metric, hidden `gamma_ww`, derivatives, off-diagonal block | Interface contract first; implementation only after approval | Stages 3C-3E, 3G | round-`S^2`, Stage 3C-3E Ricci fixtures, sheared-flat gate | High |
| Small-axis helper interface/implementation | New repo-owned regularization helper | Isolate regularized small-`x` combinations and connection limits | Taylor-like local fields, `h_xx-hww`, `h_xz`, `Z^A` | Interface contract first; implementation only after approval | Stage 3I | `hat_Gamma^x` assembled guard, regular/irregular Taylor data | High |
| RHS block wiring | Future repo-owned RHS class or wrapper | Connect CCZ4 source blocks to evolution | Grid variables and derivatives | Time derivatives | Stages 3H-3J | RHS block matrix, flat/sheared-flat, uniform-string, reference comparison | Very high |

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

| Implementation slice | Required tests before or alongside implementation |
| --- | --- |
| Determinant helper | Stage 3F/3G determinant and normalized `det h_4D = 1` fixtures; hidden `hww` participation check |
| Reduced inverse helper | Stage 3G block-inverse identity, including nonzero `h_xz` and diagonal limit |
| Trace-free projection | Full 4D trace-free projection with hidden multiplicity; `/4` denominator negative tests |
| `K_ij` reconstruction | Stage 3F/3G `K_xx`, `K_xz`, `K_zz`, `K_ww` reconstruction fixtures |
| Variable layout | Enum layout review; `hww/Aww` placement; `hat_Gamma^A` convention; AH positional hazard check |
| Grid-variable wiring | Automated layout/static-assert gate before any helper reads grid data; assert intended enum/component slots and positional assumptions |
| Cartoon Ricci helper | Stage 3C flat/cartoon geometry, Stage 3D constant-`q0`, Stage 3E nonconstant `q`, round-`S^2`, sheared-flat Stage 3G Ricci gate |
| Small-axis helper | Stage 3I regular and irregular Taylor fixtures; assembled `tilde_Gamma^x` / `hat_Gamma^x` limit guard |
| Constraint damping | Not a first slice; requires Stage 3H/3J linearized constraint-violation injection milestone |
| Gauge/Gamma driver | Not a first slice; requires ownership and convention confirmation |

## Deferred Work

Defer all of the following beyond the first implementation slice:

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
| Modifying `external/GRChombo` | Any implementation slice | Stage 2 workflow | Use scratch copy under `runs/` and repo-owned code only |
| Trusting compilation alone | Any slice | All Stage gates | Require fixture tests and documented validation before claims |
| Enum layout breaks AH/source assumptions | `UserVariables.hpp`, AH access, diagnostics | Stage 1 and Stage 3J | Review enum order and add layout tests before implementation |
| Helper formulas correct but wired to wrong grid component | Enum/component layout, AHFunctions positional assumptions, `hww/K` adjacency assumptions | Stage 3J and Stage 3K layout gate | Keep Slice 1 helpers local-value-only until automated `static_assert` or unit-layout tests exist |

## Acceptance Criteria

Stage 3K is complete when:

- this minimal C++ implementation plan exists;
- the first implementation slice is explicitly chosen;
- Slice 1 is limited to local algebra-helper functions and local/non-grid
  fixtures;
- automated enum/layout gating is required before any grid-variable wiring;
- all deferred blocks are listed;
- implementation targets are mapped without editing code;
- every proposed implementation slice links to Stage 3J tests;
- protected paths are untouched;
- no C++ implementation has started.
