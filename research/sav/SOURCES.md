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
