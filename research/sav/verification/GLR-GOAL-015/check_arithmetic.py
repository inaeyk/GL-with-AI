"""Review checks on the fixed early affine ordinates and packet ledger."""
import argparse
import hashlib
from fractions import Fraction

from flint import arb, ctx
import numpy as np


def early_pairing(archive):
    """Integrate B' p by parts, using only the frozen affine ordinates."""
    with np.load(archive, allow_pickle=False) as data:
        ordinates = np.asarray(data["p_cap"], dtype="<f8")
    digest = hashlib.sha256(ordinates.tobytes()).hexdigest()
    assert digest == "38ecd9ed4d942aeb261cc197bf5662e9d80a584fa6dae955a322a61817915c39"
    assert ordinates.shape == (193,) and np.isfinite(ordinates).all()
    values = [arb(float(value)) for value in ordinates]
    delta, early = arb(3) / 25, arb(3) / 200
    normalizer = arb("0.04679151546115").union(arb("0.04679151580708"))
    scale = delta.sqrt() / normalizer

    def bump(y):
        return scale * (-1 / (y * (1 - y))).exp()

    def second(y):
        q, p = 1 - 2 * y, y * (1 - y)
        z = 1 / p
        return bump(y) * ((q * z**2)**2 - 2 * z**2 - 2 * q**2 * z**3)

    panels = len(values) - 1
    parts = 32
    h = early / (delta * panels * parts)
    norm_square = arb(0)
    pairing = values[-1] * bump(early / delta)
    for i, (left, right) in enumerate(zip(values, values[1:])):
        norm_square += (left**2 + left * right + right**2) * early / (3 * panels)
        integral = arb(0)
        for j in range(parts):
            index = i * parts + j
            lo, hi = index * h, (index + 1) * h
            if index < 8:
                integral += h * arb(0).union(bump(hi))
            else:
                center = (lo + hi) / 2
                radius = (h**3 * second(lo.union(hi)) / 24).abs_upper()
                integral += h * bump(center) + arb(0, radius)
        # d p / d y is constant on the affine panel.
        pairing -= (right - left) * 8 * panels * integral

    assert norm_square.sqrt() > arb("635.49461514")
    assert norm_square.sqrt() < arb("635.49461516")
    assert pairing > arb("7.80653") and pairing < arb("7.86816")
    print("affine ordinate SHA-256:", digest)
    print("exact affine norm:", norm_square.sqrt().str(18))
    print("independent by-parts affine pairing:", pairing.lower().str(18), pairing.upper().str(18))
    error = arb("0.024231") + arb("0.013648127") * arb("323.984")
    strong_lower, strong_upper = pairing - error, pairing + error
    assert strong_lower > arb("3.3605") and strong_upper < arb("12.3142")
    print("early interval with inherited transfer:", strong_lower.lower().str(18), strong_upper.upper().str(18))


def wedge_classification():
    """Exclude indeterminate Arb booleans at all sign-proof wedge tests."""
    def x(r):
        return r + (r - 1).log()

    front_radius = arb(13) / 8
    front = front_radius - 2 * x(front_radius)
    for label, cut, width, n_first in (
        ("early", arb(81) / 50, arb(3) / 200, 384),
        ("packet absolute", arb("1.596"), arb(3) / 25, 384),
        ("packet signed", arb("1.596"), arb(3) / 25, 512),
    ):
        d = front + width
        segments = [(cut, arb(5) / 2, n_first), (arb(5) / 2, arb(8), 768),
                    (arb(8), arb(25), 512), (arb(25), arb(64), 256),
                    (arb(64), arb(400), 256)]
        offsets = []
        for lo, hi, cells in segments:
            for j in range(cells):
                r = lo + (hi - lo) * j / cells
                offset = d + 2 * x(r) - front_radius
                assert offset < 0 or offset > 0
                offsets.append(offset.abs_lower())
        print(label, "minimum whole-Arb distance from v_start = b(a):", min(offsets).str(12))


def ledger():
    """Read the displayed, outward-expanded ledger with exact decimal arithmetic."""
    def rational(value):
        return Fraction(value)

    early_err = rational("0.024231") + rational("0.013648127") * rational("323.984")
    early_lo, early_hi = rational("7.80653") - early_err, rational("7.86816") + early_err
    assert early_lo > rational("3.3605") and early_hi < rational("12.3142")
    print("early display ledger:", f"{float(early_lo):.12f}", f"{float(early_hi):.12f}")

    leading = rational("4393.82672")
    errors = sum(map(rational, ("44.02053", "2078.31966", "0.44415", "616.13375", "854.54175")))
    packet_hi = (errors - leading) / 4 + rational("1.63462e-14")
    assert packet_hi < rational("-200.091")
    print("packet upper display ledger:", f"{float(packet_hi):.12f}")

    energy = arb(3) / (4 * arb.pi())
    for p_lo, p_hi, e_lo, e_hi in (
        ("3.3605", "12.3142", "0.80226", "2.93980"),
        ("-2758.258", "-200.091", "-658.486", "-47.768"),
    ):
        assert arb(e_lo) < energy * arb(p_lo)
        assert energy * arb(p_hi) < arb(e_hi)
    print("outward rounding of the two published energy intervals verified")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("archive")
    args = parser.parse_args()
    ctx.prec = 160
    early_pairing(args.archive)
    wedge_classification()
    ledger()


if __name__ == "__main__":
    main()
