# GOAL-011 ingoing source and mixed match, adverse audit E

Bounded non-blind check of ingoing_source.py and the shared
validate_affine call. Source versions at inspection were SHA-256
5ac5af24536f85702bc7d0df85436b6c15869b505d8c118d2f3d20341a1343d0
and 7eda3c32e3fb1e743038f5a6f0b1a44ddd1d28ee06ffbeeaf8b3db74bc2cded9.
I reviewed the three requested claims and replayed the shared code.
No source or repository file was changed.

## Compact partial moments

Set \(b_\sigma=e^{-\sigma T}(1-T')a\), \(T'=-1/r\). The two
Green moments at fixed \(r\) are the integral from the inner
support to \(r\) of \(b_\sigma(h_x+\sigma h)\) in \(dx\), and
the integral from \(r\) to the outer support of
\(b_\sigma(g_x+\sigma g)\) in \(dx\). For the increasing real
cell coordinates, \(dx/dt=\Delta r/f\). The integrand weights
therefore include the correct positive orientation and compact
launch factors. Both profiles have the same normalization as the
certified baseline Wronskian, with \(g(2.5)=1\). The outgoing
profile is propagated backwards from the high cell endpoint and
evaluated at displacement \((t-1)\Delta r\), as required.

For each sixteenth subcell \(j\) of the current cell, the
fraction to the left of a point \(t\in[0,1]\) is
\(\min(1,\max(0,16t-j))\), and the fraction to its right is
one minus this value. Multiplication of these widths by the
entire subcell integrand boxes encloses the integrals even
when \(t\) is a box and the integrand and width are correlated.
The stored prefix and suffix handle the wholly included cells.
The real support checks cover the analytic bump from
\(r=1.595\) through \(r=13/8\). Cells straddling a support
edge use the true one-sided bump range. Cells completely below
or above use the certified full moments and the appropriate
single homogeneous profile. The overlaps with the independently
certified whole moments are consistency checks, while the
partial integrals themselves are enclosed by the interval
quadrature.

Differentiating the split Green formula gives a contact
\(+(1-T')e^{-\sigma T}a\) in the derivative of the transform.
The graph distribution changes this by \(+T'e^{-\sigma T}a\),
so the physical spatial derivative adds \(+e^{-\sigma T}a\).
Its time counterpart is \(\sigma\Psi-e^{-\sigma T}a\).
These are precisely the positive launch in psix and the
negative launch in the EF radial potential reconstruction.
No factor of \(f\), \(\delta\), or Fourier half is introduced by
the partial moment routine.

## Horizon cap and affine error

The start is \(r=1.0313414502474\ldots\), below the packet and
inside the regular horizon series radius \(t_h=0.043\).
Here the baseline is its certified ingoing normalization
times the full left launch moment. The LocalHorizon
particular column solves the actual regular EF forcing with
\(U_0=z_0=0\) and the constrained inhomogeneous horizon data.
Any other regular solution differs by an ingoing homogeneous
column, so it has the same \(\chi_L=J-P_LU\). The first two
particular-column entries are normalized \(Y,Z\) after removing
\(e^{s_1x}\). The last two are their \(J-s_1(Y,Z)\) after
removing the same factor. Thus the code's cap calculation
\(e^{s_1x}[\text{last}+s_1\text{first}-P_L\text{first}]\)
has the proper shift and normalization.

The shared validator measures the actual Acb cap box against
the exact binary candidate at its first node. Its directed
residual follows \(\chi_x=-P\chi+G-PF\). Consecutive candidate
Hermite cells have identical central values at shared nodes.
Each subcell bounds the differential defect and Hermitian
numerical abscissa over the complete interval, then propagates
a scalar norm majorant. The majorant is monotone within that
subcell, so its maximum at the two ends also supplies an
interior error bound. The right zero candidate check and the
left measured cap displacement both correctly initialize the
endpoint error. Numerical splines and DOP853 are used to
generate candidates and do not certify the error.

## Preconditioned matching

Continuity of the two normalized position and derivative
vectors requires
\[
 D U=(P_L-P_R)U=\chi_R-\chi_L.
\]
Each endpoint output is its exact binary candidate plus a
zero-centered rectangular box. Its exact midpoint is that
same candidate, so the separate Euclidean endpoint error
radii are applicable about the midpoints used for the offset.
If \(M\) is the rounded point preconditioner, \(c\) the point
center, and \(b_0\) the midpoint offset, then
\[
 (I-(I-MD))(U-c)=M(b_0-Dc)+M(b-b_0).
\]
The reported radius is the resulting norm bound using
\(\|I-MD\|_F<1\) and
\(\|b-b_0\|_2\le\epsilon_L+\epsilon_R\).
The Frobenius norms safely dominate the needed Euclidean
operator norms. No exact inverse property is required of the
point preconditioner. Choosing midpoint and preconditioner
through Acb solves therefore supplies only a candidate,
with the separate residual and contraction closing the bound.

## Replay and scope

From the temporary source directory, with bytecode disabled,
OPENBLAS_NUM_THREADS=1 and pipefail, python3 ingoing_source.py
piped through diff -u against the stored two-subdivision output
returned exit 0 and no differences. The left and right
affine match error bounds are \(0.067677\) and \(0.0075300\)
rounded upward. The determinant box is separated from zero,
and the matched position Euclidean error bound is \(0.462348\)
rounded upward. This bound is broad, but I found no correctness
blocker in the partial moments, regular cap initialization,
or preconditioned matching. This audit supplies no enclosure
for the final physical overlap integral.
