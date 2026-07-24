# Project Outline

Long-term goal: compute gravitational-wave/radiation profiles during the 5D Gregory-Laflamme transition using a reproducible GRChombo-based workflow.

For stage-by-stage gates and current checkpoint status, see
[stage_checklists.md](stage_checklists.md). That checklist is the current
status authority when older summaries disagree.
Physics and physics-design stages also produce polished review notes under
`docs/physics_notes/` when they contain substantive physics reasoning.

## Stage 0: Infrastructure And Reproducibility

- Goal: establish a stable local development and smoke-test environment.
- Main deliverables: repo scaffold, Git ignore policy, smoke-test scripts, metadata logs, archived PDF snapshots, and local Docker validation.
- Current status: essentially complete. Infrastructure, repo scaffold, local WSL/Docker smoke test, logs, and smoke-test scripts are in place.

## Stage 1: Public GRChombo Capability/Source Map

- Goal: map existing public GRChombo examples, formulation code, AMR patterns, and diagnostics relevant to a 5D black-string implementation.
- Main deliverables: capability map, source-entry notes, reusable implementation checklist.
- Current status: complete and user-approved. The public GRChombo capability/source mapping and hazard documentation have been reviewed and approved by the user.
- Note: this public-source exercise is an AI-agent capability benchmark. Pau's original code is available through collaboration but intentionally not used yet. The intended evolution keeps SO(3) symmetry and is effectively 2+1D, with the S^2 reconstructed by symmetry.
- Later collaboration-code phase: compare against or build on Pau's actual code for radiation extraction after the public-source benchmark stage is complete.

## Stage 1.5: Target-Dimension Compile Preflight

- Goal: check whether the intended compile configuration `CH_SPACEDIM=2`, `GR_SPACEDIM=4` is technically viable before physics implementation. This represents a 2+1D computational grid for 4+1D physics, with SO(3) symmetry of the transverse `S^2` absorbed into modified-cartoon source terms.
- Main deliverables: documented baseline `DIM=2` build/run result, documented `GR_SPACEDIM=4` compile preflight result or blocker, and updated stage gate status.
- Current status: complete and user-reviewed for the purpose of starting Stage 2A. The baseline `DIM=2` `ApparentHorizonFinderTest2D` Docker build/run path has been reproduced, and an AH-side scratch-copy `GR_SPACEDIM=4` compile preflight succeeded with `USE_PETSC=FALSE`. A CCZ4-side `CCZ4Test` scratch-copy probe failed on 3D test-harness assumptions before establishing a clean `GR_SPACEDIM=4` CCZ4 compile path. PETSc/AHFinder-enabled execution remains unresolved and can be deferred unless horizon-solver integration becomes necessary earlier.
- Item-level gates: see [stage_checklists.md](stage_checklists.md).

## Stage 2: Editable Build And BlackStringToy Skeleton

- Goal: create an editable project example that can be built and tested without modifying upstream GRChombo source unexpectedly.
- Main deliverables: build notes, minimal `BlackStringToy` skeleton, smoke-test parameter files, run manifest template.
- Current status: complete for the scaffold/workflow baseline. Stage 2A produced an editable `BlackStringToy` scaffold derived from the public `Examples/BinaryBH` smoke-test path, and the scratch build plus one-step smoke run succeeded under `runs/stage2_blackstringtoy/`. This scaffold is only for build/run workflow validation and is not physical black-string evolution.

## Stage 3: 5D Black-String Initial Data Plus Modified-Cartoon Geometry Derivation/Validation

