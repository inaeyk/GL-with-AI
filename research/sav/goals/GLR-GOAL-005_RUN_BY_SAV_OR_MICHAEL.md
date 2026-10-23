# GLR-GOAL-005 — safe research-run instructions for Savdeep or Michael

**Status:** READY, NOT RUNNING. This is an analytical Codex task,
not a production simulation. It may be executed by either Savdeep Sethi
or Michael Yan. Coordinate one **sole writer** on research/sav-radiation.

## I. Before launch: name one executor and verify repository permission

Choose a single designated task operator: Savdeep OR Michael. Do not
run concurrent writing Codex tasks against the shared research ref;
the nonexecuting collaborator may review read-only work and send
mathematical comments. Either operator can execute in a separate
research clone. The ability to read the GitHub repository is not
automatic authorization or permission to push.

If Michael does not have write permission, choose LOCAL-ONLY output.
He may later hand Savdeep a clean research/sav-only Git patch/commit set
for review and publication. No collaborator sends credentials in prompts,
logs, task notes, or the repository. Neither executor may change
Michael's active GRChombo checkout, jobs, parameter files or numerical
branches. Avoid touching a running simulation even for inspection.

## II. Environment

- Repository: https://github.com/inaeyk/GL-with-AI
- Only branch for this research Goal: research/sav-radiation.
- Only files writable: research/sav/** (no root CLAUDE.md or AGENTS.md).
- Open a **new Codex chat**, with GPT-6.1 Sol / High reasoning, standard
  speed if the settings are available.
- Task: research/sav/goals/GLR-GOAL-005.md.
- Adversarial gates: research/sav/goals/GLR-GOAL-005_REVIEW_GATES.md.
- Prompt: research/sav/goals/GLR-GOAL-005_LAUNCH.md.
- Start Codex from research/sav/ when possible to discover its scoped
  AGENTS.md. If starting from root, explicitly instruct the agent to
  read research/sav/AGENTS.md before changing files.
- Initial branch SHA must be *fetched*, not assumed to equal the
  preparation commit shown in the Goal. Never hard-reset to it.

## III. Clean sparse clone (recommended if an existing checkout is busy)

Use a fresh, otherwise unused directory in the executor's own
workspace (not inside Michael's active production directory):

~~~bash
git clone --filter=blob:none --sparse \
  https://github.com/inaeyk/GL-with-AI.git GL-with-AI-goal005
cd GL-with-AI-goal005
git sparse-checkout set research/sav
git fetch origin research/sav-radiation
git switch --track -c research/sav-radiation origin/research/sav-radiation
git branch --show-current
git status --short
git rev-parse HEAD
cd research/sav
~~~

The expected branch is research/sav-radiation, with a clean status.
If clone/check-out names already exist, **do not overwrite them**:
choose a new directory or examine the existing dedicated research
workspace. Sparse checkout reduces accidental access; it does not
enforce file-level permissions.

For a suitable existing **research-only** checkout (not Michael's
in-use production checkout), first preserve unrelated work and check:

~~~bash
git fetch origin research/sav-radiation
git branch --show-current
git status --short
git log --oneline -5
~~~

Only if that workspace is explicitly dedicated to research and clean,
switch to research/sav-radiation and use
git pull --ff-only origin research/sav-radiation.
If the current state is dirty, branch diverged, or ownership is
unclear, stop and coordinate. Do not hard reset, auto-rebase,
auto-merge, stash-and-drop or delete another person's work.

## IV. Launch prompt and execution mode

In Codex, read GOAL-005, its review gates, AGENTS, HANDOFF, the four
completed Goal reports and GOAL-004 negatives/independent audits. Paste
the entire block in GLR-GOAL-005_LAUNCH.md, replacing only EXECUTOR and
PUBLICATION MODE at the top. The task may use symbolic code and bounded
validated 1D ODE/Green checks, not spacetime simulation. It must
prioritize actual interval/error certification, not an attractive
floating data table.

Publication mode:
- AUTHORIZED SOLE WRITER: only if the human executor has verified
  GitHub push permission and both collaborators have coordinated
  the current shared-branch writer.
- LOCAL-ONLY: if no write permission or another writer is active.
  The agent should not attempt to publish to the shared remote.

Do not launch GOAL-005 twice, replay previous Goals or automatically
run a follow-up on completion.

## V. Coherent checkpoint commits, fast-forward pushes and ownership

The owner authorizes research-only publication at these milestones:
(a) validated physical eigenfunction/root and source error estimate,
(b) reconstructed n2/complete M2 and residual, (c) final completed
report/audits/handoff. A scientifically important obstruction may replace
either substantive checkpoint. Do **not** push every small edit.

Check source integrity and outstanding changes at the Git *root*:

~~~bash
git branch --show-current
git status --short
git fetch origin research/sav-radiation
git log --oneline origin/research/sav-radiation..HEAD
git diff --name-status origin/research/sav-radiation...HEAD
git diff --check origin/research/sav-radiation...HEAD
~~~

Check every outgoing path is in research/sav/** and inspect the diff,
the verification outputs and the exact commit list. A mismatched
remote head, diverged history or unreviewed external change is a
**stop and human reconciliation**, not permission to overwrite,
force-push, auto-merge or rewrite historical reports.

Only in authorized sole-writer mode, with reviewed fast-forward history:

~~~bash
git push origin HEAD:refs/heads/research/sav-radiation
git fetch origin research/sav-radiation
git rev-parse HEAD
git rev-parse origin/research/sav-radiation
~~~

Confirm the resulting exact hashes agree. Record a failed push as a
failure; do not declare publication success from local commits.

In LOCAL-ONLY mode, keep local commits/notes and report exact local
commit SHAs, checks, source proofs, and a reviewed research-only patch.
From the Git root, if an external file is permitted in the user's
own workspace, for example:

~~~bash
git format-patch --stdout origin/research/sav-radiation..HEAD > ../GLR-GOAL-005.patch
~~~

This saves the patch **outside** the repository. Share the patch
through an approved collaboration channel; Savdeep can inspect/apply
it without modifying Michael's production branches. No fork/PR/new
remote branch is automatically created or requested.

## VI. Two-person scientific handoff

At completion the operator sends the other collaborator the report
link/path, both audits, scientific commits, executed command log,
proof/uncertainty table and especially:

- Was the L10 GL eigenvalue **interval-certified**?
- Is the regular physical GL tensor **C5-certified** globally?
- Are n=0/n=2 sources and all Einstein/Bianchi constraints bounded?
- Was the physical n=2 forced solution **source-evaluated and enclosed**?
- Is there a full source-selected M2 and what free-data assumptions
  enter it?
- Is the massless radion C0 coefficient rigorously enclosed?
- What, if anything, can this claim about finite-time physical
  radion radiation? Was A_* or first-bulge E/M actually obtained?
- If blocked, what **one mathematical inequality** needs to be solved next?

The other collaborator reviews the mathematics before considering any
numerical implementation or new task. No request to Michael to run
production simulations follows automatically.

## VII. Hard safety boundaries

Write and publish only research/sav/**. Do not change root policies,
existing code or branches, simulate 5D or 2+1 GR, modify manuscripts,
use Overleaf, run MPI/AMR/cluster experiments or contact collaborators
automatically. All mathematical scripts must record exact versions,
inputs and outputs; no false claim of validated interval arithmetic.
The actual physics and uncertainty checks in the Goal and review gates
remain controlling even if this runbook is shorter.
