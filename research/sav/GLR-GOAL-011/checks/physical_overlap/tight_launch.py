"""Higher-order Acb certificate for the pinned GOAL-011 launch moments."""
from flint import acb, acb_mat, acb_series, arb, ctx

import certified_launch as base

ORDER = 24


def _centered_integral(density, supremum, left, right, order=ORDER):
    """Integrate a holomorphic density on a real cell using Cauchy."""
    center = (left + right) / 2
    half = (right - left) / 2
    radius = 3 * half
    assert half > 0 and order % 2 == 0
    series = density(acb_series([center, 1], prec=order))
    magnitude = supremum(acb(center) + base.box(radius))
    assert magnitude.is_finite()
    low = sum(2 * series[j] * half**(j+1) / (j+1)
              for j in range(0, order, 2))
    ratio = half / radius
    remainder = (
        2 * half * magnitude * ratio**order
        / ((order+1) * (1-ratio**2))
    ).upper()
    return low + base.box(remainder), remainder


def _bump_sup(z, derivative=False):
    # Re(1/z)=Re(z)/|z|². Bounding the real part directly avoids
    # artificial positive-real exponent bounds in complex division.
    lo, hi = z.real.lower(), z.real.upper()
    imag = base.upper(z.imag)
    assert 0 < lo and hi < 1, z
    lower_real = lo/(hi**2+imag**2) + (1-hi)/((1-lo)**2+imag**2)
    bound = (4-lower_real).exp()
    if derivative:
        reciprocal = 1/lo + 1/(1-hi)
        bound *= base.upper(1-2*z)*reciprocal**2
    return bound.upper()


def normalized_bump():
    """Normalize B(y)=exp(4-1/[y(1-y)]) on [0,1]."""
    cut = arb(1) / 128
    step = (arb(1) / 2 - cut) / 128

    def density(y):
        reciprocal = 1/y + 1/(1-y)
        return ((1-2*y) * (4-reciprocal).exp() * reciprocal**2)**2

    total = arb(0)
    for j in range(128):
        integral, _ = _centered_integral(density, lambda z: _bump_sup(z,True)**2,
                                         cut+j*step, cut+(j+1)*step)
        total += integral.real
    # (B')² <= e^8 y^-4 (1-cut)^-4 exp(-2/y). The RHS increases
    # on 0<y<=cut, so multiply the endpoint by cut.
    missing = (cut**-3 * (8-2/cut).exp() / (1-cut)**4).upper()
    return 1 / (2 * (total + arb(0).union(missing))).sqrt()


def _horner(series, t):
    result = 0
    for coefficient in reversed(series.coeffs()):
        result = result*t + coefficient
    return result


def _moment_on_cell(sigma, weight, weight_sup, fields, start, left, right):
    def amplitude(r):
        f, fx = (_horner(field.series, r-start) for field in fields)
        return fx + sigma*f

    def density(r):
        return weight(r) * amplitude(r)

    def supremum(r):
        return (weight_sup(r) * base.upper(amplitude(r))).upper()

    integral, remainder = _centered_integral(density, supremum, left, right)
    # The quadrature above is for the polynomial. Add the true-field
    # Taylor remainders separately on the real cell.
    inside = max(base.upper(left-start), base.upper(right-start))
    enclosure = acb((left+right)/2 + arb(0, (right-left)/2))
    point_error = fields[1].tail(inside) + base.upper(sigma)*fields[0].tail(inside)
    extra = ((right-left) * weight_sup(enclosure) * point_error).upper()
    return integral + base.box(extra), remainder + extra


def _disk_radius(value):
    return (value.real.rad()**2 + value.imag.rad()**2).sqrt().upper()


def _launch_quotient(moment, wronskian):
    """Enclose -moment/wronskian using midpoint disk perturbations."""
    j0 = acb(moment.real.mid(), moment.imag.mid())
    w0 = acb(wronskian.real.mid(), wronskian.imag.mid())
    c0 = -j0/w0
    w_error = _disk_radius(wronskian)
    lower = abs(w0).lower()-w_error
    assert lower > 0
    radius = ((_disk_radius(moment)+base.upper(c0)*w_error)/lower).upper()
    return c0+base.box(radius), radius


