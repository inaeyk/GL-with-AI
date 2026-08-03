# Milestone 2A design repair — constraint and implementation disposition

Classification: `M2-A DESIGN BLOCKED — the complete locked live first-order
radial principal matrix has a determinant/weighted-trace Jordan block at
coordinate speed -beta^x (algebraic multiplicity two, geometric multiplicity
one), so no complete characteristic transform or inverse exists for either
radial normal`

The full 26-entry state, sparse principal matrix, spectrum, Jordan chain,
two-normal convention, and face speeds are recorded in
`docs/derivations/milestone2a_radial_characteristic_derivation.md`. This
document locks the remaining Gamma/Z test, records why seed preparation
cannot proceed, removes the SAT fallback, and gives an exact production
response for every former M2-B gate. It changes no implementation or evidence.

Milestone 1 remains `CLUSTER_EXECUTION_BASELINE_ACCEPTED`; G-Engineering is
passed; G-Physics and overall Checkpoint G remain blocked. D12–D16 remain
closed inconclusive. This obstruction does not authorize new Fourier scans,
dense spectra, or evolution in a projected constraint nullspace.

## 1. Discrete constraint inventory

The intended seven-row single-`k` operator remains

```text
C_disc=(r0^2 H,r0 Mx,r0 Mz,det(h),r0 tr_h(A),r0 Zx,r0 Zz).
```

| row | existing owner | derivative/boundary convention | parity | dimension | disposition |
|---|---|---|---|---:|---|
| `H` | `BlackStringTargetCleanupConstraintsSource::evaluate_constraints` | target-`d=4` fourth-order visible derivatives and hidden cartoon algebra; repaired radial boundary required | even | 1 | evaluator validated; boundary input unavailable |
| `Mx` | same | same; hidden `ww` multiplicity two | even | 1 | evaluator validated; boundary input unavailable |
| `Mz` | same | same | odd | 1 | evaluator validated; boundary input unavailable |
| determinant | production cleanup determinant | algebraic, `(hxx*hzz-hxz^2)hww^2` | even | 1 | validated; its radial derivative participates in the principal Jordan chain |
| weighted trace | production cleanup contraction | algebraic, `h^{xx}Axx+2h^{xz}Axz+h^{zz}Azz+2Aww/hww` | even | 1 | validated; it is the sole left eigenfield of the defective principal pair |
| `Zx` | validation-only contracted-connection helper | fourth-order coordinate derivatives; both radial normals tested below | even | 1 | `HELPER_CAN_BE_VALIDATED_WITHIN_M2-B`, but M2-B is blocked before implementation |
| `Zz` | same | same | odd | 1 | same |

The Hamiltonian/momentum evaluator still returns only three rows. No claim
may call it a seven-condition evaluator without the four explicit additions.

## 2. Gamma/Z helper gate

Gamma/Z retains the classification
`HELPER_CAN_BE_VALIDATED_WITHIN_M2-B` because every required comparison can
be made with committed helpers. It does not make M2-A ready and does not
authorize M2-B while the characteristic defect remains.

For perturbations of the flat conformal GP metric, with exactly two hidden
`w` copies, the required coordinate formulas are

```text
gX =  dx(hxx)/2 - dx(hzz)/2 - dx(hww) + dz(hxz)
      + 2(hxx-hww)/x,
gZ =  dx(hxz) + 2 hxz/x - dz(hxx)/2 + dz(hzz)/2 - dz(hww),
Zx = (GammaX-gX)/2,
Zz = (GammaZ-gZ)/2.
```

The helper uses coordinate derivatives. For a normal jet
`q_n=n_x partial_x`, the test must convert `partial_x=n_x q_n` explicitly:
outer `n_x=+1`, inner `n_x=-1`. The algebraic `1/x` terms do not change sign.
`gX,Zx` are even in periodic `z`; `gZ,Zz` are odd. Visible derivatives use
the live fourth-order centered stencil after ghost fill, and `z` uses the live
fourth-order periodic stencil. Hidden directions contribute only the two
shown algebraic copies, never a grid derivative.

The complete fixed validation is:

1. Test the zero perturbation, pure `GammaX`, pure `GammaZ`, pure radial
   `hww`, pure radial `hxz`, pure tangential `hxz`, and mixed
   `(hxx,hxz,hzz,hww)` analytic jets at
   `x/r0={0.5,0.5625,1,2,4.4375,4.5}` and both normals.
2. Generate 256 deterministic finite jets with seed `0x4d32415a`; every
   dimensionless value and derivative scaled by its appropriate power of
   `r0` lies in `[-1/4,1/4]`. Enforce even/odd parity before evaluation.
3. Compare the helper with the direct analytic formulas above.
4. Compare it with the centered signed linearization of the live mapped
   `TensorAlgebra::compute_christoffel(...).contracted` result produced from
   `BlackStringTargetCCZ4Pointwise::expand_target`, using
   `h_fd=2^-20` and `h_fd/2` in scaled variables.
