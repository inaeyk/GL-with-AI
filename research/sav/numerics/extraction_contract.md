# Proposed early-time waveform extraction contract for Michael

**Proposal only:** no production implementation, code changes, or launches
authorized by this document. Review feasibility with Michael.

## Priority 0 — scientific prerequisites

Require stable, constraint-controlled uniform GP background through the
intended time window, validated physical GL growth with amplitude/radial
profile and multiple outer domains, and causal separation from boundaries.
As of 2026-10-22 M2-C failed its first unperturbed GP control; these are
**not** qualified in the project's current production implementation.

## Priority 1 — n=0 null-infinity scalar flux

Record enough 5D metric and coordinate data on a set of spherical
worldtubes (each includes periodic z) to reconstruct, in an asymptotic
4D Einstein-frame Bondi gauge,
`phi_0(u,rho)=Phi(u)/rho+O(rho^-2)`.
Needed: induced metric, lapse/shift and coordinates, relevant time/radial
derivatives, compact dependence, constraint monitors, and metric data
to determine rho and u. Do not replace the invariant construction by
`(sqrt(3)/4) <log g_zz>_z` at arbitrary finite radius; gauge and
nonlinear zero-mode terms can contaminate it.

Extract the waveform `Phi(u)`, news `N_phi=dPhi/du`,
`dE0/du=N_phi^2/G4`, positive-frequency spectrum,
cumulative `E0(u)/M0` and energy for a documented first-bulge
interval. Use independently defined reconstruction pathways and compare
zero-mode 5D/Bondi mass balance.

## Priority 2 — massive KK singlet channels

Project physical master data with
`F_n=(1/L)int dz F e^{-i k_n z}`, k_n=2pi n/L, preserving
real-field +/-n normalization. Use canonical symplectic energy current
from the constrained quadratic 5D (or reduced 3D) action. Asymptotically
`q_n=sqrt(omega^2-k_n^2)`: propagating only when |omega|>|k_n|;
`dE_n/domega` is proportional to `omega q_n` times squared
canonically normalized outgoing amplitude. Determine the overall
positive constant **before** implementation. Report finite-radius
energy transport and dispersive group delays; never count an evanescent
component or a radial standing wave as emitted energy.
Massive modes do not generically reach 4D future null infinity.

## Worldtubes and causal windows

Candidate *illustrative* radii for review: rho/r0=64,96,128,192.
They are NOT specified as physically valid until the actual causal
outgoing window, background gauge, wavelengths and simulation outer
boundary are known. Compare several worldtubes; the farther boundary
must not communicate spurious signals to an extraction tube in the
accepted window. Near massive KK thresholds `q_n^-1` can be far
larger than r0 and requires special dispersive exterior matching or
longer radius/time windows. Distinguish source time, travel time, and
measured arrival time.

Before any new production work, quantify expected output field count,
HDF5 footprint, MPI ownership, synchronization between AMR levels,
and reconstruction work; default-off diagnostics remain the norm.

## Required delivered data schema (future)

- Header: repository/dependency SHA, gauge, action/field normalization,
  circle circumference, r0, units, extraction method and provenance.
- For every tube: invariant areal radius, coordinate radius, Bondi-like
  retarded time, raw/reconstructed metric fields and derivatives, z FFT
  convention, constraints and uncertainty flags, sampling cadence.
- Analysis: zero-mode news, flux and cumulative energy, massive
  mode amplitudes/dispersion, complex phases, radius extrapolation,
  raw/fitted spectra, resolution and outer-boundary comparisons.
- Explicit masks for switching transients, boundary reflections, AMR
  transients, gauge adjustment and insufficient far-zone radius.

Success means a physical scalar waveform/error bound during a controlled
first-bulge event, **not** merely a numerically smooth g_zz trace.
