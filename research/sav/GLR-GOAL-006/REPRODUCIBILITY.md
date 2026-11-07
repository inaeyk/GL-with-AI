# Reproducibility

Use the independent branch `codex/glr-goal-006-bondi-control` in `inaeyk/GL-with-AI`, based directly on `61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708`. The separate Work review at `a9bfbec0efe41b881e4b12aa262f5fcfcd94c639` was read through the GitHub commit API. Its check scripts and the source branch were left intact. [Pinned science](https://github.com/inaeyk/GL-with-AI/commit/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708) · [Pinned review](https://github.com/inaeyk/GL-with-AI/commit/a9bfbec0efe41b881e4b12aa262f5fcfcd94c639)

Run from `research/sav`:

```sh
python3 -B GLR-GOAL-006/check_diagnostics.py
```

Actual environment was Python 3.14.8, NumPy 2.4.6, SciPy 1.18.0 and SymPy 1.14.0. The checker ran the original script and the coordinator and integration specialist replayed it. No input files, production evolutions or empirical GL data enter these calculations. The script uses symbolic algebra for linear ideal-end coefficients, the mass slope and the null identity. It solves a manufactured linear $n=2$ constraint end from $r=8$ to $256$ using Radau at relative tolerances $10^{-8}$ and $10^{-11}$, each with absolute tolerance one twentieth of the relative tolerance. The flux quadrature uses absolute and relative tolerances $10^{-11}$. The clock quadrature sets absolute tolerance $10^{-12}$ and uses the SciPy default relative tolerance. The finite-radius oscillator sample is floating output. The transfer inequality in [Derivations §1](DERIVATIONS.md#a-tail-in-the-original-constraint-class) is analytic.

```text
Python 3.14.8, NumPy 2.4.6, SciPy 1.18.0, SymPy 1.14.0.
Exact ideal-end coefficients, Coulomb phase and 9/14 mass slope verified.
Manufactured n=2 transfer at r=256: 0.999913352+0.000318101339j, σ=0.00164853.
Demodulated change [128,256]: 7.64e-08; no-log mutation: 0.306.
Projected lapse diagnostic: max r³|w|=1.68168.
Manufactured null energy balance error: 0; no-mass mutation: 0.00857.
Model ray source on [16,100]: 0.00157152; flux/R: 0.00265303.
Clock errors divided by ε⁴, at log R=8 and log R=3/ε²: [(4.63841, 9.12024), (5.08566, 36.48097), (5.11767, 145.92388)]
```

The stronger end is specified reconstructably by (A1)–(A3). A numerical implementation, if separately authorized, would choose a smooth cutoff with a regular zero-control collar and finite $p$ for its target derivative count, solve the same radial constraints with the explicit added sources (A2), and tune the single spherical inner lapse using the mass map. The present result uses estimates and makes no empirical calibration of these parameters.

The original quadratic waveform and certification remain reproducible by the [pinned GOAL-005 record](../GLR-GOAL-005/REPRODUCIBILITY.md). The historical Work reviewer reran those scripts. This task read that record and did not repeat the physical benchmark calculation.
