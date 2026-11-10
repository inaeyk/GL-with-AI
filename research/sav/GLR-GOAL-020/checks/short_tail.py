"""Directed GOAL-020 prefix bound from the pinned GOAL-015 weak packet data.

Run with python3 -B short_tail.py --source /path/to/research/sav/GLR-GOAL-015.
This is a nonproduction interval calculation. It neither uses a waveform
pilot nor reconstructs a forward lower-field candidate.

Read all paths relative to research/sav. GOAL-015 observables/PACKET.md and
packet_signed.txt supply the full-packet signed primitive
    Fcal = phi B'' + C_F B' + R_F,
its L2 and sup remainder estimates and the combined compact dual bound
854.54174865 for the chosen L2 test bound 1.000930. Dividing those two
constants and rounding upward gives the operator multiplier 854. The
independent packet audit at 1089b3cb36eef1c25278c4fbfe3e843bfdcff6e4
checks these inputs. GOAL-015 REPORT.md supplies P(packet) <= -200.091.
GOAL-015 packet_weak_first.txt supplies the bump norm/sup budgets.
GOAL-014 checks/d_baseline_h1.txt supplies |g(d1)| < .009755.
GOAL-014 checks/b_weak_tails.txt supplies T1 and Tinf at r_c=400.
The minimum-radius cap c=d1+2*x(400) is unchanged throughout.

On [d0,d1], B'<0 and B''>0 in the interior, since for mirrored bump
coordinate y<=1/4 the B'' numerator is (1-2y)^2(1-2p)-2p^2>0,
p=y(1-y). The signed compact tail pairing is
    4*Delta = g(d1)*Fcal(d1) - g(d0)*Fcal(d0)
              - integral(g'*Fcal) + dual + exterior.
The sharp lower contact is present and cap G cancels with the exterior.
The bulk term splits g'=B''+(g'-B''). Use C_F>=-9.077 in the bulk,
integral |B'' B'|=B'(d0)^2/2, and integral |B''|=|B'(d0)|.
Correlate the terminal and bulk principal phase by replacing phi(u) by
phi(d0) plus the Lipschitz-weighted integral. The output bounds -Delta
above, so P(d0) <= P(d1) + loss. The .095 control retains a positive
upper endpoint for this certificate and makes no assertion of a sign.
"""

import argparse
import hashlib
from pathlib import Path
import subprocess
import sys

import flint
from flint import arb, ctx


SOURCE_SHA = "65a4af854a25ca02e40f31b00fe5ff789935bdc4"
AUDIT_SHA = "1089b3cb36eef1c25278c4fbfe3e843bfdcff6e4"
DIGESTS = {
    "GLR-GOAL-015/REPORT.md": "dec98b417e5d53e1db21161282420f13d722639f10d76f392110049440c06357",
    "GLR-GOAL-015/observables/PACKET.md": "76a33c074e0f11e7195ed9831a233699ce6b63ecf0ca9b410e16d1919f7077d3",
    "GLR-GOAL-015/observables/packet_signed.py": "e4f3d471c9b1396a18dba10378642cba8f5444ecbdc7d2da83945a22f88bbd59",
    "GLR-GOAL-015/observables/packet_signed.txt": "682be20c4713c762eb513055a626ac25016826488fa31ce80fe1c57051e442a7",
    "GLR-GOAL-015/observables/packet_weak.py": "05279c0f7ab927fecdcc02f610afceb73ef0f803b1c2c22509cbd6d929faaa55",
    "GLR-GOAL-015/observables/packet_weak_first.txt": "2df635e746a3605be6e84997153db3afb00a5d4531de1fb99cbee71112d0155e",
    "GLR-GOAL-015/observables/shifted_baseline.py": "b040759759017f9072dc5c0e27e7cdf33ac8e76c1a8f65e09dcd7e17031331f5",
    "GLR-GOAL-015/analysis/early_optical_certificate.py": "5a4e85898d8f91d6e21b2a42e1a29cbe0983098706f836dff4111e012e089e99",
    "GLR-GOAL-015/source/source_geometry.py": "87f4a3fd94fd09c6c3ddefb826a2560a630684fe791c0f36b62d9ea42ba82851",
    "GLR-GOAL-015/source/mean_flux.py": "3dedce3f8592c7dc478147da9eb8b152f95ae7f15124c36e4cbcf495337fe51e",
    "GLR-GOAL-014/checks/b_weak_tails.txt": "1defd3c9df239d8a1ce6d16472e9289e975757e5d12e76847cf8e81855d9131a",
    "GLR-GOAL-014/checks/d_baseline_h1.txt": "5b628056ddb422da97678294d90f094e2a52900d72097605a0878cca25b38005",
    "GLR-GOAL-014/ADJOINT-AND-ENDPOINTS.md": "d5da427722fe8b924ee2a6899bed4090d48dadb9d8d26ee7b6a547d794220a5f",
    "GLR-GOAL-014/CURRENTS-AND-DATA.md": "8aea50bf680d695854c7e535030aa0671db7855bb2474a9e8677b1da434c6f0b",
    "GLR-GOAL-013/checks/d_leading_news.txt": "6662394ce2e9803b883b275e885086d6c3090402d7052e11df9597be296f0b48",
}


