# Physical scattering channel

Use \(x=r_*\), \(a=K^2\) with \(r_0=1\). For real \(\omega>K\), \(p_\omega=\sqrt{\omega^2-K^2}\) and \(V_K=a-a/r+O(r^{-2})\) give
\[
 u_I\sim e^{i[p_\omega x+a\log r/(2p_\omega)]},\qquad
 u_H\sim e^{-i\omega x}\quad(x\to-\infty).
\]
The first asymptotic is at spatial infinity and the second at the future horizon. The phases are multiplied by \(e^{-i\omega t}\). For \(u_H=A_{\rm in}\overline u_I+A_{\rm out}u_I\), radial current conservation gives \(p_\omega(|A_{\rm in}|^2-|A_{\rm out}|^2)=\omega\), hence \(W(u_H,u_I)=2ip_\omega A_{\rm in}\) and \(|W|\ge 2\sqrt{\omega p_\omega}\). For \(0<\omega<K\), the decaying right end has zero current and cannot be proportional to the ingoing horizon solution. At zero, a bounded horizon and decaying right solution would have zero boundary terms in the positive ground-state form and would vanish. These observations give no location of resonance poles off the physical axis.

For a fixed positive band above threshold, one can bound the outgoing endpoint by the WKB pair using local \(k_\omega(x)=\sqrt{\omega^2-V_K}\) on a sufficiently far tail. The first-order mixing system has norm controlled by \(\int |k_\omega'|/(2k_\omega)\,dx\). Its backward Volterra error is at most \(\exp(\int |k_\omega'|/(2k_\omega)\,dx)-1\). The bound scales as \(O((p_\omega^2 R)^{-1})\) for a fixed band. The horizon potential is exponentially decaying and allows the corresponding ingoing Volterra construction. The group delay at fixed \(\omega\) differentiates the phase, giving \(\omega x/p_\omega-a\omega\log r/(2p_\omega^3)\) plus the scattering phase derivative. A massive packet can show subluminal group delay while the causal front follows null characteristics.

## Unit-current threshold

On a sufficiently large exterior \(k_\omega^2\ge p_\omega^2+c/r\) with \(c>0\), uniformly for \(\omega\ge K\). Set \(\ell' = k_\omega\), \(u=k_\omega^{-1/2}w\). The transformed equation is \(w_{\ell\ell}+(1+\mathcal Q)w=0\), with \(\mathcal Q=3(k_\omega')^2/(4k_\omega^4)-k_\omega''/(2k_\omega^3)\). Since \(V_K'=O(r^{-2})\) and \(V_K''=O(r^{-3})\), \(\int_R^\infty|\mathcal Q|\,d\ell=O(R^{-1/2})\) uniformly on the upper threshold. Volterra propagation of \(w\sim e^{i\ell}\) fixes a unit-current outgoing solution, locally continuous to \(p_\omega=0\) with a phase fixed at a finite radius. At the threshold its leading form is proportional to \(r^{1/4}e^{2iK\sqrt r}\) in these units. The unit-current Wronskian bound is \(2\sqrt{\omega}\). This controls an upper-threshold construction. Analytic continuation below the branch point, turning-point asymptotics, and uniform time-domain tails of the Z transient still require work.

## Projection quadrature

For \(\lambda=-i\omega+0\), let \(G_\omega=e^{-i\omega\phi}[\mathfrak F_0/(-i\omega-s)+A(-i\omega\psi_0+\psi_1)-B\psi_0]\) with the variables of [derivations](DERIVATIONS.md#finite-slice-green-function). After subtracting the spatially decaying separated pole, the outgoing coefficient of the derivative master is
\[
 C_+(\omega)=-W^{-1}\int_{\mathbb R}u_H(\omega,x)G_\omega(x)\,dx .
\]
For polynomial initial tails, take the real-axis limit at fixed \(x\) in the split Green kernel before the outgoing limit. This avoids assuming convergence of the unsplit overlap away from the real axis. The smooth horizon jet gives \(G_\omega=O(f)\) there. The damped preparation gives enough far decay for absolute convergence on a compact positive band, with \(p_{\rm d}=8\) a proposed representative. For the original \(O(r^{-2})\) metric and velocity tail, absolute overlap convergence needs additional control, for example an oscillatory or weak spectral treatment. Neither family's overlap was evaluated.

The physical energy requires the polarization normalization in [energy](ENERGY.md). Near-zone source-bound fields, evanescent components and a finite time Fourier window do not supply this outgoing projection. With continually growing forcing, the mathematical scattering interpretation is for the homogeneous remainder after the fixed separated particular solution is removed. It gives no infinite-time finite-amplitude Einstein prediction.

## Primary literature checks

- [Brito–Cardoso–Pani, v3, Eqs. (26)–(30), Appendix B](https://arxiv.org/html/1304.6725v3) treats the homogeneous TT monopole. Its temporal-frequency reconstruction does not give a forced or static lift by substitution. [Collingbourne, v1, Props. 3.4, 3.7 and 4.1](https://arxiv.org/pdf/2007.08441v1) covers existence of the first GL instability in a band including \(k r_0=\pi/5\). Positivity at the second harmonic follows separately from the pinned supersolution.
- [Ould El Hadj, v2, Eqs. (7)–(9), (33)–(38), (67)–(70), and (172)–(181)](https://arxiv.org/html/2411.16538v2) uses a four-dimensional conserved particle source. The KK block instead has compact flux terms in its divergence. The plotted monopole quasinormal masses do not reach this \(K\). Extra branches in a reduced system require a full reconstruction and endpoint check.
- [Pasqualotto–Shlapentokh-Rothman–Van de Moortel, v2, Theorems I and 3.1](https://arxiv.org/html/2303.17767v2) gives the massive scalar threshold tail under hypotheses including compact support and pointwise potential positivity. [Konoplya–Zhidenko, v3, Eq. (1), Fig. 3](https://arxiv.org/html/2307.01110v3) gives numerical tail evidence in the homogeneous spin-2 monopole at a different mass. [Décanini–Folacci–Ould El Hadj, v3, Eqs. (3)–(6), (13)](https://arxiv.org/pdf/1602.03460v3) illustrates data-dependent excitation in the odd dipole. These results are evidence for mechanisms, with no transferred tail or pole theorem for either Z preparation.
