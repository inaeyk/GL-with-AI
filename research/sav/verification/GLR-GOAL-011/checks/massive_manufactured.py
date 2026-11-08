"""Manufacture both massive polarizations from unreduced Proca equations."""
import numpy as np
from sympy import I, Rational, diff, lambdify, symbols
from overlap_mixed import MixedSolution

r = symbols("r", positive=True)
s = -Rational(241, 1000) - I*Rational(747, 1000)
k = Rational(628, 1000)
lam = 4
f = 1 - 1/r
def dx(field):
    return f * diff(field, r)

lo, hi = 1.42, 2.18
rho = (2*r - Rational(36, 10)) / Rational(76, 100)
profile = (1 - rho**2)**8
ax = (1 + rho/4) * profile/r**4
az = (1 - rho/7) * profile/(4*r**3)
x_tx = profile/(6*r)
# Obtain A_t from the unprojected z equation E^z=0.
at = (r**4 * s**2 * az - dx(r**4*(dx(az) - I*k*ax)) + lam*f*r**2*az)/(I*k*s*r**4)
f_tx = s*ax - dx(at)
p_tx = -r**4*f_tx/f + x_tx
p_tz = -r**4*(s*az - I*k*at)
p_xz = r**4*(dx(az) - I*k*ax)
y_t = -dx(p_tx) - I*k*p_tz + lam*r**2*at
y_x = s*p_tx - I*k*p_xz - lam*r**2*ax

packed = lambdify(r, (x_tx, y_t, y_x, at, ax, f_tx, az), cse=True)
def fields(radius):
    if complex(radius).imag or not lo < radius.real < hi:
        return (0j,) * 7
    return packed(radius.real)

assert abs(complex(fields(1.8)[6])) > 0.04
ray = np.exp(1j*np.pi/4)
for tol in (2e-10, 2e-12):
    sol = MixedSolution(lambda radius: fields(radius)[:3], complex(s), float(k),
                        length=48, ray=ray, left_delta=1e-5, tol=tol)
    sample = np.linspace(1.21, 4, 201)
    errors = np.max([np.abs(np.array(sol(x)) - fields(x)[3:]) for x in sample], axis=0)
    print(f"tol={tol:g}, max errors in A_t A_x F_tx A_z: " +
          " ".join(f"{err:.4e}" for err in errors))
    assert max(errors) < 2000*tol
