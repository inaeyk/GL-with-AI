"""Exact, non-production controls for the sharp-cut and flux normalizations."""

from decimal import Decimal
import sympy as sp


u, v = sp.symbols("u v", real=True)
p = -sp.Rational(1, 3)
b = 1 - u / 2
c = 2
g = u
integrate_u = lambda expr: sp.integrate(expr, (u, 0, 1))

# F and G are zero outside their stated radial supports. The pieces vanish
# with first derivatives at the join. F has unit v-integral on [1,2].
F = u**3 * 30 * (v - 1) ** 2 * (2 - v) ** 2
G = u**3 * 2 * ((v - 1) * (3 - v)) ** 2
F_ray = sp.integrate(F, (v, 1, 2))
terminal = (g * F_ray).subs(u, 1)
volume = integrate_u(-sp.diff(g, u) * F_ray)
cap = integrate_u(g * G.subs(v, c))
exterior = integrate_u(g * (G.subs(v, 3) - G.subs(v, c)))
direct = integrate_u(g * (sp.diff(F_ray, u) + G.subs(v, 3)))
assert (terminal, volume, cap, exterior) == (1, -sp.Rational(1, 4), sp.Rational(2, 5), -sp.Rational(2, 5))
assert direct == terminal + volume + cap + exterior == sp.Rational(3, 4)
assert direct - terminal == -sp.Rational(1, 4)
assert direct - exterior == sp.Rational(23, 20)
print(f"4P exact={direct}, without terminal={direct-terminal}, without exterior cap contact={direct-exterior}")

# A distinct tilted graph control: 4w=(v-b)F, with G=H=0 and V=0.
# F now has support on the graph, and the graph/contact normalization matters.
F_graph = u**3
four_w = (v - b) * F_graph
assert sp.simplify(sp.diff(four_w, u).subs(v, b) + sp.diff(b, u) * F_graph) == 0
assert sp.diff(b, u) == -(1 + p) / (1 - p)
tilted_direct = integrate_u(sp.diff(four_w, u).subs(v, c))
omit_tilt = integrate_u((c - b) * sp.diff(F_graph, u))
wrong_tilt = omit_tilt + integrate_u(sp.diff(b, u) * F_graph)
assert (tilted_direct, omit_tilt, wrong_tilt) == (sp.Rational(3, 2), sp.Rational(11, 8), sp.Rational(5, 4))
print(f"4P tilted={tilted_direct}, zero graph velocity={omit_tilt}, reversed tilt={wrong_tilt}")

# The graph energy is E-pJ and dx=|du|/(1-p) in an unoriented integral.
et = 3
ex = -p * et
E = (et**2 + ex**2) / 2
J = -et * ex
assert E - p * J == (1 - p**2) * et**2 / 2 == 4
assert E + p * J == 6
assert (E - p * J) / (1 - p) == (1 + p) * et**2 / 2 == 3
print("graph flux per dx=4, per |du|=3, wrong sign per dx=6")

# One null seam. The field and v-tangential derivative are continuous.
# Set V smooth and constant in this test, to isolate the interface measure.
u0 = sp.Rational(1, 2)
V = sp.symbols("V", real=True)
rho = sp.Function("rho")(v)
e = (u - u0) * sp.Heaviside(u - u0) * rho
simplify = lambda expr: sp.expand(expr).subs(u * sp.DiracDelta(u - u0), u0 * sp.DiracDelta(u - u0)).expand()
R = simplify(-4 * sp.diff(e, u, v) - V * e)
assert not R.has(sp.DiracDelta)
commuted = simplify(sp.diff(R, u) + sp.diff(R, v))
assert commuted.coeff(sp.DiracDelta(u - u0)) == -4 * sp.diff(rho, v)
print("null seam: uncommuted delta=0, commuted delta=-4*rho'(v)")

# The polynomial thin-layer model can be extended by zero at s=1.
# The value is zero at the terminal cut s=0 and its first derivative is one.
s = sp.symbols("s", real=True)
psi = s * (1 - s) ** 3
chi = sp.sqrt(30) * v * (1 - v)
assert psi.subs(s, 0) == 0 and sp.diff(psi, s).subs(s, 0) == 1
assert sp.integrate(chi**2, (v, 0, 1)) == 1
square = sp.integrate(sp.diff(psi, s) ** 2, (s, 0, 1))
assert square == sp.Rational(3, 35)
print(f"thin layer: ||e_u(d,.)||_2=1, ||e_u||_2^2=({square})*width")

D = Decimal
weak = D("7.80") - D("3.8") - D("37") * D("0.1031368248")
strong_400 = D("7.80653") - D("0.024231") - D("400") * D("0.013648127")
strong_570 = D("7.80653") - D("0.024231") - D("570") * D("0.013648127")
assert weak == D("0.1839374824")
assert strong_400 == D("2.3230482")
assert strong_570 == D("0.00286661")
print(f"conditional margins: weak={weak}, strong 400={strong_400}, strong 570={strong_570}")
