"""Exact rational postprocessing of the replayed numerator and W' boxes.

The rectangular input bounds are outward rounded from pinned interval
certificates. This program independently bounds their quotient. It does
not derive the input boxes. Uses only the Python standard library.
"""
from fractions import Fraction as Q
from itertools import product


def bounds(low, high):
    return Q(low), Q(high)


def quotient(x, d, a, c):
    # I=x-id, W'=a-ic, with x,d,a,c positive on these boxes.
    denominator = a*a + c*c
    return (a*x + c*d)/denominator, (c*x - a*d)/denominator


def main():
    x = bounds(".08101", ".76583")
    d = bounds("2.78474", "3.48672")
    a = bounds("7.81873470886", "7.85596024942")
    c = bounds("1.69302372704", "1.73024927337")
    assert 0 < x[0] and 0 < d[0] and 0 < c[0] < c[1] < a[0]
    # Positive numerator of d_c Re(I/W') and d_a Im(I/W').
    assert d[0]*(a[0]**2 - c[1]**2) - 2*a[1]*c[1]*x[1] > 0
    # d_a Re < 0 and d_c Im > 0 follow directly from a > c.
    # Each component is affine in x,d at fixed a,c. All extrema
    # are therefore attained at corners of the four-dimensional box.
    real, imag = zip(*(quotient(*point) for point in product(x,d,a,c)))
    cover_real = bounds(".08285", ".18746")
    cover_imag = bounds("-.42383", "-.31759")
    assert cover_real[0] < min(real) <= max(real) < cover_real[1]
    assert cover_imag[0] < min(imag) <= max(imag) < cover_imag[1]
    assert Q(".08223") < cover_real[0] < cover_real[1] < Q(".18771")
    assert Q("-.42418") < cover_imag[0] < cover_imag[1] < Q("-.31707")
    print("exact rational cover:", tuple(map(str, cover_real)),
          "+ i", tuple(map(str, cover_imag)))
    print("approximate extrema:", *(float(v) for v in (min(real),max(real),min(imag),max(imag))))


if __name__ == "__main__":
    main()
