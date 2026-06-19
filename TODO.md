# TODO

## Immediate

- [x] Create repository scaffold.
- [x] Complete Windows WSL2 + Docker setup.
- [x] Reproduce the GRChombo `BinaryBH` very-cheap smoke test locally.
- [x] Add reproducible BinaryBH smoke-test scripts.

## Active

- [ ] Ensure future physics-stage PDF notes accompany each physics/design stage.
- [ ] Stage 4D review: confirm the smoke-only `scaffold_freeze_hidden` guard is acceptable before committing.
- [ ] Future hidden-sector RHS work must disable or replace the Stage 4D smoke-only `hww/Aww` freeze and add a loud guard against using both paths together.
- [ ] Align Stage 3I parity and axis-filling conventions with GRChombo-facing project conventions before C++ source-term work.
- [ ] Derive and document the Stage 3I `tilde_Gamma^x` sign and full `hat_Gamma^A` contracted-connection convention in the GRChombo-facing cartoon extension.
- [ ] Required future RHS validation: reproduce the linear Gregory-Laflamme threshold/growth spectrum after matching conventions.
- [ ] Required future constraint-damping validation: inject linearized constraint violations and verify `Theta` / encoded-`Z^i` damping behavior.
- [ ] Document how the project enforces cartoon/conformal determinant and trace-free constraints in the GRChombo-facing variable set, especially hidden `hww` participation.
- [ ] Add optional nonconstant profile with `f_zz != 0` so `R_zz` is a nonzero regression target.
- [ ] Factor duplicated symbolic geometry helpers into a shared module.
- [ ] Convert Stage 3C/3D symbolic checks into unit-test fixtures after review.
- [ ] Review Stage 3A black-string initial-data design note before coding.
- [ ] Resolve GP branch/sign, `K_ij` convention, and zero-shift gauge startup before implementation.
- [ ] Review Stage 2B scratch workflow hardening before committing.
- [ ] Review Stage 2A `BlackStringToy` scaffold diff and scratch build output before committing.
- [ ] Decide whether the next scaffold pass should keep inherited BinaryBH behavior untouched or trim it before later black-string initial-data work.
- [ ] Decide how and when to resolve PETSc/AHFinder-enabled execution.
- [ ] Decide whether a scratch-only minimal CCZ4 target is needed before reduced-cartoon evolution work.

## Completed

