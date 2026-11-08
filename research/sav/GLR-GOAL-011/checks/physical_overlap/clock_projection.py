"""Certified component of the induced residue orthogonal to the common clock."""
from flint import acb,arb,ctx

from certified_launch import spec
from denominator_refinement import wronskian_derivative
from tight_launch import baseline_horizon,normalized_bump


def clock_projection(numerator,s,b_norm,W):
    """Return baseline residue, clock determinant and signed perpendicular part."""
    moment,_=baseline_horizon(s,b_norm,moments_only=True)
    clock=-s*moment
    assert clock.abs_lower()>0 and W.abs_lower()>0
    oriented_area=(numerator*clock.conjugate()).imag
    perpendicular=oriented_area/(abs(clock)*abs(W))
    return -moment/W,oriented_area,perpendicular


if __name__=="__main__":
    ctx.prec=128
    ctx.cap=96
    s=spec.shift(spec.CENTER,arb("5e-8"))
    W,*_=wronskian_derivative()
    # Outward rounded four-subdivision numerator from checks/signed_projection.txt.
    I=acb(arb(".42342",".34241"),arb("-3.13573",".35099"))
    base,area,perpendicular=clock_projection(I,s,normalized_bump(),W)
    print("baseline residue",base)
    print("clock-oriented numerator area",area)
    print("clock-perpendicular signed component",perpendicular)
    print("clock-perpendicular endpoints",perpendicular.lower(),perpendicular.upper())
    assert perpendicular<0