5. Require, for every `g` and `Z` component,

   ```text
   absolute error <= 5e-12/r0,
   normalized error = |helper-reference|/(1/r0+|reference|) <= 2e-11,
   h_fd versus h_fd/2 normalized discrepancy <= 5e-9.
   ```

6. Mutations that change hidden multiplicity `2->1`, flip either radial
   normal conversion, delete either `1/x` term, flip a `dz` sign, or remove
   the factor `1/2` in `Z` must each fail at least one named analytic case by
   more than `1e-6/r0`.

Nonfinite output or failure of any absolute, normalized, refinement, parity,
or mutation requirement returns `M2-B GAMMA_Z_HELPER_VALIDATION_FAILURE` and
stops before a numerical launch. Tolerances are fixed and are not relaxed by
the implementation worker.

## 3. One-time seed correction disposition

The algebraic core of the proposed initial-data correction remains bounded:

- correction variables:
  `chi,hxx,hxz,hzz,hww,K,Axx,Axz,Azz,Aww,GammaX,GammaZ`;
- enforced rows: the seven scaled rows in section 1 at every valid radial
  point of one nonzero, non-Nyquist grid harmonic;
- field scaling: metric/`chi` columns dimensionless and `K,A,Gamma` columns
  multiplied by `r0`;
- row scaling: `(r0^2,r0,r0,1,r0,r0,r0)`;
- thin-SVD relative cutoff `1e-12 sigma_max`;
- acceptable scaled RMS residual `<=1e-8 |epsilon|`, individual
  Hamiltonian/momentum maximum `<=5e-8 |epsilon|`, and determinant,
  weighted-trace, and Gamma/Z maximum `<=1e-10 |epsilon|`;
- acceptable retained-system condition number `<=1e12`;
- amplitude normalization error `<=5e-13`, `+/-epsilon` antisymmetry
  `<=5e-13`, and retained transverse physical norm at least one quarter of
  the uncorrected seed norm.

The boundary rows cannot be supplied: the complete principal left transform
has rank at most 25 and infinite condition number. Therefore no SVD matrix is
assembled, no amplitude is normalized, and no seed is accepted. The fixed
failure is

`M2-B SEED PREPARATION BLOCKED BY M2-A CHARACTERISTIC JORDAN DEFECT`.

This least-squares operation, if a future human-approved formulation makes it
well defined, would still differ from D16: it would run once on initial data,
use the live discrete constraints and boundary equations, and then evolve the
ordinary 18 stored variables. D16 instead examined an algebraic nullspace of
a non-invariant evolution operator and proposed evolution inside that
projected space. M2 forbids the latter. This distinction does not cure the
current boundary obstruction.

## 4. Reconstruction and production interfaces are not approved

No complete matrices `T`, `T^{-1}`, or `R` exist for the locked full system.
Accordingly M2-A approves none of the formerly proposed production files,
parameters, ghost factorization, valid-surface RHS changes, or tests as an
implementation task. M2-B must not choose a constrained subsystem, delete
`C_h/C_A`, add a gauge driver, change CCZ4, or invent a penalty boundary.
Those are formulation/roadmap decisions for human review.

The SAT fallback is removed. No SAT operator, penalty coefficient, CFL rule,
KO/RK ordering, or stability claim is present, so it cannot be selected after
a failure.

## 5. Exact response to every former M2-B gate

| former gate | production response in this repair |
|---|---|
| characteristic transform/inverse roundtrip | hard fail before implementation: full transform rank `<=25`, inverse absent |
| incoming/outgoing speed classification | eigenvalue signs are recorded, but field coverage is incomplete; stop for human roadmap review |
| exact GP preservation | no repaired boundary map exists to test; do not launch |
| manufactured outgoing pulse | not authorized; do not build or launch |
| manufactured incoming control | not authorized; do not build or launch |
| no physical logic at MPI seams | no implementation authorized |
| all 18 slots and parity | no complete 54-ghost reconstruction exists; stop |
| discrete seed-constraint reduction | fixed blocked classification in section 3 |
| amplitude linearity | no seed accepted; do not launch |
| MPI/restart regression | no shared production change exists; do not run |
| boundary runtime/memory | no boundary implementation exists; no performance claim |
| Gamma/Z helper gate | specification is complete, but execution waits on a future human-approved roadmap |

No gate triggers a repair implementation. Failure or incompleteness triggers
human review. The maximum-two-repair policy is not consumed by a production
cycle because M2-B never began.

## 6. Status and audit disposition

No production code, test, fixture, parameter, build file, dependency, or
historical numerical evidence changed. No build or numerical launch occurred.
M2-B and M2-C remain unstarted. This repaired blocked design is the input to
the next focused audit; it does not request that the auditor invent the
missing eigenvector.