- [x] Review Stage 1 hazard documentation.
- [x] Decide whether Stage 1 is approved.
- [x] Review Stage 1.5 compile preflight results for starting Stage 2A.
- [x] Decide whether to run the Stage 1.5 compile preflight.
- [x] Complete the scratch-copy `GR_SPACEDIM=4` compile preflight.
- [x] Attempt the Stage 1.5B CCZ4-side scratch-copy compile probe.
- [x] Manually rerun the Stage 2A `BlackStringToy` scratch build and smoke test.
- [x] Harden the Stage 2B scratch build script against root-owned Docker outputs.
- [x] Draft the Stage 3A black-string initial-data design note.
- [x] Add physics-stage PDF note workflow and build the Stage 3A PDF note.
- [x] Compile companion PDFs for all current physics-note LaTeX sources.
- [x] Draft the Stage 3B modified-cartoon source-term derivation and validation roadmap.
- [x] Review Stage 3B modified-cartoon source-term derivation roadmap for the Stage 3 design pass.
- [x] Incorporate Stage 3B review refinements into the modified-cartoon notes.
- [x] Create the Stage 3C symbolic/geometric derivation scaffold.
- [x] Rerun the Stage 3C SymPy derivation script and record passing output.
- [x] Create and run the Stage 3D nontrivial-`gamma_ww` symbolic derivation scaffold.
- [x] Add the Stage 3D constant-q Ricci regression assertions.
- [x] Add nonconstant `q(x,z)` Ricci regression to guard derivative terms.
- [x] Add Stage 3F conformal-cartoon translation derivation and SymPy checks.
- [x] Add Stage 3G off-diagonal reduced-metric derivation and SymPy checks.
- [x] Add Stage 3G sheared-flat off-diagonal Ricci regression gate.
- [x] Add final Stage 3G normalized determinant guard and Stage 3F diagonal-only warning.
- [x] Add Stage 3H CCZ4 RHS block decomposition plan.
- [x] Patch Stage 3H review items: hatted connection variable, validation milestones, and inherited/cartoon term tags.
- [x] Add Stage 3I small-`x` regularity plan and symbolic removable-limit checks.
- [x] Patch Stage 3I review items: cartoon-axis versus physical singularity distinction and assembled `hat_Gamma^x` axis guard.
- [x] Add Stage 3J unit-test fixture design before C++ implementation.
- [x] Patch Stage 3J review items: round-`S^2` curvature fixture, Schwarzschild x `S^1` scope, tolerance policy, and separated GL/constraint-damping milestones.
- [x] Review Stage 3J unit-test fixture design before Stage 3K implementation planning.
- [x] Add Stage 3K minimal C++ implementation plan without editing source code.
- [x] Patch Stage 3K review items: Stage 4A algebra-helper-only scope, deferred Ricci/small-axis helpers, Stage 4B automated layout gate, and helper-green limitations.
- [x] Clarify convention policy: GRChombo-facing compatibility and internal consistency are authoritative; external/Pau code is later validation only.
- [x] Review Stage 3K minimal C++ implementation plan enough to explicitly approve Stage 4A source edits.
- [x] Inspect `code/BlackStringToy` files and add Stage 4A local conformal-cartoon algebra helper plus non-grid fixture.
- [x] Patch Stage 4A review items: dimension-aware `chi` power, tolerance-based floating-point guards, independent `K_ij` oracles, and Stage 4A/4B terminology.
- [x] Add Stage 4B public CCZ4 baseline-layout fixture for the current public layout and visible helper input map without grid reads or enum edits.
- [x] Decide `hww/Aww` enum layout and target build path before adding variables.
- [x] Add Stage 4C real `hww/Aww` enum entries and header-level placement assertions without grid reads or helper handoff.
- [x] Run Stage 4C scratch build-validation follow-up: full GRChombo compile passes with 27 variables; inherited smoke run now exposes uninitialized `hww/Aww`.
- [x] Review Stage 4C hidden-variable enum placement and header-level assertions before Stage 4D planning.
- [x] Stage 4D: add finite smoke-only scaffold support for `hww/Aww` behind the default-off `scaffold_freeze_hidden` parameter.
- [x] Stage 4D: keep the temporary `hww/Aww` freeze narrow; do not claim physical evolution correctness or implement cartoon Ricci/RHS terms.
- [ ] Stage 4E: review the first grid-to-helper handoff diagnostic and distinct-value mapping fixture.
- [ ] Stage 4F: review the cartoon Ricci helper interface before any Ricci formulas are implemented.
- [ ] Stage 4G: review the first local metric-derivative cartoon Ricci helper and standalone nonzero-oracle fixture.
- [ ] Stage 4H: review the Ricci/RHS compatibility decision note.
- [ ] Stage 4I: review the typed Ricci bridge contract and `451` trace-oracle fixture before any RHS wiring.
- [ ] Stage 4I review follow-up: confirm the opaque cartoon Ricci result and bridge-only RHS-facing access pattern before any RHS wiring.
- [ ] Stage 4J: review the local Ricci-to-RHS contract and standalone `451` trace-oracle fixture before any RHS source formulas or evolution wiring.
- [ ] Stage 4K: review the local RHS source-block skeleton before any real source formulas, grid reads, or evolution wiring.
- [ ] Stage 4L: review the local trace-free Ricci projection block before any additional RHS source formulas, grid reads, or evolution wiring.
- [ ] Stage 4M: review the named away-axis policy before adding explicit `1/x` or `1/x^2` RHS source terms.
- [ ] Stage 4N: review guarded singular-combination helpers before using them in any RHS/Ricci source block.
- [ ] Stage 4O: review the away-axis-only regime semantics and separate `1/x^2` primitive before real singular source terms are added.
- [ ] Stage 4P: review that raw cartoon geometry primitives expose only low-risk away-axis values before source-block use.
- [ ] Stage 4Q: review the local `hxx - hww = O(x^2)` matching guard before using it near the axis in source blocks.
- [ ] Stage 4R: review the regularity-guarded local source sub-block as the required path for `(hxx - hww) / x^2` before any real Ricci/RHS formula uses it.
- [ ] Stage 4S: review the checked-by-construction local RHS source-block integration carrying the Stage 4R guarded geometry package before any real source formula consumes it.
- [ ] Stage 4T: review the guarded diagnostic consumer before adding any real Ricci/RHS source formula using the checked package.
- [ ] Stage 4U: review the source-formula authoring gate before any real source formula uses regularity-sensitive geometry.
- [ ] Stage 4V follow-up: derive or extract the first real source-formula sub-expression that consumes the Stage 4U authoring-gate input, including its coefficient/sign convention and a hard-coded oracle, before adding code.
- [ ] Stage 4W review: confirm the hidden-sphere `R_ww` derivation target and the needed local `h_xz / x` ingredient before any `R_ww` implementation.
- [ ] Stage 4W review patch: cross-check the documented `R^chi_ww` conformal Ricci correction and varying-`chi` oracle against standard BSSN/CCZ4 conformal-Ricci formulas before implementation.
- [ ] Stage 4W review patch: pin the `A_IJ` curvature/lapse sign against the published CCZ4/BSSN formulation and the Stage 3A `K_IJ` convention caveat.
- [ ] Stage 4X: review the checked off-diagonal `h_xz / x` ingredient before implementing `R_ww`.
- [ ] Stage 4Y: review the first guarded conformal `R_ww` singular-gradient sub-block before expanding toward full `tilde R_ww`.
- [ ] Stage 4Y review patch: confirm the single-source input package closes the raw determinant / checked singular ingredient consistency hole before expanding toward more `tilde R_ww` sub-blocks.
- [ ] Future validation stage: add a grid-level or two-sided off-diagonal parity validation for `h_xz(-x,z) = -h_xz(x,z)`, or a documented near-axis tolerance policy, before claiming global axis regularity.
- [ ] Future review/lint gate: mechanically flag source-formula recomputation of `(hxx - hww) / x^2` outside `RegularityGuardedGeometrySources` / the Stage 4U authoring gate.

## Later

- [ ] Implement black-string initial data.
- [ ] Implement compact-z handling.
- [ ] Add GL perturbation controls.
- [ ] Add constraint/gauge diagnostics.
- [ ] Add horizon diagnostics.
- [ ] Add radiation extraction.
