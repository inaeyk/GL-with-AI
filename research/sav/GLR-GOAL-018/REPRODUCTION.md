# Reproduction

Run in the source-based branch's research/sav directory. The analytic paths in [DERIVATIONS](DERIVATIONS.md) require the pinned predecessor definitions and the audit's boundary qualifications. The computation is nonproduction.

Python 3.14.8, python-flint 0.9.0, NumPy 2.4.6, Arb at 192 bits. Use the rest of the GOAL-017 pinned environment for archive regeneration. The terminal check used the already available original GOAL-017 archive and independently validated its ordinate digest before arithmetic.

~~~sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1 NUMBA_NUM_THREADS=1
scratch=$(mktemp -d)
python3 -B GLR-GOAL-017/checks/late_march.py --packet 256 --late 480 --radial 720 --cluster 768 \
  --probes --output "$scratch/cluster.npz" --volume "$scratch/cluster-volume.npz"
python3 -B GLR-GOAL-018/weak_witness.py "$scratch/cluster.npz" GLR-GOAL-017/spectral/baseline.py
~~~

The first command is the pinned predecessor archive regeneration recipe. The second was executed using the original archive, with 6.54 CPU seconds and 102744064 bytes peak RSS. A coordinator replay uses the same digest and arithmetic. Validate the serialized ordered little-endian columns \((u,n_{21},n_{01})\) by digest 34284f389420d62aaacd30b111b17f0c740fca3528b9d7cc85290909996d9c07. The original NPZ byte SHA-256 is c0f54b63e1bfa983d3bcbd0e48ecec9b049483b908cce01e27a55a5ddcc19f97; the ordinate digest governs numerical identity across archive repackaging. The script checks finite aligned vectors, monotonicity and exact window endpoints before comparing that digest.

- [Script](weak_witness.py), SHA-256 2f56cfcd63e31fb105d8136f72533b78b72187f68f090d8a7e43f39de5d363b8.
- [Executed output](weak_witness.txt), SHA-256 1eaccf386dca8846bd1e3c5eb9bd931e4852a2595857f83c9781e7b08c3f9b83. CPU and memory lines vary on replay.
- Initial nuisance-span pass failed on an interval exponential rate containing zero. The final implementation bounds the deviation from the constant integrand using \(|e^{su}-1|\le |s|u e^{|s|u}\), and completes the zero-rate control. The baseline projection and template-box arithmetic use the original GOAL-017 wave and pole inputs at the pinned source parent. [Original regeneration and checks](../GLR-GOAL-017/REPRODUCTION.md)
