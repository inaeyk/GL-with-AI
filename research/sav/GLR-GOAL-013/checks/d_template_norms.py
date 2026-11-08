"""Directed central-window norms and uncertainty radii for both pole templates."""
from flint import acb, arb, ctx


ctx.prec = 160
length = arb('8.407362')


def integral(rate):
    return (rate * length).expm1() / rate


def cross(s, amp, t, other_amp):
    z = s * amp
    w = t * other_amp
    return 2 * (z * w * integral(s + t) + z * w.conjugate() * integral(s + t.conjugate())).real


def rectangle_center(re_lo, re_hi, im_lo, im_hi):
    a, b, c, d = map(arb, (re_lo, re_hi, im_lo, im_hi))
    return acb((a + b) / 2, (c + d) / 2), abs(acb((b - a) / 2, (d - c) / 2))


def uncertainty(s, amp, radius_amp, radius_s):
    # The amplitude error is the largest eigenvalue of its two-real-variable
    # Gram form. Treat the complex-frequency error by a segment derivative.
    amp_gain = abs(s) * (2 * (integral(2 * s.real) + abs(integral(2 * s)))).sqrt()
    assert s.real + radius_s < 0
    freq_gain = 2 * (abs(amp) + radius_amp) * (1 + (abs(s) + radius_s) * length) * length.sqrt()
    return (radius_amp * amp_gain + radius_s * freq_gain).upper()


def report(label, value):
    print(f'{label}: [{value.lower().str(16)}, {value.upper().str(16)}]')


def main():
    q = acb('-.177924631', '-.747343369')
    star = q + 2 * arb('.06328119')
    radius_q = arb(2).sqrt() * arb('5e-8')
    radius_star = radius_q + arb('2e-8')
    aq, raq = rectangle_center('.08223', '.18771', '-.42418', '-.31707')
    astar, rastar = rectangle_center('-.21850', '-.16857', '-.44959', '-.39966')
    q_norm_sq = cross(q, aq, q, aq)
    star_norm_sq = cross(star, astar, star, astar)
    cross_norm = cross(q, aq, star, astar)
    q_radius = uncertainty(q, aq, raq, radius_q)
    star_radius = uncertainty(star, astar, rastar, radius_star)
    report('Q and S* midpoint inner product', cross_norm)
    for name, midpoint, radius in (
        ('Q', q_norm_sq.sqrt(), q_radius),
        ('Q+S*', (q_norm_sq + star_norm_sq + 2 * cross_norm).sqrt(), q_radius + star_radius),
    ):
        report(f'{name} midpoint norm', midpoint)
        report(f'{name} uncertainty L2 cap', radius)
        report(f'{name} norm', midpoint + (-radius).union(radius))
        assert midpoint > radius


if __name__ == '__main__':
    main()
