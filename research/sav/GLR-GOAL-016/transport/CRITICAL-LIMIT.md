# The critical mass–radius limit for the pinned Z signal

This is an independent Riemann-kernel derivation for the **same** fixed-source solution used for the finite-radius certificate. The [verification route](../verification/REVIEW.md) uses a weighted first-order evolution and also addresses fixed positive mass. This derivation needs neither a completed temporal history nor a frequency band hypothesis.

Use \(r_0=1\), \(u_1=-8\), \(A=R(u_1)\), and \(x=r+\log(r-1)\). Let \(X_\alpha\) be the weighted sup norm in \(u\le u_1\) at fixed \(v\), with \(\|h(v)\|_\alpha=\sup e^{-\alpha(u-u_1)}|h(u,v)|\). Choose \(\alpha=s=2\Omega>0\). Extend the field by zero below the Cauchy graph and write \(F_*(u,v)=\mathbf 1_{v\ge R(u)} e^{s(u+x)}S(r)\). Zero value and both zero null derivatives eliminate the graph distributions. In \(u\le u_1,\ v\ge0\), the extended equation is
\[
 4(\psi_\mu)_{uv}+\mu^2\psi_\mu=F_*-V_0\psi_\mu+\mu^2(1-f)\psi_\mu=:G_\mu .
\]
Both the retarded trace at \(u=-\infty\) and the trace at \(v=0\) vanish in the relevant class. In the exact massless problem the limit of \((\psi_0)_u\) is \(n_Z\). It is exponentially weighted on \(u\le u_1\).

## Uniform bounds before scaling

The decaying GL end gives \(\|F_*(v)\|_\alpha\le P(v)\) with \(\int_0^\infty(1+v)P(v)dv<\infty\) and \(P(v)=O(e^{-c v})\) for some \(c>0\). Indeed \(2q-s>0\), the remaining \(e^{s(x-r)}\) is polynomial, and \(r\ge(v+8)/4\) since \(x\le 2r\) and \(u\le-8\). This last inequality also gives
\[
 Q(v)=64(v+8)^{-3}\ge |V_0|,\qquad W(v)=4/(v+8)\ge |1-f|.
\]
Write \(P_1=\int P\), \(Q_1=\int Q=1/2\), and \(Q_*=\int vQ(v)dv=4\), where integrals without limits in this section run from \(0\) to \(\infty\).

Let \(\mathfrak J(z)=J_0(\sqrt z)\), \(z\ge0\). Its derivative bounds \(|\mathfrak J|\le1,\ |\mathfrak J'|\le1/4,\ |\mathfrak J''|\le1/32\) follow from the integral representation of \(J_\nu\) for \(\nu=0,1,2\) and \((z^{-\nu/2}J_\nu(\sqrt z))'=-z^{-(\nu+1)/2}J_{\nu+1}(\sqrt z)/2\). In particular the constant-mass solution formula is
\[
 \psi_\mu(u,v)=\frac14\int_{-\infty}^u da\int_0^v dy\,
 \mathfrak J\!\left(\mu^2(u-a)(v-y)\right)\,G_\mu(a,y).
 \tag{1}
\]
The Bessel equation verifies the homogeneous interior equation and the unit value on each characteristic gives the source contact. Weighted absolute integrability and iteration on finite \(v\) give uniqueness. The \(u\) integral has norm at most \(1/\alpha\).

On \(\mu^2v\le M\), Grönwall in \(v\) gives
\[
 \|\psi_\mu(v)\|_\alpha \le B_M
 =\frac{P_1}{4\alpha}\exp\!\left(\frac{Q_1+M/2}{4\alpha}\right).
\]
Here \(\mu^2\int_0^v W(y)dy=4\mu^2\log(1+v/8)\le M/2\). Subtract the two exact equations and apply the **massless** integral kernel to \(-V_0(\psi_\mu-\psi_0)-\mu^2 f\psi_\mu\) to get
\[
 \|\psi_\mu(v)-\psi_0(v)\|_\alpha\le \mu^2 v\,D_M,\qquad
 D_M=\frac{B_M}{4\alpha}e^{Q_1/(4\alpha)}.
 \tag{2}
\]
The exponent \(M\) here only controls a compact set of scaling parameters.

