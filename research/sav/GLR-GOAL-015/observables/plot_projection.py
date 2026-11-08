"""Plot empirical cumulative projections and the certified endpoint intervals."""
import argparse
import hashlib

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
from scipy.integrate import cumulative_simpson

PANEL = [
    ("Early cut", .015, (3.3605, 12.3142),
     "38ecd9ed4d942aeb261cc197bf5662e9d80a584fa6dae955a322a61817915c39",
     "#17736d"),
    ("Full packet", .12, (-2758.258, -200.091),
     "f8039040c6b21af5780878e24f21584d13aa8285a58b4f36af54731162272b6b",
     "#88493a"),
]


def draw(paths, output):
    plt.rcParams.update({"font.family":"DejaVu Sans", "font.size":11,
                        "axes.spines.top":False, "axes.spines.right":False,
                        "axes.labelcolor":"#1e2d36", "text.color":"#1e2d36",
                        "xtick.color":"#334650", "ytick.color":"#334650",
                        "svg.fonttype":"none"})
    fig, axes=plt.subplots(1,2,figsize=(10,4.2),facecolor="white")
    for ax,path,(title,width,bounds,digest,color) in zip(axes,paths,PANEL):
        with np.load(path) as n:
            v=np.asarray(n["p_cap"],dtype="<f8")
            assert hashlib.sha256(v.tobytes()).hexdigest()==digest
            u=np.linspace(0,width,len(v))
            cumulative=cumulative_simpson(n["g_infty"]*n["p_infty"],x=u,initial=0)
        ax.set_title(title,loc="left",weight="bold")
        ax.axhline(0,color="#85939a",lw=1)
        ax.plot(u,cumulative,ls=(0,(4,3)),color="#344954",lw=2,
                label="Numerical cumulative projection")
        low,high=bounds
        xbar=width*1.035
        ax.vlines(xbar,low,high,color=color,lw=4,zorder=4,
                  label="Certified interval at endpoint")
        ax.hlines([low,high],xbar-width*.018,xbar+width*.018,color=color,lw=2)
        ax.set_xlim(-width*.01,width*1.09)
        ax.set_xticks(np.linspace(0,width,4))
        ax.set_xlabel(r"Elapsed retarded time $(u_{\rm phys}-u_{*,{\rm phys}})/r_0$")
        ax.grid(axis="y",color="#e8edef",lw=.8)
        ax.set_axisbelow(True)
    axes[0].set_ylabel(r"Accumulated ordered projection $\mathcal{P}$")
    fig.suptitle("Gregory–Laflamme tensor interference reverses sign across the packet",
                 x=.105,y=.97,ha="left",fontsize=14,weight="bold")
    fig.subplots_adjust(left=.105,right=.975,bottom=.27,top=.81,wspace=.33)
    handles, labels=axes[0].get_legend_handles_labels()
    fig.legend(handles,labels,loc="lower center",bbox_to_anchor=(.53,.035),
               ncol=2,frameon=False,fontsize=10)
    fig.savefig(output+".png",dpi=220,facecolor=fig.get_facecolor())
    fig.savefig(output+".svg",facecolor=fig.get_facecolor())


if __name__=="__main__":
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument("early")
    p.add_argument("packet")
    p.add_argument("output")
    args=p.parse_args()
    draw([args.early,args.packet],args.output)
