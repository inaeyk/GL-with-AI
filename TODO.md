# TODO

Current status and checkpoint authority: `research_plan/stage_checklists.md`.

## Immediate

- [x] Create repository scaffold.
- [x] Complete Windows WSL2 + Docker setup.
- [x] Reproduce the GRChombo `BinaryBH` very-cheap smoke test locally.
- [x] Add reproducible BinaryBH smoke-test scripts.

## Active

- [ ] Ensure future physics-stage PDF notes accompany each physics/design stage.
- [ ] Stage 4AO-C custom spectral research is frozen and superseded only as a
  prerequisite for the active GRChombo production path. The complete
  13-variable frozen-gauge interior/JVP/parity oracle and inner pure-outflow
  endpoint pass; the original outer WKB/boundary-bearing/eigensolver/MOTS/
  threshold objective did not pass. Stage 4AO-D and Checkpoint G remain
  incomplete.
- [x] Inventory custom-solver/GRChombo overlap and gaps; define production ownership, the four-level comparison hierarchy, prioritized adaptation backlog, and agent-capability evidence template. No overall agent score is assigned yet.
- [x] Qualify the core GRChombo/Chombo/compiler tuple. GRChombo is pinned;
  official Chombo `8684f2e000106f1abadb72642e1d15351867f98f` builds the
  four serial DIM2 libraries and passes the real `2/4/4` target plus stock
  compile/smoke gates. This is a project-qualified lock; the exact historical
  Chombo SHA remains inferred.
- [ ] Recover the former container recipe/image digests. Pin PETSc separately
  before `USE_AHFINDER`; neither is part of the qualified core initializer
  dependency.
- [x] Execute comparison batch 1: record available dependency manifest fields, prove the shared thirteen-slot/dimension map, and directly compile GRChombo conformal algebra, contracted-connection/Z, and visible physical-Ricci comparisons. All numerical families pass the fixed tolerance and dimension/sign mutations fail. Chombo/container digests remain unresolved; hidden/cartoon terms remain custom-only. Evidence: `docs/grchombo/custom_solver_grchombo_comparison_batch1_results.md`.
- [x] Execute comparison batch 2: separate raw and encoded-Z Ricci, then directly compare visible chi/metric/K/Theta/A RHS families at identical analytic jets in stock `d=3`. All families and combined rows pass the unchanged tolerance; ten active mutations fail. This is not target-`d=4` evidence. Evidence: `docs/grchombo/custom_solver_grchombo_comparison_batch2_results.md`.
- [x] Execute comparison batch 3: actual custom order-two and GRChombo order-four derivative kernels converge on periodic manufactured profiles; all 15 visible advection rows and the complete `chi`, `h_ij`, and `A_ij` shift RHS families now have independent finite-grid monitors; visible algebraic cleanup passes. Production Chombo periodic ownership/ghost exchange remains unimplemented and untested, hidden-`ww` adaptation remains pending, and batch 4 is complete.
- [x] Complete the thin real `Cell`/`FArrayBox` load/store wrapper around the
  validated 18-slot reduced Vars and GP point initializer. The DIM2 fixture
  proves exact 18-slot one-point round trips, neighbor ownership,
  representative-only `hww/Aww` writes, mutation rejection, and three GP
  storage comparisons without adding `BoxLoop` or physics.
- [x] Complete the GP `BoxLoop` initializer compute class and isolated real
  DIM2 traversal. All 20 requested cells and 18 slots match independently
  recomputed pointwise GP states exactly; 22 surrounding cells remain
  untouched. The coordinate and mutation fixtures lock radial direction 0,
  compact direction 1, cell centering, nonzero origin ownership, and use of
  the storage adapter. Adapter ownership is observed through a shared,
  thread-safe test policy: 20 real adapter calls, zero outside calls, and
  exactly one write claim for each of the 18 slots per call. The otherwise
  numerically exact direct-write bypass records zero adapter calls and fails
  independently. No live application path is changed.
