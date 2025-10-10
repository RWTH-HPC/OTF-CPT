#!/usr/bin/env python3

import numpy as np
import argparse
import os
import re

import matplotlib.pyplot as plt
from matplotlib import cm
import matplotlib.backends.backend_pdf as plt_backend
from matplotlib.patches import Rectangle
from matplotlib.colors import ListedColormap

parser = argparse.ArgumentParser(description='Plot CPT metrics from an experiment directory.')
parser.add_argument('experiment_directory', type=str, help='Path to the experiment directory containing output files. The script expects the relative or absolute path of this directory as an argument. The individual output files should follow the naming convention <prefix>-<nprocs>x<nthreads>.<suffix>.')
parser.add_argument('-o', '--out-dir', type=str, default='.', help='Path to the output directory where the plots will be saved.')
parser.add_argument('-p', '--prefix', type=str, default=None, help='Prefix of the output files to consider. If not specified, the experiment directory name will be used.')
parser.add_argument('--mpi-only', action='store_true', dest='mpi_only', help='Show only MPI metrics (if the application does not use OpenMP at all)')
parser.add_argument('--weak-scaling', action='store_true', dest='weak_scaling', help='Assume weak scaling for computational efficieny. Default is strong scaling.')
# Parse arguments
args = parser.parse_args()

# Name of experiment directory. Will also be used as prefix for output
edir=args.experiment_directory
if args.prefix is not None:
    ename=args.prefix
else:
    ename=os.path.basename(os.path.normpath(edir))

# show preview
preview=False

# Check if experiment dir exists
if not os.path.exists(edir):
    raise FileNotFoundError(f"Experiment directory {edir} does not exist.")

scales = sorted([(int(m.group(1)), int(m.group(2)), f) for f in os.listdir(edir) if (m := re.search(r'-(\d+)x(\d+)\.', f))], key=lambda k: (k[0]*k[1], k[0]))

if len(scales) == 0:
    raise ValueError(f"No valid output files found in {edir}. Ensure files follow the naming convention <prefix>-<nprocs>x<nthreads>.<suffix>.")

# Create output directory
os.makedirs(args.out_dir, exist_ok=True)

data={}
runtimes={}
multiscale = False
corescales = len(set([p*t for p,t,f in scales]))

if corescales != len(scales) and corescales != 1:
    multiscale = True    

# Colormap
top = plt.get_cmap('RdYlGn', 40)
bottom = plt.get_cmap('RdYlGn', 80)
newcolors = np.vstack((
    bottom([i**2*.5 for i in np.linspace(0, 1, 800)]),
    top(np.linspace(.5, .8, 200))
    ))
newcmp = ListedColormap(newcolors, name='pop')

for proc,threads,file in scales:
    scale = (proc, threads)
    try:
        with open(edir+"/"+file) as data_file:
            data[scale]={'Global Efficiency': -100} # We later rely on the ordering of the keys
            runtimes[scale] = {}
            for line in data_file:
                if line.startswith("=> Average Computation (in s):"):
                    runtimes[scale]['computation']=float(line.split(":")[1])
                if line.startswith("=> Total runtime (in s):"):
                    runtimes[scale]['total']=float(line.split(":")[1])
                if line.startswith("----------------POP"):
                    break
            for line in data_file:
                if line.startswith("----------------"):
                    break
                name, value = line.split(":")
                data[scale][name.strip()] = float(value)
    except FileNotFoundError:
        print("File not found: %s/%s"%(edir, file) )

basescale = next(iter(data))
for scale in data.keys():
    uct_base = runtimes[basescale]['computation']*basescale[0]*basescale[1]
    uct_series = runtimes[scale]['computation']*scale[0]*scale[1]
    if uct_base == 0.0 or uct_series == 0.0:
        print("Detected 0.0s runtime: defaulting to Computational Scalability of 1")
        data[scale]["Computational Scalability"] = 1.0
    else:
        data[scale]["Computational Scalability"] = uct_base / uct_series
        if args.weak_scaling:
            data[scale]["Computational Scalability"] *= (scale[0]*scale[1])/(basescale[0]*basescale[1])
    data[scale]["Global Efficiency"] = data[scale]["Parallel Efficiency"] * data[scale]["Computational Scalability"]

