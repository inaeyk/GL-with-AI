# M2-A complete live radial principal-system repair

Status: `M2-A DESIGN BLOCKED — the complete locked live first-order radial
principal matrix has a determinant/weighted-trace Jordan block at coordinate
speed -beta^x (algebraic multiplicity two, geometric multiplicity one), so no
complete characteristic transform or inverse exists for either radial normal`

This is the only M2-A design-repair pass before another audit. It replaces the
former homogeneous-shift/blockwise boundary proposal. It is a source-derived
continuum calculation, not numerical evidence and not an implementation.

## 1. Source and convention lock

The source path is the committed
`BlackStringToyLevel::specificEvalRHS -> BlackStringLive::RHSCompute ->
BlackStringTargetCCZ4Pointwise::LockedRHSAccess -> CCZ4RHS::rhs_equation`,
followed by `MovingPunctureGauge::rhs_gauge`, the project-owned fixed GP lapse
source, and one KO addition. The principal calculation uses the actual target
dimension `d=4`, `USE_CCZ4`,

```text
kappa1=0.1, kappa2=0, kappa3=1, covariant_z4=true, Lambda=0,
lapse_advec_coeff=0, lapse_power=1, lapse_coeff=2,
shift_Gamma_coeff=F=3/4, shift_advec_coeff=0, eta=1.
```

The 18 stored variables remain

```text
(chi,hxx,hxz,hzz,hww,K,Axx,Axz,Azz,Aww,Theta,GammaX,GammaZ,
 alpha,betaX,betaZ,Bx,Bz).
```

At the frozen GP point,

```text
alpha=chi=hxx=hzz=hww=1, hxz=0, betaX=b=sqrt(r0/x), betaZ=0,
K=3 lambda/2,
(Axx,Axz,Azz,Aww)=lambda(-7/8,0,-3/8,5/8),
Theta=GammaX=GammaZ=Bx=Bz=0, lambda=sqrt(r0/x^3).
```

The GP extrinsic-curvature coefficients multiply first radial derivatives in
several un-reduced equations. After the first-order reduction below those
terms are algebraic in the reduced state, not coefficients of
`partial_x V`, and therefore are lower order. The fixed lapse source,
`kappa` damping, GP coefficient gradients, tangential Fourier terms, and
explicit cartoon `1/x` terms are also lower order in the radial principal
matrix. The two hidden `w` directions are never grid directions: `dww` and
`Aww` occur once in storage and twice in target traces.

The sign convention is

```text
partial_t V + A^x partial_x V = lower order.
```

## 2. Complete first-order state and matrix

Introduce only radial grid derivatives:

```text
qchi=partial_x delta chi,
(dxx,dxz,dzz,dww)=partial_x(delta hxx,delta hxz,delta hzz,delta hww),
qalpha=partial_x delta alpha,
(rx,rz)=partial_x(delta betaX,delta betaZ).
```

The derivative/dynamical state, in the exact matrix order used below, is

```text
W=(qchi,dxx,dxz,dzz,dww,K,Axx,Axz,Azz,Aww,Theta,
   GammaX,GammaZ,qalpha,rx,rz,Bx,Bz)^T.                 (18 entries)
```

The remaining zero-order position state is

```text
U=(delta chi,delta hxx,delta hxz,delta hzz,delta hww,
   delta alpha,delta betaX,delta betaZ)^T.              (8 entries)
```

Thus `V=(W,U)^T` has 26 entries. Define `M` by
`partial_t W=M partial_x W+lower order`; then `A_W^x=-M`. The following 18
rows are the complete sparse definition of `M`; a missing entry is exactly
zero:

