"""Non-production Arb enclosures for the fixed-width flux test.

Run from research/sav with python-flint 0.9.0. No project code is imported.
The enclosed projection uses the exact causal RW baseline and a frozen
optical returning reference; the physical returning residual is not solved.
"""
from fractions import Fraction
import sys

import flint
from flint import arb, ctx


ctx.prec = 128
CELLS = 32768
DELTA = arb(3) / 25
RADIUS = arb(13) / 8
POTENTIAL_CAP = arb(161) / 100
J_LIPSCHITZ = arb(3) / 10
K_LO = arb(257) / 1000
K_HI = arb(304) / 1000
K_BOX = K_LO.union(K_HI)
J_LAUNCH = 3 / RADIUS - 3 / (4 * RADIUS**2)


def absolute(value):
    return value.abs_lower().union(value.abs_upper())


def normalized_box(center, radius):
    return center + arb(0, radius.upper())


def bump_jet(y):
    p = y * (1 - y)
    q = 1 - 2 * y
    inverse = 1 / p
    b = (-inverse).exp()
    first = b * q * inverse**2
    second = b * (q**2 - 2 * p**2 - 2 * q**2 * p) * inverse**4
    third = b * q * (1 - 10 * p + 24 * p**2 - 12 * p**3) * inverse**6
    return b, first, second, third


def certified_geometric_caps():
    def cut(r):
        return -r - 2 * (r - 1).log()

    r_min = arb(159) / 100
    assert cut(r_min) > cut(RADIUS) + DELTA
    assert 3 / r_min - 3 / (4 * r_min**2) < POTENTIAL_CAP
    # J'(u)=3(2r-1)(r-1)/(2r^3(r+1)). Bound each factor over the strip.
    assert 3 * (2 * RADIUS - 1) * (RADIUS - 1) / (2 * r_min**3 * (r_min + 1)) < J_LIPSCHITZ
    print('u*', cut(RADIUS).str(20), 'full right', (cut(RADIUS) + DELTA).str(20),
          'early right', (cut(RADIUS) + DELTA / 8).str(20))


def integrate():
    # This routine works with the raw bump. The normalization divides each
    # integral of a product by integral(raw b'^2), or by its square root for L1.
    step = arb(1) / CELLS
    tail_jet = 6 * arb(CELLS)**6 * (-arb(CELLS)).exp()
    tail_integral = step * tail_jet
    acc_b = tail_integral.upper()
    names = ('bp2', 'bpp2', 'lead', 'center_proj', 'proj_error', 'center_norm2',
             'err_norm2', 'center_l1', 'err_l1')
    totals = dict.fromkeys(names, arb(0))
    early = None
    for i in range(1, CELLS - 1):
        y_lo = i * step
        y_hi = (i + 1) * step
        y = y_lo.union(y_hi)
        b, bp, bpp, bppp = bump_jet(y)
        acc_b = (acc_b + step * b.upper()).upper()
        center = bp - DELTA * J_LAUNCH * b
        error = DELTA**2 * (y_hi * J_LIPSCHITZ * b
                            + POTENTIAL_CAP**2 * (DELTA * POTENTIAL_CAP * y_hi).exp() * acc_b)
        integrands = (bp**2, bpp**2, bp * bppp, center * bppp, error * absolute(bppp),
                      center**2, error**2, absolute(center), error)
        for name, value in zip(names, integrands):
            totals[name] = totals[name] + step * value
        if i + 1 == CELLS // 8:
            early = dict(totals)

    # For each omitted endpoint cell, p^-1 >= CELLS. All raw bump
    # jets through order three are bounded by tail_jet. g1 is bounded
    # by 2 tail_jet. Use a global bound of .001 on the raw error (see below).
    assert DELTA**2 * (J_LIPSCHITZ * (-arb(4)).exp()
                        + POTENTIAL_CAP**2 * (DELTA * POTENTIAL_CAP).exp() * (-arb(4)).exp()) < arb('0.001')
    left_error = DELTA**2 * step * tail_jet * (
        J_LIPSCHITZ + POTENTIAL_CAP**2 * (DELTA * POTENTIAL_CAP * step).exp())
    right_error = arb('0.001')
    for record, caps in ((totals, (left_error, right_error)), (early, (left_error,))):
        for error in caps:
            bounds = (tail_jet**2, tail_jet**2, tail_jet**2, 2 * tail_jet**2,
                      error * tail_jet, 4 * tail_jet**2, error**2, 2 * tail_jet, error)
            for name, value in zip(names, bounds):
                record[name] = normalized_box(record[name], step * value)
    return totals, early


def report(label, record, normalization, sign):
    lead = -record['bpp2'] / normalization if sign < 0 else record['lead'] / normalization
    projection = normalized_box(record['center_proj'], record['proj_error']) / normalization
    center_l2 = (record['center_norm2'] / normalization).sqrt()
    center_l1 = record['center_l1'] / normalization.sqrt()
    error_l2 = (record['err_norm2'] / normalization).sqrt()
    error_l1 = record['err_l1'] / normalization.sqrt()
    g_l2 = normalized_box(center_l2, error_l2)
    g_l1 = normalized_box(center_l1, error_l1)
    # E_{22,T}=12r0/(16pi G4) times this projection divided by delta^2.
    assert sign * lead > 0 and sign * projection > 0
    assert g_l2 > 0 and g_l1 > 0
    values = {'uncorrected leading ∫b\'b\'\'\'': lead,
              'one-scatter ∫g1 b\'\'\'': record['center_proj'] / normalization,
              '∫(g-g1)b\'\'\' absolute cap': record['proj_error'].upper() / normalization.lower(),
              'exact-baseline ∫g b\'\'\'': projection,
              'leading P': lead * K_BOX / DELTA**2,
              'exact-baseline optical P': projection * K_BOX / DELTA**2,
              '||g||2': g_l2, '||g||1': g_l1,
              'sharp L2 threshold for r': K_BOX * sign * projection / g_l2,
              'sharp sup threshold for r': K_BOX * sign * projection / g_l1,
              'sharp sup threshold for B2': K_BOX * sign * projection / (g_l1 * DELTA)}
    print(label)
    for name, result in values.items():
        print(f'  {name}: [{result.lower().str(16)}, {result.upper().str(16)}]')


def main():
    print(f'Python {sys.version.split()[0]}, python-flint {flint.__version__}, '
          f'{ctx.prec} bits, {CELLS} cells')
    certified_geometric_caps()
    full, early = integrate()
    normalization = full['bp2']
    print('raw normalization ∫raw b\'^2:', normalization.str(16))
    report('full [0,1]', full, normalization, -1)
    report(f'early [0,{Fraction(1, 8)}]', early, normalization, 1)


if __name__ == '__main__':
    main()
