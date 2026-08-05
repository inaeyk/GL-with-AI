# M2-B cycle-1 authoritative evidence replay

Date: 2026-08-05

Result: `M2-B CYCLE 1 PASS — authoritative retained evidence qualified within
16-attempt ceiling`

This replay qualifies the accepted cycle-1 implementation; it did not alter
the production mathematics, production source, or fixture source. It is an
evidence-recovery allowance, not a production repair cycle, and it does not
start M2-C.

## Immutable provenance

The retained bundle is
`/home/inaeyk/researchrepo/evidence/GL-with-AI/m2b-cycle1-final/` and must remain
intact through independent audit and commit.

- branch: `supervised/m2b-cycle1`;
- base and `HEAD`: `7d04b5b9882dcd476c1457b8d711ac7b5520b2c1`;
- implementation-diff command:

  ```bash
  git diff --binary --no-ext-diff \
    7d04b5b9882dcd476c1457b8d711ac7b5520b2c1 -- \
    code/BlackStringToy \
    | sha256sum
  ```

- command output: `a48f4e8e382e14d7591966151ce6ac1a282393d4c14c580d77639bab7e2ce0eb  -`;
- GRChombo: `37e659523830418b210acea1661dac0e00bb1b75`, clean;
- Chombo: `8684f2e000106f1abadb72642e1d15351867f98f`, clean;
- GCC/GFortran 15.2.0, Open MPI 5.0.10, parallel OpenMPI HDF5 1.14.6.

The bundle retains the complete pre-build status and changed-path lists,
binary diff and per-production-source hashes, exact build commands and logs,
runner source, parameter files, launch scripts, executables, hashes,
stdout/stderr, raw HDF5 and rank state dumps, case manifests, canonical dumps,
postprocessed tables, and timestamps/statuses. The evidence-only runner uses
the production `CH_SPACEDIM=2`, `GR_SPACEDIM=4` boundary compilation scope; its
source and build-rule diff from the earlier retained evidence runner are
preserved in `provenance/final16/`.

| Retained executable | Bytes | SHA-256 |
|---|---:|---|
| accepted M1 MPI/no-HDF5 | 1,803,560 | `e193223ab598599ab03f3ed14eb9eb5ecf5394dd08b1693986bad418c445e67d` |
| final16 M2-B MPI/no-HDF5 | 1,918,376 | `fcc32014d1b7258c310cded66809129c51baff30b1442d35867c92f305c5ed85` |
| final16 M2-B MPI/HDF5 | 2,119,992 | `1c466dcd737000fee00e7b3a0c811f8066db5fbc9df14508a74a161de3e1d2de` |

The corrected evidence parameters contain the locked 18-slot parity array
`0 0 4 0 0 0 0 4 0 0 0 1 2 0 1 2 1 2`, in the exact order
`chi,hxx,hxz,hzz,hww,K,Axx,Axz,Azz,Aww,Theta,GammaX,GammaZ,lapse,shiftX,shiftZ,Bx,Bz`.
Static required-key and array-length checks and the runner's parameter-only
mode passed for every Launch 14–16 parameter file without AMR definition or
evolution. Output directories existed before launch. Parameter hashes are:

| Parameter | SHA-256 |
|---|---|
| Launch 14 Case A | `52b8be9d919d906e7a77575287d012354586cef04ef5944001cb4ed4613180dc` |
| Launch 14 Case B | `04fc9b60e69233cd7211c33dca09dd0b6232777e447f681316d8df514eafddb6` |
| Launch 15 | `3d0f3f872c3d0c8a52e70fa726bd03e4265c24801b7d8d123cb60f0cd1e064c2` |
| Launch 16 Case A | `1ca68413c4faeae49e23eeb92053375db3e693e630f8a97ff325a42ea47ee7ad` |
| Launch 16 Case B | `8e3d13847ac8afaf82781380cb70915268a6a0483b5d67a25b58897d1cb0caa6` |

## Exact launch accounting

The human-approved final ceiling is 16 outer attempts. It is exhausted: no
launch 17 is permitted.

| Launches | Retained accounting |
|---|---|
| 1–10 | historical worker-reported activity; complete raw accounting is not independently recoverable and hidden retries cannot now be disproved |
| 11 | pre-evolution evidence-configuration abort at `Box.H:2044`, exit 134; no usable timing or state |
| 12 | accepted-M1 MPI-2 timing baseline, exit 0 |
| 13 | pre-evolution parameter-validation abort, exit 134, because `vars_parity` was absent; no AMR setup, initializer, RK stage, RHS evaluation, checkpoint, or state output |
| 14 | one outer invocation, Case A matched M2-B timing and Case B medium continuous/HDF5 reference; both cases and outer runner exit 0 |
| 15 | current-checkpoint MPI-2 restart from step 25 through step 50, exit 0 |
| 16 | one outer invocation, Case A fine GP and Case B corrected-seed smoke; both cases and outer runner exit 0 |

