# GLR-GOAL-009 — Nonlinear Vacuum Bondi Radiation Theorem

## Authorization and primary mission

You are launching a new, ambitious, extended theoretical research goal in the repository `inaeyk/GL-with-AI`.

**Primary objective:** Establish a rigorous nonlinear Einstein-gravity theorem for the gauge-invariant, early-cut radion radiation produced by a small Gregory–Laflamme perturbation of a five-dimensional Schwarzschild black string on \(\mathbb R^{1,3}\times S^1\).

Build directly upon GLR-GOAL-005 and the audited GLR-GOAL-006 Outcome B. The central challenge is to **prove**, rather than assume, the nonlinear exterior, differentiated massive-ray, and optical matching estimates needed to upgrade the previously certified quadratic waveform to physical Bondi news.

This is a proof-oriented research project, not a production numerical simulation.

Use the standard **five-agent research protocol**, with independent analytic reconstruction, adversarial verification, bounded computational checks, literature assessment, and integration.

Work autonomously through the milestones. An incomplete intermediate lemma is normally not a reason to terminate the goal. Continue pursuing other viable routes within the authorized budget, while recording failed methods and rigorous obstructions.

## 1. Repository, branch, and scientific provenance

Repository: `inaeyk/GL-with-AI`

Proposed working branch: `codex/glr-goal-009-vacuum-bondi`

Base the new branch on the exact audited GOAL-006 scientific source commit:

`80f030df0bec99bebe76fd9ab473c419ad1af5a5`

Original GOAL-006 Codex PR:

https://github.com/inaeyk/GL-with-AI/pull/5

Independent GOAL-006 Work audit:

https://github.com/inaeyk/GL-with-AI/pull/7

Audited review commit:

`c4cac8f2e2dd12cfbdf50651863d2188a75fc87c`

Full report:

https://github.com/inaeyk/GL-with-AI/blob/c4cac8f2e2dd12cfbdf50651863d2188a75fc87c/research/sav/verification/AUDIT_GOAL006_80f030df.md

Also inspect the GOAL-005 source commit:

`61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708`

and its independent Work review:

https://github.com/inaeyk/GL-with-AI/pull/3

Read the relevant GOAL-005 and GOAL-006 derivations, constraint constructions, radiation calculations, verification records, negative controls, and Michael comparison handoffs.

Pin and verify all input SHAs. If a source PR has moved, preserve the audited version and record the difference.

The Work review is independent scientific evidence, not a branch to merge into the research source.

Create the new branch only after checking that it does not already contain active work. Keep the source branches unchanged.

All authorized writes must remain under `research/sav/**`. Do not modify simulation code, numerical formulation, existing manuscript files, existing completed-goal records, or repository-wide policies.

## 2. Fixed physical problem

Work in five-dimensional vacuum Einstein gravity with exact transverse SO(3) symmetry.

The background is Schwarzschild_4 × S¹, with

- \(L/r_0=10\),
- \(G_4=G_5/L\),
- \(M_0=r_0/(2G_4)\),
- first compact GL harmonic \(k=2\pi/L=\pi/(5r_0)\),
- growth rate \(\Omega r_0\simeq0.06328119\).

Use the future-horizon-regular growing GL mode, normalized by the completed sphere-radius perturbation

\[
H_{\theta\theta}(2r_0)/(8r_0^2)=1.
\]

Retain its actual normal velocity and the audited finite initial slice

\[
\tau=v-r=0.
\]

The Z preparation sets the completed second-order scalar value and normal derivative to zero on that slice.

Use the exact, damped constraint construction established in GOAL-006 as the preferred initial-data family. This preparation preserves

- the exact first GL jet;
- \(b=1\) and \(K^z{}_z=0\) on the initial slice;
- the relevant inner-collar prescription;
- quadratic ADM mass tuning to \(M_0\);
- the original quadratic mean radion waveform.

Preserve the distinction between this damped family and the original undamped preparation, particularly for higher-order and massive KK data.

Do not silently alter the physical preparation, perturbation normalization, or clock.

## 3. The theorem to establish

Define outgoing physical cuts using a geometrically normalized optical coordinate \(u_B\), with invariant sphere-areal radius \(r_5\) and four-dimensional Einstein radius \(r_E\).

