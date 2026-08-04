# M2-A algebraic-manifold radial characteristic derivation

Status: `M2-A DESIGN READY`

This is the revised, algorithm-consistent M2-A design. It does not discard
the failed full-state calculation: Appendix A preserves its state, polynomial,
and Jordan chain as historical evidence. The repair is to analyse the tangent
space on which the production algorithm actually starts every derivative
evaluation, after determinant and weighted-trace cleanup. No production code,
test, parameter, build file, dependency, or numerical evidence is changed.

## 1. Locked source path and conventions

The committed path is

```text
BlackStringToyLevel::specificEvalRHS
  -> BlackStringLive::RHSCompute
  -> BlackStringTargetCCZ4Pointwise::LockedRHSAccess
  -> CCZ4RHS::rhs_equation
  -> MovingPunctureGauge::rhs_gauge
  -> project-owned fixed GP lapse source
  -> one fused KO addition
```

`BlackStringToyLevel::specificUpdateODE` then applies the standard
determinant and hidden-aware weighted-trace cleanup after every RK update.
The framework performs periodic/internal exchange before the next derivative
consumer. Internal MPI seams are exchange-owned and never physical-boundary
owned.

The target has `d=4`, `USE_CCZ4`, and

```text
kappa1=0.1, kappa2=0, kappa3=1, covariant_z4=true, Lambda=0,
lapse_advec_coeff=0, lapse_power=1, lapse_coeff=2,
shift_Gamma_coeff=F=3/4, shift_advec_coeff=0, eta=1.
```

The 18 stored slots are

```text
(chi,hxx,hxz,hzz,hww,K,Axx,Axz,Azz,Aww,Theta,GammaX,GammaZ,
 alpha,betaX,betaZ,Bx,Bz).
```

At a frozen GP point, in units retained explicitly through `r0`,

```text
alpha=chi=hxx=hzz=hww=1, hxz=0, betaX=b=sqrt(r0/x), betaZ=0,
K=3 lambda/2,
(Axx,Axz,Azz,Aww)=lambda(-7/8,0,-3/8,5/8),
Theta=GammaX=GammaZ=Bx=Bz=0, lambda=sqrt(r0/x^3).
```

The fixed lapse source, damping, GP coefficient gradients, `z` Fourier
terms, and explicit cartoon `1/x` terms are lower order in the radial symbol.
The two hidden `w` directions contribute algebraic multiplicity two only;
they are never grid derivatives.

The sign convention is

```text
partial_t V + A_alg^x partial_x V = lower order.
```

## 2. Explicit algebraic elimination

Let

```text
Delta = hxx*hzz-hxz^2.
```

The independent stored variables omit `hww` and `Aww`. On the positive
metric branch the exact nonlinear reconstruction is

```text
hww = Delta^(-1/2) > 0,
N   = hzz*Axx-2*hxz*Axz+hxx*Azz,
Aww = -N/(2*Delta^(3/2)).
```

These formulas enforce

```text
Delta*hww^2=1,
h^xx Axx+2 h^xz Axz+h^zz Azz+2 Aww/hww=0
```

identically, including the two hidden tensor copies. The admissibility gate
requires every input finite, `hxx>0`, `hzz>0`, `Delta>0`,

```text
Delta >= 1e-14*max(hxx,hzz)^2,
kappa_2([[hxx,hxz],[hxz,hzz]]) <= 1e8.
```

Failure is `M2-B ALGEBRAIC_RECONSTRUCTION_ADMISSIBILITY_FAILURE`; no absolute
value, branch switch, clamp, or continued stencil evaluation is allowed.

At GP the tangent relations are

```text
delta hww = -(delta hxx+delta hzz)/2,
partial_x delta hww = -(dxx+dzz)/2,

delta Aww = -(delta Axx+delta Azz)/2
             -(3 lambda/4) delta hxx-(lambda/2) delta hzz.
```

Only the first two `A` terms remain in the first-order radial principal
relation; the background-`A` metric variations are algebraic lower order.
These explicit relations are the tangent projector `P_alg`; no abstract
nullspace basis and no evolution-space projection are used.

## 3. Reduced first-order principal matrix

Introduce

```text
qchi=partial_x delta chi,
(dxx,dxz,dzz)=partial_x(delta hxx,delta hxz,delta hzz),
qalpha=partial_x delta alpha,
(rx,rz)=partial_x(delta betaX,delta betaZ).
```

The independent derivative/dynamical state is

