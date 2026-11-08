# Non-production reproduction

Run from the pinned research branch in research/sav. The recorded environment used Python 3.14.8, python-flint 0.9.0, NumPy 2.4.6, SciPy 1.18.0 and SymPy 1.14.0. No production code or 5D evolution is involved.

```sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1
python3 GLR-GOAL-012/checks/a_spectral.py
python3 GLR-GOAL-012/checks/shifted_denominators.py
python3 GLR-GOAL-012/checks/shifted_enclosure.py
python3 GLR-GOAL-012/checks/qnm_norm.py
python3 GLR-GOAL-012/checks/d_flux.py
python3 GLR-GOAL-012/checks/c_inversion.py
python3 GLR-GOAL-012/checks/shifted_transfer.py --strong --offset .0005
python3 GLR-GOAL-012/checks/shifted_transfer.py --angle 40 --length 88 --left-delta 3e-7 --tol 3e-12 --cap .012 --strong
```

- [a_spectral.py](a_spectral.py) encloses the actual baseline moment kernel and the baseline pencil at the lower shifted massive threshold. [Output](a_spectral.txt).
- [shifted_denominators.py](shifted_denominators.py) validates the downstream pencils on the full translated pole and GL boxes. The initial diagnostic asserted a lower bound on the real part that the result did not meet. The imaginary part excludes zero with margin. [Output](shifted_denominators.txt) · [Failed assertion](shifted_denominators_failures.txt).
- [shifted_enclosure.py](shifted_enclosure.py) validates both massive affine solutions and all three returning overlap pieces for a unit horizon mode. It uses the exact baseline matching at its certified root, propagates the full off-pole ingoing returning test on the rotated right ray, includes the strong collar, mean and outer tails, and converts to the physical packet amplitude using directed launch and Wronskian normalization. The initial shape failure and the overly wide normalized quotient are retained. The latter can be repeated with the option --loose-normalization. [Output](shifted_enclosure.txt) · [Failures](shifted_enclosure_failures.txt).
- [qnm_norm.py](qnm_norm.py) encloses the Gram norm and relative damping using the residue and frequency boxes. [Output](qnm_norm.txt).
- [d_flux.py](d_flux.py) encloses the exact-baseline optical projections and the missing returning residual tolerances at the actual width. The residual itself remains open. [Output](d_flux.txt).
- [c_inversion.py](c_inversion.py) exercises the spectral and inverse-error formulas on manufactured controls, including a square barrier and a free source. [Output](c_inversion.txt).
- [shifted_transfer.py](shifted_transfer.py) supplies empirical checks by the weak and strong pairing and symmetric full-transform samples. The strong control over a single wide final cell fails on the longer ray. Add --wide-strong-cells to the second command to reproduce it. Short cells restore agreement. [Initial output](shifted_transfer.txt) · [Long-ray output](shifted_transfer_variant.txt) · [Failed wide-cell run](shifted_transfer_failures.txt).

The directed radial checks use the validated methods from the exact GOAL-011 base, with new input-dependent data and bounds at the translated pole. The empirical weak and strong pairings share computed lower fields. The relative spreads of empirical runs do not certify truncation error.
