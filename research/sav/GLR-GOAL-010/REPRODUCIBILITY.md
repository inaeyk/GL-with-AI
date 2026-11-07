# Reproduction and evidence limits

The parent is `80f030df0bec99bebe76fd9ab473c419ad1af5a5`. The scripts in [checks](checks/) were recovered from the original in-memory calculation inputs. The source replay uses one JSON object per line so the full result can be archived, and the affine replay asserts the checks that originally printed a status. The causal controls combine the original oscillator and Laplace checks with the corrected rational simplification check for the potential. No physical radial Cauchy integration, Green projection, or evolution was added during publication.

## Commands and versions

Run from the repository root with the pinned parent object available. The source replay uses Python 3.14.8 and python-flint 0.9.0. The symbolic replays use SymPy 1.14.0. The near-horizon and normalization source calculations use the recovered 144-bit context before loading the pinned validator. Loading the validator changes the context to 160 bits before the exterior Riccati/amplitude flow and source evaluation.

```sh
cd research/sav/GLR-GOAL-010
python3 -B checks/check_source_intervals.py > /tmp/glr010-source-intervals.jsonl
cmp checks/source_intervals.jsonl /tmp/glr010-source-intervals.jsonl
for check in check_spatial_ode check_master_time_domain check_master_affine check_causal_controls; do
  python3 -B "checks/$check.py" || exit 1
done
```

The publication replay reproduced all 35 original Ricci intervals exactly against the original point output. The saved full JSONL SHA-256 is `2b4791a6a0d3a35f55bddabf04ff8c84a7a52366e96a8408296a3bc5c79d32d6`. It includes the Ricci and trace-reversed intervals, the independent warped result, pointwise normalized mode values, source scale and Bianchi controls. Fields for complex intervals use `[[real_lo,real_hi],[imag_lo,imag_hi]]`. The EF-to-\(\tau=0\) time factor is printed as an Arb ball string. Compare enclosure results rather than byte equality when using other runtime versions. [Saved symbolic stdout](checks/symbolic_replay.txt)

## Exact pinned inputs and algorithm

- `verification/goal005_gl_results.json` at the parent supplies the \(r=2\) Riccati ball \(u_{1,x}/u_1\). The angular value condition fixes \(u_1(2)\) through the pinned linear reconstruction.
- `GLR-GOAL-005/compute/gl_eigenvalue_results.json` supplies the previously enclosed horizon amplitude. Use the regular horizon Frobenius recurrence and the exact numerator cancellation for the EF radial metric coefficient.
- `verification/goal005_validate_gl.py` supplies the bounded endpoint and interval Taylor Riccati step, backward from \(r=40\) with cells no wider than \(1/64\) in \(\log(r-1)\). The log-amplitude integral uses Taylor coefficients through degree four plus a fifth-coefficient tube remainder and is normalized at \(r=2\). The spectral interval is inherited rather than certified anew.
- Taylor expansion of the regular first GL metric through the needed radial degree and a polar-angle jet gives the full quadratic curvature and differentiated source at the five evaluation radii. Input amplitude is factored outside the homogeneous linear reconstruction and its square outside quadratic curvature, avoiding unnecessary loss of correlation. Endpoint floats in the saved component bounds are rounded outward.

[Input derivations and proof scopes](../physics/goal005_enclosures.md) · [Historical GOAL-005 validator](../verification/goal005_validate_gl.py)

## Missing material

The Cauchy IVP and full algebraic lift were recovered. Numerical profiles of \(W_2,P_2,J_2\), the full unitary metric and normal-velocity jet, and a uniform radial Cauchy error budget were never produced. The output archive contains all available pointwise source intervals and individual analytic or enclosed Cauchy components. The source intervals are not a uniform enclosure between radii. There is no saved evaluated causal field, Green overlap for either Z family, horizon absorption, signed finite-tube partition, full frequency tail bound for the physical energy, or first-bulge fraction. The original failure record and remaining requirements are in [obstructions](OBSTRUCTIONS.md).