- Goal: design the baseline uniform black-string initial data, GL-triggering perturbation, and reduced modified-cartoon geometry/source-term bookkeeping before implementation.
- Main deliverables: initial-data design notes, modified-cartoon source-term and geometry derivation notes, symbolic validation scripts, polished physics notes, and validation gates for later implementation.
- Current status: complete. Stage 3A documents the target uniform-string initial data, GL perturbation, variable map, turduckening plan, and unresolved implementation decisions. Stage 3B documents the modified-cartoon hidden-direction source-term derivation and validation roadmap. Stages 3C-3E add symbolic geometry and Ricci gates for cartoon reductions, hidden-sector multiplicity, and nonconstant `gamma_ww=q(x,z)` derivative terms. Stage 3F translates the diagonal physical cartoon variables into conformal `chi`, `h_AB`, and `hww` bookkeeping. Stage 3G extends the conformal-cartoon bookkeeping to an off-diagonal reduced metric with `h_xz` / `gamma_xz` and adds sheared-flat off-diagonal Ricci gates. Stage 3H decomposes the future CCZ4 RHS into separately checkable blocks. Stage 3I documents small-`x` parity, axis matching, the distinction between cartoon-axis regularity and the co-located physical singularity, removable-singularity limits, and the assembled `tilde_Gamma^x` / `hat_Gamma^x` connection guard. Stage 3J designs the unit-test fixture layer. Stage 3K defines Stage 4A as the minimal first C++ implementation stage, with GRChombo-facing convention authority documented and full RHS, gauge, damping, turduckening, AH, radiation, production runs, and external/reference comparison deferred.

## Stage 4: 4+1 CCZ4 / SO(3) Modified Cartoon Implementation

