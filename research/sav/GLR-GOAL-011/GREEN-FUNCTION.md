# Retarded strong-field waveform functional

Write $L=-\partial_t^2+\partial_x^2-V_{\rm RW}$ and let $g(t,x;t',y)$ be the forward fundamental solution with $L g=\delta(t-t')\delta(x-y)$. Its free limit is $-\tfrac12\mathbf 1_{t-t'\ge |x-y|}$. Let $t=T(y)$ describe the exterior portion of $\Sigma$ with $|T'|<1$, and set $D_\Sigma=\partial_{t'}+T'\partial_y$ on source variables. The exact returning solution is
$$
 \Psi_2(t,x)=\int_{t'\ge T(y)}g(t,x;t',y)S(t',y)\,dt'dy
 -\int_{\Sigma}\big[g D_\Sigma\Psi_2-\Psi_2 D_\Sigma g\big],dy . \tag{1}
$$
The signs follow by Green's identity or the free limit. The graph normal flux uses the directed measure in $dy$. $S$ is (5) of [HIERARCHY](HIERARCHY.md), with the canonical data of [FINITE-DATA](FINITE-DATA.md). The Cauchy term is required also when the exact mean curl position vanishes.

The massive intermediate is the full $n=\pm1$ retarded solution of (2). It can be evaluated through the normally hyperbolic dual system obtained by eliminating $D_a Q^a$ using (4), with initial derivative reconstructed from dual Gauss, $dQ$ and the inverse duality. The projection on the angular equation is part of the propagated system. This yields a $1+1$ vector Green problem after Fourier projection, with no freely tunable source or boundary reflection. The retarded inverse is taken with regular ingoing horizon behavior and a causal exterior end. [C's independent reduction](investigators/C-RESONANCE.md) gives a two-polarization stationary radial representation for that intermediate inverse. No replacement of the massive wave by an algebraic inverse or a static radial particular solution is used.

The physical functional on a bounded cut window is
$$
 \mathcal W_{\rm GL}(u)=\lim_{x\to\infty}\Psi_2(u+x,x)
$$
in the common mass-matched Bondi frame. A short-strip estimate establishing the limit, its first derivative, and an evaluated leading term is given in [WAVEFORM](WAVEFORM.md). Equation (1) includes prompt, scattered, and any resonant contributions at once. Finite propagation leaves the trace zero before the first launch cut. The horizon-crossing slice and regular EF source give causal smooth coefficients on every compact exterior diamond. No homogeneous wave is injected from beyond the finite slice.

For resolvent signs use $e^{-st}$, $\Re s>0$ sufficiently large for the source under consideration, and let $h\sim e^{sx}$ at the left endpoint, $g_+\sim e^{-sx}$ at the right endpoint, with $W=h g_+'-h' g_+$. The kernel of $\partial_x^2-s^2-V_{\rm RW}$ is $h(s,x_<)g_+(s,x_>)/W(s)$. The transform for the outgoing trace carries the numerator
$$
 I(s)=\iint_{t\ge T(y)}e^{-st}h(s,y)S(t,y)\,dt\,dy
 -\int_\Sigma [w D_\Sigma\Psi_2-\Psi_2 D_\Sigma w],dy,
 \quad w=e^{-st}h(s,y), \tag{2}
$$
when the spatial limit and transform commute in this half-plane. Compact forcing and the auxiliary causal cutoff of the forcing described below justify this exchange. A finite physical observation window alone does not justify an infinite-time transform. The second term becomes $-\int h(s,y)(\Psi_{2,t}+s\Psi_2)|_{0}\,dy$ on a constant-time slice. Formula (2) also fixes the adjoint convention for the residue. See [QNM-RESIDUES](QNM-RESIDUES.md) for its continuation conditions.