There was no retry of Launches 14–16 and no hidden retry in the retained
11–16 sequence. Historical uncertainty for Launches 1–10 is not erased. Total
outer attempts are exactly `16/16`.

## Previously accepted focused gates

The focused implementation audit remains qualified and was not rerun during
this replay. It records transform roundtrip, reduced rank 23 and conditioning
below `10^3`, both-normal classification and nine-fold horizon glancing,
Gamma/Z absolute/normalized/refinement agreement, sign/multiplicity/normal/
coupling mutation rejection, exact determinant/trace reconstruction,
dependent-row chain rule, all 18 ghost slots across three layers without
valid-cell overwrite, and MPI seams as exchange-only. The boundary fixed-point,
incoming/outgoing physical, constraint and gauge controls, glancing treatment,
outer lifecycle, MPI ownership, and all-slot parity fixtures end in
`BLACKSTRING_M2B_CYCLE1_FOCUSED_PASS`. No failed characteristic or Gamma/Z gate
was bypassed.

## Matched performance: Launch 12 versus Launch 14 Case A

Only the no-HDF5 timed windows below are compared. Both use MPI-2, 256x512
valid cells, the same domain and 32-box layout, CFL 0.05, `ko_sigma=0.3`, 25
completed steps, steps 0–4 as warm-up, and steps 5–24 as the sole timed
interval. Diagnostics and plot/checkpoint output are disabled.

| Measure | M1 Launch 12 | M2-B Launch 14A |
|---|---:|---:|
| timed duration, steps 5–24 | 7.964000000 s | 7.954706445 s |
| seconds/RK step | 0.398200000 | 0.39773532225 |
| valid cells | 131,072 | 131,072 |
| RHS stages | 80 | 80 |
| valid-cell/RHS evaluations | 10,485,760 | 10,485,760 |
| seconds/valid-cell/RHS | `7.595062255859375e-7` | `7.586199231147767e-7` |
| throughput, valid-cell/RHS/s | 1,316,644.902 | 1,318,183.150 |
| peak RSS ranks 0/1 | 77,756 / 77,920 KiB | 78,668 / 78,668 KiB |
| estimated total rank peak | 155,676 KiB | 157,336 KiB |
| executable size | 1,803,560 B | 1,918,376 B |
| radial boundary calls | none | 151 solution fills/face and 100 dependent-RHS surface calls/face |

M2-B runtime overhead is `-0.116695%` (slightly faster in this sample), peak
per-rank RSS overhead is `0.959959%`, estimated total RSS overhead is
`1.066317%`, and executable-size overhead is `6.366076%`. The runtime gate is
therefore below the allowed 15% slowdown.

## Medium and fine GP evolution

Launch 14 Case B evolved the 256x512 GP state from step 0, dumped step 25,
wrote a 22,590,504-byte checkpoint, and continued to step 50. Its final state
is finite and has canonical SHA-256
`c01858b4b871d3749b7d67ecdf03d762305b3c9770c1b276e8be8a7860493513`.
Launch 16 Case A evolved the 512x1024 control for 50 half-size steps to the
same physical time as medium step 25; its final canonical SHA-256 is
`e1acce94c19caa4a6265f95d3524232e299b5f12796791d22157d4658040299a`.

The first four numerical columns compare the common time
`t=0.01953125`; the last two retain the medium continuous endpoint at
`t=0.0390625`. `L2` is radial-`x^2` weighted.

