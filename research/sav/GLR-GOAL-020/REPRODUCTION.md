# Reproduction

Run from research/sav on the GOAL-020 branch. These are bounded nonproduction checks. The exact arithmetic uses Python 3.14.8 standard library. The short-tail check uses python-flint 0.9.0 with Arb at 160 bits and the pinned GOAL-015 geometry environment.

~~~sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1
python3 -B GLR-GOAL-020/ordered_ball_arithmetic.py
python3 -B GLR-GOAL-020/checks/short_tail.py
~~~

The first check reproduces the source/flux constant arithmetic, graph normalization, radius and sign margins, literal normalized parameter rectangle, and a conservative crossing bracket from the uniform modulus. The second check validates 15 pinned source/archive digests and the clean source-directory comparison before recomputing the directed shorter bracket, with an explicit failed tighter cut. The output of each was compared to the saved output. The primary continuum proof and directed input interfaces are in [DERIVATIONS](DERIVATIONS.md) and [TEMPORAL](TEMPORAL.md).

| File | SHA-256 |
| --- | --- |
| [ordered_ball_arithmetic.py](ordered_ball_arithmetic.py) | 868c22e54b88d54ad5fe402b46ecfbfd26ccf7fe6a2d13f90d873cf455b1e7f3 |
| [ordered_ball_arithmetic.txt](ordered_ball_arithmetic.txt) | 574be5823cdc5716bea9ab785832b20e27fdb3905d47e9b6f1ce0bfa335dab3a |
| [short_tail.py](checks/short_tail.py) | 83e5f7997fd2462385f4c752a67c65ba8e1ba6536bad482c92d5f56eac204b9c |
| [short_tail.txt](checks/short_tail.txt) | 9e3fd7fdff23bacf5bafa6bab763df7b73cf0443b1124a633b26128b0a4ba218 |

The inherited signed packet bound is reproduced by `python3 -B GLR-GOAL-015/observables/packet_signed.py` with explicit check for `P_PACKET_STRICTLY_NEGATIVE True`. It reuses the pinned directed source rather than an empirical archive. The early strong and weak comparison recipes are in [GOAL-015 reproduction](../GLR-GOAL-015/REPRODUCTION.md). No exact nonlinear tensor-family simulation or candidate strong error enclosure was run.
