# Physical-source overlap at the quadrupole pole

For the fixed-canonical packet at $R/r_0=13/8$, width $\delta=0.12$ and $\int_0^1(b')^2dy=1$, the numerical residue of the **ordered** axial shear is
$$
 A_{21,q}\simeq 0.1352-0.3706i .
$$
Use $u_0=0$, $r_0=1$ inside the calculation, $x=r+\log(r-1)$, and the Jost normalization of [SPECTRAL-CERTIFICATE](SPECTRAL-CERTIFICATE.md). In dimensional variables the positive-frequency pole term in $C_{AB}/r_0$ is $\epsilon^2\eta A_{21,q}e^{s_q u_{\rm phys}}X_{AB}$ and reality supplies its conjugate. Here $s_qr_0\simeq-0.177924631-0.747343369i$. This is a numerical evaluation of the continued outgoing transform of the actual ordered solution, using the [time-to-null exchange](NULL-TRANSFORM.md). The local pole, both shifted noncoincidences and a bound on the denominator are certified. The numerical **source** numerator does not have a certified error enclosure.

The packet support on the graph is $1.59692393\lesssim r/r_0\le1.625$. Its complete baseline transform uses the compact moments of [PAIRING](PAIRING.md). The upper tangential canonical and electric coefficients are fixed to zero in the independent data, with the induced normal and constrained terms retained.

## Evaluation

The baseline solve at $s_2=s_q-2\Omega$ uses the massless Green function and the compact moments. The full cosine amplitude of the mixed problem is computed at $s_1=s_q-\Omega$ with the two massive polarizations. A derivative-free source implementation uses $U=-\mathscr P^{tx}$, where $\mathscr P$ includes the first insertion, and $B=\mathscr P^{xz}$. It imposes ingoing and outgoing end conditions by matched fundamental solutions and an inhomogeneous particular. The lapse and mass terms are the EF spherical Einstein quadratures driven by the zero Fourier component of the full GL quadratic Ricci source. [Mixed solver](investigators/C-MIXED-QUADRATURE.md) · [Mean geometry](investigators/B-MEAN-QUADRATURE.md)

For the returning densities, let $X=b_0^{-1}H$ on the two noncompact orbit slots, $S=H_{\theta\theta}/r^2$, and let $m=b_0^{-1}B_E$ in regular ingoing EF gauge. These are their static-time amplitudes in the EF basis. Write $g^{ab}=b_0^{ab}$ on those two slots, and $F_{j,vr}$ for the Fourier-transformed curls at their corresponding shifted arguments. Define
$$
 W_Q=\tfrac12(\operatorname{tr}X^2/4+7S^2/2),\quad
 Z_Q=\tfrac12[X^2-(\operatorname{tr}X^2/4+S^2/2)I],\quad
 Z_B=\beta I-m.
$$
Then the three contributions to the returning curl density and current are
$$
\begin{array}{c|cc}
 &\widehat{\mathcal C}&\widehat{\mathcal D}/(\lambda r^2)\\
 HJ & -3S r^4\widehat F_{1,vr}/2&-Xg\widehat A_1/2\\
 H^2P & W_Q\widehat Q_{0z}& Z_Qg\widehat A_0\\
 B_EP &-\beta\widehat Q_{0z}&Z_Bg\widehat A_0
\end{array}
$$
The factors $1/2$ are mean products of the full cosine amplitudes. The EF monopole already includes its quadratic averaging and the pure mean radion cancels from these two densities. A second EF derivation of the signs and the angular normalization is in [E's audit](investigators/E-OVERLAP.md).

The right integrals use one complex ray from $r=2.5$ beyond the launch support. The inner massive source integral and the final outer overlap converge there by the spectral direction tests in [QNM-RESIDUES](QNM-RESIDUES.md). At the left, direct subtraction of nearly equal Schwarzschild components followed by weak-form quadrature failed as the horizon cutoff decreased. For the adopted calculation write the Laplace amplitudes as $K=e^{-sx}(p-f\widehat{\mathcal D}^v/r^2)$, $V=e^{-sx}\widehat{\mathcal D}^v/r^2$ and $C=e^{-sx}\widehat{\mathcal C}$, where $p=\widehat{\mathcal D}^r/r^2$. Below the compact launch, the exact strong integrand in $dr$ is
$$
 \frac{2}{\lambda}r h e^{sx}\left[K'-sV-\lambda C/r^4\right].
$$
Fit the regular $K,V,C$ on a resolved collar $r-1\in[c/2,3c]$. Integrate the fitted strong form down to the horizon with the weight $(r-1)^{2s}$ extracted. Retain the term $-(2/\lambda)r h e^{sx}K$ at $r=1+c$ for the transition to the weak form. The RW horizon factor uses its regular series. The comparison code separately differentiates collocated currents over the remainder of the contour and integrates the **strong** form there as well, with no split term. It shares the computed fields and has its own quadrature. It is not an independent second inhomogeneous radial solver.

At the principal settings, $W'\simeq7.83649093-1.70981020i$. The three numerator contributions and their sum are
$$
\begin{array}{c|r@{}l}
HJ&0.211891&-4.250228i\\
H^2P&0.422425&+1.142883i\\
B_EP&-0.208245&-0.028240i\\
I&0.426072&-3.135585i
\end{array}
$$
There is partial interference, with an estimated residual imaginary part far above the measured variations.

## Numerical error and preparation checks

| Change from the principal run | Observed change in residue magnitude |
| --- | --- |
| Outer ray length $64\to48$ with the same horizon cutoff and solver tolerance | $5.1\times10^{-6}$ |
| Horizon cutoff $10^{-6}\to10^{-5}$ at fixed ray length and tolerance | $4.7\times10^{-7}$ |
| Relative ODE tolerance $2\times10^{-11}\to2\times10^{-10}$ at fixed endpoints | $9.0\times10^{-6}$ |
| Rotate the ray from $\pi/4$ to $0.9$ radians | $5.1\times10^{-6}$ |
| Change collar cap $0.008\to0.012$ and degree $6\to4$ | $1.6\times10^{-6}$ |
| Joint refinement to length 80, horizon cutoff $10^{-7}$, tolerance $2\times10^{-12}$, degree 7 | $5.4\times10^{-5}$ |
| Strong instead of weak exterior quadrature, degree 42, with a distinct collar cap | $4.3\times10^{-7}$ |

The principal run's far endpoint integrand norm is $9.3\times10^{-29}$. The estimated quadrature errors refer to the sampled or fitted integrand. They exclude fitting and ODE error. The $10^{-7}$ horizon refinement shows roundoff sensitivity despite its tighter solver tolerance. The independently run alternate-contour estimate was $0.135226-0.370630i$. Changes in the regularized runs above are below $6\times10^{-5}$ in modulus. This spread is empirical and is not a confidence or interval bound. The exact GL rate bracket is used in the spectral certificate. The overlap uses the pinned rate estimate and its matched profile, with observed GL logarithmic-derivative mismatch below $2\times10^{-11}$ in the listed physical runs. The near-horizon fit, forced endpoint data, source profile and stationary tail remain the key targets for mathematical error control. The failed direct horizon refinement is preserved separately.

Turning off the GL amplitude makes all three computed numerator pieces vanish. At $R/r_0=7/4$ with the same width and normalization the empirical residue is $0.08784-0.33171i$. The baseline axial pole for the first packet is $-0.00055879+0.00065753i$. Projecting the first packet's correction off the real direction $s_q A_{01,q}$ for a common clock translation leaves approximately $0.17761-0.34906i$. The ADM mass is fixed in both comparisons. Adding a freely specified unit upper-order outgoing packet at $R/r_0=1.46$ with the same width would change the coefficient by $-0.00085518+0.00058749i$. Its launch cut is later than the first packet window. This separate control leaves the lower-order sources unchanged and illustrates preparation dependence.

There is no pole-dominance estimate or uniform finite-amplitude error bound. The source residue value is reproducible numerical evidence within the ordered hierarchy. The certified short-packet waveform coefficient and signed early energy statement remain those of [WAVEFORM](WAVEFORM.md).

## Reproduction

Run from the scoped research directory. Principal and strong-form checks used Python 3.14.8, NumPy 2.4.6, SciPy 1.18.0 and SymPy 1.14.0.

```sh
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/physical_overlap/returning_overlap.py --left-delta 1e-6 --length 64 --tol 2e-11 --cap .008 --repeat-cap .012
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/physical_overlap/overlap_projection.py
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/physical_overlap/overlap_mixed.py
PYTHONDONTWRITEBYTECODE=1 python3 GLR-GOAL-011/checks/physical_overlap/overlap_mean.py
```

[Saved runs and options](checks/physical_overlap.txt) · [Failed horizon refinement](checks/physical_overlap_failures.txt) · [Adverse review](investigators/E-OVERLAP.md)
