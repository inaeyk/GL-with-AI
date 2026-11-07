# Verification record

| Check | Executed result | Scope |
| --- | --- | --- |
| Full point source replay | All five radii passed. All 35 Ricci intervals equal the original output exactly. Coordinate and warped-product checks agree on every component. The output also includes all trace-reversed intervals. | Shared pinned spectral input. Independent curvature implementations and analytic compact-normal normalization. The point Bianchi interval test is weaker than the on-shell analytic identity. |
| Source negative controls | Missing cosine halves fails the absolute \(Q_{zz}\) test. Omitted \(Q_{zz}\) fails differentiated Bianchi. Two polar angles satisfy the warped angular identity. | Interval implementation checks at five points. |
| Spatial data | Direct geometric lapse and momentum expansion yields \([0,0,0]\) differences at \(r=2\) for symbolic jets. Omitted drift, wrong momentum factor and reversed damping are detected. | Independent algebraic check at one radius. No evaluated radial Cauchy profile. |
| Time-domain master | All six identities reduce exactly to zero for arbitrary EF tensor functions. Missing mixed derivative and reversed radial Ricci term detected. | Separate warped-product implementation. Physical compatibility comes from the exact Cauchy development. |
| Affine master | Master and four reconstruction identities passed coefficient-wise with arbitrary separated tensor functions. | Distinct coordinate variation. The separated symbol is polynomial in the time parameter. |
| Causal and energy controls | Exact oscillator response and jet, Laplace source-pole cancellation and missing-tilt mutation, two rational potential identities and 18 positive coefficients. | Manufactured and symbolic. The potential is negative near the horizon despite the positive form. |
| Nonzero correction | Independent check of Fourier factors, slice gauge, and both separated-tail cases. | Analytic result for both Z preparations. Absorption versus escape unresolved. |
| Physical current | Independent action normalization, orientation, TT polarization and compact/Fourier factors. | Analytic identity and asymptotic comparison. No evaluated four-term budget. |

The publications [reproduction commands and versions](REPRODUCIBILITY.md) and [symbolic output](checks/symbolic_replay.txt) document all checks executed for closure. The original analysis also used primary-source checks listed in [scattering](SCATTERING.md). AI checks and the owner-reported preliminary review are separate from the forthcoming independent audit of this pinned source. The earlier GOAL-005 and GOAL-006 Work audits are inherited evidence at the exact SHAs in [goal](GOAL.md).
