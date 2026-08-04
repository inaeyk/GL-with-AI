# Milestone 2A design lock — algebraic-manifold boundary and seed

Classification: `M2-A DESIGN READY`

The revised characteristic derivation is
`docs/derivations/milestone2a_radial_characteristic_derivation.md`. It
preserves the failed raw 26-variable Jordan calculation as historical
evidence and replaces only the design with the complete 23-variable tangent-
space system used by the cleanup-bearing algorithm. This document locks the
discrete constraint, seed, interface, tolerance, and failure decisions. It
authorizes no implementation before the required focused design audit.

Milestone 1 remains `CLUSTER_EXECUTION_BASELINE_ACCEPTED`; G-Engineering is
passed; G-Physics and overall Checkpoint G remain blocked. D12–D16 remain
closed inconclusive. Nothing here authorizes Fourier scans, dense spectra,
SAT/WKB work, or evolution in a projected constraint nullspace.

## 1. Discrete constraint inventory on the algebraic manifold

`hww` and `Aww` are reconstructed exactly before constraint evaluation, so
determinant and weighted trace are identities rather than least-squares rows.
The differential single-`k` operator is

```text
C_diff=(r0^2 H,r0 Mx,r0 Mz,r0 Zx,r0 Zz).
```

| row | existing owner | discrete derivative and boundary convention | parity | output | validation status |
|---|---|---|---|---:|---|
| `H` | `BlackStringTargetCleanupConstraintsSource::evaluate_constraints` | target-`d=4`; fourth-order visible derivatives; hidden cartoon algebra; revised characteristic ghosts | even | 1 | evaluator validated; revised ghost input scheduled in M2-B |
| `Mx` | same | same; hidden `ww` multiplicity two | even | 1 | evaluator validated; revised ghost input scheduled |
| `Mz` | same | same | odd | 1 | evaluator validated; revised ghost input scheduled |
| determinant | nonlinear reconstruction | `(hxx*hzz-hxz^2)hww^2=1` | even | identity | enforced by construction, checked to `5e-13` |
| weighted trace | nonlinear reconstruction | `h^xx Axx+2h^xz Axz+h^zz Azz+2Aww/hww=0` | even | identity | enforced by construction, checked to `5e-13/r0` |
| `Zx` | validation-only contracted-connection helper | live fourth-order coordinate derivatives; both normal conversions below | even | 1 | `HELPER_CAN_BE_VALIDATED_WITHIN_M2-B` |
| `Zz` | same | same | odd | 1 | same |

No acceptance report may call the existing Hamiltonian/momentum evaluator a
five-row helper until the two Gamma/Z rows pass the following gate.

## 2. Fixed Gamma/Z validation gate

The status remains exactly `HELPER_CAN_BE_VALIDATED_WITHIN_M2-B`. The complete
test is implementable from committed connection, target-expansion, live
fourth-order derivative, and parity helpers. M2-B physical launches remain
blocked until it passes.

For perturbations of the flat conformal GP metric, with exactly two hidden
`w` copies,

```text
gX = dx(hxx)/2-dx(hzz)/2-dx(hww)+dz(hxz)
     +2(hxx-hww)/x,
gZ = dx(hxz)+2hxz/x-dz(hxx)/2+dz(hzz)/2-dz(hww),
Zx = (GammaX-gX)/2,
Zz = (GammaZ-gZ)/2.
```

`hww` and its jet are obtained from the exact algebraic reconstruction, not
treated as independent random data. For a normal jet `q_n=n_x partial_x`,
the test converts `partial_x=n_x q_n`: `n_x=+1` outer and `n_x=-1` inner.
The `1/x` terms do not change sign. `gX,Zx` are even in `z`; `gZ,Zz` are odd.
Hidden directions supply the two algebraic copies above and no grid
derivative.

The fixed M2-B gate is:

1. Test zero, pure `GammaX`, pure `GammaZ`, pure reconstructed radial
   `hww`, pure radial `hxz`, pure tangential `hxz`, and mixed
   `(hxx,hxz,hzz)` analytic jets at
   `x/r0={0.5,0.5625,1,2,4.4375,4.5}` for both normals.
2. Test 256 deterministic admissible metric jets, seed `0x4d32415a`. Each
   scaled independent value/derivative lies in `[-1/4,1/4]`; reconstruct
   `hww`, reject inadmissible metrics, and enforce even/odd parity first.
3. Compare the helper with the direct formulas above.
4. Compare it with the centered signed linearization of the live mapped
   `TensorAlgebra::compute_christoffel(...).contracted` object produced by
   `BlackStringTargetCCZ4Pointwise::expand_target`, with dimensionless
   `h_fd=2^-20` and `h_fd/2`.
5. For every `g,Z` component require

   ```text
   absolute error <= 5e-12/r0,
   |helper-reference|/(1/r0+|reference|) <= 2e-11,
   h_fd versus h_fd/2 normalized discrepancy <= 5e-9.
   ```

