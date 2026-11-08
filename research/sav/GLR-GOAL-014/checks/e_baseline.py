"""Directed independent leading-test endpoint and H1 checks, non-production."""

from argparse import ArgumentParser
from platform import python_version

import flint
from flint import arb, ctx


def positive(x):
    return arb(0).union(x.upper())


def symmetric(x):
    x = x.abs_upper()
    return (-x).union(x)


def bump(y):
    q = 1 / (y * (1 - y))
    ell = (1 - 2 * y) * q**2
    ell_y = -2 * q**2 - 2 * (1 - 2 * y)**2 * q**3
    value = (-q).exp()
    return value, value * ell, value * (ell**2 + ell_y)


def radius(u):
    return 1 + 2 * ((-(1 + u) / 2).exp() / 2).lambertw()


def ray_mass(r):
    return 3 / r - 3 / (4 * r**2)


def ray_mass_u(r):
    return (3 / r**2 - 3 / (2 * r**3)) * (r - 1) / (r + 1)


def nonnegative_square(value):
    return value.abs_lower()**2, value.abs_upper()**2


def report(label, value):
    print(f"{label}: [{value.lower().str(12)}, {value.upper().str(12)}]")


def integrate(cells):
    delta = arb(3) / 25
    root_delta = delta.sqrt()
    anchor = arb(13) / 8
    a = -anchor - 2 * (anchor - 1).log()
    step = arb(1) / cells
    endpoint_bound = (-arb(cells)).exp()
    flat = (positive(endpoint_bound), symmetric(cells**2 * endpoint_bound),
            symmetric((cells**4 + 2 * cells**3 + 2 * cells**2) * endpoint_bound))
    norm_lo, norm_hi = arb(0), arb(0)
    norms = {'early': [arb(0), arb(0), arb(0), arb(0)],
             'packet': [arb(0), arb(0), arb(0), arb(0)]}
    raw_area, raw_moment = arb(0), arb(0)
    # The same diagonal continuum candidate is used on both cuts. Interval
    # range quadrature here also encloses its derivative and endpoint values.
    cumulative = arb(0)
    previous = anchor
    m_global = arb('1.60')
    log_slope = arb(1) / 4
    for y, r_bound, mass_bound in ((arb(1) / 8, arb('1.62'), arb('1.57')),
                                  (arb(1), arb('1.59'), m_global)):
        assert radius(a + delta * y) > r_bound
        assert ray_mass(r_bound) < mass_bound
    ends = {}
    for i in range(cells):
        left = i * step
        right = (i + 1) * step
        next_radius = radius(a + delta * right)
        r = next_radius.union(previous)
        j = ray_mass(next_radius).union(ray_mass(previous))
        previous = next_radius
        val, diff, diff2 = flat if i in (0, cells - 1) else bump(left.union(right))
        lo, hi = nonnegative_square(diff)
        norm_lo += step * lo
        norm_hi += step * hi
        shear = root_delta * val
        slope = diff / root_delta
        curvature = diff2 / (delta * root_delta)
        du = delta * step
        diagonal = j**2 * shear / 2
        now = cumulative + positive(du) * diagonal
        g = slope - j * shear + now
        gp = curvature - ray_mass_u(r) * shear - j * slope + diagonal
        raw_moment = (raw_moment + du * raw_area + du**2 * shear.abs_upper() / 2).upper()
        raw_area = (raw_area + du * shear.abs_upper()).upper()
        m = arb('1.57') if i < cells // 8 else m_global
        width = right * delta
        third = m**3 * (m * width).exp()
        lip = log_slope * (log_slope * width).exp() * m**2 / 2
        defect = (third + lip) * raw_moment
        d_defect = (lip + third / 2) * raw_area + log_slope * third * raw_moment
        for name in ('packet', 'early') if i < cells // 8 else ('packet',):
            terms = norms[name]
            terms[0] += du * (g + symmetric(defect)).abs_upper()**2
            terms[1] += du * (gp + symmetric(d_defect)).abs_upper()**2
            terms[2] += du * defect.abs_upper()**2
            terms[3] += du * d_defect.abs_upper()**2
        cumulative += du * diagonal
        if i + 1 in (cells // 8, cells):
            raw_end = bump(right) if i + 1 == cells // 8 else (arb(0),) * 3
            b0, b1, b2 = raw_end
            b0, b1, b2 = root_delta * b0, b1 / root_delta, b2 / (delta * root_delta)
            end_j = ray_mass(next_radius)
            g_end = b1 - end_j * b0 + cumulative + symmetric(defect)
            gp_end = (b2 - ray_mass_u(next_radius) * b0 - end_j * b1
                      + end_j**2 * b0 / 2 + symmetric(d_defect))
            ends['early' if i + 1 == cells // 8 else 'packet'] = (g_end, gp_end)
    norm = norm_lo.lower().union(norm_hi.upper()).sqrt()
    report('Bump normalization integral square root', norm)
    for name, (g, gp) in ends.items():
        print(name)
        assert g > 0
        report('  physical N01(d)', g / norm)
        report("  physical N01'(d)", gp / norm)
        for label, square in zip(('physical N01 L2 upper', "physical N01' L2 upper",
                                  'two-scatter L2 defect upper', "two-scatter derivative L2 defect upper"),
                                 norms[name]):
            upper = (square.sqrt() / norm).upper()
            print(f'  {label}: {upper.str(12)}')


if __name__ == '__main__':
    parser = ArgumentParser(description=__doc__)
    parser.add_argument('--mesh-power', type=int, default=13, choices=range(8, 17))
    parser.add_argument('--precision', type=int, default=160, choices=(128, 160, 192))
    args = parser.parse_args()
    ctx.prec = args.precision
    print(f'Python {python_version()}, python-flint {flint.__version__}, '
          f'Arb precision {ctx.prec}, dyadic mesh 2^{args.mesh_power}')
    integrate(2**args.mesh_power)
