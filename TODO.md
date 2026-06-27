# TODO

Current status and checkpoint authority: `research_plan/stage_checklists.md`.

## Immediate

- [x] Create repository scaffold.
- [x] Complete Windows WSL2 + Docker setup.
- [x] Reproduce the GRChombo `BinaryBH` very-cheap smoke test locally.
- [x] Add reproducible BinaryBH smoke-test scripts.

## Active

- [ ] Ensure future physics-stage PDF notes accompany each physics/design stage.
- [ ] Stage 4AO-C/4AO-D: complete the GL validation gate. The 4AO-C wrapper, boundary contract, GP-shift advection block, tensor shift-stretching block, algebraic metric/chi coupling block, K-output algebraic `A^2/K^2` block, A-output non-curvature algebraic block, Theta-output non-Ricci algebraic block, Theta-output `-K_GP deltaTheta` block, trace-free `delta A` projector contract, Ricci/curvature design preflight, raw hidden physical `delta R_ww[gamma]` validation block, raw visible one-z `delta R_xz[gamma]` validation block, raw visible diagonal `delta R_zz[gamma]` / `delta R_xx[gamma]` validation blocks, raw Ricci trace/trace-free assembly, Theta Ricci scalar insertion, `A_IJ` trace-free Ricci curvature insertion, and Z4 damping/kappa convention-lock derivation pass now exist; next resolve the human `kappa1/kappa2` convention decision, then build remaining frozen-gauge operator blocks, actual-operator JVP/parity checks, linearized MOTS map, convention map, spectral targets, convergence tests, and later the 4AO-D seeded-evolution bridge.
- [ ] Stage 4AP/4AQ: align actual grid/ghost parity and finite-axis handling with GRChombo-facing conventions for `h_xz`, `h_xx-h_ww`, `W_x`, and `chi_x`.
- [ ] Stage 4AT: remove or replace the Stage 4D smoke-only `hww/Aww` freeze in the physics path and add a loud guard against using both paths together.
- [ ] Future constraint-damping validation: inject linearized constraint violations and verify `Theta` / encoded-`Z^i` damping behavior after the coupled Stage 4AO validation path is ready.
- [ ] Future production wiring: document and enforce cartoon/conformal determinant and trace-free constraints in the GRChombo-facing variable set, especially hidden `hww` participation.
- [ ] Add optional nonconstant profile with `f_zz != 0` so `R_zz` is a nonzero regression target.
- [ ] Factor duplicated symbolic geometry helpers into a shared module.
- [ ] Convert Stage 3C/3D symbolic checks into unit-test fixtures after review.
- [ ] Future initial-data implementation: code black-string fields only after the Stage 4 checkpoint gates and current checklist owners allow production wiring.
- [ ] Stage 4AO-C / horizon diagnostics: resolve PETSc/AHFinder reuse, the linearized MOTS map, and any eigensolver adapter needed for the GL gate.

## Resolved or carried forward from old Active items

