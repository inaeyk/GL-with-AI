#!/usr/bin/env bash
# Run from research/sav on the review branch (or a clean pinned source plus review files).
set -euo pipefail
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1
g=GLR-GOAL-014/checks
p=GLR-GOAL-013/checks
r=verification/GLR-GOAL-014

replay() {
    local output=$1
    local script=$2
    shift 2
    python3 "$script" "$@" | diff -u "$output" -
    printf '%s matched %s\n' "$script" "$output"
}

replay "$g/a_adjoint.txt" "$g/a_adjoint.py"
replay "$g/a_terminal_sensitivity.txt" "$g/a_terminal_sensitivity.py"
replay "$g/a_terminal_sensitivity_variant.txt" "$g/a_terminal_sensitivity.py" --panels 720
replay "$g/b_weak_tails.txt" "$g/b_weak_tails.py"
replay "$g/b_graph_mean.txt" "$g/b_graph_mean.py"
replay "$g/b_graph_mean_variant.txt" "$g/b_graph_mean.py" --cells 2048 --bits 192
replay "$g/d_baseline_h1.txt" "$g/d_baseline_h1.py"
replay "$g/d_baseline_h1_variant.txt" "$g/d_baseline_h1.py" --mesh-power 14 --precision 128
replay "$g/d_graph_projection.txt" "$g/d_graph_projection.py"
replay "$g/d_graph_projection_variant.txt" "$g/d_graph_projection.py" --mesh-power 11 --precision 128
replay "$g/e_baseline.txt" "$g/e_baseline.py"
replay "$g/e_baseline_variant.txt" "$g/e_baseline.py" --mesh-power 15 --precision 192
replay "$g/e_controls.txt" "$g/e_controls.py"
replay "$g/e_graph_projection.txt" "$g/e_graph_projection.py"
for name in a_characteristics source_tail_bound d_leading_news c_cap_transfer; do
    replay "$p/$name.txt" "$p/$name.py"
done
replay "$r/c-replay.txt" "$r/c-replay.py"
replay "$r/a/adjoint-output.txt" "$r/a/check_adjoint.py"
replay "$r/b/check_sources.txt" "$r/b/check_sources.py"
replay "$r/d/checks.txt" "$r/d/checks.py"
replay "$r/e/late_rw.txt" "$r/e/late_rw.py"
