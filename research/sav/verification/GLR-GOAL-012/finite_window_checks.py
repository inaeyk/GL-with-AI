"""Non-production Arb checks from the published frequency and residue intervals."""

from flint import acb, arb, ctx
import flint
import platform


def limits(value):
    return value.lower(), value.upper()


def clipped(value, lower, upper):
    lo, hi = limits(value)
    def clamp_point(point):
        if point < lower:
            return lower
        if point > upper:
            return upper
        if point.overlaps(lower):
            return point.union(lower)
        if point.overlaps(upper):
            return point.union(upper)
        return point

    return clamp_point(lo).union(clamp_point(hi))


def rectangle_norm(s, start, duration, bounds):
    """Find extrema of the real two-variable Gram quadratic on a rectangle."""
    re_lo, re_hi, im_lo, im_hi = map(arb, bounds)
    alpha = s.real
    scalar = (2 * alpha * duration).expm1() / (2 * alpha)
    harmonic = (2 * s * duration).expm1() / (2 * s)
    cosine = (scalar + harmonic.real) / 2
    sine = (scalar - harmonic.real) / 2
    cross = harmonic.imag / 2

    phase = s * (s * start).exp()
    p, q = phase.real, phase.imag
    # News = 2 exp(alpha t) [(p Re A - q Im A) cos(beta t)
    #                          - (q Re A + p Im A) sin(beta t)].
    m11 = 4 * (cosine * p**2 + sine * q**2 - 2 * cross * p*q)
    m22 = 4 * (cosine * q**2 + sine * p**2 + 2 * cross * p*q)
    m12 = 4 * ((sine - cosine) * p*q + cross * (q**2 - p**2))
    assert m11 > 0 and m11*m22 - m12**2 > 0

    def square(a, b):
        return m11*a**2 + 2*m12*a*b + m22*b**2

    # Lower extreme on an edge since the sole interior stationary point is 0.
    assert re_hi < 0 or re_lo > 0 or im_hi < 0 or im_lo > 0
    edge_values = [square(a, clipped(-m12*a/m22, im_lo, im_hi))
                   for a in (re_lo, re_hi)]
    edge_values += [square(clipped(-m12*b/m11, re_lo, re_hi), b)
                    for b in (im_lo, im_hi)]
    lower = min(y.lower() for y in edge_values)
    # A convex function attains its maximum over a rectangle at some vertex.
    upper = max(square(a, b).upper() for a in (re_lo, re_hi)
                for b in (im_lo, im_hi))
    assert lower > 0
    return lower.sqrt().union(upper.sqrt())


def show(label, value):
    lo, hi = limits(value)
    print(f"{label} [{lo}, {hi}]")


def main():
    ctx.prec = 128
    print(f"Python {platform.python_version()}, python-flint {flint.__version__}")
    q = acb(arb("-.177924631", "5e-8"), arb("-.747343369", "5e-8"))
    omega = arb(".06328118").union(arb(".06328120"))
    star = q + 2*omega
    period = 2*arb.pi()/abs(q.imag)
    duration = arb("8.407362")
    assert duration > period
    q_box = (".08223", ".18771", "-.42418", "-.31707")
    star_box = ("-.21850", "-.16857", "-.44959", "-.39966")
    show("period", period)
    show("ratio multiplier per period", (2*omega*period).exp())

    for c in (0, 2, 4, 8):
        base = rectangle_norm(q, c, duration, q_box)
        shifted = rectangle_norm(star, c, duration, star_box)
        ratio = shifted / base
        margin = shifted.lower() - base.upper()/2
        assert ratio > 1 and margin > 0
        print(f"window start {c}")
        show("  Q norm", base)
        show("  shifted norm", shifted)
        show("  ratio", ratio)
        print(f"  necessary other-term norm exceeds {margin.lower()}")

    # Phase-uniform test after the direct launch support.
    R = arb(13)/8
    front = -R - 2*(R-1).log()
    packet_end = front + arb(".12")
    gamma, gamma_star = -q.real, -star.real
    # Build the amplitude modulus from rectangular corners, avoiding a loose
    # ball range for abs on a complex rectangle contained in one quadrant.
    aq_max = abs(acb(arb(q_box[1]), arb(q_box[2])))
    astar_min = abs(acb(arb(star_box[1]), arb(star_box[3])))
    num = abs(star)*astar_min * (
        (1-(-2*gamma_star*period).exp())*(1/gamma_star-1/abs(star))).sqrt()
    den = abs(q)*aq_max * (1/gamma+1/abs(q)).sqrt()
    all_windows = num/den*(2*omega*packet_end).exp()
    assert all_windows > arb("1.04495")
    show("lower-bound expression at packet end", all_windows)

    # Exact L2 norm of the primitive operator gives a smaller cap budget.
    rc = arb(100)
    d = duration - front
    old_k = d/2*(6/rc - 3/(2*rc**2))
    new_k = 2*old_k/arb.pi()
    new_tail = new_k.exp() - (1+new_k+new_k**2/2+new_k**3/6)
    show("cap budget with source norm excluded", new_k)
    show("cap degree-three homogeneous remainder factor", new_tail)


if __name__ == "__main__":
    main()
