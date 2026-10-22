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

## GLR-GOAL-001 execution negatives and limits (2026-10-22)

The four numbered findings above are retained verbatim as historical
inputs; item4 is now superseded only to the extent of the exact source and
strong numerical overlap evidence in the new report. It is not superseded
by a rigorous interval proof or by a physical simulation waveform.

5. **Initial KK convention error:** the original negative vector exponent
   was inconsistent with its positive-fiber-exponent metric ansatz.
   Audit I's constant-phi scaling requires `+2sqrt(3)phi`; corrected in
   NOTATION.md. A=0 scalar flux/source results are unaffected. The initial
   full-metric Hamiltonian explanation also conflated primary/secondary
   constraint counts; its final local DOF number1 was correct.

6. **Exact local source does not prove radiation:** audit II retains a
   manufactured source `S=L_s F` for compactly supported F with exactly
   zero outgoing overlap. The physical GL tensor contraction is indefinite
   for arbitrary radial data. Positive samples are not a sign theorem.

7. **Horizon subtraction failure avoided:** raw double-precision
   Schwarzschild metric squares have severe cancellations. Profile CSVs
   mark their raw contraction NaN below delta=1e-5. Exact leading
   divergent-term cancellations, regular Frobenius reconstruction and
   high precision are retained instead. No NaN was silently interpolated
   into the primary transfer. Horizon remainder estimates are not interval
   bounds.

8. **First scalar-boundary method insufficiently controlled:** a ten-term
   outgoing asymptotic series gave L8 C0=28.29675260 (coarse),
   28.29642957 (medium), 28.29642393 (fine), despite tiny internal
   quadrature errors. It was replaced by a positive Volterra scalar-tail
   bracket on the same three configurations. No third approach or wider
   scan was used. Final L8 midpoint28.29642393, scalar-tail-only bracket
   [28.2964237549,28.2964241103], conditional on exact numerical inputs.

9. **Bounded symbolic/benchmark limitations:** initial generic full-5D
   SymPy simplification was terminated; exact rational reduced-Ricci
   arithmetic then verified every linear equation. Collingbourne v1
   apparent r²/r³ denominator inconsistency was resolved by direct Ricci
   checks, not trusted text. Original GL1994 Figure3 precise growth digits
   were not independently digitized (PDF screenshot failed); threshold
   and a rigorous literature growth lower bound agree.

10. **Independent reference horizon completion:** leading-power tail alone
    gave12.30727551; a regular quadratic horizon fit gave12.30729191,
    consistent with the final primary12.30729197. Fit residual is not a
    rigorous remainder bound. No additional GL root solves were performed.

11. **Unclosed scientific gate:** no rigorous enclosure of all GL-mode,
    horizon-series and quadrature errors, nor full second-order metric/n=2
    and next even-scalar response bound, was obtained within the allocation.
    Arbitrary temporal switching violates the first-order GL constraints.
    Thus no numerical finite-A interval, early emitted percentage, complete
    burst spectrum, or extrapolation to A~1 is claimed. Terminal verdict
    `MEANINGFUL_PARTIAL`; a new bounded analytic/validated-numerics task is
    needed for those gates. This is not a gauge-invariance failure or a
    proved cancellation.

