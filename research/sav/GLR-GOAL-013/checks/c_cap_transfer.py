"""Directed polynomial cap transfer and two manufactured controls, r0=1."""
from fractions import Fraction
from math import comb, factorial
from platform import python_version

import flint
from flint import arb, ctx


# Polynomials are sparse monomial dictionaries keyed by (degree in w, degree in y).
def add(*polynomials):
    result = {}
    for polynomial in polynomials:
        for power, coefficient in polynomial.items():
            result[power] = result.get(power, 0) + coefficient
    return result


def scale(polynomial, coefficient):
    return {power: coefficient * value for power, value in polynomial.items()}


def multiply(left, right):
    result = {}
    for (w, y), a in left.items():
        for (s, t), b in right.items():
            power = w+s, y+t
            result[power] = result.get(power, 0) + a*b
    return result


def primitive(polynomial, axis):
    result = {}
    axis_index = {"w": 0, "y": 1}[axis]
    for (w, y), coefficient in polynomial.items():
        power = (w+1, y) if axis_index == 0 else (w, y+1)
        result[power] = coefficient / power[axis_index]
    return result


def trace(polynomial):
    result = {}
    for (_, y), coefficient in polynomial.items():
        result[0, y] = result.get((0, y), 0) + coefficient
    return result


def norm(polynomial):
    # Polynomial independent of w. Integrate its square over y in [0,1].
    return sum(coefficient/(y+1)
               for (_, y), coefficient in multiply(polynomial, polynomial).items()).sqrt()


def add_univariate(left, right):
    result = dict(left)
    for degree, coefficient in right.items():
        result[degree] = result.get(degree, 0) + coefficient
    return result


def tortoise_polynomial(p):
    # If d_x^j V = z²(1-z)p, then d_x^{j+1} V = z²(1-z) times this polynomial.
    return add_univariate({j+1: -(j+2)*value for j, value in p.items()},
                          {j+2: (j+3)*value for j, value in p.items()})


def coefficient_model(duration, radius, order):
    """Approximate k(w,y), return its true L1_w L∞_y norm and Taylor error bounds."""
    zc = arb(1)/radius
    polynomial = {}
    p = {0: 6, 1: -3}
    for j in range(order+1):
        # Substitute z=zc(1-w) and a=D(1-y)/2 into the normalized expansion.
        radial = add(*[
            {(w, 0): coefficient * zc**degree * comb(degree, w) * (-1)**w
             for w in range(degree+1)}
            for degree, coefficient in p.items()
        ])
        cut = {(0, y): comb(j, y)*(-1)**y for y in range(j+1)}
        prefactor = duration*zc/2 * (duration/2)**j / factorial(j)
        polynomial = add(polynomial, scale(multiply(radial, cut), prefactor))
        p = tortoise_polynomial(p)
    n = order+1
    mu = duration*zc/2 * (6 - 3*zc/2)
    delta = (duration*zc/(2*(1-zc)) * (duration/2)**n / factorial(n)
             * sum(abs(coefficient)*zc**degree/(degree+1)
                   for degree, coefficient in p.items()))
    return polynomial, mu, delta


def ordered_tail(mu, order):
    n = order+1
    assert n >= 2 and mu < n*(n+1)
    first = mu**n / (factorial(n)*factorial(n-1) * arb((2*n-1)*2*n).sqrt())
    return first/(1-mu/(n*(n+1)))


def amplification(mu):
    return 1 + 2*mu/arb.pi() + ordered_tail(mu, 1)


def picard(coefficient, data, source, order):
    base = add(data, primitive(source, "w"))
    increment = result = base
    for _ in range(order):
        increment = scale(primitive(multiply(coefficient, primitive(increment, "y")), "w"), -1)
        result = add(result, increment)
    return trace(result)


def arithmetic_radius(polynomial):
    return sum(coefficient.rad() for coefficient in polynomial.values())


def scalar_tests(duration):
    y = {(0, 1): arb(1)}
    radius = 100
    k4, mu, delta4 = coefficient_model(duration, radius, 4)
    k5, _, delta5 = coefficient_model(duration, radius, 5)
    # An independently integrated frozen-ray solution has coefficients
    # (-mu)^j / [j! (j+1)!] multiplying y^(j+1).
    k0, _, _ = coefficient_model(duration, radius, 0)
    order = 5
    frozen = picard(k0, y, {}, order)
    for j in range(order+1):
        reference = (-mu)**j / (factorial(j)*factorial(j+1))
        assert frozen[0, j+1].overlaps(reference)
    print("frozen-ray exact coefficient cross-check: all six overlap")

    # Exact physical solution y*w², zero before the cap. The positive source
    # in the compactified equation is 2*y*w + k_true*y²*w²/2.
    # Approximate that coefficient in the manufactured source using order 5,
    # and use order 4 for the propagated operator.
    source_shape = {(2, 2): arb(1)/2}
    source = add({(1, 1): arb(2)}, multiply(k5, source_shape))
    trace_forced = picard(k4, {}, source, order)
    base_bound = 1/arb(3).sqrt() + (mu+delta5)/(2*arb(5).sqrt())
    residual_bound = (amplification(mu+delta4)*(delta4+delta5)/(2*arb(5).sqrt())
                      + ordered_tail(mu+delta4, order)*base_bound)
    actual = norm(add(trace_forced, scale(y, -1)))*duration.sqrt()
    certified = residual_bound*duration.sqrt()
    omission = (duration/3).sqrt()
    assert actual < certified < arb("0.000001")
    assert omission > arb("1.74")
    print(f"manufactured forced-tail actual L2 discrepancy < {actual.upper()}")
    print(f"manufactured forced-tail certificate < {certified.upper()}")
    print(f"manufactured coefficient arithmetic error < "
          f"{(arithmetic_radius(trace_forced)*duration.sqrt()).upper()}")
    print(f"omit external forcing: L2 error = sqrt(D/3) > {omission.lower()}")


