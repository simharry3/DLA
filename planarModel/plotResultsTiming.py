#!/usr/bin/python
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from mpl_toolkits.mplot3d import Axes3D
# from mayavi import mlab
import os
import sys
import pylab as lab
import subprocess
import math as ma
import timeit

files = ["timingDataLaptop.dat", "bgTimingData128.dat", "bgTimingData256.dat"]
conditions = ["i5-3320M\nWorld Dimension: 128", "BG/Q\nWorld Dimension: 128", "BG/Q\nWorld Dimension: 256"]
colors = ["b", "g", "r"]
i = 0

patch = []
fig = plt.figure()
ax = fig.add_subplot(111)
for element in files:
    input = element
    g = open(input, 'r')
    ranks = []
    worldSize = []
    particles = []
    time = []
    for line in g:
        ln = line.split(' ')
        ranks.append(int(ln[0]))
        worldSize.append(int(ln[1]))
        particles.append(int(ln[2]))
        time.append(float(ln[3]))

    ax.plot(ranks, time, marker="o", color=colors[i])

    patch.append(mpatches.Patch(label=conditions[i], color=colors[i]))
    i+=1

plt.legend(handles=patch, loc="upper right")
plt.xlabel("Number of Execution Ranks")
plt.ylabel("Execution Time (s)")
plt.title("Time Required to Execute 50000 Timestep DLA vs Number of Execution Ranks")
plt.show()