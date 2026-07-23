# Custom Solver / GRChombo Comparison Batch 3 Results

Status: passed for the directly callable stock-`d=3` derivative kernels,
manufactured visible CCZ4 convergence, and callable visible algebraic cleanup.
Full Chombo periodic-domain/ghost exchange remains dependency-blocked. Hidden
`ww` cleanup and all modified-cartoon terms remain production adaptation gaps.

## Locked source and numerical paths

| Item | Inspected path and value | Evidence classification |
|---|---|---|
| Custom checkpoint | `571b142ab2bd8c14abb967eb259f2ca202ec9d22` | clean committed batch-2 checkpoint before this batch |
| GRChombo | clean detached `37e659523830418b210acea1661dac0e00bb1b75` | unchanged external checkout |
| Compiler | `g++ (Ubuntu 15.2.0-16ubuntu1) 15.2.0`; `-std=c++17 -O2 -Wall -Wextra -Wpedantic -Werror` | executable manifest |
| Dimensions | direct bridge `CH_SPACEDIM=GR_SPACEDIM=3`; custom production remains physical `d=4`, gridded `d=2`, hidden multiplicity two | exact compile-time/source lock |
| Custom first derivative | fixture radial `D_x` from `Stage4AOFrozenGaugeRadialBoundary.hpp::make_centered_derivative_jet`; compact `D_z` from `periodic_dz`: centered three-point, order two | direct custom helper actually executed |
| Custom second derivative | fixture radial `D_xx` from `Stage4AOFrozenGaugeRadialBoundary.hpp::make_centered_derivative_jet`; compact `D_zz` from `periodic_dzz`: centered three-point, order two | direct custom helper actually executed |
| Custom mixed derivative | actual custom centered radial derivative composed with actual custom periodic `D_z` | direct composed custom helper |
| GRChombo derivatives | `Source/BoxUtils/FourthOrderDerivatives.hpp`: five-point centered `diff1`, `diff2`, tensor-product `mixed_diff2`; fourth-order biased `advection_term` | direct compiled GRChombo kernels |
| Configured order | `params_stage2_smoke.txt`: `max_spatial_derivative_order=4` | source/configuration lock |
| Centering/spacing | cell-centered values, one uniform scalar `dx` in `FourthOrderDerivatives`; batch uses equal `x,z` spacing `2 pi/N` | direct kernel convention |
| Kernel ghost reach | two cells for centered first/second/mixed derivatives; three for biased advection | direct source |
| Periodic handling | custom helper performs modulo wrap; test-only GRChombo patch supplies periodic analytic ghost values | custom direct; GR kernel direct; GR domain fill not direct |
| Full Chombo grid path | no local Chombo installation or reproducible Chombo/container digest | blocked by named dependency |

The inspected smoke file comments the framework default as three ghost cells,
which is sufficient for the selected fourth-order advection reach. The
fixture does not claim to execute Chombo's domain periodicity or ghost fill:
it invokes the real scalar-pointer derivative kernels on an independently
populated local ghost patch.

## Manufactured profiles and acceptance

The fixture uses constants plus several sine/cosine modes and nonseparable
`x-z` modes for 22 visible fields. It supplies nonzero `chi,K,A,Theta,Gamma`,
lapse, and shift, a positive-definite metric with nonzero `h_xy,h_xz,h_yz`,
and `det(h)=1`. All fields are independent of visible `y`. The same analytic
functions, coordinates, spacings, and sample locations are used by both paths.

There is no same-stencil finite-grid comparison: the custom path is second
order and the selected GRChombo path is fourth order. Each is compared to the
analytic derivative and continuum RHS. The predeclared Level-2 tolerance is
retained for genuinely direct equal-quantity checks. The continuum
extrapolation rule is

```text
abs(a-b) <= 5e-11 + 5e-10*max(abs(a),abs(b)).
```

## Discrete derivative convergence

Maximum analytic errors over all 22 fields and four nontrivial periodic
locations:

