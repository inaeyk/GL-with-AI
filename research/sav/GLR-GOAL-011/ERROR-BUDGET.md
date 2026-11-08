# Error budget and evidence limits

| Quantity | Evidence and control | Scope |
| --- | --- | --- |
| GL spectral input | Pinned validated $\Omega r_0\in[0.06328118,0.06328120]$, future-regular nodeless branch, decaying Coulomb solution, unitary normalization $H_{\theta\theta}(2r_0)=8r_0^2$ | Inherited GOAL-005 certificate and audit |
| $\chi_\infty/r_0$ at $13/8$ and $7/4$ | Outward-rounded bounds $(-1.103,-1.016)$ and $(-0.604,-0.537)$ | Entire signed physical ray integral included |
| Mode and quadrature error | Left Riccati Hermite defects on 12,288 cells and right defects on 8,192 cells. Propagate endpoint uncertainties, intersect $p(2)$ and enclose the integrand on each cell, with a partial cell at the launch radius | Ball arithmetic. Floating solver output is a candidate only |
| Tail after $60r_0$ | Weighted optical integral tail absolute upper bound $7.19\times10^{-16}$ before multiplying by $e^{\Omega(t_R-x_R)}$ | Uses pinned $-.65<p<-.60$ and $|H_{LL}/f|<5|U|/r$ on this tail |
| Independent quadrature | Direct logarithmic derivative of $y$ and direct component assembly, with a distinct saved-component spline control. Agreement in $\chi$ within $1.22\times10^{-7}$ | Empirical. The wider interval above is the certificate |
| Short-packet waveform | Uniform field and first-derivative estimates in width in [WAVEFORM](WAVEFORM.md), with compact evolution and weighted infinite-strip estimate | Constants not numerically evaluated. No absolute error for a specified nonasymptotic width or later ringdown window |
| Mixed massive field | Full two-polarization causal equations and short-strip propagation. Its GL-weighted field and needed derivatives are integrable on finite cuts | No evaluated massive waveform or escape energy |
| Massive matching at shifted pole | First-order Frobenius and factored outgoing Coulomb ends on a rotated contour. Matching singular value stabilizes on four pairs of radial truncations and two final integration tolerances | Empirical point values. No enclosure of endpoint remainders, determinant on a spectral disk or full overlap |
| Returning pole and shifted pencils | Acb Riccati residual and endpoint contraction certificate, with Rouché root count at radius $5\times10^{-8}$, two interval noncoincidence checks and normalized $W'$ bound | Local literature-fundamental pole |
| Physical numerator | Full finite-launch numerical overlap with separate $HJ$, $H^2P$ and $B_EP$ terms, resolved EF collar, outgoing contour and independent strong projection | Empirical spread below $6\times10^{-5}$ for the residue at $R/r_0=13/8$, $\delta=0.12$. Full error enclosure and pole dominance remain open |
| Fit-free horizon and lower inputs | Validated GL scale and matching data, two interval-enclosed baseline launch moments, Acb Frobenius collar with exact split boundary | Collar enclosure conditional on the mixed and mean matching amplitudes. Exterior source error remains empirical. [Horizon source](HORIZON-SOURCE.md) |
| Physical shifted mixed match | Interval reconstruction of the compact launch profile, regular affine horizon data, far-ray contraction and directed residual transport, followed by a preconditioned matching inverse | Physical mixed position enclosed at $r_m=2.5$ with Euclidean error below $0.33257$. The propagation to the horizon amplitudes and signed returning projection remain open. [Affine matching](EXTERIOR-SOURCE.md) |
| Physical mean cap | Factored EF Ricci integrands, outward-rounded compact quadratures and analytic tail bound below $9.054\times10^{-16}$ per component | $7.163<\mu_v<7.468$, $74.342<\beta_v<75.567$ at $r-1=1/128$ |
| Complete signed numerator and residue | Directed candidate residual pairing, physical state envelopes, contact-aware compact quadrature, analytic collar and tail, and a refined normalized Wronskian | Four-subdivision $A_{21,q}\in[0.08223,0.18771]+i[-0.42418,-0.31707]$. See [signed enclosure](SIGNED-RESIDUE.md) for refinement and common-clock projection |
| Higher amplitude orders | Fourier covariance and ordered jets | No uniform nonlinear Bondi remainder or finite-seed error |

For the optical enclosure, $p=U_x/U$ and $y=(1+k^2r^3)U/r$. The inherited validator bounds $p$ by the residual of a continuous piecewise cubic and a scalar Riccati comparison. On each cell $p$ lies in the returned box for the full growth-rate interval. To propagate $U$, use $U(x_b)=U(x_a)\exp(\int_{x_a}^{x_b}p\,dx)$ and the enclosing product of this box with the signed step. The interval from zero to the step bounds the value anywhere in the cell. The normalization fixes sign and amplitude. Interval quadrature takes the enclosed density times the exact tortoise cell length. The right tail follows by integrating $e^{-(.60-\Omega)(x-x_{60})}$ against the endpoint envelope. The optical ratio bound on that tail follows from $r\ge60$, $D=1+k^2r^3$, $f\ge59/60$, and $|3k^2r^2/D-1/r+p/f|<0.73$ in the reconstruction. E's repeat using the wider independently certified growth interval retains both displayed $K_*$ bounds.

[Physical overlap settings, limitations and commands](PHYSICAL-RESIDUE.md) cover the later numerical resonance checkpoint. The output intervals for the optical coefficient are much wider than D's numerical convergence. The prior tail's local sign cannot be integrated through the barrier. The wrong horizontal launch slice changes the optical delays. Applying the shear flux normalization directly to raw curl gives a factor-of-four discrepancy. The lack of absolute convergence of a naive real-axis inner integral at the continued frequency is tracked in [QNM-RESIDUES](QNM-RESIDUES.md).

Reproduce from the scoped research directory. The run used Python 3.14.8, python-flint 0.9.0, NumPy 2.4.6, SciPy 1.18.0 and SymPy 1.14.0 as applicable. The pinned GOAL-005 file imported by the interval check sets Arb precision to 112 bits. All checks are non-production.

```sh
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/barrier_delay.py
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/barrier_delay_independent.py
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/identity_controls.py
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/massive_match.py
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/spectral_certificate.py
```
