# Custom Solver / GRChombo Comparison Batch 4 Results

Status: passed test-only analytic-background, gauge-source, and discrete-setup
comparison. No production initial data, hidden evolution, gauge-source
adapter, time evolution, or outer-boundary work was added.

## Reproducibility and formulation lock

| Item | Inspected value |
|---|---|
| Custom checkpoint | `debafd929377498eaa22c84fdf8c84409f7c3cee` |
| GRChombo source | clean detached checkout at `37e659523830418b210acea1661dac0e00bb1b75` |
| Direct stock dimension | `CH_SPACEDIM=3`, stock visible `d=3` |
| Custom production convention | gridded `d=2`, physical `d=4`, hidden multiplicity two |
| CCZ4 convention | `USE_CCZ4`, `Lambda=0`, `kappa1=0.1`, `kappa2=0`, `kappa3=1`, `covariantZ4=true`, vacuum matter |
| Gauge convention | analytic lapse/shift supplied to the CCZ4 seam; direct `MovingPunctureGauge` with lapse advection disabled, lapse power one, coefficient two, shift-Gamma coefficient `0.75`, shift advection disabled, and `eta=1` |
| Compiler contract | C++17, `-O2 -Wall -Wextra -Wpedantic -Werror` |
| Chombo/container | unresolved; direct header kernels and algebra/gauge paths compile without Chombo |

The fixtures are
`code/BlackStringToy/tests/Stage4AOCGRChomboComparisonBatch4GPBackgroundTest.cpp`
and
`code/BlackStringToy/tests/Stage4AOCGRChomboComparisonBatch4GaugeTest.cpp`.
They directly call inspected GRChombo tensor algebra, CCZ4 RHS, fourth-order
derivative, and moving-puncture gauge templates. The independent custom path
calls the actual centered-order-two jet builder and the validated full
four-dimensional GP residual oracle.

## GP convention comparison

For the ingoing GP slicing,

\[
\alpha=1,\qquad \beta^x=\sqrt{r_0/x},\qquad
\lambda=\sqrt{r_0/x^3},
\]

\[
K_{xx}=-\lambda/2,\qquad K_{ww}=\lambda,\qquad
K=3\lambda/2,
\]

\[
A_{xx}=-7\lambda/8,\qquad A_{zz}=-3\lambda/8,\qquad
A_{ww}=5\lambda/8.
\]

There is a necessary coordinate-versus-storage distinction. The angular
coordinate metric has \(\gamma_{\theta\theta}=x^2\), but the modified-cartoon
state stores the normalized Cartesian representative
\(\gamma_{ww}=\gamma_{\theta\theta}/x^2=1\), hence custom `hww=1`, not
`hww=x^2`. Likewise, the stored \(K_{ww}=\lambda\) represents
\(K_{\theta\theta}/x^2\). The fixture rejects placing \(x^2\) in the custom
`hww` slot because that violates the four-dimensional determinant, weighted
trace, and reconstruction identities.

At \(r_0=1,x=2\), the custom values are

| Quantity | Value |
|---|---:|
| \(\lambda\) | `3.53553391e-01` |
| \(K\) | `5.30330086e-01` |
| \(A_{xx}\) | `-3.09359217e-01` |
| \(A_{zz}\) | `-1.32582521e-01` |
| \(A_{ww}\) | `2.20970869e-01` |

The honest stock-visible \(d=3\) seam keeps visible \(y\) ordinary and does
not identify it with hidden \(ww\). Starting from the overlapping visible
physical tensor \(K_{xx}=-\lambda/2\), \(K_{yy}=K_{zz}=0\), direct stock
three-dimensional trace/trace-free conventions give

| Quantity | Value |
|---|---:|
| \(K_{d=3}\) | `-1.76776695e-01` |
| \(A_{xx,d=3}\) | `-1.17851130e-01` |
| \(A_{yy,d=3}\) | `5.89255651e-02` |
| \(A_{zz,d=3}\) | `5.89255651e-02` |

These are not expected to equal the custom physical-\(d=4\) \(K,A\)
coefficients. The lapse, visible shift, unit visible conformal metric,
determinant, trace-free relation, zero background hatted Gamma/contracted
connection, and required analytic radial jets pass at the matching
dimension. A reversed extrinsic-curvature sign produces a nonzero directly
compiled stock conformal-metric RHS and is rejected. Thus the stock
three-dimensional sign is executable evidence; the target
four-dimensional/hidden coefficients remain a source/convention comparison
plus an independently validated custom residual identity.

## Directness classification

| Evidence | Classification | Result |
|---|---|---|
| Stock \(d=3\) inverse, trace, and trace-free construction | direct compiled GRChombo | pass |
| Stock contracted connection and CCZ4 visible RHS | direct compiled GRChombo | pass |
| Stock fourth-order first/second radial derivatives | direct compiled GRChombo | pass |
| Moving-puncture lapse, shift, and \(B^i\) equations | direct compiled GRChombo | pass |
| \(\gamma=h/\chi\), angular-coordinate-to-`ww` normalization, and target \(d=4\) coefficient interpretation | source/convention comparison | pass |
| `hww`, `Aww`, multiplicity-two traces, cartoon derivatives, and full \(d=4\) cancellation | custom-hidden-only | pass in the custom oracle; production adaptation remains open |
| Chombo `BoxLoop`, production ghost exchange, and a production initial-data/source adapter | blocked or out of scope | not claimed |