- [x] Stage 3I / 4AM `tilde_Gamma^x` and `hat_Gamma` convention item is solved by Stage 4AM/4AN. Evidence and current status live in `research_plan/stage_checklists.md`.
- [x] GP branch/sign, `K_ij` convention, and gauge startup are locked by Stage 4AO-A. Evidence and residual targets live in `research_plan/stage_checklists.md` and `docs/derivations/stage4AO_A_uniform_gp_background_residual.md`.
- [x] Stage 4D smoke-only `scaffold_freeze_hidden` review is superseded by the recorded Stage 4AT removal/replacement owner in `research_plan/stage_checklists.md`.
- [x] Stage 3A black-string initial-data design review is complete as a design-stage gate; implementation remains a separate future item above.
- [x] Stage 2A/2B scaffold and scratch-workflow review items are complete or superseded by later Stage 4 work; see `research_plan/stage_checklists.md`.
- [x] The scratch-only minimal CCZ4-target decision is superseded by the repo-owned `BlackStringToy` path and Stage 4AO validation harnesses.

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
- [x] Stage 4E: review the first grid-to-helper handoff diagnostic and distinct-value mapping fixture.
- [x] Stage 4F: review the cartoon Ricci helper interface before any Ricci formulas are implemented.
- [x] Stage 4G: review the first local metric-derivative cartoon Ricci helper and standalone nonzero-oracle fixture.
- [x] Stage 4H: review the Ricci/RHS compatibility decision note.
- [x] Stage 4I: review the typed Ricci bridge contract and `451` trace-oracle fixture before any RHS wiring.
- [x] Stage 4I review follow-up: confirm the opaque cartoon Ricci result and bridge-only RHS-facing access pattern before any RHS wiring.
- [x] Stage 4J: review the local Ricci-to-RHS contract and standalone `451` trace-oracle fixture before any RHS source formulas or evolution wiring.
- [x] Stage 4K: review the local RHS source-block skeleton before any real source formulas, grid reads, or evolution wiring.
- [x] Stage 4L: review the local trace-free Ricci projection block before any additional RHS source formulas, grid reads, or evolution wiring.
- [x] Stage 4M: review the named away-axis policy before adding explicit `1/x` or `1/x^2` RHS source terms.
- [x] Stage 4N: review guarded singular-combination helpers before using them in any RHS/Ricci source block.
- [x] Stage 4O: review the away-axis-only regime semantics and separate `1/x^2` primitive before real singular source terms are added.
- [x] Stage 4P: review that raw cartoon geometry primitives expose only low-risk away-axis values before source-block use.
- [x] Stage 4Q: review the local `hxx - hww = O(x^2)` matching guard before using it near the axis in source blocks.
- [x] Stage 4R: review the regularity-guarded local source sub-block as the required path for `(hxx - hww) / x^2` before any real Ricci/RHS formula uses it.
- [x] Stage 4S: review the checked-by-construction local RHS source-block integration carrying the Stage 4R guarded geometry package before any real source formula consumes it.
- [x] Stage 4T: review the guarded diagnostic consumer before adding any real Ricci/RHS source formula using the checked package.
- [x] Stage 4U: review the source-formula authoring gate before any real source formula uses regularity-sensitive geometry.
- [x] Stage 4V: document that no source formula is allowed until a real sub-expression, coefficient/sign convention, and hard-coded oracle are derived.
- [x] Stage 4W review: confirm the hidden-sphere `R_ww` derivation target and the needed local `h_xz / x` ingredient before any `R_ww` implementation.
- [x] Stage 4W review patch: document the conformal/physical split, varying-`chi` oracle target, and internal Stage 3A sign-convention dependency.
- [x] Stage 4X: review the checked local off-diagonal `h_xz / x` ingredient; keep it scoped as an ingredient, not a parity proof.
- [x] Stage 4Y: review the first guarded conformal `R_ww` singular-gradient sub-block before expanding toward full `tilde R_ww`.
- [x] Stage 4Y review patch: confirm the single-source input package closes the raw determinant / checked singular ingredient consistency hole before expanding toward more `tilde R_ww` sub-blocks.
- [x] Stage 4Z: add checked `W_x/x` ingredient and the reviewed `G^grad_ww` conformal hidden Ricci block.
- [x] Stage 4AA: lock the Hessian block derivation, coefficients, sign convention, reduced Christoffels, and primary oracles before implementation.
- [x] Checkpoint A / Claude Audit A: lock the verified nonsymmetric Hessian oracle `G^Hess_ww=-8558/2883` and full conformal sum `-3576/961` before Stage 4AB.
- [x] Stage 4AB: implement the Hessian block only with the verified nonsymmetric oracle included in the test.
- [x] Stage 4AC: assemble conformal `tilde R_ww[h]` from the reviewed Stage 4Y, 4Z, and 4AB sub-blocks without claiming physical `R_ww[gamma]`.
- [x] Checkpoint B / Claude Audit B cleanup: unify the reduced determinant policy so all conformal `R_ww` blocks reject `D <= 0`.
- [x] Stage 4AD: derive `R^chi_ww` and design its hidden/cartoon singular guard stack before implementation.
- [x] Stage 4AE: implement only the reviewed local `R^chi_ww` correction, including checked `chi_x/x`, hidden multiplicity, and linear-z/nonsymmetric oracles.
- [x] Checkpoint C / Claude Audit C: review the Stage 4AE guard stack, multiplicity, and oracles before Stage 4AF.
- [x] Stage 4AF: pass the internal hard identity gate `tilde R_ww + R^chi_ww == R_ww[gamma]` against a separately constructed physical metric jet and the Stage 4G direct Ricci engine.
- [x] Stage 4AG: add a synthetic paired two-sided off-diagonal parity-validator primitive for `h_xz(-x,z) = -h_xz(x,z)` with optional explicit axis data and validation-only tolerances; this is not actual grid-data parity validation.
- [x] Checkpoint D: review the Stage 4AF identity gate and Stage 4AG synthetic parity gate before allowing local-only Stage 4AH assembly.
- [x] Stage 4AH: assemble local away-axis physical `R_ww[gamma] = tilde R_ww[h] + R^chi_ww` through one single-source package; retain all grid/axis validation limitations.
- [x] Stage 4AI: place local physical `R_ww[gamma]` into a typed hidden Ricci/RHS contribution contract that computes `2 h^ww R_ww` and `chi 2 h^ww R_ww` without accepting a loose raw value.
- [x] Checkpoint E / Claude Audit E: review the Stage 4AI physical-`R_ww` typed boundary, hidden multiplicity, and conformal/physical contraction factors before Stage 4AJ.
- [x] Stage 4AJ: implement local physical `D_wD_w alpha` with checked `alpha_x/x`, physical `gamma=h/chi`, direct physical-form comparisons, and no source minus sign or RHS wiring.
- [x] Stage 4AK: implement the local away-axis hidden `A_ww` curvature/lapse geometric core `-D_wD_w alpha + alpha R_ww[gamma]`, without Z4, trace-free projection, chi prefactor, nonlinear terms, RHS, or evolution.
- [x] Stage 4AL: implement the full local trace-free curvature/lapse block `chi(C_IJ - h_IJ C/4)` for `xx,xz,zz,ww`, while keeping Z4, nonlinear terms, RHS, grid wiring, and evolution absent.
- [x] Checkpoint F / Claude Audit F: review the Stage 4AL physical Hessian correction, Stage 4G-vs-4AH `R_ww` agreement gate, trace-free projection, and nonsymmetric oracle before further source/RHS work.
- [x] Stage 4AM: lock the `hat_Gamma^x` hidden-contraction derivation, GRChombo `vars.Gamma` / `Z_over_chi` convention map, required Stage 4AN oracles, and Gamma RHS term classification without adding code.
- [x] Stage 4AN: implement local `hat_Gamma^x` contract tests using the Stage 4AM oracles and checked `Delta_xw` / `B/x` singular ingredients.
- [x] Stage 4AO-A: background and analytic residual lock for the exact uniform ingoing-GP black string, including `r0`, compact-`z`, background slicing gauge, evolution gauge-driver and initial-gauge startup family, `K_ij` sign, perturbation sector, geometric observable, continuum residual, full `hat_Gamma^x` hidden contraction, and analytic `1/x` cancellations.
  - [x] Add `docs/derivations/stage4AO_A_uniform_gp_background_residual.md` locking the uniform ingoing-GP background, `z~z+L`, `k_n=2 pi n/L`, GRChombo `partial_t gamma_IJ=-2 alpha K_IJ+L_beta gamma_IJ` sign, frozen-GP zero-residual targets, live moving-puncture lapse startup residual, and the background `hat_Gamma^x` hidden contraction.
  - [x] Complete the componentwise `A_IJ` residuals, full `hat_Gamma^x` RHS residual, fixed field-independent GP-holding lapse source `S_alpha(x)=+3 sqrt(r0/x^3)`, horizon-observable lock, positive-inner-radius domain, constraints, and `1/x` cancellation ledger.
