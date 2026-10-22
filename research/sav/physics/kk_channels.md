# Radiative channel classification and Fourier selection

**Status:** analytical foundation, gauge-invariant asymptotic definitions
pending an independent second-order check.

## SO(3) versus ordinary tensor radiation

Exact SO(3) rotations of transverse S^2 rule out tensor (`l>=2`)
and vector (`l>=1`) spherical harmonics. The zero-mode 4D massless
graviton has no spherical radiative news; neither does the zero-mode
graviphoton (spherical electric/magnetic charges are not traveling waves).
The n=0 KK radion is an SO(3)-singlet massless scalar and **can** have
monopole radiation. A nonzero n is a massive 4D spin-2 representation;
its spherical helicity-zero sector is a physical degree and can
transport energy if above threshold.

With `k_n=2 pi n/L`, the far-zone massive dispersion is
`omega^2=q_n^2+k_n^2`. For real frequency, `|omega|>|k_n|`
is propagating, equality has zero group velocity, and below threshold
is radial evanescence. Massive energy is a finite-worldtube/timelike
infinity issue, not generically 4D null-infinity Bondi loss.

## Order counting and selection

Normalize the fundamental GL mode as
`h_1=A(t) H_1(r) cos(k_1 z)` with `A=epsilon p_1 e^{Omega t}`.
It grows exponentially, with frequency `omega=i Omega` and a localized
asymptotic spatial tail; it is not an outgoing real-frequency KK wave.

`cos^2(k_1 z)=1/2+cos(2k_1 z)/2`.
Therefore the quadratic Einstein source contains n=0 and n=+/-2
but not n=+/-1 or l!=0. Cubic sources include n=+/-1 and +/-3, etc.
SO(3) singlets cannot source nonsinglets through invariant nonlinear
equations; exact compact Fourier charge is additive in interactions.

If the n=0 retarded overlap is nonzero and the initial data contain
no independent zero-mode wave, its outgoing waveform is generically
O(A^2), with flux O(A^4). **This does not prove the overlap nonzero.**
The forced `e^{2 Omega t}` n=2 term by itself is a Laplace-frequency
response, not an above-threshold propagating oscillatory KK wave.
Nonadiabatic rollover/initial transients can have a real spectrum that
crosses thresholds. Real-frequency massive radiation also has
dispersive arrival times `v_g=q/omega`.

## Strong consistency controls

- Half-period translation `z -> z+L/2` maps a pure cos(k_1 z)
  perturbation A to -A. Every translationally invariant n=0 quantity
  must be even in A: no linear scalar news from a pure first harmonic.
  Odd-power n=0 terms also vanish for that *restricted one-harmonic
  family*, provided the initial-data family transforms by translation.
- The scalar zero mode's stationary charge can change without fixed
  radiation energy; flux depends on time derivatives.
- A "pure GL mode" is an ideal constrained eigenmode. A practical
  near-horizon chi perturbation also excites transient/gauge/constraint
  responses and may radiate at order epsilon. Separate these cases.
- Extracted n=0 averages of nonlinear numerical metric components
  are generally gauge dependent. Construct Bondi/Einstein-frame scalar
  data and prove invariance under admissible asymptotic 5D diffeomorphisms.

Open: complete second-order constrained source, n!=0 canonical
symplectic current/normalization, and possible cancellation of n=0
outgoing amplitude.
