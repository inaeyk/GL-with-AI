# Boundary and propagation considerations (design; not code changes)

Current repository M2-A/M2-B adopts a reduced characteristic radial
boundary plus corrected initial perturbations; M2-C GP control is blocked.
Retain its history as engineering input, not as asymptotic radiation truth.

## Distinct boundary layers

1. **Nonlinear near zone:** causal/constraint/gauge characteristic
   treatment in full 5D reduced CCZ4, respecting both radial faces,
   periodic z, and hidden-SO(3) reconstruction.
2. **Weak-field exterior:** ideally match invariant compact harmonics
   onto scalar/massive master equations on Schwarzschild + controlled
   falloff, with no gauge-incoming contamination.
3. **Far asymptotic region:** n=0 outgoing to Bondi null infinity;
   n!=0 propagates dispersively to large timelike radius if on-shell.

The inner face is not necessarily pure excision for the chosen moving
puncture gauge; the actual characteristic normal speeds matter. M2-A
found incoming gauge characteristics at its selected inner face. No
boundary formula should be changed here by inference.

Standard massless Sommerfeld data do not provide transparent outgoing
conditions for arbitrary massive KK modes near threshold. Evaluate
characteristic extraction, frequency-domain Dirichlet-to-Neumann maps,
retarded convolutions or independent exterior propagation, with explicit
reflection and energy-conservation tests. This is a research comparison,
not an approval to replace current M2-B choices.

## Physical checks

- Stationary GP data -> scalar news zero and no massive radiative flux
  in two coordinate representations.
- Pure-gauge pulse -> invariant outgoing flux zero.
- Manufactured scalar pulse -> outgoing amplitude and flux conserved
  across radial tubes at late radiation times.
- Manufactured massive wave with |omega|>|k| -> correct dispersion,
  phase/group delay and flux; |omega|<|k| -> evanescence.
- Multiple exterior domains -> demonstrated reflection/systematics bound.
- No unverified region of the finite radial GP domain may be labelled a
  far-zone extraction region.