def baseline_horizon(sigma, b_norm, *, diagnostics=None, moments_only=False):
    """Enclose both pinned, full launch coefficients and their Wronskian."""
    op = base.massless(sigma)
    match, left = arb("2.5"), arb("1.125")
    launch, outer_end = arb(13)/8, arb("1.595")
    delta = arb(3)/25
    n, nr = [v.value(acb(left-1)) for v in base.rw_horizon(sigma,40)]
    ef_to_static = (sigma*(left+(left-1).log())).exp()
    h_launch = base.transport(
        op, left, launch, [ef_to_static*n, ef_to_static*(sigma*n+(1-1/left)*nr)], 40
    )
    h, hx = base.transport(op, launch, match, h_launch, 64)
    g_slope = base.spec.compact_bound(sigma,False,True,base.spec.CELLS,False)[0][0]
    g_launch = base.transport(op, match, launch, [acb(1),g_slope], 64)
    wronskian = h*g_slope-hx
    if not moments_only:
        assert wronskian.abs_lower()>0

    u_start = -launch-2*(launch-1).log()

    def y(r):
        return (-r-2*(r-1).log()-u_start)/delta

    def regular_weight(r):
        return (sigma*(r-1).log()).exp() * (r+1)/(r-1) * delta.sqrt()

    def weight(r):
        z = y(r)
        return regular_weight(r) * (4-1/z-1/(1-z)).exp()

    def weight_sup(r):
        return (base.upper(regular_weight(r)) * _bump_sup(y(r))).upper()

    # Rational interior cutoffs. The omitted pieces have y or 1-y
    # below 1/120 and use a separate exponentially small bound.
    cut_left, cut_right = arb("1.59714"), arb("1.62478")
    small = arb(1)/120
    assert y(outer_end)>1
    assert 0 < y(cut_right) < small and 0 < 1-y(cut_left) < small

    h_moment, g_moment = acb(0), acb(0)
    total_quadrature_error = arb(0)
    states = [h_launch,g_launch]
    patch_step = (outer_end-launch)/4
    for j in range(4):
        start = launch+j*patch_step
        end = start+patch_step
        fields = [base.advance(op,start,patch_step,state) for state in states]
        low, high = max(cut_left,end), min(cut_right,start)
        if low < high:
            step = (high-low)/64
            for i in range(64):
                a, b = low+i*step, low+(i+1)*step
                for k, pair in enumerate(fields):
                    integral, error = _moment_on_cell(sigma,weight,weight_sup,pair,start,a,b)
                    if k==0:
                        h_moment += integral
                    else:
                        g_moment += integral
                    total_quadrature_error += error
        states = [[v.value(acb(patch_step)) for v in pair] for pair in fields]

    span = launch-outer_end
    support = acb((launch+outer_end)/2 + arb(0,span/2))
    operator_norm = base.norm_matrix(acb_mat(op(support)))
    omitted_factor = (
        span * base.upper(regular_weight(support)) * (4-1/small).exp()
        * (1+base.upper(sigma)) * (span*operator_norm).exp()
    ).upper()
    omitted = [(omitted_factor*max(base.upper(v) for v in state)).upper()
               for state in (h_launch,g_launch)]
    h_moment = b_norm*(h_moment+base.box(omitted[0]))
    g_moment = b_norm*(g_moment+base.box(omitted[1]))
    if moments_only:
        return h_moment,g_moment
    launch_left, left_disk = _launch_quotient(g_moment,wronskian)
    launch_right, right_disk = _launch_quotient(h_moment,wronskian)
    if diagnostics is not None:
        diagnostics.update(g_slope=g_slope, h_match=h, hx_match=hx,
                           h_moment=h_moment, g_moment=g_moment,
                           quadrature_error=total_quadrature_error.upper(),
                           omitted_bound=max(omitted),
                           left_quotient_disk=left_disk,
                           right_quotient_disk=right_disk)
    return launch_left, launch_right, wronskian


def inputs(*, diagnostics=None):
    """Return the same dataclass and physical inputs as certified_launch."""
    s = base.spec.shift(base.spec.CENTER, arb("5e-8"))
    omega, k = acb(base.spec.OMEGA), acb(base.spec.K)
    b_norm = normalized_bump()
    scale, match = base.gl_scale(k,omega)
    launch_left, launch_right, w = baseline_horizon(s-2*omega,b_norm,
                                                   diagnostics=diagnostics)
    return base.Inputs(s,omega,k,b_norm,scale,match,launch_left,launch_right,w)


if __name__ == "__main__":
    ctx.prec = 128
    ctx.cap = 80
    diagnostics = {}
    result = inputs(diagnostics=diagnostics)
    for name in ("bump_normalization","baseline_w","launch_left","launch_right"):
        value = getattr(result,name)
        real_radius, imag_radius = value.real.rad(), value.imag.rad()
        disk_radius = _disk_radius(value)
        print(name, "center =", value.real.mid(), value.imag.mid())
        print("  Cartesian radii", real_radius, imag_radius)
        print("  Euclidean radius", disk_radius)
        if name.startswith("launch_"):
            assert disk_radius < arb("2e-6")
    assert result.bump_normalization.rad() < arb("6e-14")
    for name,value in diagnostics.items():
        print(name, "=", value)