```text
partial_t qchi = b partial_x qchi
               + 1/2 partial_x K - 1/2 partial_x rx,

partial_t dxx  = b partial_x dxx - 2 partial_x Axx
               + 3/2 partial_x rx,
partial_t dxz  = b partial_x dxz - 2 partial_x Axz + partial_x rz,
partial_t dzz  = b partial_x dzz - 2 partial_x Azz
               - 1/2 partial_x rx,
partial_t dww  = b partial_x dww - 2 partial_x Aww
               - 1/2 partial_x rx,

partial_t K = b partial_x K
            + partial_x(3 qchi - dxx/2 - dzz/2 - dww
                        + GammaX - qalpha),

partial_t Axx = b partial_x Axx
              + partial_x(3 qchi/4 - 3 dxx/8 + dzz/8 + dww/4
                          + 3 GammaX/4 - 3 qalpha/4),
partial_t Axz = b partial_x Axz
              + partial_x(-dxz/2 + GammaZ/2),
partial_t Azz = b partial_x Azz
              + partial_x(-qchi/4 + dxx/8 - 3 dzz/8 + dww/4
                          - GammaX/4 + qalpha/4),
partial_t Aww = b partial_x Aww
              + partial_x(-qchi/4 + dxx/8 + dzz/8 - dww/4
                          - GammaX/4 + qalpha/4),

partial_t Theta = b partial_x Theta
                + partial_x(3 qchi/2 - dxx/4 - dzz/4 - dww/2
                            + GammaX/2),

partial_t GammaX = b partial_x GammaX
                 + partial_x(-3 K/2 + 2 Theta + 3 rx/2),
partial_t GammaZ = b partial_x GammaZ + partial_x rz,

partial_t qalpha = partial_x(-2 K + 4 Theta),
partial_t rx = F partial_x Bx,
partial_t rz = F partial_x Bz,

partial_t Bx = b partial_x GammaX
             + partial_x(-3 K/2 + 2 Theta + 3 rx/2),
partial_t Bz = b partial_x GammaZ + partial_x rz.
```

These rows include all requested couplings:

- `qalpha_x` is the live lapse Hessian in `K` and the four trace-free `A`
  rows;
- `K_x`, `Theta_x`, and the longitudinal/transverse shift Hessians enter the
  two `Gamma` rows with the target-`d=4` coefficients `3/2`, `3/2`, and `1`;
- the complete `Gamma` principal rows, not homogeneous substitutes, are
  copied into `B`; and
- `dww`/`Aww` carry coefficient two only where a four-dimensional trace
  requires the two hidden tensor copies.

The zero-order block is

```text
A_U^x=diag(-b,-b,-b,-b,-b,0,0,0).
```

There is no principal `partial_x W` term in the `U` rows and no principal
`partial_x U` term in the `W` rows, so the full matrix is explicitly

```text
A^x = [ -M    0 ],
      [  0   A_U^x ].
```

This is a mathematically complete matrix recipe with fixed ordering and no
runtime eigensystem.

## 3. Spectrum and the exact obstruction

The characteristic polynomial of the full 26-by-26 `A^x` is

```text
p(lambda_x) = lambda_x^5 (lambda_x+b)^7
              ((lambda_x+b)^2-1)^4
              (lambda_x^2+b lambda_x-2)
              (lambda_x^2+b lambda_x-3/4)
              (lambda_x^2+b lambda_x-9/8).
```

The exact multiplicities are:

| coordinate eigenvalue | family | algebraic multiplicity | geometric multiplicity |
|---|---|---:|---:|
| `-b+1`, `-b-1` | coupled physical/Z4 light cones | 4 each | 4 each |
| `(-b+/-sqrt(b^2+8))/2` | live lapse | 1 each | 1 each |
| `(-b+/-sqrt(b^2+3))/2` | complete transverse shift sector | 1 each | 1 each |
| `(-b+/-sqrt(b^2+9/2))/2` | complete longitudinal shift sector | 1 each | 1 each |
| `0` | `Bx-GammaX`, `Bz-GammaZ`, and zero-order lapse/shifts | 5 | 5 |
| `-b` | derivative algebraic pair plus five zero-order advected fields | 7 | **6** |

The defect is already present in the 18-by-18 `W` block. In the equivalent
`M` matrix the eigenvalue is `mu=b`, with algebraic multiplicity two and
geometric multiplicity one. One right eigenvector is

```text
R0: qchi=1, dxx=3, dzz=1, dww=1; all other entries zero.
```

One generalized right vector satisfying `(M-bI)R1=R0` is

```text
R1: dxx=8b/3, K=3, Axx=Azz=Aww=-3/4, Theta=3/2,
    GammaX=4b/3, rx=1, Bx=4b/3; all other entries zero.
```

