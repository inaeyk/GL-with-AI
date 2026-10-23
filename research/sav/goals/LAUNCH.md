# Launch instructions — GLR-GOAL-001

> Completion annotation — 2026-10-23: this is the historical GOAL-001
> launch prompt. GOAL-001 executed on 2026-10-22 and completed with
> **MEANINGFUL_PARTIAL**; see [the report](GLR-GOAL-001_REPORT.md).
> Its preparation language grants no current execution permission.

**Prepared, not launched:** this Markdown is a ready-to-paste Codex brief.
No automatic job, background task, production code change or numerical
simulation is initiated by this file.

## Choose environment

- Repository: `inaeyk/GL-with-AI`
- Working branch: `research/sav-radiation`
- Default Codex configuration: **GPT-6.1 Sol / High / standard speed**
- Read at minimum `research/sav/AGENTS.md`,
  `research/sav/README.md`, `research/sav/NOTATION.md`,
  `research/sav/STATUS.md`, `research/sav/literature/PAST_WORK.md`,
  `research/sav/goals/GLR-GOAL-001.md` and
  `research/sav/verification/INDEX.md`.
- Do not start Codex from main or Michael's supervised/frozen branches.
- The Codex agent may write only `research/sav/**`, on the selected
  research branch. Do not merge back automatically.

## If using the Codex desktop app

Open the GitHub repo/workspace and **select or create a clean worktree**
tracking `research/sav-radiation`. Confirm there are no unrelated local
changes. Set model/effort above if the app exposes those selectors.
Read the Goal and paste the complete launch prompt below as one extended
task, not multiple tiny tasks. Do not select any "run simulation" workflow.

## For a fresh dedicated local clone (safer than altering Michael's checkout)

```bash
git clone https://github.com/inaeyk/GL-with-AI.git ../GL-with-AI-sav
cd ../GL-with-AI-sav
git fetch origin
git switch --track origin/research/sav-radiation
git status --short
```

Confirm the branch is exactly `research/sav-radiation` and the working
tree is clean. If the remote branch changes during the Codex run, stop and
reconcile before pushing; do not force-push or merge into `main`.
For an existing checkout with uncommitted changes, use an isolated clean
worktree or fresh clone instead of switching Michael's working branch.
Do not execute any GRChombo build/run.

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
