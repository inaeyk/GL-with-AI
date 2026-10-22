# Codex extended scientific Goal template (GL Radiation)

**Identity:** `T-YYYYMMDD-GLR-GOAL-NNN`
**Branch:** `research/sav-radiation`
**Writable scope:** `research/sav/**` only.
**Human owner:** Savdeep Sethi; numerical owner Michael Yan.
**Review status:** PREPARED / RUNNING / PARTIAL / VERIFIED / BLOCKED.

## Primary result
State a hard, mathematically substantial scientific target with an
independent verification criterion and meaningful negative-result branch.
Specify action/equation/gauge/field normalizations. Explain why answering
the target matters for measured radiation, not merely for code architecture.

## Milestone ladder
A. Source and convention audit.
B. Mathematical derivation in a reproducible notebook/note.
C. Independent calculation or constructive cross-check.
D. Physics extraction/observable with uncertainty/limitations.
E. Adversarial audit and handoff.

Milestones are adaptable in method and ordering; the central deliverable
must not be quietly weakened.

## Budget and permissions
- Only non-production symbolic, algebraic and bounded 1D perturbative
  calculations when actually needed.
- Declare finite representative parameter set and refinement/error budget.
- At most two substantive repair attempts for each central failed gate;
  then document obstruction and stop or pivot with human approval.
- No production numerical code, GRChombo changes, branch merges,
  force pushes, new experiment campaigns, or MPI/AMR simulation launches.
- Maintain focused scope and honest progress; avoid unbounded research
  digressions or speculative substitute results.

## Scientific gates
- Every physics assertion labeled derived/proven, checked, source-backed,
  approximate, heuristic, or conjectural.
- Compare gauges, constraint propagation and asymptotic boundary
  conditions. Reproduce at least one independent benchmark.
- Normalize observables from an action/covariant energy law; verify signs
  and factors of G_5, L, Fourier +/-n, 4pi sphere measure and r0.
- Request two independent adversarial audits or document why independent
  audit is unavailable; never self-label an unreviewed result verified.
- If proposed leading radiation is zero, report exact cancellation and
  the next allowed order. Preserve negative results.

## Stop classifications
`COMPLETE_VERIFIED`, `MEANINGFUL_PARTIAL`,
`BLOCKED_BY_MATHEMATICAL_OBSTRUCTION`,
`NOT_REPRODUCIBLE`, or `OPEN_NEEDS_APPROVAL`.

## End-of-run handoff
Summary, quantitative outputs, equations/source locations, all exact
commands and residuals, audit reports, commit list, unresolved gates,
suggested next Goal and Michael interface (no production action).
