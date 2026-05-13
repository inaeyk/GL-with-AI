# 5D Black-String Reproduction Plan

## Purpose

The immediate goal is to reproduce the diagnostic backbone of the 5D Gregory-Laflamme black-string evolution in Pau / Figueras-Andrade-Franca-Gu style simulations before attempting gravitational radiation extraction.

This is the living Markdown plan. The PDF in `research_plan/archived_pdfs/` is an archived snapshot.

Target outputs:

1. Apparent-horizon area versus time.
2. Minimum string radius versus time.
3. Logarithmic pinch-off scaling.
4. Fourier coefficients of the horizon radius on string segments.
5. Generation formation times and geometric properties.
6. Convergence of all diagnostics.

The first benchmark is `L/r0 = 10`, followed by `L/r0 = 8, 12, 16`. Only after those diagnostics are robust should the project move to radiation extraction.

## Prerequisite Status

This document describes the science target for reproducing Pau / Figueras-Andrade-Franca-Gu diagnostics. It is not an instruction to begin production simulations immediately. Implementation requires completing Stages 1-4 in [project_outline.md](project_outline.md) first: GRChombo source/capability map, editable build/`BlackStringToy` skeleton, black-string initial data and GL perturbation, and 4+1 CCZ4/SO(3) cartoon implementation. Do not begin Phase 1 `L10_med` simulations until Stage 4 is verified.

## Baseline Physical And Numerical Setup

Physical system:

- 4+1-dimensional vacuum GR.
- One compact string direction, `z ~ z + L`.
- Uniform 5D black string with Schwarzschild mass parameter `r0`.
- Small Gregory-Laflamme-triggering perturbation near the horizon.
- Production reproduction units: `G = c = 1`, `r0 = 1`.
- Primary cases: `L/r0 = 8, 10, 12, 16`.

Evolution and gauge:

- Formulation: 4+1 CCZ4.
- Symmetry reduction: SO(3) modified cartoon, reducing the effective problem to 2+1.
- Coordinates: Cartesian/radial cartoon coordinate `x` plus compact coordinate `z`.
- Spatial derivatives: sixth-order finite differences.
- Time integration: RK4.
- Dissipation: sixth-order Kreiss-Oliger for direct reproduction.
- Gauge: 1+log lapse and integrated Gamma-driver shift.
- Damping parameters: `kappa1 = 0.37` with `kappa1 -> kappa1/alpha`, `kappa2 = -0.8`, `c_alpha = 1.3`, `c_beta = 0.6`.
- Initial lapse and shift: `alpha(t=0) = 1`, `beta^i(t=0) = 0`.

Initial data:

- Start from the 5D black string in Gullstrand-Painleve coordinates.
- Perturb the conformal factor near the horizon without changing the ADM mass asymptotically:

  ```tex
  chi = 1 + epsilon sin(2 pi n z / L) exp[-(x/r0 - r0/x)^2].
  ```

- Default values: `epsilon = 0.01`, `n = 1`, `r0 = 1`.
- Use turduckening-style regularization: if `x < eps_cut`, evaluate initial-data fields at `x = eps_cut`, with `eps_cut = 0.1 r0`.
- The perturbation intentionally introduces small constraint violations; first verify that CCZ4 damping suppresses them before GL growth dominates.

Domain, boundaries, and AMR:

- Outer radial boundary: `x_outer = 256 r0`.
- Compact direction: periodic `z` with period `L`.
- Outer `x` boundary: Sommerfeld or periodic, provided it is not causally connected to the string during the relevant evolution.
- Baseline coarsest spacing: `h = 0.25 r0`.
- Refinement: 13 total levels, 2:1 refinement ratio.
- Prefer rectangular refinement boxes for reproduction, preserving the physical `Z2` symmetry of relevant string segments.
- Require at least 40 grid points across the thinnest apparent-horizon radius; aim for 60+ when feasible.

Horizon representation:

```tex
x = h(t,z), z in [0,L).
```

For each horizon time slice, save:

