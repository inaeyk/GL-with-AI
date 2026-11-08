# Fixed-width tensor interference and error budget

In the pinned real axial quadrupole normalization \(\int X_{AB}X^{AB}d\Omega=12\). The Einstein-frame shear master is \(C=\lim 2Q_z/(\lambda r)\), and its physical news is \(\dot C\) with respect to \(u_{\rm phys}\). With \(G_4=G_5/L\), the instantaneous tensor power is \(12\dot C^2/(32\pi G_4)\). The ordered \(\epsilon^2\eta^2\) coefficient of the integrated tensor power is
\[
 E_{22,T}(I)=\frac{12r_0}{16\pi G_4}\int_I N_{01}N_{21}\,du .
\]
Here \(u=u_{\rm phys}/r_0\). The interference can have either sign. The total tensor flux is a square. The radion contributes separately to mass loss, and massive KK transport is measured through timelike surfaces. The fixed mass, Bondi frame and cuts are part of this comparison. A common order-\(\epsilon^2\) clock shift changes the fixed-coordinate integral by a boundary term \(c[N_{01}^2]_{\partial I}/2\). [Independent normalization](investigators/D-WAVEFORM.md#shear-and-interference-normalization).

## What is enclosed at \(\delta=0.12\)

Use \(y=(u-u_*)/\delta\), \(g=\sqrt\delta N_{01}\), the inherited \(K_*\in(0.257,0.304)\), and the exact decomposition
\[
 \mathfrak e=\delta^{5/2}N_{21}-K_*b''',\qquad
 \int_{I_J} N_{01}N_{21}\,du
 =\delta^{-2}\big(\langle g,K_*b'''\rangle_J+\langle g,\mathfrak e\rangle_J\big).
\]
The causal baseline on the first strip has an explicit Volterra bound. Its ray mass is \(3/r_\Sigma-3/(4r_\Sigma^2)\) in \(r_0=1\) units. A frozen-ray first iterate plus an analytic residual encloses \(g\) with the full backscattered baseline and no finite-radius extraction. On \(0<y\le1/8\) both \(g\) and \(b'''\) are positive. The early physical cut is \([u_*,u_*+0.015]\), with \(u_*\simeq-0.6849927415\).

| Dimensionless quantity | Full \(0\le y\le1\) | Early \(0\le y\le1/8\) |
| --- | ---: | ---: |
| \(\delta^{-2}\langle g,K_*b'''\rangle\) | \([-1392,-1149]\) | \([6.52,7.84]\) |
| Sufficient strict bound on \(\|\mathfrak e\|_{L^2(dy)}\) for that sign | \(16.45\) | \(6.88\) |
| Sufficient strict bound on \(\|\mathfrak e\|_{L^\infty}\) for that sign | \(21.01\) | \(41.11\) |

The sharp \(L^2\) threshold for an unrestricted residual is \(|\langle g,K_*b'''\rangle|/\|g\|_2\). The numbers are conservative lower endpoints from directed arithmetic. The physical \(\mathfrak e\) includes constrained propagation, contacts and finite-width transport beyond the optical reference. Its signed projection is still open on both cuts. A manufactured compact causal shear correction of higher order in width can reverse the optical signs at this width while preserving a pole residue, since its transform is entire. This tests the inference from the short-width orders and the spectral residue, independently of the physical equations for the correction. [Derivation and negative control](investigators/D-WAVEFORM.md#finite-width-thresholds) · [Arb output](checks/d_flux.txt).

## Error ledger

| Quantity | Present control | Missing control for finite-time claims |
| --- | --- | --- |
| Physical fundamental residue | Inherited signed enclosure, fixed \(u_0=0\), mass, frame and canonical data | None for the pole alone |
| Translated fundamental | Recomputed continued source, horizon and outer tails, fresh matching, denominator and fixed-canonical excitation | A finite-time bound on the remainder after both pole pairs |
| Spectral truncation and cuts | Local continuation, specified threshold sectors and local pole checks | Global pole count, projected cut jumps, frequency wings and high-frequency remainder for a contour decomposition |
| Causal finite-cap route | Positive energy residual estimate and scalar null Volterra cap correction | Physical mesh and graph residuals, real-radius coefficient jets, evaluated source tail |
| Positive-line route | Passive RW denominator, compact-cut high-band estimate and Plancherel enclosure | Physical weighted source/contact norms and directed low-band evaluation |
| Flux at fixed width | Directed exact-baseline optical bracket | \(\langle g,\mathfrak e\rangle\) on each cut |
| Seed extrapolation | Ordered coefficients with inherited GL growth bracket and fixed-frame conditions | Uniform higher-order remainder for specified physical seeds |

The interval calculations use the inherited GL bracket and normalized eigenmode bounds for this preparation. The empirical transfer varies ray angle, cutoff, tolerance and collar to detect numerical sensitivity, with no error certificate obtained from that spread. The normalized denominator and source-tail inequalities supply directed evidence separately. [Spectral atlas](SPECTRAL-ATLAS.md) · [Causal bound](FINITE-TIME-WAVEFORM.md) · [Inversion bound](CONTOUR-AND-INVERSION.md).
