# GOAL-003 adversarial Audit I: forced geometry, inverse and endpoints

2026-10-22. Separate scientific AI investigator/reviewer `forced_master`;
not an external human referee. Input branch research/sav-radiation@
`ef646d56ab7d8bebca578afab53e62da5146ec05`. Read the full controlling
Goal/review gates, scoped contract/notation, linear and quadratic geometry,
quadratic source, and the prior foundation/GOAL-002 geometry audits.
No production files, physical evolution, mode survey or new 1D BVP were
used. The saved L10 input was not recalculated by this reviewer.

**Verdict:** the explicit source-aware master and reconstruction survive
an independent exact check. The all-equation equivalence closes through
the three source Bianchi identities with no hidden homogeneous constraint
kernel. An independent positive-supersolution argument proves the L10
n=2 scalar operator is nonnegative, hence its s>0 Laplace inverse is
bounded and nonresonant. The EF pole cancellation and constructive
regular tensor derivative bounds are established under the stated smooth,
conserved, weighted-source assumptions. These results do **not** certify
the empirical L10 source/profile errors, an evaluated physical M2, an
exact simultaneous nonlinear GL family, a nonlinear radiation remainder,
or a finite-amplitude/first-bulge claim.

## 1. Independent route before inspecting the primary implementation

The first independent result was a positive supersolution of the known
homogeneous Schrödinger operator:

\[
w=e^{-r/2}r/(1+ar^3),\qquad a=K^2,
\quad H_aw/w=fP_a(r)/(4r^2(1+ar^3)).
\]

All coefficients of P_a(1+d) become strictly positive upon a=3/2+b,
b≥0, d≥0. This proves H_a≥0 by the exact ground-state transform, so
||(H_a+s²)^−1||L²≤s^−2 for s>0. L10 a=4π²/25>3/2 is covered
without using a sampled Wronskian or the empirical GL threshold.
The source-aware master was then supplied by the primary investigator;
the reviewer independently built a linear Ricci operator from direct EF
Christoffels on the three-dimensional orbit and
Ric5_ab=Ric3_ab−2R^−1 Hess_ab R,
Ricθθ=1−(grad R)²−R Box R. This **R-based** route differs from the
primary W=R² component construction.

Take four completely arbitrary radial EF profiles A(r),B(r),C(r),S(r),
with e^{sv+iKz} dependence and qzA=0, and manufacture Q=−R'[q].
This preserves all source identities by construction while keeping
affine trace/divergence nonzero. The implementation independently checks
all three expanded Bianchi identities, the primary master, and all four
reconstructed components. The algebra uses positive real K=3/2 and
s=1/3, so it has none of the artificial real-r poles of the old
imaginary-k specialization. It remains a symbolic interface control,
not a physical GL mode or a proof obtained by numerical sampling.

General-a,s analysis additionally verifies the residual-Bianchi reduction
−(a+r^−3)d/2=0, so D=1+ar³>0 excludes a lost constraint mode.
It also verifies the source-dependent horizon cancellation described
next. These general identities complement, rather than inflate, the
positive-K manufactured test's scope.

## 2. Horizon reconstruction is checked component by component

The simpler independent EF map uses Z=f e^−sx qtr and Y=e^−sx y:

