# Certified exterior tail of the weak returning numerator

Use \(r_0=1,\ell=2,\lambda=4\), \(r=2.5+e^{i\pi/4}\rho\), \(R=64\), and the physical boxes of the existing exterior Riccati envelope. The bound covers \(\rho\ge R\) only. The four inputs are certified caps on the endpoint moduli of the GL master \(u\), shifted baseline CPM \(\psi\), returning adjoint \(h\), and Euclidean norm of the normalized massive position \(U=(Y,Z)\). The horizon-normalized \(h\) is outgoing at the exact simple RW zero. The envelope constants are uniform on the pole rectangle, but applying an outgoing envelope to an off-pole ingoing solution would be invalid.

## Weak integrand and one-form bounds

The three source matrices and curls are those of PHYSICAL-RESIDUE. It is convenient to bound the contraction in the \((t,x)\) basis. From \((v,r)\) to \((t,x)\) the vector density transforms as \(D_{tx}=fJ D_{\rm EF}\), \(J=\partial(t,x)/\partial(v,r)\), while the adjoint one-form transforms by \(J^{-T}\). Thus the density Jacobian cancels \(dx=dr/f\), and the antisymmetric curl density satisfies \(\mathcal C^{tx}=\mathcal C^{vr}\). In the weak integrand in \(dr\) the current term can be computed as \(2\lambda r^2 a^\dagger\cdot ZgA\) with \(g=\operatorname{diag}(-1/f,1/f)\). The curl term is \(-2h\mathcal C/r^3\). There is no extra Jacobian from \(r(\rho)\), since \(|dr/d\rho|=1\).

The adjoint coefficients in \((t,x)\) are \((fh+rh_x,-srh)/(\lambda r^2)\), and the baseline coefficients are \((f\psi+r\psi_x,s_2r\psi)/(2r^2)\). Their Euclidean norms are bounded by \(C_a|h|/|r|\) and \(C_0|\psi|/|r|\) using the outgoing log derivative bounds. The static GL reconstruction gives \(\|X\|_F\le C_X|u|/|r|\), \(X=gH\), and \(|S|\le C_S|u|/|r|\). The code evaluates the same rational reconstruction bounds as the first-insertion tail, including \(H_{xx}=H_{tt}-2fS\).

The massive outgoing field obeys \(U_\rho=\gamma(P_RU+L+F)\), with \(\operatorname{Herm}(\gamma P_R)\le-d_m I\), \(L=J-P_RU\), and both \(F,L\) decaying at the faster rate \(d_s=d_{\rm GL}+d_2\). Variation of constants gives the common caps
\[
U_*=\|U(R)\|+\frac{\gamma_b(F_0+L_0)|u(R)\psi(R)|}{d_s-d_m},\qquad
J_*=p_m U_*+L_0 |u(R)\psi(R)|
\]
times \(e^{-d_m(\rho-R)}\). Here \(F_0,L_0,p_m\) are the endpoint coefficients supplied by the existing tail implementation. To recover the physical mixed one-form, use \(d=\lambda+k^2r^2\), \(\sqrt D=r\sqrt d\), \(a=(\log\sqrt D)_x\), \(b=(\log\sqrt{W_c})_x\), \(\gamma_k=ikr/2\). In particular
\[
A_{1t}=-(J_Y+aY+\gamma_k s_1 Z)/\sqrt D,\qquad
A_{1x}=-s_1Y/(r\sqrt d)-ik(J_Z-bZ)/(2\sqrt d)-Y^x/(\lambda r^2).
\]
The second formula uses the exact cancellation between the two \(Y\)-terms in the flux reconstruction. For the mixed curl, \(r^4F_{1,vr}=\sqrt D\,Y+X^{tx}_{\rm insertion}\) with \(|X^{tx}_{\rm insertion}|\le C_{\rm curl}|u\psi|\). These relations give the HJ bound after integrating the rate \(d_h+d_{\rm GL}+d_m\) and bounding all negative powers of \(|r|\) by those of \(R\).