| Derivative | N=32 custom / GR | N=64 custom / GR | N=128 custom / GR | N=256 custom / GR | Observed custom / GR order | Worst field/location |
|---|---:|---:|---:|---:|---:|---|
| `D_x` | `1.22163781e-4` / `2.53605432e-6` | `3.06612187e-5` / `1.60364608e-7` | `7.67284369e-6` / `1.00520178e-8` | `1.91868244e-6` / `6.28690704e-10` | `1.99752` / `3.99265` | field 11, `z=pi/4` |
| `D_z` | `1.71047186e-4` / `4.88885463e-6` | `4.29931437e-5` / `3.10161217e-7` | `1.07627941e-5` / `1.94609122e-8` | `2.69160605e-6` / `1.21747895e-9` | `1.99659` / `3.99046` | custom field 2, `z=pi/4`; GR field 11 |
| `D_xx` | `6.58833365e-5` / `1.42998472e-6` | `1.65385709e-5` / `9.03156409e-8` | `4.13888737e-6` / `5.65954396e-9` | `1.03498733e-6` / `3.53988022e-10` | `1.99741` / `3.99334` | field 5, `z=0` |
| `D_zz` | `1.79871701e-4` / `4.21060609e-6` | `4.51685532e-5` / `2.67503881e-7` | `1.13047293e-5` / `1.67879030e-8` | `2.82697056e-6` / `1.05071022e-9` | `1.99719` / `3.98948` | field 11, `z=3pi/4` |
| `D_xz` | `1.43640287e-4` / `3.01611651e-6` | `3.61557634e-5` / `1.91127747e-7` | `9.05437494e-6` / `1.19866410e-8` | `2.26455959e-6` / `7.49760999e-10` | `1.99570` / `3.99132` | custom field 19, `z=pi/2`; GR field 11, `z=3pi/4` |
| shift divergence | `1.78552102e-4` / `4.19049624e-6` | `4.48369719e-5` / `2.65261937e-7` | `1.12217168e-5` / `1.66317422e-8` | `2.80620943e-6` / `1.04031254e-9` | `1.99719` / `3.99196` | shift-divergence aggregate |

Every measured order lies within `0.25` of its documented order. Direct
custom-versus-GRChombo finite-grid equality is intentionally not required.
Derivative sign, spacing factor, mixed-direction, mixed-ordering-slot,
periodic-wrap, wrong-order, and one-path-only mutations all fail on active
data.

## Manufactured visible-RHS convergence

The analytic reference is the batch-2 same-dimension continuum family oracle.
The custom column uses its real order-two discrete helpers. The GRChombo
column uses the directly compiled order-four kernels and the directly compiled
`compute_ricci`, `compute_ricci_Z`, and `rhs_equation` paths.

| Family | N=32 custom / GR | N=64 custom / GR | N=128 custom / GR | N=256 custom / GR | Observed custom / GR order |
|---|---:|---:|---:|---:|---:|
| raw Ricci | `2.94858954e-4` / `4.52656496e-6` | `7.40377316e-5` / `2.85023801e-7` | `1.85296829e-5` / `1.78469287e-8` | `4.63368743e-6` / `1.11647794e-9` | `1.99724` / `3.99508` |
| encoded Z | `1.58802731e-4` / `2.72723180e-6` | `3.98950635e-5` / `1.72721157e-7` | `9.98596387e-6` / `1.08307891e-8` | `2.49725334e-6` / `6.77476112e-10` | `1.99692` / `3.99166` |
| lapse Hessian | `1.21470897e-4` / `2.49847014e-6` | `3.05902011e-5` / `1.58057576e-7` | `7.66152905e-6` / `9.90847969e-9` | `1.91625698e-6` / `6.19546588e-10` | `1.99539` / `3.99251` |
| advection, all 15 visible rows | `1.37151770e-5` / `5.59075787e-7` | `3.44506400e-6` / `3.44145649e-8` | `8.62285674e-7` / `2.10468244e-9` | `2.15635192e-7` / `1.29685323e-10` | `1.99701` / `4.02460` |
| shift-input derivative kernel diagnostic | `1.63569261e-4` / `4.79562942e-6` | `4.11201955e-5` / `3.03840304e-7` | `1.02943400e-5` / `1.90548346e-8` | `2.57447895e-6` / `1.19194197e-9` | `1.99649` / `3.99140` |
| `chi` shift RHS | `1.08246048e-4` / `2.54046103e-6` | `2.71821219e-5` / `1.60813320e-7` | `6.80309264e-6` / `1.00828853e-8` | `1.70124617e-6` / `6.30682702e-10` | `1.99719` / `3.99196` |
| `h_ij` combined shift RHS | `1.99997765e-4` / `5.31408410e-6` | `5.02518635e-5` / `3.36562933e-7` | `1.25787946e-5` / `2.11050245e-8` | `3.14568878e-6` / `1.32015737e-9` | `1.99682` / `3.99163` |
| `A_ij` combined shift RHS | `6.21291744e-6` / `1.76700390e-7` | `1.56162493e-6` / `1.11940943e-8` | `3.90932733e-7` / `7.01999430e-10` | `9.77661172e-8` / `4.39120364e-11` | `1.99660` / `3.99147` |
| combined visible rows | `2.10142597e-4` / `4.87082944e-6` | `5.28006631e-5` / `3.09571160e-7` | `1.32167841e-5` / `1.94636711e-8` | `3.30523554e-6` / `1.21934932e-9` | `1.99682` / `3.98795` |