- `t`, horizon points `z_j`, and `h(t,z_j)`.
- Metric components on the horizon: `gamma_xx`, `gamma_xz`, `gamma_zz`, `gamma_ww`.
- Areal radius `R(t,z_j)`.
- Proper-length integrand along `z`.
- Optional Kretschmann scalar on the horizon.
- AMR level at each horizon point.
- Constraint norms near the horizon.

Definitions:

```tex
R(t,z) = sqrt(gamma_ww(t,h(t,z),z)) h(t,z)
```

```tex
dl_z = sqrt(gamma_zz + 2 gamma_xz h_z + gamma_xx h_z^2) dz
```

```tex
A(t) = 4 pi int_0^L R(t,z)^2 dl_z
```

```tex
A0 = 4 pi r0^2 L
```

Use `A/A0`, `t/r0`, and `R/r0` in plots.

Data layout:

```text
runs/
  L10_med/
    checkpoints/
    horizon_raw/
    horizon_processed/
    diagnostics/
    plots/
    logs/
  L10_low/
  L10_high/
  L8_med/
  L12_med/
  L16_med/
```

Near the nonlinear cascade, save horizon diagnostics at least every `Delta t/r0 ~ 0.1`, and more frequently if storage allows.

## Target Diagnostics

### 1. Apparent-Horizon Area `A/A0`

Target: reproduce `A(t)/A0` versus `t/r0` for `L/r0 = 8, 10, 12, 16`, with emphasis on `L/r0 = 10`.

Qualitative benchmark:

- `A/A0` starts near 1.
- A short early gauge-adjustment transient appears.
- Area grows during nonlinear GL evolution.
- For `L = 10`, the final simulated value should be close to `A/A0 ~ 1.37` before termination.
- Larger `L` cases should reach larger final normalized areas.

Algorithm:

1. Interpolate metric components to `x = h(t,z)`.
2. Compute `R(t,z) = sqrt(gamma_ww) h(t,z)`.
3. Compute `h_z` using periodic spectral differentiation or high-order finite differences.
4. Compute `dl_z`.
5. Integrate `A(t) = 4 pi int R^2 dl_z`.
6. Normalize by `A0`.
7. Store `t/r0`, `A/A0`, integration-error estimate, and horizon-finder residual.

Use two area estimators:

- Direct quadrature over raw horizon points.
- Resampled periodic spline or Fourier interpolation, then quadrature.

Validation:

- `L=10` final `A/A0` near 1.37.
- Growth onset and curve shape agree visually with the reference figure.
- Area is monotonic up to small numerical noise.
- All `L` cases start at 1 within perturbative error and have the correct final-area ordering.

### 2. Minimum Radius `R_min`

Target:

```tex
R_min(t)/r0 = min_z R(t,z)/r0
```

This is the raw diagnostic for pinch-off scaling.

Algorithm:

1. Compute `R(t,z)` from the horizon data.
2. Preserve both raw and mildly smoothed versions if filtering is needed.
3. Find all local minima and maxima on the periodic string.
4. Store the full ordered list `{z_min,k(t), R_min,k(t)}`.
5. Track the global minimum and local minima continuously in time.

Validation:

- `R_min` decreases rapidly after nonlinear GL growth begins.
- Late-time behavior is approximately linear in `tc - t`.
- The late-time minimum remains on the finest AMR level and is well resolved.
- For `L=10`, the inferred pinch-off time should be compatible with `tc/r0 ~ 128`, used only as posterior validation.

### 3. Pinch-Off Scaling

Target:

```tex
R_min(t) = a (tc - t)
```

and a log-log plot of `log(R_min/r0)` versus `log((tc - t)/r0)`. The expected log-log slope is near 1, with `a` of order a few `10^{-3}`.

Fitting strategy:

- Fit A: two-parameter late-time fit `R_min = a (tc - t)`, with `a > 0` and `tc > max(t_i)`.
- Fit B: three-parameter power law `R_min = b (tc - t)^p`, checking whether `p` is consistent with 1.
- Fit C: fixed-exponent scan over candidate `tc`, fitting `y_i = a (tc - t_i)` through the origin and minimizing residuals.

