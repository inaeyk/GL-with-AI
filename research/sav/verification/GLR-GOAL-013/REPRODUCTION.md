# Reproduction

The source under review is [PR #17 at 1e8e628f68af4cab174fcc44b81af4748a22115c](https://github.com/inaeyk/GL-with-AI/commit/1e8e628f68af4cab174fcc44b81af4748a22115c). All checks are fixed-background, analytic, or manufactured. No production simulation, historical complex signed-residue solve, or GL spectral root search was run.

## Independent implementation

From the review checkout root, with Python 3.14.8, NumPy 2.4.6 and SciPy 1.18.0, run

~~~sh
PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 \
  python3 research/sav/verification/GLR-GOAL-013/checks.py
~~~

The decimal cap computation uses explicit scalar majorants, with 55 digits for evaluating them. The source directed scripts supply interval arithmetic for the quoted inequalities. The baseline check uses a reciprocal-radius grid anchored at the latest graph point. It integrates a common-\(v\) primitive by interpolation at the moving graph, then integrates in \(u\) by trapezoid sums. It compares with the continuous diagonal approximation, whereas the source's certified candidate uses a step approximation to the diagonal primitive. The early-strip squared norm is sensitive to quadrature near the last sampled point. The early error printed below is an empirical difference of two approximations. The graph check starts just outside the horizon with a first-order regular seed and varies the start radius and IVP tolerance. It evaluates the inherited EF reconstruction independently, at the midpoint GL frequency, with adaptive real radial quadrature. The midpoint template check uses direct time-domain integration. Each binary64 calculation is evidence for a cross-check. The directed enclosure and proof obligations remain separate.

## Pinned source replay

From `research/sav` at the pinned source or review commit, use Python 3.14.8, python-flint 0.9.0 and SymPy 1.14.0. Compare stdout with the archived `.txt` files.

~~~sh
set -e
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1
g=GLR-GOAL-013/checks
replay () {
    expected=$1
    shift
    output=$(mktemp)
    python3 "$@" > "$output"
    diff -u "$g/$expected.txt" "$output"
    rm "$output"
    printf '%s MATCH\n' "$expected"
}
replay a_characteristics "$g/a_characteristics.py"
replay a_graph "$g/a_graph.py"
replay a_graph_variant "$g/a_graph.py" --cells 512 --bits 192
replay source_tail_bound "$g/source_tail_bound.py"
replay c_cap_transfer "$g/c_cap_transfer.py"
replay d_leading_news "$g/d_leading_news.py"
replay d_leading_news_variant "$g/d_leading_news.py" --mesh-power 14 --precision 128
replay d_template_norms "$g/d_template_norms.py"
replay e_adversarial "$g/e_adversarial.py"
~~~

All nine archived outputs matched byte for byte. The graph and leading-news variants change mesh and precision within their respective implementations. The cap replay includes both coefficient Taylor and Picard refinement, an off-cap manufactured source and graph, interface and terminal-cut mutations. The production ordered return and its convergence were outside these checks. The source's adverse cases include overly broad physical forcing at \(r_c=100\), significant error in freezing the \(u\) dependence of the cap coefficient, and signed interval squaring and series-cap failures found and repaired during development. [Source reproduction](../../GLR-GOAL-013/REPRODUCTION.md).

## Independent output

~~~text
cap 100: amplification 1.0025160544, third-order tail 0.0000096199366871, HS 0.0000051562564159, B 2.1042236377e+46
cap 400: amplification 1.0001570682, third-order tail 3.7551516866e-8, HS 2.0085856002e-8, B 0.0000016134446322
cap 500: amplification 1.0001005208, third-order tail 1.5380394002e-8, HS 8.2256614024e-9, B 6.8544843689e-24
baseline grid 256 x 256
  1.000  norm 1.0000336447  diagonal delta 2.8289522e-05  optical -1270.193529
  0.125  norm 0.013710606142  diagonal delta 1.0476983e-11  optical 7.209959384
baseline grid 512 x 512
  1.000  norm 1.0000336281  diagonal delta 2.8296686e-05  optical -1270.193505
  0.125  norm 0.01364480137  diagonal delta 1.6942384e-11  optical 7.189495756
baseline grid 1024 x 1024
  1.000  norm 1.0000336258  diagonal delta 2.829846e-05  optical -1270.193502
  0.125  norm 0.013628286235  diagonal delta 2.0939006e-11  optical 7.184383424
graph 0.0001, 1e-09: Ut 21.0768655545, Im Azt -1.0256207172, energy 6.3216112265
graph 1e-06, 1e-11: Ut 21.0768671401, Im Azt -1.0256207714, energy 6.3216121327
graph 1e-08, 1e-12: Ut 21.0768671401, Im Azt -1.0256207714, energy 6.3216121327
midpoint templates: Q norm 0.7355515208, Q+S norm 1.7550601693, cross 0.6192162577
~~~