def verify_inputs(research):
    """Pin named archives and verify the whole imported source ancestry."""
    paths = [f"GLR-GOAL-{n}" for n in ("011", "013", "014", "015")]
    for path, expected in DIGESTS.items():
        if hashlib.sha256((research / path).read_bytes()).hexdigest() != expected:
            raise ValueError(f"Unexpected digest for {path}")
    subprocess.run(["git", "diff", "--quiet", SOURCE_SHA, "--", *paths],
                   cwd=research, check=True)
    status = subprocess.check_output(
        ["git", "status", "--porcelain", "--untracked-files=all", "--", *paths],
        cwd=research, text=True)
    if status:
        raise ValueError("Pinned input directories are not clean")


def interval(lower, upper):
    return arb(lower).union(arb(upper))


def phase_enclosure(geo, raw_q, x, a, width):
    """Correlate the radial GL quadrature at constant cap on the suffix."""
    d1 = a + width
    start = a + arb(".093")
    r_cap = arb(400)

    def graph(u):
        return 1 + 2 * ((-(u + 1) / 2).exp() / 2).lambertw()

    left, right = graph(d1).lower(), graph(start).upper()
    panels = [(left, right, 80, True), (right, arb("2.5"), 1536, False),
              (arb("2.5"), arb(8), 2048, False), (arb(8), arb(25), 1024, False),
              (arb(25), arb(64), 512, False), (arb(64), r_cap, 512, False),
              (r_cap, r_cap + (d1 - start) / 2, 1, True)]
    integral = arb(0)
    for lower, upper, cells, optional in panels:
        step = (upper - lower) / cells
        for i in range(cells):
            r = (lower + i * step).union(lower + (i + 1) * step)
            q, _ = raw_q(geo, r)
            term = 2 * (2 * geo.omega * x(r)).exp() * q * step / (1 - 1 / r)
            integral += term.union(arb(0)) if optional else term
    amplitude = (geo.omega * start.union(d1)).exp() * integral
    assert amplitude > 0 or amplitude < 0
    phase = amplitude**2 / 4
    phase_lo, phase_hi = arb("1.33075"), arb("1.53038")
    assert phase > phase_lo and phase < phase_hi

    # chi=-A/2, phi=chi^2 and chi_u=Omega chi+(q_c-(1-b')q_graph)/2.
    assert left > arb("1.596") and right < arb("1.604")
    q_graph = arb(0)
    step = arb(".008") / 64
    for i in range(64):
        r = (arb("1.596") + i * step).union(arb("1.596") + (i + 1) * step)
        q_graph = max(q_graph, ((geo.omega * r).exp() * raw_q(geo, r)[0]).abs_upper())
    assert (d1 - start) / 2 < arb(".014")
    q_cap = ((geo.omega * (d1 + 2 * x(r_cap))).exp()
             * raw_q(geo, interval("400", "400.014"))[0]).abs_upper()
    chi_abs = phase_hi.sqrt()
    phase_lip = 2 * chi_abs * (geo.omega.upper() * chi_abs
                              + (q_cap + arb(26) / 21 * q_graph) / 2)
    assert phase_lip < 4
    print("graph radius endpoints", left.str(18), right.str(18))
    print("phase endpoints", phase.lower().str(18), phase.upper().str(18))
    print("q_graph upper", q_graph.str(18), "q_cap upper", q_cap.str(12))
    print("phase Lipschitz upper", phase_lip.upper().str(18))
    return phase_lo, phase_hi


def tail_bump(bump_jets, omega, width, offset):
    """One-sided bump norms and the phase-variation moment by interval cells."""
    suffix = width - offset
    end_y = suffix / width
    assert 0 < end_y < arb(1) / 4
    cells = 2048
    step = end_y / cells
    squares = [arb(0) for _ in range(3)]
    moment = arb(0)
    for i in range(cells):
        y = (i * step).union((i + 1) * step)
        jets = bump_jets(y, omega)
        for j in range(3):
            squares[j] += width * step * jets[j]**2
        moment += width * step * (suffix - width * y) * jets[2]**2
    b0, b1, b2 = bump_jets(end_y, omega)[:3]
    return suffix, squares, moment, b0, -b1, b2


