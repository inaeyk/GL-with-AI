"""Whole-cell test and endpoint certificates for the sharp GOAL-014 cuts."""
import argparse
import platform

import flint
from flint import arb, ctx


def jets(y):
    """The unnormalized bump and its first three derivatives."""
    s = 1 / (y * (1 - y))
    q = 1 - 2 * y
    q2 = q * q
    log1 = q * s**2
    log2 = -2 * s**2 - 2 * q2 * s**3
    log3 = 12 * q * s**3 + 6 * q * q2 * s**4
    b = (-s).exp()
    return b, b * log1, b * (log1 * log1 + log2), b * (
        log1 * log1 * log1 + 3 * log1 * log2 + log3
    )


def radius(u):
    return 1 + 2 * ((-(1 + u) / 2).exp() / 2).lambertw()


def mass(r):
    return 3 / r - 3 / (4 * r**2)


def mass_y(r, delta):
    return delta * (3 / r**2 - 3 / (2 * r**3)) * (r - 1) / (r + 1)


def symmetric(cap):
    return (-cap.abs_upper()).union(cap.abs_upper())


def emit(name, bounds):
    print(f'{name}: {bounds.lower().str(17)} {bounds.upper().str(17)}')



def early_jet_bounds():
    """Relative analytic error of the free outgoing compact candidate."""
    K = arb(2)/9
    M = arb('1.57')
    rate, second_rate = 522, 380
    C = 1/(1-M/rate)
    assert (1-2/((arb(3)/4)**2*(arb(64)/7)**2)-2/(arb(64)/7))*rate > second_rate
    coefficients = (
        ('|w-B|/B', C*M/rate),
        ('|w_u-Bprime|/Bprime', C*M/rate),
        ('|w_v|/Bprime', K*C/rate**2),
        ('|w_uu-Bsecond|/Bsecond',
         (M+(arb('.24')*K+M*C/4+M*M*C)/rate)/second_rate),
        ('|w_uv|/Bsecond', K*C/(rate*second_rate)),
        ('|w_vv|/Bsecond',
         (arb('4.23')*K+K*C/(4*rate)+K*K*C/rate**2)/(rate*second_rate)),
    )
    print('whole early causal strip jet bounds')
    for name, coefficient in coefficients:
        emit('  '+name+' upper', coefficient.upper())


def run(cells):
    delta = arb(3) / 25
    launch = arb(13) / 8
    a = -launch - 2 * (launch - 1).log()
    M = arb(161) / 100
    assert radius(a + delta) > arb('1.59') and mass(radius(a + delta)) < M
    h = arb(1) / cells
    assert h.is_exact()
    flat = 512 * arb(cells)**8 * (-arb(cells)).exp()
    flat_jets = (arb(0).union(flat.upper()),) + (symmetric(flat),) * 3
    partial_primitive = primitive_error = area = moment = norm_sq = arb(0)
    err_sq = err_grad_sq = grad_abs = test_abs = arb(0)
    records = {}
    r_left = launch
    for i in range(cells):
        left, right = h * i, h * (i + 1)
        middle = (left + right) / 2
        b, b1, b2, b3 = flat_jets if i in (0, cells - 1) else jets(left.union(right))
        bm, b1m, _, _ = jets(middle)
        r_right = radius(a + delta * right)
        r = r_right.union(r_left)
        r_left = r_right
        J = mass(r)
        c = (mass(radius(a + delta * middle))**2 * bm).mid()
        slope_error = (c - J**2 * b).abs_upper()
        partial_primitive += h * c
        primitive_error = (primitive_error + h * slope_error).upper()
        moment = (moment + h * area + h**2 * b.upper() / 2).upper()
        area = (area + h * b.upper()).upper()
        exp_weight = (delta * M * right).exp()
        dW = delta * M * exp_weight * area
        remainder_W = (delta * M)**2 * exp_weight * moment

        # Second-scatter diagonal defect, higher scatter, dyadic primitive.
        kernel_error = delta**3 * M**2 * (delta * right / 4).exp() / 8
        value_error = (kernel_error * moment + delta * M * remainder_W
                       + delta**2 * primitive_error / 2).upper()
        grad_error = (kernel_error * area + delta**2 * M * remainder_W / 4
                      + delta**2 * M**2 * dW + delta**2 * slope_error / 2).upper()

        candidate_grad = b2 - delta * (mass_y(r, delta) * b + J * b1) + delta**2 * c / 2
        # The dyadic primitive is increasing. Its start lies h*c below its end.
        primitive_span = (partial_primitive - h * c).union(partial_primitive)
        candidate = b1 - delta * J * b + delta**2 * primitive_span / 2
        err_sq = (err_sq + h * value_error**2).upper()
        err_grad_sq = (err_grad_sq + h * grad_error**2).upper()
        test_abs = (test_abs + h * (candidate.abs_upper() + value_error)).upper()
        grad_abs = (grad_abs + h * (candidate_grad.abs_upper() + grad_error)).upper()

        curvature = 2 * (b2 * b2 + b1 * b3)
        norm_sq += h * b1m * b1m + symmetric(h**3 * curvature / 24)
        if i + 1 in (cells // 8, cells):
            end_b, end_b1 = (arb(0), arb(0)) if i + 1 == cells else jets(right)[:2]
            end_candidate = end_b1 - delta * mass(r_right) * end_b + delta**2 * partial_primitive / 2
            records[i + 1] = (
                err_sq, err_grad_sq, test_abs, grad_abs, end_candidate, value_error,
            )

    assert norm_sq > 0
    B = norm_sq.sqrt()
    emit('normalizer B', B)
    for label, key in [('early', cells // 8), ('packet', cells)]:
        err, err_grad, l1, derivative_l1, end, end_err = records[key]
        print(label)
        emit('  L2 test error upper', (err.sqrt() / B).upper())
        emit('  L2 derivative error upper', (err_grad.sqrt() / (delta * B)).upper())
        emit('  L1 test norm upper', (delta.sqrt() * l1 / B).upper())
        emit('  L1 derivative norm upper', (derivative_l1 / (delta.sqrt() * B)).upper())
        emit('  terminal test', (end + symmetric(end_err)) / (delta.sqrt() * B))
        emit('  terminal test error upper', (end_err / (delta.sqrt() * B)).upper())


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mesh-power', type=int, default=15, choices=range(10, 17))
    parser.add_argument('--precision', type=int, default=160, choices=(128, 160, 192))
    args = parser.parse_args()
    ctx.prec = args.precision
    print(f'Python {platform.python_version()}, python-flint {flint.__version__}, '
          f'Arb precision {ctx.prec}, dyadic mesh 2^{args.mesh_power}')
    run(2**args.mesh_power)
    early_jet_bounds()