6. Mutations `hidden multiplicity 2->1`, either normal-sign flip, deletion
   of either `1/x` term, a `dz` sign flip, independent `hww` mutation, or
   removal of the `1/2` in `Z` must each fail a named analytic case by more
   than `1e-6/r0`.

Nonfinite output or any absolute, normalized, refinement, parity, analytic,
random-jet, live-mapped, or mutation failure returns
`M2-B GAMMA_Z_HELPER_VALIDATION_FAILURE` and stops before a physical launch.
The tolerances cannot be relaxed in M2-B.

## 3. One-time differential-constraint seed correction

Begin with a smooth, nonzero, parity-compatible single nonzero/non-Nyquist
Fourier harmonic `v0`. Lapse, shift, `B`, and `Theta` remain GP gauge data.
The independent correction columns are

```text
(chi,hxx,hxz,hzz,K,Axx,Axz,Azz,GammaX,GammaZ)
```

at valid radial cells outside a fixed four-cell buffer at each physical
boundary. Every trial vector first reconstructs `hww,Aww` exactly, and then
the live discrete operator evaluates only

```text
(H,Mx,Mz,Zx,Zz).
```

The four-cell zero-correction buffer preserves the homogeneous incoming
characteristic data and keeps support away from the boundary. The seed itself
must obey the repaired characteristic ghost policy; boundary characteristic
residuals are explicit acceptance rows, not unknown correction columns.

Form the centered linearized matrix `J_diff` from the committed discrete
helpers and solve the one-time minimum-norm correction

```text
min ||S_f delta v||_2
subject to S_c[J_diff delta v+C_diff(v0)]=0
and the fixed zero-buffer and amplitude constraints.
```

Use the already available thin SVD path. The fixed scales are:

- dimensionless `chi,h` columns; multiply `K,A,Gamma` columns by `r0`;
- row scale `(r0^2,r0,r0,r0,r0)` for `(H,Mx,Mz,Zx,Zz)`;
- one additional normalization row with the same scaled physical norm; and
- unit characteristic scaling after the `r0` state scaling in the derivation.

Retain singular values `sigma>=1e-12 sigma_max`; require retained condition
number `<=1e12`. Success requires

```text
scaled differential-constraint RMS <= 1e-8*|epsilon|,
max scaled H/M residual             <= 5e-8*|epsilon|,
max scaled Gamma/Z residual         <= 1e-10*|epsilon|,
max incoming boundary characteristic<= 1e-11*|epsilon|,
determinant residual                <= 5e-13,
weighted-trace residual             <= 5e-13/r0,
amplitude normalization error       <= 5e-13,
+epsilon/-epsilon antisymmetry      <= 5e-13.
```

After correction, the transverse physical norm must remain at least one
quarter of the uncorrected seed norm and two independent physical fields must
remain nonzero. Normalize once to the requested `epsilon`, repeat for
`-epsilon`, and do not modify the accepted vector thereafter. Rank loss,
excessive condition number, nonfinite output, buffer violation, tolerance
failure, loss of the requested harmonic/parity, or collapse of the physical
norm returns `M2-B SEED_DIFFERENTIAL_CONSTRAINT_CORRECTION_FAILURE` and stops
the implementation cycle.

This is not D16. It prepares one initial state using the live finite-domain
constraints and accepted boundary equations, then evolves the ordinary 18
stored variables with no projector. D16 projected an evolution operator into
a non-invariant algebraic nullspace and contemplated evolving there. M2 does
not reuse that operator, nullspace, or evolution-time projection.

## 4. Production interface map

The smallest expected M2-B diff is fixed conceptually; exact filenames may
follow existing project naming, but ownership and behavior do not change.

| interface | owner, inputs, outputs | behavior and cost | layout/MPI/restart |
|---|---|---|---|
| algebraic reconstruction | target cleanup/constraint owner; independent `h,A` -> dependent `hww,Aww` | fixed scalar formulas, stack values, `O(1)` per cell | no slot/layout change; used for physical ghosts and seed |
| reduced characteristic helper | project boundary owner; GP `b`, normal, independent state/normal jets -> 23 fields/speeds | explicit formulas in derivation; no eigensolver/allocation; `O(1)` | physical faces only; no seam call; no checkpoint effect |
| inverse characteristic helper | same; 23 fields -> independent face state/jets | exact inverse formulas; rank/condition/roundtrip gates | same |
| three-layer ghost helper | physical-boundary owner; valid cells plus incoming data -> 48 independent plus six dependent ghost slots per three layers | fixed Hermite interpolation, nonlinear dependent reconstruction; `O(N_z)` | never at MPI seams; stored layout unchanged |
| valid-surface RHS ownership | live target operator plus algebraic reconstruction owner; characteristic ghosts -> 16 independent live RHS slots plus dependent chain-rule RHS | live RHS/KO once; no provisional componentwise override; `O(N_z)` added work | no restart data; no double application |
| Gamma/Z helper gate | validation/helper owner; independent reconstructed jets -> `Zx,Zz` | default-off validation until gate passes | no layout/seam/restart change |
| seed correction | initial-data owner; single-`k` seed -> corrected independent state plus reconstructed dependent state | one thin SVD outside hot path | checkpoint stores normal 18 slots; no evolution projection |

