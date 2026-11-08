"""Directed physical mixed graph jet and its uncommuted incoming energy."""

import argparse
import sys
from pathlib import Path

from flint import acb, arb, ctx

sys.path.insert(0, str(Path(__file__).resolve().parents[2] /
                       "GLR-GOAL-011/checks/physical_overlap"))
from certified_launch import advance, gl_regular, normalized_bump, transport
from outer_gljet import njet


def operator(k, omega):
    def evaluate(r):
        t = r - 1
        d = 1 + k*k*r**3
        c = k*k*r**3 + 6*r - 11 + 18*t/d**2 - 6*(4*r - 5)/d
        return [[0, 1], [c/(r*r*t), -(1/r + 2*omega*r)/t]]
    return evaluate


def profile_and_slope(y):
    lo, hi = y.lower(), y.upper()
    if hi <= 0 or lo >= 1:
        return arb(0), arb(0)
    if lo > 0 and hi < 1:
        inverse = 1/(y*(1-y))
        b = (4-inverse).exp()
        return b, (1-2*y)*inverse**2*b
    if lo <= 0 and hi < arb(1)/2:
        closest = hi
    elif hi >= 1 and lo > arb(1)/2:
        closest = 1-lo
    else:
        raise ValueError("subdivide packet cells crossing an endpoint")
    t_min = 1/(closest*(1-closest))
    b_cap = (4-t_min).exp().upper()
    slope_cap = (t_min**2*(4-t_min).exp()).upper()
    return arb(0).union(b_cap), arb(0, slope_cap)


def velocities(r, n, nr, k, omega, norm, a, delta):
    f = 1-1/r
    p = -1/r
    u = -r-2*(r-1).log()
    b, by = profile_and_slope((u-a)/delta)
    a0 = norm*delta.sqrt()*b
    a1 = norm/delta.sqrt()*by
    at = (f*a0-r*a1)/(2*r*r)
    ax = a1/(2*r)
    vv, vr, rr, _ = njet(r, k, omega, n.real, nr.real)[0]
    factor = (omega*r).exp()/2
    ht = factor*vv
    htx = factor*(vv+f*vr)
    hx = factor*(vv+2*f*vr+f*f*rr)
    # TT trace cancels the density/radius variation in M_1.
    # The two raised inverse-metric factors carry signs (-,+) in (t,x).
    jt = -4*r*r*(ht*at-htx*ax)/f
    jx = -4*r*r*(-htx*at+hx*ax)/f
    ut = (p*jt-jx)/(1-p*p)
    az_t_imag = k*(jt-p*jx)/(4*r*r*(1-p*p))
    d_k = 4+k*k*r*r
    energy_dr = (1-p*p)/(2*f)*(ut*ut/(r*r*d_k) +
                                4*r**4/d_k*az_t_imag*az_t_imag)
    return ut, az_t_imag, energy_dr


def main(cells, bits):
    ctx.prec = bits
    ctx.cap = 80
    k = arb.pi()/5
    omega = arb("0.06328119", "0.00000001")
    op = operator(k, omega)
    start = arb(9)/8
    left, right = arb("1.595"), arb(13)/8
    delta = arb(3)/25
    a = -right-2*(right-1).log()
    assert -left-2*(left-1).log() > a+delta

    regular = gl_regular(k, omega, acb(1), 40)
    initial = [value.value(acb(start-1)) for value in regular]
    n2, nr2 = transport(op, start, arb(2), initial, 64)
    angular = njet(arb(2), k, omega, n2.real, nr2.real)[0][3]
    scale = 8/(angular*(2*omega).exp())
    state = transport(op, start, left, [scale*x for x in initial], 48)
    norm = normalized_bump()
    step = (right-left)/cells
    total = arb(0)
    point = None
    for j in range(cells):
        r = left+j*step
        local = advance(op, r, step, state, degree=20)
        slab = arb(0).union(step)
        n, nr = [value.value(acb(slab)) for value in local]
        *_, density = velocities(r+slab, n, nr, k, omega, norm, a, delta)
        total += density*step
        if j == cells//2:
            point = velocities(r, *state, k, omega, norm, a, delta)[:2]
        state = [value.value(acb(step)) for value in local]

    print("whole radial cells, bits", cells, bits)
    print("probe r", left+(cells//2)*step)
    print("U_m,t and Im A_z,t at probe", *point)
    print("one +k mode graph energy endpoints", total.lower().str(14), total.upper().str(14))
    print("normalization of exp(4-1/(y(1-y)))", norm.str(10))
    assert total.is_finite() and total > 0
    assert all(not value.contains(0) for value in point)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--cells", type=int, default=256)
    parser.add_argument("--bits", type=int, default=160)
    args = parser.parse_args()
    main(args.cells, args.bits)
