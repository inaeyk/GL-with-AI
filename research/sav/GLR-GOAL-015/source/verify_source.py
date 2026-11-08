"""Directed real geometry, two graph jets and separate mean-projection check."""
import argparse
from bisect import bisect_right
from platform import python_version

from flint import arb, ctx, __version__ as flint_version
from numpy import __version__ as numpy_version
from scipy import __version__ as scipy_version

from graph_jets import GraphEnclosure, bump_jet
from source_geometry import GeometryEnclosure, centered_mass, symmetric, x


def show(label, value):
    if isinstance(value, (tuple, list)):
        for index, component in enumerate(value):
            show(f"{label}[{index}]", component)
        return
    print(f"{label}: [{value.lower().str(12)}, {value.upper().str(12)}]")


def radial_cells(geometry, a, b):
    knots = [a] + [r for r in geometry.edges if a < r < b] + [b]
    for lower, upper in zip(knots, knots[1:]):
        yield lower.union(upper), upper-lower


def mean_moments(geometry, d, lower, upper):
    """All moments use time v=d+2x(r) and measure dv=2 dr/f."""
    sums = [arb(0)]*5
    for r, length in radial_cells(geometry, lower, upper):
        _, (mu, beta), _ = geometry.evaluate(r)
        f = 1-1/r
        factor = (2*geometry.omega*(d+2*x(r))).exp()
        s = factor*(beta-2*mu/(r*f))
        terms = [2*s/f, 2*s.abs_upper()/f, 2*s/r,
                 2*s.abs_upper()/r, 2*factor*beta.abs_upper()/r**2]
        sums = [v+length*y for v, y in zip(sums, terms)]
    return sums


def mean_projection(geometry, graph):
    """Sharp-cut full mean subprojection with true scalar potential."""
    a, d = graph.onset, graph.onset+graph.delta/8
    lower, upper = graph.radius(d), arb(100)
    a_signed, a_abs, af_signed, af_abs, beta_abs = mean_moments(
        geometry, d, lower, upper
    )
    _, near_abs, _, _, _ = mean_moments(geometry, d, lower, graph.r0)
    # GOAL-013 B §1 (3). |s_b|=|Δb_E,tx|/f and |beta|<=2 b_*/f.
    decay, floor = arb(4)/5, arb(99)/100
    tail = (2*10**9*(-decay*(upper-4)).exp()/floor**2
            *((upper+1)/decay+1/decay**2)).upper()
    a_signed += symmetric(tail)
    a_abs += tail
    af_signed += symmetric(tail/4)
    af_abs += tail/4
    beta_abs += 2*tail/upper**2
    # Static-t weight difference from u=d plus moving graph lower limit.
    d_a = (1-(-2*geometry.omega*(d-a)).exp())*a_signed.abs_upper()+near_abs

    # Inherit the directed e^{-1/(y(1-y))} derivative normalizer and the
    # early pointwise relative errors for w0, w0_u and w0_uu.
    normalization = arb("0.0467915154").union(arb("0.0467915159"))
    raw = [b*arb(-4).exp()/normalization for b in bump_jet(arb(1)/8)]
    b_end = raw[0]*graph.delta.sqrt()
    bu_end = raw[1]/graph.delta.sqrt()
    epsilon, epsilon_2 = arb("0.003017"), arb("0.004147")
    kappa = arb(522)
    f_err = 4*d_a+4*epsilon*a_abs+2*(1+epsilon)*af_abs/kappa
    f_abs = 4*(1+epsilon)*a_abs+2*(1+epsilon)*af_abs/kappa
    h_abs = f_abs/8+4*(1+epsilon)*beta_abs/kappa
    terminal_i = (4*bu_end*a_signed-2*b_end*af_signed
                  +symmetric(epsilon*(4*bu_end*a_abs+2*b_end*af_abs)))
    physical_gd = arb("0.4125472765").union(arb("0.4125472991"))
    terminal_proj = physical_gd*terminal_i/4

    # Z_u-g' and Z terms in the infinite-cap adjoint.
    m = arb("1.6")
    dual = (m*(d-a)).exp()*arb("0.000791631")*(m*f_abs+h_abs)*bu_end
    candidate = a_signed*bu_end**2/2
    # Four times the projection error. The sharp bulk of the reference uses
    # int_a^d B'' B' du = (B'(d))²/2, with B'(a)=0.
    endpoint = (f_err+epsilon*f_abs)*bu_end**2
    volume = (f_err+epsilon_2*f_abs)*bu_end**2/2
    total_error = (endpoint+volume+dual)/4
    projection = candidate+symmetric(total_error)
    show("mean tail abs terminal s bound", tail)
    show("mean signed A_star", a_signed)
    show("upper envelope for mean absolute radial s", a_abs.upper())
    show("mean graph/time variation A", d_a)
    show("mean signed terminal F ray", terminal_i)
    show("mean signed terminal contribution", terminal_proj)
    show("mean F_ref error coefficient", f_err)
    show("mean cap-free reference", candidate)
    show("mean error endpoint", endpoint/4)
    show("mean error volume", volume/4)
    show("mean error true adjoint and H", dual/4)
    show("mean physical early projection", projection)
    assert projection > 0 and terminal_proj > 0