- Goal: evolve the 5D black-string system with the needed symmetry reduction and gauge choices.
- Main deliverables: 4+1 CCZ4/cartoon implementation notes, gauge/damping configuration, AMR strategy, regression smoke tests.
- Current status: in progress through narrow, reviewed setup stages. Stage 4A is complete and committed: it adds local-value-only conformal-cartoon algebra helpers and standalone non-grid tests. Stage 4B is complete and committed: it adds a public GRChombo CCZ4 baseline-layout check. Stage 4C is complete and committed: it adds real repo-owned `hww/Aww` variables, header-level placement assertions, and a non-grid placement fixture. The full scratch Docker/GRChombo `BlackStringToy` scaffold now compiles with the 27-variable layout. Stage 4D adds only smoke-only finite scaffold support for `hww/Aww`: the default-off `scaffold_freeze_hidden` parameter is enabled by the cheap smoke file to freeze `hww = 1.0`, `Aww = 0.0`, and inert hidden RHS slots. Stage 4E is the first safe grid-to-helper handoff diagnostic: a default-off `scaffold_check_hidden_handoff` path reads the actual scaffold grid slots into the Stage 4A local algebra helper and checks finite determinant/inverse/trace/`K_ij` outputs without using those outputs for evolution; a standalone distinct-value mapping fixture catches swapped helper input slots that could remain finite in the symmetric smoke data. Stage 4F added a local-value metric-derivative cartoon Ricci helper interface and compile/type fixture. Stage 4G implements the first local away-axis metric-derivative cartoon Ricci helper with flat, constant-`q`, nonconstant-`q`, sheared-flat, and `x = 0` rejection checks. Stage 4H documents that the inherited GRChombo CCZ4 RHS uses a Gamma-based `CCZ4Geometry::compute_ricci_Z` path. Stage 4I adds a typed bridge contract with a `451` full-4D trace oracle so future RHS-facing code has a named convention boundary; after review, raw cartoon Ricci component storage is private and bridge exposure is the normal crossing point. Stage 4J defines the first local Ricci-to-RHS contract using only bridge-approved Ricci data and a standalone `451` trace-oracle fixture. Stage 4K adds a local RHS source-block skeleton that consumes the Stage 4J contract and exposes named inert output fields without implementing source formulas. Stage 4L implements only the local trace-free Ricci projection block from bridge-approved Ricci data, including the full 4D trace, hidden `ww` multiplicity, and `/4` projection denominator; it is still not wired into evolution. Stage 4M adds a named away-axis policy for finite `x > 0` checks and guarded `1/x` / `1/x^2` helpers, while still deferring true Stage 3I small-axis regularization. Stage 4N adds guarded away-axis singular-combination helpers for future `d_x f / x` and `(f - g) / x^2` factors without adding physical RHS terms. Stage 4O locks the current axis semantics as away-axis-only, with no clamp, epsilon replacement, or regularized-axis mode, and keeps `1/x^2` as a separately guarded primitive so future clamped/regularized behavior must define it independently from `1/x`. Stage 4P now exposes only the low-risk `(d_x hww) / x` away-axis primitive as a public raw output; the risky `(hxx - hww) / x^2` ingredient is source-facing only through the Stage 4R guarded path. Stage 4Q adds a local pointwise guard that catches clear violations of that matching condition before future near-axis source-block use, but still does not prove analytic regularity or implement finite axis limits. Stage 4R adds the first regularity-guarded local source-style sub-block: it calls the Stage 4Q guard before packaging the Stage 4P ingredient and the guarded metric-difference value. Stage 4S connects that Stage 4R package into the local RHS source-block skeleton so guarded geometry ingredients can be carried through the source-block layer without exposing raw Stage 4P metric-difference output; after review, the guarded package and carry output are checked-by-construction rather than open aggregates. Stage 4T adds the first guarded consumer as a diagnostic/probe: it accepts only the checked package type and does not implement Ricci, RHS physics, grid reads, or evolution wiring. Stage 4U adds the authoring gate requiring future formulas that need `(hxx - hww) / x^2` to consume the checked package or gate input, not raw `h_xx`, `h_ww`, and `x`; after review, the remaining generic-helper or hand-written bypass is documented as convention-only until a future review/lint gate exists. Stage 4V deliberately adds no source formula because no tiny authoring-gate consumer is yet derivation-locked to an existing validated Ricci/RHS expression. Stage 4W locks the hidden-sphere CCZ4 contribution map and now distinguishes conformal `tilde{R}_ww[h]`, conformal-factor `R^chi_ww`, and physical `R_ww[gamma]`; it also records a varying-`chi` oracle, the `A_IJ` curvature/lapse sign lock, and the first-principles parity reason for `h_xz = O(x)`. Stage 4X adds only the local checked `h_xz / x` ingredient for future away-axis formulas. Stage 4Y adds the first guarded formula sub-block for the conformal hidden Ricci target, computing only `G_sing = (h_zz / D) Delta_xw - q_xz^2 / D` from the Stage 4U checked `Delta_xw` path and Stage 4X checked `q_xz` package. After review, Stage 4Y single-sources those checked singular ingredients and raw determinant data from one local metric point through a non-forgeable input package, closing the raw/checked mismatch advisory without changing the formula. The integrated future roadmap now lives in `docs/implementation/stage4_hidden_sphere_Rww_plan.md`, with concrete ownership for `W_x/x`, Hessian, `R^chi_ww`, split-vs-direct physical Ricci identity, true `h_xz` parity validation, `hat_Gamma^x`, smoke-freeze removal, and Stage 4 exit review. Stage 4Y still does not implement full `tilde R_ww`, `R^chi_ww`, physical `R_ww[gamma]`, full Ricci, CCZ4 RHS, grid reads, finite-axis regularization, global parity validation, or evolution wiring. Stages 4D-4Y must not be treated as physical evolution correctness or full cartoon Ricci/RHS integration.
- Stage 4Z update: Stage 4Z adds the checked local `W_x / x` ingredient and implements only the first-derivative conformal gradient sub-block `G_grad = -(C/D) p_W + (q_xz W_z)/D - (C W_x^2 - 2 B W_x W_z + A W_z^2)/(4 W D)` with single-sourced local inputs. It still does not implement the Hessian block, full `tilde R_ww`, `R^chi_ww`, physical `R_ww[gamma]`, full Ricci, CCZ4 RHS, grid reads, finite-axis regularization, global parity validation, or evolution wiring.
- Stage 4AA update: Stage 4AA locks the conformal Hessian sub-block derivation in documentation only. It records `G^Hess_ww`, the `rho` derivatives, reduced-base Christoffels, flat and constant-cone Hessian oracles `0`, the nonconstant `W=(1+x)^2` Hessian oracle `-4`, and the combined conformal fixture value `tilde R_ww=-12`. Claude Audit A verifies the distinct nonsymmetric derivative sample with `G^Hess_ww=-8558/2883` and full conformal sum `-3576/961`, matching the independent Stage 4G conformal Ricci engine to machine precision. Stage 4AB may now implement the Hessian block, but only with this nonsymmetric oracle included in the test.
- Stage 4AB update: Stage 4AB implements only the local away-axis conformal Hessian sub-block `G_Hess` in `CartoonConformalRwwHessianBlock.hpp`, with a single-source non-forgeable input package and a standalone fixture covering the simple oracles, the verified nonsymmetric oracle, rejection cases, and a test-only conformal sum. It still does not assemble full `tilde R_ww`, implement `R^chi_ww`, physical `R_ww[gamma]`, RHS, grid reads, or evolution wiring; Stage 4AC owns the separate conformal assembly step.
- Stage 4AC update: Stage 4AC assembles only local away-axis conformal `tilde R_ww[h] = G_sing + G_grad + G_Hess` in `CartoonConformalRww.hpp`. Its single-source input package internally mints the Stage 4Y, 4Z, and 4AB sub-block inputs from one local metric/derivative point. The fixture covers flat `0`, constant-cone `-3/4`, nonconstant `W=(1+x)^2` `-12`, and verified nonsymmetric `-3576/961` oracles, with direct Stage 4G `chi=1` comparisons for the nonconstant and nonsymmetric cases. It still does not implement `R^chi_ww`, physical `R_ww[gamma]`, full Ricci, RHS, grid reads, or evolution wiring.
- Checkpoint B update: Claude Audit B found a determinant-policy mismatch before Stage 4AD. Stage 4Y now rejects finite negative `D=AC-B^2`, matching the Stage 4Z, Stage 4AB, and Stage 4AC finite-positive reduced determinant policy. The formulas are unchanged.
- Stage 4AD update: Stage 4AD derives the conformal-factor companion `R^chi_ww` and designs the guard stack for Stage 4AE. It locks `D_wD_w chi`, the full conformal Laplacian with hidden multiplicity `(2/W)D_wD_w chi`, the reduced scalar Hessian pieces, and the conformal gradient norm. Stage 4AE must add a checked local `chi_x/x` ingredient, reuse checked `q_xz` and checked `p_W`, and keep the same away-axis-only, positive-`chi`, positive-`W`, positive-determinant policy. Stage 4AD is documentation only and does not implement `R^chi_ww`, physical `R_ww[gamma]`, the identity gate, RHS, grid reads, or evolution.
- Stage 4AE update: Stage 4AE implements only local away-axis `R^chi_ww` in `CartoonConformalFactorRww.hpp`. A single-source non-forgeable input package mints checked `q_xz=B/x`, `p_W=W_x/x`, and `p_chi=chi_x/x`, validates positive `x`, `chi`, `W`, and reduced determinant, and retains hidden multiplicity `(2/W)D_wD_w chi`. The fixture locks constant `chi` zero, linear-`x` `11/144`, linear-`z` `-1/64`, and nonsymmetric `63341/48050`, with one test-only Stage 4G physical-minus-conformal comparison. Physical `R_ww[gamma]`, the general split-vs-direct gate, RHS, grid reads, and evolution remain unimplemented. Checkpoint C / Claude Audit C is required before Stage 4AF.
- Stage 4AF update: the internal local gate now passes `tilde R_ww[h] + R^chi_ww = R_ww[gamma]` against Stage 4G. The direct side explicitly constructs the physical `gamma=h/chi` value and first/second derivative jet, then invokes Stage 4G with `chi=1`. Coverage includes constant `chi`, flat linear-`x` and linear-`z` `chi`, and three nonsymmetric varying-`chi` jets with nonzero `W` second derivatives. This does not authorize physical assembly or evolution use; Stage 4AG is implemented below, Checkpoint D remains pending, and Stage 4AH owns production physical `R_ww[gamma]` assembly.
- Stage 4AG update: a local synthetic paired-stencil validator now checks odd `h_xz` parity, optional explicit axis zero, one-to-one opposite-radius partners, and even `h_xz/x` without requiring a constant quotient. It is validation-only and does not implement finite-axis regularization, actual grid-data parity validation, or physical Ricci assembly. Stage 4AP owns real grid/ghost-cell regularity validation. Checkpoint D is recorded below as clearing only local Stage 4AH work.
- Stage 4AH update: after the narrow Checkpoint D review, a local single-source factory now mints Stage 4AC and Stage 4AE inputs from one metric/conformal-factor jet and exposes conformal, correction, and physical `R_ww` values. This remains away-axis and test-only; actual-grid parity/matching, finite-axis evaluation, RHS, grid reads, and evolution remain unimplemented.
- Stage 4AI update: `CartoonPhysicalRwwRhsContract.hpp` now accepts only the Stage 4AH physical result and computes the hidden contributions `2 h^ww R_ww[gamma]` and `chi 2 h^ww R_ww[gamma]`. Stage 4AH carries same-point `chi` and `h^ww=1/h_ww`, so loose or mixed-point Ricci/metric values cannot cross this boundary. This remains a local contract only; full Ricci, live RHS, grid validation, finite-axis support, and evolution are unimplemented.
- Stage 4AJ update: `CartoonPhysicalHiddenLapseHessian.hpp` now implements local physical `D_wD_w alpha` with `gamma=h/chi` and checked `p_alpha=alpha_x/x`, alongside checked `q`, `p_W`, and `p_chi`. The fixture locks simple, varying-`chi`, and nonsymmetric oracles and compares against the compact physical formula. The source sign and full `A_ww` block remain Stage 4AK.
- Stage 4AK update: `CartoonAwwCurvatureLapseCore.hpp` now composes the reviewed Stage 4AH physical `R_ww[gamma]` and Stage 4AJ physical `D_wD_w alpha` packages from one local jet and exposes `-D_wD_w alpha`, `alpha R_ww`, and their sum. This is only the local geometric core before the Z4 term, trace-free projection, outer `chi` factor, nonlinear A/K terms, RHS, grid reads, finite-axis support, and evolution.
- Stage 4AL update: `CartoonTraceFreeCurvatureLapseBlock.hpp` now computes local `source_IJ=chi(C_IJ-h_IJ C/4)` for `xx,xz,zz,ww` from one h/chi/alpha jet. Visible Ricci crosses Stage 4G/4I, hidden `ww` crosses Stage 4AH/4AK, Stage 4G and Stage 4AH `R_ww` must agree, and the visible lapse Hessian uses the physical `gamma=h/chi` correction. Checkpoint F / Claude Audit F is complete for the local trace-free curvature/lapse block only.
- Historical Checkpoint F roadmap update: remaining Stage 4 ownership was split
  into `hat_Gamma^x` derivation/convention mapping in Stage 4AM, local
  `hat_Gamma^x` implementation/contracts in Stage 4AN, and the front-loaded GL
  validation sequence in Stages 4AO-A through 4AO-D. Its custom-branch
  validation requirements and incomplete Checkpoint G remain recorded. Its
  former requirement that 4AO-D precede production integration is superseded
  by the post-qualification GRChombo production pivot below.
