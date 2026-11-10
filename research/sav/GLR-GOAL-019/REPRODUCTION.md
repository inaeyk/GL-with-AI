# Reproduction

From research/sav in the GOAL-019 branch, run the bounded nonproduction exact symbolic check.

~~~sh
env OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1 PYTHONDONTWRITEBYTECODE=1 \
  python3 -B GLR-GOAL-019/nonmean_mellin.py
~~~

Python 3.14.8 and SymPy 1.14.0. Inputs are terminal radial powers, the optical equations and the explicit rational normal form in [MELLIN](MELLIN.md). The specialist run used 7.657 CPU seconds and about 104 MB RSS on macOS. The timing line varies. The exact tests include symbolic comparison with a direct bidegree jet calculation, rational reconstruction for \(n=5,6\), and removal of the varied angular potential as a wrong-input test. No numerical vacuum evolution or exact trace calculation was performed.

- [Exact script](nonmean_mellin.py), SHA-256 7020a0ca895be1af4eb6dc030d760450d4c58206b168324e1004314467812631.
- [Saved output](nonmean_mellin.txt), SHA-256 5f12c6063c28bf9d98886225c851734eb9e42883150019360997f471a2cfc295.
- The scalar waveform and cancellation cost are inherited from GOAL-016 at the exact source and audit pins in [GOAL](GOAL.md). The optional predecessor replay is described in [GOAL-016 reproduction](../GLR-GOAL-016/REPRODUCIBILITY.md).
