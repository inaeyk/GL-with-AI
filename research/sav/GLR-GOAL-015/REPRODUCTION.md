# Reproduce the sign certificates

Run from research/sav in the GOAL-015 checkout. The experiments are
bounded, non-production fixed-background calculations. The primary
environment was Python 3.14.8, python-flint 0.9.0, NumPy 2.4.6, SciPy
1.18.0, and SymPy 1.14.0. Arb precision is 160 bits in the main proofs.
Floats propose pilot values and candidate ODE profiles. The physical
bounds use directed interval operations, exact graph and cap geometry,
analytic cell and tail inequalities, and the pinned GL spectral input.

~~~sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1
g=GLR-GOAL-015
scratch=$(mktemp -d)
python3 GLR-GOAL-014/checks/c_compact.py --n 192 --m 720 --cap 400 --outer 430 --tol 8e-14 --dump "$scratch/early.npz"
python3 "$g/analysis/early_optical_certificate.py"
python3 "$g/observables/shifted_baseline.py"
python3 "$g/observables/check_strong_cap.py" "$scratch/early.npz"
python3 "$g/observables/optical_strong.py" "$scratch/early.npz"
python3 "$g/observables/packet_weak.py"
python3 "$g/observables/packet_signed.py"
python3 "$g/source/verify_source.py"
python3 "$g/source/local_projection.py"
python3 "$g/verification/check_contacts.py"
python3 "$g/verification/check_optical_arithmetic.py"
~~~

The two weak physical certificates do not use the pilot archives.
The early strong proof uses only the digest-bound affine ordinates
from the early pilot; the digest is checked before the comparison.
The exact cap and graph identity has a rational manufactured check.
The separate candidate quadrature, graph-conforming drive lift,
packet candidate and sufficient strong packet gate are reproduced
by the exact commands in [checks/COMPACT.md](checks/COMPACT.md).
The main final sequential replay took about 7, 11, 32 and 52 user CPU
seconds for the weak early, strong early, first packet and signed
packet runs. The source track reports about 12 and 9 user CPU seconds
for the two source checks. The compact capture and two packet meshes
took about 7, 6 and 14 user CPU seconds, respectively. These timings
exclude agent reasoning and include the named bounded computations;
other small probes and failed attempts are archived in their records.
No run used more than the authorized two compute threads.

The independent verifier reconstructed the sharp-window/graph contacts,
directional energy and trace obstructions before seeing the new
derivations where feasible. Reviews of the completed directed
certificates were non-blind and independently checked the crucial
signs, graph terms, Grönwall/Volterra constants and arithmetic.
The two early routes and the packet refinement share the physical
coefficient interface and inherited spectral/outer data. The
candidate replays reuse the pinned implementations. See
[verification](verification/INDEX.md) for which checks are independent.
The external Work audit remains separate.

The illustrative figure is generated using Matplotlib 3.11.0 after
regenerating the early and refined packet archives. Curves come from
numerical cumulative projections; only the marked endpoint intervals
are certified. Pass a writable output prefix; PNG and SVG are produced.

~~~sh
python3 "$g/observables/plot_projection.py" "$scratch/early.npz" "$scratch/packet-refine.npz" "$scratch/projection"
~~~
