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
| Conformal algebra | Custom Stage 4A derivation | High | TBD | TBD | N/A | N/A | Existing determinant/inverse/trace mutations | TBD | retain custom oracle | TBD |
| Hidden Ricci/cartoon geometry | Custom Stage 4AC-4AI | High; no stock counterpart found | TBD | TBD | TBD | TBD | Existing multiplicity/parity/axis mutations | TBD | retain oracle; adapt to production | TBD |
| Hidden lapse/Aww curvature | Custom Stage 4AJ-4AL | High | TBD | TBD | TBD | TBD | Existing trace/projection mutations | TBD | retain oracle; adapt to production | TBD |
| Hatted-Gamma/Z reconstruction | Custom Stage 4AN/4AO-C | Medium-high; GRChombo convention inspected | TBD | TBD | TBD | TBD | Existing sign/hidden/ownership mutations | TBD | retain custom oracle | TBD |
| Complete 13-row frozen interior | Custom Stage 4AO-C | High at analytic-oracle layer | TBD | TBD | N/A | N/A | Full family omission/duplication battery | TBD | retain trusted oracle | TBD |
| Nonlinear analytic JVP oracle | Independent test evaluator | High | N/A | TBD | N/A | N/A | Epsilon sweep and family mutations | TBD | retain trusted oracle | TBD |
| GP background residual | Custom analytic/discrete derivation | High | TBD | TBD | TBD | TBD | Raw-versus-source separation | TBD | retain oracle | TBD |
| Inner radial helper | Custom formulation | High but nonproduction | N/A | N/A | Existing second-order evidence | N/A | Stencil/reach/sign mutations | TBD | focused regression only | TBD |
| Stationary matrix extraction | Derived from validated custom rows | Low independence from custom source; high representation fidelity | Source/matrix match `2.44e-16` | N/A | N/A | Determinant audit exposed bad inference | Required correction | focused regression only | stationary audit |
| Diagnostic outer row layout | Custom scaffold | N/A | no accepted physics | invalid for boundary validation | stencil only | N/A | Some ownership checks | Multiple corrections | freeze deferred | boundary audit |
| One-hot WKB projector | Custom failed attempt | N/A | invalid abstraction | invalid validation | N/A | N/A | Replacement-subspace test exposed failure | Human correction required | quarantine | boundary audit |
| GRChombo production infrastructure | Direct reuse | N/A | authority | TBD adapter checks | TBD | TBD | Framework tests plus project smoke | TBD | direct adaptation/reuse | TBD |

## Required scorecard metrics

| Metric | Definition | Evidence required | Count/value |
|---|---|---|---|
| Components independently reproduced | Formula implemented without calling/copying production execution path | Source provenance plus independent oracle | TBD |
| Exact matches | Discrete conventions, integers, signs, rational coefficients, or slot maps match exactly | Level-1 result record | TBD |
| Tolerance-level matches | Floating outputs meet declared pre-run tolerances | Level-2/3 result record | TBD |
| Convention errors found | Wrong dimension, sign, formulation branch, normalization, parity, or multiplicity | Before/after formula and failing mutation | TBD |
| Implementation errors found by audit | Code behavior contradicted intended capability | Reproducer and corrected disposition | TBD |
| Documentation/gate errors | Claim or true gate exceeded evidence | Historical claim, correction, and present gate | TBD |
| Wrong-abstraction tests | Test passed but exercised a proxy rather than target physics | Test name, proxy, missing target, corrective test | TBD |
| Human interventions | User/reviewer correction materially changed interpretation or gate | Date, intervention, affected component | TBD |
| Successful independent reconstructions | Independent component later matches GRChombo or analytic authority | Reviewed comparison record | TBD |
| Direct adaptation preferred | Rebuilding would duplicate mature infrastructure or reduce reliability | Ownership decision and source location | TBD |
| Research-level unresolved problems | No accepted formulation or validation path | Blocker and deferred owner | TBD |

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
| TBD | TBD | TBD | TBD | TBD | TBD |

Populate one row per material intervention; do not collapse several corrections
into a single count.

## Final-score prerequisites

A final overall score remains forbidden until:

1. the dependency/version manifest is reproducible;
2. the first five executable comparisons pass or have reviewed mismatches;
3. every overlapping RHS family has a Level-1 and Level-2 record;
4. at least the GP background has Level-3 and Level-4 evidence;
5. invalid/wrong-abstraction tests are separated from valid evidence;
6. all human interventions are recorded componentwise;
7. reviewers approve the weighting scheme before any totals are calculated.
