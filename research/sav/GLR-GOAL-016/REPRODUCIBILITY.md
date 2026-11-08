# Reproduction and independent evidence

Run commands from the scoped source directory `research/sav` on the GOAL-016 branch. Python 3.14.8, NumPy 2.4.6, SciPy 1.18.0, python-flint 0.9.0 and SymPy 1.14.0 reproduce the recorded checks. Tube and band bounds use 112-bit Arb balls. The float operations that choose panel counts and geometry caps leave those counts positive and verify the necessary domain bounds afterward. They do not supply physical error estimates. Set `PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1` and run interval checks sequentially.

## Inherited massless input

```sh
scratch=$(mktemp -d)
cp GLR-GOAL-005/compute/gl_eigenvalue.py GLR-GOAL-005/compute/early_interval.py "$scratch/"
python3 -B "$scratch/gl_eigenvalue.py"
python3 -B "$scratch/early_interval.py"
cmp "$scratch/gl_eigenvalue_results.json" GLR-GOAL-005/compute/gl_eigenvalue_results.json
cmp "$scratch/early_interval_results.json" GLR-GOAL-005/compute/early_interval_results.json
```

Both JSON outputs matched byte for byte. [Hashes and retained timing-wrapper error](checks/inherited_interval_replay.txt)

The historical symbolic replays were `python3 -B GLR-GOAL-005/check_identities.py` and the two scripts `checks/check_optical_normal_form.py` and `checks/check_optical_identities.py` from the read-only GOAL-009 pin, under its `research/sav/GLR-GOAL-009/` directory. [Captured results](checks/inherited_identity_replay.txt)

## New exact checks and transport reproduction

```sh
g=GLR-GOAL-016
python3 -B "$g/checks/preparation_bounds.py" | cmp - "$g/checks/preparation_bounds.txt"
python3 -B "$g/vacuum/check_optical_principal.py" | cmp - "$g/vacuum/check_optical_principal.txt"
python3 -B "$g/verification/checks/scalar_sign_gates.py" | cmp - "$g/verification/checks/scalar_sign_gates.txt"
python3 -B "$g/transport/checks/check_kernels.py" | cmp - "$g/transport/checks/check_kernels.txt"
awk '/^python3 - <<.PY./{flag=1;next} flag && /^PY$/{exit} flag' "$g/stabilization/MATCH.md" |
  python3 -B | cmp - "$g/checks/stabilization_replay.txt"
```

Use the commands in the [tube and band certificate](transport/CERTIFICATE.md) for the archived directed JSON, including the two incomplete interval decisions. The final 48 and 24 radius JSON and completed-band JSON replayed byte for byte. [Terminal replay log](checks/terminal_transport_replay.txt)

The committed figure uses Matplotlib 3.11.0 and the archived tube records. The bars depict enclosures of continuum coefficients with outward-rounded values available in the JSON.

```sh
MPLCONFIGDIR="$scratch/matplotlib" python3 -B GLR-GOAL-016/checks/plot_tube_intervals.py "$scratch/early-flux.png"
```

## Exposure and verification

| Track | Reconstruction | Shared inputs and limits |
| --- | --- | --- |
| Physical preparation and observables | Graph energy cost, smooth near-minimum cancellation, interval replay, Riemann-kernel scaling | Original constraint theorem, source, and archived GL certificate. The preparation audit received the full derivation after its own current reconstruction. |
| Vacuum optical analysis | Derivation by terminal constraints and direct Christoffel check of the leading compact symbol and correction | Same terminal radial powers and candidate jets. The check independently recomputes curvature and detects a missing potential term. It supplies no control of the full nonlinear evolution. |
| Directed transport | Arb enclosure with exact constant-mass kernel, full causal residual and sign-safe whole cells, plus manufactured rational kernel controls | Shared inherited Riccati and source enclosure. Code audit exposed to the result and reviewed its proof, including a sign-changing control. The audit is distinct from a second independent physical interval solver. |
| Adversarial verification | Weighted null evolution, fixed-mass decay, signed critical comparison and preparation and tube audits | Shared GL source and GOAL-005 bounds. The new limiting derivation preceded exposure to the coordinator's Riemann-kernel route. Exposure to the preparation proof and tube code is explicit. |
| Stabilization and primary sources | Local GB operator and variational derivative, stationary stress obstruction and conditional retarded response interface | The flat model and inherited audit identified background curvature ambiguity. Exact local algebra and a published GB background check constrain the new calculation. The directional static derivative has a separate internal analytic check and no independent threshold numerical replay. |

Five tracks include the coordinator and four concurrent scoped specialists. The inherited role names governed the assignment. Exact runtime model and effort settings were unavailable for independent verification. The program uses read-only source and audit pins listed in [GOAL.md](GOAL.md). External Work audit and human scientific acceptance are subsequent gates.

## Coordinator check of the completed-band transfer

I reconstructed the Liouville defect by differentiating \(f=p^{-1/2}w\) twice in \(x\), giving \(Q=-V_{xx}/(4p^4)-5V_x^2/(16p^6)\) in \(dy=p\,dx\). In the \(L^1_x\) curvature budget, splitting \((fV_r)'=f'V_r+fV_{rr}\) gives \(2\mu^2/3+38/15\) upon integrating the monomials with \(f\) retained. Dropping that factor would give a larger absolute bound. The constant-\(V\) control in the unit-current basis gives \(|W|=2\) and a point-source transfer magnitude \(1/(2\sqrt p)\). Conserved current on opposite ends gives the same \(|W|\ge2\) lower bound for the curved operator. I checked the sign of the pole-free above-threshold pairing, the support of the two completions, the time-variation factor across the graph and the negative-frequency partner in the energy. This was a non-blind analytic and code check of the published band estimates.
