# Stage 3J Unit-Test Fixture Design

Status: design only. No C++ tests, source terms, finite-difference
implementation, or production tolerances are introduced here.

Stage 3J defines the fixture layer that should exist before project-specific
modified-cartoon CCZ4 source terms are implemented. The purpose is to keep the
future implementation from becoming one large RHS block with no local
diagnostics. The tests below separate exact symbolic gates, C++ algebra unit
tests, RHS-block tests, reference-code comparisons, and convergence or
evolution-level checks.

## Conventions

- Physical spatial dimension: `GR_SPACEDIM = 4`.
- Chombo/grid dimension: `CH_SPACEDIM = 2`.
- Gridded directions: `x,z`.
- Hidden directions: two equivalent `w` directions.
- Hidden multiplicity: `N_hidden = GR_SPACEDIM - CH_SPACEDIM = 2`.
- `hww` is a Cartesian-like hidden conformal metric component, not
  `g_theta theta`.
- The spherical/cartoon reconstruction supplies the external `x^2` factor.
- Full traces and trace-free projections are 4D, not 2D.
- The conformal trace denominator is `/4`.
- The evolved connection-sector blueprint is `hat_Gamma^A`, with `Z^A`
  encoded through `hat_Gamma^A = tilde_Gamma^A + 2 Z^A`.

The implementation convention target is GRChombo compatibility plus internal
consistency. External implementations may be used later for validation, but
they are not the source of truth for variable naming, evolved-variable basis,
trace convention, or cartoon-extension bookkeeping. Cartoon-specific
extensions must be documented explicitly when public GRChombo does not already
define them.

## Test-Type Legend

- Symbolic: a Python/SymPy derivation or regression gate.
- C++ unit: a small future C++ test for an isolated algebra/source block.
- RHS-block: a future test of a decomposed CCZ4 RHS term family.
- Evolution: a controlled short evolution or stationary-data run.
- Reference-comparison: later validation against an external implementation,
  either term-by-term or trajectory-level, after the GRChombo-facing
  conventions are fixed.
- Convergence: resolution or self-convergence evidence.

## Tolerance Policy

Stage 3J classifies tests as exact, roundoff-level, tolerance-based,
reference-based, or convergence-based. Numeric tolerances are intentionally not
chosen here. When C++ tests are written, each tolerance must be justified from
the truncation order, grid resolution, floating-point roundoff,
interpolation/ghost-zone treatment, and reference-data uncertainty relevant to
that fixture. Tolerances must not be arbitrary "whatever passes" thresholds.
Exact symbolic identities should remain exact in the symbolic scripts.

## Existing Symbolic Gates

These gates already exist in `docs/derivations/` and should remain part of the
pre-C++ checklist. They are not C++ tests yet, but they define exact expected
values and fixture data that later tests should reuse.

| Test name | Stage source | Type | Input data | Expected output | Exactness | Catches | Does not catch | Required before Stage 3K/C++? | Convention / validation note |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Flat cartoon geometry and GP trace | 3C | Symbolic | Flat GP spatial slice in `(x,theta,phi,z)` with natural GP shift | Spherical Christoffels, `K_ww = beta^x/x`, and `K = K^x_x + K^z_z + 2 K^w_w` | Exact identities | Wrong angular contraction, missing hidden multiplicity, GP trace mistakes | Full CCZ4 RHS, nontrivial `hww`, small-axis finite differencing | Yes | Overall `K_ij` sign convention still requires implementation confirmation |
| Diagonal nontrivial `gamma_ww=q` Ricci structure | 3D | Symbolic | Diagonal metric with `a(x,z)`, `c(x,z)`, `q(x,z)` | Hidden Christoffels, angular Ricci equality, flat Ricci limit, constant-`q0` cone regression | Exact identities | Wrong angular `sin^2(theta)` handling, hidden Ricci multiplicity, constant hidden-sector curvature mistakes | Nonconstant derivative terms in the abstract printed expression are only partly guarded | Yes | No for geometry engine; yes before mapping signs into GRChombo code |
| Nonconstant `q(x,z)` derivative-sector Ricci | 3E | Symbolic | `f=x(1+lambda x+mu z)`, `q=(1+lambda x+mu z)^2` | Direct Ricci equals warped-product reference formulas; active `q_x`, `q_z`, `q_xz` | Exact symbolic and substitution identities | Missing hidden derivative terms, wrong warped-product angular Ricci, stale Ricci engine behavior | `R_zz` with nonzero `f_zz`, off-diagonal base metric | Yes | No for symbolic geometry; yes before C++ layout and regularization choices |
| Diagonal conformal-cartoon algebra | 3F | Symbolic | Diagonal `gamma_xx`, `gamma_zz`, `gamma_ww`, `K_xx`, `K_zz`, `K_ww` | Determinant normalization, `q=hww/chi` identities, radius derivatives, `K_ij` round trip, 4D tracelessness, `/4` guard | Exact identities | Treating `hww` as `g_theta theta`, omitting hidden `hww` from determinant/trace, using `/2` or `/3` | Off-diagonal inverse/tracelessness; Stage 3F is diagonal-only | Yes | Document the GRChombo-facing determinant and trace-free enforcement policy |
| Off-diagonal conformal-cartoon algebra | 3G | Symbolic | Nonzero `h_xz`, `gamma_xz`, `A_xz`, `K_xz` | Block determinant/inverse, normalized `det h_4D=1`, `K_xz` reconstruction, full 4D tracelessness, diagonal limit to 3F | Exact identities | Reciprocal inverse mistakes, missing `2 h^xz A_xz`, wrong off-diagonal determinant, wrong `/4` denominator | Full Ricci/source terms and finite-difference behavior | Yes | Confirm implementation storage/order and enforcement policy |
| Sheared-flat off-diagonal Ricci gate | 3G | Symbolic | Flat pullback metrics with constant `g_xz=lambda` and `g_xz=x` | Ricci tensor and scalar vanish exactly | Exact zero | Ricci engine using diagonal-only inverse assumptions, off-diagonal Christoffel wiring mistakes | Non-flat off-diagonal Ricci source formulas, CCZ4 RHS couplings | Yes | No for the local geometry gate; yes for C++ source-term placement |
| Small-`x` regularity and `hat_Gamma^x` guard | 3I | Symbolic | Taylor data with even diagonal fields, odd off-diagonal fields, matching `h_xx-hww=O(x^2)`, and regular `Z^A` parity | Removable limits finite; violated matching exposes `1/x`; `hat_Gamma^x=tilde_Gamma^x+2Z^x` finite under documented convention | Exact limits and divergence checks | Missing axis matching, hidden multiplicity mistakes in the assembled connection, generic parity tests that miss residual `1/x` terms | Ghost-cell filling, finite-difference stencils, final sign convention | Yes | Derive and document the sign in the GRChombo-facing cartoon convention |

## Future C++ Unit-Test Candidates

These are not implemented in Stage 3J. They are the first fixtures that should
be created when the project-specific C++ layer begins. Each test should use a
small analytic fixture and compare only one algebra/source block at a time.

Initial Stage 4A coverage now lives in
`code/BlackStringToy/tests/Stage4AConformalCartoonAlgebraTest.cpp`, using
`code/BlackStringToy/ConformalCartoonAlgebra.hpp`. This fixture is local-value
only: it does not read grid variables, enum slots, `Cell`, `Vars`,
`FArrayBox`, or evolution data. The Stage 4A review follow-up adds
dimension-aware `chi` power bookkeeping, tolerance-based floating-point guards,
and independent hard-coded `K_xz`/`K_ww` reconstruction oracle values.

Initial Stage 4B coverage now lives in
`code/BlackStringToy/tests/Stage4BVariableLayoutTest.cpp`. This fixture checks
the public CCZ4 component order retained as the baseline for comparison and
records the visible helper-input map without editing the enum or reading grid
data. It is a public-layout drift guard only; the real repo-owned hidden
placement check belongs to Stage 4C.

Stage 4C coverage now lives in
`code/BlackStringToy/tests/Stage4CVariablePlacementTest.cpp`, with the real
placement assertions also compiled from `code/BlackStringToy/UserVariables.hpp`.
This confirms that `c_hww` and `c_Aww` exist, that `c_hww == c_K - 1` and
`c_Aww == c_Theta - 1`, and that the Stage 4A helper input map now has real
component slots for `hww` and `Aww`. It still does not read grid data or prove
that a future handoff passes those components correctly.

The Stage 4C scratch Docker/GRChombo build also compiles the full
`BlackStringToy` scaffold with the 27-variable layout. The inherited cheap
smoke run reaches runtime but fails because `hww` and `Aww` are not initialized
or handed off yet. Stage 4D should address only that finite scaffold-value
failure; a Stage 4D smoke pass would still not validate cartoon Ricci terms,
full CCZ4 RHS terms, or physical black-string evolution.

Stage 4D should keep that finite scaffold support explicitly smoke-only. The
temporary freeze should be controlled by a default-off parameter such as
`scaffold_freeze_hidden`, enabled only by the cheap smoke parameter file. A
passing Stage 4D smoke run then shows only that the inherited scaffold no
longer dies immediately from hidden-variable NaNs; it must not be interpreted
as a hidden-sector RHS, cartoon Ricci, or physical-evolution test.

Stage 4E adds the first grid-to-helper handoff diagnostic. The default-off
`scaffold_check_hidden_handoff` parameter enables a read-only path that loads
the actual scaffold grid slots for `chi`, `h11`, `h12`, `h22`, `hww`, `A11`,
`A12`, `A22`, `Aww`, and `K`, packages them into the Stage 4A local algebra
helper input structs, and checks finite determinant, inverse, full-4D trace,
and `K_ij` reconstruction outputs. This is a fixture-style handoff check only:
it does not write helper results back to the grid, implement RHS terms, or show
that cartoon evolution is physically meaningful.

The smoke data is intentionally cheap and can be symmetric enough that swapped
slots remain finite. The companion
`code/BlackStringToy/tests/Stage4EGridToHelperMappingTest.cpp` fixture uses
distinct local values for each relevant component and checks the exact helper
input map plus an independent `K_ww` oracle, so it can catch swaps that the
runtime finiteness check would miss.

Stage 4F adds only the future cartoon Ricci helper interface. The header
`code/BlackStringToy/CartoonRicciInterface.hpp` defines the
metric-derivative Ricci contract used by the Stage 3C-3E symbolic checks:
local-value input structures for `x`, `chi`, `h_xx`, `h_xz`, `h_zz`, `hww`,
first derivatives, and second derivatives, plus an output structure for
`R_xx`, `R_xz`, `R_zz`, and `R_ww`. This is not the Gamma-based GRChombo
`CCZ4Geometry` Ricci form. Before RHS wiring, a later stage must confirm how
this metric-derivative output is consumed by the GRChombo-facing RHS path and
resolve any mismatch. The companion interface fixture checks that these types
compile and that hidden multiplicity is two. It does not compute Ricci values,
and the future implementation must still apply the Stage 3I small-`x`
regularity rules.

Stage 4G implements the first local metric-derivative cartoon Ricci helper
against that interface. The fixture
`code/BlackStringToy/tests/Stage4GCartoonRicciMetricDerivativeTest.cpp` checks
flat zero Ricci data, the Stage 3D constant-`q` cone oracle, a Stage 3E
nonconstant-`q` warped-product oracle, the Stage 3G `x`-shear flat gate, and an
away-axis guard that rejects `x = 0`. It remains local-value only and does not
read grid data or wire outputs into the RHS.

Stage 4H is a compatibility-planning gate, not a unit test. It records that the
current inherited GRChombo CCZ4 RHS path computes Ricci through
`CCZ4Geometry::compute_ricci_Z`, which uses the evolved hatted `Gamma` variable
and `Z_over_chi`. The Stage 4G helper is metric-derivative based, so future RHS
tests must not silently mix those two Ricci conventions.

Stage 4I adds a typed Ricci bridge contract. `CartoonRicci::RicciComponents`
remains a distinct input type, and future RHS-facing code must go through
`CartoonRicciBridge.hpp` or a reviewed replacement rather than consuming
Stage 4G output directly. The bridge pins physical lower/lower Ricci
components, full 4D trace factors, hidden multiplicity, and `R_ww` as the
Cartesian-like hidden component. After the review follow-up, the raw cartoon
Ricci component storage is private, and the bridge exposes a distinct
`RhsRicciComponents` view. Ordinary code can no longer use `ricci.xx`-style
field access; that access path is intentionally restricted so the hidden
`R_ww` and off-diagonal multiplicities cannot be bypassed accidentally.

Stage 4J adds the local Ricci-to-RHS contract. It accepts the Stage 4I
bridge-approved `RhsRicciComponents` view and local algebra inputs, then
computes only named Ricci contractions for future RHS blocks. It intentionally
does not accept raw cartoon Ricci, implement RHS formulas, read grid data, or
wire anything into evolution.

The Stage 4J review follow-up keeps the sanctioned bridge path easy to use but
makes the internal `RicciAccess` doorway private. The bridge output remains a
typed by-value view, not a raw pointer, reference, mutable alias, or loose list
of doubles.

Stage 4K adds a local RHS source-block skeleton. It consumes the Stage 4J
contract type and exposes named future source output fields, but only through
an explicit inert contract-test path. It does not implement physical RHS
formulas or evolution wiring.

Stage 4L implements the first local formula block: the full-4D trace-free
projection of bridge-approved Ricci. The source block delegates the projection
arithmetic to the shared conformal-cartoon algebra helper and rejects
nonpositive `x` through the existing away-axis contract validation. It is still
local only and does not wire the projected pieces into evolution or claim a
full CCZ4 RHS.

Stage 4M adds the explicit away-axis policy fixture. The policy is the named
local owner of the finite `x > 0` decision and of guarded `1/x` and `1/x^2`
helpers for future source terms. It rejects `x = 0`, negative `x`, NaN, and
infinity, and the Stage 4L source-block boundary still rejects `x = 0` through
the policy-backed Stage 4J contract. This is not Stage 3I small-axis
regularization and does not make axis evaluation physically valid.

