# GOAL-011 signed projection, adverse audit E

One bounded non-blind comparison against the scratch defect variant
of signed_projection.py and its directed_vector helper. I derived the
continuum identity first from the stated matched system and then
compared the code. No code or repository edits.

## Signed functional

Use the bilinear transpose and write \(H=G-PF\). On a branch
\(U_x=PU+L+F\), \(L_x=-PL+H\), define the defects of arbitrary
piecewise smooth candidates by
\[
 d_z=z_x+P^\mathsf T z+a,\qquad
 d_w=w_x-P^\mathsf T w+z+b .
\]
Then
\[
 a^\mathsf T U+b^\mathsf T L+c
 =-(z^\mathsf T U+w^\mathsf T L)_x
   +z^\mathsf T F+w^\mathsf T H+c
   +d_z^\mathsf T U+d_w^\mathsf T L .
\]
This follows by differentiating the two products. In particular
the defects have positive signs. On a directed cell with
parameter \(t\), replace \(d_z,d_w\) by
\(z_t+x_t(P^\mathsf T z+a)\) and
\(w_t+x_t(-P^\mathsf T w+z+b)\). The code's
residuals and residual_derivative use these signs and the actual
\(x_t\).

With left cap \(c_0\), match \(m\), and far endpoint \(f_0\),
including the collar \(C-z_c^\mathsf T U_c\) gives the endpoint
sum
\[
\begin{split}
 &C+(z_L(c_0)-z_c)^\mathsf T U_c+w_L(c_0)^\mathsf T L_L(c_0)
   -z_R(f_0)^\mathsf T U_f-w_R(f_0)^\mathsf T L_R(f_0)\\
 &\quad +(z_R(m)-z_L(m))^\mathsf T U_m
       +w_R(m)^\mathsf T L_R(m)-w_L(m)^\mathsf T L_L(m).
\end{split}
\]
If the exact candidate values \(w_L(m)=w_R(m)=q\), continuity
of \(U,J\) gives \(L_R-L_L=(P_L-P_R)U_m\). The match
coefficient is then \(z_R-z_L+(P_L-P_R)^\mathsf T q\).
The implemented left and far signs, common-match correction,
and plus-defect integral match this identity. The right z
initial datum is exactly zero in binary64.

## Candidate endpoint gap and runtime status

The initial stored run stopped because launch was unbound
on the right branch. I reported that immediately. The current
scratch file sets launch to exact zero on that branch, and
the next observed output reached the common match correction.

The audited candidate defect still has one formal precision gap.
z0 and q are constructed by signed_projection.midpoint as
high-precision Acb point midpoints. directed_vector then converts
these to binary64 complex values before constructing the candidate
polynomials. The exact candidates used in z_L(c_0) and the common
w match are the latter values. The collar mismatch currently uses
the former z0, and the match residual the former q. The tiny
rounding difference must be included. Use the exact binary
candidate initial values in these two expressions, or round z0 and q
to those binary values before starting the flows and confirm the
candidate first node. The returned z-match and w endpoint boxes
already contain the polynomial endpoints they center on, so those
do not have this omission. This is an endpoint bookkeeping gap,
not a sign discrepancy or expected macroscopic shift.

The imported match ball of radius 0.33257 has an explicit prior
refined run with radius 0.3325617325626072. It applies to the
unchanged physical preparation, not arbitrary input_builder
choices. The hard-coded center is the displayed binary
center and the rounded-up radius has ample slack.

## Norm caps and quadrature

For complex bilinear pairings the ordinary absolute
Cauchy--Schwarz bound remains valid. The defect cap
\((\|d_z\|_2\|U\|_2+\|d_w\|_2\|L\|_2)/m\) per
subcell uses the cell-parameter defects, which already contain
\(x_t\); no further factor of \(x_t\) belongs there. The
reverse traversal of each stored leg to bound U from the match
uses the numerical abscissa of \(-x_tP\), as required.
The affine candidate and whole-subcell residual error bound
give an interval enclosure for L. The endpoint cap for w_R
uses the absolute far affine bound. These norm reductions
are legitimate after the candidate endpoint issue is repaired.

The midpoint quadrature factor is correct. On a subcell of
width \(1/m\), the error of midpoint integration is bounded
by \(\sup|\phi''|/(24m^3)\); the degree-two Acb Taylor
coefficient equals \(\phi''/2\), hence the implemented
\(\sup|v_2|/(12m^3)\). The integrand series include \(x_t\).
For the mean source, the same formula uses \(r_t\).
The midpoint mean interval is obtained from the incoming
value and a bounded derivative through the half subcell.
The interval mean jets use the true full-cell derivative
in local t independent of traversal orientation.
The nonsmooth launch support cells use direct interval
integration. The Taylor treatment therefore uses the
smooth equations only where the launch contact vanishes.

The widened collar cap is
\(0.02539853433122648\ldots <0.034<0.038\).
The derivative Cauchy step uses Q on the larger disk 0.038
to bound Q' on 0.034. The mixed analytic radius 0.043,
mean analytic radius 0.04, and larger scalar horizon
radii cover these disks. The removable divisions were
checked in the earlier horizon audit. The two homogeneous
collar columns solve for the negative of their strong
collar values as the z_c functional, with the driven
column restoring C. GL and baseline amplitude factors
are consistent. No additional sign or midpoint factor
blocker was found in this bounded audit.

## Follow-up on the corrected binary endpoints

I inspected the final scratch source at signed_projection.py SHA-256
7f6122992f68307a82dd6cfdc4bd701d9af1cdc51c2c081d9c9bab4559ff1102 and
directed_vector.py SHA-256
a90243958d8acb9a07bf5b2419c64df6400f0b96c990b71d9ccc7a291b164b4f.
The earlier endpoint gap is closed. Both \(z_0\) and the common \(q\)
now use binary_point, so the dyadic values in the collar and match
corrections are exactly the binary64 inputs to the candidate flows.
The candidate-only branch asserts zero initial displacement relative
to the stored first knot. Each flow stores all knot values once and
uses those values at both adjacent Hermite endpoints and for its
return endpoint. Thus the exact underlying candidate polynomials
share the intended endpoints even if interval evaluation encloses
their coefficients with rounding. This also makes the common \(q\)
in the left and right \(w\) candidates an exact equality.

The completed two-subdivision output in the saved two-subdivision check
gives the numerator rectangle
\([-0.2835218971,1.1142187092]+i[-3.8543392688,-2.4178454137]\)
and the residue rectangle
\([0.02779513896,0.24019498096]+i[-0.47893049648,-0.26283615697]\),
with endpoints rounded outward. The reported residue modulus lower
bound exceeds 0.26430. The independently evaluated degree-42 strong
projection in the saved degree-42 strong projection gives
\(I\approx0.42606837-3.13558498i\) from its rounded components and
\(I/W'\approx0.1352337883-0.3706200777i\). Both values lie in the
corresponding rectangles. This is agreement with a distinct
projection calculation using shared lower-order physical inputs.
The strong benchmark is numerical evidence, and the signed result
uses the directed and spectral enclosures. I did not rerun or refine
either final calculation during this follow-up.

The four-subdivision output in the saved four-subdivision check also
completed with the endpoint assertions satisfied. Its numerator
rectangle is
\([0.0810141127065,0.7658225504191]+i[-3.4867180737302,-2.7847432785302]\).
Its residue rectangle is
\([0.0822349693964,0.1877015112307]+i[-0.4241777344130,-0.3170715511066]\).
Both are nested in the two-subdivision rectangles and contain the
same strong benchmark. The output modulus enclosure is
\([0.32756214481046,0.46385192431423]\) after outward rounding.
