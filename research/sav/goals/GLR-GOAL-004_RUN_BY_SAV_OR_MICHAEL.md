# GLR-GOAL-004 — safe execution runbook for Savdeep or Michael

**This is an executable-owner runbook, not a running task.** Either
Savdeep Sethi or Michael Yan may launch GLR-GOAL-004 after they choose
**one active operator** and confirm access to the correct repository.
The other collaborator may review the notes/read-only branch, but should
not run another writing agent against the same research ref concurrently.

## 1. Coordinate the operator (required before launch)

Agree directly which person is the sole executor of
T-20261023-GLR-GOAL-004 for this run, and whether that person has
repository read+write authorization. Mark the operator in the Codex
first message using this simple single line:

- EXECUTOR: Savdeep Sethi
  OR
- EXECUTOR: Michael Yan

Do not use both simultaneously. If Michael has read-only access, he
may perform the research in his own local checkout and share a reviewed
patch/diff/report with Savdeep; he must **not** attempt an unauthorized
push or change his production branches. Savdeep can subsequently
publish the approved research-only changes through the normal
fast-forward procedure. A fork, PR or new remote branch is **not**
required or automatically authorized.

The repository's permission to read does not itself grant permission
to publish. If a clean checkout, credentials, model setting or repo
access is missing, resolve it through the user's ordinary Codex/GitHub
permissions rather than copying credentials into prompts, notes or logs.

## 2. Project and model

