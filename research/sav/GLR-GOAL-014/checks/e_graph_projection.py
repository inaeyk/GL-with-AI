"""Independent floating-point calculation of the complete graph-only summand.

Shares the pinned GL master ODE and EF reconstruction equations, while using
its own central-radius shooting, density contractions and null march. This
bounded non-production check does not provide directed GL or quadrature errors.
The fixed-mass mean shares the historical empirical GL matching and Ricci
quadrature.
"""

from pathlib import Path
from platform import python_version
import sys

import numpy as np
import scipy
from scipy.integrate import cumulative_trapezoid, quad, solve_ivp, trapezoid
from scipy.special import lambertw, wrightomega


K = np.pi / 5
OMEGA = 0.06328119
DELTA = 3 / 25
FRONT = -13 / 8 - 2 * np.log(5 / 8)
S = np.diag([-1, 1])


def bump(y):
    return np.exp(-1 / (y * (1 - y))) if 0 < y < 1 else 0.


def slope(y):
    return bump(y) * (1 - 2 * y) / (y * (1 - y))**2 if 0 < y < 1 else 0.


def gl_state(r):
    def rhs(rad, state):
        f = (rad - 1) / rad
        d = 1 + K**2 * rad**3
        c = K**2 + (6 * rad - 11 + 18 * (rad - 1) / d**2
                    - 6 * (4 * rad - 5) / d) / rad**3
        return (state[1], (c * state[0] - (1 / rad**2 + 2 * OMEGA) * state[1]) / f)

    horizon_slope = (K**2 - 5 + 6 / (1 + K**2)) / (1 + 2 * OMEGA)
    offset = 1e-6
    solution = solve_ivp(rhs, (1 + offset, 2), (1 + offset * horizon_slope, horizon_slope),
                         method='DOP853', rtol=2e-12, atol=2e-14, dense_output=True)
    assert solution.success
    return solution.sol(r)


def gl_ef(r, n, nr):
    a = K**2
    f = (r - 1) / r
    d = 1 + a * r**3
    y = d / r * n
    yr = (2 * a * r - 1 / r**2) * n + d / r * nr
    htx = -OMEGA * (f * yr + (OMEGA - 1 / (2 * r**2)) * y) / (a * d)
    hvr = ((3 * OMEGA * r**2 - (1 - 2 * a * r**3) / 2) / d**2 * y
           - (1 / (2 * a * d * r**2) - 3 * r * (r - 1) / d**2
              + OMEGA / (a * d)) * yr)
    hvv = htx - f * hvr
    angular = -(r**2 * y / 2 + ((r - 1) * yr + r * OMEGA * y) / a) / d
    hrr = -2 * (hvr + angular / r**2) / f
    return hvv, hvr, hrr, angular


def graph_news(radii, launch, launch_u):
    output = []
    n2, nr2 = gl_state(2.)
    scale = 8 / (np.exp(2 * OMEGA) * gl_ef(2., n2, nr2)[3])
    n_values, nr_values = scale * gl_state(radii)
    for r, b, bu, n, nr in zip(radii, launch, launch_u, n_values, nr_values):
        f = (r - 1) / r
        p = -1 / r
        hvv, hvr, hrr, angular = gl_ef(r, n, nr)
        h = np.array([[hvv, hvv + f * hvr],
                      [hvv + f * hvr, hvv + 2 * f * hvr + f**2 * hrr]])
        x = S @ h / f
        d = angular / (2 * r**2)
        assert abs(np.trace(x) + 4 * d) < 1e-11
        covector = np.array([-bu + f * b / r, bu]) / (2 * r)
        normal = np.array([1, -p])
        axial = np.array([-p, 1])
        m1 = -np.exp(OMEGA * r) * r**2 * x @ S
        m2 = (np.exp(2 * OMEGA * r) * r**2 / 2
              * (x @ x - (np.trace(x @ x) / 4 + 2 * d**2) * np.eye(2)) @ S)
        mixed_t = ((axial @ m1 @ normal) * (normal @ m1 @ covector)
                   / (r**3 * (1 - p**2)**2))
        direct_t = 2 * (axial @ m2 @ covector) / (r * (1 - p**2))
        output.append((1 + p) / 2 * np.array([mixed_t, direct_t]))
    return np.array(output).T


