# Bounded GL-to-radion transfer calculation — numerical evidence

2026-10-22, task `T-20261022-GLR-GOAL-001`. Non-production 1D ODEs and
quadratures only. The exact source and normalization are in
`quadratic_radion_source.md`; the exact retarded integral is in
`radion_retarded_transfer.md`. The numerical evidence here strongly
supports nonzero C0 for the four modes, but is not an interval-arithmetic
proof of the continuum coefficient. No finite-A error window is certified.

## Results and error classes

Use `r0=1`, `Htheta/(2r^2)=1` at r=2, real cosine amplitude A, and
`r_*=r+log(r-1)`. The scalar response is `Phi=r0 C0 A(u)^2` only for
the isolated exponential particular response / appropriately settled data.

| L/r0 | kr0 | Omega*r0 | C0 estimate | conservative estimated absolute C0 error |
|---:|---:|---:|---:|---:|
| 8 | 0.7853981634 | 0.02645870 | 28.2964 | 0.0003 |
| 10 | 0.6283185307 | 0.06328119 | 12.30729 | 0.00003 |
| 12 | 0.5235987756 | 0.08044276 | 7.02554 | 0.00003 |
| 16 | 0.3926990817 | 0.09161923 | 3.32104 | 0.00003 |

The errors are deliberately loose **empirical estimates**, not certified
confidence intervals or strict bounds. They exceed the observed refinement
differences, scalar-tail brackets and independent reference discrepancy.
Growth-rate estimated absolute uncertainty is 1e-8 (also empirical).
The last digits stored in JSON are computational diagnostics, not all
scientifically significant digits. Scalar-tail-only brackets below do
not enclose errors in the GL mode, floating scalar solve or quadrature.

For the three configurations listed in `gl_linear_mode.md`, scalar
outer radii are respectively 80,120,180 and the same ODE tolerances/steps
are used. All three estimates with the final Volterra-tail prescription:

| L/r0 | coarse C0 | medium C0 | fine C0 |
|---:|---:|---:|---:|
| 8 | 28.29642287 | 28.29642423 | 28.29642393 |
| 10 | 12.30729206 | 12.30729201 | 12.30729197 |
| 12 | 7.025542085 | 7.025542047 | 7.025542033 |
| 16 | 3.321037080 | 3.321037064 | 3.321037059 |

All sampled physical contractions are positive, including the regular
horizon series. This is a useful diagnostic, not a global sign theorem.
The source is exponentially tiny at the outer GL radius. Near-horizon
Schwarzschild component cancellations are handled in 60-digit arithmetic,
with eight Riccati Frobenius terms for delta=r-1<0.005. The leading
horizon-tail contributions omitted at the fine cutoff are respectively
approximately 4.0e-12,2.8e-14,2.3e-15,2.9e-16 in C0. These are asymptotic
estimates without a uniform finite-delta remainder bound. The exact
positive leading coefficient and cancellations are verified separately.

## Outgoing scalar normalization with an analytic tail bracket

An initial ten-term outgoing asymptotic series was less accurate at the
smallest s=2Omega (L=8) than internal quadrature errors suggested. Its
coarse/fine values were 28.29675260/28.29642393; trusting the quadrature
error alone would be wrong. A second, final approach replaces that series
with a positive Volterra remainder bound. These are the two substantive
scalar-boundary approaches; no third search/refinement was undertaken.

Write `y_inf=e^(-s x) v(x)`. From the scalar equation,

\[
 v(x)=1+\frac1{2s}\int_x^\infty
 [1-e^{-2s(t-x)}]V(t)v(t)dt.
\]

At a finite areal radius R define

\[
 J=\frac1{2s}\int_{x(R)}^\infty Vdx=\frac1{4sR^2},\quad
 E_1=\int_R^\infty e^{-2s[x(r)-x(R)]}\frac{dr}{r^3},\quad
 K1=\frac{1/(2R^2)-E_1}{2s}.
\]

All kernels are positive. Ordered n-fold Volterra iterates are bounded
by `J^n/n!`, hence

\[
 1+K1\le v(R)\le1+K1+e^J-1-J,\qquad
 E_1\le -v'(x(R))\le e^J E_1.
\]

