# Independent checks and claim status

The historical Work reports were inspected at their pinned sibling
commits. Their GOAL-005 numerical enclosures and GOAL-006 constraint
bounds are inputs. The Work reports did not audit GOAL-009. See the
[exact provenance](REPRODUCIBILITY.md).

| Claim | Independent work and disposition |
| --- | --- |
| Exact cubic odd remainder of the damped spatial family | Physics investigator derived the projected linearized block and its weighted inhomogeneous estimate. Integration checked the A1/A2 coefficients, parity at fixed inner lapse and the exponent threshold for the forcing. The coordinator checked the signs and scope against the pinned equations. The estimate applies to initial data. |
| Actual causal quadratic massive jets | The coordinator and physics investigator independently reduced the tensor equation to a positive compact-mass component energy. The verifier checked the weighted identity, both null boundary signs, the lower-weight induction and the finite time-jet recovery on the tilted slice. The initial quadratic projection is defined using parity at fixed inner lapse, then the tuned \(\lambda=O(\epsilon^2)\). The exact evolved even and odd sectors remain open. |
| Compact-normal mean Ricci equation and flux | The computational specialist reconstructed the normal acceleration, density transport, source sign and both flux factors before comparing the detailed source formulas. The coordinator independently recovered the divergence law and scalar rescaling. |
| Optical characteristic equations | The physics investigator used the warped-product and null expansion formulas in invariant sphere radius. The computational specialist independently used a local area variable, then checked Ricci components by direct symbolic Christoffel and warp formulas. Both recovered the square lapse equation and radial supplementary propagation. The extra integration constants and chart assumptions are explicit. |
| Optical comparison and finite-radius correction | The verifier derived the \(rr\) normal form and terminal Jacobian estimate independently of the exterior calculation. It assumes the stated differentiated areal-chart bounds. The verifier independently recomputed the GL ratios and the sign and factor in the quadratic finite-anchor correction. The correction concerns the fixed optical/areal labels at the background intersection. The intrinsic intersection with the actual initial slice is a different curve. |
| Linear optical normal form | The verifier independently eliminated the linear shift, checked the rational conjugation, and checked the admissible weighted domains and failure of a local Cauchy map. The small SymPy script tests the coefficients and a mutation on radial powers. |
| Positive benchmark and physical flux | The quadratic coefficient and analytic far-tail bounds are inherited. The asymptotic scalar stress and five-dimensional strain agree on the coefficient \(1/G_4\) with the stated asymptotic regularity. The exact physical trace and regularity have not been proved. |

The specialist intake included the historical GOAL-005 and GOAL-006
assumptions. Comparisons after independent reconstruction were non-blind.
These are distinct analytic derivations, targeted adversarial review and
symbolic implementation checks. A separate Work scientific audit and
human acceptance remain separate steps.

The [negative ledger](OBSTRUCTIONS.md) records the failed hypotheses.
Exact and manufactured controls cover the compact-normal sign, inverse
lapse, shift and source factors, the optical caustics, normalized torsion
condition, seed–radius limit, tube derivative, mass logarithm, geometric
length and mean-null cone distinctions. No check evolves a nonlinear GL
spacetime or evaluates a massive escape fraction.
