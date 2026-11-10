"""Exact bidegree-(1,1) principal compact symbol from GOAL-016 TRACE Eqs. (1)-(3)."""
from dataclasses import dataclass
from math import prod
from platform import python_version
from resource import RUSAGE_SELF, getrusage

import sympy as sp

h, s = sp.symbols("h s")
xi, zeta = sp.symbols("xi zeta")
Q = sp.Rational


@dataclass
class Jet:
    """Keys (a,b,p) mean alpha**a eta**b R**(-p) times the Fourier product."""
    terms: dict

    def __add__(self, other):
        result = self.terms.copy()
        for key, value in other.terms.items():
            result[key] = result.get(key, 0) + value
        return Jet(result)

    def __mul__(self, other):
        result = {}
        for (a, b, p), left in self.terms.items():
            for (c, d, q), right in other.terms.items():
                if a + c <= 1 and b + d <= 1:
                    key = (a+c, b+d, p+q)
                    result[key] = result.get(key, 0) + left*right
        return Jet(result)

    def scale(self, factor):
        return Jet({key: factor*value for key, value in self.terms.items()})

    def radial_power(self, power):
        return Jet({(a, b, p-power): value for (a, b, p), value in self.terms.items()})

    def dr(self):
        return Jet({(a, b, p+1): -p*value for (a, b, p), value in self.terms.items()})

    def dz(self):
        return Jet({(a, b, p): (a*xi+b*zeta)*value
                    for (a, b, p), value in self.terms.items()})

    def terminal_integral(self):
        # For Re(p)>1, this equals minus the integral from R to infinity.
        return Jet({(a, b, p-1): value/(1-p)
                    for (a, b, p), value in self.terms.items()})

    def degree(self, degree):
        return Jet({key: value for key, value in self.terms.items()
                    if key[0] + key[1] == degree})


def direct_constraint_elimination():
    one = Jet({(0, 0, 0): 1})
    w = Jet({(1, 0, h): 1, (0, 1, s): 1})
    q = w.radial_power(-1)
    beta = (q.dr()*q.dr()).radial_power(1).scale(Q(3, 4)).terminal_integral()
    p = (q.dz().radial_power(1).scale(6)
         + beta.dz().radial_power(1).scale(-4)
         + (q.dr()*q.dz()).radial_power(2).scale(3)).terminal_integral().radial_power(-2)
    first_a = (p + q.dz().scale(-3)).degree(1).terminal_integral()
    a = (p + beta.dz() + q.dz().scale(-3)
         + (q.dr()*first_a).scale(3)).terminal_integral()
    # At this bidegree only the first variation of the angular potential
    # can multiply W. The background V radial terms are linear in W.
    v = (p.scale(Q(1, 2)) + a.radial_power(-1).scale(2)).degree(1).dz()
    v = v.radial_power(2).terminal_integral().radial_power(-1)
    angular = (v*w.dr()).dr() + (v.dr()*w).radial_power(-1).scale(-1)
    # T, U and varpi below carry a tilde, denoting multiplication by b.
    compact_gradient = q.dz() + (a*q.dr()).scale(-1)
    trace = p.scale(-1) + compact_gradient + a.radial_power(-1).scale(-2)
    repeated = p.scale(Q(-1, 2)) + compact_gradient
    sphere = a.radial_power(-1).scale(-1) + compact_gradient.scale(Q(-1, 2))
    normal = (trace.dz() + (a*trace.dr()).scale(-1)
              + (compact_gradient*trace).scale(-1)
              + (repeated*repeated + sphere*sphere).scale(2))
    scalar = (one + q.scale(-3))*normal
    scalar = scalar.radial_power(1) + angular
    mixed = {key: sp.cancel(value) for key, value in scalar.terms.items()
             if key[:2] == (1, 1) and sp.cancel(value) != 0}
    assert list(mixed) == [(1, 1, h+s+1)]
    return sp.Poly(mixed[(1, 1, h+s+1)], xi, zeta), angular


def short_formula(hh):
    d0 = 3*(s+1)/(s*(s-1))
    c0 = 1-6/(s*(s-1))
    v0 = -3*(s+2)/(s*(s-1)*(s-2))
    b = -3*(hh+1)*(s+1)/(2*(hh+s+2))
    p = 3*(hh+1)*(hh-s)/((hh+s)*(hh+s+2))
    d = (3*(hh+1)*d0-p-b)/(hh+s+1)
    a = v0*(hh*(hh+s)+s-1)
    return sp.cancel(-p-2*d+(hh+1)*d0-3*c0+a), a


def f(x):
    return x*(x-1)/((x+2)*(x+3))


def pole_terms(expr):
    """Read a proper rational partial fraction with integer pole locations."""
    coefficients = {}
    for part in sp.Add.make_args(sp.apart(expr, s)):
        numerator, denominator = sp.fraction(part)
        root = sp.solve(denominator, s)
        assert not numerator.has(s) and len(root) == 1 and root[0].is_Integer
        location = int(root[0])
        power = sp.degree(denominator, s)
        lc = sp.Poly(denominator, s).LC()
        coefficients[location, power] = numerator/lc
    return coefficients