Stage 4N adds guarded away-axis singular-combination helpers. These local
helpers form `d_x f / x` and `(f - g) / x^2` through the Stage 4M policy path,
reject nonfinite inputs, and keep future source blocks from writing raw
division by `x`. This is still not Stage 3I small-axis regularization and does
not add any physical RHS source term.

Stage 4O locks in the axis-regime semantics used by those helpers before real
source terms are added. `AwayAxisOnly` is the only implemented regime; finite
positive `x` passes, while zero, negative, NaN, and infinite `x` reject. No
clamp, epsilon substitution, or regularized-axis support is implemented. The
`1/x^2` helper is kept as a separately guarded primitive so future
regularized or clamped behavior must define `1/x` and `1/x^2` semantics
independently.

Stage 4P adds the first named away-axis cartoon geometry primitive. The public
primitive block forms `(d_x hww) / x` through the Stage 4N
singular-combination helpers and Stage 4O away-axis semantics. After the
Stage 4R review patch, raw Stage 4P no longer exposes `(hxx - hww) / x^2` as
an ordinary public output because that value is regularity-sensitive. It is not
a full Ricci tensor, not a CCZ4 RHS source term, and not Stage 3I small-axis
regularization.

Stage 4Q adds the first local version of that matching guard. It checks finite
local values, uses the Stage 4O away-axis policy for `x`, and rejects clear
pointwise violations of `hxx - hww = O(x^2)`. This guard is useful before
near-axis use of the Stage 4P primitive, but it is not a proof of full
analytic regularity and does not implement Stage 3I finite-limit formulas.

Stage 4R adds the first regularity-guarded local source-style sub-block. It
calls the Stage 4Q matching guard before returning the Stage 4P low-risk
primitive, the source-facing `(hxx - hww) / x^2`, and the matching residual.
This is now the normal source-facing path for the risky cartoon geometry
ingredient, but it is still not full Ricci, not full CCZ4 RHS, and not Stage
3I small-axis regularization.

Stage 4S carries that Stage 4R package through the local RHS source-block
skeleton. The source-block fixture checks that a matching sample passes,
obvious mismatch and invalid inputs reject, and the carried values agree with
the Stage 4R guarded helper. This is a source-block integration boundary only:
it does not add a physical RHS term, grid reads, finite differences, or
evolution wiring.

After the Stage 4S review patch, both the Stage 4R guarded package and the
Stage 4S carry output are checked-by-construction. The fixtures include
compile-time type-shape checks showing the trusted package and carry output
are not open aggregates that can be brace-initialized with arbitrary doubles.

Stage 4T adds the first local consumer of the checked package as a diagnostic
probe, not as a physics formula. It accepts the Stage 4R non-forgeable package,
not raw doubles, and checks that a clear mismatch fails before the consumer can
be called.

Stage 4U adds the source-formula authoring gate. Future source formulas that
need `(hxx - hww) / x^2` must take the checked package or the Stage 4U
authoring-gate input. Convention-only: direct use of generic
`difference_over_x2(h_xx, h_ww, x)` for the regularity-sensitive metric
difference is not mechanically blocked by the current C++ type system. Stage
4U provides the required typed source-formula entry point and documentation
rule, but future formula reviews must enforce that formulas consume
`RegularityGuardedGeometrySources` instead of recomputing this value.

Stage 4V is an intentional no-formula gate. It checks whether the first real
local source-formula consumer of the Stage 4U input can be traced to an
already validated expression. The answer is currently no: Stage 4G validates a
complete metric-derivative Ricci helper, and Stage 4L validates trace-free
projection after Ricci components are already known, but neither identifies a
standalone source sub-expression whose only regularity-sensitive geometry
input is the Stage 4U package. The next fixture needs an extracted derivation
and hard-coded oracle before code is added.

Stage 4W locks the first hidden-sphere Ricci target in a derivation note. It
maps the hidden CCZ4 contributions and identifies the physical `R_ww[gamma]`
as the first serious local Ricci/source target. After review, the note
separates the conformal warped-product component `tilde{R}_ww[h]`, the
`d = 4` conformal-factor correction `R^chi_ww`, and the physical input
`R_ww[gamma]`. It records the Stage 4G-compatible flat, constant-cone, and
nonconstant-hidden-metric oracles plus a future varying-`chi` consistency
oracle. It also documents that the `R_ww` gradient expansion exposes a second
regularity-sensitive path, `h^xz h_xz / x^2`. The existing Stage 4Q/4R/4U
guard covers `hxx - hww = O(x^2)`, but there is no checked local `h_xz / x`
ingredient yet. Therefore Stage 4W adds no formula code, and the next coding
fixture should provide that checked quotient before any `R_ww` formula stage.

Stage 4X adds that source-facing checked quotient as a local `h_xz / x`
package in `CartoonCheckedHxzOverX.hpp`, with fixture coverage in
`Stage4XCheckedHxzOverXIngredientTest.cpp`. The fixture allows finite nonzero
quotients, rejects invalid axis and nonfinite inputs, checks that the trusted
package is not an open aggregate, and verifies a dummy future consumer accepts
the checked package rather than raw doubles. It does not prove global parity or
the analytic statement `h_xz = O(x)`, and it does not implement
`tilde{R}_ww[h]`, `R^chi_ww`, physical `R_ww[gamma]`, CCZ4 RHS, finite-axis
regularization, grid reads, or evolution wiring.

Stage 4Y adds the first formula-bearing sub-block from this derivation without
assembling full conformal Ricci. `CartoonConformalRwwSingularBlock.hpp`
computes `G_sing = (h_zz / D) Delta_xw - q_xz^2 / D` from the Stage 4U checked
`Delta_xw` path and the Stage 4X checked `q_xz` package. After review, the
formula accepts only a single-source `ConformalRwwSingularBlockInputs` package
minted from one local metric point, so the checked singular ingredients and
raw determinant data cannot be mixed across points. The fixture covers the
flat, constant-cone, and distinct-value oracles, the non-aggregate input
package boundary, pre-formula ingredient rejection, and determinant rejection.

The integrated Stage 4 hidden-sphere roadmap lives in
`docs/implementation/stage4_hidden_sphere_Rww_plan.md`. It turns the remaining
test ownership into concrete future gates. Stage 4Z now covers checked
`W_x/x` and the first-derivative conformal gradient block; Stage 4AB covers
the local Hessian sub-block. The remaining gates start with conformal
`tilde R_ww` assembly in Stage 4AC, then `R^chi_ww` guard stack,
split-vs-direct physical Ricci identity with varying `chi`, true `h_xz`
parity validation, and `hat_Gamma^x` hidden-contraction tests anchored by a
GL-growth/dispersion check rather than a flat-only oracle.

Stage 4AA is a documentation-only fixture-design gate for the Hessian block.
It locks

```text
G^Hess_ww =
    -(sqrt(W) / x) [
        (C / D) H_xx
      - (2 B / D) H_xz
      + (A / D) H_zz
    ],
```

with `H_ab = rho_ab - Gamma^x_ab rho_x - Gamma^z_ab rho_z`, plus the
`rho` derivatives and reduced-base Christoffels that Stage 4AB should
implement. The locked Hessian oracle values are flat `0`, constant cone `0`,
and nonconstant `W = (1 + x)^2` at `x = 1` giving `G^Hess_ww = -4`; together
with the Stage 4Y/4Z values, that fixture gives `tilde R_ww = -12`. A
distinct nonsymmetric sample with all derivative slots nonzero is recorded in
the roadmap. Claude Audit A verifies that sample with
`G^Hess_ww = -8558 / 2883`, approximately `-2.9684356573014221`, and the full
conformal sum `G^sing_ww + G^grad_ww + G^Hess_ww = -3576 / 961`,
approximately `-3.7211238293`, matching the independent Stage 4G conformal
Ricci engine to machine precision with residual about `4.44e-16`. This
nonsymmetric oracle is required for Stage 4AB because it exercises the
off-diagonal Christoffels, `rho_xz`, `W_z`-dependent terms, and the
`(-2B/D)` contraction; the simpler oracles are not sufficient by themselves.

Stage 4AB implements that Hessian block in
`CartoonConformalRwwHessianBlock.hpp`, with fixture coverage in
`Stage4ABConformalRwwHessianBlockTest.cpp`. The fixture uses a single-source
non-aggregate input package for `x`, reduced metric values, reduced-base first
derivatives, and `W` first/second derivatives. It checks the flat,
constant-cone, nonconstant-`W`, and verified nonsymmetric Hessian oracles,
plus a test-only conformal sum with Stage 4Y/4Z. It still does not add full
`tilde R_ww`, `R^chi_ww`, physical Ricci, RHS, grid reads, or evolution.

Stage 4AC assembles the conformal target in
`CartoonConformalRww.hpp`, with fixture coverage in
`Stage4ACConformalRwwAssemblyTest.cpp`. The fixture uses a single-source
non-aggregate input package that internally mints the Stage 4Y singular,
Stage 4Z gradient, and Stage 4AB Hessian inputs from one local metric and
derivative point. It checks flat `0`, constant-cone `-3/4`, nonconstant
`W=(1+x)^2` `-12`, and verified nonsymmetric `-3576/961` oracles. For the
nonconstant and nonsymmetric `chi=1` samples it also compares the assembled
conformal value against the independent Stage 4G metric-derivative Ricci
helper. It is still conformal `tilde R_ww[h]` only and does not add
`R^chi_ww`, physical `R_ww[gamma]`, RHS, grid reads, or evolution.
Checkpoint B unifies the reduced determinant policy: the Stage 4Y singular
block now rejects `D <= 0`, matching the Stage 4Z, Stage 4AB, and Stage 4AC
positive determinant requirement.

Stage 4AD is a documentation-only derivation and fixture-design gate for
`R^chi_ww`. It locks the `D_wD_w chi` formula, the full conformal Laplacian
with hidden multiplicity `(2/W)D_wD_w chi`, the reduced scalar Hessian pieces,
and the conformal gradient norm. It also records the Stage 4AE guard-stack
requirements: a new checked local `chi_x/x` ingredient, existing checked
`q_xz` and `p_W`, single-source metric/`chi` inputs, away-axis-only `x`,
positive `chi`, positive `W`, positive reduced determinant, finite derivatives,
and no global parity or finite-axis regularity proof.

