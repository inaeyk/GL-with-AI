# Milestone 1-C distributed ghost and boundary ownership

Date: 2026-07-31

Source checkpoint: `1dc0b37`

Result: `M1-C PASS — distributed ghost and boundary ownership qualified`

This report is the retained evidence for the bounded M1-C manufactured-seam
fixture. `research_plan/stage_checklists.md` remains the current-status
authority. No CCZ4 equation, gauge row, KO formula, radial extrapolation
formula, parameter meaning, or external dependency changed.

## Production paths and one repair

The fixture uses the real ownership sequence:

1. `GRLevelData::exchange(Interval(0,17))` performs Chombo intralevel and
   periodic exchange;
2. `BlackStringPerturbativeRadialBoundary::fill_solution_ghosts` fills the
   global low and high radial strips, including radial-periodic corners;
3. `BlackStringLive::make_pointwise_input` applies the live
   `FourthOrderDerivatives` first, second, and mixed stencils to all 18 slots;
4. `BlackStringKODissipation::add_from_input` applies the locked seven-point
   KO stencil in compile-time directions 0 and 1;
5. `BlackStringPerturbativeRadialBoundary::apply_outer_rhs` selects the
   global outer valid surface.

The first serial preflight completed the compact-seam case, then reproduced
a genuine radial-decomposition defect. On an internal radial box, the
provisional filler attempted `Box::setBig` or `Box::setSmall` before testing
whether the grown box intersected a physical radial strip. Chombo therefore
asserted while constructing an empty selection.

The single M1-C repair adds an early nonintersection skip before changing the
temporary selection box. It changes only surface ownership. The five
extrapolation sources, all extrapolation weights, GP subtraction, three
ghost layers, component loop, and fill order are unchanged. The repaired
radial-only and combined layouts demonstrate that internal radial boxes now
receive framework exchange data and no physical-boundary work.

## Global field and reference

All layouts use the same global problem:

| Quantity | Value |
|---|---:|
| `CH_SPACEDIM / GR_SPACEDIM / DEFAULT_TENSOR_DIM` | `2 / 4 / 4` |
| `N_x, N_z` | `32, 32` |
| `dx=dz` | `1/8` |
| valid radial faces | `x=0.5` and `x=4.5` |
| compact length | `L_z=4` |
| ghost depth | 3 |
| periodic direction | compact `z` only |
| components | 18 |
| KO coefficient | 0.3 |

For component \(c\), the deterministic field is

\[
U_c=b_c+0.03(c+1)\left[
0.31X+0.17X^2+0.11X^6+
(0.73+0.23X+0.09X^2)P_c(z)\right],
\]

where \(X=(i+1/2)/32\). The five one-\(z\)-index slots
`hxz`, `Axz`, `GammaZ`, `shiftZ`, and `Bz` use
\(P_c=\sin\theta+0.27\sin2\theta\) and \(b_c=0\). The other 13 slots use
\(P_c=\cos\theta+0.19\cos3\theta\) and a nonzero even base. Thus all slots
exercise nonzero radial and compact derivatives and nonzero radial, compact,
and additive mixed KO responses.

The reference evaluator is independent of the production helper. It applies
the published centered fourth-order coefficients directly to the analytic
manufactured value and applies the seven KO coefficients directly in each
direction. The production KO helper is also evaluated with fixture-only
direction projections to expose its radial and compact contributions; its
ordinary combined result must equal their sum. The derivative comparison
uses valid cells whose radial stencil is wholly inside the physical domain.
Periodic compact stencils include both global periodic ends.

Only one resolution is used, so no empirical convergence rate is claimed.
The derivative stencil's expected order is four; M1-C tests
decomposition/reference equality at fixed resolution.

## Layout and ownership maps

| Layout | Boxes in global index space | MPI owner map | Physical radial boxes |
|---|---|---|---|
| compact seams | four `32 x 8` boxes split at `z=8,16,24` | `0,1,0,1` | all four touch both radial faces |
| radial seams | four `8 x 32` boxes split at `x=8,16,24` | `0,0,1,1` | box 0 inner; box 3 outer |
| combined seams | four `16 x 16` boxes split at `x=16,z=16` | `0,1,2,3` | boxes 0/1 inner; boxes 2/3 outer |

Every physical-edge box has 8, 16, or 32 radial valid cells, exceeding the
five-cell closure footprint. Three ghost layers cover both the fourth-order
radius-two derivative stencils and the radius-three KO stencil.

The distributed ownership evidence is:

| Layout | Valid cells/rank | Inner ghost boxes by rank | Outer ghost boxes by rank | Outer-RHS boxes by rank |
|---|---|---|---|---|
| compact, MPI-2 | `512,512` | `2,2` | `2,2` | `2,2` |
| radial, MPI-2 | `512,512` | `1,0` | `0,1` | `0,1` |
| combined, MPI-4 | `256,256,256,256` | `1,1,0,0` | `0,0,1,1` | `0,0,1,1` |

For every layout the global valid-cell sum is 1,024 and every valid cell has
one box owner. The fixture classifies every depth-three ghost geometrically:
ghosts whose radial index remains in the global domain are framework-owned;
ghosts outside it are radial-policy-owned. These sets are disjoint.

| Layout | Framework-owned ghost cells | Radial-owned ghost cells | Radial-periodic corners | Wrong owner | Unfilled | Valid overwrites |
|---|---:|---:|---:|---:|---:|---:|
| compact | 768 | 336 | 36 | 0 | 0 | 0 |
| radial | 876 | 228 | 36 | 0 | 0 | 0 |
| combined | 648 | 264 | 36 | 0 | 0 | 0 |

