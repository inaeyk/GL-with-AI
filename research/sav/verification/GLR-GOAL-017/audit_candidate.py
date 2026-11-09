"""Independent GOAL-017 affine norm and directed modal midpoint quadrature."""

import argparse
from fractions import Fraction
from hashlib import sha256

from flint import acb, arb, ctx
import numpy as np

ctx.prec = 192
D = "8.407362"
DIGEST = "34284f389420d62aaacd30b111b17f0c740fca3528b9d7cc85290909996d9c07"
EDGES = (
    (".08223", ".18771"), ("-.42418", "-.31707"),
    ("-.21850", "-.16857"), ("-.44959", "-.39966"),
)


def inputs(path):
    with np.load(path, allow_pickle=False) as z:
        u, full, returned, base = (z[k] for k in
                                  ("u", "u_full", "p_shear_infty", "baseline"))
    arrays = (u, full, returned, base)
    if not all(x.ndim == 1 and np.isfinite(x).all() for x in arrays):
        raise ValueError("nonfinite or non-vector archive field")
    if not ((np.diff(u) > 0).all() and (np.diff(full) > 0).all()):
        raise ValueError("non-monotone grid")
    cut = np.searchsorted(full, 0.)
    if not (u.size >= 2 and full.size == returned.size and base.size == u.size
            and np.array_equal(u, full[cut:]) and u[0] == 0. and u[-1] == float(D)):
        raise ValueError("unaligned comparison grid")
    news = returned[cut:]
    digest = sha256(np.column_stack((u, news, base)).astype("<f8").tobytes()).hexdigest()
    if digest != DIGEST:
        raise ValueError(f"unexpected comparison digest {digest}")
    return u, news


def exact_norm_sq(u, y):
    a = [Fraction(float(x)) for x in u[:-1]] + [Fraction(D)]
    b = [Fraction(float(x)) for x in y]
    return sum((r-l)*(yl*yl+yl*yr+yr*yr)/3
               for l, r, yl, yr in zip(a, a[1:], b, b[1:]))


def modal_values(q, star, u):
    z, w = q*(q*u).exp(), star*(star*u).exp()
    return [2*z.real, -2*z.imag, 2*w.real, -2*w.imag]


def inflate(x, error):
    return x + arb(0, error.abs_upper())


def independent_gram(q, star, count=4096):
    gram = [[arb(0) for _ in range(4)] for _ in range(4)]
    T = arb(D)
    h = T/count
    rate = [abs(q)]*2 + [abs(star)]*2
    for k in range(count):
        vals = modal_values(q, star, h*(2*k+1)/2)
        for i in range(4):
            for j in range(i+1):
                gram[i][j] += vals[i]*vals[j]
    for i in range(4):
        for j in range(i+1):
            # Supremum of the absolute second derivative of the product.
            M = 4*rate[i]*rate[j]*(rate[i]+rate[j])**2
            gram[j][i] = gram[i][j] = inflate(h*gram[i][j], T*h**2*M/24)
    return gram


def independent_linear(q, star, u, news, count=16):
    u = [arb(float(x)) for x in u[:-1]] + [arb(D)]
    y = [arb(float(x)) for x in news]
    rate = [abs(q)]*2 + [abs(star)]*2
    total = [arb(0) for _ in range(4)]
    for l, r, yl, yr in zip(u, u[1:], y, y[1:]):
        width = r-l
        h = width/count
        local = [arb(0) for _ in range(4)]
        for k in range(count):
            t = arb(2*k+1)/(2*count)
            v = yl+t*(yr-yl)
            for i, f in enumerate(modal_values(q, star, l+t*width)):
                local[i] += v*f
        for i in range(4):
            # Second derivative of an affine ordinate times a mode.
            M = (4*rate[i]**2*abs((yr-yl)/width)
                 +2*rate[i]**3*max(yl.abs_upper(), yr.abs_upper()))
            total[i] += inflate(h*local[i], width*h**2*M/24)
    return total


def modal_radius(gram, i, s, amp, amp_radius, exponent_radius):
    a, b, c = gram[i][i], gram[i+1][i+1], gram[i][i+1]
    gain = ((a+b+((a-b)*(a-b)+4*c*c).sqrt())/2).sqrt()
    T = arb(D)
    assert s.real + exponent_radius < 0
    # Segment bound on the complex exponent derivative, with damping dropped.
    freq_gain = 2*(abs(amp)+amp_radius)*(1+(abs(s)+exponent_radius)*T)*T.sqrt()
    return (gain*amp_radius + freq_gain*exponent_radius).upper()


def grid_minimum(gram, linear, sq, q, star, combined):
    from itertools import product

    count = 12 if combined else 64
    edges = EDGES if combined else EDGES[:2]
    halfwidths, centers = [], []
    for left, right in edges:
        a, b = arb(left), arb(right)
        step = (b-a)/count
        centers.append([a+(2*j+1)*step/2 for j in range(count)])
        halfwidths.append(step/2)
    qr = abs(acb(*halfwidths[:2]))
    sr = abs(acb(*halfwidths[2:])) if combined else None
    sqr = arb(2).sqrt()*arb("5e-8")
    ssr = sqr + arb("2e-8")
    minimum = None
    for center in product(*centers):
        p2 = sum(ai*aj*gram[i][j] for i, ai in enumerate(center)
                 for j, aj in enumerate(center))
        delta2 = sq+p2-2*sum(a*b for a, b in zip(center, linear))
        rad = modal_radius(gram, 0, q, acb(*center[:2]), qr, sqr)
        if combined:
            rad += modal_radius(gram, 2, star, acb(*center[2:]), sr, ssr)
        margin = (delta2.sqrt()-p2.sqrt()/2-3*rad/2).lower()
        if not margin.is_finite():
            raise ValueError("nonfinite subbox lower bound")
        minimum = margin if minimum is None else min(minimum, margin)
    return minimum


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("candidate")
    args = parser.parse_args()
    u, y = inputs(args.candidate)
    n2 = exact_norm_sq(u, y)
    sq = arb(n2.numerator)/arb(n2.denominator)
    print("exact affine norm enclosed by", sq.sqrt().str(22))
    q = acb("-.177924631", "-.747343369")
    star = q+arb(".12656238")
    gram = independent_gram(q, star)
    linear = independent_linear(q, star, u, y)
    for label, combined in (("Q", False), ("Q+S*", True)):
        margin = grid_minimum(gram, linear, sq, q, star, combined)
        print(f"{label} alternate quadrature lower bound {margin.str(17)}")
        assert margin > arb(".31730" if not combined else ".80483")

    # Simpler independent baseline rejection using only rational bounds on
    # the pole and residue moduli plus the physical baseline norm lower.
    q_abs, a_abs, exp_gain = map(Fraction, (".77", ".000875", "1.677"))
    sigma = Fraction(".1779")
    assert q_abs*q_abs > Fraction(".177924681")**2+Fraction(".747343419")**2
    assert a_abs*a_abs > Fraction(".000565")**2+Fraction(".000667")**2
    assert 2*sigma*exp_gain**2 > 1
    p_upper = 2*q_abs*a_abs*exp_gain
    ratio_lower = Fraction(".00501844")/p_upper-1
    print(f"baseline pole norm upper {float(p_upper):.12g}; ratio lower > {float(ratio_lower):.9g}")
    assert ratio_lower > Fraction("1.22")


if __name__ == "__main__":
    main()