- Stage 4AM update: `docs/derivations/stage4AM_hatGammaX_derivation.md` now locks the GRChombo-facing `hat_Gamma^x` convention. GRChombo evolves `vars.Gamma[i]` as the hatted connection and reconstructs `Z_over_chi[i]=0.5*(vars.Gamma[i]-chris.contracted[i])`; project shorthand `hat_Gamma^i=tilde_Gamma^i+2Z^i` therefore uses the encoded `Z_over_chi` object at this boundary. The note derives the hidden contraction with `2 W^{-1} Gamma^x_ww`, records the determinant-reduced cross-check only under `D W^2=1`, locks Stage 4AN oracles `0`, `-3/4`, `-1`, and `-35/961`, and maps the GRChombo Gamma RHS terms. Stage 4AO GL validation remains future work.
- Stage 4AN update: `CartoonHatGammaX.hpp` now implements the local away-axis `hat_Gamma^x` contract from the Stage 4AM derivation. It single-sources one local point, builds `(A-W)/x` from checked `Delta_xw`, obtains `B/x` from the checked Stage 4X package, computes the reduced Christoffels internally, and exposes base, hidden, tilde, `Z_over_chi`, and hatted values. The fixture locks the Stage 4AM oracles and the `Z_over_chi=0.5(hat-tilde)` convention. Gamma RHS, Stage 4AO GL validation, grid regularity, finite-axis handling, and evolution remain future work.
- Historical Stage 4AO roadmap update: the custom GL gate was front-loaded
  into 4AO-A through 4AO-D, with the full outer spectral, threshold,
  live-gauge, and seeded-evolution objectives retained. The interior-oracle
  portion is complete, the original full spectral objective is not, and
  4AO-D/Checkpoint G remain incomplete. The former production prerequisite is
  superseded by the GRChombo adaptation decision; the custom requirements
  remain applicable if that deferred research branch is reopened.
