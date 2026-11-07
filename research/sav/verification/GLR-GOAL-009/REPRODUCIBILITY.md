# Reproducibility and scientific re-entry

Read [the report](REPORT.md) for the decision and exact commit stack and [the derivations](DERIVATIONS.md) for the mathematical audit. The main finding is the areal normalization correction, independently reconstructed from the vacuum angular equation and the Einstein-radius transformation. The next mathematical gate is a seed-uniform commuted estimate for the causal exterior, with a finite compact derivative count, a compatible characteristic constraint system and control of the corrected optical and mean scalar quantities.

## Source inventory

The source head tree is `5f993faec314cee3507e7b51c1187f09b5c206cb`. The exact parent diff has 21 paths. The complete new GOAL-009 dossier and the changed hunks in the four continuity files were read at the audited commit.

| Area | Files inspected |
| --- | --- |
| Goal and conclusions | `GOAL.md`, `REPORT.md`, `THEOREM.md`, `HANDOFF-MICHAEL.md` |
| Mathematical arguments | `DERIVATIONS.md`, `EXTERIOR.md`, `OPTICAL.md`, `OBSTRUCTIONS.md` |
| Sources and verification | `LITERATURE.md`, `REPRODUCIBILITY.md`, `VERIFICATION.md`, `AGENT-ACTIVITY.md` |
| Source checks | `checks/README.md`, `checks/check_optical_identities.py` and saved output, `checks/check_optical_normal_form.py` and saved output |
| Shared continuity | `research/sav/HANDOFF.md`, `research/sav/SOURCES.md`, `research/sav/STATUS.md`, `research/sav/verification/INDEX.md` |

