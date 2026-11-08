"""Directed arithmetic for investigator B's analytic source-tail bound."""
from platform import python_version

import flint
from flint import arb, ctx


def tortoise(r):
    return r + (r - 1).log()


def launch_cut(r):
    return r - 2 * tortoise(r)


def source_cap(radius):
    mixed_rate = arb(2) / 5
    quadratic_rate = 2 * mixed_rate
    return (
        arb(10) ** 63 * (-mixed_rate * (radius - 4)).exp(),
        arb(10) ** 55 * (radius + 6) * (-quadratic_rate * (radius - 4)).exp(),
    )


def main():
    ctx.prec = 128
    upper = arb("8.407362")
    onset = launch_cut(arb(13) / 8)
    length = upper - onset
    width = arb(3) / 25
    f_min_radius = arb(200) / 199
    assert launch_cut(f_min_radius) > upper
    assert length < 10
    assert launch_cut(arb(3) / 2) > onset + width
    assert upper + 2 * tortoise(arb(4)) < 20
    assert arb(152) * 6 + 2 * (3 * length).sqrt() < 930
    assert arb(152) * 3 + 2 * (3 * length / 2).sqrt() < 470
    normalization = arb(4).exp() / arb(2).sqrt()
    assert normalization < 40
    for j in range(4):
        maximizer = arb(max(4, 2 * j))
        derivative_cap = (
            width.sqrt() / width**j
            * normalization * 4 * maximizer ** (2 * j) * (-maximizer).exp()
        )
        assert derivative_cap < 10**7
    print(f"Python {python_version()} / python-flint {flint.__version__} / {ctx.prec} bits")
    print(f"onset={onset.str(16)} length={length.str(16)}")
    print(f"u_Sigma(200/199)={launch_cut(f_min_radius).str(12)}")
    for radius in (100, 400, 500):
        mixed, quadratic = source_cap(radius)
        total = mixed + quadratic
        v_cap = upper + 2 * tortoise(arb(radius))
        if radius == 400:
            assert total < arb("1.7e-6")
        if radius == 500:
            assert total < arb("7e-24")
        print(
            f"r_c={radius} V_c={v_cap.str(12)} "
            f"mixed_bound={mixed.str(9)} quadratic_mean_bound={quadratic.str(9)} "
            f"total_bound={total.str(9)}"
        )


if __name__ == "__main__":
    main()
