"""Exact graph, interface and skew-energy controls, and directed constants."""

from math import factorial

import sympy as s
from flint import arb, ctx


def exact_identity(value):
    assert s.expand(value) == 0


def graph_and_projection():
    u, v, z, q = s.symbols("u v z q", real=True)
    graph = 1-u/2
    field = u**2*(v-graph)
    potential = 3
    f_current = 8*u**2*v
    g_current = 2*u**2 - f_current.subs(v, graph)/2 + u**2*(v-graph)
    h_current = -4*field.diff(u, v)-potential*field+f_current.diff(u)+g_current.diff(v)
    contact = field.diff(u).subs(v, graph)
    exact_identity(4*contact-(g_current+f_current/2).subs(v, graph))
    assert contact != 0

    primitive = s.integrate(f_current, (v, graph, q))
    volume = s.integrate(h_current+potential*field, (v, graph, q))
    weak_news = (g_current.subs(v, q)+primitive.diff(u)-volume)/4
    exact_identity(weak_news-field.diff(u).subs(v, q))
    no_graph = s.integrate(f_current.diff(u)+g_current.diff(v)-h_current-potential*field,
                           (v, graph, q))/4
    exact_identity(weak_news-no_graph-contact)

    cap = s.Integer(2)
    test = u*(1+u)
    flux = s.integrate(test*weak_news.subs(q, cap), (u, 0, 1))
    by_parts_bulk = s.integrate(
        (-test.diff(u)*primitive+test*(g_current.subs(v, q)-volume)).subs(q, cap),
        (u, 0, 1))/4
    endpoint = (test*primitive).subs({u: 1, q: cap})/4
    exact_identity(flux-by_parts_bulk-endpoint)
    assert endpoint != 0

    left = s.integrate(g_current.diff(v), (v, graph, z))
    right = s.integrate(g_current.diff(v), (v, z, cap))
    exact_identity(left+right-g_current.subs(v, cap)+g_current.subs(v, graph))
    missing_face = g_current.subs(v, z).subs({u: 1, z: s.Rational(3, 2)})
    assert missing_face != 0
    print("canonical graph news at u=1", contact.subs(u, 1))
    print("exact weighted cap pairing", flux, "latest-cut term", endpoint)
    print("dropped one-sided internal interface value", missing_face)


def mixed_energy():
    t, x = s.symbols("t x", real=True)
    u, v = t-x, t+x
    graph = 1-u/2
    field = s.Matrix([u**2*(v-graph), u*(v-graph)**2])
    connection = s.diag(1, -2)
    potential = 3*s.eye(2)
    coupling = s.Matrix([[0, -1], [1, 0]])
    time = field.diff(t)
    spatial = field.diff(x)+connection*field
    source = (-time.diff(t)+field.diff(x, 2)
              -(potential+connection**2)*field+coupling*time)
    energy = (time.dot(time)+spatial.dot(spatial)+field.dot(potential*field))/2
    current = -time.dot(spatial)
    exact_identity(energy.diff(t)+current.diff(x)+time.dot(source))
    exact_identity((energy+current)/2-(time-spatial).dot(time-spatial)/4
                   -field.dot(potential*field)/4)
    exact_identity((energy-current)/2-(time+spatial).dot(time+spatial)/4
                   -field.dot(potential*field)/4)
    assert all(not s.expand(component).is_zero for component in coupling*time)
    wrong_coupling = s.Matrix([[0, 1], [1, 0]])
    wrong_work = s.expand(time.dot(wrong_coupling*time)).subs({t: 2, x: 0})
    assert wrong_work != 0
    print("both mixed couplings active, symmetric mutation work", wrong_work)


def directed_constants():
    ctx.prec = 160
    a = -arb(13)/8-2*(arb(5)/8).log()
    d = arb("8.407362")
    d_full = d-a
    r_min = 1+2*((-(1+d)/2).exp()/2).lambertw()
    print("causal front and final graph radius", a.str(16), r_min.str(16))
    for name, width, c_bound in [("packet", arb(3)/25, arb("1.144")),
                                 ("first eighth", arb(3)/200, arb("1.016"))]:
        r_graph = 1+2*((-(1+a+width)/2).exp()/2).lambertw()
        m = 3/r_graph-3/(4*r_graph**2)
        z = m*width
        resolvent = z.exp()-(1-2/arb.pi())*z
        assert m < arb("1.61") and resolvent < c_bound
        print(name, "ray mass", m.str(12), "weak shear factor", resolvent.str(12))
    for r_c in (100, 400, 500):
        mu = d_full*(6/arb(r_c)-3/(2*arb(r_c)**2))/2
        growth = (3*d_full**2/(arb.pi()**2*r_c**2)).exp()
        order = 4
        remainder = mu**order / factorial(order)**2 / (1-mu/(order+1)**2)
        print("cap radius", r_c, "ordered mass", mu.str(12),
              "energy amplification", growth.str(12),
              "degree-three relative remainder", remainder.str(12))
        if r_c == 400:
            assert growth < arb("1.000158")
            assert remainder < arb("3.77e-8")
            assert growth * arb("1.7e-6") < arb("1.701e-6")


if __name__ == "__main__":
    graph_and_projection()
    mixed_energy()
    directed_constants()
