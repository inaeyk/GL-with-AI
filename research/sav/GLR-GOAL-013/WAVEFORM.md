# Finite-window news and pole comparisons

All times are in units of \(r_0\). The physical tensor shear in the selected harmonic is \(r_0(\eta C_{01}+\epsilon^2\eta C_{21})\), with dimensionless news \(N_{ij}=\partial_u C_{ij}\). The retarded front on the fixed graph is \(u_*=-0.6849927415085289\ldots\). The retarded response includes the prompt propagation, the massive \(+k,-k\) pair and the returning Regge–Wheeler field. Its forcing includes the fixed-mass mean and the quadratic direct term. [Source ledger](SOURCE-LEDGER.md).

## Certified leading packet waveform

The two-scatter formula in [Flux](FLUX.md) gives a reproducible approximation to the leading news \(N_{01}\) directly at null infinity. The full packet error is \(<2.831\times10^{-4}\) in \(L^2(du)\), and the first-eighth error is \(<2.491\times10^{-10}\). Both are absolute errors in the normalized dimensionless master news. These bounds include RW scattering through a controlled remainder.

## Returning-waveform gate

No complete candidate \(n_{21}\) with an evaluated \(L^2\) error exists in this result. A compact candidate must carry the forced graph jets and the all-cell bulk residual of the baseline, the two coupled exact-curl fields and the sourced return. The physical graph calculation certifies a nonzero mixed launch and the cap tests certify transfer factors. The physical exterior forcing is bounded beyond \(r_c=400r_0\) by \(1.7\times10^{-6}\) in the cap source norm. The compact return candidate and its error at that radius remain open. The positive-line alternative has a denominator estimate but lacks whole-frequency cells for the physical nested source and an evaluated high-frequency constant. [Finite-cut bounds](FINITE-CUT-BOUNDS.md).

## Central QNM tests

On \(I=[0,8.407362]\), use the physical pole pairs
\[
 Q=2\Re(s_q A_{21,q}e^{s_qu}),\qquad
 S_*=2\Re(s_* A_*e^{s_*u}),\qquad s_*=s_q+2\Omega .
\]
Use a common news onset \(u_b\le0\) in their transform subtractions. Both pairs then agree with the displayed functions on all tested cuts. A late taper of the causal scalar source for an auxiliary inverse can change its continued residues. The comparison uses the untapered physical pole pairs.

The Work bound for \(\|Q\|_2\) is \([0.59933,0.87548]\). The new directed calculation gives \(\|Q+S_*\|_2\in[1.51525,1.99487]\) including the inherited pole and residue boxes. The midpoint template radii satisfy \(\|Q-Q_{\rm mid}\|_2<0.148061\) and \(\|Q+S_*-(Q+S_*)_{\rm mid}\|_2<0.239806\).

For \(P=Q,Q+S_*\), let \([L_P,U_P]\) bound its norm, \(\Delta_P\) its midpoint uncertainty, and \([d_l,d_h]\) enclose \(\|n_{21}-P_{\rm mid}\|_2\). Once the absolute waveform error \(E_{21}\) is evaluated,
\[
 D(P)\in\left[\frac{\max(0,d_l-E_{21}-\Delta_P)}{U_P},
                   \frac{d_h+E_{21}+\Delta_P}{L_P}\right].
\]
An upper endpoint strictly below \(1/2\) establishes dominance at the specified tolerance. A lower endpoint at least \(1/2\) rejects it. The template radii alone leave margins \(0.151604\) and \(0.517819\), respectively, in the sufficient upper tests \(d_h+E_{21}<L_P/2-\Delta_P\). The numerator was not certified for either test. Prompt and continuum contributions can cancel a pole template on the finite window. The available estimates do not decide such cancellation.

[Source spectral dossier](../GLR-GOAL-012/SPECTRAL-ATLAS.md) · [Source Work window audit](https://github.com/inaeyk/GL-with-AI/blob/ae8c323e1e87abd0fff26eac75b05b701a7e7b1a/research/sav/verification/GLR-GOAL-012/WINDOWS.md) · [Directed template output](checks/d_template_norms.txt).
