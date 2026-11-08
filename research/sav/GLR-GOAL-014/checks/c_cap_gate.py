"""Directed early-strip baseline residual and cap-candidate sign gate."""
import argparse
from pathlib import Path
import sys

from flint import arb, ctx
import numpy as np

sys.path.insert(0, str(Path(__file__).resolve().parents[2] /
                       "GLR-GOAL-011/checks/physical_overlap"))
from certified_launch import normalized_bump
sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "GLR-GOAL-013/checks"))
from a_graph import profile_and_slope


def main(path, subdivisions, bits):
    ctx.prec = bits
    ctx.cap = 80
    delta = arb(3)/25
    D = delta/8
    origin = arb(13)/8
    a = -origin - 2*(origin-1).log()
    bmin = 1 + 2*(((-(1+a+D)/2).exp()/2).lambertw())
    norm = normalized_bump()
    with np.load(path) as data:
        ordinates = [arb(float(p)) for p in data["p_cap"]]
        u, v = data["u"], data["v"]
    assert abs((u[-1]-u[0])-float(D)) < 1e-14
    # Candidate means the affine interpolant through these exact binary values
    # at u=a+j*D/N. The exact mathematical cap is v=d+2*x(400).
    assert abs(v[-1]-float(a+D+2*(400+arb(399).log()))) < 1e-11
    panels = len(ordinates)-1
    h = D/panels
    pnorm2 = sum(h*(l*l+l*r+r*r)/3 for l, r in zip(ordinates, ordinates[1:]))
    pnorm = pnorm2.sqrt()
    aref = arb(0)
    A1 = arb(0)
    A2 = arb(0)
    Ap2 = arb(0)
    cell_width = h/subdivisions
    for i, (l, r) in enumerate(zip(ordinates, ordinates[1:])):
        for j in range(subdivisions):
            t = (j+arb(".5", ".5"))/subdivisions
            y = (i+t)/(8*panels)
            bump, slope = profile_and_slope(y)
            w0, news0 = norm*delta.sqrt()*bump, norm/delta.sqrt()*slope
            interpolated = l+(r-l)*t
            aref += cell_width*news0*interpolated
            A1 += cell_width*abs(w0)
            A2 += cell_width*abs(w0).upper()**2
            Ap2 += cell_width*abs(news0).upper()**2
    a2, ap2 = A2.sqrt(), Ap2.sqrt()
    ap1 = norm*delta.sqrt()*profile_and_slope(arb(1)/8)[0]
    Jmax = 3/bmin-3/(4*bmin**2)
    pot1 = 4*Jmax*(D/4).exp()
    pot2 = (2*(12/bmin**3-18/bmin**4+9/bmin**5-3/(2*bmin**6))).sqrt()
    E0 = Jmax*(D*Jmax).exp()*a2

    B0 = (pot1*a2+pot2*A1)/2
    B1 = (pot1*ap2+pot2*ap1)/2

    fmax = 1-1/origin
    Vmax = fmax*(6/bmin**2-3/bmin**3)
    Ein1 = fmax*Vmax**2*A2/(2*(1-1/bmin**2)**2)
    M1 = (B1.upper()+(B1.upper()**2+4*Ein1.upper()).sqrt())/2

    rc = arb(400)
    exponent = 2*D/arb.pi()*(3/rc-3/(4*rc**2))
    amp = exponent.exp()
    g_norm_bound = arb(".01364715")  # GOAL-013 whole-cell bound on the early cut
    source_error = arb("1.701e-6")   # audited upper bound after cap amplification
    budget = E0*pnorm + g_norm_bound*((amp-1)*pnorm+source_error)
    coeff = g_norm_bound*amp
    gate = (aref-budget)/coeff
    print(f"{panels} affine cap panels; {panels*subdivisions} directed cells; {bits} bits")
    print("candidate ||p_c||_2", pnorm.str(13))
    print("reference <a',p_c>", aref.lower().str(13), aref.upper().str(13))
    print("upper baseline news error vs a'", E0.upper().str(12))
    print("uncommuted baseline flux error <=", B0.upper().str(12))
    print("commuted baseline graph energy <=", Ein1.upper().str(12))
    print("once-commuted baseline flux error <=", M1.upper().str(12))
    print("upper fixed projection error budget", budget.upper().str(12))
    print("upper coefficient multiplying compact return E_c", coeff.upper().str(12))
    print("sufficient E_c <", gate.lower().str(12))
    assert E0 < arb(".000043")
    assert gate.lower() > 550


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("candidate")
    parser.add_argument("--subdivisions", type=int, default=16)
    parser.add_argument("--bits", type=int, default=160)
    args = parser.parse_args()
    main(args.candidate, args.subdivisions, args.bits)
