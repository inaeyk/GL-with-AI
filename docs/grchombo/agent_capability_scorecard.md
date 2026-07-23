# AI-Agent Capability Scorecard Template

Status: evidence schema only. Do not assign a final overall score until the
custom-versus-GRChombo comparison hierarchy has produced reviewed results.

## Scoring policy

Record evidence at component level. A component receives no credit merely
because it compiles, produces finite values, matches its own helper, or passes
a test at the wrong abstraction. Independent source/oracle comparison,
mutation sensitivity, and scope-correct gates are separate evidence fields.

Allowed result labels:

```text
not assessed; independently reproduced; exact match; tolerance match;
partial match; mismatch; invalid validation; blocked; direct adaptation chosen.
```

No weighted total or overall grade is defined yet.

## Component evidence table

| Component | Reconstruction origin | Independence from GRChombo implementation | Formula/convention result | Pointwise result | Discrete result | Physical-run result | Mutations / negative controls | Human intervention | Recommended disposition | Evidence links |
|---|---|---|---|---|---|---|---|---|---|---|
| Custom/stock state layout | Real custom `UserVariables.hpp`, frozen-state header, and namespaced stock `CCZ4UserVariables.hpp` | Direct inspection of both actual enums | exact custom 27-slot layout; eleven documented shared correspondences; hidden/visible-y/gauge separation | N/A | N/A | N/A | swapped `h11/h13`, hidden-to-visible-y, duplicate, and omission mutations rejected | original executable gate omitted the custom layout header and required repair | retain focused regression | batch-1 results |
| Conformal algebra | Custom Stage 4A derivation plus test-only `d=3` oracle | High | same formula family; explicit `d=3` versus `d=4` denominator translation | tolerance match: max absolute `4.441e-16`, normalized `6.808e-05` for direct executable identities | N/A | N/A | wrong-dimension trace mutation detected | no batch-1 intervention | retain custom oracle | batch-1 results |
| Hidden Ricci/cartoon geometry | Custom Stage 4AC-4AI | High; no stock counterpart found | custom-only; excluded from stock comparison | not assessed; visible same-dimension Ricci passed separately | TBD | TBD | Existing multiplicity/parity/axis mutations | no batch-1 intervention | retain oracle; adapt to production | batch-1 results |
| Hidden lapse/Aww curvature | Custom Stage 4AJ-4AL | High | TBD | TBD | TBD | TBD | Existing trace/projection mutations | TBD | retain oracle; adapt to production | TBD |
| Hatted-Gamma/Z reconstruction | Custom Stage 4AN/4AO-C | Medium-high; direct GRChombo Christoffel path compiled | visible convention match; hidden increment custom-only | tolerance match: max absolute `1.837e-14`, normalized `1.036e-02` | N/A | N/A | connection-sign mutation detected; hidden increment separated | no batch-1 intervention | retain custom oracle | batch-1 results |
| Complete 13-row frozen interior | Custom Stage 4AO-C | High at analytic-oracle layer | visible same-dimension CCZ4 family conventions match; hidden/Gamma rows not assessed here | partial tolerance match: all directly overlapping `chi,h,K,Theta,A` families and combined rows pass at stock `d=3` | N/A | N/A | omission, duplication, sign, dimension, and BSSN mutations detected | no new batch-2 human intervention | retain trusted oracle; adapt hidden production path | batch-2 results |
| Nonlinear analytic JVP oracle | Independent test evaluator | High | N/A | TBD | N/A | N/A | Epsilon sweep and family mutations | TBD | retain trusted oracle | TBD |
| GP background residual | Custom analytic/discrete derivation | High | TBD | TBD | TBD | TBD | Raw-versus-source separation | TBD | retain oracle | TBD |
| Inner radial helper | Custom formulation | High but nonproduction | N/A | N/A | Existing second-order evidence | N/A | Stencil/reach/sign mutations | TBD | focused regression only | TBD |
| Stationary matrix extraction | Derived from validated custom rows | Low independence from custom source; high representation fidelity | Source/matrix match `2.44e-16` | N/A | N/A | Determinant audit exposed bad inference | Required correction | focused regression only | stationary audit |
| Diagnostic outer row layout | Custom scaffold | N/A | no accepted physics | invalid for boundary validation | stencil only | N/A | Some ownership checks | Multiple corrections | freeze deferred | boundary audit |
| One-hot WKB projector | Custom failed attempt | N/A | invalid abstraction | invalid validation | N/A | N/A | Replacement-subspace test exposed failure | Human correction required | quarantine | boundary audit |
| GRChombo production infrastructure | Direct reuse | N/A | authority; stock comparison compiled at `d=3` | direct header geometry paths execute; Chombo-grid path remains blocked | TBD | TBD | wrong-dimension and sign controls | no batch-1 intervention | direct adaptation/reuse | batch-1 results |
| Visible physical Ricci | Independent test-only differential-geometry oracle | High; does not call/copy GRChombo Ricci | same-dimension convention match with `det(h)=1` jets | tolerance match: max absolute `2.220e-16`, normalized `1.530e-04` | N/A | N/A | `Rxz` sign mutation detected | no batch-1 intervention | retain custom oracle; adapt hidden path separately | batch-1 results |
| Visible encoded-Z Ricci completion | Independent covariant derivative of reconstructed `Z_i` | High; direct side is `compute_ricci_Z-compute_ricci` | Tensor, scalar, and `d=3` TF conventions match directly; standalone `Z_over_chi`, `Z^i`, and `Z_i` are a source/convention reconstruction using a directly computed contracted connection | direct tolerance match: maximum absolute `2.776e-16`, normalized `2.8691300e-04`; standalone map source/convention discrepancy `1.110e-16` | N/A | N/A | direct omission, double insertion, and wrong TF dimension; local-reconstruction wrong lowering and missing chi | no new batch-2 human intervention | retain oracle; adapt hidden completion separately | batch-2 results |
| Visible `chi/h` RHS | Test-only generalized-dimension custom family assembly | High; direct side invokes actual `CCZ4RHS::rhs_equation` | selected `d=3` coefficients and supplied-gauge convention match | tolerance match: combined maxima `5.551e-17` (`chi`) and `2.776e-17` (`h`) | N/A | N/A | wrong RHS dimension detected | no new batch-2 human intervention | retain oracle; reuse production RHS | batch-2 results |
| Visible `K/Theta/A` RHS | Test-only generalized-dimension custom family assembly | High; direct side invokes actual `CCZ4RHS::rhs_equation` | selected CCZ4, damping, Ricci-Z, Hessian, algebraic, advection, and shift conventions match | tolerance match: combined maxima `1.110e-16`, `5.551e-17`, and `1.943e-16` | N/A | N/A | family omission/duplication/sign, wrong dimension, BSSN branch; no direct A damping | no new batch-2 human intervention | retain oracle; reuse visible production RHS and adapt hidden rows | batch-2 results |