On a cut, let \(\ell\) be the physical compact-circle circumference.

Define

\[
\phi_{\rm len}=\frac{\sqrt3}{2}\log(\ell/L)
\]

and

\[
N_\epsilon(u_B)
=\partial_{u_B}
\lim_{r_5\to\infty}r_E\phi_{\rm len}.
\]

The main target is to establish, on a fixed sufficiently early interval \(I\),

\[
\boxed{
\|N_\epsilon-\epsilon^2n_Z\|_{L^2(I)}
\le C_I\epsilon^4
}
\]

for sufficiently small \(|\epsilon|\), with a constant independent of \(\epsilon\).

The physical retarded coordinate must be normalized relative to the fixed Schwarzschild-string background and anchored consistently with GOAL-005.

Prove the required parity under the half-circle translation taking \(\epsilon\to-\epsilon\), including the optical normalization. Do not assume that parity automatically controls the asymptotic limit.

If the theorem holds, derive the corresponding scalar radiation-energy result

\[
\frac{E_I}{M_0}
=\mathcal K_Z\epsilon^4+O(\epsilon^6)
\]

and a genuine positivity statement for sufficiently small nonzero \(\epsilon\), with all asymptotic and flux-normalization assumptions justified.

The audited GOAL-005 interval is

\[
I/r_0=[-10,-8],
\]

with

\[
3.5316\times10^{-7}
<\mathcal K_Z
<4.0171\times10^{-7},
\]

and representative estimate \(\mathcal K_Z\simeq3.7325\times10^{-7}\).

**Primary acceptance:** Establish the nonlinear theorem on a sufficiently early, nontrivial interval with the appropriate quadratic coefficient identified and independently controlled.

**Stretch acceptance:** Extend it to the exact GOAL-005 interval and certified coefficient. If a different interval is used, establish its own coefficient and nonvanishing rather than transferring the GOAL-005 lower bound without proof.

An assumed nonlinear exterior theorem, assumed optical matching map, or assumed quartic source remainder is not an unconditional solution.

## 4. Milestone A — Identify a viable nonlinear exterior formulation

Develop a five-dimensional Einstein formulation suitable for a positive-mass, Schwarzschild-relative outgoing wedge.

Investigate at least two plausible approaches:

1. A hyperbolic or generalized-harmonic formulation with Schwarzschild-subtracted variables and constraint propagation.
2. A geometric optical/null formulation that exploits exact SO(3) reduction and the compact-normal identities of GOAL-006.

Select the more promising primary route after evaluating their nonlinear principal parts, characteristic geometry, constraint control, gauge behavior, and possible derivative losses.

Produce a precise theorem statement specifying:

- the initial-data regularity and weighted norms;
- the inner boundary or causal domain of dependence;
- the early outgoing wedge;
- the allowed smallness range;
- the regularity and asymptotic conditions at null infinity;
- the gauge and constraint propagation requirements;
- the quantities controlled uniformly as the extraction radius grows.

The proof must account for the GL instability. A fixed early retarded cut reaches arbitrarily late Schwarzschild time as \(r\to\infty\), so finite-time Cauchy existence does not by itself provide the desired estimate.

Investigate the competing GL growth and radial decay along outgoing null directions. The linear decay rate \(\sqrt{k^2+\Omega^2}-\Omega>0\) is promising, but its nonlinear analogue must be justified.

Avoid invoking global stability of an unstable black string.

## 5. Milestone B — Close the differentiated massive-ray estimates

GOAL-006 identified a sufficient dyadic estimate for the compact-dependent modes, but did not prove it for the nonlinear Einstein development.

Build a perturbative energy hierarchy in the preferred gauge.

Treat separately:

1. the odd first-harmonic GL field;
2. the oscillatory even field sourced at quadratic order;
3. the odd higher-order error after subtracting the first GL jet;
4. the mean zero-mode geometry and scalar field.

Prove or disprove the commuted energy inequalities that control the massive ray source terms uniformly over the early wedge.

Derive the nonlinear deformation tensors, commutator terms, gauge sources, lower-order curvature contributions, and mode-coupling terms rather than replacing them with unspecified harmless errors.

Investigate whether a renormalized energy with sufficiently mild growth can yield the dyadic summability exponent \(\delta<1\) required by GOAL-006.