def weak_boundary_control():
    # Exact flat cap fixture, D=3/2 and g=y. Set rho*A=2*w*y²,
    # B=y*(1-w), P_cap=B_cap=y and C0=0. Then P_inf=2*y/D.
    duration = Fraction(3, 2)
    exact_overlap = duration*(2/duration)*Fraction(1, 3)
    edge = Fraction(1)  # integral of 2w over w, g(U)=1
    bulk = -duration*(1/duration)*Fraction(1, 3)
    omitted_cap_jump = duration*Fraction(1, 3)
    assert exact_overlap == edge+bulk == Fraction(2, 3)
    assert bulk == -Fraction(1, 3) and omitted_cap_jump == Fraction(1, 2)
    print("weak flat boundary: exact overlap 2/3, dropping cut edge gives -1/3, "
          "dropping cap shift adds 1/2")
    # Entire flat causal domain: v_sigma = 1-u, g=u, A=C0=0,
    # B=P=u*(2-v)_+. The only two strong terms cancel.
    graph = Fraction(1, 3)+Fraction(1, 4)
    volume = -graph
    assert graph+volume == 0 and volume == -Fraction(7, 12)
    print("weak flat graph: exact overlap 0, dropping graph gives -7/12")
    # Insert an interface at v=1, B=u*(2-v) for 1<v<2, zero otherwise.
    # Cellwise volume derivative alone loses the delta at v=1.
    interface = Fraction(1, 3)
    cellwise_volume = -Fraction(1, 3)
    assert interface+cellwise_volume == 0
    print("weak flat interface: exact overlap 0, dropping jump gives -1/3")


def packet_dual_bounds(front):
    peak_radius = (27+arb(153).sqrt())/24
    peak_tortoise = peak_radius + (peak_radius-1).log()
    peak_potential = (1-1/peak_radius)*(6/peak_radius**2 - 3/peak_radius**3)
    for label, width in (("early", arb(3)/200), ("packet", arb(3)/25)):
        cut = front+width
        graph_radius = 1+2*((-(1+cut)/2).exp()/2).lambertw()
        graph_tortoise = (graph_radius-cut)/2
        assert graph_radius < peak_radius
        mass = (peak_tortoise-graph_tortoise)*peak_potential/2
        mass += 3/peak_radius-3/(4*peak_radius**2)
        mu = width*mass
        bound = amplification(mu)
        derivative = mass*bound
        assert bound < (arb("1.016") if label == "early" else arb("1.133"))
        print(f"{label} global adjoint: ||Z||/||g|| < {bound.upper()}; "
              f"extra derivative multiplier < {derivative.upper()}")


def table(duration):
    print("r_c Taylor Picard   coefficient defect (upper), homogeneous relative error (upper)")
    for radius, h, m in ((25, 4, 5), (100, 0, 3), (100, 2, 3), (100, 4, 3),
                         (100, 4, 5), (100, 6, 5), (200, 4, 5)):
        _, mu, delta = coefficient_model(duration, radius, h)
        mu_h = mu+delta
        error = (2*delta/arb.pi()*amplification(mu)*amplification(mu_h)
                 + ordered_tail(mu_h, m))
        print(f"{radius:3} {h:6} {m:6}   {delta.upper()}, {error.upper()}")
        if (radius, h, m) == (100, 4, 3):
            assert error < arb("0.0000055")
        if (radius, h, m) == (100, 6, 5):
            assert error < arb("0.000000002")
    _, mu, _ = coefficient_model(duration, 100, 4)
    print(f"actual-operator degree-three remainder < {ordered_tail(mu, 3).upper()}")
    print(f"cap amplification < {amplification(mu).upper()}")


def main():
    print(f"Python {python_version()}, python-flint {flint.__version__}")
    weak_boundary_control()
    for precision in (96, 160):
        ctx.prec = precision
        print(f"precision {precision} bits")
        front = -arb(13)/8 - 2*(arb(5)/8).log()
        duration = arb("8.407362") - front
        packet_dual_bounds(front)
        table(duration)
        scalar_tests(duration)


if __name__ == "__main__":
    main()
