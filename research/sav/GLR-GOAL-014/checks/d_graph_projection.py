"""Arb enclosure of complete physical graph summand of early interference."""
import argparse
import platform
import sys
from pathlib import Path

import flint
from flint import acb, arb, ctx

from d_baseline_h1 import emit, jets, mass, radius, symmetric

SAV = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(SAV / 'GLR-GOAL-011/checks/physical_overlap'))
from certified_launch import advance, gl_regular, normalized_bump, transport
from outer_gljet import njet


def operator(r, k, omega):
    t = r - 1
    d = 1 + k*k*r**3
    c = k*k*r**3 + 6*r - 11 + 18*t/d**2 - 6*(4*r - 5)/d
    return [[0, 1], [c/(r*r*t), -(1/r + 2*omega*r)/t]]


def graph_currents(r, fields, a0, a1, omega):
    """Return characteristic launch slopes for mixed and direct local terms."""
    f = 1 - 1 / r
    p = -1 / r
    vv, vr, rr, sphere = fields
    clock = (omega*r).exp()
    ht, htx, hx = clock*vv, clock*(vv+f*vr), clock*(vv+2*f*vr+f*f*rr)
    at, ax = (f*a0/r-a1)/(2*r), a1/(2*r)

    # M is the unhalved first mass matrix. The +k current contains M/2.
    mtt, mtx, mxx = -r*r*ht/f, r*r*htx/f, -r*r*hx/f
    normal_t, normal_x = mtt-p*mtx, mtx-p*mxx
    mixed_t = (normal_t*at+normal_x*ax)/(2*r*r*(1-p*p))
    mixed_delta = 4 * (normal_x-p*normal_t) * mixed_t

    # X is the first relative metric endomorphism b_0^{-1} h.
    xtt, xtx, xxt, xxx = -ht/f, -htx/f, htx/f, hx/f
    xtt2, xxx2 = xtt*xtt+xtx*xxt, xxx*xxx+xxt*xtx
    offdiag = xtx*(xtt+xxx)
    d = clock*sphere/(2*r*r)
    shift = 2*d*d + (xtt2+xxx2)/4
    m2tt, m2tx, m2xx = -r*r*(xtt2-shift)/2, r*r*offdiag/2, r*r*(xxx2-shift)/2
    direct_delta = 4 * ((m2tx-p*m2tt)*at + (m2xx-p*m2tx)*ax)
    # Whole-early-graph fixed-v mean boxes from b_graph_mean_variant.txt.
    mu = arb('0.6439').union(arb('0.8330'))*clock**2
    beta = arb('2.9692').union(arb('3.2478'))*clock**2
    s = beta-2*mu/(r*f)
    mean_delta = 4*r*r*(1+p)*s*(ax-at)
    return tuple(source / (4*r*(1-p)) for source in (mixed_delta, direct_delta, mean_delta))


def run(cells):
    delta = arb(3)/25
    R = arb(13)/8
    a = -R - 2*(R-1).log()
    k = arb.pi()/5
    omega = arb('0.06328119', '0.00000001')
    r_lower = arb('1.621')
    assert radius(a+delta/8) > r_lower and mass(r_lower) < arb('1.57')
    M = arb('1.57')
    op = lambda r: operator(r, k, omega)
    anchor = arb(9)/8
    unscaled = [v.value(acb(anchor-1)) for v in gl_regular(k, omega, acb(1), 40)]
    n2, nr2 = transport(op, anchor, arb(2), unscaled, 64)
    angular = njet(arb(2), k, omega, n2.real, nr2.real)[0][3]
    scale = 8/(angular*(2*omega).exp())
    left_state = transport(op, anchor, r_lower, [scale*x for x in unscaled], 48)
    profile = advance(op, r_lower, R-r_lower, left_state, degree=30)
    # GOAL-011 normalizes exp(4-1/[y(1-y)]).
    inv_norm = normalized_bump()*arb(4).exp()
    emit('GL horizon scale', scale.real)
    emit('inverse bump derivative norm', inv_norm)

    dy = arb(1)/(8*cells)
    du = dy*delta
    flat = 512 * (1/dy)**8 * (-1/dy).exp()
    area = arb(0)
    graph_area = [arb(0)] * 3
    raw = [arb(0)] * 3
    scatter = [arb(0)] * 3
    left_r = R
    for i in range(cells):
        y_l, y_r = i*dy, (i+1)*dy
        if i:
            b, b1 = jets(y_l.union(y_r))[:2]
        else:
            b, b1 = arb(0).union(flat.upper()), symmetric(flat)
        a0, a1 = inv_norm*delta.sqrt()*b, inv_norm*b1/delta.sqrt()
        right_r = radius(a+delta*y_r)
        r = right_r.union(left_r)
        left_r = right_r
        n, nr = [v.value(acb(r-r_lower)).real for v in profile]
        q = graph_currents(r, njet(r, k, omega, n, nr)[0], a0, a1, omega)

        # A first-scatter test has error <= M^2 exp(Ms) int_a^u |a0|.
        area = (area+du*a0.abs_upper()).upper()
        propagation = M*(M*delta*y_r).exp()
        g = a1-mass(r)*a0+symmetric(M*propagation*area)
        for j, launch in enumerate(q):
            graph_area[j] = (graph_area[j]+du*launch.abs_upper()).upper()
            raw[j] += du*g*launch
            scatter[j] = (scatter[j]+du*g.abs_upper()*propagation*graph_area[j]).upper()

    for name, main, correction in zip(('mixed graph', 'local direct graph', 'fixed-mass mean graph'),raw,scatter):
        emit(name+' free overlap', main)
        emit(name+' RW correction bound', correction)
        emit(name+' physical graph summand', main+symmetric(correction))
    total = sum(raw)+symmetric(sum(scatter))
    emit('complete physical graph summand', total)
    assert total < 0


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--mesh-power', default=12, type=int, choices=range(8, 15))
    parser.add_argument('--precision', default=160, type=int, choices=(128,160,192))
    args = parser.parse_args()
    ctx.prec = args.precision
    ctx.cap = 90
    print(f'Python {platform.python_version()}, python-flint {flint.__version__}, '
          f'Arb precision {ctx.prec}, early cells 2^{args.mesh_power}')
    run(2**args.mesh_power)