```text
W=(qchi,dxx,dxz,dzz,K,Axx,Axz,Azz,Theta,
   GammaX,GammaZ,qalpha,rx,rz,Bx,Bz)^T.               (16)
```

The independent zero-order state is

```text
U=(delta chi,delta hxx,delta hxz,delta hzz,
   delta alpha,delta betaX,delta betaZ)^T.            (7)
```

Thus `V_alg=(W,U)^T` has 23 entries. Define
`partial_t W=M_alg partial_x W+lower order`; then `A_W^x=-M_alg`.
The complete sparse definition is

```text
partial_t qchi = b partial_x qchi
               + 1/2 partial_x K - 1/2 partial_x rx,

partial_t dxx = b partial_x dxx - 2 partial_x Axx
              + 3/2 partial_x rx,
partial_t dxz = b partial_x dxz - 2 partial_x Axz + partial_x rz,
partial_t dzz = b partial_x dzz - 2 partial_x Azz
              - 1/2 partial_x rx,

partial_t K = b partial_x K
            + partial_x(3 qchi+GammaX-qalpha),
partial_t Axx = b partial_x Axx
              + partial_x(3 qchi/4-dxx/2
                          +3 GammaX/4-3 qalpha/4),
partial_t Axz = b partial_x Axz
              + partial_x(-dxz/2+GammaZ/2),
partial_t Azz = b partial_x Azz
              + partial_x(-qchi/4-dzz/2
                          -GammaX/4+qalpha/4),
partial_t Theta = b partial_x Theta
                + partial_x(3 qchi/2+GammaX/2),

partial_t GammaX = b partial_x GammaX
                 + partial_x(-3 K/2+2 Theta+3 rx/2),
partial_t GammaZ = b partial_x GammaZ + partial_x rz,
partial_t qalpha = partial_x(-2 K+4 Theta),
partial_t rx = F partial_x Bx,
partial_t rz = F partial_x Bz,
partial_t Bx = b partial_x GammaX
             + partial_x(-3 K/2+2 Theta+3 rx/2),
partial_t Bz = b partial_x GammaZ + partial_x rz.
```

This includes the lapse Hessian in `K,Axx,Azz`, all `K,Theta` and shift
Hessians in `Gamma`, and the complete `Gamma` principal rows copied into
`B`. The eliminated `dww,Aww` contributions are already present through the
relations in section 2. The zero-order block is

```text
A_U^x=diag(-b,-b,-b,-b,0,0,0),
A_alg^x=diag(-M_alg,A_U^x).
```

Direct substitution verifies that the dependent `dww,Aww` rows are the time
derivatives of the tangent relations at principal order. Therefore this is
equivalently the restriction of `P_alg A^x P_alg` to the explicit independent
coordinates.

## 4. Spectrum and complete characteristic basis

The reduced characteristic polynomial is

```text
p_alg(lambda_x)=lambda_x^5 (lambda_x+b)^4
 ((lambda_x+b)^2-1)^4
 (lambda_x^2+b lambda_x-2)
 (lambda_x^2+b lambda_x-3/4)
 (lambda_x^2+b lambda_x-9/8).
```

It has 23 independent eigenvectors. The coordinate speeds and multiplicities
are

| coordinate speed `lambda_x` | family | multiplicity |
|---|---|---:|
| `-b+1`, `-b-1` | physical/Z4 light | 4 each |
| `(-b +/- sqrt(b^2+8))/2` | lapse | 1 each |
| `(-b +/- sqrt(b^2+3))/2` | transverse shift | 1 each |
| `(-b +/- sqrt(b^2+9/2))/2` | longitudinal shift | 1 each |
| `-b` | independent advected `chi,hxx,hxz,hzz` | 4 |
| `0` | `Bx-GammaX`, `Bz-GammaZ`, lapse and two shifts | 5 |

The `B-Gamma` companions follow exactly from the copied live rows:

```text
partial_t(Bx-GammaX)=lower order,
partial_t(Bz-GammaZ)=lower order.
```

### 4.1 Exact scalar transform

Permute the scalar block to

```text
S=(qchi,dxx,dzz,K,Axx,Azz,Theta,GammaX,qalpha,rx,Bx)^T.
```

Let `sigma=+1,-1`, `mu_sigma=b+sigma`, and define these right columns:

