# GOAL-011 regular mixed horizon system, adverse check E

Independent EF reconstruction after seeing the prior continued overlap
and the supplied state variables. Use \(r_0=1\), \(f=1-1/r\),
\(t_h=r-1\), \(\lambda=4\), dependence \(e^{\sigma v+ikz_{\rm KK}}\)
with \(\sigma=s-\Omega\), and write \(z=A_z\).
The background orbit inverse has \(b^{vv}=0\), \(b^{vr}=1\),
\(b^{rr}=f\), \(b^{zz}=1\) and \(\sqrt{|b|}=1\).
Define \(p^{ab}=r^4 F^{ab}\), and let the only prescribed curl
density be \(X^{vr}=X\). The source convention is
\[
 \partial_b(p+X)^{ba}-\lambda r^2A^a=Y^a,\qquad Y^z=0.
\]
For \(u=-p^{vr}\) put \(U=u-X\). If \(P^{vr}\) denotes the
already completed density, then \(U=-P^{vr}\) directly.
This distinction fixes the sign of \(X\) in the following.

## Equations and horizon data

Let \(j=p^{rz}\), \(a=A_v\), and set
\[
 G=a+\frac{\sigma U+ikj+Y^r}{\lambda r^2},\qquad
 K=j-r^4(\sigma z-ik a).
\]
The \(r\) equation and the definition of \(j\) give
\(f A_r=-G\), \(f p^{vz}=K\). In detail,
\(p^{vr}=-r^4F_{vr}\),
\(p^{vz}=r^4F_{rz}\), and
\(j=r^4(F_{vz}+f F_{rz})\).
Use the \(v,z\) density equations and the remaining two curl
definitions to obtain
\[
\begin{split}
 f U'&=fY^v+ikK-\lambda r^2G,\\
 f j'&=f\lambda r^2z-\sigma K,\\
 f a'&=-\sigma G-f(U+X)/r^4,\\
 f z'&=K/r^4-ikG .
\end{split}
\tag{1}
\]
Primes are ordinary \(r\) derivatives of the EF radial amplitudes.
These equations include the compact polarization and preserve the
sourced divergence by their original density form. In particular
reconstructing \(A_r=-G/f\) from the regular series below avoids
subtracting two static-chart quantities at a numerical horizon.

