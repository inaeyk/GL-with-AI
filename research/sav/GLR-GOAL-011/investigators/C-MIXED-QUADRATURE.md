# Forced massive overlap solve

Empirical 1D diagnostic for the fixed GOAL-011 axial intermediate, \(r_0=1,\ell=2,\lambda=4\), with the Fourier \(+k\) polarization representing the full cosine amplitude. The caller implements compact means separately. The incoming branch at the left and outgoing branch on the right are the continued branches of the earlier spectral calculation.

## Signs and normalized source map

In the product tortoise chart define total \(U=-P^{tx}=r^4F_{tx}/f-X^{tx}\) and \(B=P^{xz}=r^4(A_z'-ik A_x)\), with prime \(d/dx\). Here \(P\) denotes the full linearized flux density at this order. Then \(P^{tz}=-r^4(s_1 A_z-ik A_t)\) and the three density equations give
\[
\begin{split}
A_x&=(-s_1U-ikB-Y^x)/(\lambda r^2),\\
U'&=-r^2(\lambda+k^2r^2)A_t-ik r^4s_1A_z+Y^t,\\
A_t'&=s_1 A_x-f(U+X^{tx})/r^4,\\
A_z'&=B/r^4+ik A_x,\qquad
B'=(s_1^2r^4+\lambda f r^2)A_z-ik r^4s_1 A_t.
\end{split}
\]
No derivative of the supplied source densities enters these equations. For conditioning use \(d=\lambda+k^2r^2\), \(D=r^2d\), \(W_c=\lambda r^4/d\), \(a=(\log\sqrt D)'\), \(b=(\log\sqrt W_c)'\), \(c=ik/(\sqrt\lambda\sqrt d)\), and \(\gamma=ik r/\sqrt\lambda\). The canonical positions are \(y=U/\sqrt D,\ z=\sqrt W_c A_z\), with homogeneous-slope variables \(j_y=-\sqrt D A_t-a y-\gamma s_1z,\ j_z=B/\sqrt W_c+bz-\gamma s_1 y\). Write the two positions as \(m\), the slopes as \(j\), and use the previously derived homogeneous matrix \(V_\mathrm{eff}=\operatorname{diag}(V_Y,V_Z)-s_1 C\sigma_1\). Then
\[
m'=j+F,\qquad j'=(s_1^2+V_\mathrm{eff})m+G,
\]
\[
F=(Y^t/\sqrt D,-cY^x),\qquad
G+s_1F=\left(\frac{s_1(Y^x+Y^t)-aY^t}{\sqrt D}
 +\frac{f\sqrt D\,X^{tx}}{r^4},
 -c[s_1(Y^t+Y^x)+bY^x]\right).
\]
Using \(G+s_1F\) preserves the regular-horizon cancellation. The regular source \(F_e=e^{-s_1x}F\) is bounded and \(e^{-s_1x}(G+s_1F)=O(f)\).

## Endpoint construction and evaluation

Factor \(E=e^{s_1x}\) at the left, \(E=e^{-\kappa x}r^{k^2/(2\kappa)}\) on the right. Write \(m=E m_0,\ j=E(\ell m_0+n)\), \(\ell=E'/E\). The solver transports two homogeneous columns and one particular on each leg and matches all four components at \(r_\mathrm{match}\). In the horizon particular, set \(m_0(1)=0\). Its first Frobenius coefficient follows from
\[
m_0''+2s_1m_0'-V_\mathrm{eff}m_0=F_e'+e^{-s_1x}(G+s_1F).
\]
The code estimates \(\partial_r F_e(1)\) from two local source samples and initializes \(n=m_0'-F_e\). On the far ray the particular starts at zero and the homogeneous factor at \((m_0,n)=(I,0)\) at the declared truncation. The far source must be analytic along the ray and decay faster there than the factored outgoing homogeneous solution. The shifted fundamental and GL source satisfy the endpoint rate check in the earlier derivation. Using finite factor expansions gives empirical truncation error, even if the ODE integrator reports a much smaller local tolerance.

The interface is ` MixedSolution(source, s1, k, rmatch, ray, length, left_delta, tol) `. The source callable returns `(Xtx,Yt,Yx)` at real or complex \(r\), including its radial exponential. A real `ray` is an angle in radians, and a complex `ray` specifies a unit direction. `mixed(r)` returns `(At,Ax,Ftx,Az)` for real \(r\ge1+\mathrm{left\_delta}\). `mixed.ray_fields(a)` evaluates the same tuple on the matched ray at \(r=r_\mathrm{match}+\mathrm{ray}\,a\). `prepare_right(rmax)` precomputes a dense real outer leg if needed. `sigma_min` measures the numerically matched homogeneous subspace separation and `match_error` measures only the algebraic junction residual.

## Manufactured checks

Run `python3 GLR-GOAL-011/checks/physical_overlap/overlap_mixed.py` with Python 3.14.8, NumPy 2.4.6 and SciPy 1.18.0. Both tests use \(s_1=-0.24120619-0.747343i\) and \(k=2\pi/10\). The first sets \(A_z=0\), manufactures compact \(P=r^4A_x\) with \(A_t=P'/(s_1r^4)\) so that the compact equation is exact, and derives the two sources from the original density equations. Errors in all four returned fields are \(1.83\,10^{-10}\) at \((\delta,\mathrm{length},\mathrm{tol})=(10^{-4},48,2\,10^{-10})\) and \(1.24\,10^{-12}\) at \((10^{-6},96,2\,10^{-12})\). The finer minimum matching singular value is \(0.0881836155\), reproducing the homogeneous point check.

The second keeps the same manufactured polarization and uses analytic \(P=e^{s_1 x-2(r-1)}\), \(X^{tx}=0.3P\), testing the forced horizon initializer and nonzero complex-ray tail. The real/ray maximum errors at the two settings are \(8.90\,10^{-4}/6.25\,10^{-5}\) and \(8.22\,10^{-7}/5.77\,10^{-8}\). The contraction is consistent with a first-order Frobenius truncation error amplified by the unwanted horizon branch, giving a scale \(\delta^{2+2\Re s_1}\) for these smooth manufactured sources. This scale is not a physical-source error enclosure. Neither check evaluates the physical overlap or validates error bounds for the caller's source coefficients.
