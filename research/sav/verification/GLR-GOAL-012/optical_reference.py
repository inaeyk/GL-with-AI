"""Independent directed optical check for GLR-GOAL-012.

Uses a variable first Volterra iterate and an analytic scatter remainder.
Requires python-flint. No source routines or physical evolutions are used.
"""
import platform

import flint
from flint import arb, ctx


ctx.prec = 128
cells = 32768
delta = arb(3) / 25
launch_radius = arb(13) / 8
launch_cut = -launch_radius - 2 * (launch_radius - 1).log()
ray_cap = arb(161) / 100
k_low = arb(257) / 1000
k_high = arb(304) / 1000
width = arb(1) / cells
flat_cap = 10 * arb(cells)**6 * (-arb(cells)).exp()


def ray_mass(y):
    """u=-r-2log(r-1); r=1+2W(exp(-(1+u)/2)/2)."""
    z = (-(1 + launch_cut + delta * y) / 2).exp() / 2
    r = 1 + 2 * z.lambertw()
    return 3 / r - 3 / (4 * r**2)


def bump(cell, index):
    if index in (0, cells - 1):
        signed = (-flat_cap).union(flat_cap)
        return arb(0).union(flat_cap), signed, signed, signed
    p = cell * (1 - cell)
    t = 1 / p
    dp = 1 - 2 * cell
    log1 = dp * t**2
    log2 = -2 * t**2 - 2 * dp * dp * t**3
    log3 = 12 * dp * t**3 + 6 * dp * dp * dp * t**4
    value = (-t).exp()
    return (
        value,
        value * log1,
        value * (log2 + log1 * log1),
        value * (log3 + 3 * log1 * log2 + log1 * log1 * log1),
    )


def integrate():
    accumulators = [arb(0) for _ in range(9)]
    early = None
    previous_mass = ray_mass(arb(0))
    weighted_mass_upper = arb(0)
    assert ray_mass(arb(1)) < ray_cap
    for index in range(cells):
        y_left, y_right = index * width, (index + 1) * width
        y = y_left.union(y_right)
        next_mass = ray_mass(y_right)
        mass = previous_mass.union(next_mass)
        previous_mass = next_mass
        b, d1, d2, d3 = bump(y, index)
        one_scatter = d1 - delta * mass * b
        weighted_mass_upper = (
            weighted_mass_upper + width * ((-delta * ray_cap * y).exp() * b).upper()
        ).upper()
        envelope = (
            delta**2 * mass * ray_cap * (delta * ray_cap * y_right).exp()
            * weighted_mass_upper
        ).upper()
        cell_values = (
            d1 * d1, d2 * d2, d1 * d3, -delta * mass * b * d3,
            envelope * d3.abs_upper(), one_scatter * one_scatter, envelope**2,
            one_scatter.abs_upper(), envelope,
        )
        accumulators = [
            total + width * value for total, value in zip(accumulators, cell_values)
        ]
        if index + 1 == cells // 8:
            early = list(accumulators)
    return accumulators, early


def report(label, record, norm2, full=False):
    _, d2sq, unscattered, change, projection_error, g1sq, error_sq, g1abs, error_abs = record
    raw_center = (-d2sq if full else unscattered) + change
    center = raw_center / norm2
    error = projection_error.upper() / norm2
    projection = center + (-error).union(error)
    assert projection < 0 if full else projection > 0
    flux = projection * k_low.union(k_high) / delta**2
    l2_upper = ((g1sq / norm2).sqrt() + (error_sq / norm2).sqrt()).upper()
    l1_upper = ((g1abs + error_abs) / norm2.sqrt()).upper()
    min_mag = projection.abs_lower()
    print(label)
    for name, result in (
        ("unscattered projection", (-d2sq if full else unscattered) / norm2),
        ("first scatter projection", center),
        ("absolute higher scatter error cap", error),
        ("exact-baseline optical projection", projection),
        ("optical flux", flux),
        ("sufficient L2 tolerance (strict lower bound)", k_low * min_mag / l2_upper),
        ("sufficient sup tolerance (strict lower bound)", k_low * min_mag / l1_upper),
    ):
        print(f"  {name}: [{result.lower().str(17)}, {result.upper().str(17)}]")


if __name__ == "__main__":
    print(f"Python {platform.python_version()}, python-flint {flint.__version__}, {ctx.prec} bits")
    print(f"cells {cells}, launch cut {launch_cut.str(20)}, ray mass ends",
          ray_mass(arb(0)).str(17), ray_mass(arb(1)).str(17))
    full, early = integrate()
    norm2 = full[0]
    print("normalizing bump multiplier", (1 / norm2.sqrt()).str(12))
    report("full", full, norm2, full=True)
    report("early", early, norm2)