## Background residuals

### Independent stock-versus-target families

The repaired fixture constructs two independent GP states. It does not define
the target completion as the negative of the stock residual.

At \(r_0=1,x=2\), the stock reduction has

\[
K=-\lambda/2,\qquad \partial_xK=3\lambda/8,\qquad
\nabla_i\beta^i=-\lambda/2,\qquad d=3.
\]

The target state instead has

\[
K=3\lambda/2,\qquad \partial_xK=-9\lambda/8,
\]

\[
\nabla_I\beta^I
=\partial_x\beta^x+2\beta^x/x
=-\lambda/2+2\lambda=3\lambda/2,\qquad d=4.
\]

Thus both \(K,\partial_xK\) and the shift-divergence/trace coefficient change;
the target trace correction is \(2/d=1/2\), and hidden tensor contractions
carry multiplicity two.

| Path / row | Advection | Linear \(AK\) or \(K^2\) | Quadratic \(A^2\) | Shift stretch | Shift trace | Ricci | encoded Z | lapse Hessian | damping | Total |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| stock \(K\) | \(3/32\) | \(1/32\) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | \(1/8\) |
| target \(K\) | \(-9/32\) | \(9/32\) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| stock \(A_{xx}\) | \(1/16\) | \(1/48\) | \(-1/36\) | \(1/24\) | \(-1/72\) | 0 | 0 | 0 | 0 | \(1/12\) |
| stock \(A_{yy}\) | \(-1/32\) | \(-1/96\) | \(-1/144\) | 0 | \(1/144\) | 0 | 0 | 0 | 0 | \(-1/24\) |
| stock \(A_{zz}\) | \(-1/32\) | \(-1/96\) | \(-1/144\) | 0 | \(1/144\) | 0 | 0 | 0 | 0 | \(-1/24\) |
| target \(A_{xx}\) | \(21/128\) | \(-21/128\) | \(-49/256\) | \(7/64\) | \(21/256\) | 0 | 0 | 0 | 0 | 0 |
| target \(A_{zz}\) | \(9/128\) | \(-9/128\) | \(-9/256\) | 0 | \(9/256\) | 0 | 0 | 0 | 0 | 0 |
| target \(A_{ww}\) | \(-15/128\) | \(15/128\) | \(-25/256\) | \(5/32\) | \(-15/256\) | 0 | 0 | 0 | 0 | 0 |

The independently assembled stock totals equal the directly compiled
GRChombo totals \(1/8,1/12,-1/24,-1/24\). The independently assembled target
totals equal the full-oracle target totals. On this exact GP background the
geometric Ricci, encoded-Z, unit-lapse Hessian, and damping families are
individually zero. Cancellation comes from the target \(K/A\) state, hidden
shift divergence, \(2/d=1/2\) trace correction, and multiplicity-two hidden
traces.

Mutations using stock \(K,\partial_xK\) in the target, omitting hidden shift
divergence, using the \(d=3\) trace coefficient, omitting one hidden trace
copy, or adding false hidden-Ricci/encoded-Z terms all produce nonzero target
residuals and are rejected.

### Complete analytic oracle and constraints

`Stage4AOCAnalyticFullOracle::evaluate` is invoked at zero perturbation on the
exact GP background with stored `hww=1`. Every row is evaluated by the full
formula path, including the zero-valued odd rows:

| Row | Residual |
|---|---:|
| \(\chi\) | `0.00000000e+00` |
| \(h_{xx}\) | `0.00000000e+00` |
| \(h_{xz}\) | `0.00000000e+00` |
| \(h_{zz}\) | `0.00000000e+00` |
| \(h_{ww}\) | `0.00000000e+00` |
| \(K\) | `0.00000000e+00` |
| \(A_{xx}\) | `-1.35525272e-20` |
| \(A_{xz}\) | `0.00000000e+00` |
| \(A_{zz}\) | `3.38813179e-21` |
| \(A_{ww}\) | `-6.77626358e-21` |
| \(\Theta\) | `-6.77626358e-21` |
| \(\hat\Gamma^x\) | `-5.42101086e-20` |
| \(\hat\Gamma^z\) | `0.00000000e+00` |

A separate four-dimensional covariant construction using the oracle’s full
metric, connection, extrinsic-curvature values, and derivatives gives

| Constraint | Residual |
|---|---:|
| Hamiltonian | `-2.71050543e-20` |
| \(M_x\) | `0.00000000e+00` |
| \(M_z\) | `0.00000000e+00` |

Injected nonzero contributions to \(h_{xz},A_{xz},\hat\Gamma^z,M_z\) are each
detected. No `require_close(0,0)` proxy remains.

### Separate second-order stencil study

The older GP preflight remains only a discrete stencil study. It is not the
source of the analytic all-row claim.