- [x] Pass the GP `BoxLoop` repair acceptance gate. Project code compiles with
  `-std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror`; locked Chombo/GRChombo
  headers are isolated with `-isystem`, and a project-owned unused parameter
  remains a fatal negative compile. Checkpoint completion was held pending on
  this repair and is accepted only with these checks passing.
- [x] Implement and validate the pointwise target-`d=4` hidden/cartoon RHS
  adapter by expanding the reduced `(2+2)` state/jets and directly evaluating
  locked GRChombo. Report `target_shared_hidden_suppressed`,
  `hidden_increment_decomposition`, and `target_full_grchombo`, where the
  increment is defined strictly as full minus suppressed. Direct nonlinear
  comparison is the sole 13-row numerical completion gate. The JVP sweep is
  only a roundoff/cancellation-dominated secondary diagnostic, and true
  Fourier `P_+`/`P_-` sector checks pass. This is not an independently coded
  hidden-family RHS. No BoxLoop or live application path was added.
- [x] Implement the pointwise hidden-aware target-`d=4` determinant and
  `A`-trace cleanup, Hamiltonian plus exactly two visible momentum
  constraints, and the fixed GP lapse-source hook. The repaired Hamiltonian is
  exactly `R + 3 K^2/4 - A_IJ A^IJ`; direct locked
  `CCZ4Geometry::compute_ricci` supplies `R`, and the thin formula layer
  follows `Constraints.impl.hpp`. The full result matches the independent
  analytic path across GP, non-trace-free flat, curved, off-diagonal, hidden,
  mixed, and both genuine Fourier-sector cases.
- [x] Stage 4AO-D-E1: isolate the 18-slot live black-string application;
  invoke GP initialization and the complete target-`d=4` RHS through real
  DIM2 `BoxLoops`; apply the fixed source once to lapse; run hidden-aware
  cleanup only at the post-update hook; expose observational `H,Mx,Mz`; and
  configure direction 1 as periodic with real Chombo ghost exchange.
- [x] Stage 4AO-D-E2: run the first bounded serial level-zero unperturbed GP
  diagnostic and matched-domain convergence. `(32,8),(64,16),(128,32)` share
  `L_x=8,L_z=2`, CFL `0.004`, and `t_final=0.004`; exact GP radial ghosts are
  diagnostic-only, periodic z remains framework-owned with no duplicate
  exchange, and missing/wrong-coordinate/legacy-loop mutations fail.
- [x] Add the minimal provisional radial policy for the first perturbed
  level-zero smoke: GP-subtracted fourth-order inner extrapolation with
  `x_in<r0`, plus componentwise GP-subtracted outer Sommerfeld. The focused
  GP/scalar/one-z fixture passes without radial wrap, duplicate exchange, or
  runtime diagnostics. This is not the deferred WKB/spectral boundary.
- [x] Add and execute the fixture-only adaptive Fourier sign-screen policy.
  The requested `k=0.82,0.86,0.90,0.94` and allowed additions `0.78,0.75`
  all rise initially and turn over near `t=0.3-0.4`. Their negative
  late-window fits are preserved as inconclusive transient evidence; 12
  authoritative evolutions return `AMBIGUOUS`.
- [x] Diagnose the turnover with one control and signed pairs on the exact
  `Lz=8`, `dx=dz=1/8` domain. The ten-run hard stop yields
  `LATE_TIME_LINEAR_INSTABILITY_DETECTED — PHYSICAL IDENTITY UNRESOLVED`
  for `k=pi/4` at `t=8`, while
  `k=pi/2` has `NO_MODE_PLATEAU_WITHIN_TESTED_TIME` at `t=4`. Strong
  background drift and exploding linearized constraints prevent a GL-mode
  identification despite the half-amplitude and even/odd linearity checks.
- [x] Complete the D7 four-control isolation budget without rerunning the D6
  baseline. Half CFL is neutral; fine spacing is strongly but non-convergently
  sensitive; exact-GP ghosts fail on an invalid reduced metric; and the
  available zero-coefficient gauge parameters leave `B^i` evolving. Record
  the bounded result as `MULTIPLE_OR_UNRESOLVED`, and keep the `k` scan
  suspended.