At \(r=1\), smoothness requires \(G(0)=K(0)=0\), where the
argument is \(t_h\). Choose arbitrary \(U_0,z_0\) and set
\[
 a_0=-\frac{\sigma U_0+ik\sigma z_0+Y^r_0}{\lambda+k^2},
 \qquad
 j_0=\sigma z_0-ik a_0 .
\tag{2}
\]
The conditions are also the order-zero compatibility conditions
for (1) and the source. To read the homogeneous indicial matrix,
write \(t_h w'=B(t_h)w+b(t_h)\), \(w=(U,j,a,z)\), and let \(G_0,K_0\)
denote the two horizon linear forms excluding \(Y^r_0\).
Then \(B_0=c_G G_0+c_K K_0\), with
\(c_G=(-\lambda,0,-\sigma,-ik)^{\mathsf T}\) and
\(c_K=(ik,-\sigma,0,1)^{\mathsf T}\).
They satisfy
\(G_0(c_G)=K_0(c_K)=-2\sigma\) and
\(G_0(c_K)=K_0(c_G)=0\).
Thus the indicial roots are \(0,0,-2\sigma,-2\sigma\),
semisimple for the stated nonzero \(\sigma\).
The \(t_h^{-2\sigma}\) branch can have bounded state variables
at the target because \(0<\Re(-2\sigma)<1\).
It fails smooth ingoing EF regularity and its associated
\(A_r\) is generally singular. A boundedness criterion alone
therefore selects the wrong horizon solution space.

For analytic sources in the support-free horizon collar,
\(nI-B_0\) is invertible at every positive integer \(n\).
The regular family has convergent power series with two free
constants in (2). The recurrence is
\[
 (nI-B_0)w_n=b_n+\sum_{j=1}^{n}B_jw_{n-j},
 \qquad
 (nI-B_0)^{-1}=\frac{I}{n}+\frac{B_0}{n(n+2\sigma)} .
\tag{3}
\]
This inversion uses \(B_0^2=-2\sigma B_0\). No division by
a spurious integer resonance occurs at the certified shifts.

## Explicit coefficient majorant

The diagonal of \(B\) is \(-\sigma r I\); its nonzero off-diagonal
entries are
\[
\begin{array}{ll}
 B_{13}=-\lambda r^3-k^2r^5, & B_{14}=B_{23}=-ik\sigma r^5,\\
 B_{24}=\sigma^2r^5+\lambda t_h r^2,&
 B_{31}=-\sigma^2/(\lambda r)-t_h/r^4,\\
 B_{32}=B_{41}=-ik\sigma/(\lambda r),&
 B_{42}=r^{-3}+k^2/(\lambda r).
\end{array}
\]
The inhomogeneity is
\[
 b=(t_hY^v-rY^r,\;0,\;
      -\sigma Y^r/(\lambda r)-t_hX/r^4,\;
      -ikY^r/(\lambda r))^{\mathsf T}.
\]
There is no geometric singularity in these coefficients for
\(|t_h|<1\). For \(|\sigma|<0.8\), \(\Re\sigma>-0.25\) and
\(k<0.63\), the grouped coefficients give
\(\|B_0\|_\infty\le7.24\) and
\[
\begin{split}
 L_B(\tau):=\sum_{j\ge1}\|B_j\|_\infty\tau^j
 \ \le{}&0.8\tau+4[(1+\tau)^3-1]
 +1.15[(1+\tau)^5-1]+4\tau(1+\tau)^2\\
 &+0.29[(1-\tau)^{-1}-1]
 +\tau(1-\tau)^{-4}+(1-\tau)^{-3}-1 .
\end{split}
\]
This is the scalar binomial coefficient norm term by term.
At \(\tau=0.04\) it is below 1.144. By (3),
\[
 \gamma_N:=\sup_{n>N}\|(nI-B_0)^{-1}\|_\infty
 \le \frac1{N+1}
    +\frac{7.24}{(N+1)(N+1+2\Re\sigma)} .
\]
For \(N=12\), \(\gamma_N<0.122\) and the sharper product using
the displayed bounds is \(<0.14\).

With exact parameter-dependent Taylor polynomial \(w_{\le N}\),
set \(\mathcal R=t_h w_{\le N}'-Bw_{\le N}-b\).
All coefficients through \(N\) vanish. In the weighted
coefficient norm \(\|g\|_\tau=\sum \|g_n\|\tau^n\),
the tail recurrence contracts with constant \(\gamma_N L_B\).
In particular
\[
 \|w-w_{\le N}\|_\tau
 \le \frac{\gamma_N\|\mathcal R_{>N}\|_\tau}
            {1-\gamma_N L_B(\tau)} .
\tag{4}
\]
An interval implementation may enclose the Taylor coefficients
and the residual uniformly for all parameters. The residual
tail itself is bounded by
\(\|b_{>N}\|_\tau+\sum_{n=0}^N
 \|w_n\|\tau^n\sum_{j>N-n}\|B_j\|\tau^j\).
Alternatively bound the source residual on a larger analytic
circle and use Cauchy. Equation (4) proves a finite local
remainder conditional on those source bounds and the horizon
constants. Dividing \(G\) or \(K\) by \(t_h\) shifts the
coefficient bounds by one index since their exact constants
vanish. Derivatives can be bounded at a smaller radius
\(h<\tau\), using
\(\|w_{\rm tail}\|_\tau
 \sup_{n>N} n(h/\tau)^{n-1}/\tau\).

## Fit-free strong overlap collar

Below compact launch support the EF-deweighted baseline and GL
horizon fields are their regular scalar solutions. Their scalar
equations likewise have regular analytic series and indicial
roots \(0,-2\sigma_0\) with the appropriate shifted baseline
argument, GL argument, or adjoint argument. The mean metric
contributes two analytic radial quadratures with constants
set by the global fixed-mass solution. No launch distribution
occurs in the horizon collar.

Use the regular functions \(Q,V,C\) of the returning strong
pairing and \(h=e^{sx}n_s\). The strong integral on
\(0<t_h<h_0\), before its split boundary term, is
\[
 \frac2\lambda\int_0^{h_0} t_h^{2s}L(t_h)\,dt_h,\qquad
 L=(1+t_h)e^{2s(1+t_h)}n_s
       [Q'-sV-\lambda C/(1+t_h)^4].
\]
The local products are analytic. If \(L=\sum L_n t_h^n\),
integrate termwise with
\(h_0^{n+1+2s}/(n+1+2s)\) on the positive real \(t_h\) branch.
For a bound \(\sup_{|t_h|=\rho}|L|\le M\) and \(h_0<\rho\),
the absolute error after degree \(N\) is bounded by
\[
 \frac{2M}{\lambda}\,
 \frac{h_0^{1+2\Re s}}{N+2+2\Re s}\,
 \frac{(h_0/\rho)^{N+1}}{1-h_0/\rho}.
\]
Take interval worst cases for \(s\). This leaves the finite
split boundary exact and avoids extrapolating noisy EF
differences. The mixed \(U_0,z_0\), baseline horizon amplitude,
mean integration constants, and their source bounds still need
validated global matching and interval transport. This audit
establishes a usable local tail estimate and does not enclose
the full physical residue.
