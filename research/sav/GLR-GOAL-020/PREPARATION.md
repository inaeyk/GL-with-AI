# Canonical graph and matched exact-family conditions

In graph coordinates \((\tau,r,z)\) with \(\tau=v-r\), put \(s=1/r,\ f=1-s,\ u_\Sigma=r-2x(r)\), \(u_\Sigma'=-(1+s)/f\). The outgoing graph rule is
\[
 \pi^r=\frac{\lambda r}{2}h(u_\Sigma),\quad
 A_r=-\frac{\partial_r h(u_\Sigma)}{2r}-\frac{s h(u_\Sigma)}{2r^2}
     =-\frac{\partial_r\pi^r-f\pi^r/r}{\lambda r^2},\qquad
 A_z=\pi^z=0 .
\]
Conversely, displacement \(\phi=2\pi^r/(\lambda r)\) and graph velocity \(\psi=[2r^2A_r+s(r\phi)_{,r}]/[(1+s)r]\) give \(w_v|_\Sigma=(1-p)(\psi-\phi'_u)/2\), \(p=-s\). This shows the differential relation is exactly the outgoing subspace within the leading real axial graph pair. For fixed support, \(\pi^r\) and \(A_r\) have radial Sobolev orders four and three, respectively. Upper independent pairs stay zero, and the constraint-induced upper jets are recomputed. If unit kinetic normalization is imposed, the tangent directions also satisfy \(\langle h_*',h'\rangle=0\). Smooth interior changes can be projected to the unit sphere by normalizing \(\|(h_*+t\zeta)'\|_2\). Changing the GL cosine phase by translating the entire compact preparation preserves the mean response. Axial profile phase relative to fixed cuts is a physical change. [Pinned canonical data and Gauss constraint](https://github.com/inaeyk/GL-with-AI/blob/eb439228601a649c658ce719b503ff0bdc37a83f/research/sav/GLR-GOAL-011/FINITE-DATA.md)

For literal shifted and dilated normalized bumps \(h_{l,w}(u)=\sqrt w\,B((u-a-l)/w)\), fixed support requires \(l\ge0,\ l+w\le\delta\). Thus a two-sided differentiable literal parameter curve through the saturated benchmark has zero first shift and width derivative. There are one-sided finite parameter changes within the ball. For \(w\in[\delta/2,\delta]\),
\[
 \partial_l h=-w^{-1/2}B',\quad
 \partial_w h=w^{-1/2}(B/2-yB'),\qquad y=(u-a-l)/w .
\]
Differentiate through order four in \(u\). Using \(|t_b^{(j)}|\le j! t_b^{j+1}\), the Bell bounds in [ordered_ball_arithmetic](ordered_ball_arithmetic.py) give each \(H^4\) parameter derivative below \(3\cdot10^{15}\). On \(\delta-w\in[2,3]\,10^{-136}\) and \(l\in[1/2,3/2]\,10^{-136}\), straight interpolation from the benchmark stays in the allowed support wedge and the displacement is \(<1.35\cdot10^{-120}\). These are literal changes at unit kinetic normalization, with no change of graph or cuts.

## Conditional finite-amplitude regularity

In the energy comparison of [DERIVATIONS](DERIVATIONS.md), assume the exact constrained common-clock trace is even in \(\epsilon\) under half-circle translation and has no tensor news at \(\eta=0\). The exact quotients \(A,D\) obey
\[
 A=\int_0^1\partial_\eta\mathcal N(0,t\eta)\,dt,\quad
 D=\int_0^1\int_0^1(1-s)
   \partial_\eta\partial_\epsilon^2\mathcal N(s\epsilon,t\eta)\,dt\,ds .
\]
Uniform continuity of the displayed derivatives in the physical Hilbert norm at the origin and identification of their ordered jets suffice for both quotient errors to tend uniformly to zero. No quartic theorem is required for this conditional implication.

If the exact data and frames also realize antipodal covariance, the sphere antipodal map preserves the real \(\ell=2\) scalar harmonic and reverses the sphere volume form. The axial harmonic is therefore odd. Split total tensor news into orthogonal even and odd antipodal sectors. The odd sector has \(A_e\le M_{03}\eta^2/6,\ D_e\le M_{41}\epsilon^2/24+M_{23}\eta^2/12\) when \(M_{ij}\) bounds the corresponding physical \(\partial_\epsilon^i\partial_\eta^j\) trace derivative. These factors follow by Taylor expansion of the integral formulas using parity in both variables. The even sector contributes at most \(\eta^2[M_{02,+}M_{22,+}/8+\epsilon^2 M_{22,+}^2/32]\) to the normalized total-energy error, by \(\|\mathcal N_+(0,\eta)\|\le M_{02,+}\eta^2/2\) and \(\|\Delta\mathcal N_+\|\le M_{22,+}\epsilon^2\eta^2/4\). The physical hypotheses must include fixed mass and circumference, the \(\eta^2\) ADM retuning and a matched inertial clock. Finite-radius Taylor jets do not establish the uniform null-trace derivatives. Exact total tensor energy is nonnegative.
