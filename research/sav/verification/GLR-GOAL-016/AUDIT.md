# Independent scientific audit of GLR-GOAL-016

**Outcome B survives for the specified data.** The quadratic vacuum radion, constrained preparation cost, fixed-source signed tube comparison and early mass–radius limits survive independent review. For the two stipulated scalar completions, the pulsed band's lower bound also exceeds the baseline all-band upper bound in each channel. The matched nonlinear vacuum trace and the matched semiclassical spectrum remain open.

## Audited version and scope

| Record | Exact identity and role |
| --- | --- |
| Completed Codex source | [PR #24](https://github.com/inaeyk/GL-with-AI/pull/24), open, draft and unmerged on inspection. Branch **codex/glr-goal-016-radion-stabilization** at [6e7615f9d0a88a08813203ad5b7cadde990fa042][head]. |
| Checkpoint and base | [73359b744e33ee675b9d60a50bbd9c1fdc61f573](https://github.com/inaeyk/GL-with-AI/commit/73359b744e33ee675b9d60a50bbd9c1fdc61f573) is the immediate parent. Its parent is [0b4bfcbb978b52626fb0816874df06055703c195][014], source [PR #19](https://github.com/inaeyk/GL-with-AI/pull/19). The comparison adds 35 paths, all in the source dossier. |
| Scalar dependencies | Inherited GOAL-005 [61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708][005] supplies the eigenmode, Z data and massless radiation. The GOAL-005 audit, GOAL-006 damped preparation, GOAL-008 transport and audit, and GOAL-009 optical analysis and audit were fetched at the immutable side pins in the [source charter][charter]. GOAL-010 describes the distinct massive spin-two channel. |
| Read-only ordered tensor records | GOAL-014 [review at 84c9824f826e3517c8a1ed74d96bea061ab88584][audit014], GOAL-015 [source at 65a4af854a25ca02e40f31b00fe5ff789935bdc4][015] and [review at 1089b3cb36eef1c25278c4fbfe3e843bfdcff6e4][audit015]. The two GOAL-015 pins have the GOAL-014 source as merge base with GOAL-016. Both independent reviews sit outside GOAL-016 ancestry. |
| Publication | [Work scientific review protocol][protocol]. This review branch starts directly at the audited head. |

Use the real unhalved $L/r_0=10$ GL cosine with $H_{\theta\theta}(2r_0)/(8r_0^2)=1$, the wider enclosed $\Omega r_0\in(0.06328117,0.06328121)$, the graph $v=r$ in dimensionless units and the matched window $u/r_0\in[-10,-8]$. The Z scalar value and normal derivative vanish at quadratic order. The eigenvalue bracket, gauge-completed source and ADM constraint construction are shared inputs. The [source report][source] and charter were visible, so this is a non-blind external review. Four delegated specialists and a fifth coordinator track checked the physical extraction and vacuum symbols, the directed computations, asymptotic and adversarial controls, the stabilization literature and the independent arithmetic and synthesis. Runtime model and effort were inherited without per-track verification. The [computational evidence](COMPUTATION.md) records the checks and their limits. No production evolutions entered the audit.

## Acceptance matrix

| Claim | Audit grade and scope |
| --- | --- |
| Extracted massless signal | **Directed quadratic coefficient** for the pinned normalized mode and Z family. $2\lVert n_Z\rVert_2^2\in(3.5316,4.0171)\times10^{-7}$. The exact free-source approximation has $L^2$ curvature error below $1.088\times10^{-5}$ on these cuts. Numerical source quadrature carries its own enclosure. |
| Preparation cost | **Analytic with directed inputs.** Cancellation costs at least the Z window energy in added homogeneous preparation energy. The source's upper factor 1.20 survives. An independent refinement gives a factor below 1.194 for the infimum. The normalized surviving energy is bounded below by $(\sqrt{\mathcal K_Z}-\sqrt{E_h})_+^2$ in this matched quadratic family. |
| Timelike tube comparison | **Directed continuum inequalities** for the prescribed Z source on Schwarzschild and the finite-cut physical current. Adjacent masses are strictly ordered at both tubes. The signed coefficient decreases from $24r_0$ to $48r_0$ for each sampled positive mass. |
| Early null limits | **Analytic for the frozen Z problem.** Fixed $\mu>0$ gives zero early integrated null flux. On $\mu^2x(R)\to\Lambda$, the trace is $e^{-\Lambda J/2}n_Z$. Suppression on the original $0<\Lambda\le 2\Omega r_0$ survives. An alternating-series refinement using the same history bound proves strict decrease through $\Lambda\le 5(2\Omega r_0)/3$. The two ordered limits differ. |
| Completed scalar source | **Directed and completion-dependent.** Each channel separates on $\omega r_0\in[4,4.25]$ by more than $0.0335219$. The baseline all-band work ceiling and pulsed band floor imply all-band separation greater than $0.0059839$ for the same nonnegative channel energy. Accurate total fractions remain open. |
| Finite-amplitude vacuum | **Local principal-symbol calculation and obstruction.** The leading compact correction about a circle-independent radiating mean survives. The inherited same-order forcing estimate loses one compact derivative. The coupled uniform trace estimate, full Cauchy and terminal matching and a numerical seed interval remain open. |
| Stabilization | **Stationarity obstruction and conditional directional result.** The specified Unruh state's positive outgoing power with no compensating supply precludes stationary matching. The local Gauss–Bonnet deformation gives a positive fixed-ADM derivative of $k_c^2$ at a differentiable static vacuum base in the stated coupling convention. A matched curved stress, constrained response and physical GL shift remain open. |

## Independent checks and qualification

With $a=f/r$, $a_x+a^2=f/r^3$. The physical outward current differs from the master current by $\partial_t(a\psi^2)/2$, with the opposite spatial divergence in the energy density. Relative to the reference mass $r_0/(2G_4)$, the integrated difference on a fixed tube is $a[\psi^2]_{-10}^{-8}$. The incoming-cap flux, outgoing-null storage and prescribed work enter separately. A smooth source-free local Cauchy jet $(\psi,\psi_t,\psi_x)=(0,1,1)$ has $j=-1$. [Physical derivation](PHYSICAL.md) · [Source certificate][tube].

Using the directed endpoint radii, the adjacent mass gaps exceed $4.89321\times10^{-9}$ and $6.08054\times10^{-8}$ at $24r_0$, and $5.84475\times10^{-8}$ and $8.35627\times10^{-8}$ at $48r_0$. The positive-mass radius gaps exceed $8.59866\times10^{-9}$ and $1.07902\times10^{-8}$ in increasing mass order. The first 24-radius pass overlaps for the lightest pair. The coarse 48-radius pass overlaps for both adjacent pairs. The rational check consumes archived intervals. Replay and code audit check their construction. [Check](check_margins.py) · [Output](check_margins.txt) · [Computation](COMPUTATION.md).

For the stipulated completed force $\mathcal F$, let $E$ include the positive stored master energy and accumulated positive outgoing energies on an exhausting domain. With zero data the work identity and $\lVert\psi_t\rVert_2^2\le2E$ give $d\sqrt{2E}/dt\le\lVert\mathcal F(t)\rVert_2$, by regularization at zero energy. The band uses positive Wronskian channel energy and the same $2E$ reference-mass normalization. Equivalently one can exhaust propagating spectral bands under the full-line work bound. Each channel is bounded by the total supplied energy. For the baseline the work upper is $0.027545835$, while the pulsed band alone exceeds $0.033529805$ in each channel. The exact archived bounds give a difference greater than $0.0059839$. Intermediate escape transfer and subthreshold horizon transfer remain necessary for precision totals. [Band construction and work bound][tube] · [Independent asymptotics and scattering](ADVERSARIAL.md).

At $\Lambda=0$, the model $K(\Lambda)=2\lVert e^{-\Lambda J/2}n\rVert_I^2$ has $K'(0)=(Jn)(u_0)^2-(Jn)(u_1)^2$. On translated cuts $0,1$, take $Jn=e^u(1-u)$ for $u\le1$. This early exponential history gives $K'(0)=1$. The pinned Z history has the required sign and quantitative positive comparison. The fixed-positive-mass null limit and the joint scaling proof use different uniformity regimes. [Independent asymptotics](ADVERSARIAL.md) · [Pinned critical-limit derivation][critical].

## Relation to audited GOAL-015

GOAL-015 concerns the Einstein-frame axial tensor response to a pre-existing $\ell=2$ packet at order $\epsilon^2\eta$ in the returning news. The cumulative $\int N_{01}N_{21}\,du$ changes from positive to negative between its matched cuts, with a separate early strong cap error below $323.984$. GOAL-016 concerns the spherical zero-mode radion at quadratic waveform order and order $\epsilon^4$ in its energy, together with a massive timelike propagator comparison. Spherical symmetry permits the radion and removes transverse trace-free spherical shear. A signed tensor overlap carries no scalar energy or massive escape bound. The normalized GL mode, fixed ADM mass, circle and Einstein-frame clock are shared consistency inputs. [GOAL-015 independent review][audit015] · [GOAL-016 observable conventions][observables].

The remaining vacuum gate is a truncation-uniform estimate against the exact coupled compact principal operator with the physical Cauchy, terminal and clock data. The stabilized branch needs a curved renormalized state and its constraint-compatible retarded response. Accurate late escape needs a specified physical source completion. [Negative controls and corrections](NEGATIVE.md) · [Physical assessment](PHYSICAL.md) · [Literature and matching](LITERATURE.md).

[head]: https://github.com/inaeyk/GL-with-AI/commit/6e7615f9d0a88a08813203ad5b7cadde990fa042
[014]: https://github.com/inaeyk/GL-with-AI/commit/0b4bfcbb978b52626fb0816874df06055703c195
[005]: https://github.com/inaeyk/GL-with-AI/commit/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708
[015]: https://github.com/inaeyk/GL-with-AI/commit/65a4af854a25ca02e40f31b00fe5ff789935bdc4
[audit014]: https://github.com/inaeyk/GL-with-AI/commit/84c9824f826e3517c8a1ed74d96bea061ab88584
[audit015]: https://github.com/inaeyk/GL-with-AI/blob/1089b3cb36eef1c25278c4fbfe3e843bfdcff6e4/research/sav/verification/GLR-GOAL-015/AUDIT.md
[protocol]: https://github.com/inaeyk/GL-with-AI/blob/93b2d5057c3250b14c89284ac297ebd4091250bd/research/sav/AGENTS.md#work-scientific-review-publication
[charter]: https://github.com/inaeyk/GL-with-AI/blob/6e7615f9d0a88a08813203ad5b7cadde990fa042/research/sav/GLR-GOAL-016/GOAL.md
[source]: https://github.com/inaeyk/GL-with-AI/blob/6e7615f9d0a88a08813203ad5b7cadde990fa042/research/sav/GLR-GOAL-016/REPORT.md
[tube]: https://github.com/inaeyk/GL-with-AI/blob/6e7615f9d0a88a08813203ad5b7cadde990fa042/research/sav/GLR-GOAL-016/transport/CERTIFICATE.md
[critical]: https://github.com/inaeyk/GL-with-AI/blob/6e7615f9d0a88a08813203ad5b7cadde990fa042/research/sav/GLR-GOAL-016/transport/CRITICAL-LIMIT.md
[observables]: https://github.com/inaeyk/GL-with-AI/blob/6e7615f9d0a88a08813203ad5b7cadde990fa042/research/sav/GLR-GOAL-016/transport/OBSERVABLES.md
