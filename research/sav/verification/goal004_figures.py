#!/usr/bin/env python3
"""Plot saved GOAL004 heuristic predictions; no new calculation campaign."""
from pathlib import Path
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

HERE=Path(__file__).resolve().parent
plt.rcParams.update({'font.size':10,'axes.spines.top':False,'axes.spines.right':False})
fig,axs=plt.subplots(1,3,figsize=(13,3.6),layout='constrained')
for nmodes in (1,2):
    for p,col in [('4_0','#246aa5'),('4_5','#ba542a')]:
        name='goal004_model' if nmodes==1 else 'goal004_two_mode'
        z=np.genfromtxt(HERE/f'{name}_p{p}.csv',delimiter=',',names=True)
        style='--' if nmodes==1 else '-'
        label=f'{nmodes} mode'+('s' if nmodes>1 else '')+', p='+p.replace('_','.')
        axs[0].plot(z['t'],z['contrast'],style,color=col,label=label)
        news=z['N_relax'] if nmodes==1 else z['N']
        energy=z['E_relax_over_M0'] if nmodes==1 else z['E_over_M0']
        axs[1].plot(z['t'],news,style,color=col)
        axs[2].semilogy(z['t'][1:],energy[1:],style,color=col)
for ax in axs:
    ax.set_xlim(40,150);ax.set_xlabel(r'$t/r_0$');ax.grid(alpha=.18)
axs[0].set_ylabel('radius contrast proxy');axs[0].legend(fontsize=8,loc='upper left')
axs[1].set_ylabel(r'model scalar news $N_\phi$')
axs[2].set_ylabel(r'model $E_{\rm early}/M_0$');axs[2].set_ylim(1e-10,1e-4)
fig.suptitle('GOAL-004: matched effective models — nonlinear errors uncontrolled',fontsize=12)
fig.savefig(HERE/'goal004_model_comparison.png',dpi=160)
print('Saved goal004_model_comparison.png; matplotlib',matplotlib.__version__)
