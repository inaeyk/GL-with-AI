# Custom Solver versus GRChombo Comparison Batch 1

Status: executed source and pointwise comparison evidence. This batch adds no
production solver functionality and does not alter the frozen stationary outer
boundary.

## Reproducibility manifest

| Item | Inspected value | Status |
|---|---|---|
| Custom repository | `661468ade479cf003dc5336e665dc7b70edf48c6` (`Add custom solver GRChombo comparison inventory`) | clean starting worktree and index |
| GRChombo source | `37e659523830418b210acea1661dac0e00bb1b75` | clean detached HEAD; no local branch |
| GRChombo remote | `origin = https://github.com/GRTLCollaboration/GRChombo.git` | source identity recorded; nested checkout remains ignored rather than top-level pinned |
| Compiler | `g++ (Ubuntu 15.2.0-16ubuntu1) 15.2.0` | direct bridge compiled |
| Language and flags | `-std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror` | focused fixture compiled cleanly |
| Direct comparison dimensions | `CH_SPACEDIM=3`, `GR_SPACEDIM=3`, `DEFAULT_TENSOR_DIM=3` | explicit compile definitions; this is the inspected stock GRChombo dimension |
| Custom production dimensions | physical `d=4`, gridded `d=2`, hidden multiplicity `2` | unchanged and checked by compile-time assertions |
| GRChombo formulation path | `TensorAlgebra` and `CCZ4Geometry::compute_ricci`; pure physical Ricci with `dZ_coeff=0` | invoked directly from inspected headers |
| Chombo | no local Chombo tree or static library in the ignored checkout; not needed by the header-only bridge | exact revision/digest unresolved |
| PETSc | not needed by this batch | version/build identity unresolved |
| Container | configured name `grchombo/grchombo`; Docker is unavailable in this WSL shell | image ID and digest unresolved |
| Host | Linux WSL2 `6.18.33.2-microsoft-standard-WSL2`, `x86_64` | recorded |
| Fixture | `code/BlackStringToy/tests/Stage4AOCGRChomboComparisonBatch1Test.cpp` | focused test-only bridge |
| Fixture SHA-256 | `fe9a55c2189e82afd73ff59d8449ad93565244099fb9cc01400c5743e9d1870d` | repaired executable layout gate |
| Pointwise tolerance | `5e-13 + 5e-12 max(abs(a),abs(b))` | unchanged from the predeclared contract |

The missing Chombo and container digests remain a P0 reproducibility gap. They
did not block this batch because the compiled fixture calls the source checkout
through header-only `TensorAlgebra` and `CCZ4Geometry` code paths.

## State and dimension map

The custom frozen state is a reduced `d=4` oracle. The current BlackStringToy
scaffold is a 27-slot extension of the stock three-dimensional layout. The
compiled GRChombo comparison path is stock `d=3`; no `d=4` coefficient is
numerically compared to a `d=3` coefficient.

The original batch-1 executable gate checked the frozen thirteen-variable
order and stock enum but did not include the real custom `UserVariables.hpp`.
It therefore did not prove the numeric custom slots. That gate is superseded.
The repaired fixture compiles both actual headers, with the stock enum isolated
in a test-only namespace, and checks the following independently inspected
correspondence.

| Frozen slot | Custom variable | BlackStringToy scaffold slot | Stock GRChombo `d=3` slot | Classification |
|---:|---|---:|---:|---|
| 0 | `chi` | 0 | 0 | direct |
| 1 | `hxx` / `h11` | 1 | 1 | direct |
| 2 | `hxz` / `h13` | 3 | 3 | direct |
| 3 | `hzz` / `h33` | 6 | 6 | direct |
| 4 | `hww` | 7 | none | custom hidden representative |
| 5 | `K` | 8 | 7 | direct, shifted by custom `hww` |
| 6 | `Axx` / `A11` | 9 | 8 | direct |
| 7 | `Axz` / `A13` | 11 | 10 | direct |
| 8 | `Azz` / `A33` | 14 | 13 | direct |
| 9 | `Aww` | 15 | none | custom hidden representative |
| 10 | `Theta` | 16 | 14 | direct |
| 11 | `hatGamma_x` / `Gamma1` | 17 | 15 | direct |
| 12 | `hatGamma_z` / `Gamma3` | 19 | 17 | direct |

