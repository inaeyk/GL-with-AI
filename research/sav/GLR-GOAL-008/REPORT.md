# GLR-GOAL-008 — Stabilized radion, massive transport, and mode conversion

**Outcome B.** The audited GL quadratic source gives a quantitative fixed-background transport diagnostic. A semiclassical stabilization example solves the flat-circle vacuum and determines the leading reduced action. Its stress response on a GL black string remains open. The classical winding obstruction gives a separate Outcome C result within its specified matter class.

The baseline is [GOAL-005 source 61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708](https://github.com/inaeyk/GL-with-AI/commit/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708), reviewed at [Work audit a9bfbec0efe41b881e4b12aa262f5fcfcd94c639](https://github.com/inaeyk/GL-with-AI/commit/a9bfbec0efe41b881e4b12aa262f5fcfcd94c639). The vacuum field equations and their original domain remain those of GOAL-005. The mass is an added EFT input in Level I and a matter-induced mass in the Level II example.

## Level I — Fixed background and source

The control uses the same growing \(L/r_0=10\) mode, reference-radius seed normalization and zero completed scalar data on the finite Z slice as GOAL-005. The \(m=0\) equation and its initial data exactly recover the audited waveform. Its continuum order-four energy coefficient is \((3.5316,4.0171)\times10^{-7}\) at null infinity on \(u/r_0\in[-10,-8]\). The analytic comparison recovers that limit on the same early cuts as \(\mu^2R/r_0\to0\) and \(R/r_0\to\infty\). [Transport derivation](TRANSPORT.md) · [Independent review](INDEPENDENT_REVIEW.md)

Using the pinned empirical radial profile, the finite Schwarzschild tube at \(R=48r_0\) gives the following signed coefficients \(E_R/(M_0\epsilon^4)\) on the same coordinate cuts. The diagnostics also cover \(R/r_0=12,24\). The medium-to-fine coefficient differences are at most \(1.63\times10^{-11}\) on this grid and the maximum relative balance residual is \(4.49\times10^{-5}\). Those are internal discretization checks. The source approximation and fixed-background model remain separate limitations. [Diagnostic inputs, table and controls](DIAGNOSTICS.md)

| \(\mu\) | \(E_R/(M_0\epsilon^4)\) at \(48r_0\) | Fraction of the same tube's massless value |
| ---: | ---: | ---: |
| 0 | \(3.80015\times10^{-7}\) | 1 |
| 0.02 | \(3.74500\times10^{-7}\) | 0.985 |
| 0.05 | \(3.46819\times10^{-7}\) | 0.913 |
| 0.10 | \(2.63509\times10^{-7}\) | 0.693 |
| 0.20 | \(8.71469\times10^{-8}\) | 0.229 |
| 0.30 | \(1.33951\times10^{-8}\) | 0.0352 |

For a completed transient the massive escape spectrum is \(\omega\sqrt{\omega^2-m^2}|a_m|^2/(\pi G_4)\) for real \(\omega>m\), where \(a_m\) includes the causal Schwarzschild transfer. At fixed positive mass a smooth outgoing band vanishes on fixed null cuts as radius increases. It reaches timelike tubes at frequency-dependent delays. Subthreshold response can store energy, and the retarded Green function has a horizon channel, damped resonances and threshold tails. The finite early-cut coefficients do not measure the energy arriving after the cuts or the total escape. The flat image-kernel diagnostic separately checks the all-time spectral flux and quantifies delayed arrival. [Transport derivation](TRANSPORT.md) · [Diagnostic comparisons](DIAGNOSTICS.md)

A finite source history has real Fourier content, controlled by both its growth and the way it starts and ends. The exact Z time factor depends on radius. The imaginary frequency \(i\,2\Omega\) gives a separated particular solution and sets no real threshold. For manufactured histories with the same early exponential, changing the smooth completion changes the fraction of squared source power above \(\mu=0.30\) from approximately \(8.1\times10^{-5}\) to \(0.052\). This is time-factor power before spatial projection and energy weighting. No first-bulge source history is fixed by the early GL eigenpair. [Source spectrum](SOURCE-SPECTRUM.md) · [Gate controls](DIAGNOSTICS.md)

## Level II — Action and GL matching

- **Obstruction.** A smooth warped Poincaré-invariant circle obeying the five-dimensional NEC has \(A''\le0\), hence constant warp factor by periodicity. A Ricci-flat product then requires vanishing total stress, and canonical scalar winding has a strictly positive compact-null contraction that a cosmological constant cannot cancel. A tuned \(P(X)\) winding model solves the exact product equations and has a positive reduced radion Hessian. Its winding fluctuation has no quadratic time kinetic term, and the model lacks an open healthy perturbative neighborhood. Its formally unshifted GL mode does not establish spectral robustness in a controlled stabilization. [Classical derivation](STABILIZATION.md#1-classical-consistency-tests) · [Principal-symbol check](INDEPENDENT_REVIEW.md#4-stabilization-obstruction-and-mode-selection)
- **Consistent flat compactification.** Five-dimensional Einstein EFT with periodic massive Dirac matter, the massless gravitational Casimir term and tuned bulk vacuum energy gives \(W=W'=0\) and \(W''>0\) for the flat-circle vacuum. Both external and compact pressures vanish at the same radius. For the stated two-Dirac example, \(ML\simeq2.22940\) and \(m_\phi^2L^2\simeq27.5573\,G_5/L^3\) at leading order. The reduced Einstein equations include the stress and the Hamiltonian, momentum and Gauss constraints. The construction uses the perturbative semiclassical vacuum and small \(G_5/L^3\). [Action and constraints](STABILIZATION.md) · [Primary literature](LITERATURE.md)
- **GL shift and first bulge.** The tuned two-derivative zero-mode truncation permits a constant radion on Schwarzschild. The five-dimensional problem additionally requires a horizon-compatible state, the curved stress and its retarded nonzero-harmonic response, and a corrected background. Near the GL scale the flat potential alone controls none of these. For a simple root of the matched Evans determinant, the leading rate shift is \(-D_\eta/D_s\) and the static threshold shift is \(-D_\eta/D_k\) in their respective domains. The model has no evaluation of \(D_\eta\); the sign, magnitude and nonlinear first-bulge energy remain undetermined. [Spectral conditions](STABILIZATION.md#5-what-remains-before-a-stabilized-gl-prediction)

## Conversion at exact transverse \(SO(3)\)

The invariant symmetric tensor on a round cut is proportional to the sphere metric. Its tracefree part vanishes, so the massless tensor Bondi news is zero even in the presence of spherical matter stress. A massless Maxwell field has no invariant radiative tangent one-form. Compact reflection also sets the metric vector zero mode to zero in the standing GL family, when the action, state and data share the symmetry. The radion monopole and the massive KK spin-2 spherical sector are allowed. [Symmetry derivation](STABILIZATION.md#4-leading-interactions-and-symmetry)

| Channel | First relevant classical coupling or restriction |
| --- | --- |
| Zero-mode tensor | \(h_{\alpha\beta}T_\varphi^{\alpha\beta}/M_4\) is present. The tuned minimal action has no one-radion, two-graviton tree vertex. Exact \(SO(3)\) eliminates tensor news. |
| Zero-mode vector | \(-\sqrt6\,\varphi\mathcal F^2/(4M_4)\) allows interaction with a seeded vector. The neutral classical equation admits \(F=0\) throughout. A quantum decay to two photons is a separate process. |
| Zero-mode radion | \(+1,-1\) GL harmonics source the monopole at order \(\epsilon^2\). For positive mass the outgoing right channel requires real \(\omega>m\). |
| Massive KK spin 2 | The mass derivative of \(k_n^2e^{-2\sqrt3\phi}\) gives \(\varphi H_nH_{-n}\). A radion can scatter an already populated KK mode. Translation symmetry prevents creating a single charged harmonic from pure zero modes. \(+1,+1\) generates \(+2\) at order \(\epsilon^2\). At the vacuum L10 reference its threshold is \(\omega r_0>2\pi/5\), subject to matching corrections in a stabilized model. A quantum decay into a pair requires \(m>2M_n\) and a nonzero matrix element. |

The thresholds and symmetry rules delimit possible channels. They give no relative luminosity without actual source histories, retarded overlaps and the coupled energy balance. The Level I result establishes finite-tube scalar transport for specified data, and Level II establishes the tuned asymptotic compactification. A matched stabilized GL transition and its physical total escaped energy remain the open gates for Outcome A.

[Comparison handoff](HANDOFF.md) · [Reproducibility](REPRODUCIBILITY.md) · [Five-specialist activity and checkpoints](AGENT-ACTIVITY.md)
