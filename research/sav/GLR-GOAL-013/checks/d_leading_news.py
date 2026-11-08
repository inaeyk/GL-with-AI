"""Directed two-scatter approximation to the fixed GOAL-013 leading news.

The second-scatter kernel is replaced by its diagonal ray value. Bounds for
this replacement, the remaining exact Volterra terms and all quadratures are
in FLUX.md. This script uses no historical numerical routines.
"""
import argparse
import platform

import flint
from flint import arb, ctx


def bump_jet(y):
    reciprocal = 1 / (y * (1 - y))
    q = 1 - 2 * y
    l1 = q * reciprocal**2
    qq = q * q
    l2 = -2 * reciprocal**2 - 2 * qq * reciprocal**3
    l3 = 12 * q * reciprocal**3 + 6 * q * qq * reciprocal**4
    l4 = -24 * reciprocal**3 - 72 * qq * reciprocal**4 - 24 * qq * qq * reciprocal**5
    l11 = l1 * l1
    b = (-reciprocal).exp()
    return (
        b, b * l1, b * (l2 + l11), b * (l3 + 3 * l1 * l2 + l1 * l11),
        b * (l4 + 4 * l1 * l3 + 3 * l2 * l2 + 6 * l11 * l2 + l11 * l11),
    )


def radius(u):
    return 1 + 2 * ((-(1 + u) / 2).exp() / 2).lambertw()


def ray_mass(r):
    return 3 / r - 3 / (4 * r**2)


def positive_cap(value):
    return arb(0).union(value.upper())


def symmetric_cap(value):
    value = value.abs_upper()
    return (-value).union(value)


def midpoint_integral(value, second_derivative, step):
    return step * value + symmetric_cap(second_derivative * step**3 / 24)


def report(name, value):
    print(f"{name}: [{value.lower().str(16)}, {value.upper().str(16)}]")


def integrate(cells):
    delta = arb(3) / 25
    r_launch = arb(13) / 8
    u_launch = -r_launch - 2 * (r_launch - 1).log()
    r_last = radius(u_launch + delta)
    mass_limit = arb(161) / 100
    assert r_last > arb('1.59') and ray_mass(r_last) < mass_limit
    report('launch cut', u_launch)
    report('minimum causal radius', r_last)
    width = arb(1) / cells
    assert width.is_exact()
    flat_cap = 512 * arb(cells)**8 * (-arb(cells)).exp()
    flat_bounds = (positive_cap(flat_cap),) + (symmetric_cap(flat_cap),) * 4

    # C approximates the integral of J^2 b_raw using the midpoint on each cell.
    # C_error encloses its error from preceding cells; the current fractional
    # cell is controlled by the range about the midpoint.
    cumulative = arb(0)
    cumulative_error = arb(0)
    bump_area = arb(0)
    bump_moment = arb(0)
    normalization = arb(0)
    grad2_sq = arb(0)
    approximation_sq = arb(0)
    approximation_abs = arb(0)
    approximation_error_sq = arb(0)
    approximation_error_abs = arb(0)
    optical_correction = arb(0)
    optical_error = arb(0)
    early = None

    previous_mass = ray_mass(r_launch)
    for cell in range(cells):
        left = width * cell
        right = left + width
        midpoint = left + width / 2
        span = left.union(right)
        b, d1, d2, d3, d4 = (
            flat_bounds if cell in (0, cells - 1) else bump_jet(span)
        )
        b_mid, d1_mid, d2_mid, _, _ = bump_jet(midpoint)
        next_mass = ray_mass(radius(u_launch + delta * right))
        mass = previous_mass.union(next_mass)
        previous_mass = next_mass
        # Pick a single exact dyadic (Arb midpoint) for the reproducible candidate.
        integrand_mid = (b_mid * ray_mass(radius(u_launch + delta * midpoint))**2).mid()
        integrand = b * mass**2
        candidate_cumulative = cumulative + arb(0).union(width) * integrand_mid
        correction = -delta * mass * b + delta**2 / 2 * candidate_cumulative
        candidate = d1 + correction

        # Advance positive upper bounds for int_0^right b and int_0^right (right-s)b.
        bump_moment = (bump_moment + width * bump_area + width**2 / 2 * b.upper()).upper()
        bump_area = (bump_area + width * b.upper()).upper()
        volterra_error = delta**3 * mass_limit**2 * (
            (delta * right / 4).exp() / 8 + mass_limit * (delta * mass_limit * right).exp()
        ) * bump_moment
        step_error = width * (integrand - integrand_mid).abs_upper()
        cumulative_error = (cumulative_error + step_error).upper()
        raw_error = (volterra_error + delta**2 / 2 * cumulative_error).upper()
        cumulative += width * integrand_mid

        normalization += midpoint_integral(d1_mid * d1_mid, 2 * (d2 * d2 + d1 * d3), width)
        grad2_sq += midpoint_integral(d2_mid * d2_mid, 2 * (d3 * d3 + d2 * d4), width)
        approximation_sq += width * (candidate * candidate)
        approximation_abs += width * abs(candidate)
        approximation_error_sq += width * raw_error * raw_error
        approximation_error_abs += width * raw_error
        optical_correction += width * correction * d3
        optical_error += width * raw_error * d3.abs_upper()
        if cell + 1 in (cells // 8, cells):
            end_jet = bump_jet(right) if cell + 1 == cells // 8 else (arb(0),) * 5
            record = (
                end_jet[1] * end_jet[2] - grad2_sq, approximation_sq, approximation_abs,
                approximation_error_sq, approximation_error_abs, optical_correction, optical_error,
            )
            if cell + 1 == cells // 8:
                early = record

    assert early is not None and normalization > 0
    report('normalizing multiplier', 1 / normalization.sqrt())
    for label, item in (('packet', record), ('early', early)):
        unscattered, approx_sq, approx_abs, err_sq, err_abs, correction, opt_err = item
        candidate_norm = (approx_sq / normalization).sqrt()
        error_l2 = (err_sq / normalization).sqrt().upper()
        upper_l1 = ((approx_abs + err_abs.upper()) / normalization.sqrt()).upper()
        exact_norm = candidate_norm + symmetric_cap(error_l2)
        projection = (unscattered + correction + symmetric_cap(opt_err)) / normalization
        delay = arb('0.257').union(arb('0.304'))
        assert projection < 0 if label == 'packet' else projection > 0
        print(label)
        report('  leading-news approximation norm', candidate_norm)
        report('  physical leading-news L2 error cap', error_l2)
        report('  exact leading-news L2 norm', exact_norm)
        report('  optical projection against exact leading news', delay * projection / delta**2)
        report('  sufficient L2 tolerance of scaled return error', arb('0.257') * projection.abs_lower() / exact_norm.upper())
        report('  sufficient L-infinity tolerance of scaled return error', arb('0.257') * projection.abs_lower() / upper_l1)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mesh-power', type=int, default=15, choices=range(8, 18))
    parser.add_argument('--precision', type=int, default=160, choices=(128, 160, 192))
    args = parser.parse_args()
    ctx.prec = args.precision
    print(f'Python {platform.python_version()}, python-flint {flint.__version__}, '
          f'Arb precision {ctx.prec}, dyadic mesh 2^{args.mesh_power}')
    integrate(2**args.mesh_power)
