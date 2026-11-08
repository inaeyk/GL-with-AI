#!/usr/bin/env python3
"""Non-production Arb enclosure of the L10 optical coefficient at the RW barrier."""
import importlib.util
from pathlib import Path
import platform

import flint
from flint import arb

SOURCE = Path(__file__).resolve().parents[2] / "GLR-GOAL-005/compute/gl_eigenvalue.py"
SPEC = importlib.util.spec_from_file_location("goal005_spectral", SOURCE)
spectral = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(spectral)

OMEGA = arb("0.06328118").union(arb("0.06328120"))
K2 = (arb.pi() / 5)**2


def x(r):
    return r + (r - 1).log()


def radial_ratio(r, p):
    """(H_tt/f + 2 H_tr + f H_rr)/(U/r) in the unitary representative."""
    d = 1 + K2 * r**3
    f = 1 - 1/r
    h = 3*K2*r**2/d - 1/r + p/f
    return ((2 - K2*r**3)/d - 2*OMEGA**2/(K2*f)
            + OMEGA/(K2*r*(r - 1))
            + ((2*r - 1)/(K2*r**2) - 6*r*(r - 1)/d - 2*OMEGA/K2)*h)


def propagate(cells, start_u, take):
    """Cells have propagation orientation. Integrals have physical dx>0."""
    u = start_u
    total = arb(0)
    for r0, r1, p, dx in cells:
        step = p*dx
        if max(r0, r1) > take:
            low, high = max(min(r0, r1), take), max(r0, r1)
            r = low.union(high)
            seg = x(high) - x(low)
            u_cell = u * (p * arb(0).union(dx)).exp()
            density = (OMEGA*x(r)).exp()*u_cell*radial_ratio(r, p)/(2*r)
            total += density*seg
        u *= step.exp()
    return total, u


def main():
    p2_left, left_status, left = spectral.validate_branch(OMEGA, True, True)
    p2_right, right_status, right = spectral.validate_branch(OMEGA, False, True)
    p2 = p2_left.intersection(p2_right)
    r = arb(2)
    d = 1 + K2*r**3
    h2 = 3*K2*r**2/d - 1/r + p2/(1 - 1/r)
    # H_theta theta/r^2=2 fixes U(2) including sign.
    u2 = 2*r/(-arb(1)/2 - (r - 1)*h2/(K2*r**2))
    left = [(b, a, p, -dx) for a, b, p, dx in reversed(left)]
    right = [(b, a, p, -dx) for a, b, p, dx in reversed(right)]
    print("Python", platform.python_version(), "python-flint", flint.__version__,
          "NumPy", spectral.np.__version__, "SciPy", spectral.scipy.__version__)
    print("p(2) =", p2, "U(2) =", u2)
    print("Riccati left", left_status)
    print("Riccati right", right_status)
    # From r=60 onwards the pinned p bracket [-.65,-.60] persists.
    # Elementary bounds in radial_ratio give |ratio|<5 there.
    right_value, u60 = propagate(right, u2, arb(2))
    tail_bound = (5*u60.abs_upper()*(OMEGA*x(arb(60))).exp()
                  / (120*(arb(".60") - OMEGA))).upper()
    for radius in [arb(13)/8, arb(7)/4]:
        left_value, _ = propagate(left, u2, radius)
        t_launch = -(radius - 1).log()  # tau=v-r=0
        chi = ((OMEGA*(t_launch - x(radius))).exp()
               * (left_value + right_value + arb(0, tail_bound)))
        coefficient = chi**2/4
        assert chi < 0 or chi > 0
        print(f"R={radius}  chi={chi}  chi^2/4={coefficient}")
        print("  chi endpoints:", chi.lower(), chi.upper())
        print("  coefficient endpoints:", coefficient.lower(), coefficient.upper())
    print("weighted tail absolute upper bound:", tail_bound)


if __name__ == "__main__":
    main()
