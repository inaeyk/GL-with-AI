# Time scales, energy estimates, and early-versus-late hypotheses

**Status:** literature numbers + conditional models. These do NOT constitute
a computed early radiated energy.

## Critical wavelength and harmonic thresholds

Five-dimensional uniform string critical `k_cr r0~0.876`
(`L_cr/r0~7.17`). For fundamental k1 and second harmonic k2:

| L/r0 | k1*r0 | k2*r0 | initially unstable positive harmonics |
|---:|---:|---:|---|
| 8 | pi/4 = 0.7854 | pi/2 = 1.5708 | n=1 |
| 10 | pi/5 = 0.6283 | 2pi/5 = 1.2566 | n=1 |
| 12 | pi/6 = 0.5236 | pi/3 = 1.0472 | n=1 |
| 16 | pi/8 = 0.3927 | pi/4 = 0.7854 | n=1 and n=2 |

Sign-degenerate negative n included by reality.
Numbers from k=2 pi n/L and the independently published threshold
(e.g. Gregory–Laflamme; additional spectral checks in GOAL-001).
DO NOT confuse GL unstable wavenumber with on-shell massive radiative
frequency threshold |omega|>|k_n|.

## Growth and first nonlinear onset

Let `Omega_hat(k)=Omega_GL(k)*r0` and
`A(t)=epsilon*p1*exp(Omega(t-t0))`. Then

```text
t_GL/r0 = 1/Omega_hat ;
(t_NL-t0)/r0 = [log(A_star/(epsilon |p1|))]/Omega_hat ;
Delta t_NL (epsilon -> epsilon/10) = (ln 10)/Omega_GL .
```

A_star is an explicitly defined geometric O(1) nonlinearity threshold;
epsilon alone is a coordinate perturbation if chosen in chi.
No accurate numerical Omega_hat is adopted here: Goal must solve its
own normalized GL ODE and compare with independent published curves.

For the 2023 GRChombo simulations
(https://arxiv.org/html/2210.13501v2, Table 2):

| L/r0 | first growth-rate maximum t_p/r0 | first formation t_n/r0 | t_n-t_p |
|---:|---:|---:|---:|
| 8 | 163.1 | 174.7 | 11.6 |
| 10 | 93.0 | 107.0 | 14.0 |
| 12 | 78.0 | 96.4 | 18.4 |
| 16 | 91.0 | 112.7 | 21.7 |

These geometrical events are initial-data/gauge-dependent windows, not
direct gravitational-wave burst times. Proper waveform arrival requires
retarded propagation and a field-derived time reference. The paper does
**not** find a universal timescale for subsequent generations.

## Radiation energetics

Mass `M0=r0/(2G4)`; massless radion:
`dE0/du=Phi_dot^2/G4`,
`E0/M0=(2/r0)int du Phi_dot^2`.
If `Phi=r0*C0*A^2` holds for a well-specified pure growing mode and
A<<1, `(E0/M0)_(A1 to A2)=2*(Omega*r0)*|C0|^2*(A2^4-A1^4)`.
Unknown `C0` and breakdown at A~1 prohibit a predicted fraction.

For an explicitly **toy** sigmoid response
`Phi=Q*r0/[1+exp(-(u-u*)/tau)]`,
direct integration gives `E0/M0 = (Q^2/3)*(r0/tau)`;
its spectrum in the news-Fourier convention is
```text
dE0/domega = (Q*r0)^2/(pi*G4)
                 [pi*tau*omega/sinh(pi*tau*omega)]^2 .
```
This is illustrative only; Q and tau must be determined from dynamics.
The scalar Cauchy–Schwarz bound for net coefficient change DeltaPhi in T:
`E0 >= (DeltaPhi)^2/(G4*T)`. Without DeltaPhi, no nontrivial lower bound.

## Early dominance / late cascade hypotheses

Figueras et al. III.1 conjecture a first large-scale GW burst, with
less emission from smaller generations (not a measurement). Under the
**extra assumptions** that an individual localized generation carries
mass/available energy `E_i~N_i eta_i R_i^2/G5`, its ratio is
`E_{i+1}/E_i ~ (N_{i+1}eta_{i+1}/N_i eta_i)*(R_{i+1}/R_i)^2`.
For L=10 their illustrative radii 2.04 r0 and 0.299 r0 yield
`(0.299/2.04)^2~0.0215`. The radius ratio is not a measured
radiative efficiency; thin segments carry energy scaling with R_i*ell_i,
so this argument may fail. A shortening scale can open many massive KK
frequencies and create a high-frequency spectral tail. No universal
geometric cascade is supported by the 2023 simulations.

A future observable should report both cumulative
`E_early/M0` (independently meaningful) and the eventual
`E_early/E_total` (requires late-emission bound or measurement).
The first-bulge cutoff must be specified by an invariant geometric event
and corrected for retarded propagation. A finite causal early window
can be physically useful without simulating pinch-off.
