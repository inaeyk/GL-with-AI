"""Directed early graph data and the fixed-mass mean at the early terminal ray."""
import argparse
from pathlib import Path
from platform import python_version
import sys

import flint
from flint import acb, arb, arb_mat, ctx

research = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(research / "GLR-GOAL-013/checks"))
from a_graph import operator, profile_and_slope, velocities
from certified_launch import advance, gl_regular, normalized_bump, transport
from mean_certificate import factored_mean
from outer_gljet import njet


def show(name, x):
    print(f"{name} in [{x.lower().str(12)}, {x.upper().str(12)}]")


def mean_cells(left, right, cells, state, op, k, omega, graph=None):
    step = (right - left) / cells
    slab = arb(0).union(step)
    mean_integral = [arb(0), arb(0)]
    launch_energy = arb(0)
    for j in range(cells):
        r = left + j * step
        local = advance(op, r, step, state, degree=20)
        n, nr = [value.value(acb(slab)).real for value in local]
        p = omega + (1 - 1 / (r + slab)) * nr / n
        source = factored_mean(acb(r + slab), acb(k), acb(omega), acb(n), acb(p))
        mean_integral = [q - step * dq.real for q, dq in zip(mean_integral, source)]
        if graph is not None:
            assert all(dq.real < 0 for dq in source), "early mean integrand sign"
            norm, onset, delta = graph
            _, _, density = velocities(r + slab, n, nr, k, omega, norm, onset, delta)
            launch_energy += density * step
        state = [value.value(acb(step)) for value in local]
    return state, mean_integral, launch_energy


def return_graph(r, state, k, omega, norm, mean, onset, delta):
    f, p = 1 - 1 / r, -1 / r
    u = -r - 2 * (r - 1).log()
    shape, slope = profile_and_slope((u - onset) / delta)
    w0, w0_t = norm * delta.sqrt() * shape, norm * slope / delta.sqrt()
    a0 = arb_mat([[(f * w0 - r * w0_t) / (2 * r*r)], [w0_t / (2*r)]])
    vv, vr, rr, angular = njet(r, k, omega, *[q.real for q in state])[0]
    h = arb_mat([[vv, vv + f*vr], [vv + f*vr, vv + 2*f*vr + f*f*rr]])
    h *= (omega * r).exp()
    eta = arb_mat([[-1, 0], [0, 1]])
    identity = arb_mat([[1, 0], [0, 1]])
    x = eta * h / f
    d = angular * (omega*r).exp() / (2*r*r)
    assert (x[0, 0] + x[1, 1] + 4*d).contains(0)
    m1 = -x * eta
    j = 2*r*r*m1*a0  # lambda times half-amplitude M1/r²
    at = (j[0, 0] - p*j[1, 0]) / (4*r*r*(1-p*p))
    a1 = arb_mat([[at], [-p*at]])
    x2 = x*x
    direct_m2 = (x2 - (2*d*d + (x2[0, 0] + x2[1, 1])/4)*identity)*eta/2
    mu, beta = [q * (2*omega*r).exp() for q in mean]  # EF v=r on Sigma
    current = 4*r*r*(m1*a1 + direct_m2*a0)
    F_mean = 2*(beta - 2*mu/(r*f))*(2*w0_t - f*w0/r)
    F = (current[1, 0] - current[0, 0])/(2*r) + F_mean
    G = (current[1, 0] + current[0, 0])/(2*r)
    velocity = (current[1, 0] - p*current[0, 0])/(2*r*(1-p*p))
    velocity += F_mean/(2*(1-p))
    return F, G, velocity, F_mean


def main(cells, bits):
    ctx.prec = bits
    ctx.cap = 80
    k = arb.pi() / 5
    omega = arb("0.06328119", "0.00000001")
    op = operator(k, omega)
    start, front = arb(9) / 8, arb(13) / 8
    delta = arb(3) / 25
    onset = -front - 2 * (front - 1).log()
    end_cut = onset + delta / 8
    end_radius = 1 + 2 * ((-(1 + end_cut) / 2).exp() / 2).lambertw()
    assert arb("1.6214") < end_radius < arb("1.6215")
    regular = gl_regular(k, omega, acb(1), 40)
    initial = [value.value(acb(start - 1)) for value in regular]
    n2, nr2 = transport(op, start, arb(2), initial, 64)
    angular = njet(arb(2), k, omega, n2.real, nr2.real)[0][3]
    scale = 8 / (angular * (2 * omega).exp())
    terminal_state = transport(op, start, end_radius, [scale*x for x in initial], 64)
    norm = normalized_bump()
    front_state, near, energy = mean_cells(end_radius, front, cells // 4, terminal_state,
                                          op, k, omega, (norm, onset, delta))
    _, outer, _ = mean_cells(front, arb("2.5"), cells, front_state, op, k, omega)
    # Widened real output intervals of the inherited directed semi-infinite
    # quadrature, exterior_source.txt lines 21-25, factoring e^(2 Omega v).
    match = arb(0, "0.08"), arb("0.1", "0.10")
    at_front = [a + b for a, b in zip(outer, match)]
    total = [a + b for a, b in zip(near, at_front)]
    ut, az, _ = velocities(end_radius, *[v.real for v in terminal_state], k, omega,
                           norm, onset, delta)
    print(f"Python {python_version()} / python-flint {flint.__version__} / {bits} bits")
    print(f"near/outer cells {cells//4}/{cells}")
    show("r_sigma(d)", end_radius)
    show("early mixed graph energy, one +k", energy)
    show("U_t at terminal graph", ut)
    show("Im A_z,t at terminal graph", az)
    show("factored mu over entire early graph", at_front[0].union(total[0]))
    show("factored beta over entire early graph", at_front[1].union(total[1]))
    assert energy > 0 and not ut.contains(0) and not az.contains(0)
    for label, mean in (("fixed-mass", total), ("incorrect zero at launch radius", near)):
        show(f"{label} mu factored", mean[0])
        show(f"{label} beta factored", mean[1])
        for name, q in zip(("F", "G", "w_2,t", "F_mean"),
                           return_graph(end_radius, terminal_state, k, omega, norm, mean, onset, delta)):
            show(f"{label} terminal-graph {name}", q)
    assert total[0] > near[0] and total[1] > near[1]
    change_f = return_graph(end_radius, terminal_state, k, omega, norm,
                            at_front, onset, delta)[-1]
    show("change in F from missing exterior mean only", change_f)
    if cells >= 2048:
        assert change_f > 0


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--cells", type=int, default=512)
    parser.add_argument("--bits", type=int, default=160)
    args = parser.parse_args()
    assert args.cells >= 16 and args.cells % 4 == 0
    main(args.cells, args.bits)
