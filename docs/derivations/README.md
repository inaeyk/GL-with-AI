# Derivation Checks

This directory contains reproducible derivation and check scripts for the 5D
black-string modified-cartoon reduction.

These scripts are not GRChombo evolution code. They are used to validate
geometric formulas before implementing them in the project-specific GRChombo
layer.

Stage 3C starts with machine-checkable Christoffels, trace multiplicities, and
Gullstrand-Painleve-like extrinsic-curvature formulas for the unperturbed
uniform black-string slice. Later stages should extend this area to Ricci
tensor/scalar terms, CCZ4 RHS source terms, regularized small-`x` limits, and
unit-test fixtures.

Current artifacts:

- `modified_cartoon_geometry_sympy.py`: SymPy checks for the spherical
  Christoffels, hidden-direction multiplicity, GP extrinsic curvature, and
  cartoon trace bookkeeping.
- `modified_cartoon_geometry_notes.md`: prose notes describing what the script
  checks, what it does not check, and the next derivation targets.