The raw `d1.shift` line is an input-kernel convergence diagnostic, not an
isolated RHS-family result. For the three following RHS families, GRChombo
evidence comes from subtracting two direct calls to `rhs_equation` with
identical discrete inputs except that the second has all shift derivatives
zero. This exposes the complete combined family without claiming direct
access to its internal stretching and trace-correction subterms. The custom
side independently evaluates the equivalent combined family.

### Expanded advection convergence

Each observation retains its family, row, refinement, analytic and discrete
values, error, and physical `z`. Entries below are custom / GRChombo maximum
errors over the four sampled locations.

| Row | N=32 | N=64 | N=128 | N=256 | Order custom / GR | Worst `z` custom / GR |
|---|---:|---:|---:|---:|---:|---|
| `chi` | `4.01017683e-6` / `1.11259854e-7` | `1.00728895e-6` / `8.37127361e-9` | `2.52119749e-7` / `5.61758161e-10` | `6.30485469e-8` / `3.62079599e-11` | `1.99702` / `3.86178` | `pi/4` / `pi/4` |
| `K` | `8.65258416e-6` / `2.57859274e-7` | `2.17342447e-6` / `1.86770762e-8` | `5.44000612e-7` / `1.23401514e-9` | `1.36040467e-7` / `7.89720449e-11` | `1.99701` / `3.89098` | `pi/4` / `pi/4` |
| `Theta` | `1.20209323e-5` / `4.24874811e-7` | `3.01892317e-6` / `2.74879531e-8` | `7.55589174e-7` / `1.72565905e-9` | `1.88950985e-7` / `1.07748277e-10` | `1.99713` / `3.98172` | `pi/4` / `pi/4` |
| `hxx` | `9.09350999e-6` / `3.37768711e-7` | `2.28437197e-6` / `2.19759947e-8` | `5.71782399e-7` / `1.38293736e-9` | `1.42988722e-7` / `8.64467314e-11` | `1.99695` / `3.97731` | `pi/2` / `pi/2` |
| `hyy` | `1.37151770e-5` / `5.59075787e-7` | `3.44506400e-6` / `3.44145649e-8` | `8.62285674e-7` / `2.10468244e-9` | `2.15635192e-7` / `1.29685323e-10` | `1.99701` / `4.02460` | `pi/2` / `pi/2` |
| `hzz` | `5.16050252e-6` / `2.17617197e-7` | `1.29644720e-6` / `1.33916233e-8` | `3.24508199e-7` / `8.18987774e-10` | `8.11518449e-8` / `5.04518767e-11` | `1.99691` / `4.02487` | `pi/2` / `pi/2` |
| `hxy` | `9.04979417e-6` / `4.17524081e-7` | `2.27393855e-6` / `2.50517097e-8` | `5.69205151e-7` / `1.51106552e-9` | `1.42346357e-7` / `9.24067722e-11` | `1.99680` / `4.04719` | `pi/2` / `pi/2` |
| `hxz` | `1.01448315e-5` / `4.43254660e-7` | `2.54879962e-6` / `2.69917724e-8` | `6.37989489e-7` / `1.64135337e-9` | `1.59546762e-7` / `1.00808057e-10` | `1.99687` / `4.03410` | `pi/2` / `pi/2` |
| `hyz` | `5.21841401e-6` / `2.58032488e-7` | `1.31046123e-6` / `1.42167678e-8` | `3.27982587e-7` / `8.15665308e-10` | `8.20186201e-8` / `4.85225691e-11` | `1.99717` / `4.12554` | `pi/2` / `pi/2` |
| `Axx` | `4.92295145e-6` / `2.78074293e-7` | `1.23745919e-6` / `1.57484152e-8` | `3.09786307e-7` / `9.18477227e-10` | `7.74729433e-8` / `5.51404810e-11` | `1.99656` / `4.10002` | `pi/2` / `pi/2` |
| `Ayy` | `6.22206585e-6` / `3.98499918e-7` | `1.56399679e-6` / `2.14034225e-8` | `3.91531016e-7` / `1.20712193e-9` | `9.79160208e-8` / `7.10822646e-11` | `1.99657` / `4.15093` | `pi/2` / `pi/2` |
| `Azz` | `3.95386589e-6` / `1.20567451e-7` | `9.90752908e-7` / `6.02828017e-9` | `2.47831437e-7` / `3.23198335e-10` | `6.19668146e-8` / `1.84468520e-11` | `1.99854` / `4.22473` | `0` / `pi/2` |
| `Axy` | `5.54299820e-6` / `1.92289246e-7` | `1.39001179e-6` / `8.50478290e-9` | `3.47770054e-7` / `4.71665083e-10` | `8.69592188e-8` / `3.14751970e-11` | `1.99806` / `4.19226` | custom `0`; GR `pi/2` at coarse level and `0` thereafter |
| `Axz` | `6.21323653e-6` / `2.32530229e-7` | `1.56265210e-6` / `1.58295849e-8` | `3.91248993e-7` / `1.08609429e-9` | `9.78489029e-8` / `7.06857540e-11` | `1.99622` / `3.89457` | custom `pi/4`; GR `pi/2` at coarse level and `pi/4` thereafter |
| `Ayz` | `4.56143990e-6` / `1.64111966e-7` | `1.14609804e-6` / `1.10819404e-8` | `2.86884327e-7` / `7.09633724e-10` | `7.17435891e-8` / `4.47387940e-11` | `1.99683` / `3.94696` | `0` / `0` |

