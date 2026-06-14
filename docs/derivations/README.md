# Derivation Checks

This directory contains reproducible derivation and check scripts for the 5D
black-string modified-cartoon reduction.

These scripts are not GRChombo evolution code. They are used to validate
geometric formulas before implementing them in the project-specific GRChombo
layer.

Stage 3C starts with machine-checkable Christoffels, trace multiplicities, and
Gullstrand-Painleve-like extrinsic-curvature formulas for the unperturbed
uniform black-string slice. Stage 3D extends the scaffold to a diagonal
nontrivial hidden metric component `q(x,z) = gamma_ww`, including hidden-sector
Christoffels and Ricci structural checks. Stage 3E adds a nonconstant
`q(x,z)` warped-product Ricci regression to guard hidden-sector derivative
terms. Stage 3F translates the diagonal physical cartoon variables into
conformal variables `chi`, `h_AB`, and `hww`. Stage 3G extends that
translation to an off-diagonal reduced block with `h_xz` / `gamma_xz`. Stage
3H decomposes the future CCZ4 RHS into validation blocks before implementation.
Stage 3I records small-`x` regularity expectations and symbolic removable-limit
checks. Stage 3J designs the unit-test fixture layer that should protect later
C++ implementation work.

Current artifacts:

- `modified_cartoon_geometry_sympy.py`: SymPy checks for the spherical
  Christoffels, hidden-direction multiplicity, GP extrinsic curvature, and
  cartoon trace bookkeeping.
- `modified_cartoon_geometry_notes.md`: prose notes describing what the script
  checks, what it does not check, and the next derivation targets.
- `modified_cartoon_nontrivial_hww_sympy.py`: SymPy checks for a diagonal
  reduced metric with nontrivial `gamma_ww = q(x,z)`, including hidden-sector
  Christoffels, angular contractions, Ricci multiplicity, and flat Ricci
  limits.
- `modified_cartoon_nontrivial_hww_notes.md`: prose notes and exact output for
  the Stage 3D nontrivial-`gamma_ww` symbolic check.
- `modified_cartoon_nonconstant_q_sympy.py`: SymPy regression comparing direct
  Ricci for `q=(1+lambda x+mu z)^2` against warped-product formulas.
- `modified_cartoon_nonconstant_q_notes.md`: prose notes and exact output for
  the Stage 3E nonconstant-`q(x,z)` derivative-sector regression.
- `conformal_cartoon_translation_sympy.py`: SymPy checks for translating
  physical `gamma_ww=q` into conformal `q=hww/chi`, including derivative
  identities, physical radius derivatives, and diagonal tracelessness.
- `conformal_cartoon_translation_notes.md`: prose notes and exact output for
  the Stage 3F conformal-variable translation.
- `offdiagonal_conformal_cartoon_sympy.py`: SymPy checks for the Stage 3G
  off-diagonal conformal-cartoon determinant, inverse metric, tracelessness,
  extrinsic-curvature reconstruction, normalized determinant guard,
  denominator guard, and diagonal limit.
- `offdiagonal_conformal_cartoon_notes.md`: prose notes and exact output for
  the Stage 3G off-diagonal reduced-metric extension.
- `offdiagonal_ricci_flat_gate_sympy.py`: SymPy Ricci flatness gate for
  constant-shear and `x`-dependent-shear off-diagonal metrics.
- `offdiagonal_ricci_flat_gate_notes.md`: prose notes and exact output for
  the Stage 3G sheared-flat off-diagonal Ricci regression.
- `ccz4_rhs_block_decomposition_notes.md`: Stage 3H planning note decomposing
  the future CCZ4 RHS into blocks with exact, reference-code, or
  convergence-based validation routes.
- `small_x_regularization_sympy.py`: SymPy checks for representative
  removable singularity limits near `x = 0`, including even hidden-sector
  fields, odd off-diagonal components, and the assembled
  `tilde_Gamma^x` / `hat_Gamma^x` cartoon connection singular part.
- `small_x_regularization_notes.md`: Stage 3I notes on parity, axis matching,
  conformal small-`x` conditions, and deferred numerical implementation
  choices, including the distinction between cartoon-axis regularity and the
  co-located physical black-string singularity.
- `unit_test_fixture_design.md`: Stage 3J fixture design mapping the existing
  Stage 3C-3I symbolic gates to future C++ unit tests, RHS-block tests,
  reference-code comparisons, and convergence/evolution checks before C++
  source-term implementation.
