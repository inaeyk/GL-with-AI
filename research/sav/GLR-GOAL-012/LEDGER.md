# GOAL-012 ledger

## Launch gate

The original workspace was clean at `4aac5ad001a9d6c63efa86640ce05837282fb73f` on `codex/glr-goal-008-massive-radion` and shared the canonical origin `git@github.com:inaeyk/GL-with-AI.git`. Its object store lacked the source pin with lazy fetch disabled. A separate clean GOAL-011 checkout with that same canonical origin held the pin on local branch `codex/glr-goal-011-strong-field-axial-radiation`. The new clean sparse worktree was created from that checkout at the exact source SHA, on the authorized new branch. Local target-ref and source-tree checks found no existing GOAL-012. GitHub's refs and contents endpoints returned 404 for the proposed new ref and dossier at the source pin, while the repository and source ref were accessible.

GitHub PR #13 and the remote source branch both resolved to `eb439228601a649c658ce719b503ff0bdc37a83f`. PR #14's head `75cb3ff3feef98164a65cf14105be6df77ea809c` has that same source commit as its only parent. The GOAL-007 ancestor is `0864d9d807de56cbdd6e64d215e6295508a3007f`. The local and GitHub stacks agree. In ancestry order after GOAL-007:

```text
985c66be2e307e1ca2a938162a4a73683e46bb5b
e68085515126b3f0fbb158d10f7866164c12c064
b8b1df3aeadffc3a0f5a0cfe33f3764fe58dca25
0fd20a312506d6784e7cfb172fee756d97b3a8d2
abbaf46140d9179350322f67c2ccd82a690da79b
d5c75146a29dba88e7502929dbdaeabec1284a39
e385f928cd25e508128a2ae01f40fde6f71ff300
eb439228601a649c658ce719b503ff0bdc37a83f
```

Identity and ancestry checks used Git 2.54.0, `git status --porcelain --untracked-files=all`, `git rev-parse --show-toplevel HEAD`, `git remote get-url origin`, `git for-each-ref`, `git ls-tree`, `git merge-base --is-ancestor`, `git log --format='%H %s'`, and the GitHub pulls, refs, compare and Git commits endpoints. Direct network access from the sandbox returned an SSH DNS error. The GitHub connector verified the remote pins independently of the isolated local object store.

Cycle accounting counts completed scientific or verification passes that produce a distinct technical decision or durable result. Routine reads, tool calls, synchronization and waits do not count as scientific cycles.

## First scientific review, approximately cycle 12

The three-argument transform, canonical contacts and off-pole contour conditions were reconstructed. The physical convergence estimate and lateral Coulomb threshold argument supplied local continuation without a global frequency closure. Fresh interval tests excluded downstream coincidence at \(s_q+2\Omega\) and proved that the pinned baseline bump excites its incoming pole. The empirical translated transfer had consistent weak and symmetric-transform limits. The horizon and outer strong checks retained the off-pole test. The finite-window causal energy and positive-line routes both identified evaluated physical source data as the dominant missing input.

The Work snapshot used for the review was checked with git hash-object against GitHub's directory manifest at the exact Work review SHA. All six Markdown blobs matched, including AUDIT (252526b489b7e8c91c6756d17946cc36406e4edc), CONTINUATION (3cf7392b1bb6d414519002efe9008ccc0b6f16bc) and WAVEFORM (c4c2adbfc8d5448f29febe3cffa74cc71d2a4101). The review is a separate read-only reference.

## Second scientific review, approximately cycle 25

The changed-contour overlap and shortened-cell strong quadrature agreed numerically. The overly wide strong interpolation cell and an overly loose interval normalization were retained as failures. The complete unit-mode directed calculation used new massive matching data and a separate off-pole ingoing continuation and tail. The unit numerator excluded zero. Refined Jost normalization resolved the wide shear quotient. The exact pole-pair norm comparison and holomorphic obstruction apply to the ordered trace, while evaluated physical remainder bounds are missing. The finite-width flux test yielded two directed reference signs with explicit residual thresholds and a compact-trace negative control.

The analytic uncertainty now lies primarily in evaluating the complete finite-cut causal source and its errors, and in any global frequency contour closure. The scoped, tested result is Outcome B. No next checkpoint near cycles 40 or 55 was needed. The prescribed second packet and the remaining stretch objectives were left for independent authorization.

## Review and publication preflight

The five bounded investigations and the subsequent non-blind review are linked in [INDEPENDENT-REVIEW](INDEPENDENT-REVIEW.md). Fresh replays of the threshold/launch, noncoincidence, pole norm, flux and manufactured inversion outputs matched the recorded files. The complete directed projection and both empirical contour runs completed. The baseline source worktree remained clean at the source pin. Git initially rejected the commit because the sparse temporary source object store lacked an unchanged base-tree blob. A full fetch of the exact source branch repaired the object store with no source ref or file changes, and FETCH_HEAD still resolved to the pin.
