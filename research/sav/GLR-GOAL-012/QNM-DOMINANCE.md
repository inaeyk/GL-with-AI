# Pole subtraction and dominance

Write the real news pole pairs
\[
 Q(u)=2\Re[s_q A_{21,q}e^{s_q u}],\qquad
 S_*(u)=2\Re[s_* A_*e^{s_*u}],\qquad s_*=s_q+2\Omega .
\]
For subtraction on the physical continuation sheet, give each exponential a common onset \(u_b\le c\) on a tested interval \(I=[c,d]\), multiplying its news transform by \(e^{-(s-s_{\rm pole})u_b}\). These transforms cancel the local Laurent parts and give the displayed terms throughout \(I\). Subtracting switched *news* avoids an artificial impulse from differentiating a switched shear. The resulting remainder is an exact difference on \(I\). It contains the prompt, other poles, shifted features, cuts and any inverse-contour remainder left after the chosen subtraction. A pole subtraction provides no bound on any of those pieces. [Inversion and contour conditions](CONTOUR-AND-INVERSION.md).

For a term \(Q_z=2\Re[z e^{s u}]\), write \(J(\lambda)=\int_I e^{\lambda u}du\). Its norm is
\[
 \|Q_z\|_2^2=2|z|^2 J(2\Re s)+2\Re[z^2J(2s)] .
\]
Use \(z=s_q A_{21,q}\) and the full inherited residue rectangle with the pole disk. The interval length \(8.407362\) contains at least one fundamental oscillation. The direct launch support ends at \(u_*+0.12\simeq-0.56499274\), and these windows start after that cut. The duration of the full prompt and delayed response requires the waveform. The baseline at order \(\eta\) is a separate contribution. Its ordinary fundamental amplitude for the same canonical bump is approximately \(-0.00055879+0.00065753i\); the \(\epsilon^2\eta\) pole norms alone do not determine the comparison of the full signals at finite seeds. [Baseline normalization](https://github.com/inaeyk/GL-with-AI/blob/75cb3ff3feef98164a65cf14105be6df77ea809c/research/sav/verification/GLR-GOAL-011/AUDIT.md) · [Directed norms](checks/qnm_norm.txt).

A common period \(T_q=2\pi/|\Im s_q|\) translates each pole norm by its exponential factor. The ratio of the translated-pair norm to the fundamental-pair norm increases by \(e^{2\Omega T_q}\in[2.8980999,2.8981015]\) per period. This comparison uses the nonzero shifted excitation on the same sheet and the same fixed clock.

For any duration at least \(T_q\), bound the shifted norm below using the first exact period and bound the returning norm above using \([c,\infty)\). Put \(\gamma_j=-\Re s_j\), \(z_j=s_j A_j\). The Gram formula gives
\[
 \frac{\|S_*\|_{L^2(I)}}{\|Q\|_{L^2(I)}}\ge
 e^{2\Omega c}
 \frac{|z_*|}{|z_q|}
 \sqrt{\frac{(1-e^{-2\gamma_*T_q})(1/\gamma_*-1/|s_*|)}
                  {1/\gamma_q+1/|s_q|}} .
\]
The last factor including \(|z_*|/|z_q|\) exceeds \(1.12241\) over the two certified boxes. Hence the ratio exceeds \(1.04495\) on every window with \(c\ge u_*+0.12\) and duration at least one period. For the specific windows of length \(8.407362\), direct rectangle subdivision yields the following outward-rounded values.

| Start \(c\) | \(\|Q\|_2\) | \(\|S_*\|_2\) | \(\|S_*\|_2/\|Q\|_2\) | Necessary other-term norm for \(\rho=1/2\) |
| ---: | ---: | ---: | ---: | ---: |
| 0 | \([0.5989,0.8761]\) | \([1.0608,1.2211]\) | \([1.2109,2.0388]\) | \(>0.6228\) |
| 2 | \([0.3858,0.5311]\) | \([1.0055,1.1623]\) | \([1.8935,3.0120]\) | \(>0.7399\) |
| 4 | \([0.3027,0.4397]\) | \([0.8709,1.0018]\) | \([1.9810,3.3093]\) | \(>0.6511\) |
| 8 | \([0.1519,0.2190]\) | \([0.7161,0.8234]\) | \([3.2717,5.4169]\) | \(>0.6067\) |

The final column uses the unrounded enclosures in the triangle inequality below. [Arb calculation](checks/qnm_norm.py) · [Output](checks/qnm_norm.txt).

A quantitative test of the full remainder on \(I\) is \(\|N_{21}-Q\|_2<\rho\|Q\|_2\), with \(\rho\le1/2\) the target. If \(\widetilde N\) has a certified error \(E\) on \(I\), use the supremum of \(\|\widetilde N-Q\|_2\) over the pole box plus \(E\) for the upper test. Use its infimum minus \(E\) for a lower test. With certified norm endpoints \(L_*\) for \(S_*\) and \(U_q\) for \(Q\), passing the upper dominance test would require
\[
 \|N_{21}-Q-S_*\|_2>L_*-\rho U_q .
\]
The other components would have to cancel the shifted contribution sufficiently in phase as well as magnitude. No measured finite interval is certified by this necessary condition alone. [Waveform error routes](FINITE-TIME-WAVEFORM.md).

## A late-window obstruction for the formal ordered response

The translated pole lies in \(\Re s>\Re s_q\) and is reachable from the physical half-plane by continuation within that open half-plane. Its residue in the news is \(s_* A_*\ne0\). For a finite \(c\) beyond the subtraction onset let \(I_n=[c+nT_q,c+(n+1)T_q]\). Suppose \(\|N_{21}-Q\|_{L^2(I_n)}\le\rho\|Q\|_{L^2(I_n)}\) for all sufficiently large \(n\) and some finite \(\rho\). The right-hand side decays exactly as \(e^{nT_q\Re s_q}\). Cauchy–Schwarz on each period and a geometric sum give absolute and locally uniform Laplace convergence of the remainder in \(\Re s>\Re s_q\). The compact initial part is entire. By uniqueness of continuation from the physical right half-plane the remainder would be holomorphic at \(s_*\), while subtraction of the \(s_q,\bar s_q\) pair leaves the translated pole. This is a contradiction.

Thus every finite \(\rho\) is exceeded on arbitrarily late period bins for the formal ordered coefficient. More generally the limsup exponential rate of the remainder norms over these bins is at least \(\Re s_*\): a strictly smaller rate would again give a half-plane of Laplace convergence containing \(s_*\). This is a spectral obstruction to perpetual uniform dominance. The proof assigns no first violating bin, controls no specified finite-bin ratio, and has no late-time finite-seed interpretation without uniform nonlinear control. [Continuation and residue](SPECTRAL-ATLAS.md) · [Adversarial review](investigators/E-ADVERSARIAL.md).
