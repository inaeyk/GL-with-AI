# GL linear master, gauge completion, and bounded continuum benchmarks

Task `T-20261022-GLR-GOAL-001`, 2026-10-22. Independent linear-mode
work package. Exact equations and gauge map checked by direct coordinate
Ricci variation; floating-point eigenvalues are convergence estimates,
not interval-certified enclosures. No production evolution was run.

## Units and invariant amplitude

This note uses $r_0=1$ in the equations; all displayed $r,k,\Omega$ mean
$r/r_0,kr_0,\Omega r_0$. The physical metric is recovered by scaling
coordinates and sphere lengths with $r_0$. Write the real first-order
perturbation as $A(t)H_{AB}(r)\cos(kz)$, with $A=\epsilon p_1e^{\Omega t}$;
there is no extra factor of two in the real cosine amplitude.

For a nonzero complex harmonic $e^{ikz}$, the first-order combination

$$
H^U_{\mu\nu}=h_{\mu\nu}+\frac{i}{k}
 (\nabla_\mu h_{\nu z}+\nabla_\nu h_{\mu z})
 -\frac{1}{k^2}\nabla_\mu\nabla_\nu h_{zz}
$$

is invariant under $h\mapsto h+2\nabla_{(A}\xi_{B)}$. Indeed the unique
nonzero-$k$ gauge generator setting $h^U_{zA}=0$ has covariant components
$\xi_z=ih_{zz}/(2k)$ and
$\xi_\mu=ih_{\mu z}/k-\nabla_\mu h_{zz}/(2k^2)$.
There is no residual nonzero-$k$ generator preserving $h^U_{zA}=0$.
Indices $\mu,\nu$ refer to the four-dimensional Schwarzschild factor.
The $zA$ Einstein equations then impose four-dimensional TT conditions;
TT is a consequence of the full equations, rather than extra discarded
constraints. A nonzero $H^U$ cannot be pure gauge.

Normalize the mode by

$$
\left.\frac{H^U_{\theta\theta}}{2r^2}\right|_{r=2}=1.
$$

Thus $A$ is the first-order fractional SO(3) sphere-radius perturbation
at background areal radius $2r_0$ in the uniquely completed unitary
frame. This is a first-order gauge-invariant normalization, not the
coordinate $\chi$ amplitude of a numerical initial-data ansatz. It is
not claimed to equal the event-horizon bulge amplitude. A second-order
extension of the definition must be stated before using it away from
the single-mode weak-amplitude family.

## Scalar master and exact metric reconstruction

Set $f=1-1/r$ and $D=1+k^2r^3$. The master $y$ obeys

$$
y''+P y'+\left(Q-\frac{\Omega^2}{f^2}\right)y=0,
\quad P=\frac1{r-1}-\frac5r+\frac6{rD},
\quad Q=\frac3{r^2(r-1)}-\frac{k^2r}{r-1}
             -\frac3{r^2(r-1)D}.
$$

A reconstruction satisfying **every** linearized Einstein equation is

