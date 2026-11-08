"""Enclose the two fixed-clock news pole-pair norms with the directed residue boxes."""

from flint import acb, arb, ctx
import flint
import platform


def interval(first, last):
    lower, upper = arb(first), arb(last)
    return lower.union(upper)


def integrate_exp(s, a, b):
    return (s*b).exp() * (-(s*(b-a))).expm1() / (-s)


def norm_square(s, amplitude, start, stop):
    z = s * amplitude
    return (2*(z.real**2 + z.imag**2) * integrate_exp(acb(2*s.real), start, stop).real
            + 2*(z**2 * integrate_exp(2*s, start, stop)).real)


def pole_norm(s, bounds, start, stop):
    real_lo, real_hi, imag_lo, imag_hi = map(arb, bounds)
    cells = 32
    hull = None
    for i in range(cells):
        a = (real_lo + (real_hi-real_lo)*i/cells).union(
            real_lo + (real_hi-real_lo)*(i+1)/cells
        )
        for j in range(cells):
            b = (imag_lo + (imag_hi-imag_lo)*j/cells).union(
                imag_lo + (imag_hi-imag_lo)*(j+1)/cells
            )
            enclosure = norm_square(s, acb(a, b), start, stop)
            hull = enclosure if hull is None else hull.union(enclosure)
    assert hull > 0
    return hull.sqrt()


def endpoints(ball):
    return ball.lower(), ball.upper()


def main():
    ctx.prec = 128
    print("Python", platform.python_version(), "python-flint", flint.__version__)
    s = acb(arb("-.177924631", "5e-8"), arb("-.747343369", "5e-8"))
    omega = interval(".06328118", ".06328120")
    shifted = s + 2*omega
    period = 2*arb.pi()/(-s.imag)
    print("period", endpoints(period))
    print("relative translated growth over a period", endpoints((2*omega*period).exp()))
    fundamental_box = (".08223", ".18771", "-.42418", "-.31707")
    # Outward rounding of the final shifted_enclosure.txt rectangle.
    shifted_box = ("-.21850", "-.16857", "-.44959", "-.39966")
    a_q = acb(interval(*fundamental_box[:2]), interval(*fundamental_box[2:]))
    a_star = acb(interval(*shifted_box[:2]), interval(*shifted_box[2:]))
    gamma_q, gamma_star = -s.real, -shifted.real
    q_infinite = abs(s).upper() * a_q.abs_upper() * (1/gamma_q + 1/abs(s)).sqrt()
    star_period = abs(shifted).lower() * a_star.abs_lower() * (
        (1-(-2*gamma_star*period).exp()) * (1/gamma_star - 1/abs(shifted))
    ).sqrt()
    ratio_coefficient = star_period / q_infinite
    packet_end = -arb(13)/8 - 2*(arb(5)/8).log() + arb(".12")
    post_support_lower = (ratio_coefficient * (2*omega*packet_end).exp()).lower()
    assert post_support_lower > 1
    print("phase-independent ratio lower coefficient before exp(2 Omega c)", ratio_coefficient.lower())
    print("ratio lower for all c >= packet end and duration >= exact period", post_support_lower)
    print("dimensionless windows: fundamental L2, shifted L2, ratio, cancellation lower bound at rho=1/2")
    for a in (arb(0), arb(2), arb(4), arb(8)):
        b = a+arb("8.407362")
        assert b-a > period.upper()
        fundamental = pole_norm(s, fundamental_box, a, b)
        translated = pole_norm(shifted, shifted_box, a, b)
        ratio = translated / fundamental
        assert ratio > 1
        cancellation = translated.lower() - fundamental.upper() / 2
        print(a, b, endpoints(fundamental), endpoints(translated), endpoints(ratio),
              cancellation.lower())


if __name__ == "__main__":
    main()
