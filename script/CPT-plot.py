import matplotlib.pyplot as plt
import matplotlib.backends.backend_pdf as plt_backend

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
import matplotlib as mpl
from matplotlib import cm
from matplotlib.colors import ListedColormap, LinearSegmentedColormap


from PIL import Image
import urllib
import os, sys
import re

if (len(sys.argv) != 2):
    print("Usage: %s <experiment-directory>" % sys.argv[0])
    exit()

# Name of experiment directory. Will also be used as prefix for output
edir=sys.argv[1]
ename=edir.split("/")[-1]

# show preview
preview=False

scales = sorted([(int(m.group(1)), int(m.group(2)), f) for f in os.listdir(edir) if (m := re.search(r'-(\d+)x(\d+)\.', f))], key=lambda k: (k[0]*k[1], k[0]))

data={}
runtimes={}
multiscale = False
corescales = len(set([p*t for p,t,f in scales]))

if corescales != len(scales) and corescales != 1:
    multiscale = True    

# Colormap
top = cm.get_cmap('RdYlGn', 40)
bottom = cm.get_cmap('RdYlGn', 80)
newcolors = np.vstack((
    bottom([i**2*.5 for i in np.linspace(0, 1, 800)]),
    top(np.linspace(.5, .8, 200))
    ))
newcmp = ListedColormap(newcolors, name='pop')

for proc,threads,file in scales:
    scale = (proc, threads)
    try:
        with open(edir+"/"+file) as data_file:
            data[scale]={}
            for line in data_file:
                if line.startswith("=> Total runtime (in s):"):
                    runtimes[scale]=float(line.split(":")[1])
                if line.startswith("----------------POP"):
                    break
            for line in data_file:
                if line.startswith("----------------"):
                    break
                name, value = line.split(":")
                data[scale][name.strip()] = float(value)
    except FileNotFoundError:
        print("File not found: %s/%s"%(edir, file) )

def draw_table(mode):
    with plt_backend.PdfPages(mode + "_metrics.pdf") as pdf:
        rows = [
        "Parallel Efficiency",
        "  Load Balance",
        "  Communication Efficiency",
        "    Serialisation Efficiency",
        "    Transfer Efficiency",
        "  MPI Parallel Efficiency",
        "    MPI Load Balance",
        "    MPI Communication Efficiency",
        "      MPI Serialisation Efficiency",
        "      MPI Transfer Efficiency",
        "  OMP Parallel Efficiency",
        "    OMP Load Balance",
        "    OMP Communication Efficiency",
        "      OMP Serialisation Efficiency",
        "      OMP Transfer Efficiency",
        ]
        nrows = len(rows)+1
        ncols = len(scales)+1

        fwidth = 4.2

        fig, (ax, cbax) = plt.subplots(1,2,width_ratios=[ncols+fwidth,.25],figsize=((ncols+fwidth+1)*.7,6*.8), dpi=200)

        ax.set_xlim(0, ncols+fwidth-1)
        ax.set_ylim(0, nrows)
        ax.set_axis_off()

        summer = mpl.colormaps["summer"]
        autumn = mpl.colormaps["autumn"]

        cutpoint=.20

        for y, n in enumerate(data[tuple(scales[0][:2])].keys()):
            for x, (p, t, f) in enumerate(scales):
                scale = (p,t)
                ax.add_patch(
                    Rectangle(xy=(fwidth+x,nrows - y - 2), width=1,
                            height=1, facecolor=newcmp(data[scale][n]), zorder=0)
                )
                ax.annotate(
                    xy=(fwidth+.5+x,nrows - y - 1-.5),
                    text=round(100 * data[scale][n], 1),
                    ha='center',
                    va='center',
                    color = "white" if data[scale][n]<.5 else "black"
                )

        ax.annotate(
            xy=(fwidth - .1,nrows -.5),
            text="#ranks x #threads",
            weight='bold',
            ha='right',
            va='center'
        )

        for y, n in enumerate(rows):
            ax.annotate(
                xy=(.1,nrows - y - 1-.5),
                text=n,
                weight='bold',
                ha='left',
                va='center'
            )
        for x, (p,t,f) in enumerate(scales):
            scale = (p,t)
            ax.annotate(
                xy=(fwidth+.5+x,nrows-.5),
                text=str(p)+"x"+str(t),
                weight='bold',
                ha='center',
                va='center'
            )

        ax.plot([ax.get_xlim()[0], ax.get_xlim()[1]], [nrows, nrows], lw=3, color='black', marker='', zorder=4)
        ax.plot([ax.get_xlim()[0], ax.get_xlim()[1]], [nrows-1, nrows-1], lw=1.5, color='black', marker='', zorder=4)
        ax.plot([ax.get_xlim()[0], ax.get_xlim()[1]], [0, 0], lw=3, color='black', marker='', zorder=4)
        for x in range(1, nrows-1):
            lstyle=":"
            ax.plot([ax.get_xlim()[0], ax.get_xlim()[1]], [x, x], lw=1.15, color='gray', ls=lstyle, zorder=3 , marker='')

        ax.plot([0,0], [ax.get_ylim()[0], ax.get_ylim()[1]], lw=3, color='black', marker='', zorder=4)
        ax.plot([fwidth,fwidth], [ax.get_ylim()[0], ax.get_ylim()[1]], lw=1.5, color='black', marker='', zorder=4)
        ax.plot([ncols+fwidth-1,ncols+fwidth-1], [ax.get_ylim()[0], ax.get_ylim()[1]], lw=3, color='black', marker='', zorder=4)
        for y in range(1, ncols-1):
            lstyle = ":"
            c = 'gray'
            if multiscale and scales[y-1][0] * scales[y-1][1] != scales[y][0] * scales[y][1]:
                lstyle = "solid"
                c = 'black'
            ax.plot([y+fwidth, y+fwidth], [ax.get_ylim()[0], ax.get_ylim()[1]], lw=1.15, color=c, ls=lstyle, zorder=3 , marker='')

        cb = plt.colorbar(cm.ScalarMappable(cmap=newcmp), cax=cbax)

        fig.tight_layout()
        pdf.savefig()
        plt.savefig(
            mode + '_metrics.png',
            dpi=200,
            transparent=False
        )
        if preview:
            plt.show()
        plt.close()

