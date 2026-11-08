# Reproduction

Use Python 3.14.8 with python-flint 0.9.0, NumPy 2.4.6, SciPy 1.18.0,
SymPy 1.14.0 and Numba 0.65.1. The optional figure uses Matplotlib 3.11.0.
Run from the goal branch's research/sav directory. GOAL-015 geometry
requires a separate clean checkout of the source SHA in [PROGRAM](PROGRAM.md).
All calculations are bounded fixed-background perturbation checks.
Only the final candidate and witness data needed by the scripts are
recomputed into scratch storage. The repository retains text sources,
small outputs and a figure.

~~~sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1 NUMBA_NUM_THREADS=1
g=GLR-GOAL-017
scratch=$(mktemp -d)

python3 "$g/spectral/baseline.py" --early 128 --late 256 --radial 256 --norm 8192
python3 "$g/spectral/inverse_check.py"
python3 "$g/verification/template_boxes.py"
python3 "$g/verification/horizon_contacts.py"
python3 "$g/analysis/witness_boxes.py"
python3 "$g/analysis/geometry_extension.py" "$goal015_checkout" --refinement 4 --precision 192

python3 "$g/checks/late_march.py" --packet 256 --late 480 --radial 720 --cluster 768 \
  --probes --output "$scratch/cluster.npz" --volume "$scratch/cluster-volume.npz"
python3 "$g/checks/candidate_gates.py" "$scratch/cluster.npz" --sharpen
python3 "$g/checks/current_readout.py" "$scratch/cluster.npz" "$scratch/cluster-volume.npz"
python3 "$g/checks/march_control.py"
python3 "$g/verification/candidate_replay.py" "$scratch/cluster.npz"
python3 "$g/analysis/weak_candidate.py" "$scratch/cluster.npz" "$scratch/cluster-volume.npz"
MPLCONFIGDIR="$scratch/mpl" python3 "$g/figures/plot_window.py" "$scratch/cluster.npz" "$scratch/later-window.png"
~~~

Set the shell variable goal015_checkout to the separate GOAL-015
checkout before its command. The [computational track](checks/REPRODUCTION.md)
records exact coarse/fine alternatives, the clustered candidate digest,
whole-cell probes and retained failures. The two baseline certificates and
the inverse outputs are in [spectral](spectral). Directed template boxes,
candidate-only quadrature, analytic controls and the exposure ledger are in
[verification](verification). The radius-400 exterior source bound is
inherited and was cross-checked under the same ordered hypotheses.

## Terminal replay

A fresh scratch replay reproduced the clustered binary64 candidate digest
34284f389420d62aaacd30b111b17f0c740fca3528b9d7cc85290909996d9c07.
The baseline, template boxes, horizon contact, inverse, weak witness
threshold, weak moment and refined geometry outputs matched their archived
text exactly. The separate clustered candidate quadrature is recorded in
[verification](verification/clustered-replay.txt). The clustered march with
probes used 22.97 CPU seconds and 23.36 elapsed seconds on the replay.
The central exterior IVP repeated its archived SciPy warning while the
candidate digest remained unchanged. [Failure record](checks/FAILURES.md).
