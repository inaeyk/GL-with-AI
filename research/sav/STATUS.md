# Research status — 2026-10-22

## Evidence classes

`PROVEN` direct mathematical derivation with independent audit;
`ESTABLISHED_LITERATURE` source-backed established result;
`ENGINEERING_ACCEPTED` bounded implementation test, not physical acceptance;
`NUMERICAL_NEGATIVE` bounded failed or inconclusive gate;
`DERIVED_PENDING_AUDIT` clear calculation needing independent check;
`CONDITIONAL_MODEL` assumption-dependent estimate;
`OPEN` unanswered.

| Item | Status | What is and is not established |
|---|---|---|
| Original five-dimensional GL instability | ESTABLISHED_LITERATURE | Physical growing SO(3)-singlet modes exist below threshold; no production GL mode qualified in this repo. |
| GL horizon bulge/cascade behavior | ESTABLISHED_LITERATURE | Lehner–Pretorius and Figueras et al. numerical evidence; their horizon data are not a radiation measurement. |
| Main/Milestone 1 | ENGINEERING_ACCEPTED | KO-stabilized 18-slot 2/4/4 core, MPI, HDF5/restart, level-zero equivalence; engineering only. |
| D12–D16 spectral attempt | NUMERICAL_NEGATIVE | D16 constraint nullspaces not invariant; no qualified physical candidate. |
| supervised/m2b-cycle1 | ENGINEERING_ACCEPTED | Characteristic/seed fixtures, bounded GP and restart checks; no physical GL growth acceptance. |
| frozen M2-C | NUMERICAL_NEGATIVE | First GP run failed at t=2.5 with algebraic-reconstruction admissibility exception; 11 runs not invoked. |
| Exact SO(3) warped-product action | DERIVED_PENDING_AUDIT | Local one-configuration-degree count; reproduce independently in Goal. |
| Massless radion n=0 | DERIVED_PENDING_AUDIT | Symmetry permits scalar radiation; asymptotic news normalization must be independently checked. |
| Massive KK singlet n!=0 | DERIVED_PENDING_AUDIT | Propagation above |omega|>|k_n|; canonical gravitational flux is OPEN. |
| Quadratic n=0 source from +/-1 GL modes | DERIVED_PENDING_AUDIT | Fourier selection allowed; outgoing projection coefficient and potential cancellations OPEN. |
| Early emission dominates | CONDITIONAL_MODEL | Figueras et al. hypothesize it; no measured fraction, no established bound. |
| Early E_rad/M_ADM | OPEN | No physical percentage is established; calculating this is the first Goal. |

## Next success gates

1. Audit SO(3) reduction, propagating singlets, radion Bondi normalization.
2. Linear GL BVP with eigenfunction normalization and independent references.
3. Compute full constrained second-order n=0 source and its outgoing retarded
   overlap, including possible zero result.
4. Bound early-only energy within a controlled amplitude interval; separate
   initial-data transients.
5. Deliver Michael a feasibility-reviewed numerical extraction contract.

## Operating state

Branch `research/sav-radiation`, based on
`main@7d04b5b9882dcd476c1457b8d711ac7b5520b2c1`. GLR-GOAL-001 is prepared in full with launch instructions; **Codex Goal
has not been launched** as of 2026-10-22.
No production code, numerical parameters, existing branches, or simulations
were changed to create these notes.
