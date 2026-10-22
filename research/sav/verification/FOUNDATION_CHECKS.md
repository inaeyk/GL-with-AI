# Initial narrow algebra verification (2026-10-22)

Executed privately using Python 3.13.5, SymPy 1.14.0. Test source is
reproduced in `verification/check_foundation.py` on this branch.
Outputs verified:

```text
FOURIER_SELECTION: PASS
SIGMOID_ENERGY: PASS; beta_integral=1/6
STATIC_CHARGE_TENSION_ALGEBRA: PASS
```

**Scope 1: Fourier projection.**
`cos(kz)^2-(1+cos(2kz))/2=0`. Shows that a quadratic source
from a pure n=+/-1 real harmonic has n=0 and n=+/-2. Says nothing
about the physical value of the complete gravitational source.

**Scope 2: energy in a toy scalar-charge sigmoid.**
Let `Phi=Q*r0/(1+exp(-u/tau))`. Substituting
`y=exp(-u/tau)` gives
`int du (Phi_dot)^2=(Q*r0)^2/tau int_0^inf dy y/(1+y)^4`.
The latter integral equals 1/6. With
`E/M0=(2/r0) int du Phi_dot^2`, this becomes
`E/M0=(Q^2/3)*(r0/tau)`.
This verifies integration, NOT a GL waveform or physical choice Q,tau.

**Scope 3: stationary KK charge algebra.**
Solving `2c_t-c_z=4G4 M` and
`c_t-2c_z=4G4 M n_T` and inserting
`Phi=sqrt(3)c_z/4` gives
`Phi=G4 M (1-2n_T)/sqrt(3)`. This is a stationary-asymptotic
algebra relation, not a dynamical prediction.

### Unverified major obligations

No symbolic audit yet of the 5D-to-3D curvature reduction, 3D
Hamiltonian constraints, second-order gauge transformations, nonlinear
KK projections, source `S0`, outgoing transfer `C0`, or full 5D/Bondi
flux law. The Goal is specifically meant to resolve these, including
possible cancellations. No production spacetime simulation was run.
