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
Christoffels and Ricci structural checks. Later stages should extend this area
to off-diagonal reduced metrics, CCZ4 RHS source terms, regularized small-`x`
limits, and unit-test fixtures.

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