The sole left eigenfield and a generalized left field are

```text
C_A/2 = (Axx+Azz+2Aww)/2,
-D_h/4 = -(dxx+dzz+2dww)/4,

partial_t(C_A/2) = b partial_x(C_A/2) + lower order,
partial_t(-D_h/4) = b partial_x(-D_h/4)
                     + partial_x(C_A/2) + lower order.
```

`D_h` is the radial derivative of the linearized conformal determinant
constraint `C_h=hxx+hzz+2hww`. Therefore `C_h` and `C_A` are not two
independent incoming characteristic rows that cleanup permits the boundary to
ignore. They form the missing-eigenvector Jordan chain in the complete live
principal system. Valid-cell cleanup restricts the state after RK updates,
but it does not diagonalize the unprojected RHS principal matrix used during
an RK stage and is not a continuum boundary equation.

Consequently every putative full left or right characteristic matrix has

```text
rank <= 25, sigma_min=0, kappa_2=infinity
```

for every finite `b>0`. There is no inverse transformation and no finite
conditioning bound anywhere on the intended radial domain. A complete
strongly-hyperbolic characteristic boundary cannot be obtained without a
human-approved formulation change or an explicitly derived constrained
principal system that removes the algebraic pair. Neither is authorized in
M2-A or delegated to M2-B.

## 4. Exact `B-Gamma` companions and shift sign

The copied live rows give, without approximation,

```text
partial_t(Bx-GammaX)=lower order,
partial_t(Bz-GammaZ)=lower order.
```

Both companions have coordinate principal speed zero. The transverse
five-variable sector

```text
(dxz,Axz,GammaZ,rz,Bz)
```

is independently diagonalizable and is useful for checking the shift sign.
For an `M`-eigenvalue `mu` satisfying

```text
mu^2-b mu-F=0, F=3/4,
```

the exact left shift characteristic is

```text
W_beta_z(mu) = rz + b GammaZ + (mu-b) Bz
             = rz + mu GammaZ + (F/mu)(Bz-GammaZ).
```

Direct multiplication gives `L M=mu L`. In coordinate-speed notation
`lambda_x=-mu`, the coefficient of `Bz` is `-(lambda_x+b)`. This corrects the
sign in the former homogeneous-shift proposal. The other transverse rows are

```text
mu=b+1:  W=-dxz+2Axz+GammaZ,
mu=b-1:  W=-dxz-2Axz+GammaZ,
mu=0:    W=Bz-GammaZ.
```

The longitudinal shift roots in section 3 come from the complete coupled
scalar matrix, including `K`, `Theta`, `GammaX`, `rx`, and the copied `Bx`
row. They cannot be assembled into a full invertible transform because of
the algebraic Jordan block; no homogeneous longitudinal field is retained as
a boundary prescription.

The former frozen-gauge labels `J,F,G` are not silently omitted. At radial
principal order, `z_z=(GammaZ-dxz)/2`, so
`J=dxz+2z_z=GammaZ`; the live `rz/Bz` couplings place it inside the complete
transverse light/shift/zero sector rather than leave an independent advected
`J` field. The live lapse block likewise mixes the former
`F=p_xx+p_T-2Theta` into the lapse/scalar eigenvectors, and the longitudinal
shift block mixes the former generalized `G` into the scalar/shift rows. Thus
none of `J,F,G` is an additional independent outer incoming characteristic
row of the complete live matrix. Their exact disposition is “absorbed into
the complete coupled sectors”; the only surviving `-b` Jordan obstruction is
the determinant/weighted-trace pair derived in section 3.

## 5. Both radial normals and face classifications

For outward normal `n_x`, define

```text
A^(n)=n_x A^x,
lambda_n=n_x lambda_x,
beta^n=n_x b,
c_n=(lambda_n+beta^n)/alpha=n_x(lambda_x+b), alpha=1.
```

Thus the two normal matrices are explicitly

```text
outer: n_x=+1, A^(+)=A^x,
inner: n_x=-1, A^(-)=-A^x.
```

Every existing right/left eigenvector is unchanged by multiplying the matrix
by `-1`, while every eigenvalue changes sign. This is a matrix identity, not a
prose relabeling. The defective Jordan block remains defective for both
normals.

