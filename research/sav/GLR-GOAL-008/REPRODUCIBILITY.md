# Reproduction and provenance

The independent branch starts at the GOAL-005 scientific source [61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708](https://github.com/inaeyk/GL-with-AI/commit/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708). The pinned Work audit [a9bfbec0efe41b881e4b12aa262f5fcfcd94c639](https://github.com/inaeyk/GL-with-AI/commit/a9bfbec0efe41b881e4b12aa262f5fcfcd94c639) independently reviewed that exact head of [GOAL-005 PR #2](https://github.com/inaeyk/GL-with-AI/pull/2). The audit names its reviewed head explicitly and records the same Z continuum interval quoted in the comparison handoff. The integration specialist verified the local HEAD before the new commit and inspected the audit through GitHub. The audit is an evidence input on a separate branch. GOAL-008 uses the source head as its parent.

## Non-production commands

Run from `research/sav/` with Python 3.14.8, NumPy 2.4.6 and SciPy 1.18.0. Both scripts write structured JSON to standard output. The comparison commands retain checked-in evidence and put replay outputs in a temporary directory.

```sh
glr008_tmp=$(mktemp -d)
for glr008_check in z_massive_wedge flat_retarded_flux; do
    OPENBLAS_NUM_THREADS=1 PYTHONDONTWRITEBYTECODE=1 \
        python3 "GLR-GOAL-008/checks/$glr008_check.py" \
        > "$glr008_tmp/$glr008_check.json"
    diff -u "GLR-GOAL-008/checks/$glr008_check.json" \
        "$glr008_tmp/$glr008_check.json"
done
```

On another BLAS platform, compare numeric values and assertion tolerances if text rounding differs. Each diagnostic has its own assertions. The Casimir arithmetic is a bounded standard-library check. Copy the fenced Python snippet from [the action derivation](STABILIZATION.md) to a temporary script and run it with `python3 -B`. Its three truncations verify the reported numerical tuning, while the signs and existence test come from the derivation. The inherited GOAL-005 interval has its own [commands, precision and evidence](../GLR-GOAL-005/REPRODUCIBILITY.md).

| Executed check | Numerical scope | Evidence |
| --- | --- | --- |
| Finite-slice Z characteristic wedge | Six masses, three fixed tubes and three refinements. Finite signed flux and an independent physical energy balance over the characteristic domain. | [Solver](checks/z_massive_wedge.py), [results](checks/z_massive_wedge.json) |
| Flat half-line retarded response | Manufactured image Green function in time compared to exact outgoing spectral flux, with two quadrature resolutions, finite observation time and three masses. | [Solver](checks/flat_retarded_flux.py), [results](checks/flat_retarded_flux.json) |
| Mass-sign and cutoff controls | Wrong mass sign fails the manufactured waveform and the energy balance. The initial shorter flat integration and lower quadrature order failed the original comparisons, motivating the longer observation and refinement documented in the diagnostic record. | [Controls and tolerances](DIAGNOSTICS.md) |

## Checked inputs

The integration specialist independently recomputed SHA-256 from the working files and the exact pinned Git blobs for all four inputs declared by the wedge JSON. Each matched its saved checksum. Paths are relative to `research/sav/`.

| File | SHA-256 |
| --- | --- |
| `GLR-GOAL-005/compute/early_null.py` | `213dba12d71e83e1c3004b49db23c3d1c9df850f4b4f269e0e47114c60955f3f` |
| `GLR-GOAL-005/compute/early_null_waveform.csv` | `b58cd5251b256db2e4f45531ddd2992d1fab6d9a49c27ecb5d2fc301a0e51f07` |
| `verification/gl_linear_profile_L10.csv` | `15ebc0aa3f13bb91942ac90aeffaaa234884df58d6ffc7ad874ed44e5064b64b` |
| `verification/gl_linear_results.json` | `fc6b46b8c958bcc217e74f4370120d3995189536e865d6da6de139aa29c5687f` |

The wedge shares the empirical radial source interpolation and massless point-sample data with the prior numerical calculation. Its characteristic discretization, finite-tube observables and balance are separate checks. The mass-dependent outputs are floating diagnostics. The inherited continuum enclosure applies to the specified massless null observable alone. The independent analytic reconstruction is in [the verification record](INDEPENDENT_REVIEW.md).

The coordinator replayed both final commands using the listed dependency versions. Each generated JSON matched the saved output byte for byte. This verifies the diagnostic package with the pinned inputs and the executed software environment.
