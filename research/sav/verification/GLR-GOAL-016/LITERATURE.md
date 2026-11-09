# Independent action and literature audit for GLR-GOAL-016

I inspected [the source at 6e7615f9d0a88a08813203ad5b7cadde990fa042][016], especially the stabilization dossier, and compared it with [GOAL-008][008] and [its external audit][008a]. This was a non-blind review. I knew the source's sign claims before reconstructing the bulk trace, static quadratic form and scattering current. The only numerical check in this track was a 64-term binary64 sum and scalar bisection for the flat minimum. There was no curved-state or production calculation.

## Flat matching and the semiclassical flux obstruction

The nonzero winding terms of one real scalar on a circle of proper circumference \(\ell\), per unit external Jordan volume, have the proper-time representation
\[
-\frac{\ell}{2(4\pi)^{5/2}}\sum_{w\ne0}\int_0^\infty d\tau\,\tau^{-7/2}
 \exp[-M^2\tau-w^2\ell^2/(4\tau)].
\]
The \(K_{5/2}\) integral gives \(-3\sum_{w\ge1}h(wM\ell)/(4\pi^2\ell^4w^5)\) with \(h(y)=e^{-y}(1+y+y^2/3)\). The \(w=0\) term is the local bulk renormalization. Five propagating graviton polarizations and the negative Grassmann sign of each four-component Dirac determinant give the weights \(5,-4,-4\) relative to one scalar. Pontón and Poppitz provide the determinant and frame checks, with a circle of length \(2L\) in their coordinates and half the circle contribution for a single reflection parity on the orbifold. Their quoted orbifold count therefore requires conversion before use on the full circle. [Pontón and Poppitz, JHEP 06 (2001) 019, hep-ph/0105021v3, §§2.2–2.4 and Eqs. (18)–(20)][ponton]