- [x] Complete D8 boundary/gauge isolation. Audit the exact-GP fill as
  radial-only plus corners after Chombo exchange, with all 18 slots
  preserved, zero GP error, and no valid overwrite. Localize its genuine
  abort to a valid inner cell at timestep 397/RK stage 3. Add a fixture-only
  true frozen-gauge RHS policy; all lapse/shift/`B` drift is exactly zero but
  rapid radial growth remains, and the combined control also fails. Record
  `NEITHER_CONTROL_CURES — CORE_RADIAL_EVOLUTION_UNRESOLVED`.
- [x] Complete D9 inner-excision placement isolation. Record that the
  conservative fastest lapse envelope is glancing at the old `x_in=0.5`
  face and incoming at its first valid cell, but do not claim a complete
  characteristic proof. Move only the fixture domain to `x_in=0.375` at
  `dx=1/8,1/12`; both provisional controls lose metric admissibility before
  `t=8`, the fine run fails earlier, and the optional exact-GP control also
  fails. Preserve the three positive ghost coordinates and all valid cells.
  Classify `EXCISION_PLACEMENT_NOT_SUFFICIENT`.
- [x] Complete the D10 production radial-operator trace and stop on the
  concrete dissipation-path defect. The explicit `x,z` derivative path and
  algebraic hidden expansion have no hidden grid access, but direct
  `CCZ4RHS::rhs_equation` evaluation bypasses the upstream
  `add_dissipation` call in `CCZ4RHS::compute`. Project parameters do not own
  `sigma`, the adapter base hard-wires zero, and effective coverage is
  `0/18`. Record `DISSIPATION_PATH_DEFECT_IDENTIFIED`; do not run the tangent
  iteration or repair production during D10.
- [x] Complete D11's explicitly authorized dimension-safe KO repair. The
  project helper reproduces the locked seven-point sign, normalization, and
  spacing while requesting only `x,z` strides and covering all 18 rows in
  the existing RHS cell pass. Required `ko_sigma` is project-owned and
  explicit; `sigma_ref=0.3` is sourced from locked GRChombo examples.
  Algebraic/mutation/direction/zero-sigma gates pass, and the medium/fine
  `t=8` controls avoid invalid metrics and drift `0.1` with improving state
  drift and bounded constraints. Record
  `KO_PATH_RESTORED_AND_BACKGROUND_STABILIZED`; do not yet resume the
  tangent or Fourier scans.
- [x] Complete D12's bounded KO-stabilized signed-mode diagnostic. Run
  exactly five medium and five fine cases for `k=pi/4,pi/2`; keep the
  production equations, boundary, KO helper, and hot path unchanged. Both
  controls retain the D11 stability result and both signed responses are
  linear and above leakage, but the required physical-field slopes are
  incompatible and the high mode continues to turn over. Record
  `NO_CREDIBLE_MODE_PLATEAU`; there are no accepted radial eigenfunction
  samples and no `k_cr r0` inference.
- [x] Complete D13's fixture-only matrix-free tangent extraction at
  `k=pi/4,pi/2`. Apply the real signed one-step RK map from three independent
  seeds per sector and check two finite-difference amplitudes plus raw and
  field-scaled norms. The linear/parity checks pass, but no independent seed
  pair shares a profile and rate; record
  `NONNORMAL_OR_NO_CONVERGED_MODE`. Keep production unchanged and do not call
  any sequence a physical GL mode.
- [x] Complete D14's fixture-only reduced spectrum at `k=pi/4,pi/2`.
  Use existing Chombo-linked `dgeev` on two `576x576` field-scaled
  similarity transforms, validate all columns and the matrix-free residual,
  and record `CONSTRAINT_GAUGE_OR_BOUNDARY_SPECTRUM_DOMINATES`. The resolved
  positive spectral leader is inner-boundary/constraint dominated; the
  second sector is clustered near neutral and constraint heavy. No medium
  candidate passes, so do not construct a fine matrix or identify a GL mode.
  Correct the physical left-vector transform to `D^{-1}w_s`; retain the
  classification but do not attribute D13 to severe leader nonorthogonality.
