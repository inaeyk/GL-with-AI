# TODO

## Immediate

- [x] Create repository scaffold.
- [x] Complete Windows WSL2 + Docker setup.
- [x] Reproduce the GRChombo `BinaryBH` very-cheap smoke test locally.
- [x] Add reproducible BinaryBH smoke-test scripts.

## Active

- [ ] Ensure future physics-stage PDF notes accompany each physics/design stage.
- [ ] Review Stage 3B modified-cartoon source-term derivation roadmap.
- [ ] Generate the Stage 3B physics PDF after the Markdown derivation roadmap is reviewed.
- [ ] Extend the Stage 3C derivation scaffold to Ricci and CCZ4 source-term formulas.
- [ ] Review Stage 3A black-string initial-data design note before coding.
- [ ] Resolve GP branch/sign, `K_ij` convention, and zero-shift gauge startup before implementation.
- [ ] Decide `hww/Aww` enum layout and target build path before adding variables.
- [ ] Review Stage 2B scratch workflow hardening before committing.
- [ ] Review Stage 2A `BlackStringToy` scaffold diff and scratch build output before committing.
- [ ] Decide whether the next scaffold pass should keep inherited BinaryBH behavior untouched or trim it before later black-string initial-data work.
- [ ] Decide how and when to resolve PETSc/AHFinder-enabled execution.
- [ ] Decide whether a scratch-only minimal CCZ4 target is needed before reduced-cartoon evolution work.

## Completed

- [x] Review Stage 1 hazard documentation.
- [x] Decide whether Stage 1 is approved.
- [x] Review Stage 1.5 compile preflight results for starting Stage 2A.
- [x] Decide whether to run the Stage 1.5 compile preflight.
- [x] Complete the scratch-copy `GR_SPACEDIM=4` compile preflight.
- [x] Attempt the Stage 1.5B CCZ4-side scratch-copy compile probe.
- [x] Manually rerun the Stage 2A `BlackStringToy` scratch build and smoke test.
- [x] Harden the Stage 2B scratch build script against root-owned Docker outputs.
- [x] Draft the Stage 3A black-string initial-data design note.
- [x] Add physics-stage PDF note workflow and build the Stage 3A PDF note.
- [x] Draft the Stage 3B modified-cartoon source-term derivation and validation roadmap.
- [x] Incorporate Stage 3B review refinements into the modified-cartoon notes.
- [x] Create the Stage 3C symbolic/geometric derivation scaffold.
- [x] Rerun the Stage 3C SymPy derivation script and record passing output.

## Later

- [ ] Implement black-string initial data.
- [ ] Implement compact-z handling.
- [ ] Add GL perturbation controls.
- [ ] Add constraint/gauge diagnostics.
- [ ] Add horizon diagnostics.
- [ ] Add radiation extraction.