## Field, news and signed tube flux

If \(v\to\infty,\ \mu\to0,\ \mu^2v\to 2\Lambda\ge0\), (1)–(2) give
\[
 \Psi_\Lambda(u)=\int_{-\infty}^u
   J_0\!\left(\sqrt{2\Lambda(u-a)}\right)n_Z(a)\,da ,
 \qquad N_\Lambda=\partial_u\Psi_\Lambda .
 \tag{3}
\]
To prove this, replace \(G_\mu\) by \(G_0\) in (1). The \(v\)-integrated weighted source error is at most
\[
 \mu^2\left[D_M Q_*+4 B_M\log(1+v/8)\right].
\]
It tends to zero. For \(y\le v\), the kernel argument change is bounded by \((u-a)(|\mu^2v-2\Lambda|+\mu^2 y)\). The weighted first moment of \(G_0\) in \(y\) is finite by \(P,Q\) and \(B_M\). Also \(\int_v^\infty\|G_0(y)\|_\alpha dy=O(v^{-2})\). Thus one can extend the replaced integral to \(y=\infty\) and use \(\int_0^\infty G_0(a,y)dy=4n_Z(a)\). The lower limit remains the actual Z slice through \(F_*\).

For \(\psi_u\), retain the endpoint \(a=u\), with kernel equal to one. The interior factor is \(c\,\mathfrak J'(c(u-a))\), \(c=\mu^2(v-y)\), whose derivative with respect to \(c\) is bounded by \(1/4+M(u-a)/32\) when \(c,2\Lambda\le M\). This proves convergence of \(\psi_u\) with the same error rate. Specifically the errors in \(\psi\) and \(\psi_u\) in \(X_\alpha\) are
\[
 O_M\!\left(\mu^2[1+\log(1+v)]+|\mu^2v-2\Lambda|+(1+v)^{-2}\right)
 \tag{4}
\]
with constants from the displayed integrals. For \(\psi_v\) the upper-\(y\) term is bounded by \((P(v)+[Q(v)+\mu^2 W(v)]B_M)/(4\alpha)\) and the derivative of the kernel costs at most \(\mu^2/(16\alpha^2)\int_0^v\|G_\mu(y)\|_\alpha dy\). Hence \(\psi_v\to0\). Each bound applies to a fixed \(v\) and all \(u\le u_1\), so it applies uniformly to the sloping tube \(v=u+2x(R)\) for \(u\in I\), with \(\Lambda=\lim\mu^2x(R)\).

Expanding the Bessel function in (3) and integrating its series under the exponential weight yields \(N_\Lambda=\exp[-(\Lambda/2)J]\,n_Z\), where \(Jh(u)=\int_{-\infty}^u h(a)da\). For instance, \(\int_{-\infty}^u(u-a)^j n_Z(a)da=j!J^{j+1} n_Z\), and differentiation removes one \(J\). This verifies the first-order evolution result by a distinct integral representation.

The physical tube flux has integrand \(2[(\psi_u)^2-(\psi_v)^2+(f/R)\psi(\psi_u+\psi_v)]\) when normalized as an energy fraction of \(M_0\) and divided by \(\epsilon^4\). It follows from the stress tensor of the canonically normalized scalar \(\phi/\sqrt{4\pi G_4}\), with \(\psi=r\phi\). The last term is the physical boundary term relative to the master current. Equations (3)–(4) imply
\[
 \lim_{\mu\to0,\ R\to\infty,\ \mu^2x(R)\to\Lambda}\mathcal K_{\mu,R}(I)
 =2\int_I N_\Lambda(u)^2\,du.
\]
The massless coefficient occurs at \(\Lambda=0\). The field and waveform can retain a finite mass effect in a joint limit even though the mass correction to the local potential tends to zero. The formula is for the prescribed-source scalar problem on Schwarzschild. Its physical scales are \(m_\phi r_0=\mu\) and \(x(R)=x_{\rm phys}(R)/r_0\).