| Test name | Stage source | Type | Input data | Expected output | Exactness | Catches | Does not catch | Required before Stage 3K/C++? | Convention / validation note |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Conformal determinant enforcement | 3F/3G | C++ unit | Positive diagonal and off-diagonal metric samples, including nonzero `h_xz` | `(h_xx h_zz - h_xz^2) hww^2 = 1` after the chosen enforcement path | Exact identity to roundoff | Missing hidden `hww`, wrong block determinant, accidental spherical Jacobian factors | Ricci/source-term correctness | Yes | Define project enforcement in the GRChombo-facing variable set |
| Full 4D trace-free projection | 3F/3G | C++ unit | Random symmetric reduced `A_AB`, `Aww`, positive `h_AB`, `hww` | `h^{xx}A_xx + 2h^{xz}A_xz + h^{zz}A_zz + 2Aww/hww = 0` after projection | Exact identity to roundoff | Using 2D trace, missing hidden factor, omitting off-diagonal contraction | Evolution stability or damping signs | Yes | Yes |
| `/4` trace denominator guard | 3F/3G/3H | C++ unit | Known `K_ij`, `K`, `h_ij` fixtures | `/4` reconstructs `K_ij`; `/2` and `/3` fail in a deliberate negative test | Exact identity and expected nonzero failures | Accidentally using `CH_SPACEDIM` or 3+1 intuition | Sign convention of `K_ij` | Yes | Yes for final convention naming |
| Hidden `hww` determinant/trace participation | 3F/3G | C++ unit | Same visible metric with varied `hww` | Determinant and trace tests change with `hww` as expected | Exact identity to roundoff | Treating hidden sector as passive or diagnostic-only | Ricci hidden derivatives | Yes | Yes |
| Off-diagonal inverse and determinant handling | 3G | C++ unit | Nonzero `h_xz` values near but safely below positive-definite limit | Matrix inverse satisfies `h h^{-1}=I`; determinant uses `h_xx h_zz - h_xz^2` | Exact identity to roundoff | Reciprocal inverse, wrong sign on `h^{xz}`, missing positive-definiteness checks | Nonlinear RHS correctness | Yes | No beyond storage convention |
| `K_ij` reconstruction including `K_xz` | 3G | C++ unit | Off-diagonal physical `K_xz` and conformal `A_xz` fixtures | `K_xz = chi^{-1}(A_xz + h_xz K/4)` | Exact identity to roundoff | Dropped off-diagonal extrinsic-curvature component, wrong denominator | RHS signs and gauge coupling | Yes | Yes for `K_ij` sign convention |
| Round two-sphere positive-curvature anchor | 3C/3D | Symbolic geometry / C++ geometry unit | Round `S^2` angular sector with radius `a`, or the equivalent extracted hidden angular-sector fixture | `R_{S^2} = 2/a^2` for the two-sphere scalar curvature, with the fixture convention stated explicitly | Exact symbolic identity or roundoff-level C++ check | Wrong Ricci sign, angular-index contraction error, missing angular multiplicity | Black-string source terms, full CCZ4 RHS, gauge or damping behavior | Yes before trusting cartoon Ricci signs | No for the local geometry anchor; yes before source-term placement |
| Cartoon Ricci source block fixtures | 3D/3E/3G | RHS-block | Analytic diagonal and sheared-flat metrics from symbolic scripts | C++ Ricci/source block matches symbolic expected values | Tolerance-based against symbolic exact values | Missing `1/x`, `1/x^2`, hidden derivative, or off-diagonal geometry terms | Full CCZ4 evolution and damping behavior | Yes before trusting source terms | Use GRChombo-facing term placement; compare externally later as validation |
| Small-`x` regularized combinations | 3I | C++ unit | Regular and deliberately irregular Taylor data near the axis | Regular data gives finite limits; irregular data triggers expected diagnostic/failure | Exact limit fixtures plus tolerance near finite `x` | Unsafe raw `1/x` forms, missing matching condition, over-aggressive regularization | Correct ghost-zone strategy | Yes | Yes for axis-fill and stencil conventions |
| Assembled `hat_Gamma^x` axis cancellation | 3I | C++ unit | Taylor data with `h_xx-hww=O(x^2)`, odd `h_xz`, regular `Z^x` | Assembled `tilde_Gamma^x` singular part is finite and `hat_Gamma^x` remains axis-regular | Exact symbolic target or tight tolerance | Wrong sign/multiplicity in highest-risk connection expression | Full Gamma-driver evolution | Yes | Yes, required |
| `hat_Gamma^A = tilde_Gamma^A + 2Z^A` convention | 3H/3I | C++ unit | Controlled `tilde_Gamma^A` and `Z^A` samples | Encoded `hat_Gamma^A` and recovered constraint quantity match the chosen convention | Exact identity to roundoff | Wrong factor of two, confusing lowered and raised `Z` components | Damping signs in evolution | Yes | Yes, required |
| Stage 4B public CCZ4 baseline-layout check | 3J/4B | C++ layout fixture | Public CCZ4 enum retained as a comparison baseline; visible helper input map | Public symbols bind to the expected names/indices; current slots before `K` and `Theta` are identified as public `h33` and `A33` | Compile-time assertions plus runtime string/name checks | Public CCZ4 name/order drift before adding repo-owned variables; stale visible helper input map | Real grid reads, hidden enum implementation, real `hww/Aww` placement, helper integration, source terms | Yes before Stage 4C/4D | GRChombo-facing public layout is the baseline; real hidden placement guard is Stage 4C |
| Stage 4C hidden enum placement guard | 3J/4C | Header-level compile-time check plus updated layout fixture | Repo-owned `hww/Aww` enum entries in `BlackStringToy/UserVariables.hpp` | `UserVariables.hpp` static assertions prove `c_hww == c_K - 1` and `c_Aww == c_Theta - 1`; the placement fixture verifies names and helper-map slots | Compile-time assertions plus runtime name checks | Wrong `hww/Aww` placement, missed AH positional hazard, hidden helper input mismatch | Evolution, source terms, finite-difference correctness, real grid handoff correctness | Yes before Stage 4D grid handoff | Uses actual Stage 4C enum names |
| Stage 4E grid-to-helper handoff diagnostic | 3J/4E | Scaffold diagnostic / grid-read fixture plus standalone mapping fixture | Cheap smoke scaffold grid state with Stage 4D finite hidden-variable support enabled; distinct local values `chi=2`, `h11=3`, `h12=5`, `h22=37`, `hww=11`, `A11=13`, `A12=17`, `A22=19`, `Aww=23`, `K=29` | Stage 4A helper receives finite values from the intended slots; the standalone fixture checks exact component values and `K_ww = 51.375` | Runtime finite-value checks plus exact local value mapping to roundoff | Wrong live component handoff, swapped helper-map slots, non-finite hidden slots, invalid local conformal determinant before helper use | Cartoon Ricci, RHS correctness, physical hidden-sector evolution, finite-difference correctness | Yes before any helper output is used by evolution | Check-only; helper results are not written back |
| Stage 4F cartoon Ricci interface fixture | 3J/4F | C++ interface/type fixture | Local-value metric-derivative input and output structures for future cartoon Ricci helper | Interface compiles; hidden multiplicity is two; after Stage 4G, `ricci_formulas_implemented` is true | Compile/type check only | Interface drift, accidental hidden multiplicity change, ambiguous metric-derivative versus Gamma-based Ricci contract | Source term, RHS, evolution, small-`x` safety, or physical correctness | Yes before Ricci implementation | Later RHS wiring must resolve compatibility with any Gamma-based GRChombo Ricci expectation |
| Stage 4G metric-derivative cartoon Ricci fixture | 3J/4G | C++ local geometry unit | Local Stage 4F inputs for flat data, constant `q0=4` cone at `x=2`, nonconstant `q=(1+x)^2` at `x=1`, and Stage 3G `x`-shear flat metric | Flat and shear cases give zero Ricci; constant cone gives `R_ww=-3/4`; nonconstant `q` gives `R_xx=-2`, `R_ww=-12`; `x=0` is rejected | Roundoff-level local exact oracles | Wrong angular sign, confusing `hww` with `x^2 hww`, missing metric derivatives, broken off-diagonal inverse path, unsafe axis acceptance | RHS wiring, Gamma-based Ricci compatibility, full small-`x` regularization, physical evolution | Yes before any Ricci output is used outside local tests | Metric-derivative form only; not Gamma-based `CCZ4Geometry` Ricci |
| Stage 4I typed Ricci bridge contract fixture | 3H/3J/4H/4I | C++ local contract fixture | `R_xx=2`, `R_xz=3`, `R_zz=5`, `R_ww=7`; `h^{xx}=11`, `h^{xz}=13`, `h^{zz}=17`, `h^{ww}=19`; `chi=0.5` | Raw cartoon Ricci fields are not directly accessible; bridge exposes the intended RHS-facing values; full 4D conformal-inverse contraction is `451`; physical scalar is `225.5`; missing off-diagonal or hidden factors fail; nonpositive `chi` rejected | Exact local identities to roundoff plus compile-time access restriction | Wrong Ricci component placement, missing hidden `R_ww`, missing off-diagonal factor, missing hidden multiplicity, accidental direct-use convention drift | Live RHS correctness, Gamma-driver/damping signs, physical evolution | Required before any Ricci output is wired into RHS | No evolution wiring; future RHS-facing code must cross the bridge |
| Stage 4J local Ricci-to-RHS contract fixture | 3H/3J/4H/4I/4J | C++ local contract fixture | Bridge-approved `RhsRicciComponents` with `R_xx=2`, `R_xz=3`, `R_zz=5`, `R_ww=7`; `h^{xx}=11`, `h^{xz}=13`, `h^{zz}=17`, `h^{ww}=19`; `chi=0.5`; local `x=2` | Contract accepts bridge Ricci, not raw cartoon Ricci; full 4D Ricci trace is `451`; physical scalar is `225.5`; dropping `Rww` or the off-diagonal factor fails; `x=0` rejected | Exact local identities to roundoff plus type-boundary check | Future RHS grabbing raw Ricci, omitting `Rww`, missing hidden multiplicity, missing off-diagonal factor, accepting unsafe axis input | Real RHS formula correctness, evolution correctness, small-axis regularization, Gamma-based compatibility | Required before any repo-owned RHS source block consumes Ricci | No evolution wiring; local contract only |
| Stage 4K local RHS source-block skeleton fixture | 3H/3J/4J/4K | C++ local skeleton fixture | Stage 4J `RhsLocalInputs` contract with bridge-approved Ricci; hard-coded `451` trace data | Source-block skeleton accepts the contract type, not raw cartoon Ricci; named inert fields `rhs_chi`, `rhs_hww`, `rhs_Aww`, `rhs_K` are finite only in the explicit test path; carried Ricci trace remains `451` | Compile/type check plus roundoff-level local identities | Future source block bypassing the contract, unnamed source outputs, accidental non-finite placeholders, mistaking inert zeros for implemented formulas | Real source-term formulas, evolution wiring, grid reads, small-axis regularization | Required before implementing RHS source formulas | Skeleton/container only; no physical RHS terms |
| Stage 4L trace-free Ricci projection fixture | 3F/3G/3J/4L | C++ local formula fixture | Bridge-approved Ricci values with lower metric and conformal inverse metric; hard-coded `451` trace case plus a consistent diagonal metric case | `R_xx^TF=-110.75`, `R_xz^TF=3`, `R_zz^TF=-107.75`, `R_ww^TF=-105.75` for the hard-coded case; consistent case has zero full 4D trace after projection; `x=0` is rejected | Exact local identities to roundoff | Missing hidden `ww` factor, dropping `R_ww`, using `/3` instead of `/4`, wrong trace-free contraction, unsafe away-axis acceptance | Full RHS correctness, live evolution, grid reads, small-axis regularization | Required before Ricci projection is used in RHS source formulas | Local formula block only |
| Stage 4M away-axis policy fixture | 3I/3J/4M | C++ local policy fixture | Local reduced coordinates `x = 2`, `0`, `-1`, NaN, and infinity; Stage 4L source-block input with `x = 0` | Finite positive `x` passes; zero, negative, NaN, and infinity reject; guarded `1/x` and `1/x^2` at `x=2` return `0.5` and `0.25`; Stage 4L boundary still rejects `x=0` | Exact local identities plus exception checks | Hand-rolled unsafe axis acceptance, silent epsilon/clamping behavior, unguarded inverse helpers | True Stage 3I small-axis regularization, finite axis limits, live RHS correctness | Required before future explicit `1/x` or `1/x^2` RHS terms are added | Away-axis policy only; no regularized-axis support |
| Stage 4N singular-combinations fixture | 3I/3J/4N | C++ local helper fixture | Finite values for `d_x f`, `f`, `g`, and `x`; bad zero/negative/NaN/infinite `x`; bad NaN/infinite function values | `first_derivative_over_x(6,2)=3`; `difference_over_x2(10,6,2)=1`; all bad inputs reject; helpers agree with Stage 4M inverse helpers | Exact local identities plus exception checks | Future raw `1/x` or `1/x^2` use, unsafe nonfinite inputs, bypassing axis policy | True small-axis regularization, finite axis limits, full RHS correctness | Required before future singular-looking source combinations are used in RHS blocks | Away-axis singular combinations only; no physical RHS term |
| Stage 4O axis-regime semantics fixture | 3I/3J/4O | C++ local policy fixture | Current `CartoonAxisPolicy` regime constants; local reduced coordinates `x = 2`, `0`, `-1`, NaN, and infinity | `AwayAxisOnly` is the only implemented regime; regularized-axis support is not exposed as working; guarded `1/x=0.5` and independently guarded `1/x^2=0.25` at `x=2`; invalid inputs reject | Exact local identities plus exception checks | Future clamp drift, accidental regularized-mode exposure, unsafe `1/x^2` inheritance from clamped `1/x` | True Stage 3I small-axis regularization, finite axis limits, real RHS source terms | Required before real singular source terms use the helper path | Away-axis semantics only; no clamp, epsilon replacement, or physical axis evaluation |
| Stage 4P cartoon geometry primitives fixture | 3I/3J/4P | C++ local helper fixture | Local `x=2`, `d_x hww=6`; bad zero/negative/NaN/infinite `x`; bad NaN/infinite derivative value | `dx_hww_over_x=3`; helper agrees with Stage 4N singular-combination helper; bad inputs reject; raw output has no public `hxx_minus_hww_over_x2` field | Exact local identities plus compile-time shape check and exception checks | Future raw `(hxx-hww)/x^2` source use from Stage 4P, bypassing the Stage 4R guarded path, unsafe nonfinite inputs | Full Ricci tensor, CCZ4 RHS source terms, finite axis limits, live evolution, enforcement of `hxx-hww=O(x^2)` on grid data | Required before these geometry combinations appear in source blocks | Away-axis low-risk primitive only; risky metric difference is source-facing only through Stage 4R |
| Stage 4Q regularity matching fixture | 3I/3J/4Q | C++ local guard fixture | Local matching sample `x=0.1`, `h_ww=1`, `h_xx=1+0.25x^2`; clear mismatch `h_xx=1.2`; bad zero/negative/NaN/infinite inputs | Matching sample passes with finite residual; clear mismatch rejects; invalid inputs and bad tolerance reject | Tolerance-based local exception checks | Obvious local violations of `hxx-hww=O(x^2)`, unsafe finite/nonfinite inputs, accidentally claiming analytic regularity | Full analytic regularity proof, finite axis limits, grid-data matching enforcement, real RHS source terms | Required before using `(hxx-hww)/x^2` near the axis in a source block | Pointwise guard only; no Stage 3I regularized construction |
| Stage 4R regularity-guarded source-block fixture | 3I/3J/4R | C++ local source-style fixture | Matching sample `x=0.5`, `h_ww=1`, `h_xx=1+0.25x^2`, `d_x hww=1`; clear mismatch and bad inputs | Guarded package returns `dx_hww_over_x=2`, source-facing `hxx_minus_hww_over_x2=0.25`, matching residual, and agrees with Stage 4P for `dx_hww_over_x`; mismatch and invalid inputs reject | Exact local identities plus exception checks | Future source block bypassing the Stage 4Q guard before using the risky metric-difference ingredient, unsafe invalid inputs, accidental full-RHS or regularization claims | Full Ricci tensor, CCZ4 RHS, finite axis limits, grid reads, evolution wiring | Required before these guarded ingredients are used in future source blocks | Local guarded ingredient package only |
| Stage 4S local RHS guarded-geometry integration fixture | 3I/3J/4S | C++ local source-block fixture | Matching sample `x=0.5`, `h_ww=1`, `h_xx=1+0.25x^2`, `d_x hww=1`; clear mismatch and bad inputs through `CartoonRhsSourceBlock` | Local RHS source-block output carries `dx_hww_over_x=2`, source-facing `hxx_minus_hww_over_x2=0.25`, and matching residual from the Stage 4R guarded path; mismatch and invalid inputs reject; guarded package/carry output are not open aggregates | Exact local identities plus exception and type-shape checks | Source-block integration accidentally bypassing Stage 4R, dropping guarded geometry fields, unsafe invalid inputs, reintroducing forgeable trusted packages | Full Ricci tensor, CCZ4 RHS, finite axis limits, grid reads, evolution wiring | Required before guarded ingredients are consumed by later source-block formulas | Local source-block integration only |
| Stage 4T guarded source-consumer fixture | 3I/3J/4T | C++ local consumer fixture | Checked Stage 4R/4S guarded geometry package from matching sample; clear mismatch before package construction | Diagnostic consumer accepts the checked package, reports `dx_hww_over_x=2`, `hxx_minus_hww_over_x2=0.25`, residual status, and cannot be invoked with raw doubles by type | Exact local identities plus exception and type-shape checks | Future consumer accepting raw unverified geometry values, bypassing checked package construction, or claiming diagnostic data is a source formula | Ricci tensor, CCZ4 RHS, finite axis limits, grid reads, evolution wiring, physical formula validation | Required before a later real source formula consumes guarded ingredients | Diagnostic/probe consumer only |
| Stage 4U source-formula authoring-gate fixture | 3I/3J/4U | C++ local authoring-boundary fixture | Checked Stage 4R/4S guarded geometry package from matching sample; clear mismatch before gate construction | Non-aggregate authoring-gate input carries the checked package without flattening into raw doubles; dummy consumer accepts only the gate type and returns a harmless diagnostic scalar | Exact local identities plus exception and type-shape checks | Future formula signatures accepting raw `h_xx`, `h_ww`, `x` for the risky metric difference, or laundering checked geometry back into loose doubles | Ricci tensor, CCZ4 RHS, finite axis limits, grid reads, evolution wiring, physical formula validation, direct hand-written or generic-helper recomputation of `(hxx-hww)/x^2` outside the authoring gate | Required before a later real source formula consumes regularity-sensitive geometry | Authoring gate only; generic-helper bypass prevention remains a review/lint responsibility |
| Stage 4V source-formula derivation-lock gate | 3I/3J/4G/4L/4V | Documentation gate | Existing Stage 4G metric-derivative Ricci helper, Stage 4L trace-free projection, and Stage 4U authoring gate | No speculative formula is added until a tiny source sub-expression consuming the checked Stage 4U package is derived or extracted with a named oracle | Review gate only | Prevents inventing physics coefficients or treating a diagnostic combination as Ricci/RHS physics | Future derivation itself, formula implementation, grid reads, evolution wiring, full Ricci or CCZ4 RHS correctness | Required before first real source-formula consumer of the authoring gate | Outcome B: derive or isolate the exact expression next |
| Stage 4W hidden-sphere `R_ww` derivation-lock gate | 3D/3E/3I/3J/4G/4W | Documentation gate | Hidden-sphere metric ansatz, Stage 4G flat/cone/nonconstant `R_ww` oracles, Stage 4U checked-path context, Stage 3A sign caveat | Locks `tilde{R}_ww[h]`, `R^chi_ww`, physical `R_ww[gamma]`, the varying-`chi` oracle, and the need for a checked local `h_xz / x` ingredient before implementation | Review gate only | Prevents adding `R_ww` code before the conformal/physical split, trace map, oracles, sign convention, and off-diagonal regularity blocker are documented | Formula implementation, full off-diagonal parity validation, finite axis limits, grid reads, evolution wiring | Required before implementing physical `R_ww[gamma]` | Stage 4X should add the local checked quotient first; grid-level parity validation remains future work |
| Stage 4X checked `h_xz / x` ingredient fixture | 3I/3J/4W/4X | C++ local checked-quotient fixture | Local `x` and `h_xz` values, including `x=2, h_xz=6`, `h_xz=0`, invalid axis inputs, and nonfinite `h_xz` | Checked package carries finite `h_xz / x = 3` for the allowed example and `0` for zero `h_xz`; invalid axis and nonfinite inputs reject; package is not an open aggregate | Exact local identities plus exception and type-shape checks | Future formulas needing a named checked `h_xz / x` ingredient before constructing `h^xz h_xz / x^2`; accidental raw-double consumer shape | Full `R_ww`, conformal-factor Ricci, CCZ4 RHS, finite axis limits, grid-wide parity proof, analytic proof of `h_xz = O(x)`, grid reads, evolution wiring | Required before implementing physical `R_ww[gamma]` | Local pointwise ingredient only; finite nonzero `h_xz / x` is allowed and this does not reject all parity violations |
| Stage 4Y conformal Rww singular-gradient fixture | 3I/3J/4W/4U/4X/4Y | C++ local formula fixture | Flat, constant-cone, and distinct reduced conformal metric samples passed through one local-point factory for `ConformalRwwSingularBlockInputs` | `G_sing=0` flat, `-3/4` constant cone, `4/31` distinct-value; invalid checked-ingredient construction fails before formula; zero/negative/nonfinite determinant rejects; input package is not an open aggregate | Exact local identities plus exception and type-shape checks | Hand-rolled `(hxx-hww)/x^2` or `h_xz/x` in the formula boundary, mixing checked singular ingredients from one point with determinant data from another, determinant singularity, accidental full-Ricci/RHS claims | Full `tilde R_ww`, `R^chi_ww`, physical `R_ww[gamma]`, full Ricci, CCZ4 RHS, finite axis limits, grid reads, evolution wiring, global parity proof | Required before expanding toward additional conformal hidden Ricci sub-blocks | First guarded formula sub-block only; away-axis local value; Checkpoint B aligns determinant policy with 4Z/4AB/4AC |
| Stage 4Z conformal Rww first-derivative gradient fixture | 3I/3J/4W/4X/4Y/4Z | C++ local formula fixture | Flat, constant-cone, nonconstant-`W`, and distinct reduced conformal metric samples passed through one local-point factory for `ConformalRwwGradientBlockInputs` | `G_grad=0` flat, `0` constant cone, `-5` nonconstant hidden metric, `-82/93` distinct-value; checked `p_W` and input package are not open aggregates; invalid axis, `W`, determinant, and derivative inputs reject | Exact local identities plus exception and type-shape checks | Hand-rolled `W_x/x` or `h_xz/x` in the formula boundary, mixing checked quotient ingredients from one point with determinant data from another, determinant or `W` singularity, accidental full-Ricci/RHS claims | Full `tilde R_ww`, Hessian block, `R^chi_ww`, physical `R_ww[gamma]`, full Ricci, CCZ4 RHS, finite axis limits, grid reads, evolution wiring, global parity/regularity proof | Required before conformal hidden Ricci assembly | First-derivative gradient sub-block only; away-axis local value |
| Stage 4AA Hessian derivation-lock gate | 3I/3J/4W/4Z/4AA | Documentation gate | Reduced metric fields and derivatives for the Hessian block, plus reviewed Stage 4Y/4Z oracles | `G^Hess_ww` formula, `rho` derivative formulas, reduced Christoffels, flat `0`, constant-cone `0`, nonconstant-`W` `-4`, combined `tilde R_ww=-12`, verified nonsymmetric `G^Hess_ww=-8558/2883`, and verified nonsymmetric full conformal sum `-3576/961` | Review gate only; Claude Audit A verified the nonsymmetric oracle | Prevents Hessian code before signs, factors, Christoffels, and a nontrivial off-diagonal oracle are locked | Full `tilde R_ww`, `R^chi_ww`, physical Ricci, RHS/evolution, global parity/regularity proof | Required before Stage 4AB implementation | Docs-only derivation lock; Stage 4AB must include this nonsymmetric oracle in its test |
| Stage 4AB conformal Rww Hessian fixture | 3I/3J/4W/4AA/4AB | C++ local formula fixture | Flat, constant-cone, nonconstant-`W`, and Claude-verified nonsymmetric reduced conformal metric derivative samples passed through one local-point factory for `ConformalRwwHessianBlockInputs` | `G^Hess=0` flat, `0` constant cone, `-4` nonconstant `W`, `-8558/2883` nonsymmetric; test-only conformal sum `-3576/961`; invalid axis, `W`, determinant, and derivative inputs reject; input package is not an open aggregate | Exact local identities plus exception and type-shape checks | Wrong Hessian signs/factors, wrong reduced Christoffels, missing `rho_xz`, missing `W_z` terms, wrong `(-2B/D)` contraction, unsafe or mixed raw inputs | Full `tilde R_ww`, `R^chi_ww`, physical Ricci, RHS/evolution, global parity/regularity proof | Required before Stage 4AC conformal assembly | Hessian sub-block only; away-axis local value |
| Stage 4AC conformal Rww assembly fixture | 3I/3J/4W/4Y/4Z/4AB/4AC | C++ local assembly fixture | Flat, constant-cone, nonconstant-`W`, and Claude-verified nonsymmetric reduced conformal metric derivative samples passed through one local-point factory for `ConformalRwwInputs` | `tilde R_ww=0` flat, `-3/4` constant cone, `-12` nonconstant `W`, `-3576/961` nonsymmetric; selected `chi=1` cases agree with Stage 4G direct Ricci; invalid inputs reject through underlying factories; input package is not an open aggregate | Exact local identities, Stage 4G roundoff comparison, exception checks, and type-shape checks | Dropping or double-counting a conformal sub-block, mixing sub-block inputs from different points, accidentally exposing loose block values, or claiming physical Ricci | `R^chi_ww`, physical `R_ww[gamma]`, RHS/evolution, global parity/regularity proof | Required before Stage 4AD | Conformal `tilde R_ww[h]` assembly only; away-axis local value; Checkpoint B determinant cleanup is complete |
| Stage 4AD R^chi_ww derivation and guard-stack gate | 3I/3J/4W/4AC/4AD | Documentation gate | Conformal-factor Ricci split, scalar derivatives of `chi`, reduced metric values/derivatives, existing checked `q_xz` and `p_W` context | Locks `D_wD_w chi`, `D^KD_K chi` with hidden multiplicity `2/W`, `D^K chi D_K chi`, checked `chi_x/x` requirement, constant-`chi` oracle `0`, flat `chi=1+a x` oracle `11/144`, and z-dependent oracle `-1/64` | Review gate only | Prevents `R^chi_ww` code before the hidden Laplacian multiplicity, singular `chi_x/x` ingredient, and oracle set are documented | Implementation correctness, split-vs-direct physical Ricci identity, RHS/evolution, global parity/regularity proof | Required before Stage 4AE | Docs-only derivation and guard design |
| Stage 4AE conformal-factor Rww fixture | 3I/3J/4W/4AC/4AD/4AE | C++ local formula fixture | Single-source local `x,A,B,C,W,chi`, base/W first derivatives, and `chi` first/second derivatives with checked `q_xz`, `p_W`, and `p_chi` | Constant `chi` gives `0`; flat `chi=1+ax` gives `11/144`; flat `chi=1+bz` gives `-1/64`; nonsymmetric sample gives `D_wD_w chi=131/62`, Laplacian `10430/2883`, norm `49/31`, and `R^chi_ww=63341/48050`; invalid inputs reject; packages are non-aggregate | Exact local identities, exception/type checks, and one test-only Stage 4G physical-minus-conformal comparison | Missing hidden multiplicity, raw quotient bypass, mixed-point inputs, Christoffel/sign errors, invalid `chi/W/D/x` acceptance | General split-vs-direct hard gate, physical `R_ww` assembly, RHS/evolution, global parity/regularity proof | Required before Checkpoint C and Stage 4AF | `R^chi_ww` only; away-axis local value |
| Stage 4AI physical Rww Ricci/RHS contract fixture | 3J/4AH/4AI | C++ local typed-contract fixture | Non-forgeable Stage 4AH physical result carrying `R_ww[gamma]`, same-point `chi`, and conformal `h^ww=1/h_ww` | Flat contributions `0`; cone contributions `-3/8`; linear-`x` contributions `11/72` and `11/60`; nonsymmetric case matches `2 h^ww R_ww` and `chi 2 h^ww R_ww`; contract/result types are non-aggregate and raw `double` is not invocable | Exact local identities and compile-time type-shape checks | Hidden multiplicity omission, wrong inverse metric factor, missing `chi`, accepting conformal/correction/raw values at the physical contract boundary | Full Ricci scalar, full RHS, grid validation, finite-axis support, evolution | Required before Checkpoint E and Stage 4AJ | Hidden `ww` contribution contract only; away-axis local value |
| Stage 4AJ physical hidden lapse Hessian fixture | 3J/4AE/4AJ | C++ local formula fixture | Single-source local `x,A,B,C,W,chi,W_x,W_z,chi_x,chi_z,alpha_x,alpha_z` with checked `q`, `p_W`, `p_chi`, and `p_alpha` | Constant alpha gives `0`; flat linear-`x` gives `3/20`; flat linear-`z` gives `0`; varying `chi` gives `11/80`; nonsymmetric sample gives `699/155`; varying and nonsymmetric cases match the direct physical formula | Exact local identities, direct physical-form comparison, exception checks, and type-shape checks | Accidental conformal-Hessian reuse, missing checked `alpha_x/x`, wrong `gamma=h/chi` factors, mixed-point inputs, invalid `x/W/chi/D` acceptance | Source minus sign, full `A_ww` source block, full RHS, grid reads, finite-axis support, evolution | Required before Stage 4AK | Physical hidden lapse Hessian only; away-axis local value |
| Stage 4AK Aww curvature/lapse core fixture | 3J/4AH/4AJ/4AK | C++ local composition fixture | Single-source local metric/conformal-factor/lapse jet that mints Stage 4AH physical `R_ww[gamma]` and Stage 4AJ physical `D_wD_w alpha` packages | Flat `0`; flat linear-lapse `-3/20`; constant cone `-3/2`; varying `chi` `-11/720`; nonsymmetric same-point case checks `minus_dww_alpha`, `alpha_rww`, and their sum; invalid inputs reject; packages are non-aggregate | Exact local composition, exception checks, and type-shape checks | Wrong source sign, mixed-point Ricci/Hessian packages, treating the core as full `A_ww`, accepting loose raw values | Z4 term, trace-free projection, outer `chi`, nonlinear A/K terms, full RHS, grid reads, finite-axis support, evolution | Required before Stage 4AL | Geometric core only; away-axis local value |
| Stage 4AL trace-free curvature/lapse block fixture | 3J/4G/4I/4AH/4AK/4AL | C++ local projection fixture | Single-source local h/chi/alpha jet, Stage 4G/4I visible Ricci, Stage 4AH physical `R_ww`, Stage 4AK hidden core, and computed inverse metric | Flat all zero; linear-lapse `tf_xx=tf_zz=3/40`, `tf_ww=-3/40`; constant cone trace `-3/4`; nonsymmetric sample checks hard-coded core/TF/source components and zero 4D TF trace; invalid inputs reject; packages are non-aggregate | Exact local identities, Stage 4G-vs-4AH `R_ww` agreement, trace-free zero-trace check, exception checks, and type-shape checks | Wrong physical Hessian correction, wrong hidden multiplicity, missing outer `chi`, wrong `/4` denominator, mixed-point inputs, loose raw component path | Z4 terms, nonlinear A/K terms, full CCZ4 RHS, grid reads/wiring, finite-axis support, evolution | Checkpoint F complete after review | Local trace-free geometry source only; away-axis value |
| Stage 4AM `hat_Gamma^x` derivation lock | 3I/3J/4AM | Documentation gate | Read-only GRChombo convention map plus local reduced metric derivatives `A,B,C,W,A_x,A_z,B_x,B_z,C_x,C_z,W_x,W_z` | Locks `vars.Gamma=chris.contracted+2 Z_over_chi`, the translation to project `hat_Gamma^x`, the hidden contraction with `2 W^{-1} Gamma^x_ww`, the determinant-reduced cross-check under `D W^2=1`, required oracles `0`, `-3/4`, `-1`, and `-35/961`, and the Gamma RHS term map | Review gate only | Wrong sign/factor of two in hatted convention, confusing GRChombo `Z` with `Z_over_chi`, missing hidden multiplicity, using determinant-reduced identity without determinant certification | C++ implementation, full Gamma RHS, GL validation, grid regularity, finite-axis support, evolution | Required before Stage 4AN | Docs-only; Stage 4AO remains the hard physical anchor |
| Stage 4AN `hat_Gamma^x` local contract fixture | 3I/3J/4AM/4AN | C++ local formula fixture | Single-source local `x,A,B,C,W,A_x,A_z,B_x,B_z,C_x,C_z,W_x,W_z,Z_over_chi^x`, checked Stage 4U/4R `Delta_xw`, and checked Stage 4X `B/x` | Flat `0`; constant cone `-3/4` and multiplicity-one negative check `-3/8`; determinant-one off-diagonal `-1` and determinant-reduced identity comparison; distinct case base `27/961`, hidden `-2/31`, tilde `-35/961`; with `Z_over_chi^x=1/2`, hat `926/961`; invalid inputs reject; packages are non-aggregate | Exact local identities, exception checks, type-shape checks, and GRChombo hatted convention recovery `Z_over_chi=0.5(hat-tilde)` | Hand-rolling `(A-W)/x` or `B/x`, wrong hidden multiplicity, wrong reduced Christoffels, accepting loose raw values, confusing `Z_over_chi` with lower or physical Z | Gamma RHS, GL validation, grid regularity, finite-axis support, evolution | Required before Stage 4AO | Local away-axis hatted connection only |
| Stage 4AF-4AU hidden-sphere Ricci/RHS gates | 3I/3J/4W/4AE | C++ local fixtures plus derivation, identity, GL, grid, and finite-axis gates | Direct physical Ricci comparison, synthetic parity primitive, actual grid/ghost regularity data, finite-axis limit data, `hat_Gamma^x` convention data, and GL anchor data | Concrete gates for split-vs-direct identity, synthetic `h_xz` parity primitive, physical `R_ww`, hidden lapse, trace-free source, `hat_Gamma^x`, GL growth/dispersion, actual grid regularity, finite-axis evaluation, RHS integration, and smoke-freeze removal | Derivation exactness, local identities, direct Ricci comparison, hard GL validation, grid/axis validation, and later controlled evolution checks | Hidden-sphere Ricci reaching RHS/evolution before conformal-vs-physical split, real grid regularity, finite-axis, smoke-freeze, and `hat_Gamma^x` risks are owned | Production evolution correctness or Pau reproduction by itself | Required before serious hidden-sector RHS/evolution claims | See `stage4_hidden_sphere_Rww_plan.md` |
| Gamma-driver ownership boundary | 3H | C++ unit/design check | Mock RHS block inputs for gauge and `hat_Gamma^A` terms | Gauge block owns lapse/shift/auxiliary evolution; `hat_Gamma^A` block owns their appearances in `partial_t hat_Gamma^A` | Structural review plus targeted unit checks | Double-counting gauge terms, split ownership drift | Physical correctness of chosen gauge | Yes as a design review gate | Yes |