- Stage 4AO-A update: `docs/derivations/stage4AO_A_uniform_gp_background_residual.md` now locks the uniform ingoing-GP background `ds^2=-dt^2+(dx+sqrt(r0/x)dt)^2+x^2 dOmega_2^2+dz^2`, `r0` as the Schwarzschild areal horizon radius, `z~z+L`, `k_n=2 pi n/L`, `alpha=1`, `beta^x=sqrt(r0/x)`, flat conformal metric with `chi=1`, and the GRChombo sign `partial_t gamma_IJ=-2 alpha K_IJ+L_beta gamma_IJ`. It records the full continuum residual ledger for `chi`, `h_IJ`, `K`, `A_IJ`, `Theta`, `hat_Gamma^x`, `hat_Gamma^z`, live gauge variables, and constraints. The full `hat_Gamma^x` RHS cancellation includes the hidden vector-Laplacian multiplicity. The radial domain is locked to `x in [x_in,x_out]` with `x_in>0`, excluding the physical GP singularity at `x=0`. The unmodified GP state is not a full live moving-puncture gauge equilibrium, so Stage 4AO-D uses the same GRChombo moving-puncture family plus the fixed, field-independent validation source `S_alpha(x)=+3 sqrt(r0/x^3)`, computed from the locked analytic GP background and not from evolved fields. Since the source is field-independent, it cancels only the zeroth-order lapse drift and does not change the linearized GRChombo perturbation operator; it is validation-harness-only and not production Stage 4AR/4AS wiring.
- Stage 4AO-A horizon-observable clarification: the apparent horizon is not found from the lapse or an `alpha=0` condition. Diagnostic output solves the outermost `S2 x S1` MOTS connected to `x=r0`, records `x=h(t,z)`, and measures `R_H=h(t,z)sqrt(h_ww/chi)`; spectral work uses the linearized MOTS response, and acceptance compares the horizon-harmonic growth rate rather than the lapse profile.
- Stage 4AO-B raw-residual guard: the background residual convergence preflight must use the unmodified discrete RHS. The target is zero for verified geometric/scalar/constraint components and `-3 sqrt(r0/x^3)` for the unmodified live moving-puncture lapse equation. The fixed source `S_alpha=+3 sqrt(r0/x^3)` is only a 4AO-D validation-harness source and must not cancel measured finite-grid residuals before the raw convergence test.
- Stage 4AO-B update: `Stage4AOGPDiscretePreflight.hpp` and
  `Stage4AOBDiscreteOperatorPreflightTest.cpp` implement the local discrete
  preflight harness on the provisional `r0=1`, `x in [0.5,4.0]` domain. The
  fixture demonstrates raw unmodified-RHS residual convergence, isolates the
  `delta hww` hidden-contraction contribution to `delta hat_Gamma^x`, compares
  a hand-derived actual-discrete-RHS Jacobian-vector product against finite
  differences with an epsilon plateau, and checks z-coupled periodic-stencil
  parity leakage with a flipped-parity negative guard.
  This is not an eigensolver, threshold search, production RHS integration, or
  live evolution wiring; Stage 4AO-C remains next.