```text
R_X(sigma)=(0,-2 sigma,0,0,1,0,0,0,0,0,0)^T,
R_Z(sigma)=(0,0,-2 sigma,0,0,1,0,0,0,0,0)^T,

R_C(sigma)=(-(4b+sigma)/4, 9sigma/8,-3sigma/8,
            -sigma(8b-sigma)/4,0,0,
            -sigma(8b-sigma)/8,mu_sigma,0,3/4,mu_sigma)^T.
```

For either lapse root `mu^2-b mu=2`,

```text
R_L(mu)=(-1/3,1,-1/3,-7/(12mu),-7/(16mu),7/(48mu),
         0,1,7/(6mu^2),3/(4mu),1)^T.
```

For either longitudinal-shift root `mu^2-b mu=9/8`,

```text
R_S(mu)=(-1/3,1,-1/3,0,0,0,0,1,0,3/(4mu),1)^T.
```

The scalar zero-speed right column is

```text
R_0=(1/2,-3/2,1/2,0,0,0,0,-3/2,0,b,0)^T.
```

For every column, `M_alg R=mu R`; the coordinate eigenvalue is
`lambda_x=-mu`. Order the columns as

```text
R_s=[R_X(+),R_Z(+),R_C(+),R_X(-),R_Z(-),R_C(-),
     R_L(mu_L+),R_L(mu_L-),R_S(mu_S+),R_S(mu_S-),R_0].
```

The following explicit left rows span the same eigenspaces:

```text
ell_X(sigma)=(-3sigma/2,-sigma/2,0,-3/4,1,0,0,0,0,0,0),
ell_Z(sigma)=( sigma/2,0,-sigma/2,1/4,0,1,0,0,0,0,0),
ell_C(sigma)=(3,0,0,0,0,0,2sigma,1,0,0,0),

ell_L(mu)=(0,0,0,-mu,0,0,2mu,0,1,0,0).
```

For a longitudinal-shift root put `D=64mu^2-81` and use

```text
ell_S(mu)=(1792mu^4,0,0,32mu^3 D,0,0,
           -128mu^3(32mu^2-51),
           7(256mu^4-408mu^2+243),
           -32mu^2 D,28mu D,21D).

ell_0=(0,0,0,0,0,0,0,-1,0,0,1).
```

These signs come from direct multiplication by the locked principal matrix.
In particular, no homogeneous shift formula is imported.

The exact left transformation requires no unresolved normalization. For each
light sign define

```text
L_sigma=[ell_X(sigma);ell_Z(sigma);ell_C(sigma)],
G_sigma=[[2,0,3sigma*b-3/8],
         [0,2,1/8-sigma*b],
         [0,0,sigma/2-4b]],

G_sigma^(-1)=[[1/2,0,3sigma/8],
               [0,1/2,-sigma/8],
               [0,0,-2/(8b-sigma)]].
```

The three normalized light rows are `G_sigma^(-1)L_sigma`. The normalized
lapse, longitudinal-shift, and zero rows are respectively

```text
ell_L(mu) / [7(mu^2+2)/(12mu^2)],
ell_S(mu) / [7(64mu^2-81)(8mu^2+9)/3],
(2/3)ell_0.
```

Putting those rows in the column order above gives `T_s`, and direct
multiplication gives

```text
T_s R_s=I_11,  T_s M_s R_s=diag(mu),  T_s=R_s^(-1).
```

Thus both the transform and inverse are explicit functions of `b`; no
runtime eigensystem, sign choice, pivot choice, or arbitrary nullspace basis
remains.

### 4.2 Exact transverse transform

For

```text
Z=(dxz,Axz,GammaZ,rz,Bz)^T
```

the right columns are

```text
R_z,light(sigma)=(-2sigma,1,0,0,0)^T,
R_z,shift(mu)=(1,0,1,3/(4mu),1)^T,
R_z,0=(-1,0,-1,b,0)^T,
```

where the shift roots satisfy `mu^2-b mu=3/4`. The dual rows are

```text
T_z,light(sigma)=(-1,2sigma,1,0,0)/(4sigma),
T_z,shift(mu)=(0,0,b,1,mu-b)/(2mu-b),
T_z,0=(0,0,-1,0,1).
```

The locked shift field is therefore

```text
rz+b GammaZ+(mu-b)Bz
=rz+mu GammaZ+(3/(4mu))(Bz-GammaZ),
```

which verifies the corrected sign directly. With columns/rows ordered
`light(+),light(-),shift(+),shift(-),zero`, `T_z R_z=I_5`.

