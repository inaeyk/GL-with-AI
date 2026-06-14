# Off-Diagonal Ricci Flat Gate Notes

Status: Stage 3G companion regression. This is documentation and symbolic
checking only. It is not GRChombo evolution code, does not implement CCZ4 RHS
source terms, and does not claim physical correctness of an evolution.

## Purpose

The main Stage 3G conformal-cartoon script validates off-diagonal algebra:
determinants, the reduced block inverse, full-4D tracelessness, `K_xz`
reconstruction, the `/4` denominator, and the diagonal Stage 3F limit. It does
not contain a Christoffel/Ricci engine.

This companion gate promotes the off-diagonal Ricci check into the repository.
It verifies that the existing symbolic geometry machinery handles genuine
`g_xz` cross terms by checking two metrics that are coordinate pullbacks of
flat space.

## Metrics

The coordinate order is

```text
(x, theta, phi, z).
```

The constant-shear metric is obtained from flat space by

```text
dZ = dz + lambda dx.
```

Thus

```text
dl^2 = (1 + lambda^2) dx^2
     + 2 lambda dx dz
     + dz^2
     + x^2 dOmega_2^2.
```

This has a genuine constant cross term `g_xz = lambda`.

The `x`-dependent shear metric is obtained from flat space by

```text
dZ = dz + x dx,
```

or equivalently `Z = z + x^2/2`. Thus

```text
dl^2 = (1 + x^2) dx^2
     + 2 x dx dz
     + dz^2
     + x^2 dOmega_2^2.
```

This has `g_xz = x` and `partial_x g_xz = 1`, so it exercises off-diagonal
metric derivatives together with the angular `x^2 dOmega_2^2` Christoffels.

## Repository Checks

`offdiagonal_ricci_flat_gate_sympy.py` verifies for both metrics:

- the full matrix inverse satisfies `g g^{-1} = I`;
- every Ricci tensor component vanishes;
- the Ricci scalar vanishes.

The script uses `metric.inv()`, not reciprocal diagonal components. It is a
targeted flatness gate for the off-diagonal geometry engine before future CCZ4
RHS source-term work.

## Current Run Result

Run command:

```text
python3 docs/derivations/offdiagonal_ricci_flat_gate_sympy.py
```

Exact output:

```text
Stage 3G sheared-flat off-diagonal Ricci gate
Coordinate order: (x, theta, phi, z)
Both metrics are flat pullbacks with genuine g_xz cross terms.
PASS constant-shear g_xz = lambda
PASS constant-shear partial_x g_xz = 0
PASS constant-shear flat metric inverse identity (0,0) = 1
PASS constant-shear flat metric inverse identity (0,1) = 0
PASS constant-shear flat metric inverse identity (0,2) = 0
PASS constant-shear flat metric inverse identity (0,3) = 0
PASS constant-shear flat metric inverse identity (1,0) = 0
PASS constant-shear flat metric inverse identity (1,1) = 1
PASS constant-shear flat metric inverse identity (1,2) = 0
PASS constant-shear flat metric inverse identity (1,3) = 0
PASS constant-shear flat metric inverse identity (2,0) = 0
PASS constant-shear flat metric inverse identity (2,1) = 0
PASS constant-shear flat metric inverse identity (2,2) = 1
PASS constant-shear flat metric inverse identity (2,3) = 0
PASS constant-shear flat metric inverse identity (3,0) = 0
PASS constant-shear flat metric inverse identity (3,1) = 0
PASS constant-shear flat metric inverse identity (3,2) = 0
PASS constant-shear flat metric inverse identity (3,3) = 1
PASS constant-shear flat metric R_xx = 0
PASS constant-shear flat metric R_xtheta = 0
PASS constant-shear flat metric R_xphi = 0
PASS constant-shear flat metric R_xz = 0
PASS constant-shear flat metric R_thetax = 0
PASS constant-shear flat metric R_thetatheta = 0
PASS constant-shear flat metric R_thetaphi = 0
PASS constant-shear flat metric R_thetaz = 0
PASS constant-shear flat metric R_phix = 0
PASS constant-shear flat metric R_phitheta = 0
PASS constant-shear flat metric R_phiphi = 0
PASS constant-shear flat metric R_phiz = 0
PASS constant-shear flat metric R_zx = 0
PASS constant-shear flat metric R_ztheta = 0
PASS constant-shear flat metric R_zphi = 0
PASS constant-shear flat metric R_zz = 0
PASS constant-shear flat metric Ricci scalar = 0
PASS x-shear g_xz = x
PASS x-shear partial_x g_xz = 1
PASS x-shear flat metric inverse identity (0,0) = 1
PASS x-shear flat metric inverse identity (0,1) = 0
PASS x-shear flat metric inverse identity (0,2) = 0
PASS x-shear flat metric inverse identity (0,3) = 0
PASS x-shear flat metric inverse identity (1,0) = 0
PASS x-shear flat metric inverse identity (1,1) = 1
PASS x-shear flat metric inverse identity (1,2) = 0
PASS x-shear flat metric inverse identity (1,3) = 0
PASS x-shear flat metric inverse identity (2,0) = 0
PASS x-shear flat metric inverse identity (2,1) = 0
PASS x-shear flat metric inverse identity (2,2) = 1
PASS x-shear flat metric inverse identity (2,3) = 0
PASS x-shear flat metric inverse identity (3,0) = 0
PASS x-shear flat metric inverse identity (3,1) = 0
PASS x-shear flat metric inverse identity (3,2) = 0
PASS x-shear flat metric inverse identity (3,3) = 1
PASS x-shear flat metric R_xx = 0
PASS x-shear flat metric R_xtheta = 0
PASS x-shear flat metric R_xphi = 0
PASS x-shear flat metric R_xz = 0
PASS x-shear flat metric R_thetax = 0
PASS x-shear flat metric R_thetatheta = 0
PASS x-shear flat metric R_thetaphi = 0
PASS x-shear flat metric R_thetaz = 0
PASS x-shear flat metric R_phix = 0
PASS x-shear flat metric R_phitheta = 0
PASS x-shear flat metric R_phiphi = 0
PASS x-shear flat metric R_phiz = 0
PASS x-shear flat metric R_zx = 0
PASS x-shear flat metric R_ztheta = 0
PASS x-shear flat metric R_zphi = 0
PASS x-shear flat metric R_zz = 0
PASS x-shear flat metric Ricci scalar = 0
All Stage 3G sheared-flat off-diagonal Ricci checks passed.
```

## Limitations

- This is a Ricci flatness regression, not a full modified-cartoon CCZ4 RHS
  derivation.
- It validates two flat off-diagonal metrics, not arbitrary off-diagonal
  `gamma_xz(x,z)` curvature.
- Future Stage 3H source-term decomposition still needs its own block-level
  validation and reference checks.
