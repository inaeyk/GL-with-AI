# GL Radiation — independent analytical research

**Research collaboration:** Savdeep Sethi and Michael Yan. Pau Figueras provides background scientific advice.

**Physical system:** five-dimensional vacuum Einstein gravity on asymptotic
`R^{1,3} x S^1`, with a uniform Schwarzschild black string perturbed by a
Gregory–Laflamme (GL) instability. Focus on radiation generated during the
initial nonlinear formation of large bulges, **before** the later neck cascade.

**This directory is an independent research workspace, not a production
simulation branch.** The authoritative repository is
https://github.com/inaeyk/GL-with-AI. Numerical implementation/GRChombo and
production simulations remain Michael's responsibility, under separately
approved plans. No production spacetime simulations are authorized here.

## Start here

Begin with [AGENTS.md](AGENTS.md) and the concise [HANDOFF.md](HANDOFF.md).
Use `research/sav/` as the Codex working directory so the scoped contract
is discovered. GOAL-001/002/003/004 are completed MEANINGFUL_PARTIAL.
**GLR-GOAL-005 is now owner-authorized and ready for Codex, but not launched.**
It prioritizes a validated physical L10 GL eigenfunction, conserved
quadratic sources and a source-evaluated, horizon-regular n=2/full M2
geometry. The first-bulge physical energy fraction remains open.
Savdeep or Michael may execute as sole designated research writer.
Historical prompts are retained for provenance, not
automatic replay. Codex discovers instructions along the
Git-root-to-working-directory path; opening only the Git root does not
load this descendant contract. See [instruction discovery](https://learn.chatgpt.com/docs/agent-configuration/agents-md).

1. [STATUS.md](STATUS.md): honest distinction between accepted engineering,
   numerical failures, continuum physics, conjectures, and research deliverables.
2. [NOTATION.md](NOTATION.md): coordinates, KK conventions, mode and energy normalization.
3. [SOURCES.md](SOURCES.md): frozen branch provenance and external-source references.
4. [literature/PAST_WORK.md](literature/PAST_WORK.md): critical, selective literature
   review, complementing Michael's reproduction and engineering notes.
5. [physics/so3_reduction.md](physics/so3_reduction.md),
   [physics/kk_channels.md](physics/kk_channels.md),
   [physics/early_amplitude_flux.md](physics/early_amplitude_flux.md),
   [physics/timescales_energy.md](physics/timescales_energy.md): theoretical foundation,
   with unverified steps explicitly flagged.
6. [numerics/extraction_contract.md](numerics/extraction_contract.md) and
   [numerics/validation_matrix.md](numerics/validation_matrix.md): **proposals** for
   future production extraction; not instructions to modify simulation code.
7. [agenda/RESEARCH_ROADMAP.md](agenda/RESEARCH_ROADMAP.md) and
   [agenda/NOVEL_APPROACHES.md](agenda/NOVEL_APPROACHES.md): longer-term
   classical, AI-assisted, quantum and holographic directions, feasibility-ranked.
8. [verification/INDEX.md](verification/INDEX.md): checks, negative results,
   proof obligations, and independent review status.
9. [goals/GLR-GOAL-001.md](goals/GLR-GOAL-001.md) and
   [goals/GLR-GOAL-001_REPORT.md](goals/GLR-GOAL-001_REPORT.md): executed
   goal, MEANINGFUL_PARTIAL; exact quadratic source and retarded integral,
   four numerical transfer estimates, unresolved rigorous enclosure and
   finite-A gates. `goals/LAUNCH.md` is historical.
10. [goals/GLR-GOAL-002.md](goals/GLR-GOAL-002.md),
    [goals/GLR-GOAL-002_REVIEW_GATES.md](goals/GLR-GOAL-002_REVIEW_GATES.md),
    and [goals/GLR-GOAL-002_LAUNCH.md](goals/GLR-GOAL-002_LAUNCH.md):
    historical task, acceptance rubric and launch prompt on finite-amplitude
    backreaction, admissible data, nonlinear amplitude and early radion energy.
    **Executed 2026-10-22: MEANINGFUL_PARTIAL.** See
    [GLR-GOAL-002_REPORT.md](goals/GLR-GOAL-002_REPORT.md): regular
    second-order equations/zero-mode quadratures and finite-data bounds;
    full n2 response, exact simultaneous GL data and finite-A remainder open.
11. [GOAL-003 report](goals/GLR-GOAL-003_REPORT.md): **completed
    MEANINGFUL_PARTIAL**. Explicit forced n=2 master,
    bounded regular inverse, local exact GL/scalar data, formal higher
    orders/memory and linear AH map, separate consistent radion EFT.
    Global fixed-charge data, physical M2, nonlinear radiation and finite-A
    closure remain open. [Controlling Goal](goals/GLR-GOAL-003.md) and
    [review gates](goals/GLR-GOAL-003_REVIEW_GATES.md) retain their scope;
    no subsequent Goal was launched as part of GOAL-003.
12. [GLR-GOAL-004](goals/GLR-GOAL-004.md),
    [review gates](goals/GLR-GOAL-004_REVIEW_GATES.md),
    [Sav-or-Michael runbook](goals/GLR-GOAL-004_RUN_BY_SAV_OR_MICHAEL.md)
    and [launch prompt](goals/GLR-GOAL-004_LAUNCH.md): owner-authorized,
    prepared extended analytical program for rigorous early-radiation
    control plus an independently qualified semi-analytic first-bulge
    model. **EXECUTED: MEANINGFUL_PARTIAL.**
    [Final report](goals/GLR-GOAL-004_REPORT.md) and
    [executed ledger](verification/GLR_GOAL004_CHECKS.md): physical-source
    diagnostics, new constraint/endpoint identities, computed one/two-mode
    waveform models with failed convergence, separate EFT pulse transport.
    No numerical physical A_star or first-bulge fraction; no automatic replay.
13. [GLR-GOAL-005](goals/GLR-GOAL-005.md),
    [review gates](goals/GLR-GOAL-005_REVIEW_GATES.md),
    [Sav/Michael runbook](goals/GLR-GOAL-005_RUN_BY_SAV_OR_MICHAEL.md)
    and [launch prompt](goals/GLR-GOAL-005_LAUNCH.md):
    **prepared / NOT EXECUTED**, validated positive-k GL and sourced
    quadratic Einstein response. Earlier Goals remain closed.
14. [handoffs/MICHAEL_START_HERE.md](handoffs/MICHAEL_START_HERE.md): two-way collaboration.

## Scientific priorities

- Establish exact SO(3) reduction and gauge-invariant radiative content.
- Solve and normalize the linear GL growing mode with admissible horizon and
  asymptotic conditions.
- Preserve the GOAL-001 **quadratic n=0 radion source**, gauge completion,
  retarded transfer and strong numerical nonzero-overlap evidence as
  mathematically audited input, subject to remaining interval/error checks.
- Preserve GOAL-003's executed master/inverse and local-data results, with
  formal higher orders and open global/nonlinear gates. A controlled finite
  GL amplitude window and first-bulge emitted percentage remain unestablished.
  GOAL-004 has now executed with an honest partial result;
  read the [later review qualifications](verification/GOAL003_REVIEW_QUALIFICATIONS.md)
  before upgrading any inverse/verification claim.
- Keep realistic radion stabilization as a separate EFT, with its own
  matched source, stress tensor and massive scalar timelike flux. SO(3)
  allows scalar monopoles but forbids massless tensor Bondi news.
- Estimate early growth, nonlinear onset, bulge times, spectrum, and total
  emitted energy with explicit conditional assumptions and errors.
- Provide an independently auditable extraction specification for future
  simulations, including massive KK dispersive energy transport.

## Governance

Only add to or change `research/sav/**` on `research/sav-radiation` unless
explicitly authorized otherwise. No merges, production code, physics parameter
changes, dependency edits, or numerical launches. See [AGENTS.md](AGENTS.md).
Historical failed results are scientifically valuable and retained.

