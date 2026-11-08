"""Bounded controls for GOAL-012 C. No physical hierarchy evolution."""
import cmath
import math
import platform

import flint
from flint import arb, ctx
import scipy
from scipy.integrate import quad


def certified_pole_norm():
    """Outward-rounded phase-independent Gram bound over one exact period."""
    ctx.prec = 96
    alpha_abs = arb("0.177924631", "0.00000005")
    beta_abs = arb("0.747343369", "0.00000005")
    modulus = (alpha_abs**2 + beta_abs**2).sqrt()
    period = 2 * arb.pi() / beta_abs
    amplitude_min = (arb("0.08223")**2 + arb("0.31707")**2).sqrt()
    lower_factor = modulus * amplitude_min * (
        (1 - (-2 * alpha_abs * period).exp()) * (1 / alpha_abs - 1 / modulus)
    ).sqrt()
    assert period > arb("8.40736018")
    assert period < arb("8.40736132")
    assert lower_factor > arb("0.50965")
    print("Acb/Arb 96-bit period:", period)
    print("Acb/Arb 96-bit phase-uniform lower factor:", lower_factor)


def gram_check():
    """Compare the exact two-phase formula with direct time quadrature."""
    pole = complex(-.177924631, -.747343369)
    amplitude = complex(.13523379, -.37062008)
    start = 4.0
    length = 2 * math.pi / abs(pole.imag)
    c = pole * amplitude * cmath.exp(pole * start)
    j = math.expm1(2 * pole.real * length) / (2 * pole.real)
    h = (cmath.exp(2 * pole * length) - 1) / (2 * pole)
    formula = 2 * abs(c)**2 * j + 2 * (c**2 * h).real
    direct, _ = quad(lambda u: (2 * (c * cmath.exp(pole * u)).real)**2,
                     0, length, epsabs=1e-12, epsrel=1e-12)
    error = abs(direct - formula)
    assert error < 2e-14
    print(f"Gram check: norm²={formula:.12f}, absolute difference={error:.2e}")


def positive_barrier_check():
    """Use the closed-form h for a unit square barrier and test flux sign."""
    gamma = .25
    for omega in (0., .7, 4., 16.):
        s = complex(gamma, omega)
        kappa = cmath.sqrt(s*s + 1)

        def amplitudes(x):
            cosh = cmath.cosh(kappa*x)
            sinh = cmath.sinh(kappa*x)
            h = cosh + s*sinh/kappa
            hx = kappa*sinh + s*cosh
            factor = cmath.exp(-s*x)/2
            return factor*(h + hx/s), factor*(h - hx/s)

        def production(x):
            a, b = amplitudes(x)
            return (1/s).real*abs(a+b)**2 + 4*gamma*abs(b)**2

        m_inf, b_edge = amplitudes(1)
        integral, _ = quad(production, 0, 1, epsabs=1e-11, epsrel=1e-11)
        error = abs(abs(m_inf)**2 - 1 - integral - abs(b_edge)**2)
        assert abs(m_inf) >= 1
        assert error < 3e-12
        print(f"Square barrier omega={omega:g}: |m_inf|={abs(m_inf):.10f}, "
              f"flux difference={error:.2e}")


def free_inversion_check():
    """Manufactured free Green solution N=-sin(pi*u)^3/2 on 0<u<1."""
    gamma = .4
    # Twice integrating by parts uses |(sin(pi*u)^3)''| <= 3*pi^2.
    coefficient = 3 * math.pi**2 / 2

    def exact(u):
        return -math.sin(math.pi*u)**3 / 2

    def news_transform(omega):
        s = complex(gamma, omega)
        return -3*math.pi/8 * (1 + cmath.exp(-s)) * (
            1/(s*s + math.pi**2) - 1/(s*s + 9*math.pi**2)
        )

    for band in (8., 16., 32.):
        def truncated(u):
            integral, _ = quad(
                lambda w: (cmath.exp(1j*w*u)*news_transform(w)).real,
                0, band, epsabs=1e-11, epsrel=1e-11,
            )
            return math.exp(gamma*u)*integral/math.pi

        error2, _ = quad(lambda u: (truncated(u)-exact(u))**2,
                         0, 1, epsabs=1e-11, epsrel=1e-10)
        bound = math.exp(gamma)*coefficient/(math.sqrt(3*math.pi)*band**1.5)
        assert math.sqrt(error2) < bound
        print(f"Free inversion B={band:g}: empirical L2 error={math.sqrt(error2):.6g}, "
              f"analytic cap={bound:.6g}")


if __name__ == "__main__":
    print(f"Python {platform.python_version()}, python-flint {flint.__version__}, "
          f"SciPy {scipy.__version__}")
    certified_pole_norm()
    gram_check()
    positive_barrier_check()
    free_inversion_check()
