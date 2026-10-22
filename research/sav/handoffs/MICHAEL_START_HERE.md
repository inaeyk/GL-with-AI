# Michael Yan / AI environment — GL Radiation science handoff

Date: 2026-10-22. This document is a **scientific interface**, not an
instruction to implement or run simulations.

## Relationship to your existing work

The authoritative repository is `inaeyk/GL-with-AI`. We have created
the independent branch `research/sav-radiation`, based on
`main@7d04b5b9882dcd476c1457b8d711ac7b5520b2c1`, with
all new material under `research/sav/**`.
Your `main`, `supervised/m2b-cycle1`, and
`backup/m2c-v2-frozen-campaign-20260917` have not been modified.

We understand and preserve the accepted engineering record: KO-stabilized
18-slot d=4 target/2-grid core; Milestone 1 MPI/HDF5/restart/scaling
baseline; bounded M2-B characteristic and corrected-seed fixtures.
We also recognize the **blocked** M2-C GP control at t/r0=2.5 and the
absence of accepted physical linear GL growth, AMR horizons or waveform.
We will not treat fixture success as scientific simulation acceptance.

## Scientific proposal and why it may help

Exact SO(3) does **not** forbid all radiation: there is no ordinary 4D
tensor quadrupole news, but the massless n=0 KK radion is a physical
monopole radiative mode, and n!=0 s-wave massive modes can propagate
above their KK mass threshold.

A growing n=+/-1 GL eigenmode is a nonoscillatory near-zone deformation.
Quadratic interactions **allow** an n=0 source and n=+/-2 source; whether
the complete constrained source gives nonzero radion news is under study.
Our first Codex Goal will independently derive the second-order radion
source, retarded response, and normalized energy flux. We will test
the early-bulge-dominance conjecture, not assume it.

## Proposed data interface for discussion (no action required)

A future physical extraction could use selected metric and first-derivative
data at periodic-z spherical worldtubes, plus slicing/coordinates, areal
radius, constraint data, retarded time and provenance. The primary outputs
would be `Phi(u)`, `dPhi/du`, `(dPhi/du)^2/G4`, cumulative
`E_early/M0`, and resolution/radius/boundary error estimates.
The n!=0 modes need canonical gauge-invariant amplitudes, dispersive
propagation and finite-worldtube flux; cannot be treated by a naive
massless Sommerfeld extrapolation. See
`research/sav/numerics/extraction_contract.md`.

Before any production changes, we would appreciate your assessment of:
(1) what worldtube data are already accessible without hot-path overhead;
(2) what outer-domain and gauge restrictions make early extraction reliable;
(3) whether weak-field exterior matching is preferable to direct large-r
evolution; and (4) which constraints/diagnostics best detect gauge artifacts.

## Two-way collaboration and annotations

You and your AI assistant can read these Markdown sources, send questions,
propose corrections, or append dated, attributed notes through a reviewed
contribution to `research/sav/**`. Please distinguish independent
derivation, numerical evidence, and conjecture; include branch/commit
provenance and exact field conventions. Do not cherry-pick or merge
production changes from our branch. No change to formulation, code,
boundary physics, production schedule or simulation launch occurs without
explicit human review and approval.

Contact/authority: Michael leads numerical work, Savdeep leads this
analytical thread jointly with Michael, Pau Figueras advises.
Recommended reading: `../README.md`, `../NOTATION.md`,
`../STATUS.md`, `../literature/PAST_WORK.md`,
`../goals/GLR-GOAL-001.md`.
