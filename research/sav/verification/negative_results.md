# Important negative and inconclusive results — preserve verbatim pointers

1. **D12–D16, projected spectral candidates:** D16 showed the instantaneous
   constraint-nullspace is not sufficiently invariant under the live
   tangent map. All-cell/interior leakage around 7.01%/5.68% exceeded
   the 0.1% test; full-map residuals failed. Conclusion:
   CONSTRAINT_NULLSPACE_NOT_INVARIANT. Neither a physical GL mode nor a
   GL threshold was established. The D-series was closed inconclusive;
   a new fixture-only projection is not a justified automatic response.
   Primary: `main:logs/blackstring_d16_constraint_spectrum.log`.

2. **M2-C v2 frozen campaign:** First GP control
   `medium-near-gp` exited nonzero immediately after last completed
   t/r0=2.5 sample at step 400 with
   `M2-B ALGEBRAIC_RECONSTRUCTION_ADMISSIBILITY_FAILURE`.
   All focus prelaunch checks passed, but subsequent eleven planned
   runs were not invoked. Outcome:
   PHYSICAL_RADIAL_AND_CONSTRAINT_STRATEGY_BLOCKED.
   Source: `backup/m2c-v2-frozen-campaign-20260917:
   docs/grchombo/milestone2c_strategy_closure.md`.
   It is a failed numerical baseline, not evidence GL does not exist.

3. **No measured waveform/energy fraction:** Existing horizon reproduction
   notes and the 2023 paper do not establish an initial radion news or
   KK-mode radiated-energy fraction. The assertion that the first bulge
   dominates the integrated emission remains conjectural.

4. **No demonstrated nonzero quadratic outgoing coefficient:** Symmetry
   permits n=0 at O(A^2), but no source/Green-function computation here
   has shown C0 != 0. An exact zero after constraints/gauge treatment
   is a valuable negative scientific result and must be retained.

No negative-result record authorizes repairs to Michael's simulation.