In particular, test whether a Schwarzschild-relative deformation estimate of the schematic form

\[
a(t)\lesssim\frac{\delta}{R+t}
\]

and an appropriately weighted source-work integral can close the differentiated flux hierarchy.

These are candidate estimates, not assumptions to declare proved.

Check the distinction between massless outgoing cone energy and timelike massive KK transport. The two are not interchangeable.

The Schwarzschild background's nonzero extrinsic curvature in the audited slicing must be subtracted correctly. The absolute weighted small-data norms of existing complete-data KK theorems need not apply directly.

Give explicit estimates for every nonlinear error term required at the target perturbative order.

## 6. Milestone C — Close the exact scalar trace estimate

Start from the exact compact-normal mean divergence identity of GOAL-006,

\[
\partial_a\langle
Dh^{ab}\partial_b b-DX^aT_z
\rangle
=
\langle bD(T_z^2-K_z:K_z)\rangle.
\]

Reconstruct its derivation independently, with all signs and density weights.

Identify the nonlinear scalar potential, source, and data terms in a common characteristic chart.

Establish the ray-integrated forcing remainder and initial-data estimates needed to obtain

\[
\|\partial_u\psi_\epsilon
-\epsilon^2\partial_u\psi_2\|_{L^2(I)}
\lesssim\epsilon^4
\]

at null infinity.

Pay particular attention to:

- the precise completed Z scalar jet;
- massive quadratic forcing and its differentiated ray integrability;
- the dependence of the potential on the evolving mean geometry;
- the parity-controlled remainder;
- the infinite-radius limit, rather than merely smooth dependence on compact spacetime regions.

Investigate the alternative full-optical ray identity derived by the GOAL-006 independent verifier. It may be possible to obtain the scalar theorem using compact and outgoing derivatives without first proving the strongest mean-chart estimates.

Compare these two approaches and identify the weaker sufficient hypotheses.

If neither closes, produce the sharpest new unconditional scalar estimate and isolate the exact term that cannot be bounded.

## 7. Milestone D — Construct and normalize the physical optical map

This is an essential part of the theorem, not an optional gauge appendix.

Construct outgoing optical cuts satisfying the full five-dimensional eikonal equation, with a physically meaningful asymptotic normalization.

Study a terminal-value optical construction, a geometric characteristic construction, or both.

Control:

- compact-direction optical momentum;
- generator Jacobians and possible caustics;
- mass-dependent logarithmic terms;
- dynamical retarded-time shifts;
- the relation between mean-chart cuts and full five-dimensional optical cuts;
- radius and circumference matching;
- differentiability of the limiting radiation field.

In particular, address the two independent obstructions recorded by the GOAL-006 audit:

**Compact optical tilt:** Small but poorly prepared periodic tilt can create caustics at sufficiently large radius even in a flat product geometry.

**Nonuniform clock shifts:** A finite-radius \(O(\epsilon^2\log r)\) retarded-time correction can invalidate interchange of the small-seed and infinite-radius limits.

Do not substitute an instantaneous Bondi mass into a static tortoise formula as a clock normalization.

Prove a suitable asymptotic comparison between the physical clock \(u_B\) and the perturbative reference clock, including a derivative estimate on a padded interval.

Use the gauge-invariant compact circumference rather than the uncorrected mean of \(g_{zz}\).

The audit also established that convergence of the field trace does not imply convergence of its fixed-radius tube derivatives. Treat the additional tube derivative term independently if making a finite-radius extraction claim.

A successful optical analysis must explicitly verify that the desired physical cuts and the matched initial Z jet belong to the same geometric construction.

## 8. Milestone E — Combine the results into a Bondi theorem

Assuming Milestones A–D are established, assemble their estimates into a single rigorous proof.

Prove existence of the geometric scalar radiation field, its normalized \(L^2\) news trace, and the quartic remainder.

Confirm the asymptotic Einstein-frame flux normalization independently through both:

- the dimensionally reduced scalar stress tensor;
- the five-dimensional transverse gravitational strain.

Verify that both give the same coefficient proportional to \(N^2/G_4\).

Derive the radiation-energy enclosure and specify the dependence of its constant on initial norms, the interval, and the exterior geometry.