- [ ] Establish a provisional `k_-<k_+` sign bracket in a separately
  authorized follow-up only after the D14 boundary/constraint-dominated
  spectrum is resolved. The current evidence supplies no accepted mode
  endpoint and supports no conclusion about whether `k_cr r0` is above or
  below `0.75`.
- [ ] Stage 4AO-D-F: qualify sustained unperturbed/perturbed evolution,
  boundary-location systematics, and constraint behavior. Keep AMR/MPI,
  growth-rate claims, horizons, PETSc/AHFinder, and final scoring deferred.
- [x] Audit the `2/4/4` `GRAMRLevel::define` blocker beyond its first trapped
  `RealVect` access. Classify boundary, derivative, tagging,
  interpolation/extraction, coordinate, and stock diagnostic sites without
  changing GRChombo or starting evolution. Evidence:
  `docs/grchombo/grchombo_adaptation_backlog.md`.
- [x] Implement the black-string-only grid-dimension adapter and define-only
  regression gate: bound Chombo grid loops by `CH_SPACEDIM`, preserve all
  target tensor loops at four, reject fake hidden coordinates, prove radial
  direction 0/periodic direction 1, and leave stock DIM3 and locked
  dependencies unchanged. A successful define is not radial-boundary or
  evolution acceptance. The real dimension-safe `AMR::define` fixture passes,
  and the former `RealVect m_center[i=2]` over-index blocker is cleared.
- [ ] Run substantive audits only after: assembled storage plus `BoxLoop`;
  complete hidden/cartoon 13-row equivalence; integrated cleanup/constraints
  plus fixed source; first unperturbed evolution; first perturbed growth-rate
  run.
- [ ] Stage 4AP/4AQ: align actual grid/ghost parity and finite-axis handling with GRChombo-facing conventions for `h_xz`, `h_xx-h_ww`, `W_x`, and `chi_x`.
- [ ] Stage 4AT: remove or replace the Stage 4D smoke-only `hww/Aww` freeze in the physics path and add a loud guard against using both paths together.
- [ ] Future constraint-damping validation: inject linearized constraint violations and verify `Theta` / encoded-`Z^i` damping behavior after the coupled Stage 4AO validation path is ready.
- [ ] Future production wiring: document and enforce cartoon/conformal determinant and trace-free constraints in the GRChombo-facing variable set, especially hidden `hww` participation.
- [ ] Add optional nonconstant profile with `f_zz != 0` so `R_zz` is a nonzero regression target.
- [ ] Factor duplicated symbolic geometry helpers into a shared module.
- [ ] Convert Stage 3C/3D symbolic checks into unit-test fixtures after review.
- [ ] Future initial-data implementation: code black-string fields only after the Stage 4 checkpoint gates and current checklist owners allow production wiring.
- [ ] GRChombo horizon diagnostics: lock PETSc/AHFinder-enabled execution, adapt `AHStringGeometry` and hidden expansion terms, and implement convergent `R_H`, minimum-radius, and area outputs only after the background production path passes. No custom eigensolver adapter is authorized.

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
- [ ] Stage 4AO-C: custom stationary outer-boundary research is deferred. The complete coupled interior linearization/JVP/parity and inner endpoint pass. The current outer characteristic selectors and row layout are diagnostic only. The singular leading kernel has nullity three, but no generalized-chain obstruction or complete branch classification is established; coincident leading vectors with distinct subleading series remain possible. Outer, aggregate boundary, boundary-bearing, and pencil gates are false; all solver, MOTS, threshold, production, and downstream work remains blocked.
- [ ] Stage 4AO-D: live-gauge/full acceptance: physical GL eigenvalue agreement, physical/gauge/constraint mode separation, CCZ4 constraint-subsystem decay, inner-boundary characteristics, seeded time-evolution growth bridge, and full convergence.
- [x] Before Stage 4AO-D-F, adapt the locked boundary-setup loops needed by
  `AMR::define` so grid objects use `CH_SPACEDIM`. The real define-only gate
  passes, and the former `RealVect m_center[i=2]` over-index blocker is
  cleared. Bounded serial level-zero evolution and matched-domain convergence
  are complete in E2. A minimal provisional physical radial policy now passes
  a small perturbative smoke; sustained evolution, boundary systematics, AMR,
  MPI, growth-rate extraction, broader diagnostics, and horizons remain
  incomplete.