$$
H_{tt}=a y+b y',\qquad
a=\frac{(r-1)(1/2-k^2r^3)}{rD^2}-\frac{\Omega^2}{k^2D},
\qquad b=\frac{r-1}{2k^2r^3D}-\frac{3(r-1)^2}{D^2},
$$
$$
H_{tr}=-\frac{\Omega}{k^2D}
  \left(y'-\frac{y}{2r(r-1)}\right),\qquad
H_{\theta\theta}=-\frac{r^2}{2D}y-\frac{r-1}{k^2D}y',
$$
$$
H_{rr}=\frac{H_{tt}}{f^2}
       -\frac{2H_{\theta\theta}}{r(r-1)},\qquad
H_{\varphi\varphi}=H_{\theta\theta}\sin^2\theta,
\qquad H_{zA}=0.
$$

Although $y$ equals $h_{zz}$ in the area-preserving spherical gauge,
it is not the physical zero-mode radion. For $\Omega\ne0$ it can also
be reconstructed from gauge-invariant unitary components:

$$y=2k^2r\left[\frac{r-1}{\Omega}H^U_{tr}
                         -H^U_{\theta\theta}\right].$$

An independent gauge description takes sphere perturbation zero,
$h^S_{tr}=\Omega H_v$, $h^S_{rz}=-ikH_v$, $h^S_{zz}=y$, with

$$
H_v=\frac{r^3}{2D}\left(y'-\frac{y}{2r(r-1)}\right),
\qquad H_r=\frac{H_v}{r(r-1)},
$$
$$
H_t=H_{tt}+\frac{\Omega^2}{k^2}y+ff'H_v
                        -\frac{ff'}{2k^2}y'.
$$

The generator $\xi_t=-\Omega y/(2k^2)$,
$\xi_r=H_v-y'/(2k^2)$, $\xi_z=iy/(2k)$ maps this to the unitary
reconstruction. All seven independent component identities pass exact
rational-function checks. The spherical gauge is generally singular at
the future horizon; the unitary representative with the ingoing branch
is regular there. This is why raw spherical-gauge horizon components
are unsuitable for a physical amplitude norm.

For provenance, the candidate scalar equation and gauge change were
located in [Collingbourne, arXiv:2007.08441](https://arxiv.org/html/2007.08441),
§§3.2, 3.4 and 4.1 (equations 29–31, 84–90, 127–133 in v1).
They were independently checked here against direct orbit Ricci and
sphere Ricci variations, without importing the reference's coupled
component equations. In v1 equations 39–40, some printed denominators
contain $\omega^2r^2+2M$; the equations 32–33 instead imply
$\omega^2r^3+2M$. The formulas retained above use the latter, and are
verified directly. Do not copy the former uncritically.

## Schrödinger form and boundary data

Define $r_*=r+\log(r-1)$ and $y=w u$, $w=D/r$. Direct substitution gives

$$-u_{,r_*r_*}+V_{\rm GL}u=-\Omega^2u,$$
$$V_{\rm GL}=k^2f+\frac{(6r-11)(r-1)}{r^4}
 +\frac{18(r-1)^2}{r^4D^2}
 -\frac{6(4r-5)(r-1)}{r^4D}.$$

For $\Omega>0$, use $u\sim e^{\Omega r_*}$ at the future horizon.
At infinity, $u\sim e^{-q r}r^{-\beta}$, where
$q=\sqrt{k^2+\Omega^2}$ and
$\beta=(k^2+2\Omega^2)/(2q)$. The other branches are excluded.
In ingoing coordinates $v=t+r_*$ the factor
$e^{\Omega t}(r-1)^\Omega=e^{\Omega v}e^{-\Omega r}$ is regular;
the potentially singular tensor combinations cancel by the master
recurrence. At $\Omega=0$ the threshold condition is the log-free
regular horizon solution and spatial decay. The constructions dividing
by $\Omega$ are used only by continuity in that exceptional limit;
$k=0$ requires a separate charge/radion treatment.

The numerical method uses $s=\log(r-1)$ and Riccati variables
$p=u_{,r_*}/u$, $\ell=\log|u|$:

$$p_{,s}=r(V_{\rm GL}+\Omega^2-p^2),\qquad \ell_{,s}=rp.$$

The two endpoint branches are matched at $r=2$ using a scalar root
of $p_L-p_R$. The recovered mode is nodeless. This is a second-order
ODE/one-dimensional root calculation, not a 5D evolution.

Horizon data include $p=\Omega+p_1x+O(x^2)$, $x=r-1$;
$p_1=v_1/(1+2\Omega)$ and $v_1=k^2-5+6/(1+k^2)$.
Infinity data include
$p=-q+a_1/r+a_2/r^2+a_3/r^3$,
$a_1=k^2/(2q)$,
$a_2=(-a_1-6+a_1^2)/(2q)$,
$a_3=(a_1-2a_2+17+2a_1a_2)/(2q)$.
The finite-domain errors are monitored by simultaneous domain and
tolerance refinement, rather than assumed zero.

## Bounded numerical results and limitations

Three predeclared refinements per physical $k$ were used:

| refinement | $s_L$ | $r_R$ | DOP853 relative tolerance | max $\Delta s$ |
|---|---:|---:|---:|---:|
| 1 | -16 | 30 | $2\cdot10^{-8}$ | 0.2 |
| 2 | -22 | 45 | $2\cdot10^{-10}$ | 0.1 |
| 3 | -28 | 65 | $2\cdot10^{-12}$ | 0.05 |

The reference $L/r_0=10$ was completed before the other three values:

| $L/r_0$ | $kr_0$ | $\Omega r_0$, finest | max coarse–fine difference |
|---:|---:|---:|---:|
| 8 | 0.7853981633974483 | 0.0264587018767603 | $1.11\cdot10^{-11}$ |
| 10 | 0.6283185307179586 | 0.06328118897590773 | $1.40\cdot10^{-11}$ |
| 12 | 0.5235987755982988 | 0.08044275775065503 | $1.66\cdot10^{-11}$ |
| 16 | 0.39269908169872414 | 0.09161923083002341 | $1.28\cdot10^{-11}$ |

Fine–medium differences are below $7.1\cdot10^{-14}$; matching residuals
are below $7.0\cdot10^{-15}$. Retain only $\sim8$ decimal places for
conservative scientific use: a suggested estimated absolute uncertainty
is $10^{-8}$, **not a mathematically certified error bar**. It includes a
large margin over observed refinement differences but is not a theorem.
The one separate static-threshold solve gives
$k_{\rm cr}r_0=0.8761604020340054$, residual
$-3.33\cdot10^{-16}$, agreeing with the independently published
$\simeq0.876$ threshold. No wavenumber scan was performed.

The independently published rigorous growth lower bound
$\Omega r_0>1/(20\sqrt{10})\simeq0.01581$ throughout
$0.3\le kr_0\le0.8$ in Collingbourne's convention is satisfied by
all four values. A precise independent **numerical point** benchmark
has not been digitized here: the original
[Gregory–Laflamme 1994](https://arxiv.org/pdf/hep-th/9404071), Figure 3,
uses $r_+=2$, so its plotted $(\mu,\Omega)$ both need multiplication
by two to compare with $(kr_0,\Omega r_0)$. The plot's PDF text was read;
the web screenshot failed. The new digits above must not be described
as independently reproduced published numerical digits.

Raw profile CSVs accompany the JSON results. They contain the master,
derivative, all TT components and the Riccati/log amplitude. The raw
Lorentz contraction is intentionally marked NaN for $r-1<10^{-5}$:
there, subtracting large Schwarzschild components is unreliable.
Use the exact horizon cancellation below or another regular-frame
evaluation, not the NaNs or a spline across them.

## Exact horizon cancellation needed by the quadratic source

Writing $K=H_{\theta\theta}/r^2$, the contraction can be evaluated as

$$\mathcal H=H_{\mu\nu}H^{\mu\nu}
 =2\left(\frac{H_{tt}}f-K-H_{tr}\right)
       \left(\frac{H_{tt}}f-K+H_{tr}\right)+4K^2.$$

For $y\sim a x^\Omega$ the leading $x^{2\Omega-2}$ and
$x^{2\Omega-1}$ terms cancel. The exact remaining coefficient is

$$\frac{\mathcal H}{y^2}\longrightarrow H_0=
\frac{32\Omega^4+32\Omega^3k^2+20\Omega^3+6\Omega^2k^4
 +16\Omega^2k^2+6\Omega^2+5\Omega k^4+8\Omega k^2+2k^4}
 {2k^4(\Omega+1)(2\Omega+1)(1+k^2)^2}>0.$$

This is an exact symbolic cancellation, not an argument to discard
the near-horizon source. To verify it one needs the second Riccati
coefficient

$$p_2=\frac{v_2-p_1^2+p_1}{2+2\Omega},\quad
v_2=-k^2+26-\frac{48}{1+k^2}
                   +\frac{18-18k^2}{(1+k^2)^2}.$$

For a scalar source proportional to $rf\mathcal H$, the ingoing
Laplace-basis overlap at $2\Omega$ has an integrable horizon tail
of order $\delta^{1+4\Omega}$ after a cutoff $x=\delta$.
The positive leading coefficient is insufficient by itself for a
uniform finite-$\delta$ interval bound; its remainder must also be
controlled. The quadratic form in arbitrary $(y,y')$ is not positive
definite everywhere; positivity cannot be inferred from squaring
Lorentzian metric components. The physical eigenfunction and full
retarded projection remain necessary.

## Reproducibility and verification scope

Commands (Python 3.14.8, SymPy 1.14.0, NumPy 2.4.6, SciPy 1.18.0):

```sh
python3 research/sav/verification/gl_linear_check.py
python3 research/sav/verification/gl_linear_mode.py
python3 research/sav/verification/gl_linear_mode.py --extend
```

The exact check passes: spherical-to-unitary gauge map, TT trace,
both nontrivial divergences, all six orbit Ricci components, the sphere
Ricci equation, rejection of an intentional $H_{tr}$ sign mutation,
the Schrödinger transformation, and horizon-contraction cancellations.
It uses exact rational-function arithmetic with formal
$\kappa=ik$; $k^2=-\kappa^2$ avoids approximate complex arithmetic.
The curvature pathway is
$R^{(5)}_{ab}=R^{(3)}_{ab}-2R^{-1}\nabla_a\nabla_bR$ plus
$R^{(5)}_{ij}=[1-(\nabla R)^2-R\Box R]\gamma_{ij}$.
This verifies all SO(3)-singlet linear equations, including constraints;
it is not an independent second-order source or flux audit.

Initial full-5D generic SymPy expression simplification was terminated
because it was unnecessarily slow. The exact rational-field
representation solved that computational bottleneck without changing
the equations or a physical acceptance gate. A temporary field-type
comparison in the horizon test was corrected by comparing both values
in the same rational field; its final exact identity passes.
