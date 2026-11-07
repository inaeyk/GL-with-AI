# Reproducibility and source provenance

Repository [inaeyk/GL-with-AI](https://github.com/inaeyk/GL-with-AI). The
GOAL-009 branch is `codex/glr-goal-009-vacuum-bondi`. Its
publication base is the GOAL-006 source branch
`codex/glr-goal-006-bondi-control`.

## Pinned inputs

| Evidence | Exact head | Tree | GitHub |
| --- | --- | --- | --- |
| GOAL-005 finite-slice source | `61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708` | `bd958028ea26dffd91d2700b79d49a674e81461c` | [Source PR #2](https://github.com/inaeyk/GL-with-AI/pull/2) |
| GOAL-005 Work audit | `a9bfbec0efe41b881e4b12aa262f5fcfcd94c639` | `692e32bbbc33e08ddfa0715861a8761af5774f02` | [Audit PR #3](https://github.com/inaeyk/GL-with-AI/pull/3) |
| GOAL-006 scientific source and GOAL-009 base | `80f030df0bec99bebe76fd9ab473c419ad1af5a5` | `302e4424216eb2b270f5469d43cef7a31c3167c1` | [Source PR #5](https://github.com/inaeyk/GL-with-AI/pull/5) |
| GOAL-006 Work audit | `c4cac8f2e2dd12cfbdf50651863d2188a75fc87c` | `e35ffd30b664ce9c0de80104475a4245a6013054` | [Audit PR #7](https://github.com/inaeyk/GL-with-AI/pull/7) |

The GOAL-006 source has the GOAL-005 source as its direct parent.
The GOAL-005 source and both Work audit commits each have direct parent
`ad1cbabfcf75dbde39d3d8e61eda90d88ade86dc`. The two Work
commits are independent review evidence. Neither is an ancestor of
the GOAL-009 source base.

On 2026-11-07, the coordinator checked the remote before creating the
isolated GOAL-009 worktree. The integration specialist then independently
checked Git objects, ancestry, allowed paths, remote refs and GitHub PR
metadata. PRs #2, #3, #5 and #7 were open drafts. PR #5 targeted
`codex/glr-goal-005-analytic-radiation` at the pinned GOAL-005
source. PRs #2, #3 and #7 targeted `research/sav-radiation`
at the shared parent above. The two scientific branch tips and the four
PR heads matched the pinned versions. No remote GOAL-009 branch existed
at either check. Check remote state again immediately before publication.

Source sections inspected include the [GOAL-005 mode and quadratic
equations](../GLR-GOAL-005/DERIVATIONS.md), [constraints](../GLR-GOAL-005/CONSTRAINTS.md),
[waveform](../GLR-GOAL-005/RADIATION.md), [verification](../GLR-GOAL-005/VERIFICATION.md),
and [comparison](../GLR-GOAL-005/HANDOFF-MICHAEL.md). The
[GOAL-006 derivations](../GLR-GOAL-006/DERIVATIONS.md) supply the
damped constraint end, exact mean equation, conditional ray/optical
estimates, and forced second-harmonic Cauchy system. The independent
[GOAL-005 audit §§2–6](https://github.com/inaeyk/GL-with-AI/blob/a9bfbec0efe41b881e4b12aa262f5fcfcd94c639/research/sav/verification/AUDIT_GOAL005_FINITE_SLICE_61c7b5c2.md)
and [GOAL-006 audit §§2–6](https://github.com/inaeyk/GL-with-AI/blob/c4cac8f2e2dd12cfbdf50651863d2188a75fc87c/research/sav/verification/AUDIT_GOAL006_80f030df.md)
carry the qualifications on charge, differentiated weighted falloff,
causal trace, physical cuts and finite-tube derivatives. The source
versions remain fixed throughout the comparison.

## Verification commands

From the repository root, using Git 2.54.0 and GitHub CLI 2.103.2:

    git status --short --branch
    git worktree list --porcelain
    git sparse-checkout list
    git show --no-patch --format='%H %P %T %s' \
      61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708 \
      80f030df0bec99bebe76fd9ab473c419ad1af5a5 \
      a9bfbec0efe41b881e4b12aa262f5fcfcd94c639 \
      c4cac8f2e2dd12cfbdf50651863d2188a75fc87c
    git ls-remote origin \
      refs/heads/codex/glr-goal-005-analytic-radiation \
      refs/heads/codex/glr-goal-006-bondi-control \
      refs/heads/codex/glr-goal-009-vacuum-bondi \
      refs/pull/2/head refs/pull/3/head refs/pull/5/head refs/pull/7/head
    for n in 2 3 5 7; do
      gh pr view "$n" --repo inaeyk/GL-with-AI \
        --json number,url,baseRefName,baseRefOid,headRefName,headRefOid,isDraft,state
    done

The isolated checkout used the sparse `research/sav/`
scope. Read and diff checks leave the source and audit branches
independent. Before publication, inspect the actual outgoing
commit set and complete scoped diff, confirm the base tip and remote
destination again, then verify the remote SHA and draft PR base/head
after publication.

## Analytic and symbolic evidence

- [EXTERIOR.md §1](EXTERIOR.md#1-initial-data-with-the-orders-required-by-the-massive-hierarchy)
  gives the weighted initial-data parity estimate. It uses the exact
  damped spatial constraints, their finite-order bounds and smooth
  dependence on the finite starting collar. The cubic remainder is
  established on the initial slice.
- [DERIVATIONS.md §1](DERIVATIONS.md#1-weighted-causal-massive-jets-on-the-reference-exterior)
  gives the weighted energy induction, including the tilted initial slice
  and the actual finite-slice second-harmonic data. Its inputs are the
  pinned GL end and the chosen finite-order GOAL-006 damping estimate.
  This is an analytic proof on the reference wedge. It does not use a
  massive waveform simulation or a finite-radius numerical limit.
- [Independent symbolic checks](checks/README.md) give the Ricci,
  characteristic, normalization and negative-control inputs and explain
  the exact algebra and illustrative numerical outputs. The integration
  specialist independently replayed the saved output with Python 3.14.8
  and SymPy 1.14.0. From the dossier's checks directory, this comparison
  exited with status zero.

      set -o pipefail
      {
        python3 --version
        python3 -B -c 'import sympy; print("SymPy", sympy.__version__)'
        python3 -B check_optical_identities.py
      } | diff -u check_optical_identities.txt -

- The inherited scalar coefficient is the enclosure from
  [GOAL-005's saved interval calculation](../GLR-GOAL-005/compute/early_interval_results.json)
  with the [audit scope](https://github.com/inaeyk/GL-with-AI/blob/a9bfbec0efe41b881e4b12aa262f5fcfcd94c639/research/sav/verification/AUDIT_GOAL005_FINITE_SLICE_61c7b5c2.md#8-reproduction-record).
  The enclosure is reused with its pinned clock, initial slice and
  preparation. No new numerical coefficient for a different interval
  or finite seed follows.
- [LITERATURE.md](LITERATURE.md) pins primary source versions and
  identifies which hypotheses fail or remain to be proved here.

The additional linear optical normal form in
[DERIVATIONS.md §6](DERIVATIONS.md#6-a-radial-normal-form-for-the-linear-optical-principal-part)
has a small exact rational check. With Python 3.14.8 and SymPy 1.14.0,
the following comparison exits zero from the dossier's checks directory.

    set -o pipefail
    python3 -B check_optical_normal_form.py | diff -u check_optical_normal_form.txt -

The test verifies the compact, mixed and radial coefficients, including
an independent substitution on powers of radius. Omitting the residual
Schwarzschild integral fails the equality. This is an operator identity
check on terminal radial functions.

The coordinator replayed the inherited GOAL-005 symbolic and GOAL-006
diagnostic controls from the pinned ancestry with exit status zero.

    python3 -B GLR-GOAL-005/check_identities.py
    python3 -B GLR-GOAL-006/check_diagnostics.py

These paths are relative to the repository's research/sav directory.
The runs used Python 3.14.8, SymPy 1.14.0, NumPy 2.4.6 and SciPy 1.18.0.
The GOAL-005 run reproduced the positive Bernstein minima, the flat TT
Ricci/Bianchi checks and the weighted lapse mutation. The GOAL-006 run
reproduced the manufactured transfer, mass and clock diagnostics,
including a demodulated drift of \(7.64\times10^{-8}\) versus \(0.306\)
under the no-log mutation. The numerical waveform enclosure itself
was taken from the pinned, previously audited record.