- Stage 4AO-C status: `docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md`
  starts the frozen-gauge spectral gate by defining the intended operator and
  documenting blockers. `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`
  now adds the validation wrapper, boundary-condition contract, matrix-free
  GP-shift advection block `beta_GP^x d_x(delta u)` for all 13 frozen-gauge
  perturbation variables, tensor shift-stretching for the `h_IJ` and `A_IJ`
  slots, and the algebraic metric/chi couplings
  `delta h_IJ <- -2 delta A_IJ`, `delta chi <- +(1/2) delta K`, plus the
  selected-CCZ4 K-output `3 lambda delta K - 3 lambda delta Theta` and
  physical-`delta R` insertions, plus the main-path convention lock
  `kappa1=0.1`, `kappa2=0`, `kappa3=1`, `covariantZ4=true` and simple damping
  insertions `output[K] += -0.4 deltaTheta`,
  `output[Theta] += -0.25 deltaTheta`, plus the A-output non-curvature algebraic
  linearization of `(K - 2Theta)A_IJ - 2h^KL A_IK A_LJ`, plus the
  Theta-output non-Ricci algebraic linearization of
  `0.5*(((d-1)/d)K^2 - A_IJ A^IJ)`, plus the Theta-output
  `-K_GP deltaTheta` linearization from GRChombo's `-Theta K` term, plus the
  trace-free `delta A` projector contract for the assembled operator. The
  Ricci/curvature design preflight now records the GRChombo Ricci convention,
  frozen-gauge lapse-Hessian simplification, oracle plan, and first
  recommended `delta R_ww` target. Standalone raw lower/lower physical Ricci
  helpers now validate hidden `delta R_ww[gamma]` and visible
  `delta R_xz[gamma]`, `delta R_zz[gamma]`, and `delta R_xx[gamma]` against
  Stage 4G finite-difference oracles. Raw Ricci trace / trace-free assembly
  now consumes those raw component result types and computes
  `delta R = delta R_xx + delta R_zz + 2 delta R_ww` before the `d=4`
  trace-free projection. The Theta Ricci scalar insertion now writes only
  `output[Theta] += 0.5 delta R` from that assembly, and the `A_IJ` Ricci
  curvature insertion now writes only
  `output[A_IJ] += [delta R_IJ]^TF` from the same trace-free assembly.
  The 13-variable state vector is named, gauge perturbations are frozen
  (`delta alpha=delta beta^i=delta B^i=0`), and per-variable RHS block status
  is labeled. The actual complete frozen-gauge modified-cartoon RHS
  linearization beyond these partial blocks is not yet available, so no
  eigensolver, shift-invert solve, unstable/stable point, threshold estimate,
  or `k_c r0` convention lock was added. The commonly quoted
  `k_c r0 ~= 0.876` remains provisional.

