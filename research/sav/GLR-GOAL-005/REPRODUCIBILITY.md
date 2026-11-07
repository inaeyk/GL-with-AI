# Reproduction

Run from the scoped research directory. These checks are mathematical calculations on the L10 mode, radial constraints and quadratic scalar equation. No spacetime evolution is used.

| Check | Command | Evidence |
| --- | --- | --- |
| Exact Bernstein certificate, direct flat Ricci, weighted lapse change of variables | `python3 -B GLR-GOAL-005/check_identities.py` | [Output](check_identities.txt). Python 3.14.8 and SymPy 1.14.0. |
| Independent GL spectral diagnostic and validated two-ended shooting | `python3 -B GLR-GOAL-005/compute/gl_eigenvalue.py` | [Root brackets, horizon connection, convergence and versions](compute/gl_eigenvalue_results.json). |
| Direct exterior characteristic solution and independent free quadrature | `python3 -B GLR-GOAL-005/compute/early_null.py` | [Three refinements and manufactured control](compute/early_null_results.json) and [waveform](compute/early_null_waveform.csv). |
| Interval exact quadratic coefficient, after validating the root | `python3 -B GLR-GOAL-005/compute/early_interval.py` | [Outward-rounded bounds](compute/early_interval_results.json). |

The saved-profile direct computation reads the L10 files in the pinned
baseline's `verification/` directory. The interval route has its own
Arb-enclosed exterior master and source. It does not read the saved mode
profile. Floating point DOP853 outputs in the interval route are candidate
Hermite data. Analytic endpoint bounds and ball-valued defects certify the
candidate. The code writes only its corresponding compact result artifacts.

The computational specialist's [method record](compute/README.md) documents the analytic barriers, exact curvature bound, failed coarse validation attempt and the final executed checks.
