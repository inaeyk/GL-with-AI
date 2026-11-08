"""Directed finite-window Gram bounds from the pinned shear residue boxes."""

from itertools import product
from platform import python_version

import flint
from flint import arb, arb_mat, ctx


ctx.prec = 192
LENGTH = arb("8.407362")
ALPHA_Q = arb("-.177924631")
ALPHA_STAR = ALPHA_Q + 2 * arb(".06328119")
BETA = arb("-.747343369")
Q_DISK = arb("5e-8")
STAR_DISK = Q_DISK + arb("2e-8")
Q_BOX = (
    (arb(".08223"), arb(".18771")),
    (arb("-.42418"), arb("-.31707")),
)
STAR_BOX = (
    (arb("-.21850"), arb("-.16857")),
    (arb("-.44959"), arb("-.39966")),
)


def real_basis(alpha):
    """Each column is exp(alpha*u) times c*cos(beta*u)+s*sin(beta*u)."""
    return ((alpha, 2 * alpha, -2 * BETA), (alpha, -2 * BETA, -2 * alpha))


def gram_entry(left, right):
    alpha, c1, s1 = left
    gamma, c2, s2 = right
    decay = alpha + gamma
    frequency = 2 * BETA
    phase = frequency * LENGTH
    exponent = (decay * LENGTH).exp()
    denominator = decay**2 + frequency**2
    zero = (decay * LENGTH).expm1() / decay
    cosine = (
        exponent * (decay * phase.cos() + frequency * phase.sin()) - decay
    ) / denominator
    sine = (
        exponent * (decay * phase.sin() - frequency * phase.cos()) + frequency
    ) / denominator
    cc = (zero + cosine) / 2
    ss = (zero - cosine) / 2
    cs = sine / 2
    return c1 * c2 * cc + s1 * s2 * ss + (c1 * s2 + s1 * c2) * cs


def quadratic(gram, point):
    return sum(
        gram[i][j] * point[i] * point[j]
        for i in range(len(point))
        for j in range(len(point))
    )


def lower_at_stationary_face(gram, box):
    """Find and certify a box KKT point for the convex Gram quadratic."""
    size = len(box)
    # State 0 is lower active, 1 is upper active, 2 is free.
    for states in product(range(3), repeat=size):
        free = [i for i, state in enumerate(states) if state == 2]
        active = [i for i, state in enumerate(states) if state != 2]
        point = [None] * size
        for i in active:
            point[i] = box[i][states[i]]
        if free:
            matrix = arb_mat([[gram[i][j] for j in free] for i in free])
            rhs = arb_mat([[-sum(gram[i][j] * point[j] for j in active)] for i in free])
            solution = matrix.solve(rhs)
            for index, i in enumerate(free):
                point[i] = solution[index, 0]
        if not all(box[i][0] < point[i] < box[i][1] for i in free):
            continue
        active_gradient = {
            i: sum(gram[i][j] * point[j] for j in range(size)) for i in active
        }
        if all(
            active_gradient[i] > 0 if states[i] == 0 else active_gradient[i] < 0
            for i in active
        ):
            return quadratic(gram, point).sqrt().lower(), states
    raise ArithmeticError("No directed KKT certificate found")


def unit_frequency_error(alpha, disk):
    assert alpha + disk < 0
    center_size = (alpha**2 + BETA**2).sqrt()
    return (2 * disk * (1 + (center_size + disk) * LENGTH) * LENGTH.sqrt()).upper()


def frequency_error(alpha, box, disk):
    """Segment derivative bound with residue held fixed along the segment."""
    maximum = (
        max(abs(endpoint).upper() for endpoint in box[0]) ** 2
        + max(abs(endpoint).upper() for endpoint in box[1]) ** 2
    ).sqrt()
    return (maximum * unit_frequency_error(alpha, disk)).upper()


def interval_report(label, lower, upper):
    print(f"{label}: lower {lower.str(17)}, upper {upper.str(17)}")