The complete custom enum and ownership table is:

| Custom slot | Custom name | Corresponding stock slot | Ownership |
|---:|---|---:|---|
| 0 | `chi` | 0 | directly shared |
| 1 | `h11` | 1 | directly shared |
| 2 | `h12` | 2 | stock visible-y component |
| 3 | `h13` | 3 | directly shared |
| 4 | `h22` | 4 | stock visible-y component |
| 5 | `h23` | 5 | stock visible-y component |
| 6 | `h33` | 6 | directly shared |
| 7 | `hww` | none | custom hidden representative |
| 8 | `K` | 7 | directly shared |
| 9 | `A11` | 8 | directly shared |
| 10 | `A12` | 9 | stock visible-y component |
| 11 | `A13` | 10 | directly shared |
| 12 | `A22` | 11 | stock visible-y component |
| 13 | `A23` | 12 | stock visible-y component |
| 14 | `A33` | 13 | directly shared |
| 15 | `Aww` | none | custom hidden representative |
| 16 | `Theta` | 14 | directly shared |
| 17 | `Gamma1` | 15 | directly shared |
| 18 | `Gamma2` | 16 | stock visible-y component |
| 19 | `Gamma3` | 17 | directly shared |
| 20 | `lapse` | 18 | gauge variable |
| 21 | `shift1` | 19 | gauge variable |
| 22 | `shift2` | 20 | gauge variable |
| 23 | `shift3` | 21 | gauge variable |
| 24 | `B1` | 22 | gauge variable |
| 25 | `B2` | 23 | gauge variable |
| 26 | `B3` | 24 | gauge variable |

Compile-time assertions verify every numeric custom enum value, range, and the
unique complete permutation `[0,27)`. The runtime gate verifies exactly eleven
shared frozen correspondences; `hww/Aww` ownership; and separation of seven
visible-y and seven gauge slots. It does not claim that the 27-slot custom and
25-slot stock arrays are identical.

Executed mapping mutations all fail: swapped `c_h11/c_h13`, `hww` assigned to
`h12`, `Aww` assigned to `A12`, duplicated `A11` for shared `A13`, and omitted
shared `Gamma3`. Each mutation changes active mapping data before rejection.

Dimension-independent identities compared here are metric inversion,
raising/lowering, contractions, `gamma=h/chi`, and the definition of
`Z_over_chi`. Trace removal and `Kij=Aij/chi+gamma_ij K/d` are
dimension-dependent. The production custom denominator remains four; the
direct stock comparison uses three. A deliberate `/4` trace-removal mutation
against the compiled `/3` path fails.

## Directness classification

| Capability | Classification | Evidence or blocker |
|---|---|---|
| determinant, inverse, trace, trace-free projection, raising/lowering | direct compiled GRChombo comparison | calls inspected `TensorAlgebra` templates |
| `gamma_ij=h_ij/chi` and conformal definition | source/convention comparison only | represented by the GRChombo variable contract; no standalone production function exists |
| `Kij=(Aij+hij K/d)/chi` | source/convention comparison only | `ADMConformalVars` stores the convention; no directly callable reconstruction helper exists |
| visible contracted conformal connection | direct compiled GRChombo comparison | central derivative of direct `TensorAlgebra::compute_christoffel` about the flat conformal metric |
| visible `Z_over_chi` | direct connection output plus source-locked GRChombo reconstruction | `0.5*(Gamma-chris.contracted)` is the exact `CCZ4Geometry` convention |
| physical Ricci tensor/scalar | direct compiled GRChombo comparison | calls `CCZ4Geometry::compute_ricci` with consistent contracted-Gamma jets |
| `hww/Aww`, hidden contracted connection, hidden Ricci/cartoon terms | custom-only, not directly comparable | stock commit contains no hidden representative or modified-cartoon production path |
| full Chombo-grid production invocation | blocked | local Chombo/container identity and build are unavailable; not substituted with copied coefficients |

