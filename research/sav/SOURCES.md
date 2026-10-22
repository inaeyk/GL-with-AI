# Source and provenance register

## Repository, read-only sources reviewed 2026-10-22

Authoritative repository: https://github.com/inaeyk/GL-with-AI

| Branch | Observed head | Role |
|---|---|---|
| main | 7d04b5b9882dcd476c1457b8d711ac7b5520b2c1 | repository baseline + historical checks |
| supervised/m2b-cycle1 | b5970454a649f8cbf46f694ebddcfe191b657530 | accepted M2-B focused implementation/evidence |
| backup/m2c-v2-frozen-campaign-20260917 | 7b4710b8d1ecb791de10656a5a34b9b959165f26 | frozen M2-C failed scientific-acceptance attempt |

Core audited records:
- `main:research_plan/stage_checklists.md`: active claims and milestone gates.
- `main:research_plan/reproduction_plan.md`: benchmark target and proposed
  (not current accepted) simulation setup.
- `main:research_plan/radiation_profile_plan.md`: placeholder, not a
  completed extraction theory.
- `main:docs/derivations/milestone2a_radial_characteristic_derivation.md`:
  reduced characteristic design, subject to physical validation.
- `main:logs/blackstring_d16_constraint_spectrum.log`: non-invariant
  discrete constraint-nullspace projection.
- `supervised/m2b-cycle1:docs/grchombo/milestone2b_cycle1_evidence.md`:
  accepted bounded fixtures, GP, performance/restart/seed evidence and limits.
- `backup/m2c-v2-frozen-campaign-20260917:docs/grchombo/milestone2c_strategy_closure.md`:
  GP failure and blocked scientific acceptance.
- `backup/m2c-v2-frozen-campaign-20260917:run_manifests/m2c/acceptance_summary.json`
  plus retained logs: failed launch and attempt accounting.

The source inspection was selective, not a full rerun or line-by-line
independent validation of CCZ4. Literature search found **no dedicated prior
historical/physics literature survey** in the repository research plans and
derivation index. This is a bounded absence claim, not an exhaustive census.

## Primary public references with stable identifiers

- Gregory–Laflamme (1993): https://doi.org/10.1103/PhysRevLett.70.2837
  https://arxiv.org/abs/hep-th/9301052
- Gregory–Laflamme details (1994):
  https://arxiv.org/abs/hep-th/9404071
- Gubser (2002), non-uniform perturbative strings:
  https://arxiv.org/abs/hep-th/0110193
- Sorkin (2004), critical dimension:
  https://doi.org/10.1103/PhysRevLett.93.031601
- Kudoh (2006), spherical GL-mode classification:
  https://doi.org/10.1103/PhysRevD.73.104034
- Lehner–Pretorius (2010), nonlinear cascade:
  https://doi.org/10.1103/PhysRevLett.105.101102
- Figueras, França, Gu, Andrade (2023), nonlinear comparison:
  https://arxiv.org/html/2210.13501v2 and
  https://doi.org/10.1103/PhysRevD.107.044028
- Collingbourne (2021), rigorous linear GL mode:
  https://doi.org/10.1063/5.0043059
- Lü, Mao, Wu (2019), KK scalar news/Bondi structure:
  https://arxiv.org/abs/1909.00970 and
  https://doi.org/10.1007/JHEP11(2019)005
- Harmark–Obers (2004), KK tension:
  https://arxiv.org/abs/hep-th/0403103
- Emparan et al. (2010), blackfold effective theory:
  https://doi.org/10.1007/JHEP04(2010)046

Read original equations and normalizations before using them as input to
symbolic computation; these references are a starting bibliography, not a
substitute for derivations.

## GLR-GOAL-001 execution provenance and inspected scope

Execution input: `research/sav-radiation@0b0b2e1ee1df64a80e4474d6909173d4682564f3`.
All controlling research notes, goal, verification records and Michael
handoff were retrieved at that immutable commit. The local desktop began
with an empty project Git repository; research files were materialized
without checking out/changing another branch or importing production files.
Result commits use the connected GitHub Git-tree API on the named research
branch, preserving all other paths and checking the branch-head lease.

Historical read-only reinspection: main stage checklist and D16 raw log
at `7d04b5b9882dcd476c1457b8d711ac7b5520b2c1`, M2-B evidence on the
named supervised branch, and frozen M2-C closure plus acceptance JSON.
They preserve D16's failed invariance and M2-C's failed GP control. Their
engineering test authority was not reused as continuum physics proof.

Primary literature newly inspected (2026-10-22):

- [GL1993](https://arxiv.org/html/hep-th/9301052), equations8–11:
  TT physical mode, horizon/infinity branches and scaling. GL1994
  [PDF](https://arxiv.org/pdf/hep-th/9404071), Figure3 and r+=2 convention
  were inspected by the linear investigator; screenshot failure prevented
  a precise digitized numerical growth benchmark. Do not claim one.
- [Collingbourne](https://arxiv.org/html/2007.08441), §§3.2,3.4,4.1:
  scalar master, admissible solution and gauge reconstruction. Candidate
  v1 denominators were checked against direct Ricci equations rather than
  copied; exact source derivations and error estimates are ours. The
  explicit literature lower bound Omega*r0>1/(20sqrt10) over
  0.3<=k*r0<=0.8 is a separate consistency check, not pointwise data.
- [Kudoh2006](https://arxiv.org/html/hep-th/0602001), perturbation
  classification and exceptional scalar l=0 section; no numerical C0
  imported. [Gubser2002](https://arxiv.org/abs/hep-th/0110193) inspected
  at abstract scope only during this run: static perturbative branch,
  not a time-dependent waveform calculation.
- [FFG&A2023](https://arxiv.org/html/2210.13501v2), III.1/Table2
  rechecked: first-bulge timing is geometrical; early radiation remains a
  conjecture. [Lehner–Pretorius2010](https://arxiv.org/html/1006.5960)
  nonlinear-cascade discussion, not a radion transfer benchmark.
- [Lü–Mao–Wu2019](https://arxiv.org/html/1909.00970v2), EMD/Bondi
  structure and 5D lift consulted as context; our 4D/5D flux coefficient
  was independently derived. [Harmark–Obers2004](https://arxiv.org/html/hep-th/0403103)
  charge/tension definitions consulted; stationary charges do not replace
  the dynamic scalar source.

No exhaustive novelty search or claim is made. The new contribution here
is the explicitly completed perturbative scalar source, exact checks,
retarded normalization and reproducible bounded numerical evidence.
Two separate AI derivational audits are preserved with their limitations;
they are not external human reviews. Result/closing commit references are
recorded in `goals/GLR-GOAL-001_REPORT.md` and the final task delivery.

Pinned executed scientific result:
`7110bc2cb3c0496dd62dc97d37b34d11ec780a17` (33 research-only blobs,
no deletions/outside changes, verified against the input tree).

