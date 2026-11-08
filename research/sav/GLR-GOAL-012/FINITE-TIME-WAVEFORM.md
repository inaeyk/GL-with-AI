# Finite-time causal waveform and error estimator

The null traces are \(C_{21}/r_0=\lim_{v\to\infty}\Psi_2(u,v)\) and \(N_{21}=\lim_{v\to\infty}\partial_u\Psi_2(u,v)\) in dimensionless \(u\). They vanish before \(u_*=u_\Sigma(13/8)\simeq-0.68499274\). Initial support occupies \([u_*,u_*+0.12]\) in graph cuts, with scattering continuing after those cuts.

On a diamond \(u_*\le u\le U\), \(v_\Sigma(u)\le v\le V_c\), the exact-curl mixed variable \(U_m=-Q_{1z}\) and compact potential \(A_z\) form a two-field hyperbolic system with anti-Hermitian time coupling. The upper position data vanish and their velocities are forced:
\[
 U_{m,t}|_\Sigma=\frac{pJ^t-J^x}{1-p^2},\qquad
 A_{z,t}|_\Sigma=\frac{ik(J^t-pJ^x)}{\lambda r^2(1-p^2)},\qquad
 Q_{2z,t}|_\Sigma=\frac{\mathcal D^x-p\mathcal D^t}{1-p^2},
 \quad p=T'.
\]
The currents use the tortoise density convention. The constrained potentials, mixed insertion, direct variation and fixed-mass mean supply the complete source. The lower causal radius is \(r_\Sigma(U)>r_0\). The finite diamond needs real-radius GL and mean enclosures, including the mass-matched far quadrature. [Construction and normalization](investigators/B-PHYSICAL-SOURCE.md).

For a smooth reconstructed solution let \(R\) be the error source and \(E_\Sigma\) the graph error energy, including any error influx at the early null edge. The nonnegative spatial energy of each stage gives a bound on the largest square root \(M\) of its two directional fluxes on subdiamonds:
\[
 B=\tfrac12(\|R\|_{L^1_vL^2_u}+\|R\|_{L^1_uL^2_v}),\qquad
 M\le \tfrac12\big(B+\sqrt{B^2+4E_\Sigma}\big).
\]
The return flux controls \(L^2(du)\) news error at fixed \(v\). Commuting with \(\partial_t\), retaining the commuted graph data, controls derivative losses in the source. The lower errors enter the upper stages multiplied by the GL and mean coefficient envelopes. The numerical residual needs whole-cell bounds and interface terms for reconstructions that are only piecewise smooth. [Energy and source estimates](investigators/B-PHYSICAL-SOURCE.md#positive-characteristic-energy-and-residual-certification).

A cap \(v=V_c\) yields \(P_c=\Psi_{2,u}(\cdot,V_c)\). For \(D=U-u_*\) and \(r_c=r((V_c-U)/2)\) beyond the potential peak, set
\[
 K_c=\frac D2\left(\frac6{r_c}-\frac{3r_0}{2r_c^2}\right),\qquad
 B_c=\frac14\|S_2\|_{L^1_vL^2_u(v\ge V_c)} .
\]
The null Volterra equation bounds \(\|N_{21}-P_c\|_2\) by \((e^{K_c}-1)\|P_c\|_2+e^{K_c}B_c\). Iterating the potential \(m\) times replaces \(e^{K_c}-1\) with its Taylor remainder after degree \(m\), with quadrature error separate. For \(U\simeq7.84237\), one period after the packet support, \(r_\Sigma(U)\simeq1.01195r_0\). At \(r_c=100r_0\), \(K_c\simeq0.2552\) and the degree-three potential error is below \(2\times10^{-4}\|P_c\|_2\). The physical source cap \(B_c\) needs evaluation. Shear errors follow by integrating from the zero front. [Cap formula](investigators/B-PHYSICAL-SOURCE.md#finite-cap-to-null-infinity).

The causal energy estimator and the positive-line inversion give independent bounds for the same finite-window trace. The former needs evaluated graph and volume residuals and a source cap. The latter needs weighted derivative norms and a certified low band. The complete physical inputs to both methods remain unenclosed. The calculated spectral overlap measures a singularity of the untapered trace. [Inversion method](CONTOUR-AND-INVERSION.md#positive-line-inversion-on-a-chosen-finite-window).

For finite seed, let \(M_U\) bound the regular GL first jet and the derivatives relevant to the interaction on these causal cuts. Necessary conditions include \(|\epsilon|M_U\ll1\), small axial seed in the corresponding norms, and \(|\epsilon\chi_\infty|/r_0\ll\delta\) for a perturbative shift of this narrow packet. At fixed radius the GL coefficients grow as \(e^{\Omega v}\). A finite window samples bounded \(v\) at each fixed radius and relies on the spatial GL decay at its far end. A quantitative finite-amplitude claim needs a uniform nonlinear remainder and specified seeds.
