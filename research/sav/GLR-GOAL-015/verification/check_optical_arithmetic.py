"""Independent arithmetic checks for the directed early weak and strong bounds."""

from fractions import Fraction as Q
from flint import arb, ctx


# Outward rounded constituent bounds from the primary weak calculation.
weak_terms = [Q(x) for x in ("70.17398", "51.33608", "6.88610", "4.26297", "0.88810")]
weak = (weak_terms[0] - sum(weak_terms[1:])) / 4 - Q("6.948e-16")
assert weak > Q("1.70018")
print(f"conservative weak lower using the rounded ledger: {float(weak):.12f}")

# This evaluates the potential norm bound independently of the strong script.
ctx.prec = 160
width = arb(3) / 200
R = arb(13) / 8
d = -R - 2 * (R - 1).log() + width
b = 1 + 2 * (((-1 - d) / 2).exp() / 2).lambertw()
M = (width / 4).exp() * (3/b - 3/(4*b*b) - arb(3)/400 + arb(3)/(4*400**2))
assert M < arb("1.563294")
print("strong compact M upper:", M.upper().str(16))

# The inherited strong sign gate uses a lower affine overlap and upper
# transfer and baseline errors. The first certified strong run had Ec<537.681.
margin = Q("7.80653") - Q("0.024231") - Q("0.013648127") * Q("537.681")
assert margin > Q("0.4439")
print(f"conditional strong sign lower at Ec=537.681: {float(margin):.12f}")
