"""Exact characteristic lift of the two GOAL-015 mixed polarizations.

Drive ordinates are exact binary values of the archived candidate. The
graph and null coordinates use the mathematical abscissae from the charter.
"""

import argparse
import hashlib

from flint import arb, ctx
import numpy as np

from weak_candidate import graph, lerp


def zero():
    return (arb(0), arb(0))


def map2(fn, *rows):
    return tuple(fn(*args) for args in zip(*rows))


def from_binary(rows):
    return [tuple(arb(float(x)) for x in row) for row in rows]


def partial(left, right, fraction):
    """Integral of the affine interpolant from 0 to fraction."""
    return fraction * left + fraction**2 * (right-left) / 2


def affine_range(left, right, t):
    # Exact one-sided endpoint enclosure, tighter than interval substitution.
    return lerp(left, right, t.lower()).union(lerp(left, right, t.upper()))


class MixedLift:
    def __init__(self, filename, max_v=None):
        with np.load(filename) as data:
            D = np.asarray(data["D"], dtype="<f8")
            raw = np.asarray(data["raw_graph"], dtype="<f8")
            v = np.asarray(data["v"], dtype="<f8")
        self.digest = {k: hashlib.sha256(x.tobytes()).hexdigest()
                       for k, x in {"D": D, "raw_graph": raw, "v": v}.items()}
        self.n = n = len(raw)-1
        self.R = arb(13)/8
        self.a = -self.R-2*(self.R-1).log()
        self.width = arb(3)/200
        self.h = self.width/n
        self.cap = self.a+self.width+2*(400+arb(399).log())
        assert abs(float(v[-1])-float(self.cap)) < 2e-12
        self.v = [self.R, *(arb(float(x)) for x in v[n+1:-1]), self.cap]
        if max_v is not None:
            self.v = [x for x in self.v if x <= max_v]
        self.dv = [r-l for l, r in zip(self.v, self.v[1:])]
        m = len(self.v)
        self.D = [from_binary(row) for row in D[:, n:n+m]]
        self.Dg = from_binary([D[i, n-i] for i in range(n+1)])
        self.raw = from_binary(raw)
        assert self.D[0] == [zero()] * m and self.Dg[0] == self.raw[0] == zero()
        self.Prect = [self.primitive_v(row) for row in self.D]
        self.Qrect = [[zero()]*m for _ in range(n+1)]
        for i in range(n):
            self.Qrect[i+1] = [
                map2(lambda base, l, r: base + self.h*(l+r)/2, base, l, r)
                for base, l, r in zip(self.Qrect[i], self.D[i], self.D[i+1])
            ]
        self.Yrect = [self.primitive_v(row) for row in self.Qrect]
        self.Yw = [zero()]
        for i in range(n):
            whole = arb(0).union(arb(1))
            integral = self.wedge_integrand(i, whole)
            self.Yw.append(map2(lambda prefix, value: prefix+self.h*value, self.Yw[-1], integral))

    def primitive_v(self, values):
        result = [zero()]
        for step, lo, hi in zip(self.dv, values, values[1:]):
            result.append(map2(lambda base, l, r: base+step*(l+r)/2, result[-1], lo, hi))
        return result

    def graph_data(self, i, t):
        u = self.a+self.h*(i+t)
        b = graph(u)
        p = -1/b
        dk = 4+(arb.pi()/5)**2*b**2
        raw = map2(lambda lo, hi: affine_range(lo, hi, t), self.raw[i], self.raw[i+1])
        y_t = (raw[0]/(b*dk.sqrt()), raw[1]*(2*b**2/dk.sqrt()))
        Pg = tuple((1+p)*x/2 for x in y_t)
        Qg = tuple((1-p)*x/2 for x in y_t)
        Dg = map2(lambda lo, hi: affine_range(lo, hi, t), self.Dg[i], self.Dg[i+1])
        DR = map2(lambda lo, hi: affine_range(lo, hi, t), self.D[i][0], self.D[i+1][0])
        return b, Pg, Qg, Dg, DR

    def wedge_integrand(self, i, t):
        b, _, Qg, Dg, DR = self.graph_data(i, t)
        step = self.R-b
        minus_bprime = (b-1)/(b+1)
        return map2(lambda q, dl, dr: minus_bprime*q+step*(dl+dr)/2, Qg, Dg, DR)

    def rectangle(self, i, j, t, s):
        """Enclose (Y,P,Q,D) on a subcell of grid panel (i,j)."""
        h, dv = self.h, self.dv[j]
        b, Pg, _, Dg, DR = self.graph_data(i, t)
        Pw = map2(lambda dl, dr: (self.R-b)*(dl+dr)/2, Dg, DR)
        u_interval = arb(0).union(t.upper())
        Yw = map2(lambda base, val: base+h*t*val, self.Yw[i],
                  self.wedge_integrand(i, u_interval))

        left_D = map2(lambda lo, hi: affine_range(lo, hi, t), self.D[i][j], self.D[i+1][j])
        right_D = map2(lambda lo, hi: affine_range(lo, hi, t), self.D[i][j+1], self.D[i+1][j+1])
        D = map2(lambda lo, hi: affine_range(lo, hi, s), left_D, right_D)
        Pbase = map2(lambda lo, hi: affine_range(lo, hi, t),
                     self.Prect[i][j], self.Prect[i+1][j])
        P = map2(lambda pg, pw, b0, dl, dr: pg+pw+b0+dv*partial(dl, dr, s),
                 Pg, Pw, Pbase, left_D, right_D)

        Qleft = map2(lambda base, dl, du: base+h*partial(dl, du, t),
                     self.Qrect[i][j], self.D[i][j], self.D[i+1][j])
        Qright = map2(lambda base, dl, du: base+h*partial(dl, du, t),
                      self.Qrect[i][j+1], self.D[i][j+1], self.D[i+1][j+1])
        Q = map2(lambda lo, hi: affine_range(lo, hi, s), Qleft, Qright)
        Ybase = map2(lambda yw, base, l, r: yw+base+h*partial(l, r, t),
                     Yw, self.Yrect[i][j], self.Prect[i][j], self.Prect[i+1][j])
        Y = map2(lambda base, ql, qr: base+dv*partial(ql, qr, s), Ybase, Qleft, Qright)
        return Y, P, Q, D


def main(filename, bits):
    ctx.prec = bits
    lift = MixedLift(filename, max_v=arb(8))
    for k, digest in lift.digest.items():
        print(f"{k} float64-LE SHA-256 {digest}")
    t, s = arb(0).union(arb(1)), arb(0).union(arb(1))
    Y, P, Q, D = lift.rectangle(lift.n-1, 0, t, s)
    print("last u panel, first rectangular v panel", "Y", Y, "P", P, "Q", Q, "D", D)
    # Independently assembled interval values from adjacent rectangles at a seam.
    for i, j in [(2, 2), (lift.n-2, 4), (lift.n-1, 4)]:
        v_left = lift.rectangle(i, j-1, t, arb(1))
        v_right = lift.rectangle(i, j, t, arb(0))
        assert all(a.overlaps(b) for x, y in zip(v_left, v_right) for a, b in zip(x, y))
        u_left = lift.rectangle(i-1, j, arb(1), s)
        u_right = lift.rectangle(i, j, arb(0), s)
        assert all(a.overlaps(b) for x, y in zip(u_left, u_right) for a, b in zip(x, y))
    print("adjacent trace overlap checks passed (analytic formulas give equality)")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("candidate")
    parser.add_argument("--bits", type=int, default=160)
    args = parser.parse_args()
    main(args.candidate, args.bits)