\[
Z=-\frac{s}{aD}[fY'+(s-1/(2r^2))Y]
+\frac{2r^2}{D}(f j-rfQ_{vr}-rQ_{vv}),
\]
\[
\widehat q_{\theta\theta}=rZ/s-Y/(2ar),\quad
\widehat q_{vr}=-Z'/s-2Z/(sr)+j/s+\tau,\quad
\widehat q_{vv}=Z-f\widehat q_{vr},
\]
\[
\widehat q_{rr}=N/f,\quad
N=2Z'/s+2Z/(sr)-2j/s-\tau+Y/(ar^3).
\]

After the first regular-master recurrence at r=1,

\[
N(1)=-\frac4{s(a+1)}
[Q_{vv}'+Q_{vr}+2Q_{vv}+sa\tau/4-sQ_{\theta\theta}+aj/2]_{r=1}.
\]

The bracket is the horizon v Bianchi identity for the **Einstein**
combination of the source. The executable verifies this equation for
general a,s. It rejects a deliberately nonconserved source, which leaves
the pole. This supplies the exact compatibility that a finite master
alone cannot provide. The divided difference N/(r−1)=int_0^1 N' gives
explicit derivative bounds without dividing a numerical cancellation by
small f. Higher derivatives are controlled through the regular-master
recurrence and the positive first-order derivative equations.

The full construction in
[goal003_master_independent.md](../physics/goal003_master_independent.md)
defines X_m,Y_m, derivative loss, collar/middle/exterior bounds and a
finite coefficient algorithm. No floating local residual is used as a
metric norm. On a τ slice the e^{sr} factor is included; the exterior
conversion is exactly e^{sr}qhat=(r−1)^−s qstatic. Constants grow as
e^{sT} for the separated profile on 0≤τ≤T. The estimate is a point
resolvent/source-space result, not a retarded nonlinear redshift theorem.

## 3. Green sign, infinity forcing and remaining quantitative limits

For W=uH uI'−uH' uI<0 the correct kernel is
−uH(x<)uI(x>)/W, whose derivative jump is −1. Positivity follows from
the same coercive scalar operator; no floating W sign is relied on.
The two finite-limit variation-of-constants integrals must both be kept.
An isolated overlap extended to infinity may diverge when the source
and homogeneous exponent coincide, although the full solution decays.

For mathfrak F~F0 e^−αr r^p, the exact leading asymptotic operation gives
2α(ν+β)e^−αr r^{ν−1} on e^−αr r^ν. Generic particular power is p+1;
the exceptional p=−β−1 requires a logarithm. This audit's manufactured
tail check uses an exact elementary equal-exponent solution and rejects
a source-free outer tail. The **actual** GL-source leading coefficient
and any transverse-null-wave cancellation require the primary tail
calculation; the manufactured check is not a substitute for it.

An effective scalar and regular-metric inverse bound does not enclose
the input source: C0, Omega, and the saved H derivatives retain their
empirical status. No evaluated physical M2 or numerical amplitude
threshold is certified by this audit. The massless n=0 source and
quadratures remain the prior audited inputs; this reviewer did not
rerun their entire calculation or silently promote those records.

## 4. Deliberately wrong cases actually rejected

- Omitted forced trace and divergence: fail the manufactured zA equations.
- Omitted affine source lift: fails metric reconstruction for arbitrary EF data.
- Doubled complex +2 harmonic coefficient: fails exact cosine Fourier integral.
- Lost half-Lie-squared term: leaves a spurious pure-gauge zero mode −1/2.
- Incorrect Green sign: gives derivative jump +1 instead of −1.
- Bounded master with wrong horizon recurrence: reconstructed rr pole
  coefficient is c1/2−5/6 in the chosen exact control.
- Source-free infinity tail under equal-exponent forcing: fails the
  inhomogeneous equation; x e^−αx/(2α) is required in the flat control.
- Nonconserved Einstein source: leaves the explicit horizon qrr pole.
- Direct assertion V≥0: false, V/(r−1)→−11/10 at a=3/2.

Two distinct positivity approaches were used, as budgeted. The trial
w=r/D only proves a≥2 and fails to cover L10; adding e^−r/2 proves
a≥3/2. Their formulas and failed inequalities are retained. Exact
symbolic audits were developed in bounded additions; no physical ODE
refinement or numerical campaign was launched.

## 5. Execution and primary-artifact inspection

Command:

```sh
python3 research/sav/verification/goal003_audit_master.py > research/sav/verification/goal003_audit_master_results.txt
```

The final execution exited 0 with **33 PASS checks**, Python 3.14.8 and
SymPy 1.14.0. The off-shell master-identity coefficient was checked
separately from merely checking its zero set. Versions and per-test results are retained in
`goal003_audit_master_results.json`; the complete console transcript is
`goal003_audit_master_results.txt`. The final script contains independent
operator construction, all executed wrong cases and exact assertions.
This is a separate AI audit, not external peer review or a physical
acceptance test.

After the independent derivation, the reviewer read the actual primary
`physics/goal003_forced_master.md`, including equations (1)–(8). The
source functional (3), affine lift (4), and reconstructed tensor (5)
agree with the independently checked formulas. The residual Bianchi
coefficient in (7) agrees exactly. The s≠0,K≠0 restriction, D>0,
complex-harmonic halves and source sign are all stated. The source
generator is explicit and local; it conceals no inverse. The note
correctly distinguishes scalar inversion, regular metric control,
physical input certification and nonlinear closure.

**Withheld:** an interval-enclosed L10 quadratic source/M2, exact
simultaneous GL Cauchy data, a retarded regular-metric/radiation trace
bound for nonlinear forcing, a positive quantitative A*, or a controlled
first-bulge claim. None is implied by the exact passes above.

## 6. Actual quadratic infinity tail: bounded final check

The final independent tail calculation now resolves the leading-source
caveat in section 3. It uses an orbit determinant-density expansion of
Rθθ=1−□₃W/2, the on-shell GL reconstruction, and generic q>Ω>0 with
k²=q²−Ω². It imports neither the primary quadratic source generator nor
its expressions. After factoring the exact u₁, the order-u₁² constant
in Qθθ cancels and the real-cosine +2 coefficient is
Qθθ=−q u₁²/(4r)+O(u₁²/r²). The independent null-plane calculation gives
H_AB H^AB=3u₁²/(2r²) and the leading tr/zz/zA components. Together these
give the actual forced Schrödinger source

\[
\mathfrak F/u_1^2=-7q/(16r^2)+O(r^{-3}).
\]

With α₂=2q and β₂=2β₁, the normalized particular tail is
u₂P=(7/64)c₁²e^{−2qr}r^{−β₂−1}(1+O(r⁻¹)). The full solution includes
c₂uI, whose leading decay is e^{−2qr}r^{−β₂}; the global endpoint problem
sets c₂. Pure inverse-power forcing has no resonant particular recurrence
denominator and no forced logarithm. Omitting the angular subleading
term deliberately gives the wrong leading source +9q/16 and is rejected.

The exact command was

```sh
python3 research/sav/verification/goal003_audit_master.py --tail-only > research/sav/verification/goal003_audit_master_tail_results.txt
```

It exited 0 with **6 PASS checks**, Python 3.14.8 and SymPy 1.14.0.
The separate JSON and complete console output are
`goal003_audit_master_tail_results.json` and
`goal003_audit_master_tail_results.txt`. The earlier 33-check run was
not repeated, and no ODE or full source-generator rerun was launched.

The asymptotic hypothesis is a differentiated decaying GL expansion:
after factoring e^{−qr}r^{−β₁}, its expansion through 1/r has an O(r⁻²)
remainder with derivatives through order four bounded by O(r⁻²⁻ʲ).
The proof and explicit metric expansion are recorded in section 4a of
`physics/goal003_master_independent.md`. No numerical matching radius
or remainder constant has been certified for the saved L10 profile.
The leading coefficient and no-log structure are exact asymptotic
results under this hypothesis; the physical B1 endpoint/source
enclosure remains partial.

The primary `goal003_source_checks.py` and its generated output were
inspected only after the independent derivation. The reported q=1
leading coefficient −7/16 agrees. A printed r⁻⁴ coefficient was found
unsupported because its logarithmic-derivative jet omits an O(r⁻³)
term that can affect that coefficient. That finding was sent to the
primary investigator, who will preserve the raw truncated-jet output
with its limitation and restrict scientific claims to supported orders.
This tail audit does not independently certify the primary r⁻³
coefficient or supply a physical input/remainder bound.
