# Coding Decisions

Durable coding/workflow decisions only. Use [PROJECT_LOG.md](PROJECT_LOG.md) or [CODEX_LOG.md](CODEX_LOG.md) for task notes.

## Template

### YYYY-MM-DD: Decision

- Decision:
- Reason:
- Consequences:

### 2026-07-24: Direct target-dimension pointwise RHS seam

- Decision: Instantiate locked `CCZ4RHS::rhs_equation` at target dimension
  four after expanding the reduced `(x,z,ww)` storage into
  `(x,z,w1,w2)`. Evaluate the complete target RHS directly in GRChombo, then
  evaluate a target hidden-suppressed comparison and report their subtraction.
- Reason: This retains GRChombo ownership of the complete CCZ4 RHS without
  copying the custom assembler or independently rebuilding hidden families in
  production.
- Consequences: The seam is pointwise and produces 13 physical rows only.
  Live BoxLoop wiring and hidden-aware cleanup/constraints remain separate
  future changes.

### 2026-07-24: Hidden reporting is subtraction, not a production RHS owner

- Decision: Supersede the earlier shorthand names with
  `target_shared_hidden_suppressed`, `hidden_increment_decomposition`, and
  `target_full_grchombo`. Define the increment exclusively as full minus
  suppressed. Both full and suppressed paths execute locked GRChombo after a
  policy-controlled target expansion.
- Reason: The production architecture deliberately avoids independently
  rebuilding complete hidden CCZ4 families. The subtraction makes
  hidden-sensitive cancellation observable without overstating ownership.
- Consequences: Direct finite-state nonlinear oracle comparison is the sole
  13-row numerical completion gate. The declared epsilon-sweep JVP is only a
  roundoff/cancellation-dominated secondary diagnostic; target-input
  policies, rather than post-computation row edits, test production mapping
  defects. Output-ownership mutations remain explicitly reporting-only.