def fixed_mass_graph_news(radii, launch, launch_u, outer_length, ray):
    source_dir = Path(__file__).resolve().parents[2] / 'GLR-GOAL-011/checks/physical_overlap'
    sys.path.insert(0, str(source_dir))
    from overlap_gl import GLProfile
    from overlap_mean import MeanGeometry

    gl = GLProfile(2.5, ray, outer_length, 1e-6, 1e-11, omega=OMEGA)
    mean = MeanGeometry(gl, 2.5, ray, outer_length, 1e-11)
    f = (radii - 1) / radii
    p = -1 / radii
    mean_static = np.array([mean(r) for r in radii]).T
    assert np.max(abs(mean_static.imag)) < 1e-8
    mu, beta = mean_static.real * (radii - 1)**(-2 * OMEGA)
    velocity = (beta - 2 * mu / (radii * f)) * (2 * launch_u - f * launch / radii) / (1 - p)
    return (1 + p) / 2 * velocity, (mu[-1], beta[-1])


def march(cells, outer_length, ray):
    cuts = FRONT + np.linspace(0, DELTA / 8, cells + 1)
    graph = 1 + 2 * lambertw(np.exp(-(1 + cuts) / 2) / 2).real
    outer_fraction = np.linspace(0, 1, 2 * cells + 1)[1:]
    graph_nodes = (graph[::-1] - graph[-1]) / (2 + graph[::-1] - graph[-1])
    compact_v = np.r_[graph_nodes, graph_nodes[-1] + (1 - graph_nodes[-1]) * outer_fraction]
    radii = 1 + wrightomega((graph[-1] + 2 * compact_v[None, :-1] /
                             (1 - compact_v[None, :-1]) - cuts[:, None]) / 2 - 1)
    potential = 3 * (1 - 1 / radii) * (2 * radii - 1) / radii**3 / 4
    kernel = np.c_[2 * potential / (1 - compact_v[None, :-1])**2, np.full(cells + 1, 3.)]
    mask = np.arange(compact_v.size)[None, :] >= (cells - np.arange(cells + 1))[:, None]
    rows = np.arange(cells + 1)

    def ray_integral(field):
        integ = cumulative_trapezoid(kernel * field, compact_v, axis=1, initial=0)
        return np.where(mask, integ - integ[rows, cells - rows, None], 0.)

    def solve(launch, velocity):
        field = launch.copy()
        for _ in range(9):
            field = launch - cumulative_trapezoid(ray_integral(field), cuts, axis=0, initial=0)
        return velocity - ray_integral(field)[:, -1]

    normal = np.sqrt(quad(lambda y: slope(y)**2, 0, 1, epsabs=1e-14)[0])
    b = np.array([bump(y) for y in (cuts - FRONT) / DELTA]) * np.sqrt(DELTA) / normal
    bu = np.array([slope(y) for y in (cuts - FRONT) / DELTA]) / (normal * np.sqrt(DELTA))
    g = solve(np.where(mask, b[:, None], 0.), bu)
    print(f'  cells {cells}, physical N01(d) {g[-1]:.10f}, baseline norm {np.sqrt(trapezoid(g*g,cuts)):.10f}')
    mean_q, mean_end = fixed_mass_graph_news(graph, b, bu, outer_length, ray)
    print(f'    mean outer length {outer_length}, ray {ray}, graph-end physical mu,beta {mean_end}')
    total = 0.
    graph_pieces = [*graph_news(graph, b, bu), mean_q]
    for label, q in zip(('mixed', 'direct', 'fixed-mass mean'), graph_pieces):
        primitive = cumulative_trapezoid(q, cuts, initial=0)
        graph_launch = np.where(mask, primitive[:, None] - np.r_[primitive[::-1],
                                np.zeros(compact_v.size - primitive.size)][None, :], 0.)
        returned = solve(graph_launch, q)
        projection = trapezoid(g * returned, cuts)
        total += projection
        print(f'    {label} graph-only projection {projection:+.12e}')
    print(f'    total graph-only projection {total:+.12e}')


if __name__ == '__main__':
    print(f'Python {python_version()}, NumPy {np.__version__}, SciPy {scipy.__version__}')
    for grid, length, angle in ((96, 24, 0), (192, 36, 0.25), (384, 48, 0.35)):
        march(grid, length, np.exp(1j * angle))
