# Proposed validation gates (future only)

Each stage needs engineering acceptance and an *independent physical*
acceptance. Existing M1 and M2-B fixtures do not certify an outgoing
waveform. No simulations authorized here.

| Gate | Required evidence | Status |
|---|---|---|
| Background | unperturbed GP through first-bulge-duration target; finite metric, controlled constraints, repeat in >=2 radial domains | NOT MET in frozen M2-C |
| Linear GL | stable zero-perturbation control; constrained growing mode; growth and radial profile match continuum; >=3 resolutions ideally | NOT MET |
| Selection parity | pure cosine +/-epsilon related by z -> z+L/2; zero-mode waveform even and linear component absent | PROPOSED |
| Pure gauge | no scalar news or massive invariant flux for a known gauge perturbation | PROPOSED |
| Scalar normalization | 4D EMD action vs 5D asymptotic/Bondi mass-loss law agree | PROPOSED |
| Radial extrapolation | >=3 tubes, physically justified falloff and time alignment | PROPOSED |
| Outer boundary | >=2 outward boundary locations, no causal contamination or reflected flux above error | PROPOSED |
| Discretization | >=3 resolutions, convergence in news and integrated energy; AMR temporal interpolation monitored | PROPOSED |
| Nonlinear scaling | epsilon/2 and epsilon with common pure GL projection, compare Phi~epsilon^2 and E~epsilon^4 in controlled regime | PROPOSED |
| Massive propagation | matched phases and group delays; below-threshold evanescence; no double-counting +/-n | PROPOSED |
| Global budget | Bondi scalar flux + exterior energy + horizon absorption and charges consistent within error | PROPOSED |

Provisional science target: relative uncertainty <=10% on a resolved,
nonzero early integrated energy; if not resolved, report a justified
upper limit and component error budget. This is **not** a preapproved
production numerical acceptance threshold; Michael and the human
scientific team must agree on simulation-specific values.

Error budget should separate truncation, outer-boundary reflection,
finite extraction radius, nonlinear gauge reconstruction, KK mode
leakage, temporal sampling, source/observer time-mapping and
initial-data transients. Convergence of apparent horizons alone
does not imply waveform convergence.
