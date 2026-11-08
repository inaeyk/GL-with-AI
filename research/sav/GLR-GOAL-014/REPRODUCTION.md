# Reproduction

Run from `research/sav` on the GOAL-014 branch based on
[`1e8e628f68af4cab174fcc44b81af4748a22115c`](https://github.com/inaeyk/GL-with-AI/commit/1e8e628f68af4cab174fcc44b81af4748a22115c).
The Work review at `26a09054366933a197c34fa67f617af7da76e48d` is a
read-only input outside this ancestry. The recorded environment is
Python 3.14.8, python-flint 0.9.0, NumPy 2.4.6, SciPy 1.18.0 and
SymPy 1.14.0. These are bounded fixed-background and manufactured
calculations.

## New proofs and independent controls

~~~sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1
g=GLR-GOAL-014/checks
python3 "$g/a_adjoint.py"
python3 "$g/a_terminal_sensitivity.py"
python3 "$g/a_terminal_sensitivity.py" --panels 720
python3 "$g/b_weak_tails.py"
python3 "$g/b_graph_mean.py"
python3 "$g/b_graph_mean.py" --cells 2048 --bits 192
python3 "$g/d_baseline_h1.py"
python3 "$g/d_baseline_h1.py" --mesh-power 14 --precision 128
python3 "$g/d_graph_projection.py"
python3 "$g/d_graph_projection.py" --mesh-power 11 --precision 128
python3 "$g/e_baseline.py"
python3 "$g/e_baseline.py" --mesh-power 15 --precision 192
python3 "$g/e_controls.py"
python3 "$g/e_graph_projection.py"
~~~

Compare to the corresponding text outputs under `checks/`.
Precision and mesh variants have a `_variant` suffix. The E graph
calculation prints three central real-axis and compactified runs.
The A sensitivity calculation uses empirical central geometry.
The A rational constants, B directed tail and graph arithmetic,
D directed baseline and graph overlap, and E directed baseline
are separate from those empirical routines. The graph proof uses
B's directed whole-early-graph mean boxes. The independent E graph
calculation checks the corrected slope and gives a central projection
inside D's interval.

## Compact physical pilot

Write binary candidate ordinates to a temporary directory for the
directed cap-candidate check. The branch retains source and text
outputs. The cap time and affine abscissae are defined mathematically
in the script.

~~~sh
scratch=$(mktemp -d)
python3 "$g/c_compact.py" --n 48 --m 180 --cap 40 --outer 180
python3 "$g/c_compact.py" --n 96 --m 360 --cap 80 --outer 200
python3 "$g/c_compact.py" --n 192 --m 720 --cap 400 --outer 430 --tol 8e-14 \
  --dump "$scratch/cap.npz"
python3 "$g/c_cap_gate.py" "$scratch/cap.npz"
python3 "$g/c_cap_gate.py" "$scratch/cap.npz" --bits 192
python3 "$g/c_graph_residual.py"
python3 "$g/c_graph_residual.py" --bits 192
~~~

The [recorded output](checks/c_results.txt) gives the weak, direct
differentiated-shear, discrete transpose and strong-source values.
The fine physical compact candidate has weak value near $7.8181024$.
The homogeneous continuation from $400r_0$ gives a value near
$7.8178177$. The outer constraint solve in this pilot uses a finite
numerical endpoint. Its far-end proxy is empirical. The directed
outputs are for the interpreted candidate norm and reference pairing,
the baseline flux errors and the mixed graph jet error. Whole-cell
compact physical residuals and the actual return error $E_c$ are open.
The coordinator replayed the fine compact pilot, cap gate and primary
mixed graph residual and recovered the recorded numerical values.
Platform differences may affect floating-point ordinates and last
printed digits. Recompute candidate arithmetic from the actual
generated ordinates.

## Pinned source replays

The coordinator replayed these GOAL-013 outputs byte for byte in this
environment. They check the original implementations and are distinct
from the new derivations and independent graph march.

~~~sh
p=GLR-GOAL-013/checks
for name in a_characteristics source_tail_bound d_leading_news c_cap_transfer; do
  python3 "$p/$name.py" | diff -u "$p/$name.txt" -
done
~~~

The new central scripts `a_adjoint`, `b_weak_tails`, `d_baseline_h1`,
`d_graph_projection` and `e_controls` also replayed byte for byte.
See the [review](INDEPENDENT-REVIEW.md) for negative controls and
the retained outputs for the wrong tilt factor, dependency inflation
and quadrature and precision failures.
