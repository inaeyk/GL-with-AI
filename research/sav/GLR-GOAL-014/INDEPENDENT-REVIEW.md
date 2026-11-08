# Independent adversarial review of GLR-GOAL-014

**Grade B.** The directed sharp-cut test, finite-cap adjoint, separate physical weak tails and negative early homogeneous graph overlap survive review under the pinned ordered inputs. The complete early candidate is empirically positive. Both prescribed physical interference signs remain open because the compact bulk and terminal error budgets remain open. The strong candidate gate is an alternative sufficient inequality, with its decisive physical compact cap error unevaluated.

## Provenance and verification scope

The scientific source is [GOAL-013 at `1e8e628f68af4cab174fcc44b81af4748a22115c`](https://github.com/inaeyk/GL-with-AI/commit/1e8e628f68af4cab174fcc44b81af4748a22115c), with [source PR #17](https://github.com/inaeyk/GL-with-AI/pull/17). The [GOAL-013 audit at `26a09054366933a197c34fa67f617af7da76e48d`](https://github.com/inaeyk/GL-with-AI/blob/26a09054366933a197c34fa67f617af7da76e48d/research/sav/verification/GLR-GOAL-013/AUDIT.md) was a read-only input from [PR #18](https://github.com/inaeyk/GL-with-AI/pull/18). The benchmark, the GOAL-013 identities and its published audit findings were visible from the start. I derived the GOAL-014 contact and adjoint signs by direct distributional transposition before reading the new A report. I implemented the baseline error check and central graph march separately. I saw the proposed GOAL-014 projections during the comparison and correction pass. The [investigator record](investigators/E-ADVERSARIAL.md) distinguishes the independent steps, shared formulas and reused numerical routines. The five-dimensional reduction, GL root certificate, absolute mode and flux envelopes and historical pole boxes are inherited. This review is a separate scientific calculation and an AI audit.

## Decision table

| Claim | Review finding |
|---|---|
| Exact graph cancellation and sharp test | Independently recovered via conservation of $J=(F-4w_v,G)$. The induced upper jet supplies the graph contact. The terminal $F$ ray survives. An exact control gives cap overlap $5/2$ entirely from that endpoint. |
| Weak cap adjoint and exterior splitting | Directly transposing the equation for $w_u$ reproduces the compact formula and exterior signs. With physical $G_\infty=0$, the compact $+G_c$ and exterior $-G_c$ traces cancel. The strong-source exterior route retains the compact $G_c$ trace. |
| Signed terminal-ray error | The row ODE follows by differentiating its product with the transverse error. Its endpoint and residual signs agree with the claimed formula. The numerical multipliers remain central-coefficient diagnostics. A narrow massive-field layer can have vanishing bulk residual and an order-one terminal transverse derivative. |
| Physical baseline weights | Independent directed diagonal-Picard bounds enclose $N_{01}$ and its derivative. The terminal baseline test is strictly positive on both cuts. The discrete dyadic-primitive candidate in the main report has its own whole-cell derivative error. An $L^2$ baseline bound alone cannot replace this treatment. |
| Fixed-mass mean and graph launch | The mean density is rank one with $G_{\rm mean}=0$. Both massive graph velocities follow from the original canonical constraints. The physical graph $F$ change under artificially truncated mean integration is directed. The graph point and the terminal-ray integral are separate. |
| Separate weak exterior norms | Reviewed the fixed-$u$ flux control, including transverse derivatives via $Y_x=2Y_v-Y_t$ and the PDE identity for $Y_{xv}$. The half-line $H^1$ trace gives $G_\infty=0$. These bounds are conditional on the ordered coefficient and absolute flux inputs. A divergence-free manufactured current shows that an exterior strong source norm alone does not give these bounds. |
| Homogeneous graph contribution | The corrected directed complete early interval is $[-2.05250,-1.41374]\times10^{-4}$. The distinct central GL and compactified scalar calculation gives about $-1.74125\times10^{-4}$, with separate agreement for the mixed, direct and mean pieces. The decomposition is tied to the chosen exact-curl variable. |
| Complete compact pilot | The three numerical comparisons share physical lower fields. The exact discrete transpose is an algebra check. Differentiated-current versus derivative-reduced return changes the numerical scalar reduction. Their agreement supports the candidate and leaves coefficient, lower-field and whole-cell continuum errors open. |
| Strong candidate gate | The directed cap arithmetic concerns a specified affine candidate. The fixed contribution and $E_c$ coefficient yield a sufficient $E_c\le570$ for positivity. No physical $E_c$ satisfying the gate was established. |
| Tensor-energy normalization | Harmonic norm 12 and the factor of two in the news square give $3r_0/(4\pi G_4)$. A nonconstant clock relabeling requires the inertial flux Jacobian and the displaced physical endpoints. |

## Failures that affect interpretation

- The first directed graph implementation used the wrong slope factor $1+p$ in the denominator of the characteristic launch. The graph condition gives $w_x=-p w_t$ and hence $w_u=(1+p)w_t/2$. The denominator in $w_u=o^T\mathcal D/[\lambda r(1-p)]$ is $1-p$. The uncorrected graph values disagreed with the independent march by a factor of about $4.2$. The failed output and corrected whole-cell calculation are separate. [Failed run](checks/d_graph_wrong_normal.txt) · [Corrected run](checks/d_graph_projection.txt) · [Independent march](checks/e_graph_projection.txt).
- Small weak currents and shear do not imply small strong news, even for the actual scalar potential and the correct graph. A co-oscillating baseline test can also have vanishing $L^2$ error while its product with the news error tends to a nonzero limit. The sharp dual certificate avoids this failure through a derivative and endpoint certificate, or by requiring a genuinely strong news error. [Analytic controls](investigators/E-ADVERSARIAL.md#short-scale-and-frame-controls) · [Observed quadratures](checks/e_controls.txt).
- With the empirical early overlap near $+7.8181$, the direct terminal piece is about $+19.9431$. Leaving it out would change even the empirical sign. A small exterior tail cannot remove the terminal compact error. The strong route, although based on a different numerical scalar formulation, still needs the coupled lower and return volume residuals. [Error budget](ERROR-BUDGET.md).
- A piecewise conforming uncommuted reconstruction can still fail a time-commuted seam check. The contact from a current jump has its normal flux sign. A strong residual budget must include that jump or show compatibility of the relevant derivative traces. The reviewed pilot does not supply such a bound. [Graph and seam controls](investigators/E-ADVERSARIAL.md#independent-graph-and-duality-check).

## Separate outcome grades

| Target | Audited status |
|---|---|
| Early sign | **Open.** Positive empirical value, strictly negative directed homogeneous graph subprojection. The weighted compact current errors, including the terminal $F$ ray, or the alternative $E_c$ remain unevaluated. |
| Full-packet sign | **Open.** Directed test and transfer inputs evaluated. The physical compact solve remains behind the early gate. |
| Compact strong news | **Partial.** Specified cap candidate and directed arithmetic, baseline and graph subcertificates, plus a conditional sign target. No complete physical $L^2$ error. |
| Both QNM comparisons | **Open.** Neither finite-window physical numerator is bounded by this investigation. |
| Nonlinear bridge | **Conditional.** The Taylor estimates and energy remainder follow from the listed uniform solution-to-news hypotheses. The physical family and null-infinity differentiability are open. |

The available intervals certify the subproblems above. They supply no signed tensor-energy coefficient for either full ordered projection. [Main report](REPORT.md) · [Independent evidence](investigators/E-ADVERSARIAL.md) · [Reproduction](REPRODUCTION.md).