def getAbsMetrics(series):
    absMetrics={}
    absMetrics["$TE_{omp}$"] = 1.
    absMetrics["$TE_{mpi}$"] = absMetrics["$TE_{omp}$"]*series['OMP Transfer Efficiency']
    absMetrics["$SerE_{omp}$"] = absMetrics["$TE_{mpi}$"] * series['MPI Transfer Efficiency']
    absMetrics["$SerE_{mpi}$"] = absMetrics["$SerE_{omp}$"] * series['OMP Serialisation Efficiency']
    absMetrics["$LB_{omp}$"] = absMetrics["$SerE_{mpi}$"] * series['MPI Serialisation Efficiency']
    absMetrics["$LB_{mpi}$"] = absMetrics["$LB_{omp}$"] * series['OMP Load Balance']
    absMetrics["$PE$"] = absMetrics["$LB_{mpi}$"] * series['MPI Load Balance']
    return absMetrics

def getRelMetrics(series):
    absMetrics = getAbsMetrics(series)
    relMetrics = {}
    relMetrics["$PE$"] = absMetrics["$PE$"]
    relMetrics["$LB_{mpi}$"] = absMetrics["$LB_{mpi}$"] - absMetrics["$PE$"]
    relMetrics["$LB_{omp}$"] = absMetrics["$LB_{omp}$"] - absMetrics["$LB_{mpi}$"]
    relMetrics["$SerE_{mpi}$"] = absMetrics["$SerE_{mpi}$"] - absMetrics["$LB_{omp}$"]
    relMetrics["$SerE_{omp}$"] = absMetrics["$SerE_{omp}$"] - absMetrics["$SerE_{mpi}$"]
    relMetrics["$TE_{mpi}$"] = absMetrics["$TE_{mpi}$"] - absMetrics["$SerE_{omp}$"]
    relMetrics["$TE_{omp}$"] = absMetrics["$TE_{omp}$"] - absMetrics["$TE_{mpi}$"]
    return relMetrics
    