## Production Pivot: GRChombo Adaptation And Oracle Comparison

- Goal: use GRChombo as the production framework/convention authority and the
  completed custom Stage 4AO-C interior as an independent comparison oracle.
- Inventory: `docs/grchombo/custom_solver_grchombo_overlap_gap_checklist.md`
  records formula, convention, infrastructure, and diagnostic ownership;
  `custom_solver_grchombo_comparison_test_plan.md` fixes four comparison
  levels and pre-run tolerances; `grchombo_adaptation_backlog.md` prioritizes
  production adaptations; `agent_capability_scorecard.md` defines evidence
  metrics without assigning an overall score.
- Roadmap decision: the custom solver remains a validated reference oracle
  and agent-capability benchmark; GRChombo is the production framework.
  Missing functionality will be adapted directly. The unresolved custom outer
  spectral boundary is deferred and nonblocking. This supersedes that gate
  only for the active production path; it does not claim the original
  Stage 4AO-C spectral objective passed. Stage 4AO-D and Checkpoint G remain
  incomplete. See
  `docs/grchombo/roadmap_decision_after_dependency_qualification.md`.
- Dependency state: the historical Chombo SHA is unavailable and only
  inferred. The project-qualified official tuple is GRChombo
  `37e659523830418b210acea1661dac0e00bb1b75` plus Chombo
  `8684f2e000106f1abadb72642e1d15351867f98f`, detached and clean. Core DIM2
  libraries, the real target `2/4/4` probe, `VariableStoreTest`, and
  `CCZ4GeometryUnitTest` pass. Former container provenance, PETSc/AHFinder,
  MPI, and a full black-string runtime remain unqualified.
- Completed comparison evidence: batches 1-4 cover exact state correspondence
  and conformal/connection/visible-Ricci algebra; selected visible CCZ4
  Ricci-Z and RHS families; second- versus fourth-order convergence,
  Richardson agreement, and visible cleanup; and the GP storage convention,
  stock-`d=3` versus target-`d=4` families, full custom GP residuals, fixed
  lapse-source seam, and Fourier/parity convention. This is substantial
  capability evidence, but no final overall score is assigned before
  hidden-sector production equivalence and real evolution runs.
- Completed adaptation foundations: source locks, verifier and target probe,
  the 18-slot variable and exact registration/name metadata contracts, the
  reduced `(2+2)` Vars seam, exact GP point initializer, and analytic GP
  derivative metadata, the real `Cell`/`FArrayBox` storage seam, and the
  isolated real DIM2 GP `BoxLoop` initializer. These are not live application
  wiring.
- Active sequence: [complete] `Cell`/`FArrayBox` storage seam; [complete]
  isolated GP `BoxLoop` compute/traversal; [complete] pointwise
  hidden/cartoon RHS and separately reported complete 13-row equivalence;
  [complete] pointwise hidden-aware cleanup/constraints and fixed lapse
  source; [next] live BoxLoop RHS/cleanup/source wiring and periodic `z`/ghost
  ownership; unperturbed GP evolution; perturbed Fourier-mode evolution and a
  first threshold estimate; then horizon/nonlinear diagnostics after
  PETSc/AHFinder qualification.
