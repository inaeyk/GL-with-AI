"""Check the exact powers removed in the horizon-regular EF conversion."""

from platform import python_version

import sympy as sp


def main():
    t, a, omega, n0, n1, n2 = sp.symbols("t a omega n0 n1 n2")
    r = 1 + t
    f = t / r
    d = 1 + a*r**3
    n = n0 + n1*t + n2*t**2
    y = d*n/r
    yp = (2*a*r-1/r**2)*n + d*sp.diff(n,t)/r
    b = t/(2*a*r**3*d)-3*t**2/d**2
    bf = 1/(2*a*r**2*d)-3*r*t/d**2
    vv = (f*(1-2*a*r**3)/(2*d**2)-omega**2/(a*d))*y + b*yp + omega*bf*y
    ftr = -omega*(f*yp+(omega-1/(2*r**2))*y)/(a*d)
    angular = -r**2*y/(2*d)-t*yp/(a*d)-omega*r*y/(a*d)
    vr_numerator = r*(ftr-vv)
    rr_numerator = 2*r**2*(vv-ftr-f*angular/r**2)
    regular_slope = n0*(a-5+6/(1+a))/(1+2*omega)
    print(f"Python {python_version()}, SymPy {sp.__version__}")
    for label, expr, power in (
        ("vr", vr_numerator, 0), ("rr", rr_numerator, 0), ("rr", rr_numerator, 1)
    ):
        coefficient = sp.factor(sp.diff(expr, t, power).subs(t, 0))
        constrained = sp.factor(coefficient.subs(n1, regular_slope))
        assert constrained == 0
        print(f"{label} numerator t^{power}: free={coefficient}, regular={constrained}")
    # A generic linear slope fails the second cancellation.
    assert sp.factor(sp.diff(rr_numerator, t).subs(t, 0)) != 0


if __name__ == "__main__":
    main()
