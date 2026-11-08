"""Independent exact compact/exterior and signed-ray controls."""
import sympy as s

u, v = s.symbols("u v", real=True)
b, c, d = 1 - u, s.Integer(2), s.Integer(1)
g = 1 + u
Z = g + (u - 1) * s.exp(-v)
V = 4 * s.exp(-v) / Z
w = u**2 * (1 - s.exp(-(v - b)))
chi = u**2 * (v - b) * s.exp(-v)
F, G, H = 4 * s.diff(w, v) + s.diff(chi, v), -s.diff(chi, u), -V * w

assert s.simplify(4 * s.diff(Z, u, v) + V * Z) == 0
assert s.simplify(Z.subs(u, d) - g.subs(u, d)) == 0
assert s.limit(Z, v, s.oo) == g
assert s.simplify(4 * s.diff(w, u, v) + V * w - s.diff(F, u) - s.diff(G, v) + H) == 0
assert s.simplify((4 * s.diff(w, u) - G + s.diff(b, u) * F).subs(v, b)) == 0
assert w.subs(v, b) == w.subs(u, 0) == F.subs(u, 0) == 0
assert s.limit(G, v, s.oo) == 0

density = s.expand(s.diff(Z, u) * F + s.diff(Z, v) * G + s.cancel(Z * H))
def volume(low, high):
    return s.simplify(s.integrate(s.integrate(density, (v, low, high)), (u, 0, d)))

cap_g = s.integrate((Z * G).subs(v, c), (u, 0, d))
end_compact = g.subs(u, d) * s.integrate(F.subs(u, d), (v, b.subs(u, d), c))
end_outer = g.subs(u, d) * s.integrate(F.subs(u, d), (v, c, s.oo))
bulk_compact = volume(b, c)
bulk_outer = volume(c, s.oo)
p_cap_direct = s.integrate((Z * s.diff(w, u)).subs(v, c), (u, 0, d))
p_full_direct = s.integrate(g * s.limit(s.diff(w, u), v, s.oo), (u, 0, d))
weighted_source = s.expand(Z * (s.diff(F, u) + s.diff(G, v)) - s.cancel(Z * H))
p_outer_direct = s.integrate(s.integrate(weighted_source, (v, c, s.oo)), (u, 0, d)) / 4
assert s.simplify(4 * p_cap_direct - end_compact - cap_g + bulk_compact) == 0
assert s.simplify(4 * p_outer_direct - end_outer + cap_g + bulk_outer) == 0
assert s.simplify(p_cap_direct + p_outer_direct - p_full_direct) == 0
assert s.simplify((end_compact + end_outer - bulk_compact - bulk_outer) / 4 - p_full_direct) == 0
assert cap_g != 0 and end_outer != 0
print("compact +G boundary contribution to 4P:", s.simplify(cap_g))
print("outer −G boundary contribution to 4P:", s.simplify(-cap_g))
print("outer terminal F contribution to 4P:", s.simplify(end_outer))
print("full direct and full dual projection:", p_full_direct)
print("full dual with terminal F omitted:", s.simplify(p_full_direct - (end_compact + end_outer) / 4))
print("inconsistent strong/weak mix error:", s.simplify(-cap_g / 4))

# Two-component terminal-row check with a skew coupling and inhomogeneous ray residual.
J = s.Matrix([[0, -1], [1, 0]])
C = 4 * J
W_e = s.diag(2, 5)
e = s.Matrix([v**2, 1 + v])
transverse = s.Matrix([1 + v, 2 - v**2])
ev = e.diff(v)
R = -4 * transverse.diff(v) - W_e * e + C * (transverse + ev)
B_u = s.Matrix([[s.cos(v), s.sin(v)]])
B_v = s.Matrix([[v, 1]])
B_0 = s.Matrix([[1, -v]])
row = (c - v) * B_u
assert s.simplify(row.diff(v) + row * C / 4 + B_u) == s.zeros(1, 2)
true_integrand = (B_u * transverse + B_v * ev + B_0 * e)[0]
dual_integrand = ((B_v + row * C / 4) * ev + (B_0 - row * W_e / 4) * e - row * R / 4)[0]
assert s.simplify(true_integrand - dual_integrand + s.diff((row * transverse)[0], v)) == 0
value = s.integrate(true_integrand, (v, 0, c))
graph = (row * transverse)[0].subs(v, 0)
assert s.simplify(value - graph - s.integrate(dual_integrand, (v, 0, c))) == 0
print("terminal signed two-component projection:", s.simplify(value))
print("terminal graph jet term:", graph)
print("terminal residual contribution:", s.simplify(s.integrate(-(row * R)[0] / 4, (v, 0, c))))
