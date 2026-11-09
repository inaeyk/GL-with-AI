"""Ordinary-frequency best fit using the pinned GOAL-017 baseline lift."""
from importlib.util import module_from_spec, spec_from_file_location
from pathlib import Path
import sys

from flint import acb, arb, ctx

spec = spec_from_file_location("baseline_source", Path(sys.argv[1]))
baseline = module_from_spec(spec)
spec.loader.exec_module(baseline)
ctx.prec = 192
_, values, error = baseline.integrate(128, 256, 256, 8192)
d = arb("8.407362")
s = acb("-.177924631", "-.747343369")
r = arb("5e-8")
h = d / (len(values) - 1)

def integral(rate, width):
    return (rate * width).expm1() / rate

a = integral(2*s.real, d)
z = integral(2*s, d)
g11, g22, g12 = (a+z.real)/2, (a-z.real)/2, z.imag/2
det = g11*g22-g12*g12
lam = (a-abs(z))/2
j0 = integral(s, h)
j1 = ((s*h).exp()*(s*h-1)+1)/(s*s*h)
mom = sum((s*(i*h)).exp()*(l*j0+(v-l)*j1)
          for i,(l,v) in enumerate(zip(values, values[1:])))
p1, p2 = mom.real, mom.imag
nsq = sum(h*(l*l+l*v+v*v)/3 for l,v in zip(values, values[1:]))
pr = (g22*p1*p1-2*g12*p1*p2+g11*p2*p2)/det
resid = (nsq-pr).sqrt()
# The real two-column map depends on exp(su), whose left-half-plane
# displacement is bounded by r*u. Its operator norm is the L2 bound.
dB = r*(d**3/3).sqrt()
assert lam.sqrt() > dB and s.real+r < 0
# Both best-fit coefficient vectors are bounded by ||n|| / sigma_min,
# using that the orthogonal projections have norm at most ||n||.
err = error + nsq.sqrt()*dB/(lam.sqrt()-dB)
print("center candidate norm", nsq.sqrt())
print("center projection norm", pr.sqrt())
print("center candidate best residual", resid)
print("error for waveform and pole disk", err)
print("physical best residual lower", (resid-err).lower())
print("physical best residual upper", (resid+err).upper())
print("relative to baseline norm", (resid-err)/(nsq.sqrt()+error),
      (resid+err)/(nsq.sqrt()-error))