- GitHub repository: https://github.com/inaeyk/GL-with-AI
- Working branch: research/sav-radiation
- Writable files: research/sav/** **only**
- Scientific controlling file: research/sav/goals/GLR-GOAL-004.md
- Acceptance: research/sav/goals/GLR-GOAL-004_REVIEW_GATES.md
- Codex: GPT-6.1 Sol, High reasoning, standard speed, if available.
- Prefer a **new Codex chat** for this extended Goal; do not launch two.
- Use a dedicated local/sparse research checkout or Codex cloud workspace
  with the specified branch. Do **not** use Michael's active GRChombo
  production working tree and do not switch its current branch.

## 3. Fresh research-only clone (Sav or Michael)

The following commands create a **new directory**. Choose an unused
location under your own home/workspace; do not overwrite an existing
folder. Run them in a terminal outside Michael's production checkout.

~~~bash
git clone --filter=blob:none --sparse https://github.com/inaeyk/GL-with-AI.git GL-with-AI-goal004
cd GL-with-AI-goal004
git sparse-checkout set research/sav
git fetch origin research/sav-radiation
git switch -c research/sav-radiation --track origin/research/sav-radiation
git branch --show-current
git status --short
git rev-parse HEAD
~~~

Expected: branch research/sav-radiation, clean status, current remote
research head or a reviewed descendant. **Do not hard-reset to the SHA
printed in these instructions:** the active research branch may have
advanced. Fetch and inspect it before starting. Sparse checkout is an
optional safety aid, not a permission system; the allowed-write rule
still applies.

In an already suitable research-only clone, use:

~~~bash
git fetch origin
git switch research/sav-radiation
git pull --ff-only origin research/sav-radiation
git status --short
git rev-parse HEAD
~~~

If there are unrelated uncommitted changes, or if fast-forward fails,
**stop and coordinate** rather than stash, reset, rebase or overwrite
someone else's work. The existing personal clone may contain Michael's
active work: use the fresh-clone route instead.

To ensure Codex sees the scoped agent guidance, start from the
research/sav directory in this research-only clone when the client
supports an explicit working directory; otherwise instruct Codex to
read research/sav/AGENTS.md before writing. Git root remains the repo
root regardless of current directory.

## 4. Codex desktop / cloud UI method

Start a **new Codex task/chat**, select the repository and exact
research/sav-radiation branch. Select GPT-6.1 Sol / High if exposed.
Confirm status is clean, the branch is current, and another collaborator
is not already writing to that branch.

Read research/sav/AGENTS.md, HANDOFF.md, STATUS.md, NOTATION.md,
verification/GOAL003_REVIEW_QUALIFICATIONS.md, and both new
GOAL-004 controlling files. Paste the **complete one-shot prompt**
from GLR-GOAL-004_LAUNCH.md after the EXECUTOR line. Have Codex state
its starting ref, owner mode (publisher or local-only), and intended
write scope before starting mathematics; no routine confirmation
needed unless it encounters a genuinely unauthorized operation.

Do not configure remote execution or unattended numerical simulations.
The task is analysis; small bounded 1D mathematical checks permitted
by its contract do not authorize production spacetime work.

## 5. Local commits and shared checkpoint publication

The Goal authorizes meaningful **research-only scientific checkpoints**:
(i) after a major rigorous source/constraint theorem or well-defined
obstruction, (ii) after a separately auditable semi-analytical waveform
model, and (iii) at scientific closure. Do not push each trivial edit.

At any publish checkpoint, the designated operator must inspect:

~~~bash
git status --short
git branch --show-current
git fetch origin research/sav-radiation
git log --oneline origin/research/sav-radiation..HEAD
git diff --name-status origin/research/sav-radiation...HEAD
~~~

Ensure every new/modified path is under research/sav/** and no
unintended deletions or production paths exist. Review actual changed
content, verification evidence and commit messages. If
origin/research/sav-radiation advanced from another writer, **stop and
reconcile the divergence together**. Do not force push, merge
automatically, rewrite other's commits or hide a failed lease.

Only an authorized operator with write permission and an understood
fast-forward may publish:

~~~bash
git push origin HEAD:refs/heads/research/sav-radiation
git fetch origin research/sav-radiation
git rev-parse HEAD
git rev-parse origin/research/sav-radiation
~~~

The SHAs must match after a successful final publish. An unauthorized
or read-only operator must **not** run the push. Instead supply local
commit IDs, a research/sav-only patch bundle or reviewed text diffs,
executed checks, and final report to Savdeep.

No automatic follow-on Goal, merge to main, PR, commit to Michael's
supervised/frozen branches or production code edits.

## 6. If Michael is the executor

Michael may use a separate local research checkout, his own Codex
account/client and normal GitHub permission. This task is strictly
theoretical and need not have access to his GRChombo environment.
Avoid changing any in-use simulation directory, job or parameter.

If he has repository write permission and Savdeep explicitly agrees
that Michael is the sole active branch writer, the checkpoint policy
above applies. Otherwise run **local-only** and return a reviewed
research-only diff/report with exact source and check provenance.
Michael's numerical-implementation leadership does not itself change
the authorized theoretical write or production-run scope.

If Savdeep is the executor, Michael can read the current branch,
comment on the science or provide a separately reviewed mathematical
objection without pushing while the run is active.

## 7. What the other collaborator receives on completion

Send a short, reproducible scientific handoff (or share the report URL)
with the following content in continuous prose or a concise table:

- executor name, model/effort, initial and final research refs;
- exact latest GLR-GOAL-004_REPORT.md link, all derivative notes and audit
  paths, and which commands actually ran;
- explicit yes/no/conditional answers for controlled nonzero A_*,
  finite-window radion energy, and **actual first-bulge** fraction;
- physical L10 q2/M2 norms, constraint-closure/charge status,
  homogeneous scalar-data choice and error budget;
- separate heuristic/model estimates with predictions and uncertainty;
- separate stabilized-radion EFT assumptions and propagation result;
- important negative results, open constants and what measurements
  would be useful from Michael's later separately approved simulations.

The collaborating reviewer should independently check central formulas
and challenge physical interpretation before changing any simulation
design. A completed Codex run is not production acceptance.

## 8. If any operational issue arises

Wrong branch, failed permissions, concurrent ref change, dirty
production checkout, missing repository files, partial checkout issue,
ambiguous current owner, exhausted budget or a required physics change:
**stop that operation, preserve local work and report the exact blocker**.
Never respond by force pushing, deleting a branch, editing production
code or launching a simulation. These run instructions are portable:
no absolute host paths, private credentials or personal configuration.