def tail_loss(bump_jets, omega, width, offset, phi_lo, phi_hi):
    suffix, squares, moment, b0, b1, b2 = tail_bump(bump_jets, omega, width, offset)
    n0, n1, n2 = [square.sqrt().upper() for square in squares]
    # Packet scalar mass M<1.60, W<=1/4, |W_u|<=W/4 and |b'|<=5/21.
    # Volterra gives sup|w0-B| <= M exp(MD) sqrt(D)||B||2.
    mass = arb("1.60")
    field_error = arb(".0171")
    assert mass * (mass * width).exp() * width.sqrt() * arb(".025312") < field_error
    assert arb(5) / 21 / 4 + mass * (arb(1) / 4 + mass) < arb("3.020")
    g_norm = n1 + mass * (n0 + field_error * suffix.sqrt())
    g_prime_error = mass * n1 + arb("3.020") * n0 + arb("2.960") * field_error * suffix.sqrt()
    # Integrate w0_u = B' - integral(W w0) along the outgoing ray.
    # J_infty(b)=3/b-3/(4b^2) lies in [1.562,1.60].
    g0 = b1 - interval("1.562", "1.60") * b0 + arb(0, mass * field_error)
    assert b1 < 0 and b2 > 0 and g0 < 0

    # GOAL-015 packet_signed.txt and its full-cell sup/L2 interpretation.
    c_lower, c_upper = arb("-9.077"), arb("49.403")
    residual_l2, residual_sup = arb("8.813"), arb("45.531")
    dual_gain = arb(854)
    assert dual_gain * arb("1.000930") > arb("854.542")
    primitive_l2 = phi_hi * n2 + c_upper * n1 + residual_l2
    principal = (phi_lo.union(phi_hi) * (g0 * b2 + squares[2])).upper() + 4 * moment.upper()
    subprincipal = (c_lower.union(c_upper) * g0 * b1).upper() - c_lower * b1**2 / 2
    remainder = residual_sup * (b1.abs_upper() + g0.abs_upper()) + g_prime_error * primitive_l2
    dual = dual_gain * g_norm
    g_right = arb(".009755")
    right_contact = g_right * residual_sup

    # The exterior adjoint has ||Z||inf <= exp(D*m_c)||g 1_tail||inf.
    # The sharp left and right contacts both use the physical F ray bound T1.
    # The G term uses sup|G|<=Tinf and integral_v sup_u|Z_v|<=D*m_c||Z||inf.
    tail1, tail_inf = arb("3.354e-15"), arb("2.739e-15")
    cap_mass = arb(".0075")
    g_sup = arb("4.808") + mass * (arb(".136") + field_error)
    test_sup = (width * cap_mass).exp() * g_sup
    exterior = tail1 * (g0.abs_upper() + g_right + suffix.sqrt() * (n2 + g_prime_error)
                        + width * (1 + cap_mass) * test_sup)
    exterior += tail_inf * width**2 * cap_mass * test_sup
    assert exterior < arb("1e-12")

    loss = (right_contact + principal + subprincipal + remainder + dual + exterior) / 4
    prefix = arb("-200.091") + loss
    for name, value in (("B L2", n0), ("Bprime L2", n1), ("Bsecond L2", n2),
                        ("g L2", g_norm), ("gprime-Bsecond L2", g_prime_error),
                        ("phase moment", moment), ("right contact", right_contact),
                        ("principal", principal), ("subprincipal", subprincipal),
                        ("remainder", remainder), ("dual", dual), ("exterior", exterior),
                        ("loss -Delta", loss), ("prefix", prefix)):
        print(name, "upper", value.upper().str(16))
    return prefix


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    default_source = Path(__file__).resolve().parents[2] / "GLR-GOAL-015"
    parser.add_argument("--source", type=Path, default=default_source)
    args = parser.parse_args()
    source = args.source.resolve()
    verify_inputs(source.parent)
    sys.dont_write_bytecode = True
    sys.path.insert(0, str(source / "observables"))
    from packet_weak import GeometryEnclosure, raw_q, x
    from shifted_baseline import bump_jets

    ctx.prec, ctx.cap = 160, 80
    print(f"Python {sys.version.split()[0]} python-flint {flint.__version__} {ctx.prec} bits")
    print("source", SOURCE_SHA, "audit", AUDIT_SHA, "verified archive digests", len(DIGESTS))
    geometry = GeometryEnclosure(refinement=1)
    radius = arb(13) / 8
    a = radius - 2 * x(radius)
    width = arb(3) / 25
    phi = phase_enclosure(geometry, raw_q, x, a, width)
    for offset, expected in ((".0955", True), (".095", False)):
        print("offset", offset)
        result = tail_loss(bump_jets, geometry.omega, width, arb(offset), *phi)
        closed = result < 0
        assert (closed if expected else result > 0), (offset, result)
        print("PREFIX_NEGATIVE_CERTIFIED", closed)


if __name__ == "__main__":
    main()
