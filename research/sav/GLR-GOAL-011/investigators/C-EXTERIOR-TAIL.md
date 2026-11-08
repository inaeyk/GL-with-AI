# Exterior endpoint envelopes for the ordered axial overlap

This is an analytic tail bound with outward-rounded arithmetic, in the \(r_0=1\), \(\ell=2\) conventions of GOAL-011. The only physical amplitudes required are caller-supplied enclosures for \(|u(64)|\), where \(u\) is the GL Schrödinger variable, and \(|\psi(64)|\), where \(\psi\) is the finite-launch CPM field at \(s_2=s-2\Omega\). Here the arguments \(64\) refer to the ray parameter \(\rho\), not the radius. Use \(r=2.5+w\rho\), \(w=e^{i\pi/4}\), and \(dx/d\rho=\gamma=w/f\). The default parameter boxes contain \(k=\pi/5\), \(\Omega\in[0.06328118,0.06328120]\), and the full coordinate rectangle enclosing the certified \(5\times10^{-8}\) pole disk. The massive square root is the outgoing continuation used in the spectral certificate.

## Logarithmic derivatives and amplitudes

For a scalar or matrix homogeneous mode, factor \(E=e^{-\nu x}r^a\), \(\ell=E_x/E=-\nu+a f/r\). The pairs \((\nu,a)\) are \((q_\infty,k^2/(2q_\infty))\) for GL, \((s_2,0)\) for shifted RW, \((s,0)\) for returning RW, and \((\kappa,k^2/(2\kappa))\) for the mixed massive system at \(s_1=s-\Omega\). The full outgoing log derivative is \(P_R=\ell I+N\). If \(Q\) is the homogeneous potential including the squared spectral argument minus \(\ell^2+\ell_x\), then
\[
N(\rho)=-\int_\rho^\infty \gamma(\tau)
 \left[\frac{E(\tau)}{E(\rho)}\right]^2[Q(\tau)-N(\tau)^2]\,d\tau .
\]
Use \(|r|\ge\rho\), \(R=64\), and \(f_b=1+1/R\). An attenuation lower bound for the prefactor is \(a_0=\Re(w\nu)-|\nu|/(R-1)-|a|/R\), and \(|\gamma|\le \gamma_b=1+1/(R-1)\). If \(\|Q(\rho)\|_F\le Q_R(R/\rho)^2\), the weighted ball \(\|N(\rho)\|_F\le m(R/\rho)^2\) is invariant and contractive for \(K(Q_R+m^2)<m\), \(2Km<1\), \(K=\gamma_b/(2a_0)\). The code chooses 1.02 times the smaller invariant-ball root and checks both strict inequalities with Arb. Scalar magnitudes are the \(1\times1\) special case. The exact outgoing branch follows by integrating the factored log derivative and fixing the asymptotic constant. The integrable \(N\) also gives \(\operatorname{Herm}(\gamma P_R)\le -dI\), \(d=a_0-\gamma_bm>0\). The total log derivative satisfies \(\|P_R\|_2\le |\nu|+|a|f_b/R+m\).

The GL \(Q\) bound comes from the pinned rational potential and exact cancellation of the \(1/r\) terms. For the massive \(Q\) bound at \(R=64\), a triangle bound that treats both diagonal derivatives alike is too loose. Put \(j=f/r\), \(c_d=4/(\lambda+k^2r^2)\). The two potential corrections are \([j(2-c_d)]^2-[j(2-c_d)]_x\) and \([j(1+c_d)]^2+[j(1+c_d)]_x\). The second uses \(j^2+j_x=f/r^3\) exactly. The first compares to \(6/r^2\). The common term compares to \((\lambda-k^2-a^2+a)/r^2\) before taking moduli. These exact regroupings give \(Q_R<0.003090065\) in Frobenius norm for the massive pair. The numerical constants below are rounded away from the exact bounds.

