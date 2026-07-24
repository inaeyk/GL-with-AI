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

### 2026-07-22: Differentiate encoded Z analytically and project curvature only once

- Decision: Form `partial_a Z_i=(partial_a H_i-partial_a g_i)/2` by analytic
  differentiation of the unreduced hidden-aware `g_i` formulas. Retain all
  `1/x^2` derivatives and hidden multiplicity two. In the complete A rows,
  trace-free projection remains owned only by the geometric-Ricci and
  encoded-Z curvature families.
- Reason: This matches the selected GRChombo source boundary, avoids numerical
  differentiation of production helpers, and prevents both geometric-Ricci
  duplication and a second projection of `qTF_IJ`.
- Consequences: K, Theta, and all four A rows can be assembled from named
  families exactly once. The full A RHS obeys the coupled tangent trace
  identity with the metric row; it is not independently projected again.

### 2026-07-22: Validate upper/lower Z mapping at the complete-row boundary

- Decision: Use an independent nonlinear four-dimensional Cartesian
  analytic-jet oracle to
  reconstruct `Z_over_chi^i`, set `Z^i=chi Z_over_chi^i`, lower it as
  `Z_i=h_ij Z_over_chi^j`, and form `D_i Z_j+D_j Z_i` before evaluating the
  geometric-Ricci-plus-Z split used by the selected K, Theta, and A equations.
- Reason: This validates the conformal-factor and index mapping once at the
  same boundary that consumes geometric Ricci, encoded Z, algebraic terms,
  advection, stretching, and damping.
- Consequences: K, Theta, and `A_xx/A_xz/A_zz/A_ww` are now row-complete.
  The oracle does not claim an independent direct Gamma-form Ricci
  implementation; source-level convention mapping identifies the validated
  split with the selected CCZ4 Ricci-Z content.
  Chi and metric rows, the complete 13-variable/JVP gate, boundaries, MOTS,
  eigensolver, and 4AO-D remain incomplete.

## 2026-07-24 - Target `(2+2)` CCZ4 pointwise ownership

- Stored `hww/Aww` are one Cartesian-like representative, never
  `gamma_theta_theta=x^2`. Target contractions expand that representative to
  two hidden diagonal directions with indices `2` and `3`.
- Gridded directions remain `0=x` and `1=z`; modified-cartoon `1/x` and
  `1/x^2` identities supply hidden derivatives at positive `x`.
- The production seam expands reduced `(2+2)` data to a complete target
  `d=4` state and directly calls locked GRChombo for the complete CCZ4 RHS.
  Hidden sensitivity is reported as full minus a separately evaluated target
  hidden-suppressed result. The seam does not “add only the hidden
  completion.” Exact GP cancellation and the complete direct nonlinear
  13-row oracle comparison pass. Hidden-aware cleanup and constraints remain
  the next physics adaptation.

## 2026-07-24 - Target RHS hidden increment is decomposition evidence

- The production physics owner is locked full target-`d=4` GRChombo, supplied
  by the validated reduced-to-full tensor and modified-cartoon derivative
  expansion. The hidden-suppressed result is a second GRChombo evaluation.
- `hidden_increment_decomposition = target_full_grchombo -
  target_shared_hidden_suppressed`. This reports the hidden-sensitive
  increment but does not independently assign hidden Ricci, encoded-Z, shift,
  Gamma, damping, or target-coefficient families to project code.
- On exact GP data, nonzero suppressed/increment legs cancel to roundoff. The
  hidden-suppressed target-`d=4` K row is itself approximately zero because
  `beta^x partial_x K = -9 lambda^2/4` and `K^2 = +9 lambda^2/4`; this differs
  from the older stock-`d=3` mapping with residual `1/8`.
- Genuine `P_+` and `P_-` validation uses full Fourier-consistent even/one-`z`
  jets and their derivative signs. The older labels “even-variable data” and
  “one-z-variable data” remain data classifications, not parity-sector claims.