def draw_table(mode):
    with plt_backend.PdfPages(os.path.join(args.out_dir, mode + '_metrics.pdf')) as pdf:
        rows = [
        "Global Efficiency",
        "  Parallel Efficiency",
        "    Load Balance",
        "    Communication Efficiency",
        "      Serialisation Efficiency",
        "      Transfer Efficiency"
        ]
        if not args.mpi_only:
            rows.extend([
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
                ])
        rows.append("  Computational Scalability")
        nrows = len(rows)+1
        ncols = len(scales)+1

        fwidth = 4.2

        fig, (ax, cbax) = plt.subplots(1,2,width_ratios=[ncols+fwidth,.25],figsize=((ncols+fwidth+1)*.7,6*.8), dpi=200)

        ax.set_xlim(0, ncols+fwidth-1)
        ax.set_ylim(0, nrows)
        ax.set_axis_off()

        metric_names = data[basescale].keys()
        if args.mpi_only:
            metric_names = [m for m in metric_names if not any(sub in m for sub in ['OMP', 'MPI'])]
        for y, n in enumerate(metric_names):
            for x, (p, t, _) in enumerate(scales):
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
            text="#Ranks" if args.mpi_only else "#Ranks x #Threads",
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

        plt.colorbar(cm.ScalarMappable(cmap=newcmp), cax=cbax)

        fig.tight_layout()
        pdf.savefig()
        plt.savefig(
            os.path.join(args.out_dir, mode + '_metrics.png'),
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
    with plt_backend.PdfPages(os.path.join(args.out_dir, mode + "_graph.pdf")) as pdf:
        fig = plt.figure(figsize=(12, 6), dpi=200)
        ax_tot = plt.subplot2grid(shape=(6, 3), loc=(1, 0), colspan=2, rowspan=5, fig=fig)
        if not args.mpi_only:
            ax_omp = plt.subplot2grid(shape=(6, 3), loc=(0, 2), rowspan=2, fig=fig)
            ax_mpi = plt.subplot2grid(shape=(6, 3), loc=(2, 2), rowspan=2, fig=fig)
        ax_scaling = plt.subplot2grid(shape=(6, 3), loc=(4, 2), rowspan=2, fig=fig)
        ax_legend = plt.subplot2grid(shape=(6, 3), loc=(0, 0), colspan=2, fig=fig, frameon=False)


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
        ax_tot.stackplot(xticks, y, labels=labs, colors=COLORS)
        ax_tot.set_ylim(0,1)
        if args.mpi_only:
            ax_tot.set_title("Breakdown")
            ax_tot.set_xlabel("#ranks")
        else:
            ax_tot.set_title("Hybrid breakdown")
            ax_tot.set_xlabel("#ranks x #threads")
        ax_tot.set_ylabel("Efficiency")
        handles, labels = ax_tot.get_legend_handles_labels()
        handles.reverse()
        labels.reverse()
        ax_tot.set_xticks(ax_tot.get_xticks())
        ax_tot.set_xticklabels(xticks, rotation=rotation, fontsize=lsize)

        ax_legend.set_xticks([])
        ax_legend.set_yticks([])

        if not args.mpi_only:
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
            ax_omp.stackplot(xticks, y, labels=labs, colors=COLORS)
            ax_omp.set_ylim(0,1)
            ax_omp.set_title("OpenMP breakdown")
            ax_omp.set_ylabel("Efficiency")
            ax_omp.set_xlabel("#ranks x #threads")
            ax_omp.set_xticks(ax_omp.get_xticks())
            ax_omp.set_xticklabels(xticks, rotation=rotation, fontsize=ssize)


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
            ax_mpi.stackplot(xticks, y, labels=labs, colors=COLORS)
            ax_mpi.set_ylim(0,1)
            ax_mpi.set_title("MPI breakdown")
            ax_mpi.set_ylabel("Efficiency")
            ax_mpi.set_xlabel("Number of MPI ranks")
            ax_mpi.set_xticks(ax_mpi.get_xticks())
            ax_mpi.set_xticklabels(xticks, rotation=rotation, fontsize=ssize)

            
        ax_scaling.plot(xticks, [s['total'] for s in runtimes.values()])
        ax_scaling.set_ylabel("runtime in $s$")
        ax_scaling.set_xlabel("Number of MPI ranks")
        ax_scaling.set_ylim(bottom=0)
        ax_scaling.set_xticks(ax_scaling.get_xticks())
        ax_scaling.set_xticklabels(xticks, rotation=rotation, fontsize=ssize)

        if multiscale:
            for y in range(len(scales)-1):
                if scales[y][0] * scales[y][1] != scales[y+1][0] * scales[y+1][1]:
                    ax_tot.plot([y+.5, y+.5], [ax_tot.get_ylim()[0], ax_tot.get_ylim()[1]], lw=1.5, color="red", ls="solid", zorder=3 , marker='')
                    if not args.mpi_only:
                        ax_omp.plot([y+.5, y+.5], [ax_omp.get_ylim()[0], ax_omp.get_ylim()[1]], lw=1, color="red", ls="solid", zorder=3 , marker='')
                        ax_mpi.plot([y+.5, y+.5], [ax_mpi.get_ylim()[0], ax_mpi.get_ylim()[1]], lw=1, color="red", ls="solid", zorder=3 , marker='')
                    ax_scaling.plot([y+.5, y+.5], [ax_scaling.get_ylim()[0], ax_scaling.get_ylim()[1]], lw=1, color="red", ls="solid", zorder=3 , marker='')

        ax_legend.legend(handles, labels, loc='center', ncol=4)
        fig.tight_layout()
        pdf.savefig()
        plt.savefig(
            os.path.join(args.out_dir, mode + '_graph.png'),
            dpi=200,
            transparent=False
        )
        if preview:
            plt.show()
        plt.close()


plot_data(ename)
draw_table(ename)



