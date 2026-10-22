# GOAL-002 executed verification record

2026-10-22. Non-production symbolic and bounded scalar 1D work only.
Python 3.14.8; SymPy 1.14.0; NumPy 2.4.6; SciPy 1.18.0;
mpmath 1.3.0. No dependency changes. All scripts/results below are in
research/sav/verification/. Complete stdout and JSON are preserved.

## Executed commands, exit zero

```sh
python3 research/sav/verification/gl_linear_check.py > research/sav/verification/goal002_prior_linear_recheck.txt 2>&1
python3 research/sav/verification/check_quadratic_source.py > research/sav/verification/goal002_prior_source_recheck.txt 2>&1
python3 research/sav/verification/audit_source_checks.py > research/sav/verification/goal002_prior_retarded_recheck.txt 2>&1
python3 research/sav/verification/goal002_sector_checks.py > research/sav/verification/goal002_sector_output.txt 2>&1
python3 research/sav/verification/goal002_scalar_response.py > research/sav/verification/goal002_scalar_response_output.txt 2>&1
python3 research/sav/verification/goal002_audit_geometry.py > research/sav/verification/goal002_audit_geometry_results.txt
python3 research/sav/verification/goal002_audit_energy.py > research/sav/verification/goal002_audit_energy_results.txt
```

The geometry auditor independently reran the sector command without
redirecting its stdout, observing the same eight pass/control lines.
Its output is quoted in AUDIT_GOAL002_GEOMETRY.md. This is replication;
the independent geometry pathway is its separate audit script.

The first three commands recheck existing exact equations with no physical
GL root solve and no rewriting of prior profiles/results. The sector
script builds all seven quadratic source components with Laurent harmonic
and quadratic master-jet algebra, and tests all three on-shell Bianchi
identities as exact rational functions of r at one local analytic
specialization. Its negative k² specialization is not a physical L10
mode and supplies no global endpoint/norm assertion.

The scalar script uses the saved final L10 profile, high-precision regular
contraction and new direct linear scalar bases/Green accumulation. It
uses exactly three declared configurations, recorded fully in
goal002_scalar_response_results.json. No fourth refinement, other k,
physical n2 BVP, GL root or spacetime time evolution was run.

The scalar command was evaluated again on those same three configurations
after simplifying two cancelling integral-orientation signs and recording
the separate E1 quadrature and derivative-tail diagnostics. C0 and Q were
unchanged. This added no discretization level, physical k or GL solve.

| Scalar configuration | (log delta left, R, grid, tolerance, max log step) |
|---|---|
| 1 | (-16,80,4001,2e-8,0.2) |
| 2 | (-22,120,8001,2e-10,0.1) |
| 3 | (-28,180,16001,2e-12,0.05) |

Q converges empirically toward 80.8179176; C0 toward 12.30729198.
Frobenius/GL/interpolation/floating/quad and omitted-source errors are
not a full enclosure. The scalar outer Volterra inequality is analytic
conditional on exact inputs; the printed numerical values are not
outward-rounded bounds. An ODE tolerance or W drift never substitutes
for the missing full error interval.

The geometry audit runs 17 independently implemented exact/control checks;
eight deliberate-error cases in six groups are rejected. Its machine
JSON includes mutation tests as check records, so a check count is not a
number of physical gates passed. The energy audit runs 18 checks and
11 rejected mutations. Its smooth Gaussian controls carry analytic
infinite-domain tail inequalities and separate floating quadrature
diagnostics; they are not a physical GL scattering waveform.

Both audit records state precise hypotheses, found defects and repairs,
commands, versions and limitations. Independent scientific AI agents
performed them; no external human referee endorsement is implied.

## Budget and failures preserved

- Main physical case L10 only. Existing GOAL-001 Omega/profile reused.
- Three scalar Green configurations, no new physical k eigenvalue solves.
- n2: regular reduced DAE and covariant tensor route; neither evaluated
  its constraint-compatible inverse/endpoints. No n2 numerical grid used.
- Validity: regular-frame hierarchy and positive scalar-energy route;
  neither controls M2/N4 and nonlinear trace/completion constants.
- Audit symbolic optimization interruptions are recorded by the auditor;
  they are not repeated physical repair approaches or physics failures.
- Audit findings corrected: sufficient-vs-necessary error wording,
  hierarchy lower-bound denominator, Frobenius/operator signature norm,
  and raw-EF versus time-factored profile notation. Failed mutations remain.
- Prior D16/M2-C failures retained verbatim in negative_results.md.
- Complete finite-A GL interval, exact simultaneous data family, n2
  response norm, physical full spectrum and bulge fraction NOT established.

Input blob inventory: goal002_input_tree.json. Provenance additions,
publication scope/commit refs and source-inspection limits are in SOURCES.md
and goals/GLR-GOAL-002_REPORT.md. No unauthorized path/run/merge occurred.

## Publication verification

Scientific result commit 6eb617e4d22b1ca8e8641dcf1f80ca5d9c64e681 with parent
8d4163cd876b33752f7fb6938bb7861fe221b7c0 and tree
e8f5a1e45cdb9b69bc75fd407093e9bf14283ff2. A recursive path/SHA comparison
before publication verified 31 changed/added blobs, no removed blobs,
no outside research/sav/ changes and exact agreement with the intended
publication path set. Branch update used expected_sha equal to the input
and force=false. No PR, merge, other branch or production action occurred.
The second commit is the closing provenance/pointer amendment only.