def graph_check(graph, panels, width):
    a = graph.onset
    du = width/panels
    energy = arb(0)
    for j in range(panels):
        u = (a+j*du).union(a+(j+1)*du)
        r = graph.radius(u)
        z = graph.evaluate_radius(r)
        p, pt = -1/r, (1-1/r)/r**2
        f = 1-1/r
        skew = 4*graph.geometry.k*f/(4+graph.geometry.k**2*r**2)
        bg = -skew*z.g[1], skew*z.g[0]
        for i in range(2):
            uv = (bg[i]-z.source[i])/4
            tan_u = (pt*z.g[i]+(1+p)*z.g_tan[i])/2
            tan_v = (-pt*z.g[i]+(1-p)*z.g_tan[i])/2
            uu = (tan_u-(1+p)*uv)/(p-1)
            vv = (tan_v-(p-1)*uv)/(p+1)
            assert (uu+2*uv+vv).overlaps(z.g_t[i])
            assert (vv-uu).overlaps(z.g_x[i])
        energy += du*(1+p)*sum(v*v for v in z.g)/2
    return energy


def main(refinement, bits):
    ctx.prec, ctx.cap = bits, 80
    geometry = GeometryEnclosure(refinement)
    graph = GraphEnclosure(geometry)
    print(f"Python {python_version()} / Flint {flint_version} / NumPy {numpy_version}"
          f" / SciPy {scipy_version} / {bits} bits")
    print(f"real radial cells: {len(geometry.edges)-1}")
    show("decaying log slope remainder at r=512", geometry.slope_tail)
    show("fixed-v infinity mean radii at r=512", geometry.mean_tail)
    h, _, _ = geometry.evaluate(arb(2))
    norm = h[0][3]*(2*geometry.omega).exp()/8
    assert norm.contains(1)
    show("GL angular normalization", norm)
    d = graph.onset+graph.delta/8
    show("r_sigma(d)", graph.radius(d))
    for r in (graph.radius(d), arb(2), arb("2.5"), arb(5), arb(20), arb(100), arb(400)):
        i = bisect_right(geometry.float_edges, float(r))-1
        n, p = geometry._cell_state(i, r)
        local = centered_mass(r, n, p, geometry.k, geometry.omega)
        _, mean, _ = geometry.evaluate(r, use_local_mass=False)
        assert local.overlaps(mean[0])
        show("local mu at r="+r.str(8), local)
        show("quadrature mu at r="+r.str(8), mean[0])
    _, mean, mr = geometry.evaluate(graph.radius(d))
    show("early terminal graph mean", mean)
    show("early terminal graph fixed-v mean derivative", mr)
    g = graph.evaluate(d)
    assert g.raw[0] < 0 and g.raw[1] > 0
    show("early terminal raw U_t, Im Az_t", g.raw)
    show("early terminal normalized Y_t", g.g)
    show("early terminal normalized Y_tt", g.g_t)
    show("early terminal normalized Y_tx", g.g_x)
    show("early graph energy, one +k", graph_check(graph, 512, graph.delta/8))
    show("packet graph energy, one +k", graph_check(graph, 512, graph.delta))
    mean_projection(geometry, graph)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--refinement", type=int, default=2)
    parser.add_argument("--bits", type=int, default=160)
    args = parser.parse_args()
    main(args.refinement, args.bits)