If possible, obtain an explicit nonzero seed range in which the lower energy bound remains positive.

Do not identify early-cut radiated energy with the total radiation of the fully nonlinear first bulge or pinch-off.

Exact SO(3) symmetry excludes massless tensor Bondi news, but does not exclude scalar radiation. Keep the massive KK timelike channel separate.

## 9. Independent five-agent protocol

Use the established five specialist roles, with distinct bounded tasks.

**Agent 1 — `physics_investigator`**

Primary responsibility: nonlinear Schwarzschild-relative Einstein formulation and differentiated massive-ray estimates.

Independently derive the energy hierarchy, identify closure conditions, and search for a viable bootstrap. Supply precise lemmas with every top-order term exposed.

**Agent 2 — `verification`**

Primary responsibility: optical geometry, invariant news, and adversarial proof review.

Begin with an independent geometric reconstruction before comparing detailed proposed arguments. Challenge the gauge, mass-log cancellation, compact tilt, Jacobian bounds, and limit exchanges. Attempt counterexamples.

**Agent 3 — `computational_checker`**

Primary responsibility: symbolic consistency and bounded non-production manufactured checks.

Independently verify key Einstein identities, compact-normal curvature contractions, scalar-source normalizations, selected commutator signs, and flux factors. Construct negative controls for wrong signs, gauge shifts, and derivative-limit assumptions.

Only lightweight symbolic calculations, manufactured examples, and tightly bounded perturbative ODE or characteristic checks are permitted.

**Agent 4 — `literature_researcher`**

Primary responsibility: identify rigorous tools for positive-mass exterior Einstein estimates, terminal optical constructions, wave–Klein–Gordon interactions, compactified gravitational asymptotics, and nonlinear radiation fields.

Inspect the actual statements and hypotheses of relevant primary papers, including the work of Huneau–Stingo–Wyatt, Hintz, Lindblad, and Ferko–Satishchandran–Sethi.

Identify precisely which results apply, which require adaptation, and which cannot be imported. Do not replace missing proofs with general citations to stability literature.

**Agent 5 — `integration`**

Primary responsibility: independent provenance verification, cross-agent consistency, checkpoint integration, reproducibility records, publication hygiene, and the comparison handoff for Michael.

Check the source SHAs and verify that the final record does not overstate partial results or conceal negative findings.

The coordinator should synthesize the five workstreams, resolve disagreements, and write the primary proof or obstruction record.

Record actual agent invocations, contributions, limitations, and independence. Do not claim an agent or verification step ran unless it did.

## 10. Independent verification and falsification

For every central theorem claim, require a derivation and a distinct critical review.

Where feasible, use two genuinely different analytic formulations.

Include explicit negative controls for:

- the wrong compact-normal Ricci sign;
- a missing lapse or compact-shift contribution;
- loss of the quadratic zero-mode source;
- using pointwise massive-potential positivity where it fails;
- incorrect treatment of the Schwarzschild mass logarithm;
- a flat compact-tilt optical caustic;
- failure of fixed-tube derivative convergence;
- a non-geometric circle-length average;
- exchanging the seed and extraction-radius limits without uniform bounds;
- confusing a mean-chart null hypersurface with a full five-dimensional null cone.

Preserve all counterexamples, failed estimates, and unsupported claims as scientific results.

A successful symbolic identity or manufactured check does not establish nonlinear Einstein control.

## 11. Stretch objectives

Attempt these only after substantial progress toward the primary theorem:

1. Extend the theorem to the exact benchmark interval \(u/r_0\in[-10,-8]\).
2. Obtain an explicit, independently certified bound on \(C_I\) and a finite seed range for positive radiation.
3. Determine whether the original undamped GOAL-005 preparation admits the same physical theorem under weaker decay hypotheses.
4. Establish an invariant finite-radius extraction estimate sufficiently strong to support a future comparison with Michael's numerical waveforms.
5. Identify a generalizable positive-mass KK exterior theorem applicable beyond the particular GL mode.

Do not divert into GOAL-007's nonspherical tensor amplitudes, GOAL-008's stabilized-radion dynamics, or the separate quantitative massive-second-harmonic goal.

## 12. Outcome and stopping rules

