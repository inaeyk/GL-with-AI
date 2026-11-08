# Numerical reproducibility and directed-arithmetic audit

Audit baseline [GLR-GOAL-015 source PR #21](https://github.com/inaeyk/GL-with-AI/pull/21) at [65a4af854a25ca02e40f31b00fe5ff789935bdc4](https://github.com/inaeyk/GL-with-AI/commit/65a4af854a25ca02e40f31b00fe5ff789935bdc4). The replay read the authors' conclusions and used their implementations. The separate affine quadrature, rounding and wedge checks use fresh review code. All calculations are bounded and non-production.

## Clean replay

The checkout was clean and had the exact source head before and after the replay. The environment was macOS 27.0.1 arm64, Python 3.14.8, python-flint 0.9.0, NumPy 2.4.6, SciPy 1.18.0, and SymPy 1.14.0. The main directed calculations used 160-bit Arb precision. The commands ran sequentially with the common compute-library thread caps set to one. The eleven primary commands took 135.39 total user CPU seconds.

Run from `research/sav` in the stacked review checkout with assertions enabled. The source paths have the pinned contents. The independent scripts reside in this review directory.

~~~sh
export PYTHONDONTWRITEBYTECODE=1 OPENBLAS_NUM_THREADS=1 OMP_NUM_THREADS=1
export MKL_NUM_THREADS=1 VECLIB_MAXIMUM_THREADS=1 NUMEXPR_NUM_THREADS=1
unset PYTHONOPTIMIZE
scratch=$(mktemp -d)
g=GLR-GOAL-015
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
python3 verification/GLR-GOAL-015/check_arithmetic.py "$scratch/early.npz"
~~~

| Pinned calculation | Fresh result | User CPU s |
| --- | --- | ---: |
| GOAL-014 pilot, 192 by 720, cap 400 | Affine cap regenerated. 193 little-endian binary64 ordinates, SHA-256 `38ecd9ed4d942aeb261cc197bf5662e9d80a584fa6dae955a322a61817915c39`. Pilot overlap 7.81810240688 is empirical. | 6.88 |
| Early weak | Enclosure `[1.70707534, 13.47109706]` rounded outward. | 6.91 |
| Shifted scalar weights | All three exact Bernstein derivative-ratio inequalities and six rational jet weights passed. | 0.59 |
| Candidate cap check | Exact manufactured cap identity passed. Separate candidate-current readout error below 18.989. | 0.37 |
| Early strong | Physical compact cap error below 323.983790. Both printed gates, 400 and 570, true. | 11.81 |
| Packet weak, absolute first pass | `[-3518.552740, 560.203914]` outward. Sign flag false as expected. | 31.66 |
| Packet weak, signed refinement | `[-2758.257097, -200.091728]` outward. Sign flag true. | 53.48 |
| Fixed-infinity source and local projection | Directed source and local projection outputs reproduced, including the signed mean, direct and affine pieces in their fixed split. | 13.98 + 9.44 |
| Contact and optical arithmetic | Exact manufactured contact, omitted-term controls and arithmetic matched. | 0.24 + 0.03 |

Nine of the eleven commands have corresponding tracked text snapshots. All nine stdout streams matched byte for byte (`analysis/early_optical_certificate.txt`, `observables/check_strong_cap.txt`, `observables/optical_strong.txt`, `observables/packet_weak_first.txt`, `observables/packet_signed.txt`, `source/verified-source.txt`, `source/local-projection.txt`, `verification/check_contacts.txt`, `verification/check_optical_arithmetic.txt`). These are deterministic replay checks. They share the coefficient construction, directed input boxes and inherited tails.

The pilot printed a SciPy DOP853 warning, `invalid value encountered in scalar divide`, and exited successfully with the exact candidate digest. The two weak routes do not load its arrays. The strong route compares the true cap trace to an arbitrary but exactly specified affine function and uses only the 193 digest-bound ordinates for this comparison. An independent one-ULP change to ordinate 100 in a scratch archive was rejected by the actual strong reference reader. The float pilot, candidate ordinates, and Arb physical enclosures therefore play distinct roles. Bit-identical archive regeneration on different SciPy builds or architectures has not been tested. The [193 exact hexadecimal ordinates](early-cap-ordinates.hex) retain this affine trace in a portable text representation. Rebuilding an archive containing only `p_cap` reproduced the physical strong output byte for byte. The separate candidate-current readout uses the full pilot archive.

## Independent arithmetic and checks

From the same directory, run the new exact and directed checks.

~~~sh
review=verification/GLR-GOAL-015
python3 "$review/check_geometry.py" .
python3 "$review/check_strong_geometry.py"
python3 "$review/check_weak.py"
python3 - "$review/early-cap-ordinates.hex" "$scratch/early-portable.npz" <<'END_PY'
from pathlib import Path
import sys
import numpy as np

lines = Path(sys.argv[1]).read_text().splitlines()
ordinates = [float.fromhex(line) for line in lines if line and not line.startswith("#")]
assert len(ordinates) == 193
np.savez(sys.argv[2], p_cap=np.array(ordinates, dtype="<f8"))
END_PY
python3 "$review/check_arithmetic.py" "$scratch/early-portable.npz"
python3 "$g/observables/optical_strong.py" "$scratch/early-portable.npz"
~~~

[Source replay excerpts](verification-output.txt) and [independent check output](independent-output.txt) preserve the results. The narrower affine pairing and the published outward interval use the same fixed-mass geometry and inherited exterior hypotheses.

The independent [review script](check_arithmetic.py) avoids importing the source implementation. For the fixed affine cap trace $p_c$, it computes the squared norm from the exact affine-panel formula and uses
$$
 \langle B',p_c\rangle = B(d)p_c(d)-\sum_i (p_c')_i\int_{u_i}^{u_{i+1}} B(u)\,du .