| Branch | \(m\) upper bound | \(d\) lower bound | \(\|P_R\|_2\) upper bound |
| --- | ---: | ---: | ---: |
| GL | 0.002083162 | 0.4295119 | 0.6385407 |
| shifted RW | 0.002597643 | 0.2976983 | 0.8095888 |
| returning RW | 0.001994618 | 0.3884193 | 0.7702260 |
| shifted massive | 0.027303964 | 0.04449364 | 0.6453280 |

In particular \(|u(\rho)|\le |u(R)|e^{-d_{\rm GL}(\rho-R)}\) and \(|\psi(\rho)|\le|\psi(R)|e^{-d_2(\rho-R)}\). Their \(x\)-derivatives are bounded by multiplying by their total log derivative caps.

## Physical first insertion and affine tail

Let \(K_{\rm sph}=H_{\theta\theta}/r^2\). The pinned GL reconstruction with \(y=(1+k^2r^3)u/r\) gives the estimates \(|H_{tt}|,|H_{tx}|,|H_{xx}|,|K_{\rm sph}|\le C_i|u|/|r|\), using \(H_{xx}=H_{tt}-2fK_{\rm sph}\) and \(y_r/(1+k^2r^3)=(u/r)[p_{\rm GL}/f+3k^2r^2/(1+k^2r^3)-1/r]\). In the same \((t,x)\) chart the baseline one-form satisfies \(A_t=(f\psi+r\psi_x)/(2r^2)\), \(A_x=s_2\psi/(2r)\), and the density curl is \(Q_z=2r\psi\). The TT weight cancels in the first variation of \(M^{ab}\), and the \(P\) curl block varies by \(3K_{\rm sph}\). Thus \(X^{tx}=6rK_{\rm sph}\psi\), and with \(S_0=\operatorname{diag}(-1,1)\), the two-component source \(Y=-\lambda r^2 S_0 H S_0 A/f\). These are the full cosine first-insertion coefficients in the supplied mixed solver.

The derivative-free source map gives the following component caps over the same physical parameter box:
\[
|F_y|\le 3.27737\,|u\psi|/\rho^2,\quad
|F_z|\le 1.89219\,|u\psi|/\rho,\qquad
|G_y|\le 6.86042\,|u\psi|/\rho^2,\quad
|G_z|\le 0.838663\,|u\psi|/\rho .
\]
For \(d_s=d_{\rm GL}+d_2>0.7272102\) the code therefore bounds
\[
\|G-P_R F\|_2
 \le 0.032297081\,|u(R)\psi(R)|(R/\rho)e^{-d_s(\rho-R)}.
\]
The outward equation for \(L=J-P_RU\) is \(L_\rho=-\gamma P_R L+\gamma(G-P_RF)\), with outgoing \(L(\infty)=0\). Its backward homogeneous operator contracts at least at rate \(d_m=0.04449364\), so
\[
\|L(\rho)\|_2\le 0.042515958\,|u(R)\psi(R)|(R/\rho)e^{-d_s(\rho-R)}.
\]
The signs follow by differentiating \(L\) and using the homogeneous Riccati equation. The boundary constant is an absolute Euclidean-norm cap, so each complex component lies in the corresponding centered disk. For use beyond the affine match, the forward outgoing equation \(U_\rho=\gamma(P_R U+L+F)\) also gives exponential caps for \(U,J\) from a caller-supplied \(\|U(R)\|_2\).

## Reproduction and scope

Run `python3 GLR-GOAL-011/checks/physical_overlap/outer_tail.py` with python-flint 0.9.0. The entry point uses 128-bit Arb/Acb and checks all contraction and positivity inequalities. The `OuterTail` methods `fields(rho,u64,psi64)`, `forcing(rho,u64,psi64)`, `affine_bound(u64,psi64,rho=None)` and `mixed_state(rho,u64,psi64,mixed_u64)` return absolute bounds. The logarithmic derivative records are `gl`, `rw`, `returning` and `mixed`. Independent direct substitution of the rational GL and massive \(Q\) at \(\rho=64,128,512\) checked the stated upper bounds at those points. The global claim uses the analytic inequalities in the implementation, rather than that point check. The calculation does not certify the supplied endpoint amplitudes, the mean source, any compact transport, or the full pole residue.
