"""Independent sharp baseline and Bondi flux controls. No project imports."""
from platform import python_version

import flint
import numpy as np
import scipy
import sympy as sp
from flint import arb, ctx
from scipy.special import roots_legendre, wrightomega


def packet_jets(y):
    inv = 1 / (y * (1 - y))
    value = (-inv).exp()
    return value, (1 - 2 * y) * inv**2 * value


def radius(u):
    return 1 + 2 * ((-(1 + u) / 2).exp() / 2).lambertw()


def primitive(q):
    return 3 * q - 3 * q**2 / 4


def bounds(label, ball):
    print(f"{label} [{ball.lower().str(12)}, {ball.upper().str(12)}]")


def directed_baseline():
    """Range sums bound first two scatters, alternating inequality bounds rest."""
    ctx.prec = 160
    cells = 8192
    step = arb(1) / cells
    delta = arb(3) / 25
    R = arb(13) / 8
    start = -R - 2 * (R - 1).log()
    endpoint_r = radius(start + delta)
    ray_mass = arb("1.61")
    assert endpoint_r > arb("1.59") and primitive(1 / endpoint_r) < ray_mass
    assert ray_mass * delta < 1

    # b0 <= exp(-cells) and |b0'| <= cells**2 exp(-cells) on end cells.
    flat = (-arb(cells)).exp()
    normalization_sq = arb(0)
    scatter = arb(0)
    prefix = {}
    for i in range(cells):
        y = (i * step).union((i + 1) * step)
        if i in (0, cells - 1):
            b = arb(0).union(flat.upper())
            bp = (-(cells**2) * flat).union(cells**2 * flat)
        else:
            b, bp = packet_jets(y)
        normalization_sq += bp * bp * step
        scatter += primitive(1 / radius(start + delta * y))**2 * b * step
        if i + 1 in (cells // 8, cells):
            prefix[i + 1] = scatter

    normal = normalization_sq.sqrt()
    bounds("bump derivative normalizer", normal)
    for end in (cells // 8, cells):
        y = end * step
        length = delta * y
        r = radius(start + length)
        first = arb(0)
        if end < cells:
            b, bp = packet_jets(y)
            first = (bp - delta * primitive(1 / r) * b) / (delta.sqrt() * normal)
        diagonal = delta**(arb(3) / 2) * prefix[end] / (2 * normal)
        # K(d,v)/K(z,v) lies between exp(-length/4) and exp(length/4).
        a1 = (diagonal * (-length / 4).exp()).union(diagonal * (length / 4).exp())
        # For a_n = A_d T^n B with n >= 1, a_(n+1) <= length*M*a_n.
        g = first + ((1 - ray_mass * length) * a1).union(a1)
        assert g > 0
        label = "early" if end < cells else "packet"
        bounds(label + " a1", a1)
        bounds(label + " news", g)


def quadrature_baseline():
    """Central check of the actual off-diagonal two-scatter kernel."""
    delta, R = 0.12, 13 / 8
    start = -R - 2 * np.log(R - 1)
    root, weight = roots_legendre(96)
    unit = (1 + root) / 2
    weight /= 2

    def bump(y):
        return np.exp(-1 / (y * (1 - y)))

    def slope(y):
        return bump(y) * (1 - 2 * y) / (y * (1 - y))**2

    def inv_tortoise(x):
        return 1 + wrightomega(x - 1)

    def potential(r):
        return 3 * (r - 1) * (2 * r - 1) / (4 * r**4)

    norm = np.sqrt(weight @ slope(unit)**2)
    for fraction in (1 / 8, 1):
        d = start + delta * fraction
        z = start + delta * fraction * unit
        # The Lambert W identity r_sigma=1+2*W(exp(-(1+u)/2)/2)
        # is equivalent to the stable wrightomega evaluation below.
        rz_graph = 1 + 2 * wrightomega(-(1 + z) / 2 - np.log(2))
        q_graph = 1 / rz_graph[:, None]
        q = q_graph * unit[None, :]
        rz = 1 / q
        rd = inv_tortoise(rz + np.log(rz - 1) + (z[:, None] - d) / 2)
        integrand = ((3 - 1.5 * q) * (primitive(q_graph) - primitive(q))
                     * potential(rd) / potential(rz))
        kernel = (integrand @ weight) * q_graph[:, 0]
        a1 = delta**1.5 * fraction * (weight @ (kernel * bump(fraction * unit))) / norm
        print(f"central actual a1, fraction {fraction:g}: {a1:.12g}")


def angular_and_cut_controls():
    x, u = sp.symbols("x u", real=True)
    # x=cos(theta), det(gamma)=1 and Gamma^phi_(x phi)=-x/(1-x*x).
    Y = sp.sqrt(5 / (16 * sp.pi)) * (3 * x*x - 1)
    X_phi = -(1 - x*x) * sp.diff(Y, x)
    X_xphi = sp.diff(X_phi, x) / 2 + x * X_phi / (1 - x*x)
    norm_y = 2 * sp.pi * sp.integrate(Y**2, (x, -1, 1))
    norm_x = 2 * sp.pi * sp.integrate(sp.cancel(X_phi**2 / (1 - x*x)), (x, -1, 1))
    norm_xab = 2 * sp.pi * sp.integrate(2 * X_xphi**2, (x, -1, 1))
    assert (norm_y, norm_x, norm_xab) == (1, 6, 12)
    print(f"exact harmonic squared norms: {norm_y}, {norm_x}, {norm_xab}")

    n01, n21, xi = 1 + u, u*u - 2, u * (1 + u)
    fixed = sp.integrate(n01*n21, (u, 0, 1))
    flux_change = xi*n01*sp.diff(n01, u) + sp.diff(xi, u)*n01*n01/2
    boundary = sp.integrate(flux_change, (u, 0, 1))
    assert boundary == (xi*n01*n01/2).subs(u, 1) - (xi*n01*n01/2).subs(u, 0)
    print(f"clock fixture original {fixed}, held labels {fixed+boundary}, "
          f"moving physical endpoints {fixed+boundary-boundary}")


if __name__ == "__main__":
    print(f"Python {python_version()}, flint {flint.__version__}, "
          f"NumPy {np.__version__}, SciPy {scipy.__version__}, SymPy {sp.__version__}")
    directed_baseline()
    quadrature_baseline()
    angular_and_cut_controls()