## Integration, Reference, And Convergence Tests

### Stage 4AM `hat_Gamma^x` Derivation Gate

Stage 4AM is documentation-only. It records that GRChombo evolves
`vars.Gamma[i]` as the hatted connection and reconstructs
`Z_over_chi[i]=0.5*(vars.Gamma[i]-chris.contracted[i])`; the project
shorthand `hat_Gamma^i=tilde_Gamma^i+2Z^i` therefore uses
`Z^i=Z_over_chi[i]` at the GRChombo-facing boundary. The locked hidden
contraction is

```text
tilde_Gamma^x =
    H^xx Gamma^x_xx
  + 2 H^xz Gamma^x_xz
  + H^zz Gamma^x_zz
  + 2 W^{-1} Gamma^x_ww,
```

with `Gamma^x_ww` carrying `(A-W)/x`, `B/x`, and `W_x` terms. The required
Stage 4AN local oracles are flat `0`, constant cone `-3/4`,
determinant-one off-diagonal `-1`, and the distinct derivative sample
`tilde_Gamma^x=-35/961`, split as base contraction `27/961` plus hidden
contraction `-2/31`. The determinant-reduced identity is only a cross-check
under `D W^2=1`.

The future Gamma RHS map must preserve all GRChombo terms and classify
dimension-dependent `d=4` factors, hidden/cartoon additions, gauge terms, and
Z4/damping terms. Stage 4AO remains a hard GL dispersion/growth-rate gate:
flat tests alone are insufficient, the physical growth observable must be
geometric, and Pau is not the convention authority.

