# Verification index — scientific foundation

Date: 2026-10-22. Evidence categories:
`CHECKED_SIMPLE_ALGEBRA`, `SOURCE_CONFIRMED`,
`DERIVED_UNAUDITED`, `NOT_ATTEMPTED`, `FAILED`.

| Check | Status | Evidence / limitation |
|---|---|---|
| cos(kz)^2 selection n=0 and +/-2 | CHECKED_SIMPLE_ALGEBRA | SymPy 1.14.0, Python 3.13.5, `check_foundation.py`. |
| Sigmoid charge-transition energy `E/M=Q^2*r0/(3*tau)` | CHECKED_SIMPLE_ALGEBRA | Beta-function integral 1/6; not a physical GL result. |
| Static KK mass/tension charge algebra | CHECKED_SIMPLE_ALGEBRA | Solved 2 linear relations; assumes stationary asymptotic charges. |
| Figueras et al. early-burst hypothesis | SOURCE_CONFIRMED | 2210.13501v2 III.1 explicitly conjectural. |
| M2-C blocked, one GP run and 11 not attempted | SOURCE_CONFIRMED | Frozen campaign closure, raw manifest and acceptance JSON. |
| SO(3) reduction action and Euler–Lagrange equations | DERIVED_UNAUDITED | Check warped-product curvature, signs, boundary terms, Hamiltonian dof count. |
| 4D radion news normalization from 5D asymptotics | DERIVED_UNAUDITED | Check 4D EMD/Bondi flux vs 5D symplectic/Bondi law. |
| Linear GL physical eigenfunctions `Omega(k)` | NOT_ATTEMPTED | Recompute eigenfunctions and reproduce published threshold/cases. |
| Full quadratic n=0 source | NOT_ATTEMPTED | Includes gauge, constraints and all components. |
| Nonzero outgoing projection coefficient C0 | NOT_ATTEMPTED | May vanish! Require clean retarded Green-function computation. |
| Massive KK canonical energy flux | NOT_ATTEMPTED | Threshold is known; norm and physical master current are not. |
| Full numerical GRChombo/AMR waveform extraction | NOT ATTEMPTED | Explicitly prohibited as production work in this research track. |

The four checked source/algebra claims are narrow. There has been
**no independent symbolic verification of the full Einstein expansion** and
no production numerical runs. Record exact output, code version, and
proof scope before upgrading any result. Source-backed literature claims
are not automatically proofs of our equations.

## Research-level acceptance protocol

For a nontrivial new identity or coefficient: (1) analytic derivation;
(2) independent derivation in a different parameterization or gauge;
(3) machine check with exact symbolic identities and deterministic
positive/negative tests; (4) adversarial mathematical reviewer not relying
on the first derivation; (5) field normalization and dimensional checks.
Retain failed attempts and boundary-condition obstructions in separate logs.
