# Launch instructions — GLR-GOAL-001

**Prepared, not launched:** this Markdown is a ready-to-paste Codex brief.
No automatic job, background task, production code change or numerical
simulation is initiated by this file.

## Choose environment

- Repository: `inaeyk/GL-with-AI`
- Working branch: `research/sav-radiation`
- Default Codex configuration: **GPT-6.1 Sol / High / standard speed**
- Read at minimum `research/sav/AGENTS.md`, `README.md`,
  `NOTATION.md`, `STATUS.md`,
  `literature/PAST_WORK.md`,
  `goals/GLR-GOAL-001.md` and
  `verification/INDEX.md`.
- Do not start Codex from main or Michael's supervised/frozen branches.
- The Codex agent may write only `research/sav/**`, on the selected
  research branch. Do not merge back automatically.

## If using the Codex desktop app

Open the GitHub repo/workspace and **select or create a clean worktree**
tracking `research/sav-radiation`. Confirm there are no unrelated local
changes. Set model/effort above if the app exposes those selectors.
Read the Goal and paste the complete launch prompt below as one extended
task, not multiple tiny tasks. Do not select any "run simulation" workflow.

## If using an already cloned local working copy

Before handing it to Codex, confirm local uncommitted changes are safe.
If needed use a fresh worktree instead of changing Michael's checkout:

```bash
git fetch origin research/sav-radiation
git worktree add -b glr-goal-001-work ../GL-with-AI-sav \
  origin/research/sav-radiation
cd ../GL-with-AI-sav
git status --short
```

This optional local worktree name `glr-goal-001-work` is for a *local*
workspace only. When publishing, commit/push to the intended
`research/sav-radiation` branch via the ordinary reviewed workflow;
do **not** push a new remote branch or overwrite a ref inadvertently.
If the remote research branch changed, fetch and reconcile explicitly.
Alternatively, checkout the remote research branch in a clean dedicated
clone. Do not execute any GRChombo build/run.

## Copy this complete Codex task prompt

```text
Execute T-20261022-GLR-GOAL-001 in inaeyk/GL-with-AI on the
research/sav-radiation research branch.

Read research/sav/AGENTS.md and the complete
research/sav/goals/GLR-GOAL-001.md as the controlling scientific task.
Also read the linked notation, evidence status, literature summary,
verification records and Michael handoff. Treat historical numerical
and AI-authored notes as review inputs, not proof.

Primary objective: derive the complete second-order, constrained,
gauge-invariant n=0 KK radion source from a normalized unstable n=+/-1
SO(3)-singlet GL mode of a 5D Schwarzschild black string. Prove whether
it has nonzero outgoing retarded scalar-news overlap at infinity and,
if nonzero, determine the normalized transfer coefficient C0(k*r0)
and weakly nonlinear flux, energy fraction and spectrum within a
controlled amplitude interval. Investigate and preserve exact
cancellations rather than assuming radiation.

Pursue all milestones of GLR-GOAL-001 with substantial autonomy,
independent derivations, symbolic verification and two adversarial
mathematical reviews, respecting the finite small-ODE analysis budget.
If the exact coefficient is not accessible, provide the strongest
rigorously checked partial calculation and explicit obstruction.

Only change research/sav/**. No modifications to code/, repository root,
existing branches, Michael's numerical setup or GRChombo, and no
production numerical simulations or cluster launches. Do not merge.
Return a scientifically useful final report and a clear honest stop
classification, exact commit references, and Michael's updated
science-only handoff. Do not ask for routine intermediate confirmation
unless physics assumptions or scope require human approval.
```

## After Codex completes (human review, not auto-launch)

Read its commits and `GLR-GOAL-001_REPORT.md`, run narrow symbolic
verification if desired, and independently audit all key equations,
normalizations, cancellation claims and numerical 1D error estimates.
Only after a scientific review decide whether to authorize a follow-on
Goal or send specific implementation proposals to Michael.

## Common pitfalls

- A PASS on a small symbolic test is not a physical outgoing waveform.
- Pure exponential GL growth is a near-zone instability; nontrivial
  retarded zero-mode scalar news must be derived, not assumed.
- n!=0 massive energy is not the n=0 Bondi-null-infinity flux.
- A claimed early E/M numerical percentage without C0 and source
  history is speculation.
