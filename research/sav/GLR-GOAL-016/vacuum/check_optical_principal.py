"""Check terminal-power optical principal symbols by direct warped Ricci."""

from itertools import product

import sympy as sp


R = sp.Symbol("R", positive=True)
k = sp.Symbol("k", real=True)
INDICES = [(0, 0), (1, 0), (0, 1), (1, 1)]
AXES = range(3)
S = sp.Rational


class Jet:
    """Biaffine jet in alpha and eta, with exp(ikz) implicit in eta."""

    def __init__(self, values=0):
        if not isinstance(values, (tuple, list)):
            values = (values, 0, 0, 0)
        self.values = dict(zip(INDICES, map(sp.sympify, values)))

    def __add__(self, other):
        other = other if isinstance(other, Jet) else Jet(other)
        return Jet([self.values[ij] + other.values[ij] for ij in INDICES])

    __radd__ = __add__

    def __neg__(self):
        return Jet([-self.values[ij] for ij in INDICES])

    def __sub__(self, other):
        return self + -other

    def __mul__(self, other):
        other = other if isinstance(other, Jet) else Jet(other)
        values = []
        for h, l in INDICES:
            terms = (
                self.values[i, j] * other.values[h - i, l - j]
                for i in range(h + 1)
                for j in range(l + 1)
            )
            values.append(sp.expand(sum(terms)))
        return Jet(values)

    __rmul__ = __mul__

    def derivative(self, axis):
        if axis == 0:
            return Jet()
        if axis == 1:
            return Jet([sp.diff(self.values[ij], R) for ij in INDICES])
        return Jet([sp.I * k * j * self.values[i, j] for i, j in INDICES])

    def exp(self):
        assert self.values[0, 0] == 0
        return 1 + self + S(1, 2) * self * self

    def coefficient(self, ij, degree):
        return sp.factor(sp.expand(self.values[ij]).coeff(k, degree))

    def vanishes(self):
        return all(sp.simplify(value) == 0 for value in self.values.values())


def curvature(s, include_potential):
    """Return the radial, mixed, angular-potential and scalar combinations."""
    alpha = Jet((0, 1, 0, 0))
    eta = Jet((0, 0, 1, 0))
    q = alpha * R**-1 + eta * R**(-s - 1)
    beta = -S(3, 2) * (s + 1) / (s + 2) * alpha * eta * R**(-s - 2)
    d0 = 3 * (s + 1) / (s * (s - 1))
    d1 = (3 * (s + 3) / (2 * (s + 2)) + 3 * d0) / (s + 1)
    shift = (eta * d0 * R**-s + alpha * eta * d1 * R**(-s - 1)) * (sp.I * k)

    V = Jet(1 - 1 / R)
    if include_potential:
        V += (3 * (s + 2) / (s * (s - 1) * (s - 2))) * k**2 * eta * R**(1 - s)
    B = (beta - q).exp()
    b2 = (2 * q).exp()
    inv_B = (q - beta).exp()
    inv_b2 = (-2 * q).exp()
    zero = Jet()
    g = [[-B * V, -B, -B * shift], [-B, zero, zero], [-B * shift, zero, b2]]
    inv = [
        [zero, -inv_B, zero],
        [-inv_B, inv_B * V + inv_b2 * shift * shift, -inv_b2 * shift],
        [zero, -inv_b2 * shift, inv_b2],
    ]
    for i, j in product(AXES, repeat=2):
        assert (sum(g[i][c] * inv[c][j] for c in AXES) - int(i == j)).vanishes()

    gamma = {}
    for i, m, n in product(AXES, repeat=3):
        gamma[i, m, n] = S(1, 2) * sum(
            inv[i][j] * (
                g[j][n].derivative(m) + g[j][m].derivative(n) - g[m][n].derivative(j)
            )
            for j in AXES
        )
    sphere_radius = R * (-q * S(1, 2)).exp()
    inv_sphere_radius = R**-1 * (q * S(1, 2)).exp()
    hessian = {}
    for m, n in product(AXES, repeat=2):
        hessian[m, n] = sphere_radius.derivative(m).derivative(n) - sum(
            gamma[j, m, n] * sphere_radius.derivative(j) for j in AXES
        )

    def ric(m, n):
        raw = sum(
            gamma[l, m, n].derivative(l) - gamma[l, m, l].derivative(n) for l in AXES
        )
        raw += sum(
            gamma[l, l, j] * gamma[j, m, n] - gamma[l, n, j] * gamma[j, m, l]
            for l, j in product(AXES, repeat=2)
        )
        return raw - 2 * inv_sphere_radius * hessian[m, n]

    rr, rz = ric(1, 1), ric(1, 2)
    nn = inv_b2 * (ric(2, 2) - 2 * shift * rz + shift * shift * rr)
    radius_gradient = sum(
        inv[i][j] * sphere_radius.derivative(i) * sphere_radius.derivative(j)
        for i, j in product(AXES, repeat=2)
    )
    radius_wave = sum(inv[i][j] * hessian[i, j] for i, j in product(AXES, repeat=2))
    angular = Jet(1) - radius_gradient - sphere_radius * radius_wave
    potential = angular + (R**2 / 2) * (-q).exp() * nn
    scalar = -R * (beta - q).exp() * nn
    return rr, rz, potential, scalar


def check():
    for power in (4, 5, 6):
        s = S(power)
        rr, rz, potential, scalar = curvature(s, include_potential=True)
        assert rr.vanishes()
        assert rz.vanishes()
        assert potential.coefficient((0, 1), 2) == 0
        # The sign change converts the k^2 coefficient to a dz^2 coefficient.
        c0 = -scalar.coefficient((0, 1), 2) * R**s
        c1 = -scalar.coefficient((1, 1), 2) * R**(s + 1)
        assert c0 == 1 - 6 / (s * (s - 1))
        assert c1 == -3 - 12 * (s**2 + 2) / ((s - 2) * (s - 1) * (s + 1) * (s + 2))

        _, _, bad_potential, bad_scalar = curvature(s, include_potential=False)
        assert bad_potential.coefficient((0, 1), 2) != 0
        bad_c1 = -bad_scalar.coefficient((1, 1), 2) * R**(s + 1)
        assert sp.factor(c1 - bad_c1) == -3 * (s + 2) / (s * (s - 2))
        print(f"s={s}, c0={c0}, c1={c1}, c1 omitting potential={bad_c1}")

    s = sp.Symbol("s", real=True)
    rr, rz, potential, scalar = curvature(s, include_potential=True)
    assert rr.vanishes()
    assert rz.vanishes()
    assert potential.coefficient((0, 1), 2) == 0
    c0 = sp.factor(-scalar.coefficient((0, 1), 2) * R**s)
    c1 = sp.factor(-scalar.coefficient((1, 1), 2) * R**(s + 1))
    assert sp.factor(c0 - (1 - 6 / (s * (s - 1)))) == 0
    assert sp.factor(c1 + 3 + 12 * (s**2 + 2) / ((s - 2)*(s - 1)*(s + 1)*(s + 2))) == 0
    print(f"symbolic powers: c0={c0}, c1={c1}")

    def primitive(x):
        return 3 / (x - 1) - 3 / x + 3 / (x + 1) - 3 / (x + 2)

    kernel = -12 * (s**2 + 2) / ((s - 2) * (s - 1) * (s + 1) * (s + 2))
    assert sp.factor(s * (primitive(s) - primitive(s - 1)) - kernel) == 0
    print("first rational correction identity exact")


if __name__ == "__main__":
    check()