Integrate the horizon scalar Riccati solution `pH=yH'/yH` and its log
`ellH` out to R. If `yH` is normalized to 1 at r=2, its Wronskian obeys

\[
 -W=e^{\ell_H(R)-s x(R)}[(s+p_H(R))v(R)-v'(x(R))].
\]

This gives the correct positive normalization and explicit scalar-tail
brackets without assuming an asymptotic series converges. For the finest
configuration the C0 scalar-tail-only brackets are

| L/r0 | lower | upper |
|---:|---:|---:|
| 8 | 28.2964237549 | 28.2964241103 |
| 10 | 12.3072919600 | 12.3072919847 |
| 12 | 7.02554202915 | 7.02554203776 |
| 16 | 3.32103705748 | 3.32103706059 |

These inequalities are analytic for exact ODE/E1 inputs. Their floating
evaluation is not itself a validated interval integration of those inputs.
There is no uncertainty cancellation hidden in the stated final verdict.

## Independent reference calculation

Audit II did not import the primary Frobenius contraction or re-solve a
physical GL eigenvalue. It used the saved L10 metric components, recomputed
their Lorentz contraction in high precision, solved the scalar bases in
direct linear form, and used a fixed-profile Simpson quadrature plus a
regular horizon-power fit. Its result is C0=12.3072919086602, within 6.4e-8
of the final primary midpoint. The scalar basis normalization separately
agreed at roughly 3e-13 in log amplitude with the initial reference solve.
Leading-tail completion alone differed by 1.65e-5, exposing a real
finite-cutoff correction; a quadratic regular fit removed most of it.
The fit residual is not a remainder proof. See
`../verification/AUDIT_SOURCE_RETARDED.md`, section 8, and
`../verification/audit_source_transfer_results.json`.

## Conditional energy-law coefficients, not emitted percentages

Let `Pcoef=8 Omega_hat^2 C0^2`, `Ecoef=2 Omega_hat C0^2`, so
`d(E0/M0)/d(u/r0)=Pcoef A^4` and
`Delta E0/M0=Ecoef (A2^4-A1^4)` to leading order under the conditions
of the retarded note. The corresponding central values are

| L/r0 | Pcoef | Ecoef | onset shift Delta(t/r0) for epsilon/10 |
|---:|---:|---:|---:|
| 8 | 4.48425 | 42.3703 | 87.0256 |
| 10 | 4.85249 | 19.1703 | 36.3866 |
| 12 | 2.55519 | 7.94103 | 28.6239 |
| 16 | 0.740646 | 2.02099 | 25.1321 |

Large C0 values partly reflect normalization at r=2r0 where the mode is
smaller than in some near-horizon frames. They are not a large radiation
fraction and cannot be combined with A=1 or an arbitrary A=0.1.
Bounds on the complete second-order metric/n=2 response, next even
radion order, and scalar initial transients were not obtained. Thus a
numerically controlled finite-amplitude energy fraction, full burst
spectrum and first-bulge emission estimate remain open. Windowed-news
spectra are explicit conditional expressions, not full physical spectra.

## Budget and reproduction

Four physical k values times exactly three declared GL discretizations,
one separate threshold solve; no dense spectral scan. Reference completed
first, then the other three. Scalar quadratures share those configurations.
Two scalar-boundary approaches were used, followed by an independent
saved-profile reference check. No additional physical-k or refinement
campaign is authorized by this result.

Commands already executed, Python3.14.8, SymPy1.14.0, NumPy2.4.6,
SciPy1.18.0, mpmath1.3.0:

```sh
python3 research/sav/verification/gl_linear_mode.py
python3 research/sav/verification/gl_linear_mode.py --extend
python3 research/sav/verification/radion_transfer.py
python3 research/sav/verification/audit_source_transfer.py
```

The first two regenerate the raw CSVs needed by the independent-profile
check. The primary quadrature uses reconstructed on-shell profiles directly.
Complete configuration/results are retained in the adjacent verification
JSON files; exact symbolic checks have their own transcript. These commands
are reproducibility instructions, not a request to repeat a completed
budget or to launch any spacetime simulation.
