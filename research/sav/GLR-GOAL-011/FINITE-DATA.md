# Finite-slice preparation at the Regge–Wheeler barrier

Use the completed coordinates of [HIERARCHY](HIERARCHY.md) and the spacelike hypersurface $\Sigma:\tau=v-r=0$. Its product limit is
$$
 b_0=-f,d\tau^2+2s,d\tau dr+(1+s)dr^2+dz^2,\qquad s=r_0/r.
$$
The background crosses the future horizon smoothly, and the prescribed packet vanishes on an open collar of the horizon. The spherical data take the GOAL-005 normalized growing first jet and Z prescription at quadratic order, with zero completed scalar position and velocity. The mean Einstein geometry is (3) of [HIERARCHY](HIERARCHY.md), with $G_4M_\mathrm{ADM}=r_0/2$ through order two. This fixes the homogeneous mass constant. The order-two shift of the slice under completion, if representing the data in a different spherical chart, must be included. Only the product slice enters the leading short-packet coefficient. The order-two monopole field in the returning operator is determined by the fixed mass and the GL source. The quadratic second harmonic in the spherical data is permitted and drops out of this projection by Fourier selection.

## Canonical prescription

Prescribe the completed tangential $A_i$ and the electric density
$$
 \pi^i=\sqrt\gamma R^4 F^{ia}n_a
$$
on the slice, where $n$ is future-pointing and $\gamma$ is the spatial orbit metric determinant. The full normal equation is
$$
 n^a A_a=\frac{\partial_i\pi^i}{\lambda\sqrt\gamma R^2}.
$$
This gives the induced normal component, including its compact dependence. The spatial curl is $d_\Sigma A$. The tangential equations and $dF=0$ propagate Gauss. Equivalently the dual electric density $\widetilde\pi^i=\sqrt\gamma R^{-2}(dQ)^{ia}n_a$ obeys $\widetilde\pi^z=-\lambda A_r$, $\widetilde\pi^r=\lambda A_z$, and $Q_z=\pi^r$, $Q_r=-\pi^z$ in the stated orientation. The dual Gauss constraint is $Q_n=\partial_i\widetilde\pi^i/(\lambda\sqrt\gamma R^{-4})$. Matching the densities gives the same preparation in either formulation.

Use $u_\Sigma(r)=r-2x(r)$, and write $a=a_\delta(u_\Sigma(r))$, $W=a_\delta'(u_\Sigma(r))$. The following original canonical pair is held fixed in the completed slice coordinates at each order in $\epsilon$:
$$
 A_r=\frac{(1+s)rW-s(ra)_{,r}}{2r^2},\qquad
 \pi^r=\tfrac\lambda2 ra,\qquad A_z=\pi^z=0. \tag{1}
$$
Indeed at $\epsilon=0$, the $\tau,r$ equations give $Q_z=\lambda r\Psi_0/2$, $\Psi_0|_\Sigma=a$, $\partial_\tau\Psi_0|_\Sigma=W$. This is the outgoing characteristic preparation, since $\partial_v\Psi_0|_\Sigma=0$ in the $(u,v)$ chart. It allows ingoing scattering after launch.

Setting the *independently prescribable* higher-order canonical pair to zero in (1) leaves nontrivial forced higher-order metric and velocity data. For example $Q_{2z}|_\Sigma=0$ and in the exact-curl convention of [HIERARCHY](HIERARCHY.md),
$$
 \partial_\tau Q_{2z}|_\Sigma=
 \mathcal D^r+\frac{s}{1+s}\mathcal D^\tau ,
$$
where the density $\mathcal D$ is evaluated in $(\tau,r)$ and $A_{2r}^{0}|_\Sigma=0$. The normal equation determines $A_{2\tau}^{0}$ algebraically and supplies the analogous mixed datum. For a background-curl formulation one must also subtract the Cauchy jet of $\mathcal C$. The $n=\pm1$ compact polarization is evolved, even though its tangential canonical data vanish initially. Axial harmonics at this order leave the spherical ADM charge and circle length unchanged.

## Explicit smooth packets and scales

Measure $r,x,u$ in units of $r_0$ and $\Psi$ in units of $r_0$. Let
$$
 b(y)=c_b\begin{cases}\exp[-1/(y(1-y))],&0<y<1,\\0,&\text{otherwise},\end{cases}
 \qquad \int_0^1 (b')^2dy=1.
$$
For $R/r_0=13/8$ or $7/4$, set $u_*=u_\Sigma(R)$ and
$a_\delta(u)=\sqrt\delta\,b((u-u_*)/\delta)$ for dimensionless $0<\delta\ll1$. The smooth support runs inward from $R$ by $O(\delta r_0)$. Both radii are in the barrier region. The maximum of $V_{\rm RW}$ occurs at $r/r_0=(9+\sqrt{17})/8\simeq1.640388$. The two packets define separate preparations.

The baseline outgoing kinetic energy in the scalar Killing energy is exactly $\int (a'_\delta)^2du=1$ in these units. On a graph $t=T(x)$ the energy density in $dx$ is $(\Psi_t^2+\Psi_x^2+V\Psi^2)/2+T'\Psi_t\Psi_x$. Using (1), the potential correction is $O(\delta^2)$ and finite. The physical tensor flux normalization is in [WAVEFORM](WAVEFORM.md). The small-width limit is taken on the ordered coefficients, with $\delta$ fixed before the perturbative seed limit. Comparing the correction to the baseline in a finite-seed interpretation would require $|\epsilon\chi_\infty|/r_0\ll\delta$, small enough $\eta$ for this packet, and a separate uniform remainder estimate.

## Other permitted preparations

Compact changes of $A_i,\pi^i$ at mixed and returning orders give legitimate independent radiation jets through the same Gauss formula. A smooth compact mixed change modifies the returning source $E_1A_1$. A smooth compact mean cubic change adds an ordinary homogeneous Regge–Wheeler solution. The results for (1) specify a fixed-canonical-data experiment, with exact invariance under simultaneous gauge pullback of chart, data and observation cuts. The homogeneous counterexample in [QNM-RESIDUES](QNM-RESIDUES.md) quantifies the loss of universality when (1) is varied.
