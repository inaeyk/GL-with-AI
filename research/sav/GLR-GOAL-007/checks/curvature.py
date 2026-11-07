"""Exact non-production coordinate-jet checks of two-parameter 5D Einstein terms."""

from itertools import product
import platform

import sympy as sp


DIM = range(5)
ORDERS = sorted(product(range(3), range(2)), key=sum)
ZERO = (0, 0)
SPLITS = {
    order: [(p, (order[0] - p[0], order[1] - p[1])) for p in ORDERS
            if p[0] <= order[0] and p[1] <= order[1]]
    for order in ORDERS
}
MATRIX_ZERO = sp.zeros(5)
t, r, x, phi, z = sp.symbols("t r x phi z", real=True)
COORDINATES = (t, r, x, phi, z)
POINT = {t: 0, r: 2, x: sp.Rational(1, 3), phi: 0,
         sp.cos(z): sp.Rational(3, 5), sp.sin(z): sp.Rational(4, 5)}
f = 1 - 1 / r
BASE = sp.diag(-f, 1 / f, r**2 / (1 - x**2), r**2 * (1 - x**2), 1)


def value(expr):
    return sp.cancel(expr.xreplace(POINT))


def partial_matrix(metric, *axes):
    return metric.applyfunc(lambda entry: value(sp.diff(entry, *axes)))


def metric_jets(terms):
    return {
        order: (metric.applyfunc(value),
                [partial_matrix(metric, a) for a in COORDINATES],
                [[partial_matrix(metric, a, b) for b in COORDINATES]
                 for a in COORDINATES])
        for order, metric in terms.items()
    }


def einstein(jets):
    """Return coefficients of G_ab from its coordinate Christoffel formula."""
    null_jet = (MATRIX_ZERO, [MATRIX_ZERO] * 5, [[MATRIX_ZERO] * 5] * 5)
    g, dg, ddg = ({order: jets.get(order, null_jet)[i] for order in ORDERS}
                  for i in range(3))
    inverse = {ZERO: g[ZERO].inv()}
    for order in ORDERS[1:]:
        inverse[order] = -inverse[ZERO] * sum(
            (g[p] * inverse[q] for p, q in SPLITS[order] if p != ZERO),
            MATRIX_ZERO,
        )
    d_inverse = {}
    for order in ORDERS:
        d_inverse[order] = [
            -sum((inverse[p] * dg[s][a] * inverse[b]
                  for p, q in SPLITS[order] for s, b in SPLITS[q]), MATRIX_ZERO)
            for a in DIM
        ]

    def lower(order, a, b, c):
        return (dg[order][b][a, c] + dg[order][c][a, b] - dg[order][a][b, c]) / 2

    def d_lower(order, e, a, b, c):
        return (ddg[order][e][b][a, c] + ddg[order][e][c][a, b]
                - ddg[order][e][a][b, c]) / 2

    gamma = {
        order: {(a, b, c): sum(inverse[p][a, d] * lower(q, d, b, c)
                                for p, q in SPLITS[order] for d in DIM)
                for a, b, c in product(DIM, repeat=3)}
        for order in ORDERS
    }

    def d_gamma(order, e, a, b, c):
        return sum(d_inverse[p][e][a, d] * lower(q, d, b, c)
                   + inverse[p][a, d] * d_lower(q, e, d, b, c)
                   for p, q in SPLITS[order] for d in DIM)

    def ricci_entry(order, b, c):
        derivative = sum(d_gamma(order, a, a, b, c) - d_gamma(order, c, a, b, a)
                         for a in DIM)
        product_term = sum(gamma[p][a, a, d] * gamma[q][d, b, c]
                           - gamma[p][a, c, d] * gamma[q][d, b, a]
                           for p, q in SPLITS[order] for a, d in product(DIM, repeat=2))
        return derivative + product_term

    ricci = {order: sp.Matrix(5, 5, lambda b, c: ricci_entry(order, b, c))
             for order in ORDERS}
    trace = {order: sum(sp.trace(inverse[p] * ricci[q]) for p, q in SPLITS[order])
             for order in ORDERS}
    return {order: ricci[order] - sum((g[p] * trace[q] / 2 for p, q in SPLITS[order]),
                                     MATRIX_ZERO)
            for order in ORDERS}


def coefficient(expression, e, j, order):
    """Differentiation at zero, including both Taylor factorials."""
    i, k = order
    return expression.diff(e, i).subs(e, 0).diff(j, k).subs(j, 0) / (
        sp.factorial(i) * sp.factorial(k))