### Stage 4AN `hat_Gamma^x` Local Contract Fixture

The Stage 4AN fixture checks the local away-axis implementation of
`tilde_Gamma^x` and `hat_Gamma^x = tilde_Gamma^x + 2 Z_over_chi^x`. The input
package is single-source and non-aggregate. It builds `(A-W)/x` as
`x Delta_xw` from the existing checked Stage 4U/4R package and obtains `B/x`
from the checked Stage 4X package, so the regularity-sensitive ingredients are
not hand-rolled.

The required oracles are flat `0`, constant cone `-3/4` with a negative
hidden-multiplicity-one value `-3/8`, determinant-one off-diagonal `-1`,
distinct derivative base `27/961`, hidden `-2/31`, tilde `-35/961`, and the
same distinct point with `Z_over_chi^x=1/2` giving `hat_Gamma^x=926/961`.
The fixture also recovers `Z_over_chi^x=0.5*(hat_Gamma^x-tilde_Gamma^x)`.
It does not implement the Gamma RHS, GL validation, grid regularity,
finite-axis handling, or evolution wiring.

### Stage 4AO GL Gate Fixture Sequence

Stage 4AO is decomposed into 4AO-A through 4AO-D; it is not one executable
gate. Flat tests and `hat_Gamma^x`-only tests are insufficient.

Stage 4AO-A is documentation/analytic fixture work for the Background and
analytic residual lock. It must lock the exact uniform ingoing-GP black-string
background, `r0`, compact-`z` period, background slicing gauge, evolution
gauge-driver and initial-gauge startup family, `K_ij` sign, perturbation
sector, and geometric observable; evaluate the continuum background residual;
include the full `hat_Gamma^x` hidden contraction; and verify the `1/x`
cancellations analytically. Stage 4AO-C frozen gauge and Stage 4AO-D live
gauge must use that same pre-locked gauge family. No discrete or spectral work
may begin before this passes.

The 4AO-A derivation fixture is
`docs/derivations/stage4AO_A_uniform_gp_background_residual.md`. It locks the
uniform ingoing-GP background, compact-`z` normalization, GRChombo
`K_IJ` sign, frozen-GP zero-residual targets, live moving-puncture lapse
startup residual, full background `hat_Gamma^x` hidden contraction, and the
analytic cancellation of the background cartoon `1/x` connection terms. It
also locks the componentwise `A_IJ` residual ledger, full `hat_Gamma^x` RHS
residual, constraints, live-gauge interpretation, and positive-inner-radius
domain `x in [x_in,x_out]` with `x_in>0`. The unmodified GP state is not a
full live-gauge equilibrium because the lapse startup residual is nonzero.
Stage 4AO-D therefore uses the GRChombo moving-puncture gauge family plus the
fixed, field-independent GP-holding lapse source
`S_alpha(x)=+3 sqrt(r0/x^3)`. This source is computed from the locked analytic
background and locked `r0`, is not recomputed from evolved fields, has no
linearized derivative, and is validation-harness-only rather than production
Stage 4AR/4AS wiring.
The horizon diagnostic is separate from the lapse: no `alpha=0` condition is
used. At selected output times the fixture path must solve the outermost
`S2 x S1` MOTS connected to `x=r0`,
`Theta_+=D_i s^i+K_ij s^i s^j-K=0`, record `x=h(t,z)`, and measure
`R_H=h(t,z)sqrt(h_ww/chi)` at that surface. Spectral fixtures use the
linearized MOTS response to map eigenvectors to `delta R_H`; acceptance compares
the horizon-harmonic growth rate, not the lapse profile. Frozen/live fixtures
use the same slicing convention, same GP background, and same horizon-radius
observable.

Stage 4AO-B is the discrete preflight fixture set. It must show background
residual convergence at the scheme order, isolate `delta hat_Gamma^x` with a
`delta hww`-only hidden-contraction test, compare the analytic or symbolic
Jacobian against a finite-difference Jacobian of the actual RHS, and verify
cosine/sine parity-sector block diagonalization. No eigensolver work may begin
before 4AO-B passes.

The raw background-residual convergence fixture must use the unmodified
discrete RHS. Its continuum target is zero for the verified geometric, scalar,
and constraint components, and `-3 sqrt(r0/x^3)` for the unmodified live
moving-puncture lapse equation. The fixed source `S_alpha=+3 sqrt(r0/x^3)` is
a 4AO-D validation-harness source; it must not be used to cancel measured
finite-grid residuals before the raw 4AO-B convergence test. After raw
residual convergence is demonstrated, the validation harness may use the
locked source for the stationary live-gauge 4AO-D problem.

Stage 4AO-C is the frozen-gauge spectral fixture. It must cover a threshold
zero crossing, primary-source convention mapping for `k_c r0`, unstable and
stable points, shift-invert targeting, finite-difference epsilon plateau, and
radial/boundary-location convergence. The commonly quoted
`k_c r0 ~= 0.876` value is provisional until that primary-source mapping is
complete.

Stage 4AO-D is the live-gauge/full acceptance fixture. It must check that the
physical GL eigenvalue agrees with the frozen-gauge result, physical/gauge/
constraint modes are separated, constraint-subsystem decay matches the derived
CCZ4 formulation, inner-boundary characteristics are checked, the eigenvector
is seeded into the time-evolution path and reproduces the spectral growth
rate, and the full convergence battery passes.

Dedicated 4AO-B/C/D validation harnesses may construct and evaluate the actual
RHS, perform finite-difference Jacobian checks, solve the frozen/live spectral
systems, and run the seeded-eigenvector evolution bridge. The blocker is
production integration, not those internal validation harnesses: Stage 4AR
controlled local RHS integration and Stage 4AS default-off live wiring remain
blocked until 4AO-D passes. Checkpoint G passes only after 4AO-D. Pau is not
the convention authority, and `hat_Gamma^x` alone is not the physical growth
observable.

### Stage 4AF Split-Vs-Direct Identity Fixture

The Stage 4AF fixture is an internal hard gate, not external validation. It
compares the Stage 4AC plus Stage 4AE split with Stage 4G after independently
constructing `gamma=h/chi`, `gamma_i`, and `gamma_ij`. Required coverage is
now locked as constant `chi`, flat linear-`x` and linear-`z` `chi`, and three
distinct nonsymmetric varying-`chi` points. Every nonsymmetric point has
nonzero `B`, `chi_x`, `chi_z`, base first derivatives, and
`W_x,W_z,W_xx,W_xz,W_zz`; one case carries nonzero `A,B,C` second derivatives
and checks their cancellation explicitly. The tolerance is `1e-10`; observed
identity residuals are at most about `4.44e-16`. Stage 4AG parity validation
is implemented below; Checkpoint D remains pending before Stage 4AH.

### Stage 4AG `h_xz` Parity Fixture

The Stage 4AG fixture validates a small synthetic paired near-axis stencil
rather than a single away-axis quotient. It accepts smooth odd `a x+b x^3`
data at multiple radii, confirms that the even quotient `a+b x^2` may vary
with radius, and rejects even `x^2` contamination, nonzero supplied axis data,
missing or mismatched partners, invalid coordinate signs, and nonfinite inputs.
The validated token is non-aggregate and factory-only. This fixture does not
test actual grid or ghost-cell data, diagonal matching, `W_x` parity,
`chi_x` parity, full smoothness, or finite-axis regularization. Stage 4AP owns
the real grid/ghost-data regularity gate.

### Stage 4AH Away-Axis Physical `R_ww` Fixture

The Stage 4AH fixture checks a single-source local assembly of Stage 4AC plus
Stage 4AE. It covers flat zero, constant-cone `-3/4`, flat linear-`x`
`11/144`, flat linear-`z` `-1/64`, and a fully nonsymmetric point with nonzero
off-diagonal, conformal-factor, and hidden-metric derivatives. The
nonsymmetric sum is also compared with Stage 4G using an independently formed
physical metric jet. Input/result types are non-aggregate and invalid data is
rejected by the underlying factories. This fixture does not validate actual
grid parity or matching, finite-axis values, RHS, or evolution.

