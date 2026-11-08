#!/usr/bin/env python3
"""Exact manufactured constant-mass kernel and independent moment checks."""
from math import comb, factorial

from flint import arb, fmpq

from tube_intervals import kernel_integrals


def enclosure(terms, next_term):
    return arb(terms).union(arb(terms + next_term))


def moment_term(n, order, p, k, d, c):
    integral = sum(fmpq(comb(n, j)) * c**(n - j) * d**(n + j + p + 1)
                   / (n + j + p + 1) for j in range(n + 1))
    return (k**n * factorial(order) / (factorial(n) * factorial(order + n))) * integral


def check_moments():
    d, c, k = fmpq(3), fmpq(7), fmpq(1, 100)
    assert k * d * (c + d) < 1  # all correct series terms decrease
    actual = kernel_integrals(arb(d), arb(c), arb(0), arb(k), .25)
    for value, (nu, p) in zip(actual, ((0, 0), (1, 0), (1, 1))):
        terms = [moment_term(n, nu, p, k, d, c) for n in range(17)]
        exact = enclosure(sum((-1)**n * terms[n] for n in range(16)), terms[-1])
        wrong_lower = arb(sum(terms[:16]))
        assert value.contains(exact)
        assert value < wrong_lower
        print(f"order={nu}, moment={p}, correct enclosure contains exact series; "
              "wrong mass sign excluded")


def check_zero_data_triangle():
    # For flat t=0 and F=1, integrate the Riemann series on p,q>=0,p+q<=2T.
    # Exact Dirichlet moment is (2T)^(2n+2) (n!)^2/(2n+2)!.
    mu, T = fmpq(1, 5), fmpq(3)
    for n in range(17):
        direct = ((-mu**2 / 4)**n * (2 * T)**(2 * n + 2)
                  * factorial(n)**2 / (4 * factorial(n)**2 * factorial(2 * n + 2)))
        exact = (-mu**2)**n * T**(2 * n + 2) / factorial(2 * n + 2)
        assert direct == exact
    t = arb(T)
    m = arb(mu)
    series = sum((-mu**2)**n * T**(2 * n + 2) / factorial(2 * n + 2)
                 for n in range(16))
    next_term = (-mu**2)**16 * T**34 / factorial(34)
    assert enclosure(series, next_term).overlaps((1 - (m * t).cos()) / m**2)
    print("zero-data causal triangle matches (1-cos(mu*T))/mu^2")


if __name__ == "__main__":
    check_moments()
    check_zero_data_triangle()