| Field | Medium t25 Linf | Medium t25 L2 | Fine t50 Linf | Fine t50 L2 | Medium t50 Linf | Medium t50 L2 |
|---|---:|---:|---:|---:|---:|---:|
| chi | 1.145095e-08 | 3.330565e-10 | 7.203896e-10 | 2.082692e-11 | 7.874711e-09 | 3.090450e-10 |
| hxx | 7.958867e-08 | 1.979266e-09 | 4.967021e-09 | 1.229722e-10 | 1.501191e-07 | 4.019990e-09 |
| hxz | 2.259757e-17 | 8.075142e-18 | 2.354550e-16 | 1.369351e-16 | 4.943890e-17 | 1.654063e-17 |
| hzz | 1.145603e-08 | 3.330755e-10 | 7.203580e-10 | 2.082637e-11 | 7.871655e-09 | 3.089654e-10 |
| hww | 3.406632e-08 | 8.246605e-10 | 2.141807e-09 | 5.117434e-11 | 7.353165e-08 | 1.872408e-09 |
| K | 2.534373e-06 | 4.858844e-08 | 1.906090e-07 | 3.097955e-09 | 5.279558e-06 | 1.004731e-07 |
| Axx | 1.052531e-06 | 2.500234e-08 | 6.484394e-08 | 1.492528e-09 | 1.327203e-06 | 3.676513e-08 |
| Axz | 8.495919e-16 | 3.003273e-16 | 1.248038e-14 | 6.188106e-15 | 1.050387e-15 | 4.014674e-16 |
| Azz | 6.210491e-07 | 1.185619e-08 | 4.695035e-08 | 7.568899e-10 | 1.317362e-06 | 2.492512e-08 |
| Aww | 6.230420e-07 | 1.514476e-08 | 3.797337e-08 | 9.132338e-10 | 8.578885e-07 | 2.347135e-08 |
| Theta | 1.116016e-07 | 1.582357e-09 | 4.783849e-09 | 7.745698e-11 | 1.715926e-07 | 3.915790e-09 |
| GammaX | 1.390884e-06 | 3.590833e-08 | 8.670900e-08 | 2.162996e-09 | 1.555443e-06 | 4.554171e-08 |
| GammaZ | 6.124555e-16 | 7.072705e-17 | 1.621702e-15 | 5.009789e-16 | 7.889684e-16 | 1.038600e-16 |
| lapse | 4.227993e-08 | 8.606189e-10 | 3.091914e-09 | 5.415097e-11 | 1.682672e-07 | 3.272840e-09 |
| shiftX | 1.046325e-08 | 2.706252e-10 | 7.070440e-10 | 1.746153e-11 | 3.042975e-08 | 8.522963e-10 |
| shiftZ | 2.469194e-18 | 5.092537e-19 | 1.572794e-17 | 4.381984e-18 | 6.962685e-18 | 1.447837e-18 |
| Bx | 1.376114e-06 | 3.551372e-08 | 8.574076e-08 | 2.138831e-09 | 1.515992e-06 | 4.436901e-08 |
| Bz | 6.092905e-16 | 7.024213e-17 | 1.614890e-15 | 4.976496e-16 | 7.803381e-16 | 1.025901e-16 |

| Metric | Medium t25 Linf | Medium t25 L2 | Fine t50 Linf | Fine t50 L2 | Medium t50 Linf | Medium t50 L2 |
|---|---:|---:|---:|---:|---:|---:|
| H | 9.052116e-06 | 1.177801e-07 | 4.048441e-07 | 5.409892e-09 | 1.775356e-05 | 3.328538e-07 |
| Mx | 7.599936e-05 | 1.814963e-06 | 5.190139e-06 | 1.146202e-07 | 6.002109e-05 | 1.422851e-06 |
| Mz | 1.022647e-13 | 2.026145e-14 | 5.376859e-13 | 1.596976e-13 | 6.507159e-14 | 2.447772e-14 |
| Zx | 5.299908e-07 | 1.343316e-08 | 4.410144e-08 | 8.806099e-10 | 7.459848e-07 | 1.771221e-08 |
| Zz | 2.186628e-15 | 1.026981e-15 | 5.418966e-15 | 2.147405e-15 | 3.048765e-15 | 1.122629e-15 |
| det | 4.440892e-16 | 2.020941e-16 | 4.440892e-16 | 1.992375e-16 | 4.440892e-16 | 1.848697e-16 |
| trace | 8.881784e-16 | 3.720382e-17 | 4.440892e-16 | 3.549433e-17 | 4.440892e-16 | 3.697907e-17 |

The medium/fine comparison shows improvement in every non-roundoff GP field
and in `H`, `Mx`, and `Zx`; odd-sector `Mz,Zz` values remain at roundoff scale.
This is consistent control, not a claimed convergence order. Minimum metric
eigenvalues are `0.9999999264683507` (medium step 50) and
`0.9999999978581932` (fine); maximum conditions are `1.0000002236507424` and
`1.0000000071088275`.

The deterministic state analysis finds worst spurious compact-direction
content `3.908e-14` over medium steps 25–50 and `8.260e-14` over fine steps
0–50, zero compact Nyquist projection, and worst radial-Nyquist amplitudes
`5.568e-9` and `2.155e-10`, respectively. Worst parity defects are absolute
roundoff (`2.101e-15` medium and `2.496e-14` fine). No boundary-localized or
radial-grid-scale growth is apparent over the bounded intervals. Medium and
fine each distribute their 64x64 boxes across both ranks; all internal MPI
seams are exchange-only. Physical low/high radial-face counts are 301/301
solution fills and 200/200 dependent-RHS surface calls for medium step 50 and
the same counts for fine step 50.

