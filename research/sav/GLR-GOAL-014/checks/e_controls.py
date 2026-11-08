"""Exact graph/clock controls and actual-RW oscillatory check, non-production."""

from math import cos, sin, sqrt
from platform import python_version

import scipy
import sympy as sp
from scipy.integrate import quad


def graph_and_seam():
    u, v, potential = sp.symbols('u v V', real=True)
    b = 2 - u / 2
    w = u * (v - b)
    f = 4 * w.diff(v)
    g = 0
    h = -potential * w
    assert sp.simplify(-4 * w.diff(u, v) - potential * w + f.diff(u) - h) == 0
    graph_p = w.diff(u).subs(v, b)
    assert graph_p == u / 2 and sp.simplify(4 * graph_p - g + b.diff(u) * f) == 0
    assert sp.expand(-b.diff(u) * f + g) == 2 * u  # omitted graph jet
    true = sp.integrate(w.diff(u).subs(v, 4), (u, 0, 1))
    terminal = sp.integrate(f, (v, b, 4)).subs(u, 1) / 4
    assert terminal == true == sp.Rational(5, 2)
    sigma = v - 3 - u / 2
    assert sigma.diff(u) == -sp.Rational(1, 2)  # [F]=1 and [G]=0
    print('Exact graph/terminal fixture: nonzero graph jet, terminal-only cap overlap 5/2.')
    print('Omitted graph jet gives +2u contact on the RHS of 4p_v.')
    print('Oblique seam v=3+u/2, unit F jump: coefficient in div(F,G) is -1/2.')


def cut_relabel():
    q = sp.symbols('q', real=True)
    n = 1 + q**2
    correction = 1 - q
    xi = 2 + 3 * q + q**2
    endpoint = (xi * n**2 / 2).subs(q, 1) - (xi * n**2 / 2).subs(q, 0)
    original = sp.integrate(n * correction, (q, 0, 1))
    transport = sp.integrate(n * (correction + xi * n.diff(q)) + xi.diff(q) * n**2 / 2, (q, 0, 1))
    broken = sp.integrate(n * (correction + xi * n.diff(q)), (q, 0, 1))
    assert transport - endpoint == original
    assert broken - endpoint != original
    print(f'Exact cut relabel: original={original}, fixed-label={transport}, boundary={endpoint}.')
    print('Moving endpoints cancel the change only with the flux Jacobian included.')


def oscillatory():
    chi = lambda u: u**2 * (1 - u)**2
    d_chi = lambda u: 2 * u * (1 - u) * (1 - 2 * u)
    print('n; actual-RW ||w||X bound; ||A_F||2 bound; ||N||2; fixed test; co-oscillatory pairing')
    for n in (32, 128, 512):
        news = lambda u: chi(u) * cos(n*u) + d_chi(u) * sin(n*u) / n
        norm = sqrt(quad(lambda u: news(u)**2, 0, 1, limit=512, epsabs=1e-12)[0])
        fixed = quad(lambda u: chi(u) * news(u), 0, 1, limit=512, epsabs=1e-12)[0]
        moving = quad(lambda u: chi(u) * cos(n*u) * news(u), 0, 1, limit=512, epsabs=1e-12)[0]
        print(f'{n}; {1/(16*n):.8g}; {9/(20*n):.8g}; {norm:.10g}; {fixed:.6g}; {moving:.12g}')
    print('Analytic limits for norm^2 and co-oscillatory pairing: 1/1260.')


if __name__ == '__main__':
    print(f'Python {python_version()}, SciPy {scipy.__version__}, SymPy {sp.__version__}')
    graph_and_seam()
    cut_relabel()
    oscillatory()