### Stage 4AI Physical `R_ww` Contract Fixture

The Stage 4AI fixture accepts only the Stage 4AH result and verifies the
hidden contributions `2 h^ww R_ww[gamma]` and
`chi 2 h^ww R_ww[gamma]`. Flat, cone, linear-`x`, and nonsymmetric Stage 4AH
cases are covered. Compile-time checks keep both result types non-aggregate
and prove that a loose raw `double` is not accepted. The fixture does not test
the full Ricci scalar, full RHS, actual grid regularity, finite-axis values, or
evolution. Checkpoint E / Claude Audit E is recorded as complete before
Stage 4AJ.

### Stage 4AJ Physical Hidden Lapse Hessian Fixture

The Stage 4AJ fixture checks the physical-metric `D_wD_w alpha` block. It
locks constant lapse zero, flat linear-`x` `3/20`, flat linear-`z` zero,
varying-`chi` `11/80`, and nonsymmetric `699/155` oracles. The varying-`chi`
case must differ from the conformal-metric value `3/20`. The varying-`chi` and
nonsymmetric cases are independently compared against
`(1/(2x^2)) gamma^ab partial_a(x^2 gamma_ww) partial_b alpha`; observed
residuals are at roundoff or zero in the fixture. Invalid axis, `W`, `chi`,
determinant, and nonfinite derivatives reject. This fixture does not apply the
source minus sign, implement the full `A_ww` source block, read grid data,
support finite-axis evaluation, or wire evolution.

### Stage 4AK Aww Curvature/Lapse Core Fixture

The Stage 4AK fixture checks only
`C_ww=-D_wD_w alpha+alpha R_ww[gamma]`. It composes the reviewed Stage 4AH
physical Ricci package and Stage 4AJ physical lapse Hessian package from one
same-point jet. The fixture locks flat zero, flat linear-lapse `-3/20`,
constant cone `-3/2`, varying-`chi` `-11/720`, and a nonsymmetric same-point
case that checks `minus_dww_alpha`, `alpha_rww`, and their signed sum. Invalid
axis, `W`, `chi`, determinant, finite-lapse, and derivative failures reject
through the underlying factories. This is not the Z4 vector term, trace-free
projection, outer `chi` prefactor, nonlinear A/K terms, full RHS, grid read,
finite-axis support, or evolution.

### Stage 4AL Trace-Free Curvature/Lapse Block Fixture

The Stage 4AL fixture checks
`source_IJ=chi(C_IJ-h_IJ C/4)` with
`C_IJ=-D_ID_J alpha+alpha R_IJ[gamma]`. Visible Ricci components cross the
Stage 4G/4I bridge, the hidden component comes through Stage 4AH/4AK, and
Stage 4G physical `R_ww` must agree with Stage 4AH before assembly. The
nonsymmetric oracle uses nonzero `h_xz`, `alpha_xz`, `chi_x`, and `chi_z` and
locks:
`core=(-20.79847381200139,-26.639625390218519,-26.550102902069604,-11.562081165452653)`,
`trace=-13.620460168805643`,
`TF=(3.0373314834084866,-19.829395305815698,-9.524527691062552,-1.3467360388484213)`,
and
`chi TF=(15.186657417042433,-99.146976529078486,-47.62263845531276,-6.7336801942421065)`.
This fixture does not implement Z4 terms, nonlinear A/K terms, full RHS, grid
reads/wiring, finite-axis support, or evolution. Checkpoint F / Claude Audit F
is recorded as complete after reviewing this local block. Further source/RHS
work is blocked by Stage 4AO's GL growth/dispersion gate, Stage 4AP's real
grid/ghost-data regularity gate, and Stage 4AQ's finite-axis source-evaluation
gate.

### Stage 4AO-B Discrete Operator Preflight Fixture

The Stage 4AO-B fixture is a local validation harness for the locked 4AO-A
uniform ingoing-GP background. It is not an eigensolver, not a threshold
search, not production RHS integration, and not live evolution wiring.

- Harness: `code/BlackStringToy/Stage4AOGPDiscretePreflight.hpp`.
- Fixture:
  `code/BlackStringToy/tests/Stage4AOBDiscreteOperatorPreflightTest.cpp`.
- Provisional radial domain: `r0=1`, `x in [0.5,4.0]`, with
  `0<x_in<r0<x_out`.
- Raw residual check: evaluates the unmodified local discrete RHS and does not
  apply the 4AO-D holding source `S_alpha`. Verified zero-target components
  converge under refinement, while the unmodified live moving-puncture lapse
  equation is checked against `-3 sqrt(r0/x^3)`.
- Hidden-contraction check: perturbs only `delta hww` and isolates the
  hidden-contraction contribution to `delta hat_Gamma^x`, including the
  hidden multiplicity-two factor and the away-axis `1/x` cancellation pattern.
- Jacobian check: compares a hand-derived linearization of the actual
  discrete local RHS against a finite-difference Jacobian-vector product and
  records large-epsilon nonlinear error, a useful middle window, and
  too-small-epsilon roundoff.
- Parity check: the original z-independent scalar-multiplier check was
  insufficient because it only tested cosine/sine orthogonality. The fixture
  now applies periodic finite-difference `D_z` and `D_zz` stencils to a
  representative z-coupled sub-operator: scalar output receiving
  `D_z beta^z`, `h_xz`-like output receiving `D_z beta^x`, and
  `hat_Gamma^z`/momentum-z-like output receiving `D_z K`, with `D_zz`
  preservation terms. It then projects the raw output onto allowed and
  forbidden parity components and includes a flipped-`beta^z` negative guard.

Observed fixture output records raw residual errors `0.0150518`,
`0.00428846`, and `0.00114621` on 256, 512, and 1024 intervals, with ratios
`3.50983` and `3.74144`; hidden-contraction error `3.47146e-15`, which
validates the formula and hidden multiplicity rather than finite-difference
convergence; JVP errors `0.00183042`, `1.82974e-07`, `4.68507e-09`, and
`0.000375647`; even-sector allowed norm `7.39609` with leakage
`2.05279e-17`; odd-sector allowed norm `5.16184` with leakage
`3.93873e-17`; and flipped-`beta^z` negative-guard leakage `0.79241`.

### Stage 4AO-C Frozen-Gauge Spectral Fixture

Stage 4AO-C is not yet an executable spectral fixture. The status note
`docs/derivations/stage4AO_C_frozen_gauge_spectral_gate.md` records the
intended frozen-gauge operator and the blockers that prevent an honest
spectral test.

The first Stage 4AO-C substeps are now validation-only contract and partial
operator fixtures:

- Contract:
  `code/BlackStringToy/Stage4AOFrozenGaugeOperator.hpp`.
- Contract fixture:
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeOperatorContractTest.cpp`.
- GP-shift advection fixture:
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeAdvectionBlockTest.cpp`.
- Tensor shift-stretching fixture:
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeTensorShiftStretchingBlockTest.cpp`.
- Algebraic metric/chi coupling fixture:
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeAlgebraicCouplingBlockTest.cpp`.
- Selected-CCZ4 K-equation `K(K-2Theta)` / physical-`delta R` fixture:
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeKCCZ4BlockTest.cpp`.
- CCZ4 simple K/Theta damping insertion fixture:
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeCCZ4DampingInsertionTest.cpp`.
- Frozen-GP contracted-connection and Z-reconstruction helper fixture:
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeContractedConnectionTest.cpp`.
- First non-advection hatted-Gamma Z/kappa and `kappa3` shift-gradient fixture:
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeHatGammaZ4KappaBlockTest.cpp`.
- A-equation algebraic non-curvature fixture:
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeAAlgebraicBlockTest.cpp`.
- Theta-equation algebraic non-Ricci fixture:
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaAlgebraicBlockTest.cpp`.
- Theta-equation `-K_GP deltaTheta` fixture:
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaMinusKBlockTest.cpp`.
- Trace-free `delta A` projector fixture:
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeTraceFreeDeltaAProjectorTest.cpp`.
- Hidden physical `delta R_ww[gamma]` fixture:
  `code/BlackStringToy/tests/Stage4AOCHiddenPhysicalDeltaRwwTest.cpp`.
- Visible physical `delta R_xz[gamma]` fixture:
  `code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRxzTest.cpp`.
- Visible physical `delta R_zz[gamma]` fixture:
  `code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRzzTest.cpp`.
- Visible physical `delta R_xx[gamma]` fixture:
  `code/BlackStringToy/tests/Stage4AOCVisiblePhysicalDeltaRxxTest.cpp`.
- Raw Ricci trace / trace-free assembly fixture:
  `code/BlackStringToy/tests/Stage4AOCRicciTraceFreeAssemblyTest.cpp`.
