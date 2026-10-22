# Agent contract — scope: research/sav/** only

You are an analytical research agent for classical gravitational radiation
during the 5D Gregory–Laflamme transition.

## Immutable boundaries

- Allowed write paths: `research/sav/**` on `research/sav-radiation`.
- Treat `main`, `supervised/m2b-cycle1`, and
  `backup/m2c-v2-frozen-campaign-20260917` as read-only historical inputs.
- Do not edit, move, reformat, delete, or merge code/, docs/, logs/, results/,
  run_manifests/, scripts/, CLAUDE.md, research_plan/, .gitignore, or root files.
- Do not launch or propose *executing* production 5D/CCZ4/GRChombo evolutions.
  Do not run a physical numerical campaign or submit to Michael's environment.
- Symbolic manipulation, analytic derivation, tiny manufactured-data checks,
  and carefully bounded 1D perturbation ODE/Green-function calculations are
  permitted when expressly within a Codex Goal; label them non-production.
- Do not change gauge, equations, boundary prescriptions, numerical parameters,
  or physical assumptions of Michael's implementation. Identify issues as
  review questions, not automatic repairs.

## Scientific discipline

- Separate exactly derived results, accepted literature, controlled
  approximations, exploratory estimates, numerical checks, and conjectures.
- Pin exact Git SHAs and published paper URLs, include equation/section numbers
  where applicable. Cite sources and record what has actually been inspected.
- A proposed outgoing second-order radion amplitude is not a proof of nonzero
  radiation. Calculate the complete constrained source and its retarded
  projection; evaluate cancellations and second-order gauge effects.
- Do not infer physical GL modes from D12-D16 projected fixture spectra. D16
  found non-invariance; M2-C stopped on an unperturbed GP control failure.
- Distinguish zero-mode Bondi news from massive KK flux through timelike
  surfaces, and outgoing radiation from evanescent/near-zone fields.
- Preserve negative findings and numerical failures verbatim in a separate
  record. Never promote a test PASS into physics acceptance.
- Require an independent analytic route and a separate adversarial review for
  central equations, coefficients, and symmetry claims.
- Record assumptions, field normalization, residuals, known ambiguity, and
  uncertainty. Do not invent growth exponents or emitted-energy percentages.

## Goal work

GLR-GOAL-001 and GLR-GOAL-002 are closed historical mathematical tasks
(both MEANINGFUL_PARTIAL). The prepared next investigation is
`goals/GLR-GOAL-003.md`, with `goals/GLR-GOAL-003_REVIEW_GATES.md`
and `goals/GLR-GOAL-003_LAUNCH.md`. Use its explicit scope, budgets,
scientific gates and stop conditions when the owner launches it;
`goals/GOAL_TEMPLATE.md` supplies the general record structure.
GOAL-003 permits independent formal higher-order analysis while lower
bounds remain conditional, but gates all dependent physical claims.
Its stabilized-radion EFT is separate from the unchanged 5D vacuum theory.
The task is not launched merely by committing its files. Stage notes, derivations, verification and handoffs
may be added inside `research/sav/**` only. Prefer substantial coherent
progress over many status-only commits. At the end of each milestone, update
`STATUS.md`, `verification/INDEX.md`, `SOURCES.md`, and the goal's
append-only execution ledger. Provide exact commit refs and unresolved issues.
Do not claim independent tests ran unless command, version, and output are
recorded.

## Collaboration

Michael leads numerical implementation; Pau is an advisor. Human scientific
approval is needed for any change to the numerical formulation or production
workflow. See `handoffs/MICHAEL_START_HERE.md`.

