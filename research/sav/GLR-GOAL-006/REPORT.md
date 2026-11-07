# GLR-GOAL-006 — Nonlinear Radiation and Bondi Control

**Outcome B.** A modified exact Z constraint family has arbitrarily strong prescribed finite weighted decay relative to the Schwarzschild-string slice. A separate characteristic estimate gives the quartic news remainder under explicit exterior evolution and optical matching hypotheses, including a flux hypothesis compatible with the original differentiated $r^{-2}$ momentum tails. The required nonlinear Einstein estimates on the full physical wedge remain open.

Scientific baseline [61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708](https://github.com/inaeyk/GL-with-AI/commit/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708). Independent Work review [a9bfbec0efe41b881e4b12aa262f5fcfcd94c639](https://github.com/inaeyk/GL-with-AI/commit/a9bfbec0efe41b881e4b12aa262f5fcfcd94c639). The setting is exact transverse $\mathrm{SO}(3)$, vacuum $\mathbb R^{1,3}\times S^1$, $L/r_0=10$, and the future-regular growing GL mode on the audited finite slice $\tau=v-r=0$. Z sets the completed quadratic radion value and normal derivative to zero. The inherited continuum result on $I/r_0=[-10,-8]$ is
$$
 3.5316\times10^{-7}<\mathcal K_Z=2\int_{I/r_0}n_Z^2\,d(u/r_0)<4.0171\times10^{-7}.
$$
It is the early-cut quadratic energy coefficient for this preparation and seed normalization.

## Exact spatial result

The mean of the fixed-leaf first jet vanishes. The mean momentum and lapse equations give $w_\infty(\epsilon,0)=O(\epsilon^2)$. The independently checked charge derivative $\partial_\lambda M/M_0=9/14$ yields $\lambda(\epsilon)=O(\epsilon^2)$ for the pinned family. This controls the tuned scalar expansion on compact slabs using the two-parameter local solution. [Derivations §1](DERIVATIONS.md#1-original-constraint-family-and-the-tuned-mass-rate)

The unmodified constraint equations permit persistent oscillatory momentum profiles after a mass-dependent radial phase is removed. An exact ideal-end example with a nonzero limiting profile violates even the critical weighted square integral. The coefficient of this tail for the particular pinned GL family remains undetermined. [Derivations §1](DERIVATIONS.md#a-tail-in-the-original-constraint-class)

Route A changes only the exterior rule for the curvature function $d$. It preserves $b=1$ and $K^z{}_z=0$ pointwise, the exact GL first jet, the inner collar prescription at fixed inner lapse, and the ability to tune ADM mass to $M_0$. Mean momentum is damped directly. A circle inverse derivative damps one of the oscillatory momentum variables, and a cross energy transfers the decay to the other. For fixed damping parameter $p$ the first-jet-subtracted angular lapse and scaled momenta are $O(\epsilon^2r^{-p/2})$ in periodic Sobolev norms after mass tuning. One can choose $p$ to cover any fixed finite radial weight and derivative count. The background slice still has spherical $K_0=O(r^{-2})$. The modified preparation changes higher-order massive Cauchy data and can change the tuned inner lapse at order $\epsilon^2$. The same quadratic mean Z waveform survives. [Derivations §2](DERIVATIONS.md#2-route-a-damping-with-the-exact-z-leaf-data)

## Conditional news theorem

On the full early wedge, the exact compact-normal Ricci equation yields a mean scalar equation $\psi_{uv}=V_c\psi+F$ in an effective double-null chart. The Volterra estimate gives a field trace and an $L^2(I)$ news trace if the potential is uniformly integrable in the ray norm and the initial free-field and forcing differences are quartic there. An $O(\epsilon^2)$ difference of potentials suffices. The quadratic coefficient is exactly the pinned curved Z response in the completed gauge. A sufficient massive forcing estimate uses three dyadic commuted flux bounds, for the odd field, the oscillatory even field, and the odd error after subtracting the first jet. Quadratic mean products gain $1/r$ relative to these massive fluxes, allowing dyadic growth exponent $\delta<1$. This argument avoids a positive spatial weight on $K$ for the product estimate. [Derivations §3](DERIVATIONS.md#3-route-b-exact-scalar-equation-and-a-trace-estimate)

If the effective chart matches geometric circle-length cuts in $H^1(I)$ at infinity, and the normalized physical clock differs by $O_{C^1}(\epsilon^2)$ on a padded window, the theorem gives
$$
 \|N_\epsilon-\epsilon^2 n_Z\|_{L^2(I)}\le C_I\epsilon^4.
$$
The missing gate is to establish a uniform Einstein development and prove the differentiated flux, mean potential, initial-jet, and optical bounds of §§3–4 on that development. Fast spatial decay from Route A does not furnish the optical estimate. ADM tuning cancels the static mass logarithm in the comparison chart. It leaves the evolving eikonal to be controlled. The compact-tilt flat-vacuum caustic and the secular log example identify distinct failures of naive clocks and limit exchanges. An independent full-optical derivation gives a second exact ray identity involving only compact and outgoing derivatives in its quadratic source. [Derivations §4](DERIVATIONS.md#4-optical-clock-and-geometric-news)

Given the missing bounds and the standard scalar flux identification on the resulting KK end, let $b_Z=\|n_Z\|_{L^2(I/r_0)}$. The physical scalar energy would satisfy
$$
 2[\max(0,\epsilon^2b_Z-C_I\epsilon^4)]^2\le E_I/M_0\le
 2(\epsilon^2b_Z+C_I\epsilon^4)^2 .
$$
Here the news estimate is expressed in dimensionless cuts. This would give an order-six absolute energy error and strictly positive scalar energy for sufficiently small nonzero seeds. There is no value for $C_I$ or certified finite-seed range here.

## Massive response and comparison

The causal $n=\pm2$ system is normally hyperbolic on the background when supplied the complete forced trace and divergence and compatible Cauchy jet. The defects satisfy homogeneous subsidiary equations. Regular ingoing coordinates and direct unitary completion include the future horizon without a frequency-domain matching assumption. A finite-$\tau$ energy estimate and the sourced Einstein Lagrange identity give a well-posed finite-time formulation and a signed tube, horizon, storage and source-work balance. A separated particular response leaves a homogeneous Cauchy correction. No quantitative full causal lift or emitted massive energy has been evaluated. [Derivations §5](DERIVATIONS.md#5-causal-massive-second-harmonic-and-its-energy)

The [Michael handoff](HANDOFF-MICHAEL.md) gives a preparation-matched comparison on fixed tubes, separate radius and seed tests, and the massive timelike accounting. [Verification](VERIFICATION.md) distinguishes independent reconstruction, non-blind audit, literature and manufactured checks. [Reproducibility](REPRODUCIBILITY.md) pins inputs and commands. [Agent activity](AGENT-ACTIVITY.md) records the bounded work and checkpoints. The next scientific gate is a fixed-Bondi-wedge Einstein estimate proving (B4) and a normalized optical map for either constraint family. No first-bulge energy follows from the early-cut result.
