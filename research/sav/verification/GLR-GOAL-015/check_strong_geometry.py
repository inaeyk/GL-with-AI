"""Independent early-cap geometry and contact checks. No source imports."""
from flint import arb, ctx
import sympy as sp


def check_nonnegative(expression, variable, lower):
    """Sufficient certificate via coefficients in (variable - lower)."""
    z = sp.Symbol("z", nonnegative=True)
    numerator, denominator = sp.cancel(expression).as_numer_denom()
    for part in (numerator, denominator):
        coefficients = sp.Poly(part.subs(variable, lower + z), z).all_coeffs()
        assert min(coefficients) >= 0 and max(coefficients) > 0
    assert denominator.subs(variable, lower) > 0


def graph_and_potential():
    r = sp.Symbol("r", positive=True)
    f = 1 - 1 / r
    p = -1 / r
    b_prime = -(1 + p) / (1 - p)
    V = f * (6 / r**2 - 3 / r**3)
    zeta = V / (2 * (1 - p))
    xi = V / (2 * (1 + p))
    log_x = sp.cancel(f * sp.diff(V, r) / V)
    lower = sp.Rational(81, 50)
    for quantity in (
        1 - V, sp.Rational(1, 2) - log_x, sp.Rational(1, 2) + log_x,
        1 - zeta, 1 - b_prime * sp.diff(zeta, r), 1 + b_prime * sp.diff(zeta, r),
        2 - xi, 2 - b_prime * sp.diff(xi, r), 2 + b_prime * sp.diff(xi, r),
    ):
        check_nonnegative(quantity, r, lower)
    start = sp.Rational(13, 8)
    assert -b_prime.subs(r, start) < sp.Rational(6, 25)
    assert -1 / b_prime.subs(r, lower) < 5

    def front(radius):
        return -radius - 2 * (radius - 1).log()

    start = arb(13) / 8
    width = arb(3) / 200
    end = front(start) + width
    left, right = arb("1.62143634172470"), arb("1.62143634172471")
    assert front(left) > end > front(right)
    graph_r = left.union(right)

    def J(radius):
        return 3 / radius - 3 / (4 * radius**2)

    assert J(graph_r) < arb("1.57")
    m = (width / 4).exp() * (J(graph_r) - J(arb(400)))
    assert m < arb("1.563294")
    print("tilted graph and scalar inequalities passed")
    print("b(d) enclosure:", graph_r.str(19))
    print("early compact potential bound:", m.str(19))


def check_contacts():
    u, v = sp.symbols("u v", real=True)
    b = 1 - u / 2
    c = 3
    V = 1 + u + v
    F = u**2 * (1 + v**2)
    G = u**2 * (v + 2)
    W = u**2 * (v - b)**2 * (1 + v)
    W += (v - b) * (G - sp.diff(b, u) * F).subs(v, b) / (-4 * sp.diff(b, u))
    H = sp.diff(F, u) + sp.diff(G, v) - V * W - 4 * sp.diff(W, u, v)
    assert sp.expand((4 * sp.diff(W, u) - G + sp.diff(b, u) * F).subs(v, b)) == 0
    assert W.subs(u, 0) == 0
    cap = (G - F).subs(v, c)
    graph = (1 - sp.diff(b, u)) * F.subs(v, b)
    ray = sp.integrate(sp.diff(F, u) + sp.diff(F, v) - H - V * W, (v, b, c))
    assert sp.expand(4 * sp.diff(W, u).subs(v, c) - cap - graph - ray) == 0
    assert cap.subs(u, 1) != 0 and graph.subs(u, 1) != 0

    # Weak omission control, on a different graph with V=0.
    b = 1 - u
    F = 2 * u**2 * (v - b)
    test = u
    primitive = sp.integrate(F, (v, b, 2))
    terminal = (test * primitive).subs(u, 1)
    bulk = -sp.integrate(sp.diff(test, u) * primitive, (u, 0, 1))
    assert terminal == 4 and bulk == -sp.Rational(31, 30)
    assert terminal + bulk == sp.Rational(89, 30)
    print("manufactured strong contacts passed; weak total and omission:", terminal + bulk, bulk)


if __name__ == "__main__":
    ctx.prec = 160
    graph_and_potential()
    check_contacts()
