# Research status — 2026-10-22

## Evidence classes

`PROVEN` direct mathematical derivation with independent audit;
`ESTABLISHED_LITERATURE` source-backed established result;
`ENGINEERING_ACCEPTED` bounded implementation test, not physical acceptance;
`NUMERICAL_NEGATIVE` bounded failed or inconclusive gate;
`DERIVED_PENDING_AUDIT` clear calculation needing independent check;
`CONDITIONAL_MODEL` assumption-dependent estimate;
`OPEN` unanswered.

## GLR-GOAL-001 execution result — 2026-10-22

**Terminal classification: MEANINGFUL_PARTIAL.** See
`goals/GLR-GOAL-001_REPORT.md`. The exact quadratic scalar source,
gauge completion and retarded integral are derived and independently
audited. Four positive normalized numerical overlaps are strong evidence,
not full rigorous interval enclosures. A controlled finite-A energy
fraction / full burst spectrum is not established.

| New item | Evidence | Limit |
|---|---|---|
| SO(3) action, GHY cancellation, Hamiltonian one local DOF | Independent derivation + audit I; exact warped identities | Smooth non-null boundary; null/corner terms need their prescription |
| Unit-gauge GL master and spherical-gauge conversion | All seven SO(3) linear Ricci equations and TT conditions pass exact rational checks | Numeric mode enclosure is empirical |
| GL continuum threshold | kcr*r0=0.8761604020, agrees ~0.876 literature | One threshold solve, not rigorous root interval |
| Growth at L/r0=8,10,12,16 | Omega*r0≈0.02645870,0.06328119,0.08044276,0.09161923 | Three refinements each; estimated error1e-8; published precise point not independently digitized |
| Complete quadratic physical zero-mode scalar source | Box phi0=−sqrt(3)k²A² H_mu nu H^mu nu/16 | Requires actual regular on-shell TT GL mode and completed gauge |
| Retarded transfer | Explicit −1/W overlap; two derivation routes, causal caveats audited | Finite-time independent outgoing scalar data must be specified |
| Four C0 estimates | 28.2964,12.30729,7.02554,3.32104; independent L10 check within7e-8 | Invariant normalization at r=2r0; strong numerical evidence, no complete interval proof |
| Scalar news flux | dotPhi²/G4 from 4D action and 5D TT current | Does not supply a waveform or finite-A remainder bound |
| Finite-A energy fraction and full burst spectrum | OPEN | Need second-order metric/n=2 bounds, next scalar order and transient control |

Audit correction: the stated KK ansatz requires **positive**
`e^{+2sqrt(3)phi}F²` coupling. Uncharged scalar results unchanged.
The old Hamiltonian explanation mixed two constraint counts; corrected.

The table below is the retained **pre-execution** evidence baseline.

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

Branch `research/sav-radiation`, execution input
`0b0b2e1ee1df64a80e4474d6909173d4682564f3`, originally based on
`main@7d04b5b9882dcd476c1457b8d711ac7b5520b2c1`.
GLR-GOAL-001 was explicitly launched by the owner and has reached its
honest partial stop. The four-k/three-refinement analysis allowance is used.
No production code, numerical parameters, other branches, or simulations
were changed. No merge or Overleaf/manuscript work occurred. Michael's
updated science-only interface is `handoffs/MICHAEL_START_HERE.md`.

