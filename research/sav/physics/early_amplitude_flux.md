# Early radion source and normalized energy flux

**Status:** leading KK scalar action and formal second-order construction;
uncomputed physical Green-function coefficient. DO NOT quote E_rad percentage.

**GOAL-001 execution update:** this introductory note is superseded for
the source by `quadratic_radion_source.md`, for the retarded/flux derivation
by `radion_retarded_transfer.md`, and for numerical estimates by
`radion_transfer_numerics.md`. The complete scalar source is now derived
and independently audited, and four floating C0 estimates exist. A rigorous
finite-k overlap enclosure and controlled finite-A emitted fraction remain
open. The old "uncomputed" statements below describe the initial record.

## Linear field and second-order zero mode

Let `g=g_bg+A h_1 cos(kz)+A^2 h_2+...`,
`A=epsilon p_1 e^{Omega(t-t0)}` in a chosen invariant normalization
and constrained initial-data family. The exact second-order Einstein
equation is `E'[h_2]=-(1/2) E''[h_1,h_1]`.
The zero Fourier projection of the RHS is nonzero algebraically in
generic gauges but its **physical radiative projection** requires full
gauge/constraint treatment and may cancel.

A reproducible calculation must specify:
- first-order gauge choice and regular nonzero-k master variable;
- a second-order gauge-invariant zero-mode completion (include first-
  order gauge displacement terms);
- full nonlinear source from all metric components, not just delta g_zz;
- retarded/no-incoming boundary data and initial switching; and
- normalization matching between the master scalar and Einstein-frame
  asymptotic phi.

For n=0 the linear scalar propagates on the 4D Schwarzschild factor.
For the l=0 radial variable `psi=r phi_0`,
```text
[partial_t^2-partial_{r_*}^2+V_0(r)] psi = S_0,
r_* = r+r0 log(r/r0-1),
V_0=f(r)*r0/r^3, f(r)=1-r0/r .
```
The reduced nonlinear source `S_0` and its normalization are the
principal unknowns. Compute the retarded Green function through a
horizon-ingoing and infinity-outgoing basis (frequency/Laplace
continuation carefully justified); use at least two independent
derivations and Wronskian tests.

For a pure exponential forcing in a controlled interval, **if**
`Phi(u)=r0*C0(k r0,Omega r0)*A(u)^2+...` is the resulting outgoing
coefficient, then with our action conventions:
```text
d(E0/M0)/d(u/r0) = 8*(Omega*r0)^2*|C0|^2*A^4 + O(A^6),
E0(u1,u2)/M0 = 2*Omega*r0*|C0|^2*(A2^4-A1^4) + higher orders.
```
This formula assumes the C0 response has settled, no significant
independent transient, and A<<1. Initial switching or time dependence
of C0 makes the complete convolution necessary. Even if computed, these
expressions **cannot** be extrapolated to A~1 without further theory.

## Gauge-invariant flux at null infinity

For a z-independent zero mode use
`ds5^2=e^{-2phi/sqrt3}ds4E^2+e^{4phi/sqrt3}(dz+2A)^2`.
With uncharged A=0, the action contains
`S_phi=-(8 pi G4)^(-1) int sqrt(-g4E)(nabla phi)^2`.
At 4D Bondi infinity,
`phi_0=Phi(u)/rho+O(rho^-2)`;
the stress tensor yields
```text
dE0/du=(partial_u Phi)^2/G4
E0/M0=(2/r0) int du (partial_u Phi)^2
dE0/domega = (omega^2/(pi G4)) |Phi_tilde(omega)|^2, omega>0 .
```
For nondecaying scalar memory, transform `N_phi=partial_u Phi`:
`dE0/domega=|N_tilde(omega)|^2/(pi G4)`, omega>0,
with Fourier transform `int du exp(i omega u) f(u)`.
The full Bondi mass law must be independently derived from the 5D metric
and asymptotic Einstein-frame gauge: local flux is a physical, invariant
observable only after the asymptotic conditions are fixed.

## Bounding versus guessing

For a finite retarded-time interval T with scalar coefficient change
`DeltaPhi`, Cauchy–Schwarz implies
`E0 >= (DeltaPhi)^2/(G4 T)`, conditional on measured/derived endpoint
coefficient and no omitted time tails. No general positive lower
bound can be claimed before demonstrating nonzero scalar news; no
unconditional subpercent upper bound is established.

Massive KK modes need their *own* canonical symplectic energy flux.
A near-zone n=2 coefficient is not automatically an observable E2.

