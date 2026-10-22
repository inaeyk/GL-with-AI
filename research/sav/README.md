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
   [goals/LAUNCH.md](goals/LAUNCH.md): ambitious ready-to-start Codex goal,
   not launched by creation of these files.
10. [handoffs/MICHAEL_START_HERE.md](handoffs/MICHAEL_START_HERE.md): two-way collaboration.

## Scientific priorities

- Establish exact SO(3) reduction and gauge-invariant radiative content.
- Solve and normalize the linear GL growing mode with admissible horizon and
  asymptotic conditions.
- Obtain the **complete quadratic n=0 radion source**, including second-order
  gauge/constraint issues, its retarded transfer coefficient, and normalized
  physical flux. The outgoing coefficient may vanish; this is a question.
- Estimate early growth, nonlinear onset, bulge times, spectrum, and total
  emitted energy with explicit conditional assumptions and errors.
- Provide an independently auditable extraction specification for future
  simulations, including massive KK dispersive energy transport.

## Governance

Only add to or change `research/sav/**` on `research/sav-radiation` unless
explicitly authorized otherwise. No merges, production code, physics parameter
changes, dependency edits, or numerical launches. See [AGENTS.md](AGENTS.md).
Historical failed results are scientifically valuable and retained.