| Intervals | \(dx\) | Maximum residual | Worst row | Worst \(x\) |
|---:|---:|---:|---|---:|
| 256 | `1.36718750e-02` | `1.50517791e-02` | \(K\) | `5.27343750e-01` |
| 512 | `6.83593750e-03` | `4.28846373e-03` | \(K\) | `5.13671875e-01` |
| 1024 | `3.41796875e-03` | `1.14620667e-03` | \(K\) | `5.06835938e-01` |
| 2048 | `1.70898438e-03` | `2.96399955e-04` | \(K\) | `5.03417969e-01` |

Pairwise orders are `1.81140120`, `1.90359372`, and `1.95125006`.
The unmodified lapse target remains exactly
\(-3\lambda=-1.06066017\) at the comparison point.

## Fixed GP-holding lapse source

The directly compiled gauge equation produces the raw drift
\(-3\lambda\). A test-only source

\[
S_\alpha(x)=3\sqrt{r_0/x^3}
\]

cancels that zeroth-order drift exactly. A real test-only adapter owns this
operation and updates only `rhs.lapse`. It is not production-wired.

The same locked \((r_0,x)\) was evaluated while perturbing independently all
20 evolved-field coordinates: \(\chi\), four stored \(h\) components, \(K\),
four stored \(A\) components, \(\Theta\), both stored hatted-Gamma components,
lapse, three shift components, and three \(B\) components. Every source value
was bitwise unchanged and every numerical Fréchet derivative was zero:

| Check | Result |
|---|---:|
| fields tested | `20` |
| maximum correct-source field Jacobian | `0.00000000e+00` |
| deliberately wrong \(S_\alpha=2K\) derivative | `2.00000000e+00` |

With locked \(r_0\) unchanged, varying a separate mock evolving horizon
radius leaves the correct source unchanged. A mutated source using that
horizon radius changes and is rejected. Mutated adapters that write into
`shift[0]` or `B[0]` fail output-ownership checks. Wrong-sign and
missing-factor-three adapters also fail through their actual lapse outputs.

## Discrete setup convergence

The same analytic stock-visible GP profiles were sampled away from the axis
and singularity. The custom path calls
`make_centered_derivative_jet`; the GRChombo path calls the actual
`FourthOrderDerivatives` kernels. Their finite-grid values are not expected
to be equal.

| \(N\) | custom \(D_x\) error | custom \(D_{xx}\) error | GR \(D_x\) error | GR \(D_{xx}\) error | custom visible-RHS error | GR visible-RHS error |
|---:|---:|---:|---:|---:|---:|---:|
| 32 | `1.50450394e-04` | `2.41791742e-04` | `2.37823765e-07` | `3.68047782e-07` | `2.30206315e-05` | `2.07911240e-08` |
| 64 | `3.76014660e-05` | `6.04307067e-05` | `1.48433589e-08` | `2.29723138e-08` | `5.75462674e-06` | `1.29780608e-09` |
| 128 | `9.39967097e-06` | `1.51065995e-05` | `9.27385224e-10` | `1.43867229e-09` | `1.43862350e-06` | `7.07530146e-11` |
| 256 | `2.34987428e-06` | `3.77658491e-06` | `5.79540305e-11` | `9.75228787e-11` | `3.59653794e-07` | `2.13052076e-11` |

| Quantity | Observed order |
|---|---:|
| custom \(D_x\) | `2.00018683` |
| custom \(D_{xx}\) | `2.00017965` |
| GRChombo \(D_x\) | `4.00089876` |
| GRChombo \(D_{xx}\) | `3.96062158` |
| custom visible RHS | `2.00005826` |
| GRChombo visible RHS, \(N=32\) through 128 plateau | `4.09948021` |

The finest GRChombo RHS error (`2.13052076e-11`) is reported separately
because cancellation/roundoff ends the clean fourth-order plateau. No
tolerance or convergence order was changed after observing the result.

## Perturbation initialization convention

The test-only convention is

\[
k_n=2\pi n/L,\qquad \delta U=\epsilon u(x)\cos(k_nz)
\]

for scalar/even slots in \(P_+\), with one-\(z\) slots
`hxz`, `Axz`, and `hatGamma_z` assigned sine. \(P_-\) swaps the sine/cosine
assignment. The fixture uses \(n=2,L=7,\epsilon=10^{-4}\), checks all thirteen
frozen slots and zero gauge perturbations, and deliberately leaves the
physical radial profile `UNLOCKED`.

## Adaptation decision

Before the first unperturbed production evolution:

1. pin the GRChombo, Chombo, container, compiler, and dimension tuple;
2. implement a reviewed `CH_SPACEDIM=2`, `GR_SPACEDIM=4` state adapter with
   normalized `hww/Aww` representatives and no visible-\(y\) alias;
3. add exact GP initial data through GRChombo `BoxLoop` ownership;
4. add the fixed, background-only lapse source at the lapse equation only;
5. adapt the hidden/cartoon RHS, hidden-aware cleanup, and constraints;
6. configure the compact periodic \(z\) domain and production ghost exchange;
7. add raw and source-held background residual diagnostics; and
8. pass the Level-1--3 target-dimension gates before invoking GRChombo time
   integration.

No final agent-capability score is assigned.
