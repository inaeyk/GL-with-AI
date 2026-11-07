# Finite-slice radion transport and spectral diagnostics

## Schwarzschild wedge with the pinned L10 source

The input is the GOAL-005 scalar problem at
[61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708](https://github.com/inaeyk/GL-with-AI/commit/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708).
The source convention, finite-slice waveform and independent analytic
interval are in [GOAL-005 RADIATION.md](../GLR-GOAL-005/RADIATION.md).
The calculation imports its empirical profile reconstruction and
unchanged far radial taper from
[early_null.py](../GLR-GOAL-005/compute/early_null.py). This empirical
point estimate is distinct from that report's independent outward-rounded
enclosure. The output records SHA-256 digests of the four input files.

Set $r_0=1$, $k=\pi/5$, $\Omega=0.06328118897590773$, $s=2\Omega$,
$f=1-1/r$, $x=r+\log(r-1)$, $u=t-x$, $v=t+x$, and $\psi=r\phi_2$.
The first-order mode has $H_{\theta\theta}(2)/8=1$. In the fixed-source
diagnostic,
$$
4\psi_{uv}=F-V_\mu\psi,\qquad
 F=e^{s(u+v)/2}\frac{\sqrt 3\,k^2rf}{16}H_{\alpha\beta}H^{\alpha\beta},
\qquad
 V_\mu=f/r^3+f\mu^2 .
$$
Both initial scalar traces vanish on $v=r$. There is no temporal
multiplier of the first-order GL mode. The inherited smooth source taper
on $60\le r\le65$ and empirical source interpolation are common to all
runs. The causal wedge $u\le-8$, $v\le95.70029520342011$ above the Z
slice intersects it only at $r\ge5.15255$. It reaches none of the
strong-field source near the horizon. This is an order-two test-field
solution at finite radius and finite time.

Label a ray by $b$ on the Z slice, so $v=b$ initially,
$u=g_0(b)=-b-2\log(b-1)$, and $g=-g_0'=(b+1)/(b-1)$. Then
$$
\psi_v(b,v)=\frac14\int_b^v g(b')\,(F-V_\mu\psi)(b',v)\,db',
\qquad
\psi(b,v)=\int_b^v\psi_v(b,v')\,dv'.
$$
The code uses aligned trapezoids in $b$ and $v$. With $b_i$ ascending,
the new row follows row $i+1$. Writing $Q=F-V_\mu\psi$ and
$\alpha=(b_{i+1}-b_i)/8$, one has for $v_j\ge b_{i+1}$
$$
(\psi_v)_{ij}=C_j-D_j\psi_{ij},\qquad
 C_j=(\psi_v)_{i+1,j}+\alpha(g_i F_{ij}+g_{i+1}Q_{i+1,j}),\quad
 D_j=\alpha g_i V_{\mu,ij}.
$$
Set $C_i=D_i=\psi_{ii}=0$. Trapezoidal integration in $v$ gives
a lower-bidiagonal recurrence for the new $\psi$ row, solved without
a domain-wide iteration. The other derivative is evaluated from
$\psi_u=\int_b^v Q(b,v')dv'/4$.

## Local Killing energy and check

Write $a=f/r$ and $D_x\psi=\psi_x-a\psi$. With the angular integral
and the normalization $T_{\alpha\beta}$ of the Einstein-frame radion,
strip the overall physical energy factor $\epsilon^4 r_0/G_4$. Then
$$
e=\tfrac12\bigl[\psi_t^2+(D_x\psi)^2+f\mu^2\psi^2\bigr],
\qquad j=-\psi_t D_x\psi,\qquad \partial_te+\partial_xj=F\psi_t .
$$
The identity follows from $a_x+a^2=f/r^3$. In particular the current
$-\psi_t\psi_x$ of the conjugated Schrödinger equation differs by a
finite-radius time derivative. For the table, $j$ is the physical
current and $M_0=r_0/(2G_4)$. At fixed $R$, $dt=du$ in the
dimensionless coordinates. The reported coefficient is
$\mathcal K_{\rm wall}=2\int_{-10}^{-8}j(t,R)\,du$.

The exact integral balance on the wedge has zero contribution from the
Z slice. The future $u=-8$ and $v=v_{\rm end}$ surfaces contribute
$\int (e-j)\,dv/2$ and $\int(e+j)\,du/2$, respectively. Their sum equals
$\frac12\iint F\psi_t\,du\,dv$. The maximum fine-grid signed relative
balance error in absolute value is $4.49\times10^{-5}$. The
maximum medium-to-fine wall-coefficient difference is
$1.63\times10^{-11}$, compared with $6.53\times10^{-11}$ from coarse to
medium. These are floating residuals and differences for the same
empirical input, not interval enclosures.

| $\mu$ | $\mathcal K_{\rm wall}$ at $R=12$ | at $R=24$ | at $R=48$ |
| ---: | ---: | ---: | ---: |
| 0 | $3.99382\,10^{-7}$ | $3.86643\,10^{-7}$ | $3.80015\,10^{-7}$ |
| 0.02 | $3.98597\,10^{-7}$ | $3.84260\,10^{-7}$ | $3.74500\,10^{-7}$ |
| 0.05 | $3.94504\,10^{-7}$ | $3.71983\,10^{-7}$ | $3.46819\,10^{-7}$ |
| 0.10 | $3.80211\,10^{-7}$ | $3.31205\,10^{-7}$ | $2.63509\,10^{-7}$ |
| 0.20 | $3.27869\,10^{-7}$ | $2.07679\,10^{-7}$ | $8.71469\,10^{-8}$ |
| 0.30 | $2.55675\,10^{-7}$ | $9.46079\,10^{-8}$ | $1.33951\,10^{-8}$ |

For example, the normalized order-two $\psi$ at $(u,R)=(-9,48)$
decreases from $1.84287\times10^{-4}$ at zero mass to
$1.54328\times10^{-4}$ at $\mu=0.10$ and $3.65127\times10^{-5}$ at
$\mu=0.30$. At $\mu=0$, the finite-radius $\psi_t$ samples at $R=48$
on $u=-10,-9,-8$ agree with the pinned numerical null news within
$8.7\times10^{-5}$ relative. The empirical pinned null energy
coefficient is $3.73258\times10^{-7}$, within the inherited interval
$(3.5316,4.0171)\times10^{-7}$ for the exact quadratic null problem.
The finite-radius massless current still contains geometric and
endpoint terms. Its integral at $R=48$ exceeds the pinned null
coefficient by about $1.8\%$. The positive-mass entries describe only
the stated extraction window. A later pulse, tails, work by the source,
near-zone storage and backscatter can change what eventually crosses
a larger surface.

For fixed compact causal wedges, the retarded operator is analytic
in $\mu^2$. Iteration of the integral kernel on a finite characteristic
rectangle yields an ordered integration bound with denominator $(n!)^2$,
and the wedge is a subset of that rectangle. Differentiating the equation
at zero mass gives the same massless retarded problem driven by
$-f\psi_0$ with zero scalar jet. This explains why a small change at fixed
radius and cuts can start at order $\mu^2$, with constants that depend
on the size of the wedge. The result supplies no uniform expansion in
the radius or time of extraction.

A manufactured solution $\psi=e^{-(b-b_{\min})}(1-b/v)^2$ tests both
vanishing initial traces and the derivatives at $\mu=0.30$. Doubling
resolution from 48 to 96 reduces the three supremum errors by about
four. At resolution 96 the maximum field error is $1.39\times10^{-5}$.
Reversing the sign of the mass term in the solver while retaining the
same manufactured source and physical current gives field error $11.84$
and relative balance residual $+14.29$. This verifies sensitivity to the
mass sign; the manufactured source is unrelated to GL dynamics.

## Frequency content of finite histories

At fixed $r$ the time transform of the source restricted to a
finite interval $[t_0,t_1]$ has the elementary factor
$$
\frac{e^{(s+i\omega)t_1}-e^{(s+i\omega)t_0}}{s+i\omega}.
$$
For the early wedge, $t_0=-\log(r-1)$ and the interval ends at the
relevant future boundary. This transform describes that source
restriction. Its artificial sharp edges affect the high-frequency
content. The formal restriction of $e^{st}$ to $t\le0$ alone has
positive-frequency squared-source fraction
$1-(2/\pi)\arctan(\mu/s)$ above $\mu$.

A separate smooth scalar-source history $h_\beta(t)=e^{st}/(1+e^{(s+\beta)t})$
for $\beta>0$ shares the leading $e^{st}$ as $t\to-\infty$ and
decays at positive time. Using $\widetilde h=\int e^{i\omega t}h\,dt$
and changing variables to $y=e^{(s+\beta)t}$ gives the beta integral
$$
\widetilde h_\beta(\omega)=\frac{\pi}{s+\beta}
\csc\!\left(\frac{\pi(s+i\omega)}{s+\beta}\right).
$$
The squared modulus has an exponential high-frequency tail. The
numerical frequency quadrature agrees with time-domain Parseval
at $3\times10^{-16}$ relative for the four tested rolloffs.
The table compares the fraction of $\int_0^\infty|\widetilde h|^2\,d\omega$
above each candidate mass, before any radial transfer or flux weighting.

| Scalar source history | $\mu=0.10$ | $\mu=0.20$ | $\mu=0.30$ |
| --- | ---: | ---: | ---: |
| Smooth $\beta/s=0.5$ | 0.05931 | 0.002205 | 0.00008061 |
| Smooth $\beta/s=1$ | 0.15422 | 0.01387 | 0.001166 |
| Smooth $\beta/s=2$ | 0.28638 | 0.05931 | 0.01151 |
| Smooth $\beta/s=4$ | 0.40183 | 0.14377 | 0.05210 |
| Sharp early restriction $t\le0$ | 0.57430 | 0.35918 | 0.25415 |

The smooth histories supply examples of sensitivity to the later source
time dependence. They are scalar-only gates, distinct from the Z wedge
and from a solved GL continuation. The growth scale $s$ fixes a Laplace
point, while real frequencies above $\mu$ can occur for $\mu>s$.
A total transported spectrum would require the full source history,
its radial overlap with the outgoing Green function, and the physical
initial scalar data.

## Flat half-line retarded check

A separate manufactured model on $x\ge0$ takes Dirichlet data at $x=0$
and source $h_s(t)\delta(x-A)$, with $A=2$ and
$h_s=1/[2\cosh(st)]$. The only use of the GL number here is to set the
clock $s$. Use $\psi_{tt}-\psi_{xx}+\mu^2\psi=h_s(t)\delta(x-A)$
with vanishing past response. The time-domain Green function is the
image difference of the line kernel
$\frac12\Theta(t-|x|)J_0(\mu\sqrt{t^2-x^2})$. The independent
frequency route gives, for $x>A$,
$$
\widetilde\psi=e^{ipx}\frac{\sin(pA)}p\,\widetilde h_s,\qquad
p=\sqrt{(\omega+i0)^2-\mu^2}\quad (\omega\ge0).
$$
The branch is real positive for $\omega>\mu$ and imaginary positive
below. The real-frequency all-time integrated reduced current is
$$
\int -\psi_t\psi_x\,dt
=\frac1\pi\int_{\mu}^{\infty}\omega p
\left|\frac{\sin(pA)}p\widetilde h_s\right|^2d\omega.
$$
At a real subthreshold frequency the spatial logarithmic derivative is
real, so it makes zero contribution to this integrated current.
The reduced current in the flat code is defined for $\psi$; using the
four-dimensional spherical scalar current at finite time adds the
known time endpoint term. The all-time integral is the same.

We integrate the derivatives of the Bessel time convolution and measure
the retarded cut $u=t-x$ when half the exact spectral energy has
crossed the surface. Distances and times in this table are model units.

| $\mu$ | All-time reduced energy | Half-energy cut at $x=12$ | at $x=24$ | at $x=48$ |
| ---: | ---: | ---: | ---: | ---: |
| 0 | 0.0819185 | 0.00 | 0.00 | 0.00 |
| 0.10 | 0.0363688 | 1.40 | 4.49 | 13.73 |
| 0.30 | 0.000903726 | 7.68 | 19.50 | 42.48 |

The delayed energy is consistent with $v_g=p/\omega$ for each
propagating frequency. The causal front itself is luminal, as shown by
the support of the Green kernel. For this smooth source extending into
the past, the response has no sharp first arrival. At $\mu=0.30,x=48$,
integrating only to $u=50$ counts about 57% of the all-time result.
At $u=1200$, the largest relative deficit in the tested cases is
$2.61\times10^{-4}$; the largest coarse-to-fine difference at that same
cut is $9.33\times10^{-11}$ absolute. The remaining difference is
consistent with a late-time tail, which decreases on extending the
time interval from $u=600$ to $u=1200$ in every positive-mass case.

## Reproduction and negative records

From the parent directory of this report, with Python 3.14.8,
NumPy 2.4.6 and SciPy 1.18.0 in the executed environment:

    OPENBLAS_NUM_THREADS=1 PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-008/checks/z_massive_wedge.py
    OPENBLAS_NUM_THREADS=1 PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-008/checks/flat_retarded_flux.py

Both commands emit JSON to standard output. The saved [Schwarzschild
wedge](checks/z_massive_wedge.json) and [flat Green-function
diagnostic](checks/flat_retarded_flux.json) outputs include configurations
and measured residuals. The Schwarzschild resolutions $48,96,192$
correspond to $481,961,1921$ aligned nodes. Energy and waveforms use
floating quadrature. The test asserts a fine/medium wall-energy
difference below $2\times10^{-10}$, a fine-grid physical balance error
below $3\times10^{-4}$, and a difference below $10^{-4}$ relative
for the three finite-radius massless derivatives versus the pinned
null samples.

The first flat attempt used Gauss orders 64 and 96 and retarded time
cut 600. It failed the provisional $4\times10^{-5}$ relative all-time
energy tolerance and $2\times10^{-10}$ absolute refinement tolerance.
For example, the fine solution at $(\mu,x)=(0.30,48)$ differed from
the spectrum by $-1.1690\times10^{-3}$ relative, with a $5.73\times10^{-6}$
coarse-to-fine absolute difference. The retained run uses Gauss orders
144 and 216, cutoff 1200 and a finite-time/spectral tolerance of
$3\times10^{-4}$, with a separate same-cut quadrature tolerance of
$2\times10^{-10}$. The strong improvement in refinement error and
the residual change with the observation cutoff separate numerical
integration error from the expected delayed signal.