The total homogeneous stress vanishes precisely when \(W=W'=0\). With \(F(x)=\sum_{w\ge1}h(wx)/(\zeta(5)w^5)\) and \(x=M\ell\), this gives \(8(5F-xF')=25\). The derivative of \(5F-xF'\) is negative for \(x>0\), since
\[
y^2h''-4yh'=e^{-y}y^2(y^2+3y+3)/3.
\]
The endpoints are \(5\) and \(0\), giving one positive minimum. The second derivative of \(V(\phi)=(L/\ell)^2W(\ell)\) at the tuned root is \((4L^2/3)W''(L)\). With the canonical field \(\sqrt 2 M_4\phi\), \(m_\phi^2=16\pi G_4 L^2W''(L)/3>0\). A binary64 evaluation used \(w=1,\ldots,64\), \(h'=-e^{-y}y(1+y)/3\), \(h''=e^{-y}(y^2-y-1)/3\), and 48 bisections starting at \([2,2.5]\). It returned \(ML=2.229401409612\), \(\mu^2/[(G_5/r_0^3)(r_0/L)^5]=27.5573448143\), \(k_1/M=2.81833\), and the horizon curvature scale divided by \(M\) equal to \(8.34847\). This checks arithmetic independently and makes no directed error claim. At \(L/r_0=10\), \(\mu=0.1,0.2\) would require \(G_5/r_0^3\simeq36.288,145.152\), respectively. The transport sweep and controlled flat one-loop model occupy different regimes. The massive fermion derivative expansion also fails as a near-string local approximation for these ratios.

For the specified Unruh prescription, the Fermi occupation in the horizon channel is positive on every finite band above the \(n=0\) Dirac threshold \(\omega>M\). Write the radial Dirac equation in a unit-current basis as \([-i\sigma_2\partial_x+\sqrt f(M\sigma_3+\kappa\sigma_1/r)]U=\omega U\). Its constant sesquilinear current is \(U^\dagger\sigma_2 U\). At the horizon the coefficients other than the derivative are integrably small. The unit ingoing solution has current \(-1\). At the right end the characteristic roots are separated on any closed frequency band above \(M\), and after extracting their long-range phases the basis derivative is \(O(r^{-2})\). The two amplitudes are finite and satisfy \(|a_{\rm in}|^2-|a_{\rm out}|^2=1\). Hence the absorption probability is strictly positive, and current unitarity makes the reverse transmission probability equal. Multiplying the occupation by \(\omega\Gamma\,d\omega/(2\pi)\) and the positive channel degeneracy gives strict outward power on that band. The velocity times density-of-states factor is \(v_g\,dp=d\omega\). The argument needs a single open channel and no quantitative lower bound on the full power. Page's massless calculation checks the occupation and counting structure. Doran et al. supply the massive radial threshold and independent Wronskian conventions. [Page, PRD 13 (1976) 198, §II, Eqs. (4), (6), (11)–(12)][page] · [Doran et al., PRD 71 (2005) 124020, gr-qc/0503019v1, §§2–4, Eqs. (16), (18)–(25)][doran]

Stationarity, sphere rotations and circle translation and reflection leave a possible \(dt\,dr\) term, which can be removed locally outside the horizon. Time reflection of this diagonal metric forces every covariant time-even geometric mixed \(r,t\) component to vanish. The outgoing current is \(-T^r{}_t\), which is strictly positive in the integrated channel above. At first backreaction order the quantum stress is evaluated on the vacuum background. A change of its metric argument induced by that same backreaction contributes at higher order in a joint expansion about the vacuum. The local renormalization terms have zero mixed flux in the stationary geometry. Thus the stationary equation fails under the specified state and absent compensating inward power. This is a timelike-tube statement about massive particles. A stationary bath or a time-dependent metric changes the hypotheses. Page's paper computes massless rates. It should be cited for the occupation and phase-space normalization only, alongside the massive radial and threshold argument.

## Local Gauss–Bonnet directional derivative

The action coefficient here multiplies \(\alpha \mathcal G_5\). At flat product curvature the Lovelock tensor and its first metric variation vanish, so the retained flat quadratic determinant is insensitive to \(\alpha\). On the Schwarzschild product only the compact component of the Lovelock tensor survives, \(H_{zz}=-\mathcal K_4/2\). An independent derivation of the radion equation uses the trace of \(G+\alpha H=0\) in five dimensions. At order \(\alpha\), \(R_5=-\alpha\mathcal K_4/3\), whence \(R^{(5)}_{zz}=\alpha\mathcal K_4/3\). The linear KK identity \(R^{(5)}_{zz}=-(2/\sqrt 3)\Box_E\phi\) then yields \(\Box_E\phi=-\alpha\mathcal K_4/(2\sqrt 3)\). The external equation reduces to linear Einstein-frame vacuum. Fixed ADM mass and asymptotic clock eliminate the Schwarzschild mass and time variations in that frame.

Putting \(\phi=\alpha p\), integration with zero horizon flux and zero asymptotic value gives
\[
r^2fp'=-\frac{2}{\sqrt3 r_0}\big[1-(r_0/r)^3\big],\qquad
p=\frac{2}{\sqrt3 r_0}\left(\frac1r+\frac{r_0}{2r^2}+\frac{r_0^2}{3r^3}\right)>0 .
\]
As a convention check \(g_{zz}=1+4\alpha p/\sqrt3\) agrees coefficient-by-coefficient with the published \(\alpha_{\rm BDR}a_1(r)\) when \(\alpha_{\rm BDR}=4\alpha\). The physical mass radius follows independently from the horizon area radius \(r_h=e^{-\alpha p(r_0)/\sqrt3}r_0+O(\alpha^2)\), giving \(r_0=r_h[1+11\beta/36]+O(\beta^2r_h)\), \(\beta=\alpha_{\rm BDR}/r_h^2\). This agrees with the published mass formula. For \(K(\beta)=k_c(\beta)^2r_h^2\), the fixed-mass conversion at zero coupling is
\[
r_0^4\left.\frac{dk_c^2}{d\alpha}\right|_{\mathrm{ADM},0}
 =4\left[K'(0)+\frac{11}{18}K(0)\right].
\]
The published figure shows numerical finite-coupling thresholds parameterized by the five-dimensional horizon radius. The conversion and a resolved derivative near the origin would be needed to make it an independent numerical test. I did not extract a slope from the figure. [Brihaye, Delsate and Radu, JHEP 07 (2010) 022, arXiv:1004.2164v2, Eqs. (2.1)–(2.4), (2.7), (3.25), (3.29), §4.2 and Fig. 7][bdr]

For a static external TT perturbation \(h e^{ikz}\), the four-dimensional Lovelock identity cancels the fixed-\(z\) external variation. A term with a single compact curvature index cannot pair with the external background. The extra compact derivative enters through \(\delta R_{\rho\sigma}=-(\partial_z^2h_{\rho\sigma})/2\) beyond its leaf variation, in the Lovelock term \(-4R_{\mu\rho\nu\sigma}R^{\rho\sigma}\). The net compact insertion is \(-2k^2\mathcal R h\), \((\mathcal R h)_{\mu\nu}=R_{\mu\rho\nu\sigma}h^{\rho\sigma}\). Its \(k=0\) and flat controls are zero.

The background correction to the Einstein quadratic form is independent of the direct insertion. The external leaf term reduces to the Einstein-frame form at each \(z\) plus scalar derivative terms starting at \(O(\alpha^2)\). The spacelike extrinsic term has two inverse compact lapses and one lapse times the leaf volume, yielding \(e^{-2\sqrt3\phi}\) multiplying the \(k^2/2\) term in the linearized Einstein equation. Therefore the projected Einstein background insertion is \(-\sqrt3 k^2p\) and has the same direction as the projected Lovelock term. On the smooth Euclidean bolt domain the vacuum Einstein Hessian kills the vacuum kernel and annihilates the derivative of the mode under pairing, including induced compact components. The derivative of the vacuum equation with respect to \(k^2\) is \(h/2\). Using \(\Delta_Lh=-k_c^2h\) gives
\[
\begin{split}
\langle h,\mathcal R h\rangle &=
 \tfrac12(\|\nabla_E h\|^2+k_c^2\|h\|^2),\\
\left.\frac{dk_c^2}{d\alpha}\right|_{\rm ADM,0}
 &=2\sqrt3\,k_c^2\frac{\langle h,ph\rangle}{\|h\|^2}
 +4k_c^2\frac{\langle h,\mathcal R h\rangle}{\|h\|^2}.
\end{split}
\]
In an orthonormal Euclidean frame the diagonal curvature block is \(c\) times the matrix with rows \((0,2,-1,-1)\), \((2,0,-1,-1)\), \((-1,-1,0,2)\), \((-1,-1,2,0)\), \(c=r_0/(2r^3)\). Its eigenvalues are \(0,4c,-2c,-2c\); off-diagonal eigenspaces have curvature eigenvalues \(c\) or \(-2c\). Thus \(0<p\le 11/(3\sqrt3 r_0^2)\), \(\mathcal R\le2/r_0^2\), and the source interval \(2k_c^4<dk_c^2/d\alpha\le(46/3)k_c^2/r_0^2\) follows. This is a conditional first variation at a simple vacuum static threshold on a differentiably matched domain. Reall justifies the static vacuum threshold–Euclidean negative-mode interface, including the regular horizon condition. It does not evaluate the EGB variation. [Reall, PRD 64 (2001) 044005, hep-th/0104071v2, Eqs. (1.4), (1.5), (1.10), §4.3 and §5][reall]

## Domain control and open matching

Even the elementary far-field comparison \(p_m=e^{-mr}/r\) versus \(p_0=1/r\) converges pointwise while \(\sup_{r\ge R}r|p_m-p_0|=1\) for every \(m>0\). This manufactured exterior control demonstrates why continuity in a mass-sensitive weighted domain needs proof when extending the fixed-mass derivative to a massive matched model. At the actual benchmark, the whole retarded stress variation, local contact term, independent state data, constraint and boundary variations, and a nondegenerate left-null denominator are needed. The Ward identity follows by varying the mixed-index covariant conservation equation. The closed-time-path response literature gives the retarded stress commutator and contact-term structure in its own setting, without supplying the curved KK Dirac kernel. The source's local EGB result measures freedom left by the flat tuning. The curved matched threshold and positive-growth derivative remain open. The specified Unruh state fails the stationary premise. [Anderson, Molina-París and Mottola, PRD 67 (2003) 024026, gr-qc/0209075v1, §III, Eqs. (3.6)–(3.9)][amp]

GOAL-015's [audited tensor interference][015a] concerns a separately seeded axial harmonic on the vacuum string at fixed charge and clock. Its coefficient and early cap estimate do not constrain the stationary curved Dirac stress or the local coupling. This separation also excludes reading the radion transport sweep as a prediction of the matched vacuum tensor waveform.

[016]: https://github.com/inaeyk/GL-with-AI/blob/6e7615f9d0a88a08813203ad5b7cadde990fa042/research/sav/GLR-GOAL-016/stabilization/MATCH.md
[008]: https://github.com/inaeyk/GL-with-AI/blob/4aac5ad001a9d6c63efa86640ce05837282fb73f/research/sav/GLR-GOAL-008/STABILIZATION.md
[008a]: https://github.com/inaeyk/GL-with-AI/blob/a54aacd959711945edb12668e1d26edb1b9bde33/research/sav/verification/AUDIT_GOAL008_MASSIVE_RADION_4aac5ad0.md
[015a]: https://github.com/inaeyk/GL-with-AI/blob/1089b3cb36eef1c25278c4fbfe3e843bfdcff6e4/research/sav/verification/GLR-GOAL-015/AUDIT.md
[ponton]: https://arxiv.org/pdf/hep-ph/0105021v3
[page]: https://blackholes.tecnico.ulisboa.pt/gritting/pdf/black_holes/Don-Page_Particle-emission-rates-from-a-black-hole.pdf
[doran]: https://arxiv.org/pdf/gr-qc/0503019v1
[bdr]: https://arxiv.org/pdf/1004.2164v2
[reall]: https://arxiv.org/pdf/hep-th/0104071v2
[amp]: https://arxiv.org/pdf/gr-qc/0209075v1