## Comparison A: conformal algebra

Three deterministic positive-definite metrics were used, including full
off-diagonal data. Direct GRChombo determinant, inverse, trace, trace-free
projection, vector raising, and lowering were compared to a test-only
dimension-general linear-algebra oracle at `d=3`.

| Maximum | Value | Worst case/component | Result |
|---|---:|---|---|
| absolute error | `4.4408920985006262e-16` | metric case 2, determinant | pass |
| normalized error | `6.8074202814251110e-05` | metric case 2, determinant | pass |

The `gamma=h/chi` and `Kij` reconstruction conventions agree at source level.
They are not labeled direct executable comparisons because the inspected
GRChombo headers expose storage conventions rather than standalone production
functions for those transformations.

Negative control: replacing the compiled `d=3` trace-free denominator by
`d=4` is detected well outside the tolerance.

## Comparison B: contracted connection and encoded Z

The cases are diagonal, off-diagonal, pure hatted-Gamma, and mixed. The
compiled GRChombo connection path is differentiated about the flat conformal
metric and compared with the visible part of the production custom linearized
helper. The custom hidden increment is separated explicitly:

```text
Delta Gamma^x_hidden = -partial_x hww + 2(hxx-hww)/x
Delta Gamma^z_hidden = -partial_z hww + 2 hxz/x
Delta Z_over_chi_hidden = -0.5 Delta Gamma_hidden
```

| Maximum | Value | Worst case/component | Result |
|---|---:|---|---|
| absolute error | `1.8374191057546341e-14` | mixed, contracted `Gamma^x` | pass |
| normalized error | `1.0351656933828390e-02` | mixed, `Z_over_chi^x` | pass |

The visible stock contribution passes. The displayed hidden correction is
custom-only evidence and is not called a GRChombo match. A deliberate
contracted-connection sign mutation fails.

## Comparison C: visible physical Ricci

The direct side calls `CCZ4Geometry::compute_ricci` at `d=3`. The independent
test-only oracle first forms the physical metric and its derivatives from
`gamma=h/chi`, then computes Christoffels, Christoffel derivatives, the Ricci
tensor, and scalar from the general differential-geometric definition. It
does not call or copy the GRChombo Ricci implementation.

Conformal metric jets satisfy `det(h)=1` through second derivative order.
Cases are flat, diagonal-curved, off-diagonal, conformal-factor-only, and
mixed. All six stock Ricci tensor components and the scalar are compared; the
custom production-relevant overlap is `Rxx`, `Rxz`, and `Rzz`. Hidden/cartoon
additions are excluded.

| Maximum | Value | Worst case/component | Result |
|---|---:|---|---|
| absolute error | `2.2204460492503131e-16` | off-diagonal, Ricci scalar | pass |
| normalized error | `1.5299362891930797e-04` | off-diagonal, `Rxz` | pass |

A deliberate `Rxz` sign mutation fails. This result validates the visible
same-dimension formula path only; it does not validate `Rww` or hidden
multiplicity in production GRChombo.

## Batch disposition

The repaired exact custom 27-slot layout, eleven shared correspondences, and
hidden/visible-y/gauge ownership separation pass. This is not an identical
complete-state-array claim. All three numerical families also pass unchanged.
The required translation is explicit: the stock source was compiled and
evaluated at `d=3`, while the production custom oracle remains `d=4` with two
hidden directions. The outer-boundary research remains frozen.

Recommended next batch:

1. resolve or formally pin the GRChombo/Chombo/container dependency tuple;
2. compare encoded-Z Ricci increments separately from raw Ricci;
3. compare visible `chi`, metric, `K`, `Theta`, and `A` RHS families at
   identical analytic jets in stock `d=3`;
4. introduce the reviewed target-dimension adapter before any `d=4`
   production comparison;
5. keep hidden/cartoon production adaptation separate until its source owner
   and component map are approved.