The four advected and three zero-speed `U` transforms are identity. A fixed
permutation between `(S,Z,U)` and the stated `V_alg` ordering completes the
23-by-23 `T_+` and `R_+=T_+^{-1}`.

### 4.3 Both radial normals

Let `D_-` multiply exactly

```text
(qchi,dxx,dxz,dzz,qalpha,rx,rz)
```

by `-1` and leave every dynamical and zero-order entry unchanged; let
`D_+=I`. With outward-normal derivative variables `V_n=D_n V_x`, the two
normal symbols and transforms are explicitly

```text
A_alg^(n)=n D_n A_alg^x D_n,
R_n=D_n R_+,
T_n=T_+ D_n,
T_n R_n=I,
lambda_n=n lambda_x.
```

This is the implemented inner transform, not a prose sign reversal. Since
`D_-` is orthogonal, both normals have identical singular values and
condition numbers.

## 5. Strong-hyperbolicity and conditioning gate

Scale all `W` entries by `r0` and leave dimensionless `U` entries unchanged,
then normalize every right column to unit two-norm. On the complete locked
domain `x/r0 in [0.5,4.5]`, hence
`b in [sqrt(2/9),sqrt(2)]`, a deterministic 200001-point monotone audit plus
the exact denominator checks above gives

| block | maximum `kappa_2` | location |
|---|---:|---|
| scalar 11-by-11 | `36.701446` | `x/r0=0.5` |
| transverse 5-by-5 | `6.964910` | `x/r0=0.5` |
| full 23-by-23 | `36.701445735` | `x/r0=0.5` |

The full values are `35.539033` at the first inner valid cell,
`kappa_2(b=1)=31.112685441` at `x=r0`, `26.826276` at the first outer valid
cell, and `26.825692` at the outer face. The horizon crossing `b=1` is semisimple:
five permanent zero-speed characteristic fields are joined by four light-speed
fields that additionally become zero there. The total zero-eigenvalue
algebraic multiplicity is nine and the total geometric multiplicity is nine;
all nine eigenvectors remain independent, and the reduced 23-by-23 transform
remains full rank and diagonalizable at the horizon. The only positive-`b`
transform denominator collision is `b=1/8`, outside the locked domain. Both
normals pass the fixed `kappa_2<=1e3` gate with a factor greater than 27 margin.
M2-B hard-fails on nonfinite coefficients, rank below 23 at relative threshold
`1e-12`, `kappa_2>1e3`, or transform/inverse residual above `5e-12`.

## 6. Both-face classification and margins

Negative `lambda_n` is incoming, positive is outgoing, and zero is glancing.
The reference geometry is `r0=1`, `x_in=0.5`, `x_out=4.5`, `dx=0.125`.

| sample | classification | minimum nonzero margin |
|---|---|---:|
| inner face, `b=1.414214` | both light branches, four advected fields, and the positive lapse/shift roots outgoing; negative lapse, transverse-shift, and longitudinal-shift roots incoming; five zero fields glancing | incoming `0.410927`; outgoing `0.414214` |
| inner first cell, `b=1.333333` | same | incoming `0.426240`; outgoing `0.333333` |
| outer face, `b=0.471405` | `mu=b+1` light quartet, positive lapse/shift roots, and four advected fields incoming; `mu=b-1` light quartet and negative lapse/shift roots outgoing; five zero fields glancing | incoming `0.471405`; outgoing `0.528595` |
| outer first cell, `b=0.474713` | same | incoming `0.474713`; outgoing `0.525287` |

At the inner face the incoming normal speeds are exactly the negative roots

```text
-0.874032 (lapse), -0.410927 (transverse shift),
-0.567648 (longitudinal shift),
```

and at the first valid cell they are `-0.896805,-0.426240,-0.586108`.
Therefore strict excision is disallowed. The mixed inner boundary has complete
coverage and is the locked design.

## 7. Incoming data and outgoing ownership

All conditions act on `delta U=U-U_GP`.

At the inner face:

- all light, advected-metric, and positive-root gauge fields are outgoing and
  are fourth-order extrapolants of interior characteristic values;
- the negative-root lapse, transverse-shift, and longitudinal-shift fields
  are incoming and receive fixed GP gauge data, exactly zero in `delta U`;
- `Bx-GammaX`, `Bz-GammaZ`, and zero-order lapse/shifts are glancing and use
  equality to their fourth-order interior extrapolants.

At the outer face:

