# Michael Yan / AI environment — GL radiation science handoff

2026-10-22. **Science-only interface; no implementation or simulation
action requested.** Savdeep leads this analytical thread jointly with
Michael; Pau Figueras advises. Michael retains numerical responsibility.

## Result and provenance

GLR-GOAL-001 reached **MEANINGFUL_PARTIAL**. The complete quadratic n=0
radion equation and its retarded transfer functional are derived and
passed two separate adversarial AI mathematical reviews. Four bounded
continuum calculations strongly support positive outgoing overlap.
A rigorous finite-k interval proof and a controlled finite-A energy
fraction remain open. See [the report](../goals/GLR-GOAL-001_REPORT.md).

Input research commit: 0b0b2e1ee1df64a80e4474d6909173d4682564f3.
Scientific result commit: **7110bc2cb3c0496dd62dc97d37b34d11ec780a17**
on research/sav-radiation. Only research/sav/** changes.
Your main, supervised M2-B and frozen M2-C branches, production
formulation/parameters and GRChombo setup were not modified.
No production runs, cluster submissions or merges occurred.

Accepted engineering remains separate from physics acceptance. D16's
projected constraint nullspace was non-invariant; frozen M2-C's GP control
failed at t/r0=2.5, with later11 launches uninvoked. The new continuum ODE
is independent and does not qualify that production path.

## Scientific quantities and normalization

For a genuine pure n=±1 growing SO(3)-singlet mode, use the unique
nonzero-k unitary completion hU_zA=0. Normalize Htheta theta/(2r²)=1 at
background areal radius2r0, and write hU=A(t)H(r)cos(kz),
A=epsilon p1 exp[Omega(t-t0)]. A is not a numerical chi perturbation or
unspecified horizon amplitude. The exact master and two-gauge map are in
[gl_linear_mode.md](../physics/gl_linear_mode.md).

The completed second-order scalar is phi0=sqrt(3)<qUzz>/4, with no
factorial in the metric-q expansion. Its full source is
\[
\Box_4\phi_0=-\frac{\sqrt3k^2}{16}A^2
(H_{tt}^2/f^2+f^2H_{rr}^2+2H_{\theta\theta}^2/r^4-2H_{tr}^2).
\]
The minus tr-square sign and horizon cancellations are essential.
Raw finite-radius average log(gzz) is not this completed field.
At infinity the nonzero KK modes decay, and the matched Bondi-Einstein
scalar obeys phi0=Phi(u)/rho+..., Nphi=dPhi/du.
Physical outputs are P0=Nphi²/G4 and E0/M0=(2/r0)int Nphi²du.
There is no spherical zero-mode tensor/photon news.

For this amplitude and r_*=r+r0 log(r/r0-1) convention:

| L/r0 | Omega*r0 | C0 estimate in Phi=r0 C0 A(u)² |
|---:|---:|---:|
| 8 | 0.02645870 | 28.2964 |
| 10 | 0.06328119 | 12.30729 |
| 12 | 0.08044276 | 7.02554 |
| 16 | 0.09161923 | 3.32104 |

These are empirically converged coefficients, not interval-certified
numbers or emission fractions. C0 rescales quadratically with mode
normalization. Independent L10 quadrature agrees within7e-8.
The exponential law isolates a particular retarded response; finite-time
constrained data can add outgoing scalar transients. Arbitrary switching
of the linear GL metric violates its vacuum constraints.

## Concrete future extraction data for scientific discussion

An offline perturbative comparison would need:

1. Background r0,L,G5, asymptotic clock, actual slicing/coordinate map,
   invariant sphere radius and compact period/phase, plus commit/data
   provenance. GP time/radial coordinates require their actual map.
2. Fundamental complex Fourier metric components and derivatives sufficient
   for the invariant unitary tensor, plus zero-mode second-order data.
   In a general gauge the completion contains **second derivatives** of
   first-order hzz; metric plus first derivatives alone are not generally
   enough without on-shell master reconstruction or equivalent gauge
   information. An invariant master and reconstruction can replace that
   additional derivative input in the pure linear regime.
3. Radial/time derivative information or equivalent canonical data,
   constraints/parity checks, worldtube/retarded-time mapping, and an
   explicit scalar initial-data/transient prescription. Distinguish
   independent n=0 waves from the quadratic growing-mode response.
4. Radius/resolution error estimates on Phi and Nphi, stationary/gauge
   null controls, and a causal boundary-clean window. At finite radius use
   the completed scalar and Schwarzschild exterior response; do not use
   massless Sommerfeld extrapolation for every KK harmonic.

The scientific comparison would be normalization-consistent Phi/A²,
outgoing news, and interval energy with uncertainty. Mass/tension shifts
are distinct from time-dependent news. Massive n≠0 flux needs its own
normalization and timelike-worldtube propagation; it is excluded from
these numbers. The existing numerics/extraction_contract.md is an initial
proposal; the derivative/gauge requirements here refine its input contract.

No controlled numerical finite-A window was obtained: complete q2
metric/n=2 and next even-scalar norms and finite-data transients still need
bounds. These results give no reliable A~1 or first-bulge energy fraction.
FFG&A Table2 geometric windows are not waveform measurements.
The permitted four-k/three-refinement analysis allowance is used;
no automatic rerun or production repair is implied.

Next scientific work is validated overlap-error control, then regular-frame
second-order backreaction/transient bounds. Production formulation,
extraction implementation or simulation decisions require separate human
scientific authorization. This handoff requests no change to current work
or a running environment.