def getAbsOmpMetrics(series):
    absOmpMetrics={}
    absOmpMetrics["$TE_{omp}$"] = 1.
    absOmpMetrics["$SerE_{omp}$"] = absOmpMetrics["$TE_{omp}$"] * series['OMP Transfer Efficiency']
    absOmpMetrics["$LB_{omp}$"] = absOmpMetrics["$SerE_{omp}$"] * series['OMP Serialisation Efficiency']
    absOmpMetrics["$PE_{omp}$"] = absOmpMetrics["$LB_{omp}$"] * series['OMP Load Balance']
    return absOmpMetrics

def getRelOmpMetrics(series):
    absOmpMetrics = getAbsOmpMetrics(series)
    relOmpMetrics = {}
    relOmpMetrics["$PE_{omp}$"] = absOmpMetrics["$PE_{omp}$"]
    relOmpMetrics["$LB_{omp}$"] = absOmpMetrics["$LB_{omp}$"] - absOmpMetrics["$PE_{omp}$"]
    relOmpMetrics["$SerE_{omp}$"] = absOmpMetrics["$SerE_{omp}$"] - absOmpMetrics["$LB_{omp}$"]
    relOmpMetrics["$TE_{omp}$"] = absOmpMetrics["$TE_{omp}$"] - absOmpMetrics["$SerE_{omp}$"]
    return relOmpMetrics
    
def getAbsMpiMetrics(series):
    absMpiMetrics={}
    absMpiMetrics["$TE_{mpi}$"] = 1.
    absMpiMetrics["$SerE_{mpi}$"] = absMpiMetrics["$TE_{mpi}$"] * series['MPI Transfer Efficiency']
    absMpiMetrics["$LB_{mpi}$"] = absMpiMetrics["$SerE_{mpi}$"] * series['MPI Serialisation Efficiency']
    absMpiMetrics["$PE_{mpi}$"] = absMpiMetrics["$LB_{mpi}$"] * series['MPI Load Balance']
    return absMpiMetrics

def getRelMpiMetrics(series):
    absMpiMetrics = getAbsMpiMetrics(series)
    relMpiMetrics = {}
    relMpiMetrics["$PE_{mpi}$"] = absMpiMetrics["$PE_{mpi}$"]
    relMpiMetrics["$LB_{mpi}$"] = absMpiMetrics["$LB_{mpi}$"] - absMpiMetrics["$PE_{mpi}$"]
    relMpiMetrics["$SerE_{mpi}$"] = absMpiMetrics["$SerE_{mpi}$"] - absMpiMetrics["$LB_{mpi}$"]
    relMpiMetrics["$TE_{mpi}$"] = absMpiMetrics["$TE_{mpi}$"] - absMpiMetrics["$SerE_{mpi}$"]
    return relMpiMetrics

