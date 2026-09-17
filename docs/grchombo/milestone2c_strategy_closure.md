# Milestone 2-C strategy closure

Date: 2026-08-13

Classification: `PHYSICAL_RADIAL_AND_CONSTRAINT_STRATEGY_BLOCKED`

## Outcome

The exact M2-C v2 matrix and qualified provenance were frozen before the first
scientific launch. All focused pre-launch gates passed. The first ordered
launch, `medium-near-gp`, then exited nonzero after its last completed sample
at `t=2.5000000000000187`, step 400, with
`M2-B ALGEBRAIC_RECONSTRUCTION_ADMISSIBILITY_FAILURE` on both MPI ranks. The
raw manifest records this failed launch as attempt 1. The contract forbids a
silent retry and requires later launches to stop, so attempts 2 through 12
were not invoked.

This is a numerical-launch failure, not a scientifically insufficient set of
successful runs. The closure is therefore BLOCKED rather than PARTIAL. Human
review is required.

## Frozen campaign authority and provenance

The retained matrix is rooted at repository commit
`1f1ab5c8e4df80ef77e4fc949ed4ee2f1e167d73` and M2-B base commit
`b5970454a649f8cbf46f694ebddcfe191b657530`. The frozen campaign SHA-256 is
`91a1ac71bd932c9ba2de4bc7edfebe1cb347750b103cf7594b7ea08292ac9db3`.
It preserves the locked launch order, domains, resolutions, modes,
amplitudes, sampling, and common comparison region `1.25 <= x <= 3.0`.

Qualified read-only continuation provenance is retained without modifying a
dependency:

- runtime bundle `gl-m2c-v2-locked-37e65952-8684f2e0`, manifest SHA-256
  `cc402e7123b209af1fd62a2cc3a061853393582b134aebfd1dd6ae6c3d25194f`;
- GRChombo `37e659523830418b210acea1661dac0e00bb1b75`;
- Chombo `8684f2e000106f1abadb72642e1d15351867f98f`;
- focused executable SHA-256
  `8b0f82ad3dc9cdf66f2118a40f65151001099d7b1fba4dc920230c08efd03856`;
- engine-qualified transport-helper SHA-256
  `00ecf5d040865b97884c488883abd342581c2a432debe7a54e4646bceee3d2d6`.

The worker-r000 missing-runtime and worker-r001 missing-transport obstructions
are retained as historical events. Both were resolved by the qualified
read-only continuation authority before this execution; neither consumed a
scientific attempt. No network operation occurred.

## Engineering evidence

The two-rank focused parameter preflight ran for all twelve frozen parameter
files with numerical launch count zero. It observed transform rank 23 in every
case and maximum transform condition `26.96654810229121`. Successful completion
of the locked constructor gates covered the nine-fold horizon glancing
degeneracy, Gamma/Z analytic and mutation cases, determinant and weighted-trace
reconstruction, and dependent RHS chain rules. A frozen-source invariant check
plus the two-rank preflight retained the exchange-only MPI-seam policy. Exact
commands, exit codes, stdout, stderr, hashes, and gate records are under
`run_manifests/m2c/prelaunch/`.

No production file changed. The first launch reported two ranks, one box on
each rank, 1,536 globally owned valid cells, one internal MPI seam, and
`seam_policy=exchange_only`. The accepted M2-B production path is unchanged,
so `performance.status = reused_m2b`, slowdown fraction `-0.00116695`, and
`restart_regression_status = reused_m2b` remain the applicable engineering
evidence. They do not override the failed M2-C launch.

## Numerical evidence

`tmp/m2c/raw_manifest.json` and its tracked snapshot record exactly one
attempt: `medium-near-gp`, exit code 1. The exact command, environment,
parameter and executable hashes, full stdout/stderr, ownership table, all
retained field/constraint measurements, radial profiles, status history,
runtime, and peak RSS are under
`run_manifests/m2c/launches/medium-near-gp/` and are hash-addressed by the raw
manifest.

Through the final completed sample at `t=2.5`, the state was finite and
metric-admissible and ownership passed. The final recorded minimum metric
eigenvalue was `0.98379930187514486`, determinant defect
`6.6613381477509392e-16`, weighted-trace defect
`4.4408920985006262e-16`, and maximum state drift
`0.064517964690957275`. Immediately after that sample, both ranks reported the
algebraic-reconstruction admissibility failure and MPI returned exit code 1.
These partial GP data do not meet the required `t_f=8` control gate and are not
used to infer low- or high-k behavior.

Failed gate: `launch_exit_zero:medium-near-gp`. Because launch 1 failed, the GP
control campaign, seeded responses, plateau, resolution, boundary,
half-amplitude, even/odd, high-k separation, and differential-constraint gates
were not completed.

## Physical interpretation

No physical conclusion follows from a failed GP control. This campaign neither
identifies nor excludes a GL mode, makes no critical-wavenumber claim, and does
not begin Milestone 3. G-Physics and overall Checkpoint G remain blocked.

## Fixed checks

The authoritative fixed-check results are recorded in
`run_manifests/m2c/fixed_checks.json`:

- `m2c-summary`: PASS;
- `m2c-retained`: PASS;
- `m2c-hygiene`: PASS.

The checks validate the honest BLOCKED closure, retained numerical provenance,
and workspace hygiene; they do not turn a failed numerical launch into
scientific acceptance.