- [ ] Stage 4AO deferred-branch hard rules: flat tests alone are insufficient;
  no eigensolver before its prerequisite gates; Pau is not the convention
  authority; and `hat_Gamma^x` alone is not the observable. The former
  production block through 4AO-D is superseded by the active GRChombo
  adaptation sequence.
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

## GRChombo comparison continuation

- [x] Batch 3: actual custom order-two versus GRChombo order-four derivative
  kernels converge at their documented orders on four refinements.
- [x] Batch 3: raw Ricci, encoded Z, lapse Hessian, all 15 advection rows,
  complete `chi`/metric/`A` shift RHS families, and all visible combined rows
  converge to the common continuum result. Raw shift-derivative convergence
  is retained separately as an input-kernel diagnostic; extrapolated
  custom/GR maximum is `4.91998803e-13`.
- [x] Batch 3: direct stock visible `TraceARemoval` and
  `PositiveChiAndAlpha` pass independent component, residual, idempotence, and
  ordering checks.
- [x] Qualify official Chombo
  `8684f2e000106f1abadb72642e1d15351867f98f` as the project dependency:
  required serial DIM2 libraries, the real target `2/4/4` header probe, and
  stock GRChombo compile/smoke checks pass. Historical exact provenance was
  not recovered and remains explicitly inferred.
- [ ] Recover the former container recipe/image digests and separately pin
  PETSc/AHFinder before claiming those runtime paths reproducible.
- [x] Adapt hidden-aware determinant and A-trace cleanup. The target
  determinant uses `hww^2`, the weighted trace uses two hidden copies, and
  the pointwise cleanup is idempotent. No callable stock determinant
  normalization owner exists; the hidden-aware normalization is project
  adaptation.
- [x] Batch 4: compare the GP analytic/setup seam through direct stock
  tensor/RHS/derivative/gauge paths and the independent full custom `d=4`
  oracle. The repaired gate independently decomposes stock and target K/A
  families, invokes the actual 13-row analytic oracle plus `H,Mx,Mz`, and
  validates a lapse-only test adapter with a zero 20-field Jacobian.
  Coordinate `gamma_theta_theta=x^2` remains stored as custom `hww=1`; raw
  lapse remains `-3 lambda`.
- [x] Production-adaptation preflight: add the tracked GRChombo source lock
  and read-only verifier; lock the future 18-slot target-`d=4/2` state with
  hidden multiplicity two and no visible-`y`; define thin GRChombo adaptation
  points, the 13-row oracle seam, and audit checkpoints. No production path
  was implemented.
- [x] First adaptation contract: verify the dependency checkout, implement the
  isolated target 18-slot enum/name/metadata registration, and pass exact
  macro/slot/parity/permutation/multiplicity plus mutation checks. The live
  27-slot smoke scaffold is unchanged.
- [x] Implement the storage-agnostic reduced Vars seam, exact 18-slot GP
  pointwise initializer, and analytic `beta^x/lambda/K/A_IJ` radial metadata.
  Exact mapping, reconstruction, convergence, and mutation fixtures pass.
- [x] Using the project-qualified Chombo lock, wrap the reduced seam in a
  black-string `Cell`/`FArrayBox` storage adapter and validate the GP
  `BoxLoop` compute class.

## Later

- [x] Integrate exact black-string GP initial data through a live Chombo
  `BoxLoop`, with one storage-seam call per valid cell.
- [x] Implement compact-z domain ownership through Chombo periodic
  `ProblemDomain` and real `LevelData::exchange`: low/high global wraps and
  fourth-order first-, second-, and mixed-derivative stencils at both sides
  of the internal two-box seam pass separately, with no radial wrapping.
- [ ] Add GL perturbation controls.
- [x] Add observational `H,Mx,Mz` diagnostics and the fixed GP lapse-source
  hook; no hidden momentum output.
- [ ] Add horizon diagnostics.
- [ ] Add radiation extraction.