- the two scalar tensor rows `T_X(+),T_Z(+)` receive homogeneous
  GP-subtracted radiative physical data;
- the scalar `T_C(+)` and transverse `T_z,light(+)` rows receive homogeneous
  constraint-preserving Z4 data;
- the positive-root lapse and two shift rows receive fixed GP gauge data;
- the four incoming advected `chi,hxx,hxz,hzz` rows receive fixed GP
  background data;
- every negative-root field is equality-constrained to its fourth-order
  interior characteristic extrapolant; and
- all five glancing fields use equality to their fourth-order interior
  extrapolants.

The algebraic determinant and weighted trace have no independent incoming
rows on the reduced manifold. They are enforced exactly by reconstruction,
not omitted because later cleanup happens. The former `J,F,G` inventory is
also complete: transverse `J` is in `T_z,light`, lapse-coupled `F` is in the
lapse/scalar rows, and longitudinal `G` is in the complete scalar shift row.

## 8. Three-layer independent ghost reconstruction

At each physical radial surface let `y` be distance into the domain divided
by `dx`, so the first five valid centers are at
`y=(1/2,3/2,5/2,7/2,9/2)`. For values `u0,...,u4` there, the fixed degree-four
one-sided formulas are

```text
u_face = (315u0-420u1+378u2-180u3+35u4)/128,

partial_y u_face = -31u0/8+229u1/24-75u2/8+37u3/8-11u4/12,
partial_n u_face = -partial_y u_face/dx.
```

Use these formulas to evaluate outgoing and glancing characteristics. Insert
the fixed incoming data above and apply the exact inverse transform. This
yields at the boundary face:

```text
seven independent primitive values:
  chi,hxx,hxz,hzz,alpha,betaX,betaZ;
their seven outward-normal derivatives;
nine independent dynamical values:
  K,Axx,Axz,Azz,Theta,GammaX,GammaZ,Bx,Bz.
```

For each primitive, put `s_face=-dx*partial_n u_face`. The unique degree-five
polynomial is fixed by `(u_face,s_face)` plus its values `u0,...,u3` at the
first four valid centers. Its three ghost values, nearest to farthest, are

```text
g1=-9088u_face/3675-64s_face/35+4u0-2u1/3+4u2/25-u3/49,
g2=-18048u_face/245-192s_face/7+90u0-20u1+27u2/5-36u3/49,
g3=-19840u_face/49-960s_face/7+500u0-125u1+36u2-250u3/49.
```

For each dynamical field, the unique degree-four polynomial is fixed by its
face value `w_face` plus `w0,...,w3` at the first four valid centers:

```text
g1=128w_face/35-4w0+2w1-4w2/5+w3/7,
g2=128w_face/7-30w0+20w1-9w2+12w3/7,
g3=384w_face/7-100w0+75w1-36w2+50w3/7.
```

These rational coefficients are the complete fixed Hermite interpolation;
there is no stencil choice left to M2-B. Equivalently, with independent
ghost ordering

```text
g_alg=(layer_near slots
       chi,hxx,hxz,hzz,K,Axx,Axz,Azz,Theta,GammaX,GammaZ,
       alpha,betaX,betaZ,Bx,Bz;
       layer_middle same order;
       layer_far same order)^T                         (48)
```

the fixed formulas are the already-factorized square system
`R_alg g_alg=d_alg` with `R_alg=I_48`. Coordinates are scaled by `r0`;
primitive equations by
their field scale; derivative equations by `r0` times that scale; curvature,
Gamma, and `B` fields by `r0`. The norm is the resulting dimensionless
weighted two-norm. Require 48 finite outputs, scaled formula residual
`<=5e-12`, and the characteristic inverse gate from section 5. Any missing,
nonfinite, or inconsistent reconstruction is a hard boundary-fill failure.

After those 48 values are obtained, reconstruct `hww,Aww` in every ghost
cell with section 2 before any live derivative or KO stencil reads it. Thus
all 54 stored ghost values exist, but only 48 are independent, and every
ghost cell satisfies determinant and weighted trace by construction.
The interpolation is formally fourth order at the physical boundary and
costs `O(N_z)` with fixed-size stack storage and no per-cell allocation,
logging, virtual dispatch, or second volume RHS pass.

## 9. Outer valid-surface RHS and cleanup lifecycle

