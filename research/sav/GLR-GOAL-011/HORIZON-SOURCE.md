# Fit-free horizon source and validated launch inputs

The physical packet, time origin and pole normalization are those of [PHYSICAL-RESIDUE](PHYSICAL-RESIDUE.md). A new regular EF calculation removes the empirical horizon extrapolation from the residue check. With the same exterior midpoint calculation it gives
$$
 A_{21,q}=0.13523245-0.37062029i
$$
at the quoted numerical resolution. Moving the split from $r-1=1/128$ to $3/256$ and raising the Taylor degree from 34 to 44 changes this estimate by about $6\times10^{-10}$. This is a comparison of two numerical calculations sharing most of their exterior inputs. The conditional collar error enclosure and the validated lower fields below are separate results.

## Regular mixed horizon system

In ingoing EF coordinates $(v,r,z)$ remove $e^{s_1v+ikz}$ from the mixed field and source, with $s_1=s_q-\Omega$. Let $u=-\mathscr P^{vr}$ and $j=\mathscr P^{rz}$ be the total mixed fluxes, $a=A_v$, and $z=A_z$. The first insertion has $X=X^{vr}$ and $Y^v,Y^r$ in this chart. Set $t_h=r-1$ and
$$
 b_f=-\frac{s_1u+ikj+Y^r}{\lambda r^2}-a,
 \qquad e_f=\frac{j}{r^4}-s_1z+ika .
$$
They are $f A_r$ and $f F_{rz}$, respectively. The derivative-free equations are
$$
 \begin{aligned}
 f u_r&=\lambda r^2 b_f+ik r^4 e_f+fY^v,&
 f z_r&=e_f+ik b_f,\\
 f a_r&=s_1 b_f-f(u+X)/r^4,&
 f j_r&=f\lambda r^2 z-s_1 r^4 e_f .
 \end{aligned}
$$
In particular $F_{vr}=(u+X)/r^4$. These equations give $t_h y'=A(t_h)y+B(t_h)$ with analytic coefficients. At the horizon $b_f=e_f=0$, leaving $u_0,z_0$ free, with
$$
 a_0=-\frac{s_1u_0+ik s_1 z_0+Y^r_0}{\lambda+k^2},
 \qquad j_0=s_1z_0-ik a_0.
$$
The indicial eigenvalues are $0,0,-2s_1,-2s_1$ and $A_0^2=-2s_1 A_0$. The regular family uses ordinary power series. The other branches may be bounded at the target frequency and still fail smoothness. The compact and regular pieces are both retained in the reconstruction. [Independent EF derivation](investigators/E-HORIZON-SYSTEM.md)

The two homogeneous columns use $(u_0,z_0)=(\sqrt{\lambda+k^2},0)$ and $(0,\sqrt{\lambda+k^2}/2)$. The particular has $u_0=z_0=0$. These give the same normalization as the two reduced massive unknowns in the earlier numerical solver. The three columns are evaluated at a strictly positive $t_h$ by the local enclosure and converted to the first-order solver variables. No numerical difference of $A_x-A_t$ at the horizon enters the local physical source.

## Validated series and collar integration

For $n\ge1$, the coefficients satisfy
$$
 (nI-A_0)y_n=B_n+\sum_{j=1}^n A_jy_{n-j},\qquad
 (nI-A_0)^{-1}=\frac1n\left(I+\frac{A_0}{n+2s_1}\right).
$$
The scalar GL and massless RW equations use the same regular-singular form with the derivative as a second variable and $2s_1$ replaced by $1+2\Omega$ or $1+2s$. In each case the Frobenius branch and normalization are fixed by the horizon data. The GL conversion to EF divides by one and two powers of $t_h$ only after exact vanishing of the relevant coefficients. The constant terms in the two numerators vanish identically. For the second numerator its linear term vanishes using
$$
 N_r(1)=\frac{k^2-5+6/(1+k^2)}{1+2\Omega}N(1).
$$
The analogous division by $t_h$ in $A_r$ uses $b_f(1)=0$ from the exact regular initial condition. The audit checks these removable singularities algebraically. An interval zero-containment test in the code is a guard on that algebra.

For a series $v$ write $\|v\|_\rho=\sum\|v_n\|_\infty\rho^n$. If $y^{[N]}$ is the exact Taylor truncation, the omitted coefficients solve a tail fixed-point problem with contraction constant
$$
 \Gamma_N\sum_{j\ge1}\|A_j\|_\infty\rho^j,qquad
 \Gamma_N=\frac{1}{N+1}\left(1+\frac{\|A_0\|_\infty}{N+1+2\operatorname{Re}s_1}\right).
$$
Use matrix row norms, Acb coefficient arithmetic and Cauchy bounds from a larger disk for the omitted $A,B$ coefficients. The tail error is bounded by the projected weighted residual times $\Gamma_N$ divided by one minus the contraction constant. This bounds the actual analytic remainder in $\ell^1_\rho$, so evaluation at $t_h<\rho$ gains $(t_h/\rho)^{N+1}$. In the principal mixed horizon calculation $\rho=0.043$ and the contraction is below $0.026$. The initial local balls at $t_h=1/256$ have width below $1.5\times10^{-37}$ **for the specified point inputs**. That width measures local arithmetic and truncation. It excludes uncertainty in the supplied lower amplitudes and any subsequent numerical matching.

