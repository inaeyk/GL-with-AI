"""Non-directed quadrature check on the same specified affine pilot ordinates."""

import argparse
from platform import python_version

import numpy as np
import scipy
from scipy.special import roots_legendre


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("candidate", help="NPZ produced by checks/late_march.py")
    args = parser.parse_args()
    with np.load(args.candidate) as saved:
        u = saved["u"].copy()
        return_news = saved["p_shear_infty"][np.searchsorted(saved["u_full"], 0.0):]
    assert u[0] == 0.0 and u[-1] == float("8.407362") and len(u) == len(return_news)
    legendre_nodes, legendre_weights = roots_legendre(12)
    fraction = (legendre_nodes + 1) / 2
    width = np.diff(u)
    sample_u = u[:-1, None] + width[:, None] * fraction[None, :]
    sample_news = return_news[:-1, None] + np.diff(return_news)[:, None] * fraction[None, :]
    weight = width[:, None] * legendre_weights[None, :] / 2
    pole_q = complex(-0.177924631, -0.747343369)
    pole_star = pole_q + 0.12656238
    q = 2 * np.real(pole_q * complex(.13497, -.370625) * np.exp(pole_q * sample_u))
    star = 2 * np.real(pole_star * complex(-.193535, -.424625) * np.exp(pole_star * sample_u))
    print(f"Python {python_version()}, NumPy {np.__version__}, SciPy {scipy.__version__}")
    for label, difference in (
        ("n", sample_news), ("n-Q", sample_news-q), ("n-Q-S*", sample_news-q-star)
    ):
        print(f"||{label}||2 {np.sqrt(np.sum(weight * difference**2)):.16g}")


if __name__ == "__main__":
    main()