- Theta Ricci scalar insertion fixture:
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaRicciScalarBlockTest.cpp`.
- A-equation trace-free Ricci curvature insertion fixture:
  `code/BlackStringToy/tests/Stage4AOCFrozenGaugeARicciCurvatureBlockTest.cpp`.

The contract fixture checks the 13-variable frozen-gauge perturbation vector,
the exclusion of `delta alpha`, `delta beta^i`, and `delta B^i`, the
per-variable RHS inventory labels, and the radial-domain contract
`0<x_in<r0<x_out` with compact `z`. It also asserts that only GP-shift
advection, tensor shift-stretching, the algebraic metric/chi coupling, and
the selected-CCZ4 K-equation `K(K-2Theta)` and physical-`delta R` blocks, the
simple K/Theta damping insertion, the A-equation non-curvature
algebraic block, the Theta-equation non-Ricci algebraic block, and the
Theta-equation `-K_GP deltaTheta`, Theta Ricci, and A Ricci insertion blocks have
`implemented_now` coverage, no frozen-gauge RHS variable is complete,
boundary tests are not implemented, eigensolver and shift-invert support are
absent, and production RHS wiring is absent. It also checks the separate
trace-free `delta A` projector contract flag. It is a guard against stale
status confusion, not the GL spectral gate.

The hidden physical `delta R_ww[gamma]` fixture is the first Ricci
implementation substep. It checks only the raw lower/lower hidden component on
the locked flat GP spatial background. The oracle is a central difference of
the Stage 4G metric-derivative physical Ricci engine, with the reviewed Stage
4AH split path as a secondary central-difference oracle. The fixture covers
zero input, pure `delta h_ww`, pure `delta chi`, z-dependent scalar hidden
data, radial visible-metric data, and mixed radial/z off-diagonal data. It
uses an epsilon sweep and requires the `1e-5` to `1e-6` plateau to agree with
both oracle paths. Negative guards show that wrong hidden multiplicity,
dropping hidden `1/x` terms, wrong off-diagonal z-derivative sign, or treating
the raw component as already trace-free would fail. This component-local
fixture does not perform the visible-component calculations or the A/Theta/K
Ricci insertions itself; those globally completed validation pieces have their
own fixtures. Full-operator JVP/parity, MOTS, eigensolver work, and threshold
searches remain missing.

The visible physical `delta R_xz[gamma]` fixture is the first one-z Ricci
implementation substep. It checks only the raw lower/lower visible
off-diagonal component on the locked flat GP spatial background. Stage 4G is
the primary and sole independent nonlinear oracle for this substep because no
Stage 4AC/4AE-style split path exists for visible `R_xz`. The fixture covers
zero input, pure `delta h_xz` as a zero oracle, an even-sector diagonal scalar
mode, a radial/z mixed hidden mode, and a pure conformal-factor mixed
derivative mode. It uses the same epsilon sweep and requires the `1e-5` to
`1e-6` plateau. The one-z parity projection gives nonzero allowed sine output
and near-roundoff forbidden cosine leakage. Negative guards show that wrong
mixed-derivative sign, dropped conformal-factor mixed derivative, hidden
multiplicity applied to visible `R_xz`, spurious `h_xz` contribution, or wrong
parity assignment would fail. This component-local fixture does not itself
implement `delta R_xx`, `delta R_zz`, the globally completed A/Theta Ricci
insertions, full-operator JVP/parity, MOTS, eigensolver work, or threshold
searches.

The visible physical `delta R_zz[gamma]` fixture is the next raw diagonal
Ricci implementation substep. It checks only the raw lower/lower visible
`zz` component on the locked flat GP spatial background. Stage 4G is again
the primary and sole independent nonlinear oracle because no Stage
4AC/4AE-style split path exists for visible `R_zz`. The fixture documents the
positive conformal-factor signs implied by `gamma=h/chi` and covers zero
input, pure `delta h_zz`, one-z `delta h_xz` with mixed derivative and
`1/x` contribution, z-dependent `delta h_ww`, mixed `delta chi`,
z-dependent `delta h_xx`, and mixed radial/z coverage. It uses the same
epsilon sweep and requires the `1e-5` to `1e-6` plateau. Even-sector parity
checks show scalar/diagonal inputs and one-z `h_xz` inputs both produce even
`R_zz` output with near-roundoff forbidden sine leakage. Negative guards show
that wrong `partial_xz(delta h_xz)` sign, dropping
`2 partial_z(delta h_xz)/x`, dropping `-partial_x(delta h_zz)/x`, dropping
`-partial_zz(delta h_ww)`, flipping the positive `chi` signs, hidden
multiplicity applied to visible `R_zz`, wrong parity, or treating raw `R_zz`
as already trace-free would fail. This component-local fixture does not itself
implement `delta R_xx` or the globally completed A/Theta Ricci insertions;
full-operator JVP/parity, MOTS, eigensolver work, and threshold searches remain
missing.

The visible physical `delta R_xx[gamma]` fixture is the raw radial Ricci
implementation substep. It checks only the raw lower/lower visible `xx`
component on the locked flat GP spatial background. Stage 4G is again the
primary and sole independent nonlinear oracle because no Stage 4AC/4AE-style
split path exists for visible `R_xx`. The fixture documents the positive
conformal-factor signs implied by `gamma=h/chi` and covers zero input, pure
`delta h_xx`, pure `delta h_zz`, pure `delta h_ww`, one-z `delta h_xz`,
mixed `delta chi`, z-dependent diagonal scalar input, and mixed radial/z
coverage. It uses the same epsilon sweep and requires the `1e-5` to `1e-6`
plateau. Even-sector parity checks show scalar/diagonal inputs and one-z
`h_xz` inputs both produce even `R_xx` output with near-roundoff forbidden
sine leakage. Negative guards show that wrong `partial_xz(delta h_xz)` sign,
dropping a radial `1/x` term, dropping conformal-factor terms, hidden
multiplicity applied to visible `R_xx`, wrong parity, or treating raw `R_xx`
as already trace-free would fail. This component-local fixture does not itself
perform the globally completed raw trace/free assembly or A/Theta Ricci
insertions; full-operator JVP/parity, MOTS, eigensolver work, and threshold
searches remain missing.

The raw Ricci trace / trace-free assembly fixture consumes the validated raw
`delta R_xx`, `delta R_xz`, `delta R_zz`, and `delta R_ww` result types and
assembles only
`delta R = delta R_xx + delta R_zz + 2 delta R_ww` plus the `d=4`
trace-free components. `R_xz` is not included in the scalar trace, and
`R_xz^TF` equals raw `R_xz` because `h_xz_GP=0`. The fixture independently
central-differences Stage 4G raw components and forms the oracle trace/free
projection from those oracle components over an epsilon sweep. It covers zero
input, pure `delta h_xx`, pure `delta h_zz`, pure `delta h_ww`, pure
`delta h_xz`, pure `delta chi`, mixed radial/z scalar data, and parity cases.
Negative guards show that dropping hidden multiplicity two, adding `R_xz` to
the trace, using `d=3`, projecting `R_xx` before full assembly, or treating
the result as an A-curvature or Theta RHS insertion would fail. This assembly
fixture does not itself perform those insertions; the separate A and Theta
insertion fixtures now do. It does not implement the full operator or
eigensolver gate.

The GP-shift advection fixture checks the first actual matrix-free partial
operator block:

```text
L_adv[delta u] = beta_GP^x d_x(delta u),
beta_GP^x = sqrt(r0/x),
```

applied to all 13 perturbation variables on interior radial grid points. It
uses second-order centered radial `d_x` on a quadratic test profile, records
that boundary outputs are placeholders rather than boundary validation, and
checks periodic second-order `D_z`/`D_zz` scaffolding on `sin(z)`. Negative
guards show that a wrong beta profile, wrong derivative sign, or wrong slot
ordering would fail. This fixture still does not include Ricci, hidden
cartoon sources, K/A couplings, Theta constraints, hatted-Gamma evolution,
boundary validation, eigensolver work, or threshold searches.

The tensor shift-stretching fixture checks the non-advection tensor part of
the GP-shift Lie derivative for `h_IJ` and `A_IJ`:

```text
delta T_ik partial_j beta^k
+ delta T_jk partial_i beta^k
- (1/2) delta T_ij div beta,
```

with the hidden `ww` cartoon stretch `+2 delta T_ww beta^x/x`. On the locked
GP background the fixture checks the coefficients
`-7 lambda/4`, `-5 lambda/4`, `-3 lambda/4`, and hidden `+5 lambda/4` for
the `xx`, `xz`, `zz`, and `ww` metric/A slots. Scalars and hatted-Gamma slots
must receive zero from this block. Negative guards show that dropping the
hidden `ww` stretch, using a `d=3` trace coefficient, flipping the sign of
`partial_x beta^x`, or stretching scalar/hatted-Gamma slots would fail. This
fixture still does not include K/A algebraic dynamics beyond the separate
metric/chi coupling fixture, Ricci, constraints, hatted-Gamma evolution, MOTS,
eigensolver work, or threshold searches.

The algebraic metric/chi coupling fixture checks the next local GRChombo
kinematic convention block on the locked GP background:

```text
delta h_IJ <- -2 delta A_IJ,
delta chi  <- +(1/2) delta K.
```

The `1/2` coefficient is the `2/d` factor with `d=4`,
`alpha_GP=1`, `chi_GP=1`, and `K_GP=div beta_GP`. The fixture confirms no
reciprocal `A_IJ <- h_IJ` coupling is applied and that `K`, `Theta`, `A_IJ`,
and hatted-Gamma outputs remain zero for this block. Negative guards show that
the wrong metric sign, a `-1` metric coefficient, a `d=3` chi coefficient,
reciprocal `A_IJ` coupling, or accidental scalar/hatted-Gamma output would
fail. This fixture still does not include remaining K/A algebraic dynamics,
Ricci, constraints, hatted-Gamma evolution, MOTS, eigensolver work, or
threshold searches.

The selected-CCZ4 K fixture supersedes the earlier BSSN-style fixture. The
former `A_IJ A^IJ + K^2/d` row, including its inverse-metric and `delta A_IJ`
coefficients, is rejected historical evidence and no longer exists in the K
operator. The fixture checks only

```text
F_K = R + K(K - 2Theta),
output[K] += 3 lambda input[K],
output[K] += -3 lambda input[Theta],
output[K] += delta R_xx + delta R_zz + 2 delta R_ww.
```

Its oracle central-differences the nonlinear Stage 4G physical Ricci scalar
plus independently perturbed nonlinear `K(K-2Theta)` about
`K_GP=3 lambda/2`, `Theta_GP=0`. The epsilon sweep is `1e-2`, `1e-4`, `1e-5`,
`1e-6`, and `1e-7`; the `1e-5` to `1e-6` plateau must be below `2e-7`.

Branch cases require pure `delta K` to give `+3 lambda delta K`, pure
`delta Theta` to give `-3 lambda delta Theta`, pure `delta A` to give zero,
metric/chi and pure `delta h_xz` jets to enter only through the Ricci scalar,
and a combined perturbation to agree with the nonlinear CCZ4 finite
difference. A separately central-differenced nonlinear BSSN expression is a
negative oracle, and every case must distinguish the selected branch from it.
Z/hat-Gamma-dependent Ricci terms, hat-Gamma evolution, the full operator,
MOTS, and spectral work remain missing. The simple K/Theta damping insertion
is tested separately. Frozen-gauge lapse-Hessian variation vanishes; locked
`Lambda=0` leaves no cosmological term.

The CCZ4 damping fixture independently derives the coefficients from
`kappa1=0.1`, `kappa2=0`, and `d=4`:

```text
delta rhs_Theta|kappa = -0.5*kappa1*(5+3*kappa2)*deltaTheta,
delta rhs_K|kappa     = -4*kappa1*(1+kappa2)*deltaTheta.
```

It checks `-0.25 deltaTheta` in the Theta output and `-0.4 deltaTheta` in the
K output, with no `deltaK`, hatted-Gamma, or other output contribution. It
also confirms the corrected CCZ4 K block remains separate and unchanged.
Genuine wrong-sign mutation guards compare the actual coefficients with the
independently derived negative damping values and reject their positive-sign
antidamping mutations. Further guards cover `d=3`, wrong nonzero-`kappa2`
dependence, spurious `deltaK`, spurious hatted-Gamma writes from the K/Theta
block, non-K/Theta output writes, and false K/Theta/full-operator completion.
The main path
deliberately locks `kappa3=1`, `covariantZ4=true`; zero damping remains a later
diagnostic.

The contracted-connection fixture checks the validation-only helper

```text
g_x = 0.5 dx(h_xx) - 0.5 dx(h_zz) - dx(h_ww)
      + dz(h_xz) + 2(h_xx-h_ww)/x,
g_z = dx(h_xz) + 2 h_xz/x
      - 0.5 dz(h_xx) + 0.5 dz(h_zz) - dz(h_ww),
Z_i = 0.5(H_i-g_i).
```

It does not assume the determinant constraint and produces no Gamma RHS
output. A central epsilon sweep finite-differences the Stage 4AN nonlinear
full `x` contraction and an independently coded nonlinear full `z`
contraction. Pure `h_ww`, `h_xz`, diagonal metric, nonzero hatted-Gamma, and
zero cases are covered. Negative guards reject hidden multiplicity one and
wrong derivative signs. The determinant-reduced identities are checked only
on a trace-certified perturbation and explicitly rejected as a general
identity. Fourier projections require `g_x` in the scalar/even sector and
`g_z` in the one-z/opposite-parity sector. The helper itself still writes no
Gamma RHS. Connection-A, vector-Hessian, K/Theta/chi-gradient, complete-RHS,
and eigensolver claims remain false.

The first hatted-Gamma RHS fixture independently derives

```text
K0 = 3 lambda/2,
c_g = 2 K0/d = 3 lambda/4,
c_Z = (4 K0/d)(kappa3-1) - 2 kappa1 = -0.2,
c_Hx = lambda/2,
```

from `d=4`, `kappa1=0.1`, and `kappa3=1`, then validates only

```text
output[hat_Gamma^x] += c_g g_x + c_Z Z_x + c_Hx H_x,
output[hat_Gamma^z] += c_g g_z + c_Z Z_z.
```

Pure `H_x/H_z`, pure metric through `g_x/g_z`, and explicit Z reconstruction
cases are covered. Mutation guards reject an extra `+(lambda/2)H_z`, positive
Z antidamping, hidden multiplicity one inherited through `g_i`, and duplicate
`beta_GP^x partial_x H_i` advection. All non-Gamma outputs stay zero, the
scalar/even and one-z parity sectors remain separate, and both Gamma-variable
completion flags, the full-operator flag, and eigensolver access remain false.
Connection-A, vector/shift-Hessian, K/Theta/chi-gradient, and complete row
assembly are outside this fixture.

The A-equation algebraic non-curvature fixture checks the GRChombo A RHS
convention block

```text
(K - 2 Theta) A_IJ - 2 h^KL A_IK A_LJ
```

linearized about the locked GP background, with `h^KL` supplied by the
conformal inverse metric. The fixture locks the A-output coefficients

```text
A_xx: (49/32) lambda^2 delta h_xx
      + 5 lambda delta A_xx
      - (7/8) lambda delta K
      + (7/4) lambda delta Theta,

A_xz: (21/32) lambda^2 delta h_xz
      + 4 lambda delta A_xz,

A_zz: (9/32) lambda^2 delta h_zz
      + 3 lambda delta A_zz
      - (3/8) lambda delta K
      + (3/4) lambda delta Theta,

A_ww: (25/32) lambda^2 delta h_ww
      - lambda delta A_ww
      + (5/8) lambda delta K
      - (5/4) lambda delta Theta.
```

It confirms inverse-metric variation is present, no spurious `K` or `Theta`
coupling appears in `A_xz` because `A_xz_GP=0`, the `A_ww` component equation
does not get an extra hidden-multiplicity factor, and no non-A output slot is
touched. Negative guards show that dropping inverse-metric variation, adding
hidden multiplicity to the `A_ww` component equation, using wrong `K` or
`Theta` coefficients, adding spurious `A_xz` couplings, or touching non-A
outputs would fail. This fixture itself does not include the separately
implemented trace-free Ricci insertion or the frozen-gauge-vanishing lapse
Hessian; remaining A dynamics, Theta/constraint terms, hatted-Gamma evolution,
MOTS, eigensolver work, and threshold searches remain missing.

The Theta-equation algebraic non-Ricci fixture checks only

```text
0.5 * (((d - 1) / d) K^2 - A_IJ A^IJ)
```

from the GRChombo CCZ4 Theta RHS, linearized about the locked GP background.
It deliberately excludes Ricci scalar terms, `-Theta K`, simple kappa damping,
`Z dot grad alpha`, and cosmological terms. The fixture locks the Theta-output
coefficients

```text
(+49/64) lambda^2 delta h_xx,
( +9/64) lambda^2 delta h_zz,
(+25/32) lambda^2 delta h_ww,
( +7/8 ) lambda   delta A_xx,
( +3/8 ) lambda   delta A_zz,
( -5/4 ) lambda   delta A_ww,
( +9/8 ) lambda   delta K.
```

It confirms inverse-metric variation is present, hidden `ww` trace
multiplicity two is included, `delta h_xz` and `delta A_xz` give zero on the
diagonal GP background, and no non-Theta output slot is touched. Negative
guards show that dropping inverse-metric variation, dropping hidden `ww`
multiplicity, using a `d=3` K coefficient, adding spurious `xz` couplings, or
touching non-Theta outputs would fail. The Ricci scalar and simple kappa
damping contributions are implemented in separate fixtures; remaining
constraint terms, hatted-Gamma evolution, MOTS, eigensolver work, and
threshold searches remain missing.

The Theta minus-K fixture checks the remaining simple non-damping algebraic
Theta factor in the GRChombo convention,

```text
0.5 * (-2 Theta K) = -Theta K.
```

On the locked GP background `Theta_GP=0` and `K_GP=3 lambda/2`, so this
substep implements only

```text
delta Theta_RHS <- -(3 lambda / 2) deltaTheta.
```

There is no `delta K` term from this block because `Theta_GP=0`. The fixture
checks the pointwise and grid apply paths, confirms no non-Theta output is
touched, and keeps `complete_operator=false` and `eigensolver_allowed=false`.
Negative guards show that the wrong sign, a `-lambda` coefficient, a spurious
`delta K` contribution, or touching non-Theta outputs would fail. The locked
simple kappa damping insertion remains separate and is checked by its own
fixture.

The trace-free `delta A` projector fixture locks the structural subspace used
by the future assembled frozen-gauge operator. On the locked GP background it
checks

```text
delta_tr_A =
    delta A_xx + delta A_zz + 2 delta A_ww
  + (7 lambda / 8) delta h_xx
  + (3 lambda / 8) delta h_zz
  - (5 lambda / 4) delta h_ww.
