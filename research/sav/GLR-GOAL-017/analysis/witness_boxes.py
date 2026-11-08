"""Directed lower thresholds for two smooth weak rejection tests."""

from itertools import product

from flint import acb, arb, ctx


ctx.prec = 192
END = arb("8.407362")
Q = acb("-0.177924631", "-0.747343369")
STAR = Q + arb("0.12656238")
Q_CORNERS = tuple(acb(x, y) for x, y in product(
    ("0.08223", "0.18771"), ("-0.42418", "-0.31707")
))
STAR_CORNERS = tuple(acb(x, y) for x, y in product(
    ("-0.21850", "-0.16857"), ("-0.44959", "-0.39966")
))
Q_MID = acb("0.13497", "-0.370625")
STAR_MID = acb("-0.193535", "-0.424625")
IOMEGA = acb(0, arb.pi() / END)


def laplace(rate):
    """Integral of exp(rate*u) over the exact fixed window."""
    if rate.is_zero():
        return acb(END)
    return (rate * END).expm1() / rate


def harmonic_pair(poles_and_residues):
    terms = [(s, s * a) for s, a in poles_and_residues]
    return terms + [(s.conjugate(), z.conjugate()) for s, z in terms]


def gram(left, right, weight):
    return sum(z * y * weight(s + t) for s, z in left for t, y in right).real


def sine(rate):
    return (laplace(rate + IOMEGA) - laplace(rate - IOMEGA)) / acb(0, 2)


def sine_squared(rate):
    return (2 * laplace(rate) - laplace(rate + 2 * IOMEGA)
            - laplace(rate - 2 * IOMEGA)) / 4


def exponent_error(pole, corners, radius):
    # Uniform derivative bound for 2Re(A s exp(s*u)), u in [0,END].
    assert pole.real + radius < 0
    amax = max(abs(a).upper() for a in corners)
    return 2 * amax * radius * (1 + (abs(pole) + radius) * END) * END.sqrt()


def weak_threshold(midpoint_terms, candidate_terms, error):
    midpoint = harmonic_pair(midpoint_terms)
    norm = gram(midpoint, midpoint, sine_squared).sqrt()
    q = []
    for items in candidate_terms:
        p = harmonic_pair(items)
        projection = gram(midpoint, p, sine) / norm
        p_norm = gram(p, p, laplace).sqrt()
        q.append(projection - p_norm / 2 - arb("1.5") * error)
    return min(value.lower() for value in q)


def main():
    q_error = exponent_error(Q, Q_CORNERS, arb("5e-8"))
    s_error = exponent_error(STAR, STAR_CORNERS, arb("7e-8"))
    a = weak_threshold([(Q, Q_MID)], ([(Q, x)] for x in Q_CORNERS), q_error)
    b = weak_threshold(
        [(Q, Q_MID), (STAR, STAR_MID)],
        ([(Q, x), (STAR, y)] for x, y in product(Q_CORNERS, STAR_CORNERS)),
        q_error + s_error,
    )
    assert a > arb("0.1702965")
    assert b > arb("0.5102615")
    print("Enclosed exponent-only L2 errors (Q and S*):", q_error, s_error)
    print("Q weak rejection lower threshold:", a)
    print("Q+S* weak rejection lower threshold:", b)


if __name__ == "__main__":
    main()