Use multiple fitting windows:

- After first-generation formation.
- After second-generation formation.
- Latest 30% of available `R_min` data.
- Threshold windows such as `R_min/r0 < 0.2`, `< 0.1`, and `< 0.05`.

Report the fitting-window spread as a systematic error.

Validation:

- For `L=10`, expect approximately `tc/r0 ~ 128` and `a ~ 0.005`.
- Slope close to 1.
- Stable `tc` under reasonable window changes.
- The tracked minimum is physical, not pointwise numerical noise.

### 4. Fourier Coefficients

Target: reproduce the Fourier analysis of the apparent-horizon radius on unstable string segments, especially ratios `c_k(t)/c_0(t)`.

For `L=10`, expected behavior:

- `c1` dominates.
- Even modes are negligible.
- `c3` and `c5` remain much smaller than `c1` through the relevant interval.
- For larger `L`, higher odd modes become more important.

Primary reproduction convention for the paper-style diagnostic:

- Use the first-generation string segment in the `L=10` case.
- Use the fixed coordinate interval `z in [0, 5 r0]`, or the translated periodic equivalent.
- Use the sine decomposition:

  ```tex
  R(t,z) = c0(t) + sum_{k=1}^{Kmax} c_k(t) sin(pi k z / L_s,1).
  ```

Robustness diagnostic:

- Dynamically identify neighboring extrema bounding a string segment.
- Map the segment to `s in [0,L_seg]`.
- Optionally use proper length.
- Compute a cosine expansion:

  ```tex
  R(t,s) = a0(t)/2 + sum_{k=1}^{Kmax} a_k(t) cos(pi k s / L_seg).
  ```

Implementation:

- Interpolate to a uniform segment grid.
- Use `Kmax = 10` initially and `Kmax = 20` for robustness checks.
- Solve coefficients by discrete least squares.
- Store coefficient ratios, residuals, segment endpoints, and coordinate/proper lengths.

For `L=10`, focus roughly on `t/r0 ~ 100` to `117`, refined by the actual horizon data.

Validation:

- `c1/c0` dominates.
- `c3/c0` and `c5/c0` are smaller.
- Even coefficients are close to zero.
- Fit residuals stay controlled.

### 5. Generation Formation Times And Properties

Target: reproduce generation tables for `L=10` and then `L=8, 12, 16`.

Measure:

- Generation index `i`.
- Formation time.
- Number of satellite bulges per string segment `n_s`.
- String-segment radius `R_s,i`.
- Bulge/equatorial radius `R_h,i` or `R_h,f`.
- String-segment length `L_s,i`.
- Ratios such as `L_s,i/R_s,i` or `R_s,i/L_s,i`.

Feature extraction:

1. Start from `R(t,z)`.
2. Smooth gently if necessary.
3. Find local maxima and minima.
4. Classify maxima as candidate bulges and minima/plateaus as candidate necks.
5. Track extrema across time using periodic distance and radius continuity.
6. Record persistent new local maxima as formation events.

Implement both formation-time definitions:

- Threshold definition: first time `R_bulge(t) >= 1.5 R_surr(t)`. This is mainly for Lehner-Pretorius-style comparison.
- Derivative-based definition: compute `d ln R_h,i / dt`, define `t_p,i` as the absolute maximum, and define `t_n,i` as the first subsequent local minimum. Use `t_n,i` as the primary formation time for paper-style tables.

At `t_n,i`, measure `R_s,i`, `R_h,f`, `L_s,i`, and `n_s`.

Validation:

- First two generations should agree at the percent level when definitions match.
- Later generations should be reported with larger uncertainty.
- Ambiguous events should be labeled rather than forced into precise times.

### 6. Convergence

Target: show that diagnostics are not artifacts of grid spacing, AMR, horizon finding, or post-processing.

Primary convergence case: `L/r0 = 10`, `r0 = 1`.

Resolution set:

- Baseline medium spacing: `h = 0.25 r0` on the coarsest grid.
- Low: `Delta_low = (10/7) h`.
- Medium: `Delta_med = h`.
- High: `Delta_high = (2/3) h`.