def difference_inverse(expr):
    """Solve b(s)-b(s-1)=expr within rational functions if possible."""
    terms = pole_terms(expr)
    lo = min(p for p, _ in terms)
    hi = max(p for p, _ in terms)
    orders = sorted({k for _, k in terms})
    residue_sums = {k: sum(terms.get((p, k), 0) for p in range(lo, hi+1))
                    for k in orders}
    if any(residue_sums.values()):
        return None, residue_sums
    solution = 0
    for k in orders:
        running = 0
        for p in range(lo, hi):
            running += terms.get((p, k), 0)
            solution += running/(s-p)**k
    assert sp.cancel(solution-solution.subs(s,s-1)-expr) == 0
    return solution, residue_sums


def main():
    print(f"Python {python_version()}, SymPy {sp.__version__}, exact rational identities")
    poly, angular = direct_constraint_elimination()
    direct = poly.coeff_monomial(zeta**2)
    crossed = poly.coeff_monomial(xi*zeta)
    assert sp.cancel(direct-short_formula(h)[0]) == 0
    assert sp.cancel(poly.coeff_monomial(xi**2)-direct.xreplace({s:h,h:s})) == 0
    assert sp.cancel(crossed-crossed.xreplace({s:h,h:s})) == 0
    c1 = -3-12*(s**2+2)/((s-2)*(s-1)*(s+1)*(s+2))
    assert sp.cancel(direct.subs(h,0)-c1) == 0
    assert sp.limit(s*(direct+3),s,sp.oo) == 0
    print("Direct bilinear derivation matches short C_h formula and pinned h=0.")
    print("Each compact derivative allocation obeys interchange symmetry.")

    n = h+1
    k = sp.cancel(f(-s-n)/f(-s-1)*direct+3)
    g = 3*s*n*(h**2+2*h*s+5*h+2*s**2+2*s+2)/((s+h)*(s+1)*(s+2)*(s+h-1))
    hh = (s+h)*g.subs(s,s-1)/(s-1)
    assert sp.cancel(g-hh-k) == 0
    assert sp.cancel(s*hh.subs(s,s+1)-(s+n)*g) == 0
    assert sp.limit(s*k,s,sp.oo) == 0
    assert sp.limit(g,s,sp.oo) == 0
    assert sp.limit(hh,s,sp.oo) == 0
    print("general rational G(s):",sp.factor(g))
    print("general rational H(s):",sp.factor(hh))
    b_star = 3/(s-1)-3/s+3/(s+1)-3/(s+2)
    assert sp.cancel(g.subs(h,0)-s*b_star) == 0
    assert sp.cancel(hh.subs(h,0)-s*b_star.subs(s,s-1)) == 0
    residual = s*(s+1)*hh.subs(s,s+2)-(s+n)*(s+n+1)*g
    assert sp.cancel(residual-s*(s+n+1)*k.subs(s,s+1)) == 0
    print("radial flat commutator is s(s+n+1)K(s+1).")

    angular_coeff = sp.Poly(angular.terms[(1,1,h+s+1)],xi,zeta).coeff_monomial(zeta**2)
    sigma = Q(4)
    # On Re(s)>=sigma and h>3, all four denominator factors have modulus
    # at least |s|. Bound the remaining polynomial by the triangle inequality.
    g_bound = 6*n*(1/sigma+(h+1)/sigma**2+(h**2+5*h+2)/(2*sigma**3))
    h_bound = 6*n*(1+h/sigma)*(1/sigma+(h-1)/sigma**2+(h**2+3*h+2)/(2*sigma**3))
    for exponent in (5,6):
        h_value = exponent-1
        ck = sp.cancel(direct.subs(h,h_value))
        kk = sp.cancel(k.subs(h,h_value))
        rise = prod(s+j for j in range(exponent))
        b, residues = difference_inverse(sp.cancel(kk/rise))
        assert b is not None
        reconstructed_g = sp.cancel(rise*b)
        reconstructed_h = sp.cancel(reconstructed_g-kk)
        assert sp.cancel(reconstructed_g-g.subs(h,h_value)) == 0
        assert sp.cancel(reconstructed_h-hh.subs(h,h_value)) == 0
        d_background = 3*(h_value+1)/Q(h_value*(h_value-1))
        assert sp.limit(crossed.subs(h,h_value)/s,s,sp.oo) == 2*d_background
        print(f"n={exponent} (h={h_value}):")
        print("  pre-transform C(s):",sp.factor(ck))
        print("  mixed a_z g_z coefficient:",sp.factor(crossed.subs(h,h_value)))
        print("  transformed K(s):",sp.factor(kk))
        for x in (4,5,6):
            angular_value = angular_coeff.subs({s:x,h:h_value})
            print(f"  s={x}: C={ck.subs(s,x)}, angular={angular_value}, K={kk.subs(s,x)}")
        print("  shift-orbit discrete residue sums:",residues)
        print("  independently reconstructed G:",sp.factor(reconstructed_g))
        print("  independently reconstructed H:",sp.factor(reconstructed_h))
        print(f"  sigma={sigma} norm bounds: ||G|| <= {g_bound.subs(h,h_value)}, "
              f"||H|| <= {h_bound.subs(h,h_value)}")
    resource = getrusage(RUSAGE_SELF)
    print(f"CPU {resource.ru_utime+resource.ru_stime:.3f} s, peak RSS "
          f"{resource.ru_maxrss} (bytes on macOS)")


if __name__ == "__main__":
    main()
