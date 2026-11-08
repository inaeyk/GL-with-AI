"""Directed uncommuted physical mixed graph defect on the early strip."""
import argparse
from pathlib import Path
import sys

from flint import acb, arb, ctx
from c_compact import GLProfile, LAM, point_velocities
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "GLR-GOAL-013/checks"))
from a_graph import operator, profile_and_slope
from certified_launch import advance, gl_regular, normalized_bump, transport
from outer_gljet import njet


def main(n, subdivisions, bits, tol, outer):
    ctx.prec = bits
    ctx.cap = 80
    k, omega = arb.pi()/5, arb("0.06328119", "0.00000001")
    origin, delta = arb(13)/8, arb(3)/25
    a = -origin - 2*(origin-1).log()

    def graph(t):
        return 1 + 2*(((-(1+a+delta*t/8)/2).exp()/2).lambertw())

    cap = graph(1)
    gl = GLProfile(2.5, 1 + 0j, outer - 2.5, 0.5, tol)
    ordinates = point_velocities(gl, n)
    op = operator(k, omega)
    start = arb(9)/8
    regular = gl_regular(k, omega, acb(1), 40)
    initial = [z.value(acb(start-1)) for z in regular]
    n2, nr2 = transport(op, start, arb(2), initial, 64)
    norm_h = 8/(njet(arb(2), k, omega, n2.real, nr2.real)[0][3]*(2*omega).exp())
    state = transport(op, start, cap, [norm_h*z for z in initial], 48)
    norm = normalized_bump()
    graph_energy_error = arb(0)
    max_difference = [arb(0), arb(0)]
    cell_count = n*subdivisions
    step_t = arb(1)/cell_count
    r = cap
    for j in range(cell_count):
        # Walk outwards in r, i.e. backwards along the retarded graph.
        t_hi = 1-j*step_t
        t_lo = t_hi-step_t
        right = graph(t_lo)
        step = right-r
        local = advance(op, r, step, state, degree=20)
        nv, nr = [z.value(acb(arb(0).union(step))) for z in local]
        radius_cell = r.union(right)
        f, p = 1-1/radius_cell, -1/radius_cell
        b, by = profile_and_slope(t_lo.union(t_hi)/8)
        a0, a1 = norm*delta.sqrt()*b, norm/delta.sqrt()*by
        at, ax = (f*a0/radius_cell-a1)/(2*radius_cell), a1/(2*radius_cell)
        h = njet(radius_cell, k, omega, nv.real, nr.real)[0]
        vv, vr, rr = h[:3]
        factor = (omega*radius_cell).exp()/2
        ht, htx, hx = factor*vv, factor*(vv+f*vr), factor*(vv+2*f*vr+f*f*rr)
        jt = -LAM*radius_cell**2*(ht*at-htx*ax)/f
        jx = -LAM*radius_cell**2*(-htx*at+hx*ax)/f
        exact = (p*jt-jx)/(1-p*p), k*(jt-p*jx)/(LAM*radius_cell**2*(1-p*p))
        i = (cell_count-j-1)//subdivisions
        tau = n*t_lo.union(t_hi)-i
        candidate = [arb(float(v0)) + (arb(float(v1))-arb(float(v0)))*tau
                     for v0, v1 in zip(ordinates[i], ordinates[i+1])]
        err = [abs(z-h).upper() for z, h in zip(exact, candidate)]
        max_difference = [max(z, q) for z, q in zip(max_difference, err)]
        Dk = LAM+k*k*radius_cell**2
        D = radius_cell**2*Dk
        W = LAM*radius_cell**4/Dk
        energy_density = (1-p*p)/(2*f) * (err[0]**2/D+W*err[1]**2)
        graph_energy_error += step*energy_density
        state = [z.value(acb(step)) for z in local]
        r = right
    assert r.overlaps(origin)
    print(f"{n} linear graph panels, {cell_count} directed cells, {bits} Arb bits")
    print("maximum directed absolute jet errors", *(z.str(12) for z in max_difference))
    print("upper uncommuted graph energy error",
          graph_energy_error.upper().str(12))
    print("upper uncommuted graph energy error square root",
          graph_energy_error.sqrt().upper().str(12))
    assert graph_energy_error.sqrt() < arb(".02")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--n", type=int, default=192)
    parser.add_argument("--subdivisions", type=int, default=4)
    parser.add_argument("--bits", type=int, default=160)
    parser.add_argument("--tol", type=float, default=8e-14)
    parser.add_argument("--outer", type=float, default=430)
    args = parser.parse_args()
    main(args.n, args.subdivisions, args.bits, args.tol, args.outer)