def plot_data(mode):
    with plt_backend.PdfPages(mode + "_graph.pdf") as pdf:
        fig = plt.figure(figsize=(12, 6), dpi=200)
        ax1 = plt.subplot2grid(shape=(6, 3), loc=(1, 0), colspan=2, rowspan=5, fig=fig)
        ax2 = plt.subplot2grid(shape=(6, 3), loc=(0, 2), rowspan=2, fig=fig)
        ax3 = plt.subplot2grid(shape=(6, 3), loc=(2, 2), rowspan=2, fig=fig)
        ax4 = plt.subplot2grid(shape=(6, 3), loc=(4, 2), rowspan=2, fig=fig)
        axl = plt.subplot2grid(shape=(6, 3), loc=(0, 0), colspan=2, fig=fig, frameon=False)
        axes = [ax1, ax2, ax3, ax4, axl]


        blues = plt.get_cmap("Blues")
        oranges = plt.get_cmap("Oranges")
        greens = plt.get_cmap("Greens")
        reds = plt.get_cmap("Purples")
        DARK=.8
        LIGHT=.5
        
        rotation = 30
        lsize = "small"
        ssize = "xx-small"
        if len(scales) < 8:
            rotation = 0
            lsize = "medium"
            ssize = "small"
        if len(scales) < 6:
            rotation = 0
            lsize = "medium"
            ssize = "small"

        pdata={}
        COLORS = [reds(DARK), reds(LIGHT), greens(DARK), greens(LIGHT), oranges(DARK), oranges(LIGHT), blues(DARK)]
        COLORS.reverse()

        xticks = [str(p)+"x"+str(t) for p,t,f in scales]
        for p,t,f in scales:
            scale = p,t
            relMetrics = getRelMetrics(data[scale])
            labs = list(relMetrics.keys())
            for k,v in relMetrics.items():
                pdata[k]=pdata.get(k,[])+[v]
        y = list(pdata.values())
        ax1.stackplot(xticks, y, labels=labs, colors=COLORS)
        ax1.set_ylim([0,1])
        ax1.set_title("Hybrid breakdown")
        ax1.set_ylabel("Efficiency")
        ax1.set_xlabel("#ranks x #threads")
        handles, labels = ax1.get_legend_handles_labels()
        handles.reverse()
        labels.reverse()
        ax1.set_xticks(ax1.get_xticks())
        ax1.set_xticklabels(xticks, rotation=rotation, fontsize=lsize)
        
        axl.set_xticks([])
        axl.set_yticks([])


        pdata={}
        COLORS = [reds(DARK), greens(DARK), oranges(DARK), blues(DARK)]
        COLORS.reverse()

        for p,t,f in scales:
            scale = p,t
            relOmpMetrics = getRelOmpMetrics(data[scale])
            labs = list(relOmpMetrics.keys())
            for k,v in relOmpMetrics.items():
                pdata[k]=pdata.get(k,[])+[v]
        y = list(pdata.values())
        ax2.stackplot(xticks, y, labels=labs, colors=COLORS)
        ax2.set_ylim([0,1])
        ax2.set_title("OpenMP breakdown")
        ax2.set_ylabel("Efficiency")
        ax2.set_xlabel("#ranks x #threads")
        ax2.set_xticks(ax2.get_xticks())
        ax2.set_xticklabels(xticks, rotation=rotation, fontsize=ssize)

        pdata={}
        COLORS = [reds(LIGHT), greens(LIGHT), oranges(LIGHT), blues(DARK)]
        COLORS.reverse()

        for p,t,f in scales:
            scale = p,t
            relMpiMetrics = getRelMpiMetrics(data[scale])
            labs = list(relMpiMetrics.keys())
            
            for k,v in relMpiMetrics.items():
                pdata[k]=pdata.get(k,[])+[v]
        y = list(pdata.values())
        ax3.stackplot(xticks, y, labels=labs, colors=COLORS)
        ax3.set_ylim([0,1])
        ax3.set_title("MPI breakdown")
        ax3.set_ylabel("Efficiency")
        ax3.set_xlabel("Number of MPI ranks")
        ax3.set_xticks(ax3.get_xticks())
        ax3.set_xticklabels(xticks, rotation=rotation, fontsize=ssize)
        ax4.plot(xticks, list(runtimes.values()))
        ax4.set_ylabel("runtime in $s$")
        ax4.set_xlabel("Number of MPI ranks")
        ax4.set_ylim(bottom=0)
        ax4.set_xticks(ax4.get_xticks())
        ax4.set_xticklabels(xticks, rotation=rotation, fontsize=ssize)

        if multiscale:
            for y in range(len(scales)-1):
                lstyle = ":"
                c = 'gray'
                if scales[y][0] * scales[y][1] != scales[y+1][0] * scales[y+1][1]:
                    ax1.plot([y+.5, y+.5], [ax1.get_ylim()[0], ax1.get_ylim()[1]], lw=1.5, color="red", ls="solid", zorder=3 , marker='')
                    ax2.plot([y+.5, y+.5], [ax2.get_ylim()[0], ax2.get_ylim()[1]], lw=1, color="red", ls="solid", zorder=3 , marker='')
                    ax3.plot([y+.5, y+.5], [ax3.get_ylim()[0], ax3.get_ylim()[1]], lw=1, color="red", ls="solid", zorder=3 , marker='')
                    ax4.plot([y+.5, y+.5], [ax4.get_ylim()[0], ax4.get_ylim()[1]], lw=1, color="red", ls="solid", zorder=3 , marker='')

        axl.legend(handles, labels, loc='center', ncol=4)
        fig.tight_layout()
        pdf.savefig()
        plt.savefig(
            mode + '_graph.png',
            dpi=200,
            transparent=False
        )
        if preview:
            plt.show()
        plt.close()



plot_data(ename)
draw_table(ename)