All temporary field vectors are fixed-size stack arrays. Boundary coefficients
are built once from the explicit formulas. No production object gains a
virtual member, mutex, counter, logger, or per-cell allocation. Runtime and
memory are compared with the accepted Milestone 1 baseline; slowdown above
15% requires explanation and approval.

No new parameter choice is delegated to M2-B. The already planned boundary
strategy uses the committed domain/layout and GP parameters; any future user-
facing enable switch must default off until the focused gates pass.

## 5. Locked M2-B validation matrix and production response

No test is run in M2-A. M2-B uses the predeclared tolerances below.

| gate | fixed tolerance | exact production response to failure |
|---|---|---|
| transform/inverse roundtrip, both normals and all families | relative max `<=5e-12`; rank 23 at `1e-12`; `kappa_2<=1e3` | repair only the explicit transform row/sign/normal map; otherwise stop cycle |
| speed classification | analytic vs helper `<=5e-13`; sign margin matches derivation within `5e-13` | repair speed/ownership table; no launch |
| algebraic ghost constraints | determinant `<=5e-13`; trace `<=5e-13/r0` | repair nonlinear reconstruction; no stencil access |
| exact GP preservation | finite; scaled boundary residual `<=2e-10`; fourth-order convergence ratio `>=12` on the focused manufactured refinement | repair GP subtraction/interpolation/chain rule; stop cycle |
| manufactured outgoing pulse | incoming contamination `<=1e-8` of outgoing norm; observed boundary order `>=3.5` | repair outgoing extrapolation only |
| manufactured incoming control | prescribed-field error `<=1e-10`; all outgoing equality errors `<=5e-12` | repair incoming row or inverse map only |
| MPI seam ownership | zero physical-boundary calls at seams; slotwise equality `<=5e-13` | repair ownership predicate; no physical run |
| all 18 slots/parity | every slot finite; even/odd leakage `<=5e-13`; hidden multiplicity mutations rejected | repair mapping/reconstruction only |
| Gamma/Z helper | section 2 | hard stop before physical launch |
| seed differential constraints | section 3 | hard stop seed preparation; repair operator/scaling/buffer only |
| amplitude linearity | normalized `+/-` antisymmetry `<=5e-13` | repair one-time normalization only |
| affected MPI/restart regression | bitwise where unchanged; otherwise normalized max `<=5e-13` with documented changed boundary cells | repair serialization/ownership; no expansion of tests |
| boundary overhead | `O(N_z)`, zero per-cell allocation, total slowdown `<=15%` vs M1 | optimize fixed boundary path or request approval; no second RHS pass |

One failed gate consumes at most the declared production repair cycle only
after implementation exists. The exact fallback is a bounded correction to
the named production formula. A second failed repair stops for human roadmap
review. SAT, WKB, dense spectra, parameter sweeps, and fixture-only projection
are prohibited responses.

## 6. Boundary and cleanup lock

The boundary design acts only on independent characteristic fields. Inner
incoming lapse and shift families receive GP gauge data; outer incoming
physical rows receive homogeneous radiative data, incoming Z4 rows receive
homogeneous constraint-preserving data, gauge rows receive GP data, and
incoming advected independent metric rows receive GP background data.
Outgoing and glancing rows are exact equality constraints to their interior
extrapolants.

Three independent ghost layers are reconstructed first; `hww,Aww` are then
reconstructed exactly before derivatives or KO. The characteristic ghosts are
the sole incoming-data owner. The live target operator evaluates the 16
independent outer valid-surface RHS slots once with those ghosts; the
provisional componentwise outer override is disabled. The dependent
`hww,Aww` RHS is the exact time derivative of the algebraic formulas. After
the RK update, the existing cleanup runs once before any subsequent derivative
access. Internal MPI seams use exchange only.

This lifecycle makes the boundary and seed consistent with the algorithmic
constraint manifold without double-applying cleanup and without changing the
18-slot production/checkpoint layout.

## 7. Audit and authorization

M2-A used no numerical launch and made no implementation change. The next
action is one focused substantive design audit of:

1. the explicit algebraic elimination and reduced principal matrix;
2. both-normal transforms, spectrum, and conditioning;
3. incoming/outgoing ownership and three-layer reconstruction;
4. Gamma/Z formulas and fixed tolerances; and
5. the one-time differential-constraint SVD specification.

After that audit passes, exactly one initial M2-B implementation cycle is
authorized within the existing 10-launch focused-test budget. If the reduced
symbol itself is disproved, stop and return to human review of a near-inner
gauge change or a different inner geometry/treatment.