## Required scorecard metrics

| Metric | Definition | Evidence required | Count/value |
|---|---|---|---|
| Components independently reproduced | Formula implemented without calling/copying production execution path | Source provenance plus independent oracle | 3 batch-1 families plus 6 batch-2 groups: encoded-Z and visible `chi,h,K,Theta,A` |
| Exact matches | Discrete conventions, integers, signs, rational coefficients, or slot maps match exactly | Level-1 result record | repaired custom 27-slot permutation, eleven shared correspondences, and explicit compile-time dimension identities |
| Tolerance-level matches | Floating outputs meet declared pre-run tolerances | Level-2/3 result record | 3 batch-1 families; batch-2 raw/encoded/combined geometry and five visible RHS row groups |
| Convention errors found | Wrong dimension, sign, formulation branch, normalization, parity, or multiplicity | Before/after formula and failing mutation | 0 new errors; 1 required dimension translation was made explicit before comparison |
| Implementation errors found by audit | Code behavior contradicted intended capability | Reproducer and corrected disposition | 0 custom or GRChombo defects in batches 1-2; two draft batch-2 oracle/isolation defects were detected by the direct bridge and corrected before acceptance |
| Documentation/gate errors | Claim or true gate exceeded evidence | Historical claim, correction, and present gate | 1 batch-1 state-map evidence overclaim repaired |
| Wrong-abstraction tests | Test passed but exercised a proxy rather than target physics | Test name, proxy, missing target, corrective test | 1 batch-1 gate checked frozen order plus stock enum instead of the actual custom 27-slot enum |
| Human interventions | User/reviewer correction materially changed interpretation or gate | Date, intervention, affected component | 1 batch-1 layout-gate repair; historical entries remain separately countable |
| Successful independent reconstructions | Independent component later matches GRChombo or analytic authority | Reviewed comparison record | 3 batch-1 families plus encoded-Z and five visible batch-2 RHS row groups |
| Direct adaptation preferred | Rebuilding would duplicate mature infrastructure or reduce reliability | Ownership decision and source location | GRChombo tensor/geometry production paths; no new runtime infrastructure rebuilt |
| Research-level unresolved problems | No accepted formulation or validation path | Blocker and deferred owner | custom stationary outer boundary remains one explicit unresolved research area |