The reference geometry is `r0=1`, `x_in=0.5`, `x_out=4.5`, `dx=0.125`.
Negative `lambda_n` is incoming, positive is outgoing, and zero is glancing.
The displayed margin is `|lambda_n|`.

| face/sample | family, coordinate speeds | normal speeds `lambda_n` | classification and minimum margin |
|---|---|---|---|
| inner face `x=0.5`, `b=1.414214` | light `-0.414214,-2.414214` | `0.414214,2.414214` | both outgoing; `0.414214` |
| inner first cell `x=0.5625`, `b=1.333333` | light `-0.333333,-2.333333` | `0.333333,2.333333` | both outgoing; `0.333333` |
| inner face | lapse `0.874032,-2.288246`; shift-z `0.410927,-1.825141`; shift-x `0.567648,-1.981862` | `-0.874032,2.288246`; `-0.410927,1.825141`; `-0.567648,1.981862` | one incoming per gauge pair; incoming margin `0.410927` |
| inner first cell | lapse `0.896805,-2.230139`; shift-z `0.426240,-1.759573`; shift-x `0.586108,-1.919441` | `-0.896805,2.230139`; `-0.426240,1.759573`; `-0.586108,1.919441` | one incoming per gauge pair; incoming margin `0.426240` |
| inner face/first cell | defective advected `-b` | `1.414214` / `1.333333` | outgoing eigenvalue, but one field missing; field-coverage margin zero |
| outer face `x=4.5`, `b=0.471405` | light `0.528595,-1.471405` | same | one outgoing/one incoming; incoming margin `1.471405` |
| outer first cell `x=4.4375`, `b=0.474713` | light `0.525287,-1.474713` | same | one outgoing/one incoming; incoming margin `1.474713` |
| outer face | lapse `1.198019,-1.669423`; shift-z `0.661825,-1.133230`; shift-x `0.850831,-1.322236` | same | one outgoing/one incoming per pair; incoming margin `1.133230` |
| outer first cell | lapse `1.196637,-1.671350`; shift-z `0.660607,-1.135320`; shift-x `0.849537,-1.324250` | same | one outgoing/one incoming per pair; incoming margin `1.135320` |
| outer face/first cell | defective advected `-b` | `-0.471405` / `-0.474713` | incoming eigenvalue, but one field missing; field-coverage margin zero |
| both faces | five zero-speed companions | `0` | glancing; speed margin zero |

Strict inner excision remains disallowed: three gauge fields enter at the
inner face, and the full field inventory is incomplete because of the Jordan
block. A mixed inner boundary also cannot be locked because there is no
complete left transform with which to enumerate and preserve every incoming,
outgoing, and glancing field.

## 6. Boundary and reconstruction hard stop

The former least-norm ghost proposal is withdrawn. A complete reconstruction
would have used the exact ghost unknown ordering

```text
g=(delta U[-1,slot 0..17],
   delta U[-2,slot 0..17],
   delta U[-3,slot 0..17])^T                         (54 entries),
```

where `-1` is nearest the physical face on either side. A valid equation
ordering would have to contain, in order, all incoming characteristic data,
equality constraints for every outgoing characteristic extrapolant, explicit
glancing/advected treatment, `C_h/C_A`, and GP fixed-point rows. The complete
left transform needed to construct those rows has rank at most 25. Therefore
the matrix `R` in

```text
R g = d
```

cannot be specified with complete field coverage, regardless of ghost-layer
scaling, weighting, QR/SVD orientation, rank tolerance, residual tolerance,
or conditioning limit. Any claimed factorization would either omit the
Jordan companion or add a non-characteristic equation. The mandatory
conditioning test fails identically (`kappa_2=infinity`), before a numerical
rank threshold is applied.

No inner data, outer data, outgoing equality constraint, valid-surface RHS
replacement, or all-18-slot ghost map is approved by this design. The current
provisional componentwise ghost fill and outer valid-surface override remain
historical implementation state only; M2-A does not approve them for M2-B.
There is no double application because no repaired boundary implementation
is authorized.

The production response is a hard stop and human roadmap review. An SAT
fallback is not specified or authorized.
