# Physics Decisions

Durable physics decisions only. Use [PROJECT_LOG.md](PROJECT_LOG.md) for day-to-day notes.

## Template

### YYYY-MM-DD: Decision

- Decision:
- Reason:
- Consequences:

### 2026-07-22: Separate geometric Ricci from the CCZ4 encoded-Z completion

- Decision: For the Stage 4AO-C frozen-GP non-Gamma audit, interpret the exact
  selected GRChombo `compute_ricci_Z` result as
  `R^Z_IJ=R^g_IJ+2D_(I Z_J)`. Existing raw Ricci helpers own `R^g_IJ` only.
- Reason: The source constructs Ricci from evolved hatted Gamma and reconstructed
  `Z_over_chi`; assuming the metric-derived raw Ricci already includes that
  completion would remove the K/Theta/A dependence on the encoded constraint
  vector.
- Consequences: The precise remaining terms are `+q` in K, `+q/2` in Theta,
  and `[q_IJ]^TF` in A, with a four-dimensional trace and two hidden `ww`
  copies. The separately oracleable shared q-tensor block now exists and must
  not be combined with a second full Ricci path on top of existing raw Ricci.

### 2026-07-22: Keep the encoded-Z tensor helper below the mapping and row boundaries

- Decision: The first encoded-Z helper takes `z_i` and its derivative jet
  directly, returns one representative `q_ww` but uses two copies in the
  scalar trace, and applies the `d=4` trace-free projection once. It does not
  expand `Z_i=0.5(H_i-g_i)` or insert row-specific weights.
- Reason: This boundary has an independent analytic spherical
  modified-cartoon oracle and isolates the hidden-direction and projection
  conventions without duplicating geometric Ricci.
- Consequences: K `+=q`, Theta `+=q/2`, and A `+=qTF_IJ` are implemented as
  separate typed insertion blocks. The nonlinear selected-branch
  finite-difference oracle remains deferred until complete row assembly,
  where upper/lower-Z and conformal mapping are validated once. No non-Gamma
  completion gate opens.

### 2026-07-22: Keep encoded-Z row weights separate from Ricci construction

- Decision: Apply `q`, `q/2`, and `qTF_IJ` only in separate K, Theta, and A
  insertion blocks that accept the opaque encoded-Z completion and no
  geometric-Ricci argument.
- Reason: Row-specific weights and output ownership are independently testable
  at this boundary, while the helper remains the sole owner of hidden trace
  multiplicity and the four-dimensional projection.
- Consequences: Geometric Ricci remains in its existing insertion blocks,
  representative `A_ww` is written once, and complete row assemblers must
  consume both families exactly once. The insertion flags may be true while
  every non-Gamma variable-completion and global spectral gate stays false.