The locked outer implementation disables the provisional componentwise
`apply_outer_rhs` owner. Incoming principal data are imposed only by the
characteristic face/ghost reconstruction in section 8. The ordinary live
target operator then evaluates the 16 independent valid-surface RHS slots
once, using those ghosts, and fused KO remains part of that same evaluation.
There is no post-hoc transform of a 16-slot RHS by the 23-state first-order
matrix, no second RHS/KO evaluation, and no overwrite of outgoing fields.
Thus the characteristic ghost owner supplies incoming data and the live
operator owns every independent surface RHS. It never assigns an independent
componentwise boundary RHS to `hww,Aww`. Let overdots denote that independent
boundary RHS and set

```text
dotDelta=hzz*dothxx+hxx*dothzz-2*hxz*dothxz,
dotN=dothzz*Axx+hzz*dotAxx
     -2*(dothxz*Axz+hxz*dotAxz)
     +dothxx*Azz+hxx*dotAzz,

dothww=-(hww/(2Delta))*dotDelta,
dotAww=-dotN/(2Delta^(3/2))+3N*dotDelta/(4Delta^(5/2)).
```

These are the exact time derivatives of the nonlinear reconstruction. They
keep the valid-surface RHS tangent to the algebraic manifold. The subsequent
standard cleanup is not a second boundary condition: it removes the finite-RK
nonlinear drift after the update. No derivative, KO stencil, or exchange
observes the state between RK update and cleanup.

The exact lifecycle is

1. periodic and internal exchange; MPI seams stop here;
2. independent characteristic physical ghost fill;
3. nonlinear `hww,Aww` ghost reconstruction;
4. one live target-`d=4` RHS and one fused KO addition;
5. retain the single boundary-conditioned live RHS for all independent outer
   valid-surface slots and derive the dependent `hww,Aww` RHS by the chain
   rule above; the provisional componentwise outer override is absent;
6. RK update;
7. the existing algebraic cleanup before any subsequent derivative access.

Both physical boundaries have characteristic ghost fill only; the live outer
valid-surface RHS owner runs once and only once. GP-subtracted zero data and analytic GP
reconstruction make GP a fixed point to the fourth-order boundary truncation
error.

## 10. Gate and bounded fallback

The reduced symbol is complete and bounded, so one M2-B implementation cycle
may follow the required focused design audit. If transform rank, conditioning,
roundtrip, characteristic ownership, algebraic reconstruction, or GP
preservation fails, stop that cycle and make only the production change tied
to the failed row/stencil/reconstruction formula. Do not introduce SAT, WKB,
dense spectra, or a fixture-only projection.

If implementation evidence instead disproves the reduced symbol or it cannot
meet the locked conditioning gate, M2-A reclassifies
`M2-A DESIGN BLOCKED — REDUCED PRINCIPAL SYSTEM NOT STRONGLY HYPERBOLIC` and
returns to human review. The only remaining roadmap options are (1) a
human-approved gauge modification near the inner boundary or (2) a different
production inner geometry/treatment. No third characteristic formulation is
started automatically.

## Appendix A. Historical failed raw-state derivation

The superseded raw first-order state was

```text
W_raw=(qchi,dxx,dxz,dzz,dww,K,Axx,Axz,Azz,Aww,Theta,
       GammaX,GammaZ,qalpha,rx,rz,Bx,Bz),
U_raw=(chi,hxx,hxz,hzz,hww,alpha,betaX,betaZ).
```

Its 26-by-26 characteristic polynomial was

```text
lambda_x^5 (lambda_x+b)^7 ((lambda_x+b)^2-1)^4
(lambda_x^2+b lambda_x-2)
(lambda_x^2+b lambda_x-3/4)
(lambda_x^2+b lambda_x-9/8).
```

At `M_raw` eigenvalue `mu=b`, the derivative block had algebraic
multiplicity two and geometric multiplicity one. Its sole left eigenfield and
generalized companion were

```text
C_A/2=(Axx+Azz+2Aww)/2,
-D_h/4=-(dxx+dzz+2dww)/4,

partial_t(-D_h/4)=b partial_x(-D_h/4)
                  +partial_x(C_A/2)+lower order.
```

This determinant/weighted-trace Jordan block made the raw transform rank at
most 25 with infinite condition number. That conclusion remains correct for
the unprojected slots; it is preserved as evidence, not reinterpreted as a
complete raw characteristic basis. The revised design succeeds only because
the actual post-RK cleanup algorithm restricts every subsequent derivative
evaluation to the explicitly reconstructed algebraic-constraint manifold.
