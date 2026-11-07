# Stack and reproducibility review

## Verified intake

- The clean isolated GOAL-007 worktree started on
  `codex/glr-goal-007-nonspherical-ringdown` at
  `61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708`.
  The source checkout was serving another GOAL while the specialists
  wrote their disjoint GOAL-007 artifacts. Only the coordinator owns
  Git writes. Copy only the GOAL-007 artifacts to the isolated worktree.
- GitHub's compare endpoint reported the remote
  `codex/glr-goal-005-analytic-radiation` ref identical to the pinned
  source. It separately reported the remote `review/glr-goal-005-61c7b5c2`
  ref identical to `a9bfbec0efe41b881e4b12aa262f5fcfcd94c639`.
  Both commits have parent `ad1cbabfcf75dbde39d3d8e61eda90d88ade86dc`.
  The audit is a sibling reference input. Its GitHub commit response
  provided the two audit paths and patch, while the sparse local store
  confirmed the commit object and did not materialize the audit file.
  The GOAL-007 remote branch search returned no match at intake.
- At intake the GOAL-007 evidence directory had no unresolved local
  Markdown targets, conflict markers, or machine-specific paths.
  Recheck these on the finished dossier.

## Executed replay

Python 3.14.8 and SymPy 1.14.0. Both commands reproduced their saved
stdout byte for byte with pipe failure enabled, run from `research/sav`.

```sh
set -o pipefail
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-007/checks/curvature.py | diff -u GLR-GOAL-007/checks/curvature.txt -
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-007/checks/radiation.py | diff -u GLR-GOAL-007/checks/radiation.txt -
```

| Saved output | SHA-256 at replay |
|---|---|
| `checks/curvature.txt` | `a3a556ad8f4a2051e3e40605ab5d57dbdaa1b5e3cc0263e14bc1bc7a1445f1f6` |
| `checks/radiation.txt` | `6d4a306cb6d16eeaa8231019c117d7de0665c0128f6a67c6b2d33072eaff6b5c` |

The checks reproduce the local algebra and manufactured retarded
calculation documented in [CHECKS.md](CHECKS.md). Scientific acceptance
of the physical projection belongs to the analytical and adversarial
records.

## Coordinator publication gates

1. In the isolated worktree, inspect the current branch, status, all
   outgoing commits and complete diff against the pinned source. Verify
   every changed path begins with `research/sav/GLR-GOAL-007/`.
   Run `git diff --check` on the complete stack. Preserve proof and
   counterexample evidence in the dossier.
2. Refresh the exact remote base and target refs before publishing.
   Target `codex/glr-goal-007-nonspherical-ringdown` with a normal push.
   Confirm the live base can isolate the GOAL-007 diff.
3. Verify remote head SHA and compare paths after publishing. Confirm
   the PR is draft, points to the exact outgoing head, and is stacked
   on `codex/glr-goal-005-analytic-radiation`. Report the observed PR
   link and exact source, audit, and output SHAs.
4. Link the terminal report and handoff from the dossier. Record the
   achieved scientific outcome, assumptions, open strong-field and
   global gates, and the independence boundaries of the specialist work.
