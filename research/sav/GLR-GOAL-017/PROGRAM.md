# Inputs and authority

The owner launched GOAL-017 with the Work program and charter. The
[charter](GOAL.md) is byte-for-byte the selected launch file. The owner's
launch additionally authorizes the draft source PR against
`codex/glr-goal-014-certified-interference`. The designated write scope is
`research/sav/GLR-GOAL-017/` on `codex/glr-goal-017-physical-ringdown`.

| Work file | SHA-256 |
| --- | --- |
| `GLR-015-017-research-program.zip` | `72f0fec8ff93c6aa7e79cd7731ae9decb9fb80f417da4f1ef56ad1ca633a6764` |
| `PROGRAM.md` | `bd1854f31f14aca4dc9d054cbcd9d5182ead36d16cccb2d0153c3c44c5d3f53f` |
| `GLR-GOAL-017-LAUNCH.md` | `8697d479221c491be3a3c02c5cf6d8deb2c085a6c8825ee552e48a5a09502a62` |
| `GLR-GOAL-015-LAUNCH.md` | `735c17acb0c2cb792dd994cbc4d24165b0318e15893770f7d4576fa6bf66963b` |
| `GLR-GOAL-016-LAUNCH.md` | `0033eefbb8a05d0aa38288233bbc663ab927feaeaff4e129da739d32a7dce70e` |

The four extracted files were compared byte-for-byte with their ZIP entries.
This local overview records the GOAL-017 portions of the external program.
GOAL-017 keeps the 10-hour elapsed, 5-CPU-hour aggregate, two-thread and
4 GiB per-goal ceilings and at most two substantial refinements per selected
strong estimator. Up to 3.5 CPU-hours are allocated to the full waveform,
1 to independent verification and 0.5 to the comparison. All computations
here are non-production fixed-background perturbation calculations or small
manufactured controls.

| Record | Exact frozen input | Use |
| --- | --- | --- |
| 005 source | [61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708](https://github.com/inaeyk/GL-with-AI/commit/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708) | Inherited L10 spectral and constrained spherical preparation |
| 007 source | [0864d9d807de56cbdd6e64d215e6295508a3007f](https://github.com/inaeyk/GL-with-AI/commit/0864d9d807de56cbdd6e64d215e6295508a3007f) | Ordered axial and optical results |
| 011 source | [eb439228601a649c658ce719b503ff0bdc37a83f](https://github.com/inaeyk/GL-with-AI/commit/eb439228601a649c658ce719b503ff0bdc37a83f) | Constraint, exact-curl hierarchy and physical ordinary pole |
| 011 audit | [75cb3ff3feef98164a65cf14105be6df77ea809c](https://github.com/inaeyk/GL-with-AI/pull/14) | Independent residue qualifications, outside ancestry |
| 012 source | [87fd711bf2e8117467caad1ff52c1cd344966974](https://github.com/inaeyk/GL-with-AI/commit/87fd711bf2e8117467caad1ff52c1cd344966974) | Physical translated pole and contour qualifications |
| 012 audit | [ae8c323e1e87abd0fff26eac75b05b701a7e7b1a](https://github.com/inaeyk/GL-with-AI/pull/16) | Independent windows and inversion qualifications, outside ancestry |
| 013 source | [1e8e628f68af4cab174fcc44b81af4748a22115c](https://github.com/inaeyk/GL-with-AI/pull/17) | Causal wedge, physical source and exterior transfer |
| 013 audit | [26a09054366933a197c34fa67f617af7da76e48d](https://github.com/inaeyk/GL-with-AI/pull/18) | Causal and cap review, outside ancestry |
| 014 source | [0b4bfcbb978b52626fb0816874df06055703c195](https://github.com/inaeyk/GL-with-AI/pull/19) | Sole branch base, sharp-cut adjoint, early compact pilot |
| 014 audit | [84c9824f826e3517c8a1ed74d96bea061ab88584](https://github.com/inaeyk/GL-with-AI/pull/20) | Independent audit and counterexamples, outside ancestry |
| 015 source | [65a4af854a25ca02e40f31b00fe5ff789935bdc4](https://github.com/inaeyk/GL-with-AI/pull/21) | Read-only transfer eligibility and short-window methods |
| 015 audit | [1089b3cb36eef1c25278c4fbfe3e843bfdcff6e4](https://github.com/inaeyk/GL-with-AI/pull/22) | Scope and transfer qualifications, outside ancestry |

GOAL-006, 008, 009 and 010 are side problems. Scalar stabilization, nonlinear
spherical trace hypotheses and massive spin-two transport do not change the
ordered vacuum tensor problem here. The finite-seed comparison explicitly
retains its own uniform trace hypothesis.

Use vacuum Schwarzschild times a circle, L/r0 = 10, kr0 = pi/5, the
inherited Omega r0 in [0.06328118, 0.06328120], and the unhalved real
cosine normalized by H_theta theta/(2 r^2) = 1 at r/r0 = 2 in static time.
The mixed evolution carries the actual +k coefficient, paired with its
conjugate in the mean. The fixed real ell = 2 axial packet starts at
R/r0 = 13/8 on v = r, with width 3/25 and zero independent upper canonical
pairs. Constraint-induced upper velocities, both mixed polarizations, direct
quadratic terms and the fixed-mass mean enter the return. Use the common
Einstein-frame inertial Bondi clock, fixed ADM mass, circumference and
physical cuts. [Physical data](../GLR-GOAL-011/FINITE-DATA.md) ·
[Sources and contacts](../GLR-GOAL-013/SOURCE-LEDGER.md) ·
[Frame and flux](../GLR-GOAL-014/FLUX-AND-FRAME.md).
