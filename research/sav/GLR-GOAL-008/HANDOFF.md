# GOAL-008 comparison handoff

Use the vacuum Z preparation and its completed scalar as the common benchmark for comparing a massive transport calculation against GOAL-005. The inherited result concerns the exact quadratic problem on Schwarzschild. The new finite-tube data use a sampled GL profile and a prescribed radion mass in the scalar equation. A self-consistent stabilized black string would require its own background, source and constraint data.

## Match the source and cuts

Use \(L/r_0=10\), \(k=2\pi/L\), \(G_4=G_5/L\), \(M_0=r_0/(2G_4)\), and \(x=r+r_0\log(r/r_0-1)\). In the future-regular fundamental with \(h^U_{zA}=0\), set \(H_{\theta\theta}/(2r^2)=1\) at \(r=2r_0\). The first jet is \(\epsilon e^{\Omega t}H\cos(kz)\), with \(\Omega r_0\simeq0.06328119\) and \(s=2\Omega\). The completed coefficient of \(\epsilon^2\) is \(\phi_2=\sqrt3\langle q^{(2),U}_{zz}\rangle/4\), and \(\psi=r\phi_2\). Prepare zero scalar value and normal velocity on \(v-r=0\), where \(v=t+x\). Keep \(u=t-x\) and the same additive constant in \(x\). The cuts \(u/r_0=-10,-9,-8\) originate at \(r/r_0\simeq6.57,5.84,5.15\). They sample the exterior GL tail. Independent scalar data interfere with the sourced response, so the Z specification matters. [Pinned setup and scalar gauge completion](../GLR-GOAL-005/HANDOFF-MICHAEL.md)

## Compare like observables

| Calculation | Observable | Benchmark and evidence |
| --- | --- | --- |
| Vacuum Z, exact quadratic problem | \(\mathcal K_Z=(2/r_0)\int (\partial_u\lim_{r\to\infty}\psi)^2\,du\) on \(u/r_0\in[-10,-8]\) | \(3.5316\times10^{-7}<\mathcal K_Z<4.0171\times10^{-7}\), a continuum enclosure. [Independent Work audit](https://github.com/inaeyk/GL-with-AI/commit/a9bfbec0efe41b881e4b12aa262f5fcfcd94c639) |
| Vacuum Z, sampled profile | Null trace samples at \(u/r_0=-10,-9,-8\) | \(6.7414\times10^{-5},\,2.1119\times10^{-4},\,6.5154\times10^{-4}\), with \(\mathcal K_Z\simeq3.7325\times10^{-7}\). [Pinned waveform](../GLR-GOAL-005/compute/early_null_waveform.csv) |
| Fixed-source massive Z | \(E_R/(M_0\epsilon^4)\) across \(R/r_0=48\) on the same cuts | \(3.80015\times10^{-7}\) at \(\mu=0\), \(2.63509\times10^{-7}\) at \(\mu=0.1\), and \(1.33951\times10^{-8}\) at \(\mu=0.3\), from mesh-refined diagnostics. [Worldtube results](checks/z_massive_wedge.json) |

For a tube comparison record \(\psi\), \(\psi_t\) and \(\psi_x\) with the same clock. At fixed \(R\), \(\partial_u|_R=\partial_t=\partial_u|_v+\partial_v|_u\). Use the signed outward power \(-\psi_t(\psi_x-f\psi/R)/G_4\) for the quadratic coefficient. On a finite window the term \(f\psi\,\psi_t/(RG_4)\) contributes to the integrated power. This includes energy crossing both ways and is sensitive to the closing cut. The same-radius zero-mass result is the reference for the listed massive runs.

For an ended, localized history, integrate the signed net power through the completed transient and account for interior storage, horizon absorption and scalar source work. In the far outgoing channel record the real-frequency spectrum weighted by \(\omega\sqrt{\omega^2-m_\phi^2}\). The slower part of the packet can arrive after fixed retarded cuts. Record the time history or taper and any added data with a spectrum. The exponential rate \(s\) sets an early time scale; propagation is determined by the real-frequency spectrum of the actual history. [Transport and spectral checks](DIAGNOSTICS.md)

## Stabilized comparison gate

The derived one-loop Casimir example satisfies the asymptotic flat product equations with a positive radion mass. A GL comparison requires the renormalized stress in a specified curved state, the corrected black-string solution, the projected constraints and the retarded stress response at \(n=1\) and at quadratic order. Match the same geometric seed, data and charges in that theory before comparing growth, source, and transport. The local reduced action describes zero modes in its derivative regime. The GL near region involves compact harmonics and curvature scales outside that reduction. The formal classical winding counterexample also illustrates the need to check the fluctuation kinetic operator. [Action and constraints](STABILIZATION.md)

With exact transverse \(SO(3)\) the massless tensor news is zero. A spherical massive scalar or the massive KK spin-2 spherical sector has separate timelike observables. The zero-mode Maxwell field has no spherical radiative degrees of freedom, and compact reflection removes it for symmetric standing data. A first-bulge energy budget requires a constrained backreacting development, a geometric event definition, and a controlled remainder. The tube values above apply to the specified early exterior window.
