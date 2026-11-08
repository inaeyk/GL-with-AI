# Contour conditions and finite-window inversion

## The causal continuation and its local scope

Set \(T(x)=r(x)-x\) and zero-extend across \(t=T(x)\). Testing the baseline extended equation with \(e^{-\sigma t}h\) gives
\[
 -J_h(\sigma)=-\int e^{-\sigma T}(1-T')a_\delta(h_x+\sigma h)\,dx .
\]
After splitting the Green integral at the observation radius, the physical derivatives retain \(+e^{-\sigma T}a_\delta\) in the radial derivative and \(-e^{-\sigma T}a_\delta\) in the time derivative. The upper velocities follow from the canonical data. The strong equation keeps their graph terms. In the weak form these cancel the divergence contacts, while the finite left split still contributes \(+2\phi q_c/\lambda\), where \(\phi=r e^{-st}h\), \(q_c=f r^{-2}\mathcal D^t\) in the pinned notation. The leading model \(\widehat q_c=-\widehat p\propto-e^{sx}\) has zero strong source and a spurious divergent weak primitive if the boundary is dropped. [Physical contacts](investigators/B-PHYSICAL-SOURCE.md) · [Independent sign check](investigators/E-ADVERSARIAL.md) · [Pinned pairing](../GLR-GOAL-011/PAIRING.md).

At the horizon the sufficient Volterra conditions are \(1+2\Re\sigma>0\), \(1+2\Re s_1>0\), and \(1+2\Re s>0\), with the forced rows using the regular Eddington–Finkelstein factor \(f\). Rotate beyond the compact support to \(r=2.5+e^{i\theta}\rho\). For \(m_\theta(z)=\Re(e^{i\theta}z)\) the right construction needs
\[
 m_\theta(\sigma),\ m_\theta(s),\ m_\theta(\kappa)>0,\qquad
 m_\theta(q+\sigma+\kappa)>0,\qquad
 m_\theta(q+\kappa-s)>0 .
\]
Here \(q=\sqrt{k^2+\Omega^2}\). The outgoing massive branch joins \(\Re\kappa>0\) in the physical half-plane to \(\Im\kappa<0\) along the lower propagating corridor. The last rate controls the growing \(e^{+sx}\) part of the ingoing test. The direct and forced-particular terms use \(m_\theta(2q+\sigma-s)=2(q-\Omega)\cos\theta>0\). The Work off-pole margin exceeds \(0.058\) in a thin corridor about \(-\Im s=0.747343\) for \(0\le\theta\le\pi/4\). This gives the radial continuation from a physical right overlap to both fundamentals. At \(s_q\) the inner real-axis exponent is about \(-0.04049\), so the naive absolute radial pairing on that axis fails. [Reconstruction and rates](investigators/A-SPECTRAL.md#endpoint-conditions-and-candidate-singularities) · [Pinned Work continuation](https://github.com/inaeyk/GL-with-AI/blob/75cb3ff3feef98164a65cf14105be6df77ea809c/research/sav/verification/GLR-GOAL-011/CONTINUATION.md).

A radial arc defining \(I(s)\) and a frequency-plane arc inverting it require different bounds. A finite frequency rectangle gives an exact Cauchy identity with its residues, oriented slit jumps, displaced vertical segment and both horizontal wings, while retaining the original inverse outside the rectangle. The horizontal corridor allows local frequency detours in its band and the conjugate band. A global deformation requires a larger domain, control of both massive lips and the two massless cuts, and frequency-uniform estimates for the wings and high-frequency remainder. The local pole proof supplies none of those global bounds. The standard Schwarzschild Green function decomposition provides context for the returning inverse, while the two inserted inverses and extended source change the full transfer. [Spectral atlas](SPECTRAL-ATLAS.md) · [Leaver, Phys. Rev. D 34, 384](https://doi.org/10.1103/PhysRevD.34.384) · [Casals–Ottewill, Phys. Rev. D 87, 064010](https://doi.org/10.1103/PhysRevD.87.064010).

## Positive-line inversion on a chosen finite window

For observation through \(d\), taper the complete returning scalar source in \(u\) only after \(d\). Keep the original graph data and leave the lower hierarchy unchanged. Causality preserves the trace through \(d\). The auxiliary source has bounded retarded-time support, finite left radius and a decaying right tail. Positive scalar energy gives an \(L^2\) news trace and the physical Laplace pairing for \(\Re s>0\). The taper can change the continued residues, so the inverse on this line is compared to the physical untapered pole terms afterwards.

For \(h=e^{sx}m\), \(W=-2sm(+\infty)\). With \(a=m+m_x/(2s)\), \(b=-m_x/(2s)\), the real nonnegative RW potential gives
\[
 (|a|^2-|b|^2)'=V\Re(1/s)|a+b|^2+4\Re s\,|b|^2.
\]
The Jost endpoints give \(|W|\ge2|s|\). In the strong numerator use \(u,\ell=x-X(u)\ge0\), where \(X(u)\) is the graph position. Differentiate at fixed \(\ell\) to include the moving lower limit and the compact graph flux. Two integrations by parts give \(|\widehat N_\chi(\gamma+i\omega)|\le K/|\gamma+i\omega|^2\) for \(|\omega|\ge B\), with \(K\) a weighted source/contact derivative bound. If \(e_j\) encloses the errors of constant approximants on frequency cells of widths \(h_j\) in \([-B,B]\), the weighted Plancherel bound on \(I=[c,d]\) is
\[
 \|N_{21}-\widetilde N\|_{L^2(I)}
 \le e^{\gamma(d-u_*)}
 \sqrt{\frac{\sum_j h_j e_j^2}{2\pi}+\frac{K^2}{3\pi B^3}} .
\]
This inversion includes prompt, pole and continuous contributions together. The missing physical inputs are the weighted source/contact derivative integrals and low-band cells. Exact free-source and unit-barrier controls check the normalization, sign and inversion bound. [Derivation and computable constants](investigators/C-INVERSION.md) · [Manufactured checks](checks/c_inversion.txt).
