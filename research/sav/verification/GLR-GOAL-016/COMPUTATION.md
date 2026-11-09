# Computational review of GOAL-016

This note audits [source PR #24](https://github.com/inaeyk/GL-with-AI/pull/24) at [6e7615f9d0a88a08813203ad5b7cadde990fa042](https://github.com/inaeyk/GL-with-AI/commit/6e7615f9d0a88a08813203ad5b7cadde990fa042). The dimensions use \(r_0=1\), the window is \(u\in[-10,-8]\), the real GL cosine is unhalved, and the fixed Z radion graph has zero quadratic scalar data. The comparisons \(\mu=0,0.10,0.20\) hold that source and graph fixed on Schwarzschild. The positive masses specify distinct prescribed-source scalar propagators. The verification saw the original source and its recorded conclusions. The current and error reconstructions, exact signed-current counterexample and endpoint arithmetic below are independent of the tube implementation. The Arb replays share the original algorithm and the GOAL-005 spectral and radial input enclosure.

## Signed physical current and continuum enclosure

For \(f=1-1/r\), \(a=f/r\), \(x=r+\log(r-1)\), \(V_\mu=f/r^3+\mu^2 f\), and \(u=t-x,\ v=t+x\), use \(4\psi_{uv}+V_\mu\psi=F\). The reduced spherical stress gives
\[
 j=-\psi_t(\psi_x-a\psi)
   =(\psi_u+\psi_v)(\psi_u-\psi_v+a\psi),\qquad
 e=\tfrac12[\psi_t^2+(\psi_x-a\psi)^2+f\mu^2\psi^2].
\]
Since \(a_x+a^2=f/r^3\), the master densities obey \(e-e_m=-\partial_x(a\psi^2)/2\) and \(j-j_m=+\partial_t(a\psi^2)/2\). Thus \(\partial_te+\partial_x j=F\psi_t\) and the physical tube coefficient differs from its master counterpart by \(a(R)[\psi^2]_{u_0}^{u_1}\), including the common factor two from the \(M_0\) convention. Integration of \(\partial_u(e-j)+\partial_v(e+j)=F\psi_t\) gives the null strip storage \(\tfrac12[\int(e-j)dv]\) plus the timelike tube flux \(\int j\,du\). This orientation gives positive outward signs and zero graph flux for the Z data. A finite-cut ordering therefore permits a change in strip storage and source work.

An exact sign counterexample illustrates the endpoint term. For flat spherical waves with \(r=R=1\), take \(\psi=A(t-r)-A(t+r)\) with \(A\) smooth, compactly supported near \([0,1]\), equal to \(2-u\) on that interval, and zero on \([2,3]\). This solution is regular at the origin because the numerator is smooth and odd in \(r\). On the tube during \(0\le u\le1\), \(\psi=2-u\) and the master flux is \(+1\) integrated over the cut. The physical flux is \(1+(1^2-2^2)/2=-1/2\). Thus even pure outgoing master data on a tube can give a negative signed physical finite-cut flux. The example tests the current interpretation. It uses a different geometry and preparation.

For the constant-mass reference, the Riemann kernel \(F_0(k(u-U)(v-V))={}_0F_1(1;-k(u-U)(v-V))\), \(k=\mu^2/4\), equals one on either characteristic edge. Differentiating the double integral gives \(4\partial_u\partial_v+\mu^2\) and the \(1/4\) integration prefactor. The two differentiated kernels are \(-k(v-V)F_1\) and \(-k(u-U)F_1\). With \(\Delta=V_\mu-\mu^2\) and \(dv=2\,dr/f\) at fixed \(u\), the ray error mass is \(D=\frac12\int|\Delta/f|\,dr\). On the causal wedge ending at \(u=-8\) the graph radius exceeds 5. The signed integrand is \(r^{-3}-\mu^2/(r-1)\); the single sign change in the \(\mu=0.10\) case lies in \([9,10]\) by monotonicity of \(r^3/(r-1)\). Absolute integration over that cell encloses the crossing. The ray bound yields the Grönwall convolution \(J=\int_{-\infty}^u e^{D(u-U)}A(U)dU\). Differentiation of the error integral gives edge terms \(D_RJ\) and \((\mu^2/R+R^{-3})J_1/4\). In the remaining interior terms \(v-V\le v_{\max}-5\) and integration of \(u-U\) produces the additional primitive \(J_2\). This reproduces the three radii
\[
 D J_1,\qquad D_R J+k(v_{\max}-5)D J_1,\qquad
 (\mu^2/R+R^{-3})J_1/4+kD J_2.
\]
The sharper kernel for the \(j\)-fold primitive follows by writing the nested integration delays as a total delay \(d'\) and a primitive offset \(b\), and bounding \(e^{-Db}\le1\). Integrating over the remaining simplex gives \(e^{-(s-D)d'}(d')^j/j!\). This explains both the sharper formula and why the crude one is conservative.

The radial masses integrate positive densities on the checked early cells, permitting partial graph cells to range from zero to the full mass upper bound. The factor \(1/4\) becomes \(1/(2f)\) after changing the complementary integration variable to radius. The radial group multiplies an enclosed positive integral by a whole-cell kernel range. The kernel derivative bound follows from \(|F_\nu^{(j)}|\le \nu!/(\nu+j)!\) for nonnegative argument, using the normalized positive Bessel projection representation. Simpson truncation is bounded on the whole integration cell by the fourth derivative, including the \(d F_1^{(4)}+4F_1^{(3)}\) product contribution for the affine delay factor, with the exponential included before product differentiation. The curvature correction is applied to all three intervals before forming the signed power. The archived intervals enclose the full current for the prescribed source and distinct propagators. [Directed derivation](https://github.com/inaeyk/GL-with-AI/blob/6e7615f9d0a88a08813203ad5b7cadde990fa042/research/sav/GLR-GOAL-016/transport/CERTIFICATE.md) · [Pinned implementation](https://github.com/inaeyk/GL-with-AI/blob/6e7615f9d0a88a08813203ad5b7cadde990fa042/research/sav/GLR-GOAL-016/transport/checks/tube_intervals.py).

## Arithmetic and failures

Independent decimal endpoint parsing subtracts the printed lower endpoint's ball radius, adds the upper endpoint's ball radius, and recomputes each separation without reading the JSON Boolean flags. The gaps below are lower bounds rounded down.

| Comparison | Directed gap lower bound |
| --- | ---: |
| \(K_{24}(0)-K_{24}(0.10)\) | \(4.8932103\times10^{-9}\) |
| \(K_{24}(0.10)-K_{24}(0.20)\) | \(6.0805406\times10^{-8}\) |
| \(K_{48}(0)-K_{48}(0.10)\) | \(5.8447553\times10^{-8}\) |
| \(K_{48}(0.10)-K_{48}(0.20)\) | \(8.3562757\times10^{-8}\) |
| \(K_{24}(0.10)-K_{48}(0.10)\) | \(8.5986735\times10^{-9}\) |
| \(K_{24}(0.20)-K_{48}(0.20)\) | \(1.0790287\times10^{-8}\) |

All six integrated coefficients are positive. The coarse \(R=48\) results give negative adjacent separation margins near \(-4.86141\times10^{-8}\) and \(-4.11827\times10^{-8}\). Its 0 versus 0.20 comparison still separates. The first \(R=24\) archive overlaps in its first comparison by \(2.04724\times10^{-9}\). These are failures to resolve the interval decision at those settings. The tool intentionally emits JSON and exits successfully on unresolved cases, so a harness has to read the sign flags or verify the endpoint gaps. An extra run at the final \(R=24\) mesh omitted `--sharper-resolvent`. Its output differed from the archived sharp certificate as expected and still certifies weaker adjacent gaps \(2.9994539\times10^{-9}\) and \(5.4418234\times10^{-8}\). The tighter estimator increases the margins. None of these checks extends monotonicity to a continuum of masses, radii or other cuts. The prescribed massive propagator comparison is separate from a stabilized GL solution or GOAL-015's tensor interference coefficient.

## Completed source band and all-band consequence

In the source Fourier convention the left and right Jost currents are \(-1,+1\), so each positive-frequency outgoing channel carries \(\omega|a|^2\); the real partner and \(M_0\) normalization give \(K_{\rm ch}(B)=2\int_B\omega|a|^2d\omega/\pi\). For \(p=\sqrt{\omega^2-V}\), \(dy/dx=p\), and \(f=p^{-1/2}w\), direct differentiation yields \(w_{yy}+w=(p_{xx}/(2p^3)-3p_x^2/(4p^4))w\). The Wronskians in \(x,y\) agree, since the derivative of \(p^{-1/2}\) cancels in the determinant. With \(V_r=\mu^2r^{-2}-3r^{-4}+4r^{-5}\) and \(V_x=f V_r\), integrating absolute derivative bounds for the three products \(fr^{-k}\) gives \(2\mu^2/3+38/15\) for \(\|V_{xx}\|_1\). The square bound uses \((a+b+c)^2\le3(a^2+b^2+c^2)\) and \(\int_1^\infty fr^{-k}dr=1/((k-1)k)\). These reproduce the error \(\eta<0.01045513\) and permit uniform positive band-energy bounds with the delayed bump. The resulting gap is \(>0.0335219\) per channel on \([4,4.25]\).

The archived coarse baseline work bound also gives an all-band separation for this particular completion. For the master energy \(E_m\), the source work inequality is \(\sup_t\sqrt{2E_m(t)}\le\int\|\mathcal F(t)\|_2dt\). The normalized energy coefficient is \(2E_m\). The upper bound on a full outgoing channel follows from energy balance outside a tube and positivity of the channel energy. At a right tube \(x=R\), the net flux through time \(T\) equals \(E_{x>R}(T)\) minus the integrated source work in \(x>R\). Its upper bound is \(E_m(T)+\sup_t\sqrt{2E_m(t)}\|\mathcal F\,\mathbf1_{x>R}\|_{L^1_t L^2_x}\). The second term tends to zero as \(R\to\infty\) by dominated convergence. The horizon uses the left exterior and the inward orientation. In the stated outgoing-channel spectral representation the full channel is nonnegative and dominates each restricted band.

The numerical all-band bound uses Cauchy–Schwarz with \(1+t^2\), \(\int(1+t^2)^{-1}dt=\pi\), and \(\mathcal F=2fMe^{su}\chi\) on the graph future. The integrand \(4fM^2e^{2su_1}(1+t^2)dr\) can use \(f\le1\) on each finite radial cell, \(|t|\le3r\) and support duration at most \(3r\). Here \(M\) is pointwise bounded by dividing the interval integrand times cell width by that exact width, as in the inherited GOAL-005 construction. The far radial estimate gives the \(300000r^{13}|U|^4e^{2sx}dx\) envelope by \(M\le50r^5|U|^2e^{sx}/f\) and \(4(50)^2(30)=300000\). This checks the factors in the baseline upper bound \(0.0275458347369492\), which is below the pulsed band lower bound \(0.0335298055756842\). It follows that either all-band channel gains more than \(0.0059839\) in the stated normalization. This proves strict separation in addition to the stronger high-band margin, while leaving precision totals, intervening frequencies and the subthreshold horizon accounting open. The pulse changes the prescribed source after the early wedge. [Band and work output](https://github.com/inaeyk/GL-with-AI/blob/6e7615f9d0a88a08813203ad5b7cadde990fa042/research/sav/GLR-GOAL-016/transport/checks/completed_band.json).

## Reproduction record

Python 3.14.8, NumPy 2.4.6, SciPy 1.18.0 and python-flint 0.9.0, with 112-bit Arb as in the pinned source. Use the pinned checkout's `research/sav` as working directory and write stdout to scratch. The exact kernel control, completed-band JSON, coarse failing 48 JSON and final 48 JSON matched byte for byte. The final 48 calculation took 54.6 CPU seconds. The separate crude fine \(R=24\) control took 104.8 CPU seconds. The final sharper \(R=24\) JSON also matched byte for byte and took 106.0 CPU seconds.

~~~sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1
d=GLR-GOAL-016/transport/checks
python3 -B "$d/check_kernels.py"
python3 -B "$d/completed_band.py"
python3 -B "$d/tube_intervals.py" --radius 48 --u-cells 16 --radial-group 12
python3 -B "$d/tube_intervals.py" --radius 48 --u-cells 128 --radial-group 3 --max-step .25
python3 -B "$d/tube_intervals.py" --radius 24 --u-cells 192 --radial-group 2 --max-step .25
python3 -B "$d/tube_intervals.py" --radius 24 --u-cells 192 --radial-group 2 --max-step .25 --sharper-resolvent
python3 -B verification/GLR-GOAL-016/transport_margins.py "$d"
~~~


The final \(R=48\), final \(R=24\), and completed-band replay SHA-256 values are respectively `8ce7c464bd9315040d47f07fb6ef25c6686f2763be603be244f236f54baae39f`, `0e00168c8aff0420ecf06638142e42f1a9dacd2e2fbc79b384a5bf8dae00aa9f`, and `64424e39af91a6beb8d37630d9477976763cfc28cc33a6d74590eedddd8ae402`.

The coordinator independently replayed the two inherited GOAL-005 commands below in scratch using Python 3.14.8 and the same directed libraries. The root and early massless waveform JSON matched their archived counterparts byte for byte. The root JSON SHA-256 is `e5bb9cc477183599afa31cf133a90b6a2c9855a8d2a89fa95964f84c79c24c9c`. The early waveform JSON SHA-256 is `e2189250db560a70d4ab9a7926470de26c607353f3f344d1720b678cc69f6f06`. These runs reproduce the same spectral/source algorithm. The rational check is a separate reduction of the resulting outward intervals.

~~~sh
cp GLR-GOAL-005/compute/gl_eigenvalue.py GLR-GOAL-005/compute/early_interval.py "$scratch/"
python3 -B "$scratch/gl_eigenvalue.py"
python3 -B "$scratch/early_interval.py"
cmp "$scratch/gl_eigenvalue_results.json" GLR-GOAL-005/compute/gl_eigenvalue_results.json
cmp "$scratch/early_interval_results.json" GLR-GOAL-005/compute/early_interval_results.json
python3 -B verification/GLR-GOAL-016/check_margins.py
~~~

[Independent endpoint computation](transport_margins.py) · [Endpoint output](transport_margins.txt) · [Coordinator arithmetic](check_margins.py) · [Output](check_margins.txt).