Every framework-owned ghost matches the periodic/global manufactured value
in all 18 slots. Every radial ghost matches an independent GP-subtracted
five-point extrapolation reference. The radial-periodic-corner comparison
uses periodic source values, proving that exchange completed before radial
corner filling. No internal radial seam or compact seam invokes physical
radial work.

## Derivative and KO seam results

The serial and distributed error magnitudes and norms agree. All first,
second, and mixed derivative comparisons are exactly zero. KO discrepancies
are roundoff from the independently ordered reference sum.

| Layout | Operator | Seam samples | Seam `L_inf` | Seam weighted `L_2` | Worst slot/cell |
|---|---|---:|---:|---:|---|
| compact | `dz`, `dzz`, `dxz` | 7,488 each | 0 | 0 | none |
| compact | KO `z` | 11,232 | `2.664697889426e-16` | `1.214718722947e-15` | `Bx (26,31)` |
| compact | KO `x+z` | 11,232 | `2.710505431214e-20` | `4.496836223195e-20` | `Bx (20,0)` |
| radial | `dx`, `dxx` | 6,912 each | 0 | 0 | none |
| radial | `dz`, `dzz` periodic end | 1,872 each | 0 | 0 | none |
| radial | `dxz` | 7,920 | 0 | 0 | none |
| radial | KO `x` | 10,368 | `2.664535259514e-16` | `1.169288533728e-15` | `shiftX (9,1)` |
| radial | KO `z` | 2,808 | `2.664697889426e-16` | `6.937509717022e-16` | `Bx (26,31)` |
| radial | KO `x+z` | 11,232 | `2.710505431214e-20` | `4.386966458737e-20` | `Bx (20,0)` |
| combined | `dx`, `dxx` | 2,304 each | 0 | 0 | none |
| combined | `dz`, `dzz` | 3,744 each | 0 | 0 | none |
| combined | `dxz` | 5,472 | 0 | 0 | none |
| combined | internal-corner `dxz` | 576 | 0 | 0 | none |
| combined | KO `x` | 3,456 | `2.664535259514e-16` | `6.796558144011e-16` | `shiftX (16,2)` |
| combined | KO `z` | 5,616 | `2.664697889426e-16` | `8.263296980606e-16` | `Bx (26,31)` |
| combined | KO `x+z` | 7,776 | `2.710505431214e-20` | `3.623967860818e-20` | `Bx (20,0)` |

Across each global stencil-safe set there are 14,976
cell-component comparisons per operator. The largest global KO discrepancies
are `2.664535259514e-16` in `x`,
`2.664697889426e-16` in `z`, and
`2.710505431214e-20` in the additive result. The reported global KO-`x`
worst provenance can differ between serial and MPI because several cells tie
at the same maximum; the error magnitude and weighted norm are identical.

All 18 rows pass. The normal KO path requests exactly direction masks 0 and
1, and the live derivative source uses only the two compile-time production
strides. No direction 2 or 3 is accessed.

## Mutation gates

The combined serial mutation runner changes the live fixture operations, not
metadata:

| Mutation | Real path changed | Result |
|---|---|---|
| internal radial seam treated as physical | production radial filler receives the internal `x=8` plane as a fake physical edge | rejected by ghost/reference mismatch |
| exchange omitted | live fourth-order seam derivative reads poisoned ghosts | rejected as nonfinite |
| last KO component omitted | production KO policy runs only 17 component rows | rejected by nonzero slot-17 response |
| hidden direction used | audited live fourth-order stencil requests direction 2 | rejected before any invalid stride read |

## Builds, launches, and scope

Both fixture variants compile with
`-std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror`:

- serial executable SHA-256:
  `25b26dc2feb5b6f40159b58abbca8ec8fbfb2fa89ed47b01ff8d40307c0de525`;
- MPI executable SHA-256:
  `f54e87056f5c6db5a223c27c8d3797184dd8bdc77ea9458414e69dc99f826162`.

The separate project-owned warning-negative compile also passed: an unused
project parameter remains fatal under the same warning set.

The hard cap was respected with five fixture process launches:

1. pre-repair serial preflight, which reproduced the internal-radial-box
   assertion after completing the compact layout;
2. repaired serial reference for all three layouts plus the combined mutation
   runner;
3. repaired compact-seam MPI-2;
4. repaired radial-seam MPI-2;
5. repaired combined-seam MPI-4.

One sandboxed `mpirun` attempt failed in PMIx before starting any fixture
rank; it is an execution-environment denial, not a numerical launch. The
successful serial reference took 0.11 seconds and 6,096 KiB peak RSS. The
tool-observed MPI fixture wall times were approximately 0.19, 0.10, and
0.17 seconds; these startup-dominated values are not scaling evidence.

No evolution, HDF5/restart, AMR refinement, perturbation, Fourier/spectral
analysis, horizon work, or unrelated fixture ran.

## Decision

The real Chombo exchange, live fourth-order derivative assembly, project KO
helper, radial ghost policy, and outer valid-surface ownership are
decomposition-safe for the three required manufactured layouts after the one
scoped ownership repair.

`M1-C PASS — distributed ghost and boundary ownership qualified`

Audit M1-ABC may begin and should cover M1-A through M1-C together.