def report(name, basis, box, frequency_cap):
    gram = [[gram_entry(left, right) for right in basis] for left in basis]
    midpoint = tuple((a + b) / 2 for a, b in box)
    corners = list(product(*box))
    base_lower, states = lower_at_stationary_face(gram, box)
    base_upper = max(quadratic(gram, point).sqrt().upper() for point in corners)
    center = quadratic(gram, midpoint).sqrt()
    fixed_frequency_radius = max(
        quadratic(gram, tuple(x - mid for x, mid in zip(point, midpoint))).sqrt().upper()
        for point in corners
    )
    lower = (base_lower - frequency_cap).lower()
    upper = (base_upper + frequency_cap).upper()
    radius = (fixed_frequency_radius + frequency_cap).upper()
    print(f"{name}: active-face states {states}")
    print(f"  midpoint {center.str(17)}")
    interval_report("  norm", lower, upper)
    print(f"  radius upper {radius.str(17)}")
    print(f"  pass allowance lower {(lower/2-radius).lower().str(17)}")
    print(f"  reject subtractand upper {(upper/2+radius).upper().str(17)}")
    return lower, upper, radius, center


def translated_schur(star_upper, star_frequency):
    """Distance of the isolated translated pair from the ordinary pair span."""
    q_basis = real_basis(ALPHA_Q)
    star_basis = real_basis(ALPHA_STAR)
    q_gram = arb_mat([[gram_entry(x, y) for y in q_basis] for x in q_basis])
    star_gram = arb_mat([[gram_entry(x, y) for y in star_basis] for x in star_basis])
    cross = arb_mat([[gram_entry(x, y) for y in star_basis] for x in q_basis])
    schur = star_gram - cross.transpose() * q_gram.solve(cross)
    gram = [[schur[i, j] for j in range(2)] for i in range(2)]
    # The exact Schur complement is positive semidefinite as a projected Gram.
    lower, states = lower_at_stationary_face(gram, STAR_BOX)
    upper = max(
        quadratic(gram, point).sqrt().upper() for point in product(*STAR_BOX)
    )
    left, right, off = q_gram[0, 0], q_gram[1, 1], q_gram[0, 1]
    singular_min = ((left + right - ((left - right)**2 + 4 * off**2).sqrt()) / 2).sqrt()
    q_frequency = unit_frequency_error(ALPHA_Q, Q_DISK)
    assert singular_min > q_frequency
    # The best-fit coefficients on the perturbed and center subspaces are
    # bounded by the target's norm divided by each operator's smallest singular
    # value. Use one bound for both directions of the comparison.
    coefficient_cap = (star_upper + star_frequency) / (singular_min - q_frequency)
    perturbation = star_frequency + coefficient_cap * q_frequency
    interval_report(
        f"Isolated S* distance to ordinary span (active face {states})",
        (lower - perturbation).lower(), (upper + perturbation).upper(),
    )


def main():
    print(f"Python {python_version()}, python-flint {flint.__version__}, precision {ctx.prec}")
    q_frequency = frequency_error(ALPHA_Q, Q_BOX, Q_DISK)
    star_frequency = frequency_error(ALPHA_STAR, STAR_BOX, STAR_DISK)
    q = report("Q", real_basis(ALPHA_Q), Q_BOX, q_frequency)
    star = report("S*", real_basis(ALPHA_STAR), STAR_BOX, star_frequency)
    total = report(
        "Q+S*", real_basis(ALPHA_Q) + real_basis(ALPHA_STAR),
        Q_BOX + STAR_BOX, q_frequency + star_frequency,
    )
    assert q[0] > arb(".59933") and q[1] < arb(".87548")
    assert total[0] > arb("1.51525") and total[1] < arb("1.99487")
    print(f"Necessary other-term norm {(star[0]-q[1]/2).lower().str(17)}")
    print(f"S* norm minus half combined norm {(star[0]-total[1]/2).lower().str(17)}")
    # On the center templates alone, both half-relative-error open balls intersect.
    overlap = (q[3] + total[3]) / 2 - star[3]
    assert overlap > 0
    print(f"Midpoint ball overlap slack {overlap.str(17)}")
    translated_schur(star[1], star_frequency)
    worst_period = 2 * arb.pi() / (abs(BETA) - Q_DISK)
    assert LENGTH > worst_period
    print(f"Fixed cut minus largest fundamental period {(LENGTH-worst_period).lower().str(17)}")


if __name__ == "__main__":
    main()
