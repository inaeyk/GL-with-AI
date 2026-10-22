# Launch GLR-GOAL-002 in Codex

**Ready-to-paste extended task. Preparing these files does not start Codex.**
Use the existing repository research branch. The scientific goal permits
non-production symbolic work and bounded one-dimensional perturbative
equations only; no production simulations.

## Codex environment

- Repository: inaeyk/GL-with-AI
- Branch: research/sav-radiation (do **not** start from main,
  supervised/m2b-cycle1 or frozen M2-C).
- Model: GPT-6.1 Sol; reasoning: High; speed: standard.
- Clean local dedicated checkout/worktree; verify branch and status first.
- Allowed modification scope: research/sav/** only. Do not merge to main.
- Read first: research/sav/AGENTS.md; research/sav/goals/GLR-GOAL-002.md;
  research/sav/goals/GLR-GOAL-002_REVIEW_GATES.md; research/sav/STATUS.md;
  research/sav/NOTATION.md; research/sav/goals/GLR-GOAL-001_REPORT.md;
  research/sav/verification/AUDIT_FOUNDATION.md and AUDIT_SOURCE_RETARDED.md.

If using a new dedicated local clone (rather than Michael's active checkout):

~~~sh
git clone https://github.com/inaeyk/GL-with-AI.git ../GL-with-AI-sav
cd ../GL-with-AI-sav
git fetch origin
git switch --track origin/research/sav-radiation
git status --short
~~~

If the branch is already checked out, do not create a duplicate checkout:
use a clean existing one, or a separate worktree checked out from the
research branch. Avoid force pushing and reconcile concurrent branch edits.

## Paste the following as ONE ambitious Codex task

~~~text
Execute T-20261022-GLR-GOAL-002 in inaeyk/GL-with-AI on the
research/sav-radiation branch. Read the complete controlling
research/sav/goals/GLR-GOAL-002.md and the independent scientific
acceptance matrix in GLR-GOAL-002_REVIEW_GATES.md, together with
AGENTS.md, NOTATION.md, the GLR-GOAL-001 final report/derivations,
both prior audits, the current STATUS, literature, and negative results.

Primary objective: bridge the verified second-order GL-to-zero-mode
radion source/retarded coefficient to a *controlled finite-amplitude
early-radiation energy estimate* for physical constraint-satisfying
initial data. Independently recheck the GOAL-001 source first. Derive
and solve or represent the complete horizon-regular second-order
n=0 Einstein-frame metric/radion and n=±2 gravitational sectors,
with all constraints, gauge completions and charge freedoms. Construct
a quantified validity norm/interval and physical finite-time
initial-data prescription, distinguish free scalar transients from
GL-generated radiation, and determine the strongest justified
finite-interval scalar news, energy fraction and spectrum. Seek a
cubic amplitude equation and a connection to first-bulge dynamics
only when the lower-order hierarchy has passed its acceptance gates.

Make ambitious progress with methodological autonomy and finite
budgets: main case L/r0=10, one additional control only if justified;
use existing GOAL-001 results, at most three refined 1D discretizations
per new sector and two mathematical repair approaches per failed gate.
Do two distinct adversarial mathematical audits with actual checks.
Report failures, exact cancellation, unresolved inequalities and
non-universality if present. DO NOT extrapolate A~1 or quote a
first-bulge emitted percentage absent an admissible finite-A
error bound. A rigorous and valuable partial result is preferable
to false closure. Deliver GLR-GOAL-002_REPORT.md, verification
records, a scientific handoff for Michael and a terminal classification.

Write only research/sav/**. Do not change Michael's simulation
code, physics parameters, other branches or production workflow.
Do not run, submit or launch any 5D spacetime, CCZ4, GRChombo,
MPI/AMR or production numerical simulations. Small symbolic and
bounded 1D perturbative checks are permitted by the Goal.
Do not launch a subsequent Goal or merge anything automatically.
~~~

## After completion

Review the science commits and GLR-GOAL-002_REPORT.md on the branch.
Read both new adversarial audits, reproduce the narrow key identities,
and independently assess whether an A_max and radiated-energy
uncertainty actually follow from the physics. Explicitly decide
whether another Goal is warranted. No automatic production handoff
or numerical implementation is authorized.