$$
It reads the inherited directed bracket for the bump normalizer and computes each integral by a whole-cell Arb bound on the second derivative plus the midpoint remainder. On the first eight subcells the monotonic positive bump gives a finite direct integral enclosure at the flat endpoint. An initial review calculation that divided by an interval power of $y(1-y)$ on a very early cell gave an infinite bound. Inverting before taking powers and using the monotonic front bound removed that failure.

The directed affine norm is approximately $635.49461514755506$. The independent pairing lies in $[7.83731914926,7.83731944188]$, within the inherited pairing bracket $[7.80653,7.86816]$. Using the same physical bound $E_c<323.984$ and inherited transfer, this alone gives the narrower early enclosure $[3.39131337,12.28332522]$. It independently checks the candidate pairing stage while sharing the cap and transfer estimates.

The published bracket from the inherited wider pairing follows using exact decimal rationals.
$$
 E_{\rm transfer}\le 0.024231 + 0.013648127\,(323.984)
 = 4.446005777968 .
$$
The resulting endpoints are $3.360524222032$ and $12.314165777968$, inside the published rounded bracket. The rounded signed-packet budget gives
$$
 \mathcal P_{\rm packet}\le
 \frac{-4393.82672+44.02053+2078.31966+0.44415+616.13375+854.54175}{4}
 +1.63462\times10^{-14}
 =-200.09172+1.63462\times10^{-14}.
$$
The actual script upper endpoint is $-200.091728878366364\ldots$, giving independent agreement on the final budget subtraction. Direct Arb scaling by $3/(4\pi)$ confirms outward rounding of both published energy intervals. Subtraction of the two published projection intervals gives a strictly negative contribution $[-2770.5722,-203.4515]$ on the remainder of the packet following the early cut.

The boolean test `v0 <= R` can be false on an overlapping Arb interval. For the exact default sign-proof grids, independent 160-bit evaluations of all left panel endpoints show strict separation from $R=b(a)$. The minimum separation exceeds $8.9783\times10^{-4}$ on the early grid and $4.6633\times10^{-3}$ on each packet grid. The numerical classification therefore agrees with the analytical wedge test on all replayed panels. A conservative general implementation can treat an undecidable comparison as a wedge cell.

Both packet scripts exit successfully regardless of the sign flag, so exit status by itself does not distinguish the successful refinement from the initial inconclusive attempt. A machine check should require `P_PACKET_STRICTLY_NEGATIVE True` in signed stdout and confirm `False` in the recorded negative control. The strong script likewise prints its gates, so check those results explicitly. Run with assertions enabled to retain the candidate digest and interval guard checks.

The replay verifies the numerical enclosures on the pinned ordered construction. The separate source audit checks the local GR/KK hierarchy. A new global spectral proof, a pointwise packet waveform error and a finite-amplitude remainder remain outside the replay evidence.