## Checkpoint/restart equivalence

Launch 14 Case B's step-25 checkpoint has SHA-256
`50e69608484d7ddcdd856a04708b8ec5239229c76eb61a79537c8cc798465035`.
Launch 15 bypassed initialization, restored step 25,
`t=0.019531250000000007`, all 18 fields, the full 32-box layout and both MPI
ranks, then resumed exchange and characteristic boundary operations to step
50, `t=0.039062499999999986`. Load balancing assigned the same box geometry in
a different rank order; this did not change the state.

| Comparison | Canonical SHA-256 | All-field Linf | All-field weighted L2 | Result |
|---|---|---:|---:|---|
| continuous step 25 vs restored step 25 | `ccde97bfe2bdf0d10aeb3dfeaf12a1ae38a93c7a8d5bacac81cb1403fd798704` | 0 | 0 | bitwise equal |
| continuous step 50 vs restarted step 50 | `c01858b4b871d3749b7d67ecdf03d762305b3c9770c1b276e8be8a7860493513` | 0 | 0 | bitwise equal |

There is no nonzero worst variable/cell. Constraints, determinant, and trace
are consequently identical cellwise. The restart segment records 150
solution fills and 100 dependent-RHS surface calls per physical face, with
internal seams exchange-only.

## Corrected seed and smoke

Launch 16 Case B used mode 1, amplitude `1e-9`, 32 radial by 16 compact cells,
and the fixed four-cell boundary buffer. The retained SVD has rank 148,
largest retained singular value `1.1034597014132007e3`, smallest retained
singular value `3.3104151099015635e-4`, first rejected singular value
`3.730214499239455e-10`, and condition estimate
`3.333297078401783e6`. All 161 singular values are retained in raw stdout and
the postprocessed JSON.

| Constraint | Before max | Before RMS | After max | After RMS |
|---|---:|---:|---:|---:|
| H | 1.266169e-08 | 7.439015e-09 | 4.080478e-20 | 1.190968e-20 |
| Mx | 1.317595e-09 | 4.502622e-10 | 6.007326e-20 | 1.174226e-20 |
| Mz | 1.438474e-10 | 8.077561e-11 | 6.753052e-20 | 1.586043e-20 |
| Zx | 1.110856e-09 | 5.539144e-10 | 2.081899e-20 | 3.704686e-21 |
| Zz | 1.824466e-09 | 1.109856e-09 | 8.613614e-20 | 1.629878e-20 |

The scaled residual RMS is `1.2733020782208691e-20`, the unscaled linear
solve residual is `1.173448827790891e-10`, amplitude renormalization is
`7.340426142500801e-10`, physical-norm retention is
`1.5863098886000198`, amplitude error is zero, and the amplitude-level buffer
defect is zero. Determinant and weighted-trace identities are
`2.220446049250313e-16` and `1.1102230246251565e-16`. Initial single-mode
purity is `0.9999999999999933`, final purity is
`0.9999999999997801`, parity-leakage power is `1.09e-32` initially and
`2.02e-16` finally, and smoke mode-amplitude retention is
`0.9997011426945674`. The final state SHA-256 is
`add7e5eaf7132a8b1e96e62bf9b110b403eb0003750efefe7058b70585251f8c`.

The MPI-2 smoke reached `t=5e-4`, remained finite, and retained minimum metric
eigenvalue `0.9999981924710714` and maximum condition
`1.000007179486997`. The coarse full-state diagnostic includes the background
GP discretization error (`Mx` changes from `0.3053077` to `0.3036375`); the
locked correction gate is the separately retained differential five-
constraint residual table above. No invalid metric, buffer corruption,
boundary-localized growth, or radial-grid-scale growth was observed. This
short seed smoke is not M2-C physical-mode evidence.

## Qualification and limitations

Cycle 1 qualifies implementation, bounded GP evolution, current-checkpoint
restart, matched performance, MPI ownership, and corrected-seed smoke within
the `16/16` ceiling. It does not establish an M2-C growth rate, nonlinear
physics, broad-resolution convergence order, long-time stability, AMR, or
apparent-horizon behavior. Launches 1–10 remain historically incomplete, and
their hidden retry history cannot be disproved. The external bundle, rather
than deleted older temporary files, is the authoritative replay evidence.