```

The fixture verifies that `h_xz` and `A_xz` do not contribute at the diagonal
background, then applies the validation-only projector

```text
delta A_xx <- delta A_xx - delta_tr_A / 4,
delta A_zz <- delta A_zz - delta_tr_A / 4,
delta A_ww <- delta A_ww - delta_tr_A / 4.
```

This preserves `A_xz` and all non-A variables, removes the linearized trace,
is idempotent, and leaves already trace-free data unchanged. Negative guards
show that wrong hidden multiplicity, dropping metric-variation terms, a
`d=3`/three-component projection, or accidental non-A modification would fail.
This is a validation-only assembly contract, not final production trace
handling.

The Ricci/curvature design preflight has no executable fixture yet. It records
the GRChombo `CCZ4Geometry` Ricci convention, the frozen-gauge simplification
`delta(D_I D_J alpha)=0`, the intended
`delta R_IJ = delta tilde R_IJ + delta R^chi_IJ + hidden/cartoon terms`
decomposition, and the oracle plan based on finite-differencing the Stage 4G
metric-derivative Ricci engine and the reviewed Stage 4AC/4AE/4AH hidden
`R_ww` path. The recommended first Ricci fixture is a narrow hidden
`delta R_ww[gamma]` block with a nonlinear finite-difference oracle. No
visible Ricci component, trace-free curvature source, full operator JVP, or
spectral fixture is implemented by the design preflight.

The future fixture must freeze gauge perturbations,

```text
delta alpha = delta beta^i = delta B^i = 0,
```

while evolving the coupled perturbation vector

```text
delta chi,
delta h_xx, delta h_xz, delta h_zz, delta h_ww,
delta K,
delta A_xx, delta A_xz, delta A_zz, delta A_ww,
delta Theta,
delta hat_Gamma^x, delta hat_Gamma^z.
```

It must recheck parity and Jacobian consistency on the actual frozen-gauge
operator, not on the representative 4AO-B harness. It must also lock radial
spectral boundary conditions, use a targeted spectral method such as
shift-invert, map candidates to the linearized horizon-radius observable
`delta R_H`, and demonstrate unstable/stable points, a threshold estimate,
radial convergence, and boundary-location convergence.

`code/BlackStringToy/tests/Stage4AOCFrozenGaugeThetaRicciScalarBlockTest.cpp`
checks the next narrow Theta RHS insertion. It consumes the validated Ricci
trace assembly and verifies only

```text
delta Theta_RHS += 0.5 delta R,
delta R = delta R_xx + delta R_zz + 2 delta R_ww.
```

The fixture confirms the positive sign, hidden `ww` multiplicity two,
absence of `R_xz` in the scalar trace, no non-Theta output writes, and the
same incomplete-operator/eigensolver guards. Negative guards show that wrong
sign, missing `0.5`, dropped hidden multiplicity, spurious `R_xz`, touching
non-Theta slots, or claiming a complete Theta RHS would fail.

`code/BlackStringToy/tests/Stage4AOCFrozenGaugeARicciCurvatureBlockTest.cpp`
checks the next narrow A-equation curvature insertion. It consumes the validated
trace-free Ricci assembly and verifies only

```text
delta A_xx_RHS += [delta R_xx]^TF,
delta A_xz_RHS += [delta R_xz]^TF,
delta A_zz_RHS += [delta R_zz]^TF,
delta A_ww_RHS += [delta R_ww]^TF.
```

The fixture confirms the GRChombo frozen-gauge simplification
`delta(D_I D_J alpha)=0`, the prefactor-one insertion from
`delta(chi alpha R_IJ)=delta R_IJ`, no non-A output writes, no extra hidden
multiplicity on representative `A_ww`, and the trace-free source condition
`A_xx + A_zz + 2 A_ww = 0`. Negative guards show that wrong sign, raw Ricci
instead of trace-free Ricci, a `d=3` projection, double-counted hidden
multiplicity, touching non-A slots, or claiming a complete A RHS would fail.

No actual spectral fixture is added yet because the full modified-cartoon CCZ4
frozen-gauge RHS linearization is still missing beyond GP-shift advection,
tensor shift-stretching, the local algebraic metric/chi coupling, and the
selected-CCZ4 K-output `K(K-2Theta)` and physical-`delta R` blocks, plus the A-output non-curvature algebraic
block, the simple K/Theta damping insertion, Theta-output algebraic non-Ricci block, Theta-output
`-K_GP deltaTheta` block, trace-free `delta A` projector contract, raw Ricci
component blocks, raw trace/free assembly, and the Theta Ricci scalar
insertion, plus the A-output trace-free Ricci curvature insertion. A toy or
calibrated spectral operator would not be an honest GL gate.

These are not substitutes for unit tests. They catch coupled failures that
small algebra tests cannot catch, and several require the future implementation
or external/reference-code comparison.

| Test name | Stage source | Type | Input data | Expected output | Exactness | Catches | Does not catch | Required before Stage 3K/C++? | Convention / validation note |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Flat/Minkowski zero RHS | 3H | RHS-block/evolution | `chi=1`, flat `h`, zero `A_ij`, `K=0`, zero shift or documented gauge | RHS vanishes or reduces to known pure-gauge terms | Exact zero or gauge-specified tolerance | Gross source-term errors, nonzero hidden flat residues | Constraint-damping signs; many nonlinear terms are inactive | No before planning; required before long runs | Gauge convention yes |
| Sheared-flat zero Ricci/source check | 3G/3H | RHS-block | Sheared-flat metrics from Stage 3G Ricci gate | Geometry source terms vanish in flat pullback cases | Exact zero or tight tolerance | Off-diagonal geometry mistakes in source assembly | Non-flat Ricci and physical black-string behavior | No before planning; required before implementation trust | No for geometry, yes for source block placement |
| Gauge-fixed uniform GP string stationarity | 3A/3H | Evolution/RHS-block | Unperturbed uniform black string outside turduckening region with chosen gauge | RHS vanishes or is pure gauge, and constraints remain controlled outside cutoff | Tolerance-based | Coupled metric, `K`, `A_ij`, gauge, and hidden-source errors | Interior turduckening correctness, GL growth spectrum | No before planning; required before production | Yes: gauge, branch, sign, and reference comparison |
| Uniform Schwarzschild/black-string anchor - slicing-dependent | 3A/3H | Symbolic geometry / evolution / future spacetime check | For a GP-type slice, the spatial metric is the flat GP spatial metric times `S^1` and the nontrivial black-hole geometry is carried by `K_ij`; alternatively, a future spacetime-Ricci engine may check Schwarzschild_4 x `S^1` | GP spatial Ricci vanishes for the GP spatial metric times `S^1`; full spacetime Ricci flatness is out of scope for the current spatial-cartoon fixture layer until a spacetime engine exists | Exact zero for the GP spatial geometry; future reference/spacetime check for full Ricci flatness | Confusing spatial-slice Ricci with spacetime Ricci, careless Schwarzschild x `S^1` assumptions, GP geometry regressions | Does not validate arbitrary Schwarzschild slicings, full CCZ4 RHS, or turduckening | No before planning; required before trusting uniform-string anchors | Yes: slicing, sign, gauge, and variable conventions |
| Stage 4AO physical-sector linear GL dispersion/growth gate | 3A/3H/4AM-4AO | Front-loaded 4AO-A through 4AO-D validation sequence, not one executable gate | Linear perturbations around a uniform string after matching radius convention, `z` periodicity, perturbation sector, gauge choice, measured growth variable, and slicing/conformal-variable convention | 4AO-A locks the background/residual, 4AO-B locks the discrete preflight, 4AO-C locks the frozen-gauge spectral gate, and 4AO-D locks live-gauge/full acceptance | Semi-analytic/reference-based | Physical linearized RHS mistakes in metric, `K`, `A_ij`, `hat_Gamma^x`, gauge/source coupling, and cartoon-sector terms; replaces the independent oracle that protected the Ricci stages | Full nonlinear dynamics; because the GL mode is constraint-satisfying in the continuum, it is necessary but not sufficient for damping signs | Production Stage 4AR/4AS RHS/evolution integration is blocked until 4AO-D; dedicated 4AO-B/C/D validation harnesses may exercise the RHS, spectral systems, and seeded-eigenvector evolution bridge | See "Stage 4AO GL Gate Fixture Sequence"; Pau is not the convention authority; anchor conventions must be matched explicitly |
| Constraint-violation damping injection | 3H | RHS-block/evolution | Linearized flat or uniform-string data with controlled Hamiltonian, momentum, `Theta`, and encoded-`Z^A` violations through `hat_Gamma^A = tilde_Gamma^A + 2Z^A` | Prefer derived/documented linearized CCZ4 constraint-subsystem decay rates and signs for the chosen `kappa_1`, `kappa_2`, gauge, and background; otherwise mark as high-risk reference/formulation-derived | Formulation-derived, reference-based, and tolerance-based | Wrong damping sign, wrong `kappa_1`/`kappa_2`, missing hidden multiplicity, wrong `/4` bookkeeping, wrong `hat_Gamma^A` coupling to encoded `Z^A` | Physical GL growth correctness and nonlinear source-term completeness | No before planning; required before trusting the constraint-damping block | Yes, required |
| External term-by-term comparison | 3H/Stage 4 | Reference-comparison | Shared local fields and gauge data for each decomposed RHS block | Project RHS terms match an external implementation after mapping from the GRChombo-facing project conventions | Reference-based | Term omissions, sign mistakes, hidden-sector source drift | Bugs shared with reference or mismatched setup | No before planning; required for high-risk blocks | Validation only, not convention authority |
| External trajectory-level comparison | 3H/Stage 4+ | Reference-comparison/evolution | Short controlled runs with shared parameters | Diagnostics follow an external/reference trajectory within documented tolerance | Reference/tolerance-based | Integration-level coupling errors | Isolated source-term attribution | No before planning; required before physics interpretation | Validation only, not convention authority |
| Resolution self-convergence | 3H/Stage 6 | Convergence | At least three resolutions for controlled fixtures and later black-string runs | Expected convergence rate for constraints, RHS residuals, horizons, and modes | Convergence-based | Discretization and implementation coupling errors | Convergence to wrong continuum equations if source terms are wrong | No before planning; required before claims | No for method, yes for acceptance thresholds |

## Pre-C++ Gate List

The following gates should be true before Stage 3K turns this design into an
implementation plan or before C++ source-term work begins:

- All existing symbolic scripts from Stages 3C-3I pass.
- No stale stage-label or old conformal-cartoon filename references remain.
- The determinant condition, hidden `hww` participation, full 4D trace, and
  `/4` denominator conventions are documented.
- The `hat_Gamma^A = tilde_Gamma^A + 2Z^A` convention is documented, including
  the fact that `Z^A` is encoded rather than the primary evolved connection
  variable.
- Small-`x` risks, cartoon-axis parity, the co-located physical singularity,
  and turduckening/interior regularization are documented as distinct issues.
- The unresolved GRChombo-facing convention choices below are listed
  explicitly and carried into Stage 3K.
- Each future C++ source block has a proposed exact, reference, convergence, or
  evolution-level validation route.

## C++ Unit-Test Design Principles

- Test small algebra/source blocks, not the entire CCZ4 RHS at once.
- Isolate hidden-multiplicity factors in tests where possible.
- Pair every off-diagonal implementation with a diagonal-limit regression.
- Use both regular and deliberately irregular Taylor data for axis-limit tests.
- Treat flat data as necessary but insufficient, especially for
  constraint-damping signs.
- Compare source blocks against symbolic fixtures before comparing full
  trajectories.
- Do not claim agreement from compilation, executable creation, or HDF5 output
  alone.
- Keep reference-code comparisons explicit about convention maps and expected
  tolerances.

## Required Linearized Milestones

Two linearized milestones must remain separate:

1. Physical-sector GL dispersion: the linearized RHS around the uniform 5D
   black string should reproduce the Gregory-Laflamme threshold/growth spectrum
   in the SO(3)-symmetric sector after matching radius convention, periodic
   boundary condition in `z`, perturbation sector, gauge choice, extraction
   variable, and slicing/conformal-variable convention. This validates the
   physical linearized RHS coupling of metric, `K`, `A_ij`, gauge, and
   cartoon-source terms.
2. Constraint-violation damping injection: controlled linearized violations
   should be injected around flat or uniform-string data, then `Theta`,
   encoded `Z^A`, the Hamiltonian constraint, and the momentum constraints
   should be tracked. This is the non-vacuous damping-sector test. If feasible,
   derive or document the expected linearized CCZ4 constraint-subsystem decay
   rates/signs for the chosen `kappa_1`, `kappa_2`, gauge, and background;
   otherwise this fixture remains high risk and reference/formulation-derived.

The GL mode is expected to be constraint-satisfying in the continuum. It is
therefore necessary for the physical sector but not sufficient for validating
constraint-damping signs or `kappa_1`, `kappa_2` behavior.

## Known Unresolved Convention And Validation Items

- Document how the project enforces conformal determinant and trace-free
  conditions in the GRChombo-facing variable set when hidden `hww`
  participates.
- Derive and document the exact `tilde_Gamma^x` sign and full
  `hat_Gamma^A` convention in the GRChombo-facing cartoon extension, including
  raised/lowered `Z` handling.
- Document Gamma-driver and `hat_Gamma^A` coupling ownership in the
  GRChombo-facing implementation.
- Gauge choice for uniform-string stationarity checks.
- Radius, periodicity, perturbation-sector, gauge, and extraction-variable
  conventions for the GL dispersion comparison.
- Which external/reference comparisons can be term-by-term and which are only
  reliable at the trajectory or diagnostic level.

## Non-Goals

Stage 3J does not:

- implement C++ tests;
- implement CCZ4 source terms;
- implement finite-difference stencils or axis ghost filling;
- choose final production tolerances;
- settle all GRChombo-facing convention details or external validation
  mappings;
- replace the Stage 3K minimal C++ implementation plan.

## Acceptance Criteria

Stage 3J is complete when:

- this unit-test fixture design document exists;
- every validated symbolic gate from Stages 3C-3I is mapped to future test
  usage;
- required pre-C++ gates are identified;
- symbolic, C++ unit, RHS-block, reference, evolution, and convergence tests
  are clearly separated;
- unresolved convention confirmations are tracked;
- no implementation code has changed.
