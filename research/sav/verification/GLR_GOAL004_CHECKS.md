# GOAL-004 executed checks, provenance and finite budget ledger

2026-10-23. Non-production mathematical verification and explicit effective
models. Executor/publisher Savdeep Sethi; only research/sav/**.
Terminal scientific decision **MEANINGFUL_PARTIAL**; [report](../goals/GLR-GOAL-004_REPORT.md).

## Input and actual operations

Clean original checkout: research/sav-radiation@97a52c42c39018ac9b41f70b0ba9b08cd67c05bc.
Fresh fetched input: **2f4648c379c474ed124e3ef10b62efcd4a783c66**.
The intervening 85fa857d, c2f899d8, 2f4648c3 commits are Sav-authored
research-only preparation; their actual changed paths were inspected.
The connected authenticated GitHub account returned savsethi, id45701520.
The user explicitly designated Savdeep Sethi as executor and authorized
sole writer. This verifies declared identity/history, not cryptographic
signatures or exhaustive absence of other processes. No other work was
interrupted. A later ref change is a publication stop condition.

Actual Git operations to start, from research/sav:

```sh
git status --short --branch
git branch --show-current
git rev-parse HEAD
git log -6 --format='%H %an <%ae> %s'
git config user.name
git config user.email
git remote -v
git fetch --no-tags origin refs/heads/research/sav-radiation:refs/remotes/origin/research/sav-radiation
git log --format='%H %an <%ae> %s' HEAD..origin/research/sav-radiation
git diff --name-only HEAD..origin/research/sav-radiation
git diff --stat HEAD..origin/research/sav-radiation
git pull --ff-only origin research/sav-radiation
```

The lazy blob reads initially failed DNS in the restricted process;
authorized network retrieval succeeded. The runbook-specified pull
fast-forwarded the clean branch; no merge commit/reset/rebase. Complete
GOAL/gates/runbook were then read from the current checkout. The input
manifest records all research Git blob IDs, without production contents.
All source/commit references are public repository provenance, not private
transcripts or machine setup. No newer unverified input is silently used.

## Commands actually executed and their scope

These commands ran from research/sav. Final scientific commands exited 0.
Some commands were rerun only after specific source/diagnostic additions;
that does not allocate new physical configurations.

```sh
python3 verification/goal004_source_geometry.py --probe
python3 verification/goal004_source_geometry.py > verification/goal004_source_geometry_output.txt
python3 verification/goal004_source_geometry.py --post > verification/goal004_source_geometry_diagnostics_output.txt
python3 verification/goal004_data_checks.py > verification/goal004_data_checks_output.txt
python3 verification/goal004_model.py > verification/goal004_model_output.txt
python3 verification/goal004_two_mode.py > verification/goal004_two_mode_output.txt
python3 verification/goal004_audit_geometry.py > verification/goal004_audit_geometry_results.txt
python3 verification/goal004_audit_geometry.py --inspect-primary > verification/goal004_audit_geometry_physical_point.txt
python3 verification/goal004_audit_dynamics.py > verification/goal004_audit_dynamics_results.txt
python3 verification/goal004_figures.py
python3 verification/goal004_validate_artifacts.py
```

The initial source probe failed with operand dispatch and a contraction
diagnostic, as separately preserved; the final full source/post commands
pass. The dynamics audit's coefficient-extraction failure is preserved
in its failures file. A read-only Python saved-profile calculation also
estimated a_h, T(0) and the matched viscosity before the retained scripts
were authored. That exploratory raw reliable-segment T(0)=13.2770628
omitted the near-horizon piece; the retained regular calculation replaces
that diagnostic, without deleting its stated limitation.

The final artifact validator initially rejected its own literal
host-path/credential-prefix search pattern. Requiring a concrete path
component or a token-length suffix removes that false self-match. It
found no actual host paths or credentials. This was validation tooling,
not a scientific failure; the corrected command was rerun.
Staged review then caught an empty input-tree manifest from a root-relative
pathspec used in the research subdirectory. Running that read-only tree
query at the discovered Git root and asserting its controlling-file entry
fixed the manifest before any commit; no input evidence was inferred from
the empty query.

| Implementation/output | Actual result | Limits |
|---|---|---|
| source geometry / results + diagnostics + NPZ | 481 full-source points, physical positive-k local seven linear equations/three Bianchi checks, both zz harmonics; exactly three forced n=2 Green configurations | On-shell local jets are not a global physical profile check; C5/C3 errors and all-seven reconstructed q2 residuals not certified |
| data checks / JSON + stdout | 16 primary exact/manufactured checks including three rejected false cases | Constraint coefficients/mass map, linear kernel, Gaussian transport and kernel nonuniqueness; no nonlinear global theorem |
| one-mode model / JSON + 2 waveform + 2 spectrum CSV | Two action closures, two scalar kernels, explicit auxiliary/free scalar dependence, energy and residuals | Uncontrolled entropy/finite-k/metric/kernel errors; .63 discarded-mode residual |
| two-mode model / JSON + 2 waveform + 2 spectrum CSV | Second/final modal repair, correct dynamic stable response, exact contrast extrema, positive Rayleigh energy ledger | Physical q2 unmatched; next residual ~.8, energy changes ~9x; not converged |
| geometry audit / JSON + stdout | 37 exact checks, seven deliberate wrong cases; five physical-input point checks, two wrong cases | Independent estimators, shared old operator and empirical point input disclosed; no physical M2/global-family certification |
| dynamics audit / JSON + stdout | 109 independently authored exact/manufactured/artifact checks, 18 deliberate wrong cases | Independent identities/variations/output checks, shared model assumptions; no physical error enclosure |
| figure | One inspected three-panel comparison PNG | Scientific model sensitivity visualization, not observed radiation |

Check counts count assertions/records, not independent theorems.
Versions: Python3.14.8, SymPy1.14.0, NumPy2.4.6, SciPy1.18.0,
mpmath1.3.0, Matplotlib3.11.0. Script/input SHA256 values are in the
respective JSONs; the final evidence manifest adds current hashes.
The source-run hash predates a later post-only addition, whose final hash
is explicitly recorded in diagnostics; no full source recalculation is
pretended. No host-specific executable path is required for reproduction.

## Predeclared finite allocation and actual use

Only physical L/r0=10. Existing normalized profile/eigenvalue/C0 retained
with empirical status. **Zero new GL eigenvalue/root solves.** Source
sector three declared inverse configurations:

| log(delta_left) | Rright | grid | relative tolerance |
|---:|---:|---:|---:|
| -16 | 30 | 4001 | 2e-8 |
| -18 | 45 | 8001 | 2e-10 |
| -18 | 65 | 16001 | 2e-12 |

No fourth configuration, optional L8, other k, mass ODE survey, physical
n=0 ODE, 2+1/5D evolution, MPI/AMR or production run. Static scalar
source quadrature, exact/manufactured checks and four small fluid ODEs
are expressly non-production effective mathematics.

Source strategies: endpoint/connection Volterra enclosures and
source-aware Green/weighted inverse. Data strategies: differentiated
nonlinear exterior identity with conditional coefficients and exact
linear parabolic kernel. Model modal strategies: one-mode action and
the second/final stable-mode repair. Each stops after the two bounded
approaches and records the missing estimates; no third repair, large
scan or budget extension. The independent EFT remains conditional and
changes no vacuum equation.

## Independent audits and negative controls

Reviewer I derived its weighted comparison, H1 commutators and KID
identity before seeing new primary code. Its background mass subfamily
was separately checked by the primary author. Its later physical-point
Rzz/Fourier derivation shares the primary empirical radial input and is
not a second eigenfunction enclosure. Reviewer II derived flux/current,
energy/data/fourth-order identities before model code, then independently
varied the proposed fluid actions and inspected actual saved outputs.
The proposed constitutive actions themselves are shared assumptions;
their independent variation is not an independent derivation from full
5D gravity. Both are distinct AI reviewers, not external human referees.

Wrong cases actually rejected include source Fourier/Lorentz factors,
Ricci/Einstein distinction, KID boundary deletion, ground-state drift,
vanishing charge derivative, bad commutators, nonzero-source=>radiation,
interference deletion, unclipped lower bound, quartic/Lie omissions,
static-versus-dynamic memory, wrong logarithmic-time exponents,
two-point kernel uniqueness, massive Bondi/threshold/current factors,
universal quadratic small-mass error and model coefficient mistakes.
Prior exact EF/gauge/asymptotic branch controls are reused from the old
audits rather than silently counted as new physical reruns. The full
physical q2 residual and nonlinear N4 remain unexecuted/unclosed gates.

Audit repairs: EF contraction diagnostic 2fBC->4fBC; model residual
pressure sign and spatial kinetic stress; log-radius-gradient label;
spectral rms DC half weight. All are in the final scripts and preserved
in [negative results](negative_results.md). The independent two-mode
output check uses its own angular quadrature and acceleration from saved
velocity derivatives; it imports no primary ODE implementation.

## Scientific milestone ledger

1. **Launch/provenance:** clean research-only input and sole operator
   established, controlling files inspected, investigator and two distinct
   bounded adversarial reviews delegated. No prior Goal replay.
2. **Source/data milestone:** physical source evaluated, exact endpoint
   and matching identities obtained, background mass derivative closed;
   regular input/global nonlinear closure explicitly failed at (2)/(5)
   in the report/data note. New empirical source values are not M2.
3. **Model milestone:** two action closures and two scalar kernels
   computed. The large residual triggered one final second-mode repair;
   its still-large residual and changed energy were retained as failure
   of convergence. No physical fraction certified.
4. **EFT/audits:** exact memory-dependent Gaussian transport functions,
   distinct checks/mutations and actual output review completed. Separate
   stabilization theory and shared-authorship qualifications retained.
5. **Closure:** report, handoff/status/source/index/negative pointers,
   reproducible scripts/outputs and input/evidence hashes assembled.
   Final task assessment MEANINGFUL_PARTIAL, no follow-on authorization.

## Publication and final validation

Only coherent completed scientific material is committed. Before each
push freshly fetch the authorized ref, inspect branch/HEAD/status,
complete outgoing commit set and diff, reject paths outside research/sav,
and require normal fast-forward. Any concurrent ref movement is a stop,
not permission to merge/rewrite/force. Actual result/remote verification
is appended after it happens and pinned in goal004_publication.json.
No publication success is inferred from instructions alone.