For the direct term, \(|\operatorname{tr}X^2|\le\|X\|_F^2\) holds also on the complex ray by Cauchy–Schwarz on the transposed entries. Thus the coefficients \(Z_Q,W_Q\) have absolute caps \((5C_X^2/4+C_S^2/2)/2\) and \((C_X^2/4+7C_S^2/2)/2\), respectively, multiplying \(|u|^2/|r|^2\). In the curl density use \(Q_{0z}=2r\psi\). The integrated decay rate is \(d_h+2d_{\rm GL}+d_2\). All compact mean factors \(1/2\) in HJ and H²P are already included.

## Mean EF quadrature and conversion to static time

Put \(n=e^{-\Omega x}u\). On every unit complex \(r\) disk centered on the tail, the regular system for \((n,n_r)\) has row-sum norm bounded by one. Its centered data obey \(|n_r|\le|n|\) by the GL Riccati cap. Gronwall along a radial segment within the disk and the rational EF reconstruction give the componentwise Cauchy bound \(|H|,|H_r|,|H_{rr}|\le64|r n|\) for the coefficients multiplying \(e^{\Omega v}\). The code rechecks the bounds in mean_certificate, with actual uniform jet coefficient below \(20.698\). It separately majorizes the exact component expressions in mean_series, including \(Q_{\theta\theta}/r^2\) before taking the supremum. This gives \(|\mathcal T^v{}_v|,|\mathcal T_{rr}|<0.794 M^2\), \(M=64|r n|\). Retain the deliberately looser inherited \(128M^2\) for the quadrature.

Writing \(a_n=d_{\rm GL}+\Omega(\Re w-1/(R-1))\), \(w=e^{i\pi/4}\), gives an exponential rate bound for \(n(\tau)\) relative to \(n(\rho)\) for every \(\tau\ge\rho\ge R\). The two EF mean radial integrands are bounded by \(64^3(\tau+2.5)^4|n(\tau)|^2\). Since \((\tau+2.5)/(\rho+2.5)\le e^{(\tau-\rho)/(R+2.5)}\), their tails at a variable \(\rho\) are bounded by
\[
\frac{64^3(\rho+2.5)^4 |n(\rho)|^2}{2a_n-4/(R+2.5)}.
\]
Multiply by \(e^{2\Omega\Re x(\rho)}\) to convert to the static mean coefficients. This cancels the factor in \(|n(\rho)|^2\) exactly. Consequently \(|\mu|,|\beta|\le B_*e^{-d_B(\rho-R)}\) with \(d_B=2d_{\rm GL}-4/(R+2.5)>0\) and \(B_*=64^3(R+2.5)^4|u(R)|^2/(2a_n-4/(R+2.5))\).

In the \((t,x)\) basis the mean lower block has a constant-entry part \(2\mu/r\) and beta entries \((-2f,-f,-f,0)\beta\). Thus \(\|Z_B\|\le|\beta|+(4|\mu|/|r|+\sqrt6 f_b|\beta|)/f_{\min}\). The curl is \(-2r\beta\psi\). Integrate both caps with total rate \(d_h+d_2+d_B\). There is no additional compact factor on the mean term, because its source already includes the Fourier mean.

## Reproduction and numerical bound

Run `python3 GLR-GOAL-011/checks/physical_overlap/return_tail.py` with python-flint 0.9.0. The input-order API is `returning_tail(u64, psi64, h64, mixed_u64, tail=None)`. It returns the three absolute integrals and their total as Arb caps. The representative replay uses 128-bit precision and upward-rounded physical endpoint caps \(3.346\,10^{-13},6.881\,10^{-11},1.262\,10^{-11}\), together with the supplied \(\|U(64)\|\le1000\).

| Weak source | Absolute integral upper bound |
| --- | ---: |
| \(HJ\) | \(1.635104\,10^{-22}\) |
| \(H^2P\) | \(7.864317\,10^{-50}\) |
| \(B_EP\) | \(1.752993\,10^{-33}\) |
| Sum | \(1.636\,10^{-22}\) |

The output certifies the tail conditional on the supplied cutoff enclosures and the ordered branch conventions. It does not enclose the compact overlap or establish the supplied massive endpoint bound.
