"""Render the directed finite-cut intervals from the archived JSON."""
import argparse
from pathlib import Path
import json

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D
from matplotlib.ticker import MultipleLocator


ROOT = Path(__file__).resolve().parents[1]
CHECKS = ROOT / "transport" / "checks"


def center(text):
    return float(text.strip("[]").split(" +/- ")[0])


def intervals(path, radius):
    for item in json.loads(path.read_text())["radii"]:
        if item["r"] == radius:
            return [
                (center(row["mu"]), center(row["schwarzschild_signed_flux"]["lower"]) * 1e7,
                 center(row["schwarzschild_signed_flux"]["upper"]) * 1e7)
                for row in item["values"]
            ]
    raise ValueError(f"No interval record for radius {radius} in {path}")


def bar(ax, x, lower, upper, color):
    ax.vlines(x, lower, upper, color=color, lw=3.0, zorder=4)
    ax.hlines([lower, upper], x - 0.003, x + 0.003, color=color, lw=2.1, zorder=4)


def render(out, at24):
    fig, ax = plt.subplots(figsize=(8.2, 4.35), layout="constrained")
    fig.set_facecolor("#ffffff")
    for radius, offset, path, color in (
        (24, -0.009, at24, "#a85300"),
        (48, 0.009, CHECKS / "tube48.json", "#005da8"),
    ):
        for mass, lower, upper in intervals(path, radius):
            bar(ax, mass + offset, lower, upper, color)
    bar(ax, -0.026, 3.5316, 4.0171, "#465267")
    ax.set(
        xlim=(-0.041, 0.23), ylim=(0, 4.9),
        xlabel=r"Radion mass $\mu=m_\phi r_0$",
        ylabel=r"Signed energy coefficient $\mathcal{K}_{\mu,R}$  ($10^{-7}$)",
        xticks=[0, 0.10, 0.20], xticklabels=["0", "0.10", "0.20"],
    )
    ax.yaxis.set_major_locator(MultipleLocator(1))
    ax.grid(axis="y", color="#e3e8ef", lw=0.8)
    ax.tick_params(axis="both", color="#8590a1", labelcolor="#1a2a43")
    for spine in ("right", "top"):
        ax.spines[spine].set_visible(False)
    for spine in ("left", "bottom"):
        ax.spines[spine].set_color("#a4afbe")
    ax.set_title(
        r"Fixed Z source on the early cut $-10\leq u/r_0\leq -8$",
        loc="left", color="#465267", fontsize=11, pad=8,
    )
    fig.suptitle(
        "Radion energy through two extraction tubes",
        color="#14243d", fontsize=17, fontweight="semibold", x=0.095, ha="left",
    )
    ax.legend(
        [Line2D([0], [0], lw=3, color=c) for c in ("#465267", "#a85300", "#005da8")],
        [r"Massless at $\mathscr{I}^+$", r"$R/r_0=24$", r"$R/r_0=48$"],
        ncols=3, frameon=False, loc="upper right", fontsize=9.5,
    )
    fig.savefig(out, dpi=180, metadata={"Software": "Matplotlib 3.11.0"})


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("output", type=Path)
    parser.add_argument("--at24", type=Path, default=CHECKS / "tube24.json")
    args = parser.parse_args()
    render(args.output, args.at24)
