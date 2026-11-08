"""Directed full-packet negativity threshold for a cap-news candidate.

This checks the specified candidate pairing and propagation error.
The physical compact cap defect remains an independent input.
"""

import argparse
import hashlib
import platform

import flint
from flint import arb, ctx
import numpy as np

from weak_candidate import bump, graph, J, symmetric


CAP_DIGESTS = {
    128: "70f6323e17d2de27659cd8b1c50137c8622374c7bfbedbba05c23f3dfe14174f",
    256: "f8039040c6b21af5780878e24f21584d13aa8285a58b4f36af54731162272b6b",
}


def calculate(filename, subcells, bits):
    ctx.prec = bits
    with np.load(filename) as data:
        raw = np.asarray(data["p_cap"], dtype="<f8")
        nodes_u = np.asarray(data["u"], dtype="<f8")
        cap_num = float(data["v"][-1])
    ordinates = [arb(float(x)) for x in raw]
    panels = len(ordinates)-1
    digest = hashlib.sha256(raw.tobytes()).hexdigest()
    assert digest == CAP_DIGESTS[panels], f"Unexpected cap digest {digest}"
    assert np.isfinite(raw).all()
    R, delta, rc = arb(13)/8, arb(3)/25, arb(400)
    a = -R-2*(R-1).log()
    d = a+delta
    cap = d+2*(rc+(rc-1).log())
    assert np.max(abs(nodes_u-(float(a)+np.linspace(0,float(delta),panels+1)))) < 2e-14
    assert abs(cap_num-float(cap)) < 2e-12
    norm = arb("0.04679151546115").union(arb("0.04679151580708"))
    M = arb("1.61")
    assert J(graph(d)) < M
    h = delta/panels
    du = h/subcells
    pnorm2 = sum(h*(l*l+l*r+r*r)/3 for l, r in zip(ordinates, ordinates[1:]))
    prefix = moment = area = projection = arb(0)
    first, second, direct, remainder_work = [arb(0) for _ in range(4)]
    for i, (lo, hi) in enumerate(zip(ordinates, ordinates[1:])):
        slope = (hi-lo)/h
        for j in range(subcells):
            t = (arb(j)/subcells).union(arb(j+1)/subcells)
            y = (i+t)/panels
            u = a+delta*y
            B, _ = bump(y, delta, norm)
            p = lo+(hi-lo)*t
            jmass = J(graph(u))
            derivative = jmass**2*B
            inner = prefix+arb(0).union(du)*derivative
            remainder = M**2*((delta/4).exp()/8+M*(M*delta).exp())*(
                moment + du*area + du**2*B.abs_upper()/2
            )
            # The B' pairing integrates by parts. B vanishes at both cuts,
            # and the cap candidate is continuous across all u seams.
            direct -= du*slope*B
            first -= du*p*jmass*B
            second += du*p*inner/2
            remainder_work += du*p.abs_upper()*remainder
            prefix += du*derivative
            moment = (moment+du*area+du**2*B.abs_upper()/2).upper()
            area = (area+du*B.abs_upper()).upper()
    projection = direct+first+second+symmetric(remainder_work)
    pnorm = pnorm2.sqrt()
    alpha = 2*delta/arb.pi()*J(rc)
    amp = alpha.exp()
    g_norm = arb("1.00093")
    # GOAL-013 physical strong forcing norm B_c = ||S||_{L1_v L2_u}/4.
    ext_forcing = arb("1.7e-6")
    constant = g_norm*((amp-1)*pnorm+amp*ext_forcing)
    factor = g_norm*amp
    sufficient = (-projection.upper()-constant.upper())/factor
    print(f"Python {platform.python_version()} NumPy {np.__version__} "
          f"python-flint {flint.__version__}; {panels} affine panels "
          f"with {subcells} directed subcells; {bits} bits")
    print("SHA-256 of exact little-endian binary64 p_cap ordinates", digest)
    for name, value in (
        ("cap candidate norm", pnorm), ("free baseline overlap", direct),
        ("first scatter overlap", first), ("second scatter overlap", second),
        ("remaining test overlap radius", remainder_work.upper()),
        ("candidate pairing with exact leading news", projection),
        ("exterior error constant", constant), ("physical compact error multiplier", factor),
        ("sufficient E_c threshold", sufficient),
    ):
        print(f"{name}: {value.lower().str(14)} {value.upper().str(14)}")
    assert projection < 0
    assert sufficient > arb(1400)
    print("The physical cap defect must be bounded separately.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("candidate")
    parser.add_argument("--subcells", type=int, default=32)
    parser.add_argument("--bits", type=int, default=160)
    args = parser.parse_args()
    calculate(args.candidate, args.subcells, args.bits)