## Known audit events to carry into later scoring

These entries are evidence categories, not scores:

| Event | Category | Required scorecard treatment |
|---|---|---|
| BSSN `A^2/K^2` terms were initially attributed to the selected CCZ4 K row | Convention/formulation error | Count after Level-1 branch comparison confirms the correction |
| One-hot transformed amplitudes were presented as a WKB subspace | Implementation plus abstraction error | Record as invalid validation and quarantine the projector |
| Selector rank/mixing and scalar-profile residuals were used as boundary evidence | Wrong-abstraction tests | Record each affected test and its missing full-vector target |
| Hard-coded zero outer parity diagnostics were described as validation | Documentation/test error | Record human correction and replacement requirement |
| Interpolation at three k values was described as an exact boundary pencil | Representation/gate error | Record removal and continued false pencil gate |
| A universal fourfold-light determinant factor and generalized-chain obstruction were claimed, then disproved/retracted | Symbolic/documentation error | Record exact-rational counterexample and human correction |
| Sparse stationary matrices independently match the complete custom source rows to `2.44e-16` | Successful representation result | Credit matrix representation only, not determinant or boundary conclusions |

## Human-intervention record

| Date | Component | Intervention | Why autonomous result was insufficient | Resulting gate/disposition | Evidence |
|---|---|---|---|---|---|
| 2026-07-23 | Batch-1 state mapping | Require the executable fixture to include and validate the real custom 27-slot enum | The original passing gate checked frozen order and stock classification but not numeric custom slots; an `h11/h13` swap was not discriminated | Original exact-map evidence retracted and replaced by the repaired 27-slot, eleven-shared, ownership-separated gate | batch-1 results |

Populate one row per material intervention; do not collapse several corrections
into a single count.

## Batch-1 evidence note

Batch 1 began from clean committed inventory checkpoint
`661468ade479cf003dc5336e665dc7b70edf48c6`. The direct bridge invokes the
inspected GRChombo headers. It establishes three tolerance matches and one
repaired exact custom-layout result: the 27 custom slots form the expected
complete permutation, eleven frozen variables have the documented stock
correspondence, and hidden, visible-y, and gauge ownership are separate. It
does not claim identical complete custom and stock state arrays. It does not
assess the complete thirteen-row RHS, discrete stencils, physical runs, hidden
production geometry, or any boundary condition. No final overall score is
assigned.

## Batch-2 evidence note

Batch 2 began from clean committed batch-1 checkpoint
`921e639dede8a020a34b20d182f43693e757561d`. The direct bridge invokes the
inspected production `CCZ4RHS::rhs_equation` and geometry routines at stock
`d=3`. Raw Ricci, encoded Z, their combination, every exercised visible
`chi/h/K/Theta/A` family, and every combined row pass the unchanged Level-2
tolerance. Ten active mutations are detected. Separate shift-stretch and
trace-correction subterms and locked-zero matter/Lambda terms remain
source-only. The standalone Z-index map is also source/convention evidence:
the fixture locally reconstructs it from a directly computed GRChombo
contracted connection because no callable standalone output exists. Hidden
`ww` terms remain custom-only. Two construction mistakes
in the unaccepted draft oracle—a one-pass lower-Z evaluation and an
unsubtracted Ricci background in the Hessian isolation—were exposed by the
direct path and corrected before results were recorded. No custom-production
or GRChombo defect and no new human intervention was established. No final
overall score is assigned.

## Final-score prerequisites

A final overall score remains forbidden until:

1. the dependency/version manifest is reproducible;
2. the first five executable comparisons pass or have reviewed mismatches;
3. every overlapping RHS family has a Level-1 and Level-2 record;
4. at least the GP background has Level-3 and Level-4 evidence;
5. invalid/wrong-abstraction tests are separated from valid evidence;
6. all human interventions are recorded componentwise;
7. reviewers approve the weighting scheme before any totals are calculated.
