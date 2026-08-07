# M2-B outer-face authority repair

Date: 2026-08-07

This M2-C production repair cycle 1 makes one bounded change to the accepted
M2-B implementation: a physical radial domain may end at exactly
`x_out/r0=3.5` or `x_out/r0=4.5`, using the existing floating-point comparison
tolerance. The inner face remains exactly `x_in/r0=0.5`. Values 3.0, 4.0, and
5.0 are explicit negative controls. The default and existing M2-B parameter
files remain at 4.5.

## Precondition and production scope

A production/fixture source audit found no outer-face constant in the
characteristic equations, transforms, speed classification, nonlinear
reconstruction, three-layer ghost coefficients, seed construction, or MPI
ownership rules. The transform and classification use the local GP value
`b=sqrt(r0/x)`; reconstruction is pointwise; the boundary obtains its face
from the live domain; and the seed buffer is defined by a fixed cell count.
The only production obstruction was the domain-authority guard in
`SimulationParameters.hpp`.

Accordingly, the only production hot-path diff is the parameter-validation
predicate. No RHS, boundary surface loop, KO, gauge, source, allocation,
checkpoint, or dependency source changed. The characteristic surface work
remains `O(N_z)` and MPI seams remain exchange-only.

## Focused qualification

The retained source-matched evidence is in
`/home/inaeyk/researchrepo/evidence/GL-with-AI/m2c-v2-preflight/outer-face-repair-authoritative-final/`.
Both GP controls use `x_in=0.5`, `x_out=3.5`, `Lz=8`, `ko_sigma=0.3`, three
ghost layers, and MPI-2. The repair qualification uses the accepted M2-B
bounded smoke interval `t=0.0005` and its `dt_multiplier=0.001`; the unlaunched
M2-C v2 campaign remains frozen at CFL 0.05.

| Gate | Result |
|---|---|
| exact authority | 3.5 and 4.5 accepted; 3.0, 4.0, and 5.0 rejected with status 134 |
| reduced transform at 3.5 | residual `5.55112e-16`, rank 23, condition `26.9665 < 10^3` |
| classification | outward normal: 11 incoming, 7 outgoing, 5 glancing; inward normal reverses incoming/outgoing |
| Gamma/Z and algebraic gates | PASS |
| GP fixed point and three ghost layers | PASS |
| MPI ownership | 3.5 and 4.5 physical faces owned; internal seams exchange-only |
| unchanged 4.5 smoke | PASS |

| GP endpoint metric | medium 24x64, dx=dz=1/8 | fine 36x96, dx=dz=1/12 |
|---|---:|---:|
| maximum valid-state drift | `1.986861584564e-4` | `3.762592421408e-5` |
| maximum radial-ghost drift | `6.798469877431e-3` | `9.939782664414e-4` |
| maximum boundary drift | `1.986861584564e-4` | `3.762592421408e-5` |
| radial Nyquist fraction | `0.1646216717170` | `0.1145720783048` |
| determinant defect | `3.330669073875e-16` | `3.330669073875e-16` |
| weighted-trace defect | `8.881784197001e-16` | `4.440892098501e-16` |
| minimum metric eigenvalue | `0.9999981869195` | `0.9999996112510` |

Both endpoints are finite and metric-admissible. The fine control improves the
valid, ghost, boundary, and radial-Nyquist measures. No new boundary-localized
or grid-scale instability appears over the bounded qualification interval.
These controls are not M2-C Fourier or scientific-mode evidence.

## Reproducibility

Baseline before the repair is `97d56f5618d942d7f1a89a2160880e9b2baf6534`.
The binary diff over `code/BlackStringToy` has SHA-256
`6028c6cdefdd5e8ba2081c52654bff7c23feacc903ef28e6c7276ba402c41dd9`.
Dependencies remain GRChombo `37e659523830418b210acea1661dac0e00bb1b75`
and Chombo `8684f2e000106f1abadb72642e1d15351867f98f`.

The focused executables were built with:

```text
make -j2 all DIM=2 DEBUG=FALSE OPT=TRUE MPI=TRUE USE_HDF=FALSE \
  FC=gfortran-15 CXX=g++ MPICXX=mpicxx \
  XTRACONFIG=.M2C_OUTER_FACE_QUAL
```

The focused-gate executable SHA-256 is
`f9d84225fe5f891d94c28fb527f94322fb54fc96d84047302b21e86c3729a420`;
the live-boundary executable SHA-256 is
`3b6beaa584627a1e31e099024e442cb57070154da051a98902e25d9c58bd831e`.

Audit status: PASS. The fresh read-only focused audit found no substantive
issue and independently reproduced the source, evidence, and dependency
hashes.
