# GOAL-005 science handoff for Michael Yan

The L10 benchmark describes the early outgoing radion from the far exterior of a specified GL seed. Its sign is established at quadratic order on sufficiently early cuts. The point samples below are from the finite-slice scalar response to a saved GL profile. A separate calculation encloses the exact quadratic energy coefficient on the same window. This is a science handoff for a separately authorized comparison.

## Match the data and clock

- Set $L/r_0=10$, $k=2\pi/L$ and $G_4=G_5/L$. Use the future-horizon-regular growing $n=\pm1$ mode with $\Omega r_0\simeq0.06328119$. Normalize the cosine amplitude $\epsilon$ by the uniquely completed unitary tensor $h^U_{zA}=0$ with $H_{\theta\theta}/(2r^2)=1$ at $r=2r_0$. Equivalently $K_{\rm sphere}(2r_0)=2$ with $K_{\rm sphere}=H_{\theta\theta}/r^2$. The mode has the factor $\epsilon e^{\Omega t}\cos(kz)$ in the stated Schwarzschild clock. For the specified slicing, the validated linear apparent-horizon contrast is $[6.8950,6.9055]|\epsilon|$ at $\tau=0$ to first order. A coordinate metric amplitude requires its own map to this seed.
- On $\tau=v-r=0$, use Z preparation. The completed order-two radion and its normal derivative both vanish on the slice. Restrict the growing mode and its velocity to this slice and keep the constrained response associated with the data. The global exterior constraint construction includes a $C^1$ mass-tuning map and states the falloffs used. See [constraints](CONSTRAINTS.md).
- Use $r_*=r+r_0\log(r/r_0-1)$, $v=t+r_*$, $u=t-r_*$ at background order. On $\tau=0$, $u/r_0=-b-2\log(b-1)$ for $b=r/r_0$. The benchmark rays $u/r_0=-10,-9,-8$ start on the initial slice at $b\simeq6.57,5.84,5.15$. Their causal domains sample the exterior GL tail. A nonlinear comparison would use the corresponding Einstein-frame Bondi clock, with the actual mass-dependent asymptotic time shift accounted for.

## Compare the order-two waveform

With the expansion $g=g_0+\epsilon h_1+\epsilon^2q_2+\cdots$, the completed scalar is $\varphi_2=\sqrt{3}\langle q^U_{zz}\rangle/4$. Its radiation coefficient is $\Phi_2=\lim_{\rho\to\infty}\rho\varphi_2$ at fixed $u$, and $N_2=\partial_u\Phi_2$. Here $\rho$ is the Einstein-frame areal radius. The completed scalar at a finite radius includes the coordinate correction to the compact metric component.

| $u/r_0$ | $\Phi_2/r_0$ | $N_2$ |
| ---: | ---: | ---: |
| $-10$ | $5.82202\times10^{-5}$ | $6.74140\times10^{-5}$ |
| $-9$ | $1.84271\times10^{-4}$ | $2.11188\times10^{-4}$ |
| $-8$ | $5.75685\times10^{-4}$ | $6.51540\times10^{-4}$ |

For $I/r_0=[-10,-8]$, the scalar flux is $N_\varphi^2/G_4$. With $M_0=r_0/(2G_4)$, the sampled coefficient $\mathcal K_0=(2/r_0)\int_I N_2^2\,du\simeq3.7326\times10^{-7}$ multiplies $\epsilon^4$. The point samples are diagnostics for the saved profile. The independent outward-rounded benchmark gives $3.5316\times10^{-7}<\mathcal K_0<4.0171\times10^{-7}$ on the same cut window, from an enclosed exact exterior GL source and the curved Volterra response. See the [sampled waveform](compute/early_null_waveform.csv) and [quadrature evidence](compute/early_null_results.json).

For a perturbative comparison, report $\partial_u(\rho\varphi)/\epsilon^2$ on several fixed areal worldtubes at matched retarded times. Check seed convergence at fixed radius, radial and time resolution, exterior backscatter, and causal boundary clearance. A uniform nonlinear radiation estimate is needed to turn that convergence into an error at null infinity. Record independent outgoing scalar Cauchy data alongside the GL projection. They add coherently to the forced news and can change or cancel the leading signal on the selected interval. A future-boundary prescription alone leaves this freedom.

The massive $n=2$ channel has threshold $\lvert\omega\rvert>2k$ for propagating real frequencies and requires a separate timelike flux accounting. The early weak-tail benchmark supplies no controlled prediction at first bulge. Its natural domain is a specified preparation, weak seed, and exterior cut, with the nonlinear news remainder still open.
