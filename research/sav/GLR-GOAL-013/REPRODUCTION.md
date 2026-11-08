# Reproduction

Use the GOAL-013 checkout with first parent [87fd711bf2e8117467caad1ff52c1cd344966974](https://github.com/inaeyk/GL-with-AI/commit/87fd711bf2e8117467caad1ff52c1cd344966974). PR #16 at [ae8c323e1e87abd0fff26eac75b05b701a7e7b1a](https://github.com/inaeyk/GL-with-AI/commit/ae8c323e1e87abd0fff26eac75b05b701a7e7b1a) is the read-only review input and is outside this branch's ancestry. The independent review can be inspected at its pinned Git commit. The checks use Python 3.14.8, python-flint 0.9.0 and SymPy 1.14.0. Computations are non-production and run from research/sav.

~~~sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1
g=GLR-GOAL-013/checks
python3 "$g/a_characteristics.py"
python3 "$g/a_graph.py"
python3 "$g/a_graph.py" --cells 512 --bits 192
python3 "$g/source_tail_bound.py"
python3 "$g/c_cap_transfer.py"
python3 "$g/d_leading_news.py"
python3 "$g/d_leading_news.py" --mesh-power 14 --precision 128
python3 "$g/d_template_norms.py"
python3 "$g/e_adversarial.py"
~~~

Compare stdout with the identically named text files in checks, using the _variant file for the second invocation of each script. The scripts perform bounded symbolic, directed interval and manufactured checks.

| Check | Evidence |
|---|---|
| a_characteristics | Exact scalar graph, interface and terminal-cut mutations. Exact two-field energy check with both skew couplings active and a symmetric mutation. Arb finite-cut and weak-transfer constants. |
| a_graph | Physical GL interval transported to every launch radial cell. Real-axis mixed graph jets, directed graph energy and resolution/precision variation. Reuses the source's horizon Taylor and normalization code. |
| source_tail_bound | Directed causal geometry, bump caps and exterior source bound arithmetic for the analytic proof in B. |
| c_cap_transfer | Directed cap operator constant and tail iteration errors over radius, degree and precision. Manufactured off-cap source with a known exact news and negative cut/contact controls. |
| d_leading_news | Independent directed two-scatter baseline formula and full/early physical error bars. Evaluated optical reference against the exact causal baseline. |
| d_template_norms | Directed norm and midpoint uncertainty of \(Q\) and \(Q+S_*\) including inherited frequency/residue rectangles. |
| e_adversarial | Separate energy/conormal/edge checks, cap growth and phase-rank controls. |

The recorded adverse outcomes are part of the evidence. See [A](checks/a_failures.txt), [C](checks/c_failures.txt) and [D](checks/d_failures.txt), and the [independent review](INDEPENDENT-REVIEW.md). The historical residue enclosures enter as pinned inputs. Replaying the historical spectral solver is a different check from any of these causal certificates.

All dimensionful radii and times are scaled by \(r_0\), all news norms use dimensionless \(du\), and the single Fourier graph energy uses the \(+k\) coefficient including its factor \(1/2\). The bounds do not provide a numerical time series for the physical returning coefficient. The physical leading approximant is defined by the deterministic quadrature in [Flux](FLUX.md) and its reproducible input profile.