All 15 rowwise orders pass their declared `0.25` windows. The aggregate
worst is `hyy` at `z=pi/2`. Metric and `A_ij` omission, duplication, and sign
mutations are rejected on active data independently of the combined-row gate.

### Shift RHS-family convergence

| `chi` shift family | N=32 | N=64 | N=128 | N=256 | Order custom / GR | Worst component/location |
|---|---:|---:|---:|---:|---:|---|
| divergence contribution | `1.08246048e-4` / `2.54046103e-6` | `2.71821219e-5` / `1.60813320e-7` | `6.80309264e-6` / `1.00828853e-8` | `1.70124617e-6` / `6.30682702e-10` | `1.99719` / `3.99196` | `chi`, `z=pi/2` |

| `h_ij` combined shift family | N=32 | N=64 | N=128 | N=256 | Order custom / GR | Worst component/location |
|---|---:|---:|---:|---:|---:|---|
| all six components | `1.99997765e-4` / `5.31408410e-6` | `5.02518635e-5` / `3.36562933e-7` | `1.25787946e-5` / `2.11050245e-8` | `3.14568878e-6` / `1.32015737e-9` | `1.99682` / `3.99163` | `hxz`, `z=pi/2` |

| `A_ij` combined shift family | N=32 | N=64 | N=128 | N=256 | Order custom / GR | Worst component/location |
|---|---:|---:|---:|---:|---:|---|
| all six components | `6.21291744e-6` / `1.76700390e-7` | `1.56162493e-6` / `1.11940943e-8` | `3.90932733e-7` / `7.01999430e-10` | `9.77661172e-8` / `4.39120364e-11` | `1.99660` / `3.99147` | `Axz`, `z=pi/2` |

Every individual tensor component passes. Custom component orders span
`1.99648-1.99770` for `h` and `1.99660-1.99794` for `A`; GRChombo spans
`3.99139-3.99259` for `h` and `3.99147-3.99259` for `A`. The executable
records analytic and discrete values, errors, components, refinements, and
physical locations for every component. Omission, duplication, coefficient,
and sign mutations are rejected independently for each nonzero shift group.

The worst combined finite-grid difference is the visible `h_xz` row (row 7):
`2.05271767e-4`, `5.24910919e-5`, `1.31973205e-5`, and
`3.30401619e-6` over the four refinements. It is truncation error, not a
continuum mismatch. Four-level Richardson extrapolation gives:

| Quantity | Maximum error | Worst row |
|---|---:|---|
| custom versus GRChombo extrapolated rows | `4.91998803e-13` | `Azz` (row 11) |
| custom extrapolation versus analytic rows | `1.00397295e-12` | reported by fixture |
| GRChombo extrapolation versus analytic rows | `8.70747918e-13` | reported by fixture |

The two extrapolated implementations satisfy the predeclared continuum
agreement tolerance for every visible `chi,h,K,Theta,A` component. Families
are tracked independently, so the combined result cannot conceal Ricci,
encoded-Z, Hessian, any visible advection row, or any complete shift
RHS-family error. Raw shift derivatives remain separately labeled input
diagnostics.