Expected rescaling factors:

- `Q_2 ~ 1.87`
- `Q_3 ~ 2.72`
- `Q_4 ~ 3.94`

The reference area convergence is roughly third order, plausible despite sixth-order spatial derivatives because RK4, AMR interpolation, and AMR boundaries reduce the global effective order.

Diagnostics to converge:

- Apparent-horizon area: compare `A_low`, `A_med`, and `A_high` on common times.
- Minimum radius: compare raw, smoothed, and local-fit minima plus `z_min(t)`.
- Pinch-off parameters: compare `a`, `tc`, and optional exponent `p`.
- Fourier coefficients: compare `c1/c0`, `c3/c0`, `c5/c0`, even-mode contamination, and residuals.
- Generation times: compare `t_i`, `t_p,i`, `t_n,i`, and geometric properties.
- Horizon reconstruction systematics: track residuals, estimator differences, Fourier residuals, constraint norms, and points across the thinnest neck.

Acceptance criteria for reproduction:

1. `A/A0` for `L=10` matches the benchmark curve and final value.
2. `R_min(t)` supports a stable finite-`tc` extrapolation.
3. The late-regime log-log slope is close to 1.
4. The `L=10` Fourier analysis shows `c1` dominance and negligible even modes.
5. The first two generation times and radii agree well under matching definitions.
6. Area shows approximately third-order convergence over the tested interval.
7. Other diagnostics are at least qualitatively convergent with quantified uncertainties.
8. Late-generation ambiguities are explicitly labeled.

## Work Schedule

### Phase 1: `L10_med` Preliminary Reproduction

Goal: establish a minimal medium-resolution `L/r0 = 10`, `r0 = 1` run.

Tasks:

- Implement initial data.
- Run a short evolution to verify gauge settling and constraint damping.
- Add apparent-horizon finder output.
- Compute `A/A0` and `R_min/r0`.
- Produce first area and minimum-radius plots.

Deliverable: `L10_med` preliminary reproduction notebook.

### Phase 2: Horizon-Analysis Pipeline

Goal: build reusable post-processing diagnostics.

Tasks:

- Build horizon-analysis library.
- Implement extrema detection and tracking.
- Implement both generation-time definitions.
- Implement Fourier coefficient extraction.
- Implement pinch-off fits.

Deliverable: `horizon_analysis.py` or equivalent, validated on `L10_med`.

### Phase 3: Medium-Resolution `L/r0 = 8, 10, 12, 16` Sweep

Goal: reproduce the multi-`L` diagnostic trends.

Deliverables:

- Multi-`L` area plot.
- Multi-`L` generation table.
- Multi-`L` pinch-off fit table.
- Fourier comparison for clean segments.

### Phase 4: `L10` Convergence Campaign

Goal: validate the main diagnostics against resolution.

Runs:

- `L10_low`
- `L10_med`
- `L10_high`

Deliverables:

- Area convergence plot.
- `R_min` convergence plot.
- Pinch-off parameter convergence table.
- Fourier convergence plot.
- Generation-time convergence table.

### Phase 5: Reproduction Memo

Goal: document the completed reproduction and its limitations.

Include:

- Simulation parameters.
- Post-processing definitions.
- Plots and tables.
- Convergence results.
- Known mismatches.
- Lessons for radiation extraction.

Only after this memo should the project move to wave extraction or radiation diagnostics.

## Notes For Later Radiation Project

The reproduction diagnostics are prerequisites for radiation extraction:

- Apparent-horizon area gives a rough energy-budget check, though not a waveform.
- Generation times define natural radiation windows: first-generation burst, cascade, near-pinch-off regime, and post-pinch-off collision-like dynamics.
- Fourier coefficients identify dominant GL source harmonics.
- Minimum-radius scaling records how close the run reaches the singular local regime.
- Convergence of horizon quantities is necessary but not sufficient for convergent wave extraction.

The next step after reproduction is to add radiation extraction at large radius while preserving these diagnostics in the same run metadata.
