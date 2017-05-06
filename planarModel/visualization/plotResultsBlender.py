#!/usr/bin/python
import bpy
import sys
import math

worldsize = 16
input = "/home/quaczar/Documents/Projects/DLA/planarModel/data/largeRun.dat"
g = open(input, 'r')
x = []
y = []
z = []
cond = []

conditions = {"Collided", "Active", "Aggregator"}
colors = {"Collided":"b", "Active":"0.5", "Aggregator":"r"}
ln = g.readline().split(' ')
time = float(ln[1])
steps = float(ln[0])
for line in g:
 ln = line.split(' ')
 x.append(int(ln[0]))
 y.append(int(ln[1]))
 z.append(int(ln[2]))
 cond.append(int(ln[3]))

# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')

#Clear scene:
bpy.ops.object.select_by_type(type='MESH')
bpy.ops.object.delete()
print("Simulation Bodies in List: ", len(x))
value = 100
for i in range(0, value):
    xV = (x[i] - (worldsize / 2)) * 1
    yV = (y[i] - (worldsize / 2)) * 1
    zV = (z[i]) * 1

    print("Object ", i, "/", value)

    # red = makeMaterial('Red', (1,0,0), (1,1,1), 1)
    origin = (xV,yV,zV)
    bpy.ops.mesh.primitive_uv_sphere_add(location=origin)
bpy.ops.object.select_by_type(type='LAMP')

bpy.ops.object.select_by_type(type='CAMERA')
bpy.ops.transform.translate(value=(max(x), min(y), max(z)))