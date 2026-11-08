# GOAL-011 exterior GL metric jets

The reconstruction uses the GOAL-005
[61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708](https://github.com/inaeyk/GL-with-AI/commit/61c7b5c2efd1ef1ad1ef545d9853949e6fe3d708)
linear master and metric reconstruction (physics/gl_linear_mode.md,
“Scalar master and exact metric reconstruction”). Use $r_0=1$,
$f=(r-1)/r$, $x=r+\log(r-1)$, $v=t+x$, and the compact-unitary
growing tensor with unhalved cosine amplitude. The static master
is $u$ with $u_{xx}=(V+\Omega^2)u$, $p=u_x/u$.

Set $a=k^2$, $D=1+ar^3$, and $n=e^{-\Omega x}u$, so that
$n_r=(p-\Omega)n/f$. The master equation gives
$$
 n_{rr}=q n+b n_r,\qquad n_{rrr}=q_r n+(q+b_r)n_r+b n_{rr},\qquad
 q=V/f^2,\quad b=-(f_r+2\Omega)/f ,
$$
where
$$
 V=f\left[a+\frac{6r-11}{r^3}
     +\frac{18(r-1)}{r^3D^2}-\frac{6(4r-5)}{r^3D}\right].
$$
These equations supply the derivative of the input master required
for two derivatives of the metric. Define
$$
 Y=Dn/r,\quad
 A=(1/2-ar^3)/D^2,\quad
 B=\frac{1}{2a r^2D}-\frac{3r(r-1)}{D^2},\quad
 T=-\frac{\Omega}{aD}\bigl[fY_r+(\Omega-1/(2r^2))Y\bigr].
$$
Here $T$ is the EF-rescaled static $fH_{tr}$. Factoring
$e^{\Omega v}$, the pinned Schwarzschild-coordinate reconstruction
and the EF pullback give
$$
 \begin{aligned}
 h_{vr}&=(3\Omega r^2/D^2-A)Y-(B+\Omega/(aD))Y_r,\\
 h_{vv}&=T-fh_{vr},\\
 h_{\theta\theta}&=-r^2Y/(2D)-(r-1)Y_r/(aD)-\Omega rY/(aD),\\
 h_{rr}&=-2(h_{vr}+h_{\theta\theta}/r^2)/f .
 \end{aligned}
$$
For example the pinned $h_{vv}$ before simplification is
$(f A-\Omega^2/(aD)+\Omega B)Y+fBY_r$. Subtracting it from $T$
exposes a factor $f$ and gives the displayed $h_{vr}$. The last
line follows from the linear TT trace. Product and quotient
derivative rules act on all of these expressions.

The standalone jet(r,k,omega,u,p) returns (h,h_r,h_rr) with each
four-vector ordered (hvv,hvr,hrr,h_theta_theta).
njet(r,k,omega,n,nr) takes the regular EF master and its $r$ derivative
directly and requires neither a logarithm nor a division by $u$.
The module preserves the supplied master normalization.
Use principal $\log(r-1)$ on the right-going ray, with $\Re r>1$.
Exterior enclosure boxes must exclude zeros of $r$, $f$, $D$
and $k$. The formulas are in the regular EF basis. An interval
evaluation at the horizon itself would require the separate regular
series because this exterior expression still divides by $f$.

For a numerical control, load the prior companion overlap_gl.GLProfile
with rmatch=2.5, length=40, left_delta=1e-7 and tol=1e-12. Compare
its jet(r) with this module, taking
$p,\ell=\mathrm{profile.right.sol}(((r-2.5)/{\rm ray}).\Re)$ and
$u=\mathrm{profile.scale}\ \mathrm{profile.n\_match}
e^{\Omega x(2.5)} e^{\ell-\mathrm{profile.log\_match}}$.
On the real ray at $r=2.5,3.1,7,20$ and the angle-0.71 ray at
distances 1.3 and 16, the largest error in any value or derivative
scaled by $1+|\mathrm{reference}|$ was $8.58\times10^{-13}$ at
the match point, where the reference uses the left profile and
the test uses the right profile. Away from the match point the
maximum was $2.40\times10^{-15}$. At the same six points the
midpoints of 128-bit python-flint 0.9.0 ACB evaluations agreed with
the ordinary numeric results to $2.32\times10^{-15}$ scaled.
This checks the reconstruction on empirical GL data. A rigorous
physical result also requires validated balls for the input master,
growth exponent and parameters, with denominators and branch
conditions verified on the integration boxes.