This is a substantial extended goal. Use at most **60 substantive research cycles**, with explicit synthesis checkpoints approximately every 12 cycles.

The coordinator may allocate effort adaptively across the five agents, but must not silently expand the budget.

At each checkpoint record:

- newly established mathematical results;
- failed hypotheses or attempted inequalities;
- independent verification status;
- the strongest remaining obstruction;
- a decision on which authorized analytic approach to prioritize next.

Do not terminate merely because one promising ansatz fails. Test the alternative geometric or energy formulation before ending.

Use the following outcome categories:

**Outcome A — Physical theorem established.**
The nonlinear exterior, required ray bounds, and optical map are proved for a specified family and early interval; the physical Bondi-news estimate follows.

**Outcome B — Substantial new partial theorem.**
At least one previously missing, nontrivial analytic gate is closed unconditionally, with its assumptions proved for the specified setting. Remaining dependencies are isolated in precise mathematical form.

**Outcome C — Rigorous obstruction or counterexample.**
A proposed route or natural theorem formulation is shown to fail, with a precise counterexample or no-go result under explicitly stated assumptions and a viable reformulation if available.

**Outcome D — Unresolved.**
No substantial new theorem or obstruction is established. Report the strongest valid calculations, failed methods, and exact open lemmas without inflating the result.

A mere restatement of GOAL-006's conditional estimate does not qualify as Outcome B.

Do not claim a first-bulge radiation fraction or a full nonlinear GL evolution from early exterior control.

## 13. Required deliverables

Create a new dossier under

`research/sav/GLR-GOAL-009/`

containing at least:

- `GOAL.md` — contract, assumptions, milestones and acceptance criteria.
- `REPORT.md` — scientific conclusions and clearly labeled outcome.
- `THEOREM.md` — exact theorem statements, proof dependencies and status.
- `DERIVATIONS.md` — full derivations, commutators, energy identities and optical construction.
- `VERIFICATION.md` — independent reconstructions and adversarial findings.
- `OBSTRUCTIONS.md` — failed routes, counterexamples and unresolved analytic gates.
- `REPRODUCIBILITY.md` — source pins, scripts, versions, inputs, commands and results.
- `AGENT-ACTIVITY.md` — five-agent execution and checkpoint ledger.
- `HANDOFF-MICHAEL.md` — physical observables, preparation and cut matching, finite-radius requirements, and items requiring future numerical validation.

Include small verification scripts and saved outputs where genuinely useful.

Use Markdown with LaTeX mathematics. Preserve a readable proof dependency graph showing exactly what is proved, what follows conditionally, and what remains conjectural.

Maintain the existing research history. Do not rewrite the GOAL-005 or GOAL-006 records.

## 14. Publication and final reporting

Create coherent local commits and publish the authorized work to the new Codex branch. Open a draft PR targeting the GOAL-006 scientific source branch, preserving the stacked ancestry.

Before pushing, verify the outgoing diff, allowed file paths, full commit history, base and destination. Never force-push or merge.

The PR description must include the primary theorem target, achieved outcome, newly proved estimates, unresolved gates, verification status, and comparison with GOAL-006.

At completion report:

- the new draft PR URL;
- branch and full final commit SHA;
- exact source/base commit SHA;
- the strongest rigorous physical or mathematical result;
- any failed critical proof step;
- independent verification status;
- the next precise analytic milestone;
- whether the result is suitable for an independent ChatGPT Work scientific audit.

After Codex completion, the separate Work review will be requested under the established publication protocol: pin the source, independently audit it, publish a separate review draft PR, and cross-link the original.

**No production numerical simulations. No changes to Michael Yan's GRChombo code or workflow. No manuscript edits or merges.**

## Final instruction

The scientific aim is not simply to reproduce another small-amplitude scalar-wave calculation.

We already have a nonzero, quantitatively controlled quadratic radiation coefficient. The challenge now is to understand whether that perturbative calculation becomes a genuine, gauge-invariant statement about the exact vacuum Einstein evolution.

Pursue the strongest attainable theorem with methodological ambition, independent verification, and rigorous treatment of obstructions.

Begin by pinning the inputs, reconstructing the GOAL-006 review's three outstanding gates, launching the five specialists, and writing the precise theorem and proof-dependency plan. Then proceed autonomously through the authorized research budget.
