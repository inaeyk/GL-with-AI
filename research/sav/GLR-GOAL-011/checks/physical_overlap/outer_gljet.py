"""GOAL-011 exterior GL metric jets in the ingoing EF basis, r0=1.

Public jet(r,k,omega,u,p) returns (h,h_r,h_rr), each with component
order (hvv,hvr,hrr,h_theta_theta) and factored time dependence
exp(omega*v). The static master u obeys u_xx=(V+omega**2)u and
p=u_x/u. The input normalization is preserved. Scalar arithmetic
supports ordinary real/complex values and python-flint acb.
"""
import cmath
import math
from dataclasses import dataclass


@dataclass(frozen=True)
class _Jet:
    """Value and first two unscaled radial derivatives."""

    value: object
    first: object = 0
    second: object = 0

    @staticmethod
    def wrap(value):
        return value if isinstance(value, _Jet) else _Jet(value)

    def __add__(self, other):
        other = self.wrap(other)
        return _Jet(self.value + other.value, self.first + other.first,
                    self.second + other.second)

    __radd__ = __add__

    def __neg__(self):
        return _Jet(-self.value, -self.first, -self.second)

    def __sub__(self, other):
        return self + (-self.wrap(other))

    def __rsub__(self, other):
        return self.wrap(other) + (-self)

    def __mul__(self, other):
        other = self.wrap(other)
        return _Jet(
            self.value * other.value,
            self.first * other.value + self.value * other.first,
            self.second * other.value + 2 * self.first * other.first
            + self.value * other.second,
        )

    __rmul__ = __mul__

    def __truediv__(self, other):
        other = self.wrap(other)
        y = self.value / other.value
        dy = (self.first - y * other.first) / other.value
        ddy = (self.second - 2 * dy * other.first - y * other.second) / other.value
        return _Jet(y, dy, ddy)

    def __rtruediv__(self, other):
        return self.wrap(other) / self


def _scalar_log(x):
    if hasattr(x, "log"):
        return x.log()
    return cmath.log(x) if x.imag else math.log(x.real)


def _scalar_exp(x):
    if hasattr(x, "exp"):
        return x.exp()
    return cmath.exp(x) if x.imag else math.exp(x.real)


def njet(r, k, omega, n, nr):
    """Compute (h,h_r,h_rr) from n=exp(-omega*x)*u and its r derivative."""
    a = k**2
    radial = _Jet(r, 1)
    inverse = 1 / radial
    inverse2 = inverse * inverse
    r2 = radial * radial
    r3 = r2 * radial
    f = 1 - inverse
    D = 1 + r3 * a
    D2 = D * D

    # n_rr = q*n + b*n_r, obtained by differentiating the static GL
    # master equation in r after factoring exp(omega*x).
    q = ((6 * radial - 11) / r3 + a
         + 18 * (radial - 1) / (r3 * D2)
         - 6 * (4 * radial - 5) / (r3 * D)) / f
    b = -(inverse2 + 2 * omega) / f
    nrr = q.value * n + b.value * nr
    nrrr = q.first * n + (q.value + b.first) * nr + b.value * nrr
    amplitude = _Jet(n, nr, nrr)
    amplitude_r = _Jet(nr, nrr, nrrr)

    # Y=(D/r)n and Y_r=((D/r)_r)n + (D/r)n_r. Carry their jets
    # with the product/quotient rules to differentiate the tensor twice.
    Y = (D * inverse) * amplitude
    Yr = (radial * (2 * a) - inverse2) * amplitude + (D * inverse) * amplitude_r
    A = (1 - r3 * (2 * a)) / (2 * D2)
    B = inverse2 / (2 * D * a) - 3 * radial * (radial - 1) / D2
    ftr = -(f * Yr + Y * omega - Y * inverse2 / 2) * omega / (D * a)

    # Algebraically cancel the removable f factor in (ftr-hvv)/f.
    # The remaining f denominator in hrr is safe on the exterior ray.
    vr = ((r2 * (3 * omega)) / D2 - A) * Y - (B + (1 / (D * a)) * omega) * Yr
    sphere = -(r2 * Y / (2 * D)
               + (radial - 1) * Yr / (D * a)
               + radial * Y * omega / (D * a))
    vv = ftr - f * vr
    rr = -2 * (vr + sphere * inverse2) / f

    metric = (vv, vr, rr, sphere)
    return (tuple(j.value for j in metric),
            tuple(j.first for j in metric),
            tuple(j.second for j in metric))


def jet(r, k, omega, u, p):
    """Reconstruct EF radial jets from the exterior static master (u,p)."""
    x = r + _scalar_log(r - 1)
    n = _scalar_exp(-omega * x) * u
    nr = (p - omega) * n / (1 - 1 / r)
    return njet(r, k, omega, n, nr)
