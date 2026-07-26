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

### 2026-07-26: Isolated target-dimension application adapters

- Decision: Keep target-only `Coordinates.hpp` and a forward-only
  `Lagrange.hpp` in `BlackStringToy`, and compile only the GRChombo core
  needed by E1.
- Reason: Locked stock coordinates reject `CH_SPACEDIM=2` with
  `DEFAULT_TENSOR_DIM=4`, while the E1 path needs only the validated
  cell-centred `x,z` convention. The locked interpolation header also contains
  a compiler-visible defect in code E1 does not instantiate.
- Consequences: Core initialization/RHS/cleanup/constraint and ghost fixtures
  build without modifying either dependency. Interpolation, AHFinder, and the
  locked `Lagrange` implementation remain deferred and must be qualified
  before those paths are enabled.
