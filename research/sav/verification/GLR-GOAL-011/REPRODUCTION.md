# Bounded reproductions

All source runs used the checkout at [eb439228601a649c658ce719b503ff0bdc37a83f](https://github.com/inaeyk/GL-with-AI/commit/eb439228601a649c658ce719b503ff0bdc37a83f). The source was clean and its merge base with the GOAL-007 stack was the exact pin. The environment used Python 3.14.8, python-flint 0.9.0, NumPy 2.4.6, SciPy 1.18.0, SymPy 1.14.0 and mpmath 1.3.0 as applicable. All checks are non-production.

## Pinned source commands

From the repository root with the source and the audit present on the stacked review branch

~~~sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1
source_dir=research/sav/GLR-GOAL-011/checks
physical_dir="$source_dir/physical_overlap"

python3 "$source_dir/identity_controls.py"
python3 "$source_dir/barrier_delay.py"
python3 "$source_dir/barrier_delay_independent.py"
python3 "$source_dir/spectral_certificate.py"
python3 "$physical_dir/ingoing_source.py" --subdivisions 8
python3 "$physical_dir/return_coefficients_check.py"
python3 "$physical_dir/overlap_projection.py"
python3 "$physical_dir/signed_projection.py" --subdivisions 2
python3 "$physical_dir/signed_projection.py" --subdivisions 4
python3 "$physical_dir/clock_projection.py"
~~~

The two- and four-subdivision runs took about 3 and 5 minutes respectively on the audit host. The signed run uses the hard-coded eight-subdivision mixed match ball. The independent upstream replay covers that ball for the same pinned packet. [Input binding and upstream replay](ARITHMETIC.md#mixed-match-replay-and-input-binding)

| Check | Replayed result |
| --- | --- |
| Pinned identity controls | Three passes for upper-data rank, graph cancellation with horizon boundary, and outgoing launch contact |
| Optical interval, $13/8$ | $K_*\in[0.2571920718,0.3039349600]$, rounded outward |
| Optical interval, $7/4$ | $K_*\in[0.0715890229,0.0910883808]$, rounded outward |
| Independent optical script | $K_*\simeq0.2805598551946,\,0.08133784803108$ at the two radii |
| Local pole | Rouché remainder $<1.30921\times10^{-8}$ versus comparator modulus $>1.69729\times10^{-7}$ |
| Shifted pencils | $\Im D(s-2\Omega)>0.28$, $\Im\det D_k(s-\Omega)>0.40$ |
| Eight-subdivision mixed match | Euclidean radius $0.3325617325626072$ before rounding to $0.33257$ |
| Source-coefficient comparison | Largest scaled discrepancy $1.264\times10^{-14}$ rounded up, empirical |
| Strong degree 42 | $0.13523378825882212-0.3706200776960893i$, empirical |
| Two-subdivision $A_{21,q}$ | $[0.02779513896954,0.24019498095428]+i[-0.47893049647369,-0.26283615697443]$, rounded outward |
| Four-subdivision $I$ | $[0.08101411270651,0.76582255041901]+i[-3.48671807373016,-2.78474327853022]$, rounded outward |
| Four-subdivision $A_{21,q}$ | $[0.08223496939647,0.18770151123066]+i[-0.42417773441299,-0.31707155110667]$, rounded outward |
| Four-subdivision modulus | $0.32756<\lvert A_{21,q}\rvert<0.46386$ |
| Four-subdivision whole tail | $<1.383\times10^{-26}$ |
| Clock perpendicular endpoints | $[-0.44589263154194,-0.33745293738320]$, rounded outward |

The finer signed interval lies inside the coarser result. Left and right mixed defect caps decreased from $0.06130772,0.11229946$ to $0.01669852,0.03039773$, rounded upward. Both replays reproduced the stored numerical endpoints to the displayed precision. The strong-form run evaluated degrees 20, 30 and 42. Its independent strong exterior quadrature shares the lower fields and a regularized horizon collar with the weak empirical run. The pinned interval method uses separate enclosure logic and a fit-free collar.

To extract the full refined $W'$ box from the same calculation and precision used in the signed run

~~~sh
PYTHONPATH="$physical_dir" python3 - <<'PY'
from flint import ctx
from denominator_refinement import wronskian_derivative

ctx.prec = 128
ctx.cap = 128
W, *_ = wronskian_derivative()
for component in (W.real, W.imag):
    print(component.lower(), component.upper())
PY
~~~

Rounded outward, the real interval is $[7.81873470886,7.85596024942]$ and the imaginary interval is $[-1.73024927337,-1.69302372704]$. Arb's compact ball display rounds its printed midpoint, so the explicit endpoints are used for independent quotient postprocessing.

## New controls

From the same repository root

~~~sh
audit_checks=research/sav/verification/GLR-GOAL-011/checks
python3 "$audit_checks/spectral_check.py"
python3 "$audit_checks/rw_jost_check.py"
python3 "$audit_checks/rectangle_check.py"
python3 "$audit_checks/arithmetic_controls.py"
PYTHONPATH="$physical_dir" python3 "$audit_checks/mean_ricci.py"
PYTHONPATH="$physical_dir" python3 "$audit_checks/massive_manufactured.py"
~~~

- The continued fraction gives $-0.17792463137787139656-0.74734336883608367159i$. An independent direct-amplitude calculation with frequency variational equations gives $W'\simeq7.83649093605-1.70981019848i$. [Spectral code](checks/spectral_check.py)
- The direct-amplitude solver with a different horizon initialization, multiple ray angles and a frequency difference gives $W'\simeq7.8364909381-1.7098101933i$ and the clock perpendicular component $-0.391646838$. [Jost and clock code](checks/rw_jost_check.py)
- Exact rational quotient postprocessing gives $[0.08285,0.18746]+i[-0.42383,-0.31759]$ using the source numerator and denominator boxes. [Quotient code](checks/rectangle_check.py)
- The manufactured signed-adjoint identity agrees exactly at $131/15+192i/35$. Omitting the match changes the result by $3$ and conjugating only the defect covectors changes it by $-7709/630-2587i/210$. A separate nonnormal growth example and sampled massive-tail test check two bound mechanisms. [Arithmetic code](checks/arithmetic_controls.py)
- Direct five-dimensional Christoffel and Ricci assembly at four manufactured EF jets agrees with the pinned reduced implementation to $8.882\times10^{-16}$ in maximum scaled error. This tests the quadratic Ricci contractions. [Mean Ricci code](checks/mean_ricci.py)
- The manufactured Proca input with both massive polarizations and a nonzero electric source recovers $(A_t,A_x,F_{tx},A_z)$ to a maximum error of $3.955\times10^{-13}$ at the tighter setting. This is a forced-solver control. [Massive code](checks/massive_manufactured.py)

The waveform audit includes an additional independent normalized bump quadrature and the analytic positive subinterval proof. [Waveform](WAVEFORM.md#reproduction)