Use regular transformed $K,V,C$ of [PHYSICAL-RESIDUE](PHYSICAL-RESIDUE.md). The exact strong collar integrand is $t_h^{2s}G(t_h)$, with
$$
 G(t_h)=\frac{2}{\lambda}r N_s e^{2sr}\,[K'-sV-\lambda C/r^4],
 \qquad h=e^{sx}N_s .
$$
For a split $c$, integrate the low coefficients as $G_n c^{n+1+2s}/(n+1+2s)$ and enclose the remaining sum by Cauchy on a disk larger than $c$. The weak exterior needs the boundary $-(2/\lambda)r h e^{sx}K$ at $t_h=c$. Both this value and its tail are enclosed by the same analytic calculation. At $c=1/128$, the individual point-input Cauchy tail bounds are below $2.9\times10^{-9}$, $1.1\times10^{-10}$ and $2.4\times10^{-11}$ for $HJ,H^2P,B_EP$. These are errors in the local **numerator pieces** only.

## Validated physical lower inputs

The physical rate interval and the certified returning pole disk are inputs. The compact ordinary flow is validated by Taylor coefficients at a real mesh point, with a complex-disk bound $\|y(z)\|\le\|y_0\|\exp(MR)$ when $\|A(z)\|\le M$ on the disk of radius $R$. Cauchy bounds the omitted Taylor terms at the endpoint and on quadrature cells. For the GL profile, transport the regular horizon-normalized $N,N_r$ to $r=2$ and impose $H_{\theta\theta}(2)=8$ in the static-time convention. The horizon scale is enclosed in $(-25.909,-25.907)$, and the normalized $N,N_r$ at $r=2.5$ are enclosed as well.

For the baseline RW field at $s_2=s_q-2\Omega$, transport the horizon solution $h$ and use the certified right logarithmic derivative at $r=2.5$ with $g_+(2.5)=1$. The compact launch moments use interval quadrature on the physical support, including its flat edges. The bump normalization uses the derivative-square integral with a separate analytic bound for the flat endpoint tail. The inward coefficient $C_-=-J_g/W$ and the outward coefficient $C_+=-J_h/W$ satisfy the conservative boxes
$$
 \begin{aligned}
 -0.00718&<\operatorname{Re}C_-<-0.00702,&
 0.00661&<\operatorname{Im}C_-<0.00679,\\
 -0.00659&<\operatorname{Re}C_+<-0.00641,&
 0.01083&<\operatorname{Im}C_+<0.01097 .
 \end{aligned}
$$
The right coefficient is in the stated $g_+(2.5)=1$ normalization. The exact physical GL and baseline lower data are contained in these enclosures. The source itself requires the additional forced matching and monopole boxes.

For an illustrative conditional estimate, take componentwise half-widths $0.0002$ on the two mixed horizon amplitudes and $0.002$ on the two EF mean cap amplitudes, centered on the midpoint solve. Factor the collar dependence into the exact products of the GL scale, baseline coefficient, mixed homogeneous amplitudes and mean cap values. This avoids treating repeated copies of the same input as independent within the series algebra. The code supplies interval values of all three collar pieces in those boxes. These mixed and mean box widths are assumptions for this estimate. The full physical residue is still empirical, with the same exterior matching and integration limitations as before.

## Exterior validation interface

The remaining forced matching can be enclosed without propagating an interval fundamental matrix for the whole inhomogeneous system. For the reduced two-field system write $U_x=J+F$, $J_x=(s_1^2I+V)U+G$. For either exact homogeneous logarithmic derivative $P$ put $L=J-PU$. The exact affine equation and matching condition are
$$
 L_x=-PL+G-PF,\qquad (P_L-P_R)U_*=L_R-L_L .
$$
On a directed cell with $\gamma=dx/d\xi$, the error of a candidate $\widetilde L$ is bounded by scalar Gronwall with rate $\lambda_{\max}(\operatorname{Herm}(-\gamma P))$ and forcing the enclosed residual $\widetilde L_\xi+\gamma P\widetilde L-\gamma(G-PF)$. The left data follow from the analytic columns above. At the right the outgoing condition yields an integral from infinity. Where $\operatorname{Herm}(-\gamma P_R)$ has lower bound $aI$ on the outward ray with $a>0$, the backward kernel has norm at most $e^{-a(\rho-R)}$. This gives a direct tail bound from an enclosure of the decaying $G-P_RF$. The denominator certificate gives invertibility of $P_L-P_R$ at the physical target. The subsequent [affine matching and mean calculation](EXTERIOR-SOURCE.md) executes those transports and encloses the physical mixed matching state. Transport to the horizon amplitudes and the full projection remain open.

## Reproduction

```sh
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/physical_overlap/certified_launch.py
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/physical_overlap/validated_horizon.py --certified-lower
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/physical_overlap/validated_horizon.py --cap .01171875 --degree 44
```

[Saved checks](checks/horizon_source.txt) · [Independent code audit](investigators/E-HORIZON-INTERVALS.md)
