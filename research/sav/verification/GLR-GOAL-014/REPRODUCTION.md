# Scope and reproduction

The scientific inputs are [source PR #19](https://github.com/inaeyk/GL-with-AI/pull/19) at [0b4bfcbb978b52626fb0816874df06055703c195](https://github.com/inaeyk/GL-with-AI/commit/0b4bfcbb978b52626fb0816874df06055703c195), and the read-only [GOAL-013 Work review at 26a09054366933a197c34fa67f617af7da76e48d](https://github.com/inaeyk/GL-with-AI/commit/26a09054366933a197c34fa67f617af7da76e48d). The review tree directly descends from the audited GOAL-014 source.

## Read set and review tracks

| Track | Pinned read set | Independent evidence |
| --- | --- | --- |
| A causal adjoint | GOAL, REPORT, HANDOFF, LEDGER, ADJOINT-AND-ENDPOINTS, ERROR-BUDGET, A investigator note and A checks. GOAL-013 finite-cut and cap energy identities, and the four new top-level continuity entries. | Green identity plus propagation transpose, direct cap/exterior control, mixed signed terminal row control. [Derivation](a/ADJOINT.md) · [Check](a/check_adjoint.py) · [Output](a/adjoint-output.txt). |
| B sources and constraints | CURRENTS-AND-DATA, B investigator note and checks. GOAL-011 hierarchy, fixed data and mean constraint helpers. GOAL-013 source ledger, graph helper, physical flux and jet envelopes. | Density/Fourier and graph reconstruction, directed tail evaluation, independent divergence-free current control. [Derivation and checks](b/SOURCES-AND-CONSTRAINTS.md). |
| C compact numerical validation | STRONG-NEWS-PILOT, ERROR-BUDGET, C investigator note and checks including failure log. GOAL-011 physical-overlap GL/mean/bump helpers and GOAL-013 graph and cap equations. | Derivation of cap propagation and residual norms, rational margin, subcell and commuted-seam controls. [Audit](C-COMPACT.md) · [Replay](c-replay.py) · [Output and ordinate digest](c-replay.txt). |
| D observable and baseline | PHYSICAL-PROJECTION, FLUX-AND-FRAME, D investigator note, D and E baseline and graph checks and wrong-tilt record. GOAL-011 waveform, GOAL-013 baseline and flux, primary Schwarzschild and Bondi formulas. | New positivity proof for the sharp leading trace, independent range sums and Gauss comparison, exact harmonic normalization and clock fixture. [Derivation](d/OBSERVABLES.md) · [Check](d/checks.py) · [Output](d/checks.txt). |
| E adversarial and late return | INDEPENDENT-REVIEW, E investigator note and checks, conditional stability and nonlinear bridge, GOAL-011 finite data and waveform, GOAL-012 spectral atlas/contour/templates, GOAL-013 waveform and Work audit. | Exact late-news construction, graph representation test, and radial limit versus parameter-jet example. [Review](e/REVIEW.md) · [Check](e/late_rw.py) · [Output](e/late_rw.txt). |

This is a five-track external review with four delegated investigators and the coordinator doing track A and integration. The original claims were available, so the audit is non-blind. Each note states which derivations, fixtures and implementations are independent and which operators, geometry, certified inputs and original outputs are shared. Inspect the links from the pinned parent for the exact sources. Historical analytic and spectral certificates were used at their pinned versions. Their full root searches and complex residue enclosures were outside the replay. The project rules are in [research/sav/AGENTS.md](../../AGENTS.md) and the [Work publication commit](https://github.com/inaeyk/GL-with-AI/commit/93b2d5057c3250b14c89284ac297ebd4091250bd).

## Deterministic checks

Use Python 3.14.8, python-flint 0.9.0, NumPy 2.4.6, SciPy 1.18.0 and SymPy 1.14.0. Run from the review checkout's `research/sav` directory.

~~~sh
bash verification/GLR-GOAL-014/replay.sh
~~~

The driver compares stdout against the pinned files for the 14 A, B, D and E runs, including variants, and the four GOAL-013 inputs listed by the source. The C replay runs three compact cases, two cap checks and two mixed graph checks. It compares the 61 archived stdout lines, records the previously documented SciPy error-estimator warning, and hashes the regenerated binary strong cap ordinates interpreted at exact abscissae. The binary dump is scratch data. The four independent checks are also compared against their retained outputs. All comparisons passed in the pinned environment. [Replay record](REPLAY.txt). The late scalar round trip and the independent ordinary quadrature are empirical consistency tests with possible last-digit platform changes. Recompute interval arithmetic on any new candidate ordinates.

The original [reproduction instructions](../../GLR-GOAL-014/REPRODUCTION.md) retain the source environments, exact grids and raw-command definitions. The source retained [failed terminal sensitivity invocation](../../GLR-GOAL-014/checks/a_terminal_failures.txt), [dependency inflation in the mean](../../GLR-GOAL-014/checks/b_graph_mean_failures.txt), [empirical IVP warning and non-finite first cap quadrature](../../GLR-GOAL-014/checks/c_failures.txt), [wrong graph tilt](../../GLR-GOAL-014/checks/d_graph_wrong_normal.txt), and [independent graph name-shadowing exception](../../GLR-GOAL-014/checks/e_graph_projection_failures.txt). The audit preserves these negative results by pinned reference and adds independent falsification fixtures. No production evolution or production code change enters the check set.

[Audit and acceptance matrix](AUDIT.md).