The recomputed Richardson values (shown to the fixture's printed precision)
are:

| Row | Custom | GRChombo | Analytic |
|---|---:|---:|---:|
| `chi` | `2.15631088e-1` | `2.15631088e-1` | `2.15631088e-1` |
| `K` | `1.85079991e-1` | `1.85079991e-1` | `1.85079991e-1` |
| `Theta` | `8.05988685e-2` | `8.05988685e-2` | `8.05988685e-2` |
| `hxx` | `-9.90123477e-2` | `-9.90123477e-2` | `-9.90123477e-2` |
| `hyy` | `8.01581442e-2` | `8.01581442e-2` | `8.01581442e-2` |
| `hzz` | `-2.79552433e-2` | `-2.79552433e-2` | `-2.79552433e-2` |
| `hxy` | `-2.27754363e-2` | `-2.27754363e-2` | `-2.27754363e-2` |
| `hxz` | `1.12616200e-2` | `1.12616200e-2` | `1.12616200e-2` |
| `hyz` | `-2.44543909e-2` | `-2.44543909e-2` | `-2.44543909e-2` |
| `Axx` | `1.23893398e-2` | `1.23893398e-2` | `1.23893398e-2` |
| `Ayy` | `-1.29533471e-2` | `-1.29533471e-2` | `-1.29533471e-2` |
| `Azz` | `3.30091544e-3` | `3.30091544e-3` | `3.30091544e-3` |
| `Axy` | `1.63492102e-4` | `1.63492102e-4` | `1.63492102e-4` |
| `Axz` | `-1.54682780e-2` | `-1.54682780e-2` | `-1.54682780e-2` |
| `Ayz` | `3.67075652e-3` | `3.67075652e-3` | `3.67075652e-3` |

## Visible algebraic cleanup

`Stage4AOCGRChomboComparisonBatch3CleanupTest.cpp` invokes the actual
`TraceARemoval::compute` and `PositiveChiAndAlpha::compute` implementations
through an isolated test-only Cell. The independent side is a local
same-dimension matrix oracle, not a second call to GRChombo.

| Check | Result | Classification |
|---|---:|---|
| trace-removal component maximum | `2.77555756e-17` | direct compiled GRChombo |
| maximum normalized component error | `3.65454015e-05` | direct compiled GRChombo; Level-2 pass |
| post-cleanup visible trace | `5.55111512e-17` | direct compiled GRChombo |
| idempotence maximum | `1.38777878e-17` | direct compiled GRChombo |
| determinant before/after trace cleanup | `1.36136500` / `1.36136500` | proves trace cleanup does not normalize determinant |
| positivity result | `chi=2.5e-4`, `lapse=3.5e-4` | direct compiled GRChombo |
| positivity/trace order | identical in either order | direct; operations own disjoint fields and commute |
| determinant normalization | no runtime cleanup routine found in inspected stock path | source-only/absent |
| hidden-aware `d=4` trace/determinant cleanup | no stock `hww/Aww` owner | adaptation gap |

Wrong trace dimension, omitted trace cleanup, injected hidden multiplicity,
omitted positivity cleanup, and locally diagnosed wrong/omitted determinant
normalization are detected. The determinant-exponent controls are explicitly
source/design-only because no inspected stock determinant-normalization output
exists. Reversing the two callable stock cleanup operations is not a valid
negative control: trace removal and positivity enforcement touch disjoint
fields and correctly commute.

## Directness and blockers

- **Direct compiled GRChombo:** fourth-order scalar derivative and mixed
  kernels, biased advection kernel, `compute_ricci`, `compute_ricci_Z`,
  visible `rhs_equation`, `TraceARemoval`, and `PositiveChiAndAlpha`.
- **Different-order convergence comparison:** all custom-versus-GRChombo
  finite-grid derivative and RHS values.
- **Source-only/absent:** stock determinant normalization and the standalone
  Chombo periodic-domain ownership path.
- **Blocked by dependency:** full Chombo grid construction, periodic ghost
  exchange, and production BoxLoop cleanup execution; no local Chombo install
  or reproducible Chombo/container digest exists.
- **Custom-hidden-only:** `hww/Aww`, multiplicity-two traces/determinants,
  hidden derivatives, and modified-cartoon RHS terms.

## Scope and disposition

The visible fourth-order derivative kernels and visible cleanup routines
should be reused from GRChombo. Periodic `z`, ghost ownership, AMR, and
advection must also remain framework-owned and be tested once a reproducible
Chombo build exists. The custom order-two stencils remain focused oracle
regressions, not production infrastructure. Hidden-aware trace/determinant
cleanup and modified-cartoon derivatives require adaptation.

No production equation, external GRChombo source, smoke parameter, boundary
condition, eigensolver, MOTS path, staging state, or commit was changed. No
final overall agent score is assigned.
