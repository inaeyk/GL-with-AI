# Bounded reproductions

Use the exact source head in [AUDIT.md](AUDIT.md). The audit environment was Python 3.14.8, python-flint 0.9.0, NumPy 2.4.6 and SciPy 1.18.0. The source worktree was read only. The runs used fixed-background radial ODEs, interval arithmetic, manufactured transfer checks and finite-dimensional quadratures. No production evolution was used.

## Source replay

From the repository root at the audited source commit, or from the stacked review tree:

~~~sh
cd research/sav
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1
for check in a_spectral shifted_denominators shifted_enclosure qnm_norm d_flux c_inversion
do
    python3 "GLR-GOAL-012/checks/$check.py"
done
python3 GLR-GOAL-012/checks/shifted_transfer.py --angle 40 --length 88 --left-delta 3e-7 --tol 3e-12 --cap .012 --strong
~~~

All seven stdout files matched their committed counterparts byte for byte. These runs reproduce the packet moment, threshold baseline and translated noncoincidences, full directed shifted shear interval, norm table, optical reference, manufactured inversions and long-ray weak/strong comparison. The empirical long-ray pair was \(-0.193656770802-0.424611465804i\) weak and \(-0.193656770835-0.424611465796i\) strong. The methods share lower fields. The full coupled numerator has interval replay and derivation/code review evidence. A separate full coupled interval implementation was outside these bounded checks. [Source checks and archived stdout](../../GLR-GOAL-012/checks/README.md).

## Independent calculations

From the repository root of the stacked review tree:

~~~sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1
audit=research/sav/verification/GLR-GOAL-012
python3 "$audit/independent_jost.py"
python3 "$audit/finite_window_checks.py"
python3 "$audit/optical_reference.py"
~~~

| Check | Method and result |
| --- | --- |
| [Jost matching](independent_jost.py) · [output](independent_jost.txt) | Full factored columns with independent ODE stepping and no source imports. Two choices of radius, collar, angle and tolerance recover the baseline zero, its moment and normalization, both translated inverse gates and the threshold baseline noncoincidence. The massive far seed approximates the outgoing subspace. This is empirical convergence evidence. |
| [Window norms and cap](finite_window_checks.py) · [output](finite_window_checks.txt) | Arb enclosure with analytic quadratic edge/vertex optimization, exact period factor and the improved scalar cap constant. The inputs are the pinned frequency and shear-residue boxes. |
| [Optical reference](optical_reference.py) · [output](optical_reference.txt) | Interval ray-mass inversion via the real Lambert function and a variable-ray Volterra iterate. The exact baseline enters through a signed quadrature and a higher-iterate majorant. The optical-delay bracket is inherited. |

## Negative checks and reuse qualifications

- A principal massive square root at the two lower-half-plane test points reverses the sign of the outgoing decay on the \(\pi/4\) ray. The physical choice gives rates \(0.08699\) at \(s_q\) and \(0.16848\) at \(s_*\). The unrotated inner affine rate fails at \(s_q\). [Sheet and rates](SPECTRAL.md).
- The source's archived overly wide strong interpolation gives an amplitude around \(-4.21\times10^5+7.64\times10^4i\). Short cells restore weak/strong agreement. The early denominator diagnostic demanded a real part larger than \(0.38\), while the computed returning mismatch has real part about \(0.205\) and its imaginary part excludes zero. The loose normalized Cartesian quotient includes zero, although the factor moduli separately exclude it. Those archived failure settings are distinct from the successful replay. [Source failure records](../../GLR-GOAL-012/checks/README.md).
- Naive interval powers in nearly flat optical cells can create artificial undefined values for signed balls crossing zero. The independent implementation uses direct interval products there and analytic flat endpoint bounds. The two optical computations have different valid widths. [Independent code](optical_reference.py).
- Two fixed residues permit arbitrarily different finite-window news under a change of independent upper canonical data. The shifted excitation also has a kernel among normalized lower profiles. These are analytic controls with a preparation change. The complete finite-window residual for the pinned experiment remains unevaluated. [Data constructions](WINDOWS.md).