def warp_reference(u, e, j):
    """Direct unexpanded lapse-warp formula, using W^2=f(1+u)."""
    coords = COORDINATES[1:]
    b = BASE[1:, 1:]
    inv = b.inv()
    lapse_squared = f * (1 + u)

    def connection(a, i, k):
        return sum(inv[a, c] * (b[c, k].diff(coords[i]) + b[c, i].diff(coords[k])
                                 - b[i, k].diff(coords[c])) / 2 for c in range(4))

    def lapse_term(i, k):
        di = lapse_squared.diff(coords[i])
        dk = lapse_squared.diff(coords[k])
        hess = di.diff(coords[k]) - sum(connection(a, i, k) * lapse_squared.diff(coords[a])
                                       for a in range(4))
        return value(hess) / (2 * value(lapse_squared)) - value(di * dk) / (
            4 * value(lapse_squared)**2)

    q = sp.Matrix(4, 4, lapse_term)
    dynamic = -q + b.applyfunc(value) * sp.trace(inv.applyfunc(value) * q)
    # The base Einstein tensor cancels at nonzero parameter degree. The tt
    # component is proportional to R[b]=0 on the Schwarzschild time slice.
    dynamic5 = sp.zeros(5)
    dynamic5[1:, 1:] = dynamic
    return {order: dynamic5.applyfunc(lambda expr: coefficient(expr, e, j, order))
            for order in ORDERS if order != ZERO}


def warped_checks():
    e, j = sp.symbols("e j")
    p2 = (3 * x**2 - 1) / 2
    h = (1 + r) * sp.cos(z) / r
    p = (2 + r) * p2 / r
    q = (1 + 2 * r) * sp.cos(z)**2 / r
    v = (3 - r) * p2 * sp.cos(z) / r
    w = (r - 1) * p2 * sp.cos(z)**2 / r
    fields = {(1, 0): h, (0, 1): p, (2, 0): q, (1, 1): v, (2, 1): w}
    jets = metric_jets({ZERO: BASE, **{order: sp.diag(-f * field, 0, 0, 0, 0)
                                       for order, field in fields.items()}})
    full = einstein(jets)
    u = sum(e**i * j**k * field for (i, k), field in fields.items())
    reference = warp_reference(u, e, j)
    assert full[ZERO] == MATRIX_ZERO
    assert all(full[order] == expected for order, expected in reference.items())

    def subset(*orders):
        return einstein({order: jets[order] for order in (ZERO, *orders)})

    h_order, p_order, q_order, v_order, w_order = fields
    cubic = subset(h_order, p_order)[w_order]
    mix_hp = subset(h_order, p_order)[v_order]
    mix_hv = subset(h_order, v_order)[w_order]
    mix_pq = subset(p_order, q_order)[w_order]
    linear_v = subset(v_order)[v_order]
    linear_w = subset(w_order)[w_order]
    assert full[v_order] == linear_v + mix_hp
    assert full[w_order] == linear_w + mix_hv + mix_pq + cubic
    for piece in (mix_hp, mix_hv, mix_pq, cubic, linear_v, linear_w):
        assert piece != MATRIX_ZERO
    wrong = linear_w + mix_hv + mix_pq + 2 * cubic
    assert wrong != reference[w_order]
    print("Warp reference matches all 125 nonbackground components")
    print("Mixed, cubic G_rz:", full[v_order][1, 4], full[w_order][1, 4])
    print("Cubic G_rz pieces (linear, hv, pq, hhp):",
          *(piece[1, 4] for piece in (linear_w, mix_hv, mix_pq, cubic)))
    print("Wrong cubic-factor G_rz:", wrong[1, 4])


def pure_gauge_checks():
    e, j = sp.symbols("e j")
    p2 = (3 * x**2 - 1) / 2
    shifted = r + e * (1 + r) * sp.cos(z) + j * (r - 1) * p2
    radial = sp.Matrix([shifted.diff(axis) for axis in COORDINATES])
    shifted_f = 1 - 1 / shifted
    exact = sp.diag(-shifted_f, 0, shifted**2 / (1 - x**2), shifted**2 * (1 - x**2), 1)
    exact += radial * radial.T / shifted_f
    terms = {order: exact.applyfunc(lambda expr: coefficient(expr, e, j, order))
             for order in ORDERS}
    jets = metric_jets(terms)
    complete = einstein(jets)
    assert all(ein == MATRIX_ZERO for ein in complete.values())
    cubic_order = (2, 1)
    incomplete = einstein({order: jet for order, jet in jets.items() if order != cubic_order})
    assert all(ein == MATRIX_ZERO for order, ein in incomplete.items() if order != cubic_order)
    assert incomplete[cubic_order] != MATRIX_ZERO
    print("Curved finite diffeomorphism cancels all retained Einstein coefficients")
    print("Omitted cubic gauge completion gives G_rz =", incomplete[cubic_order][1, 4])


