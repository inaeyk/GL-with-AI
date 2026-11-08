# Full-window candidate and directed checks

All calculations here are fixed-background, non-production calculations
for the ordered hierarchy. Inputs are the GOAL-014 source
[0b4bfcbb978b52626fb0816874df06055703c195](https://github.com/inaeyk/GL-with-AI/commit/0b4bfcbb978b52626fb0816874df06055703c195)
and the read-only scientific audit
[84c9824f826e3517c8a1ed74d96bea061ab88584](https://github.com/inaeyk/GL-with-AI/commit/84c9824f826e3517c8a1ed74d96bea061ab88584).
The source and graph maps are those of
[GOAL-013](../../GLR-GOAL-013/SOURCE-LEDGER.md) and the
[GOAL-014 compact pilot](../../GLR-GOAL-014/STRONG-NEWS-PILOT.md).
The inherited early GOAL-015 coefficient and graph interface is a
read-only precedent. No GOAL-015 pilot values or early strong errors
are used for the later window.

## Candidate equations and geometry

Use dimensionless null coordinates $u=t-x$, $v=t+x$ with
$x=r+\log(r-1)$. The numerical domain starts at the physical front
$a=-13/8-2\log(5/8)$ and ends at $d=8.407362$. The final graph
radius is about $1.0090210771$, and the cap $v=d+2x(400)$ is about
$820.3852848338$. The node calculation uses binary64 approximations to
these coordinates. Directed late-time interpolants bind the first and
last comparison abscissae to the exact decimals $0$ and $8.407362$.
A future compact certificate must account for the binary interior nodes,
curved graph and cap geometry explicitly.

The march stores each field $W$ and its two null derivatives. The known value on the old edge plus an
integral of the local drive gives each derivative at the next node. A
short right-end polynomial stencil integrates along each ray. It uses a
trapezoid for the first interval after a graph/front edge. If $d_{uv}$
is the new drive, the two derivative updates have the form
$P=P_b+B_v d_{uv}$ and $W_v=Q_b+B_u d_{uv}$. Averaging the two line
integrals of the field gives $W=W_b+B_u B_v d_{uv}$. Substitution in
$4d_{uv}=-V_m W+C(P+W_v)-S$ yields a local scalar or real $2\times2$
implicit solve. Here $C$ is the physical skew coupling of the two
mixed polarizations. The graph and front are inserted before interior
updates. This local solve avoids extending the early global Picard
iteration over the long massive strip. The manufactured polynomial
fixture independently supplies exact graph/front values and a forced
two-polarization solution. It tests the update, including the skew
signs, without testing the physical source.

The leading scalar is solved together with its first two stationary
time derivatives. Analytic bump jets and the tilted graph identities
give their graph data. The mixed fields use their induced graph
velocity $g$, with $Y|_\Sigma=0$, $Y_x=-p g$ and
$g_x^{\Sigma}=d_\Sigma g/dx$, where $p=-1/r$. For the shifted
field $(\partial_t+\Omega)Y$ the displacement is $g$, and
\[
 Y_{tt}|_\Sigma
 =\frac{-p_xg-2p g_x^{\Sigma}+C g-S|_\Sigma}{1-p^2},\qquad
 Y_{tx}|_\Sigma=g_x^{\Sigma}-pY_{tt}|_\Sigma .
\]
In the code $d_\Sigma g/dx$ includes the explicit $p_x$ derivative
of the constraint equations, $J_x+pJ_t$ and the derivatives of both
stationary rescalings. This shift is driven by $(\partial_t+2\Omega)w_0$,
because the first insertion carries $e^{\Omega t}$. The returning
currents include the two mixed polarizations, affine $J$ contribution,
direct quadratic GL term and the fixed-mass Einstein-frame mean.
The GL and mean candidate values use the pinned numerical IVPs and the
finite exterior endpoint $430$. Their sample mismatch and tail proxy
are diagnostics. They do not bound whole-cell physical coefficient errors.

The returning scalar shear uses the causal weak Volterra equation in
[GOAL-013, Eq. (2)](../../GLR-GOAL-013/FINITE-CUT-BOUNDS.md).
The numerical shear-spline slope and the evaluation with
$\partial_u\int_b^c F\,dv$ are two readouts of the same weak shear
and use the same lower fields. A third readout uses the exact identity
\[
 4w_u=G_c-F_c+(1-b')F_\Sigma+\int_b^c(F_t-H-Vw)\,dv.
\]
It computes $F_t$ using the separately shifted lower hierarchy.
The physical source beyond the cap is dropped in the candidate. The
sampled cap trace is continued with the homogeneous potential in
$1/r(d,v)$ using DOP853. The inherited physical tail and the directed
operator estimates in [Strong news](../analysis/STRONG-NEWS.md) are
distinct from the numerical continuation. A directed continuation
quadrature error $E_q$ for these affine candidate data remains open.

## Observed diagnostics

| Grid $(N_{\rm packet},N_{\rm late},N_{\rm radial};N_{\rm cluster})$ | Empirical cap disagreement, shifted-current vs shear-slope | Directed $\|n_{21}\|_2$ for the continued shear-slope affine data |
| --- | ---: | ---: |
| $(64,80,180;0)$ | $6.139401$ | $0.09633399975$ |
| $(128,240,360;0)$ | $0.7995979$ | $0.08757482002$ |
| $(256,480,720;0)$ | $0.1025900$ | $0.08742919689$ |
| $(256,480,720;768)$ | $0.02840983$ | $0.08741378901$ |

The last grid replaces the initial radial mesh through $v\simeq12$
with a logarithmically stretched finer patch in $v$, starting at the
front-graph corner. The continued shear-slope traces on the last two
meshes differ by about $0.00043006$ in sampled $L^2(I)$. The shifted
$F_t$ formula has a much larger numerical cancellation. On the
clustered mesh $\|\int |(F_t)_h|\,dv\|_2$ has an upper bound
$6763.448$ for the bilinear candidate, while the same directional
norm for $\partial_u F_h$ is at most $3.231042$. The signed
$\|\int (F_t)_h\,dv\|_2$ is about $0.142197$ for the
bilinear data. These results use Arb on exact dyadic ordinates.
They concern the respective candidate norms, which bound no physical
current error by themselves.

The late bilinear current lift is continuous at aligned seams, with
piecewise ordinary $\partial_u F_h$. It yields a separately integrated
cap trace $p_{\rm bilinear}$ by affine integration in $v$. The
$H+Vw$ term uses its own bilinear model of the nodal sum. The
result differs from the shear-slope cap affine candidate by less than
$1.59530\times10^{-4}$, and from the continued shear-slope affine
candidate by less than $8.71794\times10^{-4}$. These are directed
candidate readout differences. They neither bound interpolation error
relative to the physical source nor substitute for the early shear and
continuation terms. The lift is used only on $I$ where a causal
zero collar sits above the entire graph. This leaves full-strip
reconstruction through the launch graph and early causal strip open.

The optional mixed probes use a tensor bicubic Hermite polynomial
with the candidate nodal $Y,Y_u,Y_v,Y_{uv}$, the last computed from the
nodal equation. The Bernstein coefficient maximum gives an outward
whole-cell cap for its residual with bilinear *candidate* potential,
skew and source. Ten selected cells give a quantitative diagnosis of
where the commuted stencil can fail. At $(u,v)\simeq(-0.655,1.626)$
the two uncommuted caps are below $(0.0877,0.0361)$, while the
shifted caps are below $(28.88,15.42)$. At $(0.2,1.610)$ the
corresponding caps are below $(0.003259,0.000862)$ and
$(1.194,1.256)$. The whole-cell probes avoid confusing nodal
equation satisfaction with continuum accuracy. They give no
whole-volume error or physical source enclosure.

The attempted scalar residual reconstruction illustrates this
distinction separately. Extending $U=w_0-\phi(u)$ by zero below
the graph gives $4U_{uv}+VU=-V\phi\,\mathbf1_{v\ge b(u)}$,
with no distributional graph term since $U$ and both of its first
derivatives vanish at the graph. A rectangular tensor Hermite lift
has matching value and first derivative traces by construction.
The exact leading residual is bounded cellwise by the Bernstein
coefficient cap and the variation of $V$. The result using the
unsubtracted nodal march and then subtracting $\phi$ had compact
error bound $0.234611$. Solving for the deviation from the outgoing
bump before constructing the lift reduced this to $0.005693$.
The latter still does not resolve the $0.00503$ late norm on
that coarse mesh, even before the exterior budget. The leading
physical waveform certificate in [Comparison](../spectral/COMPARISON.md)
uses a distinct method.

## Directed template gates for the final candidate

Define $n_{21}$ as the continuous affine interpolation of the
continued shear-slope nodes on $I$ and $n_{01}$ similarly from the
continued leading nodes. The data digest binds the exact binary64
ordinates and abscissae, with the endpoint convention described above.
For the final clustered candidate, Arb integration yields
\[
 \|n_{21}\|_2\simeq0.0874137890067,\quad
 \|n_{01}\|_2\simeq0.00505738094041,\quad
 \langle n_{01},n_{21}\rangle\simeq1.13837692859\,10^{-5}.
\]
The midpoint mismatches are $0.749038754525$ and $1.789273540192$.
Their ratios to the midpoint template norms are $1.018336219$ and
$1.019494130$. For any allowed template $P$,
\[
 \|N_{21}-P\|_2-\tfrac12\|P\|_2\ge
 \|n_{21}-P\|_2-\tfrac12\|P\|_2-E_{21}.
\]
The map on the right before subtracting $E_{21}$ is
$3/2$-Lipschitz in the template $L^2$ norm. Subdivide the
ordinary residue rectangle in $64^2$ boxes and the joint rectangle
in $12^4$ boxes, without assuming any covariance. At each center
use the exact real-pair Gram and candidate-template cross integrals.
Subtract $3/2$ times the directed template radius including the
pole-frequency boxes. The conservative lower bounds on the
right-hand margin before subtracting $E_{21}$ are $0.31731$
for $Q$ and $0.80484$ for $Q+S_*$. Those absolute physical error
ceilings would suffice to reject the corresponding template.
Both physical comparisons remain unresolved.

The fixed-parameter candidate relative signal change is
$17.2844\,\epsilon^2$ in $L^2$ to the displayed order. In the
physical comparison its numerator is the true coefficient norm
$\|N_{21}\|_2$, which lies between
$\max(0,\|n_{21}\|_2-E_{21})$ and $\|n_{21}\|_2+E_{21}$.
A positive lower bound on the matched difference from this method
needs $E_{21}<0.08741378$, a smaller error than the two
template rejections require. An additional uniform nonlinear
news remainder is necessary to make a finite-amplitude statement.

## Exact missing input and stopping condition

The mesh and readout refinements only reduce sampled numerical
disagreement. For an evaluated absolute $E_{21}$, the free plus
scattered estimator in [Strong news](../analysis/STRONG-NEWS.md)
needs a full-strip conforming return lift, an outward scalar
residual or Volterra defect, the undifferentiated $F,G,H$ error
and early shear endpoint, and the late differentiated ray
$\|\int_b^c(\partial_u F-\partial_u F_h)\,dv\|_{L^2(I)}$.
A sufficient route to the last term uses a weighted
$\partial_u$ lower-drive residual and the compatible seam terms
for $Y_{uu}$, or a certified shifted lower solve using $F_t$.
A conforming mixed lift with compatible physical graph jets and
full-cell coefficients must also bound the uncommuted drive
residual and its propagation on the entire causal strip. The
separate horizon coefficient extension supplies the physical
coefficient interface but still needs comparison against the
candidate on each cell. The observed cancellation and probe
scales favor the outgoing $\partial_u$ route. The compact error,
early shear and $E_q$ must together fit the directed absolute
margin. The available probes and mesh differences leave that
inequality unclosed.

## Commands and dependencies

Run from the pinned worktree's `research/sav` with Python 3.14.8,
NumPy 2.4.6, SciPy 1.18.0, Numba 0.65.1 and python-flint 0.9.0.
All commands below use one compute thread. The clustered volume is
approximately 73 MiB of scratch data. The large arrays are regenerated.
The pilot and checks here consumed well below 5 CPU minutes.

```sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1 NUMBA_NUM_THREADS=1
g=GLR-GOAL-017/checks
scratch=$(mktemp -d)
python3 "$g/march_control.py"
python3 "$g/late_march.py" --packet 64 --late 80 --radial 180 --output "$scratch/coarse.npz"
python3 "$g/late_march.py" --packet 128 --late 240 --radial 360 --output "$scratch/middle.npz"
python3 "$g/late_march.py" --packet 256 --late 480 --radial 720 --output "$scratch/fine.npz"
python3 "$g/late_march.py" --packet 256 --late 480 --radial 720 --cluster 768 --probes \
  --output "$scratch/cluster.npz" --volume "$scratch/cluster-volume.npz"
for candidate in coarse middle fine cluster; do
  python3 "$g/candidate_gates.py" "$scratch/$candidate.npz" --sharpen
done
python3 "$g/current_readout.py" "$scratch/cluster.npz" "$scratch/cluster-volume.npz"
python3 "$g/candidate_gates.py" "$scratch/cluster.npz" --sharpen --bits 192
python3 "$g/current_readout.py" "$scratch/cluster.npz" "$scratch/cluster-volume.npz" --bits 160
python3 "$g/baseline_bound.py" --direct
python3 "$g/baseline_bound.py"
```

See [archived outputs](RESULTS.txt) and [failed checks](FAILURES.md).