The audit read the pinned scoped `research/sav/AGENTS.md`, root project `CLAUDE.md`, notation and GOAL-003 review qualifications for re-entry. It used the Work publication protocol from the separate draft at `93b2d5057c3250b14c89284ac297ebd4091250bd`. Relevant parts of the GOAL-005/006 derivations, scalar certificate, and separate Work audits were inspected as pinned dependencies. [Scoped contract](https://github.com/inaeyk/GL-with-AI/blob/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/research/sav/AGENTS.md) · [Project instructions](https://github.com/inaeyk/GL-with-AI/blob/30b5348503dcb7db8e45b9c1a3f520b2e3aabe10/CLAUDE.md) · [Work publication protocol](https://github.com/inaeyk/GL-with-AI/blob/93b2d5057c3250b14c89284ac297ebd4091250bd/research/sav/AGENTS.md#work-scientific-review-publication)

## Executed checks

The scripts in this directory require Python and SymPy. The saved run used Python 3.14.8 and SymPy 1.14.0.

~~~sh
set -e
{
  python3 --version
  python3 -B -c 'import sympy; print("SymPy", sympy.__version__)'
  for check in check_initial_data.py check_optical_system.py check_areal_news.py; do
    printf '\n%s\n' "$check"
    python3 -B "$check"
  done
} > /tmp/glr009-work-checks.txt
diff -u CHECK_OUTPUT.txt /tmp/glr009-work-checks.txt
~~~

- [Initial data](check_initial_data.py) reconstructs spatial curvature and momentum from Christoffel symbols, verifies the linearized damped block, and checks the oscillator and spherical ADM slope. It rejects a wrong radial momentum term and damping sign. The weighted estimate itself is analytic.
- [Optical system](check_optical_system.py) evaluates independent warped Ricci formulas on two exact manufactured jets in both radial charts. These are finite point checks for nonlinear component identities. The Euler-symbol and arbitrary-mass Vaidya calculations are symbolic identities. Controls reject omission of the compact principal term and inference of vacuum from the four main optical equations.
- [Areal news and causal boundaries](check_areal_news.py) verifies the angular equation and the Einstein-radius leading pullback, rejecting unit areal normalization for variable $\Phi$. It checks weighted current signs, the exact high-frequency density ratio, a widened-cone flux of $-3181/16038$, and a periodic reparametrization that changes the metric mean but preserves length. The distributional radial-averaging argument is analytic.
- An initial circumference check compared two differently factored exact SymPy expressions structurally and failed. Simplifying their difference resolved the comparison. The saved run passes both the length and metric-mean checks. No tolerance was introduced. [Saved output](CHECK_OUTPUT.txt)

From a checkout at the **source** SHA, both GOAL-009 source check programs reproduced their pinned outputs (one trailing blank line ignored in the saved normal-form output). The inherited GOAL-005 and GOAL-006 diagnostics also ran successfully. The latter were run with NumPy 2.4.6 and SciPy 1.18.0. The observed demodulated massive transfer change was $7.64\times10^{-8}$, compared with $0.306$ for the manufactured no-log mutation. These are diagnostic calculations, separate from the continuum certificate.

~~~sh
cd research/sav/GLR-GOAL-009/checks
{
  python3 --version
  python3 -B -c 'import sympy; print("SymPy", sympy.__version__)'
  python3 -B check_optical_identities.py
} > /tmp/glr009-source-optics.txt
diff -u check_optical_identities.txt /tmp/glr009-source-optics.txt
python3 -B check_optical_normal_form.py > /tmp/glr009-source-normal-form.txt
sed '/^$/d' check_optical_normal_form.txt | diff -u - /tmp/glr009-source-normal-form.txt
cd ../..
python3 -B GLR-GOAL-005/check_identities.py
python3 -B GLR-GOAL-006/check_diagnostics.py
~~~

The interval coefficient and GL eigenpair bracket are inherited pinned certificate evidence. This audit checked their role, source and potential normalization, and recorded bounds. It did not rerun the interval integrations or a physical evolution. All newly executed checks were symbolic or lightweight diagnostics. [Pinned numerical enclosure](https://github.com/inaeyk/GL-with-AI/blob/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708/research/sav/GLR-GOAL-005/compute/early_interval_results.json) · [Prior independent audit and reproducibility](https://github.com/inaeyk/GL-with-AI/blob/a9bfbec0efe41b881e4b12aa262f5fcfcd94c639/research/sav/verification/AUDIT_GOAL005_FINITE_SLICE_61c7b5c2.md#8-reproduction-record)

## Applicability of primary sources

- Huneau–Stingo–Wyatt Theorem 1.1 and the exterior §3 estimate are relevant to compact commutations and weak-null structure in a full $4+1$ harmonic system. Their absolute small-data norm (1.10) applied to the audited tilted background $K=O(r^{-2})$ would cost $\int^\infty r^{-1+2\kappa}\,dr$. A positive-mass optical cut also differs from their flat-radius exterior by a logarithm. A new relative estimate is required. [arXiv:2307.15267v1](https://arxiv.org/html/2307.15267v1)
- Hintz Theorem 1.1 supplies a positive-mass exterior and a weighted inverse in $3+1$ vacuum. The rescaled KK equation has a singular $x^{-2}\partial_z^2$ term at $x=1/r=0$, beyond the stated inverse. Lindblad §10, Propositions 24 and 26–27 give a terminal eikonal method after differentiated metric asymptotics. The independent compact moment estimate is necessary in the present characteristic argument. [Hintz, version of record](https://doi.org/10.1007/s00205-023-01931-3) · [Lindblad, arXiv:1606.01591v4](https://arxiv.org/pdf/1606.01591v4)
- Ferko–Satishchandran–Sethi take the compactified asymptotic expansion as an input in (3.5). Their (4.46), (5.28) and §6.4 corroborate the conditional strain and frame normalization. The expansion and massive suppression still have to be obtained from the GL data. [arXiv:2109.11599v3](https://arxiv.org/html/2109.11599v3)
- Sussman's Theorems 1–2 treat massive waves on prescribed admissible backgrounds with the specified short-range conditions. They do not supply the coupled, positive-mass, commuted Einstein estimate here. Chen–Lindblad §1 and `4–5 establish radiation and timelike asymptotics for model wave–Klein–Gordon systems, with different principal and geometric structure. [Sussman, arXiv:2305.01119v2](https://arxiv.org/pdf/2305.01119v2) · [Chen–Lindblad, arXiv:2301.07083v3](https://arxiv.org/html/2301.07083v3)
- Brito–Cardoso–Pani §IV after (30) fixes the reference massive monopole potential. The source script reproduced the near-horizon negative brackets for both harmonics and its sufficient positive far-exterior bound. Those results concern a frozen scalar master. The reference-wedge proof instead uses a positive compact mass term in the tensor system and estimates curvature separately. [arXiv:1304.6725v3](https://arxiv.org/html/1304.6725v3)

## Independent workstreams

Five specialists ran in two waves under the four-child concurrency limit. The coordinator read the pinned change set, checked the normalization by a second route, ran the source and independent programs, and prepared the publication. Runtime model and reasoning-effort identifiers were not independently verified. The source outcome and shared assumptions were visible at intake. Where feasible the specialists reconstructed their assigned calculation before comparing the detailed proof. These are separate derivations or implementations with shared pinned inputs and partial prior exposure.

| Role | Completed work and independence |
| --- | --- |
| Physics investigator | Reconstructed the constraints, mass tuning, damping energy and zero adapted scalar jet. Implemented independent symbolic curvature and damping checks after seeing the advertised result. |
| Verification | Reconstructed current, causal boundaries and Cauchy compatibility before comparing the weighted induction. Derived separate massive and widened-cone counterexamples. |
| Computational checker | Independently related the two optical radii, derived the supplementary Bianchi system and checked it with an arbitrary Vaidya mass. Wrote a separate exact-jet checker after exposure to the abstract and selected formulae. |
| Literature researcher | Reconstructed flux, clock and channel conditions from fixed conventions, verified targeted primary statements, and found the areal normalization conflict by both reduction and the vacuum angular equation. |
| Integration | Independently verified exact commit ancestry and the five live draft pins, compared scope with the publication protocol, and checked the report's claim boundaries before publication. |

The audit is scientific review evidence. Repository and scientific acceptance remain with the human reviewers.