def flat_bianchi_checks():
    minkowski = sp.diag(-1, 1, 1, 1, 1)
    p = sp.Matrix([3, 5, 0, 0, 4 * sp.I])
    q = sp.Matrix([2, 0, 2, 0, 0])
    h = sp.diag(0, 0, 1, -1, 0)
    v = sp.diag(0, 1, 0, -1, 0)

    def mode_jet(covector, polarization):
        # All entries multiply exp(covector . X), evaluated at X=0.
        return (polarization, [covector[a] * polarization for a in DIM],
                [[covector[a] * covector[b] * polarization for b in DIM] for a in DIM])

    for covector, polarization in ((p, h), (q, v)):
        assert (covector.T * minkowski * covector)[0] == 0
        assert covector.T * minkowski * polarization == sp.zeros(1, 5)
        assert sp.trace(minkowski * polarization) == 0

    jets = {ZERO: mode_jet(sp.zeros(5, 1), minkowski),
            (1, 0): mode_jet(p, h), (0, 1): mode_jet(q, v)}
    exact = einstein(jets)
    assert exact[ZERO] == exact[1, 0] == exact[0, 1] == MATRIX_ZERO
    mix = exact[1, 1]
    assert mix != MATRIX_ZERO
    assert (p + q).T * minkowski * mix == sp.zeros(1, 5)

    # Break the massless wave equation without changing its polarization.
    wrong_q = sp.Matrix([3, 0, 2, 0, 0])
    wrong = einstein({**jets, (0, 1): mode_jet(wrong_q, v)})
    assert wrong[0, 1] != MATRIX_ZERO
    violation = (p + wrong_q).T * minkowski * wrong[1, 1]
    assert violation != sp.zeros(1, 5)
    print("Nonzero on-shell flat mixed source has zero background divergence")
    print("Off-shell flat mixed divergence:", list(violation))


def axial_reduction_check():
    # Odd axisymmetric perturbation, g_a_phi = rho² P'_2 A_a.
    orbit_indices = (0, 1, 4)
    orbit_coords = (t, r, z)
    orbit = sp.diag(-f, 1 / f, 1)
    radius2 = r**2 * (1 - x*x)
    y2 = (3 * x*x - 1) / 2
    covector = sp.Matrix([(t + r) * sp.cos(z), t * r**2 * sp.cos(z),
                         (t - r) * sp.sin(z)])
    axial = sp.zeros(5)
    for a, field in zip(orbit_indices, covector):
        axial[a, 3] = axial[3, a] = radius2 * y2.diff(x) * field
    direct = einstein(metric_jets({ZERO: BASE, (0, 1): axial}))[0, 1]

    strength = sp.Matrix(3, 3, lambda a, b: covector[b].diff(orbit_coords[a])
                         - covector[a].diff(orbit_coords[b]))
    upper = orbit.inv() * strength * orbit.inv()
    contra = orbit.inv() * covector
    # det(g_orbit)=-1 here, so the divergence of an antisymmetric tensor
    # is its ordinary coordinate divergence.
    proca = sp.Matrix([sum((r**4 * upper[b, a]).diff(orbit_coords[b]) for b in range(3))
                       - 4 * r**2 * contra[a] for a in range(3)])
    reference = -(1 - x*x) * y2.diff(x) * orbit * proca / (2 * r**2)
    assert all(direct[a, 3] == value(expected)
               for a, expected in zip(orbit_indices, reference))
    constraint = sum((r**2 * contra[a]).diff(orbit_coords[a]) for a in range(3))
    assert direct[2, 3] == value((1 - x*x) * y2.diff(x, 2) * constraint / 2) != 0
    wrong = reference - (1 - x*x) * y2.diff(x) * covector * 2
    assert any(direct[a, 3] != value(expected)
               for a, expected in zip(orbit_indices, wrong))
    print("Axial Proca and divergence constraint match direct 5D Einstein components")
    print("Nonzero manufactured axial G_xphi:", direct[2, 3])


if __name__ == "__main__":
    print("Python", platform.python_version(), "SymPy", sp.__version__)
    warped_checks()
    pure_gauge_checks()
    flat_bianchi_checks()
    axial_reduction_check()
