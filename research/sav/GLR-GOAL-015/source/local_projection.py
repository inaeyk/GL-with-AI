"""Directed early local-return blocks, excluding the dynamic mixed fields."""
import argparse

from flint import arb, arb_mat, ctx

from graph_jets import GraphEnclosure, bump_jet, ef_transform
from source_geometry import GeometryEnclosure, symmetric, x
from verify_source import radial_cells, show


def local_matrices(r, k, h):
    """Return direct/affine pairs (M_2/r², P_2/P_0) at v=0."""
    f = 1-1/r
    a, b, c, h_theta = ef_transform(r, h)
    eta = arb_mat([[-1, 0], [0, 1]])
    x_b = eta*arb_mat([[a, b], [b, c]])*(1/f)
    d = h_theta/(2*r*r)
    square = x_b*x_b
    trace = square[0, 0]+square[1, 1]
    isotropic = 2*d*d+trace/4
    m_direct = (square-arb_mat([[isotropic, 0], [0, isotropic]]))*eta/2
    m1 = -x_b*eta
    m_affine = -2*m1*eta*m1/(4+k*k*r*r)
    return ((m_direct, 7*d*d+trace/8), (m_affine, -18*d*d))


def current_rows(r, mass, curl):
    """Triples of coefficients of (w_u,w_v,w) for F,G,H."""
    f = 1-1/r
    ft, fx = [mass[1, j]-mass[0, j] for j in range(2)]
    gt, gx = [mass[1, j]+mass[0, j] for j in range(2)]
    ht, hx = mass[0, 0], mass[0, 1]
    return ((fx-ft, fx+ft, f*ft/r),
            (gx-gt, gx+gt, f*gt/r),
            (f*(hx-ht)/r, f*(hx+ht)/r, (f*f*ht-4*f*curl)/r**2))


def moments(geometry, d, a, b):
    """Signed Fu coefficient integral, absolute F/H and pointwise G rows."""
    # Each row stores signed F_u coefficient, abs F columns, abs H
    # columns, and the three supremum caps for G.
    counts = [[arb(0)]*10 for _ in range(2)]
    for r, step in radial_cells(geometry, a, b):
        f = 1-1/r
        h = geometry.evaluate(r, use_local_mass=False)[0][0]
        time = (2*geometry.omega*(d+2*x(r))).exp()
        a0, b0, c0, angular = ef_transform(r, h)
        d_b = angular/(2*r*r)
        null_direct = -2*time*d_b*(a0+2*b0+c0)/f
        null_affine = -4*null_direct/(4+geometry.k**2*r**2)
        for stored, (mass, curl), null in zip(
            counts, local_matrices(r, geometry.k, h), (null_direct, null_affine)
        ):
            fr, gr, hr = current_rows(r, time*mass, time*curl)
            fr = (fr[0].intersection(null), fr[1], fr[2])
            measure = 2*step/f
            stored[0] += measure*fr[0]
            for i, z in enumerate(fr+hr):
                stored[i+1] += measure*z.abs_upper()
            for i, z in enumerate(gr):
                stored[7+i] = max(stored[7+i], z.abs_upper())
    return counts


def main(refinement, bits):
    ctx.prec, ctx.cap = bits, 80
    geometry = GeometryEnclosure(refinement)
    graph = GraphEnclosure(geometry)
    a, d = graph.onset, graph.onset+graph.delta/8
    r_d, cap = graph.radius(d), arb(100)
    compact = moments(geometry, d, r_d, cap)
    near = moments(geometry, d, r_d, graph.r0)
    # GOAL-013 B §1. For r>=100, all F,G,H row entries in each
    # block are bounded by 100 h^2, h=10 exp[-2(r-4)/5].
    decay, floor = arb(4)/5, arb(99)/100
    tail_point = (10**4*(-decay*(cap-4)).exp()).upper()
    tail_ray = (2*tail_point/(floor*decay)).upper()
    normalizer = arb("0.0467915154").union(arb("0.0467915159"))
    bu_end = bump_jet(arb(1)/8)[1]*arb(-4).exp()/(graph.delta.sqrt()*normalizer)
    eps0, eps2, epsv = arb("0.003017"), arb("0.004147"), arb("8.181e-7")
    w_cap = (1+eps0)/522
    m, gnorm = arb("1.6"), arb("0.000791631")
    amplification = (m*(d-a)).exp()
    endpoint_g = arb("0.4125472765").union(arb("0.4125472991"))
    for name, totals, shell in zip(("direct", "affine"), compact, near):
        signed = totals[0]+symmetric(tail_ray)
        fa, fb, fc, ha, hb, hc = [z+tail_ray for z in totals[1:7]]
        ga, gb, gc = [max(z, tail_point) for z in totals[7:10]]
        graph_shift = ((1-(-2*geometry.omega*(d-a)).exp())*signed.abs_upper()
                       + shell[1])
        intrinsic = eps0*fa+epsv*fb+w_cap*fc
        mismatch = graph_shift+intrinsic
        abs_f, abs_h, sup_g = [(1+eps0)*u+epsv*v+w_cap*w for u, v, w in
                              ((fa, fb, fc), (ha, hb, hc), (ga, gb, gc))]
        terminal = (signed+symmetric(intrinsic))*bu_end
        ref = signed*bu_end**2/8
        err_endpoint = (mismatch+eps0*abs_f)*bu_end**2/4
        err_volume = (mismatch+eps2*abs_f)*bu_end**2/8
        err_dual = gnorm*bu_end*(amplification*(m*abs_f+abs_h)
                                +(amplification-1)*sup_g)/4
        enclosure = ref+symmetric(err_endpoint+err_volume+err_dual)
        show(f"{name} signed F_u radial coefficient", signed)
        show(f"{name} coefficient graph/time shift", graph_shift)
        show(f"{name} F ray", terminal)
        show(f"{name} sharp terminal projection", endpoint_g*terminal/4)
        show(f"{name} reference", ref)
        show(f"{name} total upper error", (err_endpoint+err_volume+err_dual).upper())
        show(f"{name} physical early projection", enclosure)
        if name == "direct":
            assert enclosure > 0 and terminal > 0
        else:
            assert enclosure < 0 and terminal < 0


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--refinement", type=int, default=2)
    parser.add_argument("--bits", type=int, default=160)
    arguments = parser.parse_args()
    main(arguments.refinement, arguments.bits)