- [x] Stage 4AO-B: discrete operator preflight harness: raw unmodified-RHS background residual convergence on the provisional `r0=1`, `x in [0.5,4.0]` domain, with target zero for verified geometric/scalar/constraint components and `-3 sqrt(r0/x^3)` for the unmodified live moving-puncture lapse equation; `S_alpha=+3 sqrt(r0/x^3)` is not used to cancel measured finite-grid residuals; the fixture also checks `delta hww` hidden-contraction isolation for `delta hat_Gamma^x`, hand-derived actual-discrete-RHS Jacobian-vector agreement, and z-coupled periodic-stencil parity-sector block diagonalization with a flipped-parity negative guard.
- [ ] Stage 4AO-C: frozen-gauge spectral gate remains blocked/incomplete. `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md` defines the intended frozen-gauge operator and records missing prerequisites: complete coupled frozen-gauge modified-cartoon RHS linearization, actual-operator parity/JVP checks, radial spectral boundary conditions, shift-invert or equivalent targeted extraction, linearized MOTS/horizon observable map, primary-source `k_c r0` convention map, threshold zero crossing, unstable/stable points, and radial/boundary convergence.
- [ ] Stage 4AO-D: live-gauge/full acceptance: physical GL eigenvalue agreement, physical/gauge/constraint mode separation, CCZ4 constraint-subsystem decay, inner-boundary characteristics, seeded time-evolution growth bridge, and full convergence.
- [ ] Stage 4AO hard rules: flat tests alone are insufficient; no discrete or spectral work before 4AO-A; no eigensolver before 4AO-B; validation harnesses may exercise RHS/spectral/evolution-bridge code; production Stage 4AR/4AS integration remains blocked until 4AO-D; Pau is not the convention authority; `hat_Gamma^x` alone is not the observable.
- [ ] Checkpoint G / Claude Audit G: review Stages 4AM-4AO-D; passes only after 4AO-D.
- [ ] Stage 4AP: validate actual grid/ghost-cell regularity for `h_xz=O(x)`, `h_xx-h_ww=O(x^2)`, `W_x=O(x)`, and `chi_x=O(x)`.
- [ ] Stage 4AQ: implement finite-axis source evaluation and regularized limits with no epsilon replacement or silent clamping.
- [ ] Stage 4AR: integrate controlled local RHS source blocks without live evolution.
- [ ] Checkpoint H / Claude Audit H: review Stages 4AP-4AR before live evolution.
- [ ] Stage 4AS: add live evolution wiring behind an explicit default-off parameter.
- [ ] Stage 4AT: remove or replace the smoke-only hidden freeze in the physics path.
- [ ] Stage 4AU: perform final Stage 4 exit review.
- [ ] Checkpoint I / Claude Audit I: review Stages 4AS-4AU before any Stage 5/Pau diagnostic reproduction.
- [ ] Future review/lint gate: mechanically flag source-formula recomputation of `(hxx - hww) / x^2` outside `RegularityGuardedGeometrySources` / the Stage 4U authoring gate.

## Later

- [ ] Implement black-string initial data.
- [ ] Implement compact-z handling.
- [ ] Add GL perturbation controls.
- [ ] Add constraint/gauge diagnostics.
- [ ] Add horizon diagnostics.
- [ ] Add radiation extraction.
