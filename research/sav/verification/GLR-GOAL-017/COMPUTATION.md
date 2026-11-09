# Computational review of GOAL-017

The scope is the pinned fixed-background ordered problem on \(I=[0,8.407362]\), in \(r_0=1\) units and the matched Einstein-frame clock. The source is [PR #23](https://github.com/inaeyk/GL-with-AI/pull/23) at [9e2fe32c35151a8a4108864622cf2aef88bb0839](https://github.com/inaeyk/GL-with-AI/commit/9e2fe32c35151a8a4108864622cf2aef88bb0839). This review shares the pinned GR/KK reduction, prepared packet and pole/residue boxes. It inspected the original claims and code before running source replays and independently implementing real-mode quadrature and exact rational affine integration.

## Physical baseline certificate

For \(p=u-u_*\), the graph-conforming lift is \(w_h=A(p)+\iint F(s,z)\mathbf1_{s+z\ge\delta}\,dz\,ds\). The double primitive and its first derivatives vanish on the graph. This is important because the affine density itself can have a nonzero diagonal trace inside the packet. In the fit triangles the physical region is the upper-right part of the square, and the exact area times the average of the three active nodal values is its mass. A full rectangle has area times the average of its four nodal values. Both relations give the row-prefix recursion and the isolated affine weight of the newest node. At the patch seam \(z=\delta\), \(F_{\rm in}=[q\,g(1/R)/2]F_{\rm out}\) uses the ratio of \(dv/dz\). The field is a single primitive and the density \(\alpha F\) has matching physical one-sided values there. The graph diagonal contacts in \(w_{h,pp}\) and \(w_{h,zz}\) have positive sign, from differentiating the lower integration bound \(\delta-p\) or \(\delta-z\). The rectangular interpolation remainder is bounded by \(h^2M_{pp}/8+k^2M_{zz}/8\). For the triangle, Taylor expansion at an interior point followed by averaging over the three barycentric weights cancels the first derivatives and leaves the safe bound \((h^2M_{pp}+2hkM_{pz}+k^2M_{zz})/2\). These local derivative bounds apply one-sidedly at mesh seams. At the far compactification endpoint \(\zeta_a=0\), the flow is fixed at \(\zeta=0\), its first variation is one, and the limiting coefficient is \(K=3\). [Pinned implementation](https://github.com/inaeyk/GL-with-AI/blob/9e2fe32c35151a8a4108864622cf2aef88bb0839/research/sav/GLR-GOAL-017/spectral/baseline.py).

For the error \(e=w-w_h\), write \(R=w_{h,uv}+Vw_h/4=\alpha(F+Kw_h)\). The Killing current gives \(e_u^2+Ve^2/4\) as the flux per \(du\) on constant \(v\), and \(e_v^2+Ve^2/4\) per \(dv\) on constant \(u\). The work source is \(-4R(e_u+e_v)\) per \(dt\,dx\), or \(-2R(e_u+e_v)\) per \(du\,dv\). Since \(V\ge0\) and initial error flux is zero, the square of the supremum flux norm over subdiamonds is bounded by twice that norm times \(\|R\|_{L^1_vL^2_u}+\|R\|_{L^1_uL^2_v}\). Dividing and taking the infinity limit yields the stated bound. The first residual norm pulls back to \(\|F+Kw_h\|_{L^1_zL^2_p}\) and the second to \(\|\sqrt{\alpha}(F+Kw_h)\|_{L^1_pL^2_z}\); the factors cancel differently because of the order of integration. The compactified causal equation has bounded \(K\), so a bounded, continuous Volterra trace at the far endpoint supplies the trace used with the flux argument.

The 192-bit replay gives the same \(E_0<3.898\times10^{-5}\), \(\|N_{01}\|_2\in[0.00501844,0.00509641]\), and physical leading-pole mismatch ratio \([2.02612,2.16772]\). The fixed leading residue alone also fails by an independent loose inequality. Its input boxes give \(|s_q|<0.77\), \(|A_{01,q}|<0.000875\), and \(-\Re s_q>0.1779\). Thus \(\|P_{01}\|_2<2(0.77)(0.000875)(1.677)=0.0022597575\), by bounding \(\int_0^\infty e^{-2(0.1779)u}du\) and taking its square root. The reverse triangle bound \(\|N_{01}-P_{01}\|_2/\|P_{01}\|_2>0.00501844/0.0022597575-1>1.22\) already exceeds \(1/2\). This independent test shares the physical baseline lower bound and the pinned residue box. It avoids a waveform-template cross term.

## Returning candidate and conditional decisions

The clustered march is a numerical proposal for the ordered return and printed the inherited SciPy DOP853 warning `RuntimeWarning: invalid value encountered in scalar divide`. The replay still reproduced the little-endian binary64 digest `34284f389420d62aaacd30b111b17f0c740fca3528b9d7cc85290909996d9c07`. Independent exact rational arithmetic using the interior binary64 time points and final exact decimal \(8.407362\) gives an Arb enclosure \([0.08741378900667258368657\ \pm 2.11\times10^{-24}]\) for its affine \(L^2\) norm. The source directed arithmetic reproduced the midpoint ratios \(1.0183362189\), \(1.0194941299\) and subbox sufficient ceilings \(0.3173133838414584\), \(0.8048488267053903\). Its independent numerical quadrature output reproduced byte for byte.

A new directed cross-check uses real mode functions \(f_i=2\Re(s_i a_i e^{s_i u})\), \(a_i=1,i\), and midpoint integration rather than the source's complex antiderivative. Gram entries use 4096 equal cells, with whole-cell second derivative bounds \(4|s_i s_j|(|s_i|+|s_j|)^2\). Affine candidate-mode products use 16 subcells per affine cell and the bound \(4|s_i|^2|n'|+2|s_i|^3\max|n|\). Each cell uses the midpoint error \(h^3M/24\). For the amplitude uncertainty of each conjugate pair, use the largest eigenvalue of its independent two-real-coordinate Gram block and the radius of the amplitude rectangle. For exponent uncertainty, differentiate \(2\Re(s A e^{su})\) along the complex segment and discard its damping using the negative real part. Over the published residue subdivision, \(\|n-P\|_2-\|P\|_2/2\) has Lipschitz constant \(3/2\) in \(P\). The new method encloses the minimum of its conservative subbox lower bounds near \(0.31731172756560829\) and \(0.80484527383879449\). Both clear the reported sufficient thresholds \(0.31731\) and \(0.80484\). This quadrature shares the source template parameter boxes, candidate ordinates and subdivision sizes. It uses neither the source affine-pole cross integrals nor its exact Gram. [Pinned original gates](https://github.com/inaeyk/GL-with-AI/blob/9e2fe32c35151a8a4108864622cf2aef88bb0839/research/sav/GLR-GOAL-017/checks/candidate_gates.py).

The triangle inequality transfers either rejection only after evaluating a valid physical \(E_{21}\). A source or graph error may move the physical return even when the grid refinement and alternate readout look close. The shifted-current discrepancy \(0.02840983\) on the final clustered mesh and the candidate-only bilinear comparison measure different reconstructions of shared lower fields. The ten residual probes use interpolated coefficients and selected cells. The 015 weak two-cut signs and early cap error control other observables and supply no later strong error. A positive matched ordered separation from this norm argument requires \(E_{21}<0.08741378900667\). The larger rejection ceilings cannot supply that stricter condition. Uniform finite-seed derivative bounds remain an additional physical input.

## Fail-open controls

A scratch mutation replacing the second late return ordinate with NaN caused the source gate program to exit successfully and emit `[100.0000000000000, 100.0000000000000]` for both subbox rejection thresholds after emitting NaN for earlier norms. The loop initializes `low = arb(100)` and uses `min(low, margin)`. With NaN at every center it keeps the finite positive sentinel. The reported digest changed. A second mutation flipping the sign of only the second time abscissa kept the same endpoints and generated positive ceilings \(0.31728846\) and \(0.80482906\) on a folded time grid. The actual pinned array is finite, strictly ordered and grid-aligned, and passes the independent check above. For reusable entry points, require finite one-dimensional arrays, strict abscissa monotonicity, exact alignment with the selected full-grid slice and an expected digest for replay. Check every directed intermediate for finiteness before comparing minima, and initialize the minimum from the first validated subbox.

Reproduce in scratch against the real clustered archive, then pass each mutated archive to `candidate_gates.py --sharpen`.

~~~python
import numpy as np

with np.load("cluster.npz", allow_pickle=False) as original:
    archive = {key: original[key] for key in original.files}
    archive["u"][1] *= -1
    np.savez("folded-u.npz", **archive)
    archive["u"][1] *= -1
    late = np.searchsorted(archive["u_full"], 0.0)
    archive["p_shear_infty"][late + 1] = np.nan
    np.savez("nonfinite-candidate.npz", **archive)
~~~

## Reproduction record

Python 3.14.8, NumPy 2.4.6, SciPy 1.18.0, SymPy 1.14.0, Numba 0.65.1 and python-flint 0.9.0. All work uses single-threaded bounded one-dimensional fixed-background calculations, analytic reconstruction and input mutations in scratch. Commands below start in the pinned GOAL-017 checkout's `research/sav`, with `scratch` an existing separate directory. The spectral baseline, directed template boxes, witness boxes, independent candidate replay, inverse controls, horizon contact controls, candidate weak readout and the refined near-horizon geometry extension matched their archived text. The standard march took 27.2 CPU seconds and emitted the same SciPy warning. The gate arithmetic reproduced the archived numerical fields and candidate digest. The independent script additionally uses that digest as a hard precondition. The separate 112-bit bilinear readout matched the archived results block and remains an estimate for fixed candidate ordinates. The geometry replay used a clean checkout at the GOAL-015 source SHA `65a4af854a25ca02e40f31b00fe5ff789935bdc4`. Its 3072 collar cells cover \(r\in[113/112,25/16]\), and its \(H_{rr}\) second radial jet full-cell diameter can be as large as \(5728.569\). The coverage and directed input construction survive, with that limitation on derivative precision.

~~~sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1 NUMBA_NUM_THREADS=1
g=GLR-GOAL-017
python3 -B "$g/spectral/baseline.py" --early 128 --late 256 --radial 256 --norm 8192
python3 -B "$g/verification/template_boxes.py"
python3 -B "$g/analysis/witness_boxes.py"
python3 -B "$g/spectral/inverse_check.py"
python3 -B "$g/verification/horizon_contacts.py"
python3 -B "$g/checks/late_march.py" --packet 256 --late 480 --radial 720 --cluster 768 \
  --probes --output "$scratch/cluster.npz" --volume "$scratch/cluster-volume.npz"
python3 -B "$g/checks/candidate_gates.py" "$scratch/cluster.npz" --sharpen
python3 -B "$g/verification/candidate_replay.py" "$scratch/cluster.npz"
python3 -B "$g/checks/current_readout.py" "$scratch/cluster.npz" "$scratch/cluster-volume.npz"
python3 -B "$g/analysis/weak_candidate.py" "$scratch/cluster.npz" "$scratch/cluster-volume.npz"
python3 -B "$g/analysis/geometry_extension.py" "$goal015_checkout" --refinement 4 --precision 192
python3 -B verification/GLR-GOAL-017/audit_candidate.py "$scratch/cluster.npz"
python3 -B verification/GLR-GOAL-017/malformed_archive.py "$g/checks/candidate_gates.py" "$scratch/cluster.npz"
~~~


The directed geometry extension also matched the archived text byte for byte at refinement 4 and 192 bits. It checks the exact GOAL-015 Git HEAD and the clean scoped input paths before importing the geometry. A coordinator trial passing a Git archive export was rejected at the Git identity check, while the clean detached checkout at the full source pin reproduced the output. The coordinator's separate inverse and horizon-contact replays also matched byte for byte. See the geometry script for the input boundary and the source program for its frozen role.

~~~sh
python3 -B verification/GLR-GOAL-017/check_margins.py
python3 -B verification/GLR-GOAL-017/baseline_projection.py "$g/spectral/baseline.py"
~~~

[Independent candidate quadrature](audit_candidate.py) · [Output](audit_candidate.txt) · [Malformed archive control](malformed_archive.py) · [Output](malformed_archive.txt) · [Baseline projection](baseline_projection.py) · [Output](baseline_projection.txt).
