"""Render the illustrative later-window waveform comparison from the pilot NPZ."""
from argparse import ArgumentParser

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    parser = ArgumentParser(description=__doc__)
    parser.add_argument("candidate")
    parser.add_argument("output")
    args = parser.parse_args()
    with np.load(args.candidate) as data:
        u = data["u"]
        q, star = data["q"], data["star"]
        later = data["u_full"] >= 0
        shear, commuted = data["p_shear_infty"][later], data["p_infty"][later]

    plt.rcParams.update({
        "font.family": "DejaVu Sans", "font.size": 11,
        "axes.spines.top": False, "axes.spines.right": False,
        "axes.labelcolor": "#202732", "xtick.color": "#465264",
        "ytick.color": "#465264", "figure.facecolor": "white",
    })
    fig, ax = plt.subplots(2, 1, figsize=(8.4, 6.0), sharex=True,
                           gridspec_kw={"height_ratios": [1.18, 1]},
                           layout="constrained")
    navy, teal, ochre = "#19334a", "#007b79", "#aa5414"
    ax[0].plot(u, q, color=ochre, ls="--", lw=1.5, label=r"$Q$")
    ax[0].plot(u, q + star, color=teal, lw=1.6, label=r"$Q+S_*$")
    ax[0].plot(u, shear, color=navy, lw=2.2, label=r"Return candidate $n_{21}$")
    ax[0].set_title("Later ordered tensor news and the fixed pole templates",
                    loc="left", fontweight="bold", pad=10)
    ax[0].set_ylabel("Ordered news")
    ax[0].set_ylim(-1.31, 1.14)
    ax[0].legend(frameon=False, ncol=3, loc="upper center")

    ax[1].plot(u, shear, color=navy, lw=2.0, label="Shear derivative")
    ax[1].plot(u, commuted, color="#79529a", lw=1.15, ls="--",
               label="Commuted-current readout")
    ax[1].set_title("Numerical readouts of the same sampled hierarchy",
                    loc="left", fontsize=11, pad=8)
    ax[1].set_xlabel(r"Retarded time $u_{\rm phys}/r_0$")
    ax[1].set_ylabel("Ordered news, zoom")
    ax[1].set_ylim(-0.066, 0.079)
    ax[1].legend(frameon=False, ncol=2, loc="upper right")

    for panel in ax:
        panel.set_xlim(0, u[-1])
        panel.axhline(0, color="#adb6bf", lw=0.7, zorder=0)
        panel.grid(axis="x", color="#e0e5e9", linewidth=0.65)
    fig.supxlabel("Empirical return candidate  •  Midpoint pole parameters  •  Physical error open",
                  color="#465264", fontsize=9)
    fig.savefig(args.output, dpi=150, metadata={"Software": "Matplotlib"})


if __name__ == "__main__":
    main()
