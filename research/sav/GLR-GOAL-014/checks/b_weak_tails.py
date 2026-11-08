"""Directed arithmetic for the physical weak-current exterior bounds."""
from platform import python_version

import flint
from flint import arb, ctx


def bounds(radius):
    """Upper bounds uniform in u, including the two endpoint rays."""
    am = 10**5 * (-(arb(2) / 5) * (radius - 4)).exp()
    a0 = 10**8 * (100 + 10**9 * (radius + 6)) * (radius + 5)
    a0 *= (-(arb(4) / 5) * (radius - 4)).exp()
    mixed = 12 * 10**48 * am
    scalar = 8 * 10**13 * a0
    integral = arb(3).sqrt() * mixed + arb(2).sqrt() * scalar
    trace = arb(2).sqrt() * (mixed + scalar)
    return integral, trace


def main():
    for bits in (128, 192):
        ctx.prec = bits
        print(f"Python {python_version()} / python-flint {flint.__version__} / {bits} bits")
        assert (1 - arb(1) / 100) * (arb.pi() / 5)**2 > arb(1) / 4
        assert 1 / (2 * arb(105)) < arb(1) / 12
        assert (arb(1) - arb(1) / 100) * (6 - arb(3) / 100) > 2
        for radius in (100, 330, 350, 400, 500):
            integral, trace = bounds(radius)
            if radius == 350:
                assert integral < arb("1.63e-6") and trace < arb("1.33e-6")
            if radius == 400:
                assert integral < arb("3.36e-15") and trace < arb("2.74e-15")
            print(f"r_c={radius} ray_integral_bound={integral.str(12)} "
                  f"current_sup_bound={trace.str(12)}")


if __name__ == "__main__":
    main()
