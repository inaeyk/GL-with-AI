# Novel approaches — feasibility-ranked ideas, not endorsed algorithms

All proposals below are analytical/design research only until separately
approved. The initial Goal is the conventional second-order radion
problem; new methods should help its verification or later extraction.

## A. Near-term classical methods (priority high)

1. **Exterior weak-field matching:** evolve/have Michael output controlled
   invariant worldtube data near the string, then propagate n=0 massless
   and n!=0 massive master variables separately to large radius.
   Benefit: avoid large expensive nonlinear far zones. Challenge:
   second-order gauge-invariant matching, finite-radius truncation,
   constraints and double counting. Test on analytic scalar pulses first.
2. **Constraint-preserving, frequency-aware massive boundaries:**
   derive Dirichlet-to-Neumann or causal convolution conditions for
   dispersive KK channels. Benefit: less reflection at manageable radius.
   Challenge: long-memory/nonlocal boundary kernel and coupled nonlinear
   gauge modes. Verify with manufactured modes across threshold.
3. **Reduced 2+1 action as independent reference solver:** potentially
   a small, carefully controlled analytic/linear or toy test code,
   *not* an alternative production nonlinear simulation by default.
   Challenge: regularity, gauge choice, horizon treatment and error
   certification.
4. **Spectral/finite-element exterior master solver:** useful for 1D
   Green functions and controllable boundary-value error; explicitly
   distinguish from the rejected D12-D16 finite-domain tangent-map
   projection, whose instantaneous constraint space was non-invariant.

## B. AI-assisted algorithms (conditional value)

- Physics-informed surrogates of the *linear exterior transfer operator*
  and radion source integrals, trained/validated on independent
  high-precision solutions; conserve symplectic flux within tolerance.
- Error-aware adaptive waveform sampling, constraints/near-boundary
  anomaly detection and compression of radial worldtube data.
- AI support for symbolic derivation, differential identity discovery,
  code review and independent audits — every physics conclusion must
  survive exact checks.

**Disallow** an unverified neural replacement of full GRChombo dynamics
or claims of energy balance without analytic normalization. Learned
models must report out-of-domain failure and uncertainty.

## C. Quantum algorithms (exploratory, low current priority)

Toy Hamiltonian simulations or spectral estimation for a discretized
1D master operator may serve as complexity benchmarks. But
state-preparation, readout, nonunitary growth and classical low-dimensional
solvers are major barriers. No credible advantage is established for
the physical D=5 GL radiation calculation. Require a resource/accuracy
comparison before engineering investment.

## D. Holographic/effective analogies (exploratory)

- Blackfold/hydrodynamics: derive long-wavelength instability and
  compare `c_s^2=-1/2`; assess errors at k*r0~0.4-0.8 and distinguish
  near-zone dynamics from radiation that escapes to infinity.
- Large-D effective models and matching: potentially useful for
  deformation/cascade heuristics but not automatically accurate at D=5.
- AdS/holographic black strings: conceivable analogues of instabilities
  and dissipation, but **the KK-flat vacuum string has no direct AdS/CFT
  dual**. Demand a precise decoupling limit, state/operator map and
  matching observable before claiming a holographic computation.

## Promotion rule

Move a speculative method into a Codex Goal only when it addresses a
clearly identified mathematical/physical bottleneck; supply baseline
complexity, conserved quantities, independent tests, and honest
stopping conditions. Preserve negative feasibility assessments.