- GP `BoxLoop` checkpoint repair: accepted only after real test-policy
  instrumentation observes one 18-slot adapter call per requested cell and
  the numerically exact direct-write bypass observes none. Project-owned code
  remains under strict `-Werror`; dependency headers alone are `-isystem`.
- Audit policy: substantive independent audits follow the assembled
  storage-plus-`BoxLoop` initializer, complete hidden/cartoon 13-row
  equivalence, integrated cleanup/constraints/fixed source, first unperturbed
  evolution, and first perturbed growth-rate run. Avoid per-substep audits.
- Completed pointwise cleanup/constraint/source checkpoint: hidden determinant
  and trace multiplicity are two, the projection denominator is four, exactly
  `H,Mx,Mz` are emitted, the Hamiltonian is
  `R+3K^2/4-A_IJ A^IJ` with direct locked GRChombo Ricci, and the fixed source
  is lapse-only and field-independent. The constraint target total matches
  the independent long-double source-convention oracle under the locked
  tolerance, including a non-trace-free discriminator and genuine Fourier
  parity projections.
- Exact next implementation substage: live BoxLoop RHS/cleanup/source wiring
  and periodic-`z` ownership. Keep evolution, diagnostics, horizons, and
  AHFinder out of scope.
- Infrastructure policy: reuse GRChombo RK4, AMR, MPI/OpenMP,
  checkpoint/restart, ghost exchange, parameter parsing, interpolation,
  reductions, and AH framework. Do not independently rebuild them.
- Known source boundary: inspected GRChombo commit `37e6595` contains no
  black-string or modified-cartoon production implementation. An authoritative
  collaboration branch/source for those terms, if one exists, has not been
  supplied. The project lock is qualified independently of the unrecoverable
  historical Chombo SHA. Container and PETSc-enabled AH provenance remain
  unresolved.
- Production-adaptation preflight: the inspected GRChombo origin and full
  commit are now locked in a tracked manifest with a read-only detached/clean
  verifier. The target design is a compact 18-slot black-string state with
  `CH_SPACEDIM=2`, `GR_SPACEDIM=4`, `DEFAULT_TENSOR_DIM=4`, no visible-`y`
  slots, and one stored `ww` representative carrying hidden multiplicity two.
  The isolated enum/name/metadata contract and focused mutation fixture now
  pass; the old 27-slot live smoke scaffold is unchanged. The dedicated
  reduced Vars adapter and pointwise GP initializer pass storage-agnostic
  tests. The real Chombo storage wrapper is now the authorized next substage.
  PETSc/container provenance remains unresolved, and no production RHS,
  cleanup, source, grid, or evolution path has been added.

## Stage 5: Pau Diagnostic Reproduction

- Goal: reproduce the first scientific milestone: Pau / Figueras-Andrade-Franca-Gu style 5D black-string horizon diagnostics.
- Main deliverables: `L10_med` reproduction, horizon-analysis pipeline, multi-`L` sweep, generation tables, Fourier diagnostics, reproduction memo.
- Current status: not started.

## Stage 6: Convergence And Validation

- Goal: establish that the reproduction diagnostics are numerically robust.
- Main deliverables: `L10_low/med/high` campaign, area convergence, `R_min` convergence, pinch-off fit stability, Fourier/generation-time convergence notes.
- Current status: not started.

## Stage 7: Radiation/Wave-Extraction Design

- Goal: choose and validate a radiation-extraction strategy appropriate for the 5D GL transition.
- Main deliverables: extraction design memo, observable definitions, boundary/large-radius requirements, validation tests.
- Current status: not started.

## Stage 8: Waveform/Spectrum/Radiated-Energy/Angular-Distribution Computation

- Goal: compute the radiation observables from validated simulations.
- Main deliverables: waveform pipeline, spectra, radiated energy by mode, angular distribution, dependence on generation times and Fourier modes.
- Current status: not started.

## Stage 9: Physics Interpretation And Paper/Thesis Writeup

- Goal: interpret the numerical results and prepare publishable documentation.
- Main deliverables: physics narrative, figures/tables, uncertainty budget, paper or thesis chapters.
- Current status: not started.
