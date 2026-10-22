# GLR-GOAL-001 executed check record

2026-10-22. Non-production analytical calculations only.
Environment: Python3.14.8, SymPy1.14.0, NumPy2.4.6, SciPy1.18.0,
mpmath1.3.0. No package/dependency changes were needed.

## Exact checks (executed, exit0)

```sh
python3 research/sav/verification/gl_linear_check.py
python3 research/sav/verification/check_quadratic_source.py
python3 research/sav/verification/audit_foundation_checks.py
python3 research/sav/verification/audit_source_checks.py
python3 research/sav/verification/check_foundation.py
```

The complete GL exact output is retained in `gl_linear_verification.txt`;
the two auditors' exact output is reproduced in their Markdown reports.
The primary quadratic-source output was:

```text
Python 3.14.8 SymPy 1.14.0
DIRECT_CHRISTOFFEL_QZZ: PASS; Qzz0=-k^2 tr(H^2)/8
CUBIC_ACTION_SOURCE: PASS; Box(phi)=-sqrt(3) k^2 tr(H^2)/16
MUTATIONS_FOURIER_SIGN_LORENTZ: REJECTED
NONZERO_K_UNITARY_COMPLETION: PASS
SCHWARZSCHILD_SCALAR_OPERATOR: PASS
5D_TT_RADION_FLUX: PASS; factor-two mutation REJECTED
SCOPE: exact algebra, not an on-shell GL eigenmode/nonzero-overlap proof
```

The inherited narrow checks were also rerun and passed Fourier selection,
the toy sigmoid integral1/6, and stationary charge/tension algebra. None
is misclassified as a physical GL-radiation calculation.

The exact GL check covers the spherical/unitary map, TT trace, two
divergences, six orbit Ricci equations and sphere Ricci equation,
Schrödinger transformation, intentional Htr sign mutation and horizon
contraction cancellation. The source/audit checks independently cover
Fourier factors, Lorentz sign, source sign, wrong W sign, wrong flux2,
wrong vector exponent, missing half Lie² gauge completion, sphere/gradient
signs and a manufactured nonzero source with zero outgoing overlap.

## Bounded numerical checks (executed, exit0)

```sh
python3 research/sav/verification/gl_linear_mode.py
python3 research/sav/verification/gl_linear_mode.py --extend
python3 research/sav/verification/radion_transfer.py
python3 research/sav/verification/audit_source_transfer.py
```

The GL root solver ran exactly three declared configurations for each of
four physical k and one separate threshold root; see the two GL JSON files.
Matching residuals are below7e-15, but this residual alone does not bound
continuum truncation error. Exact metric reconstruction was independently
checked before interpreting it as an on-shell continuum-mode calculation.

The transfer JSON stores all three configuration outputs, QUADPACK
estimates, horizon leading-tail estimates and analytic scalar-tail-only
brackets. It does **not** equate those with a complete uncertainty bound.
First scalar boundary approach: truncated asymptotic series; second/final:
positive Volterra bound. Each used the same declared configurations.
Some profile/quadrature reproductions reused existing eigenvalues; they
did not add new physical k root solves or discretization levels. Initial
reference script was run before extension data existed, then reused when
the other three cases were available. The final output is the Volterra
version. Initial results/failures are preserved in `negative_results.md`.

The independent auditor re-used only the final saved L10 metric profile,
with another scalar-basis algorithm and horizon treatment. Its JSON
central value12.3072919086602 agrees with the final primary midpoint
12.30729197235746 within6.4e-8. Finite-profile fitting is an independent
numerical cross-check, not a rigorous continuum error enclosure.

## Review verdicts and remaining proof obligations

Audit I: action/boundary/DOF, gauge completion, uncharged scalar flux and
closed scalar source accepted within stated hypotheses; two initial
convention/counting faults found and corrected.
Audit II: source, cubic-action equality, retarded sign and static threshold
positivity accepted; manufactured cancellation retained. Supplemental
reference calculation and positive Volterra inequalities accepted, but
finite-k interval proof and finite-A control **not certified**.

Two distinct scientific AI agents performed these adversarial reviews;
they are not independent external human referees. No production numerical
acceptance is inferred. The final scientific classification is
`MEANINGFUL_PARTIAL`, not `COMPLETE_VERIFIED`.
