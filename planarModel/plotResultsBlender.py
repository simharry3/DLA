#!/usr/bin/python
import bpy
import sys
import math


input = "/home/quaczar/Documents/RPI/2017/spring/pcomp/project/parallelPlanes/laptopData/l4-128-8192.dat"
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
value = 100
for i in range(0, value):
    xV = (x[i]-30) * .95
    yV = (y[i]-30) * .95
    zV = (z[i]) * .8

    print("Object ", i, "/", value)

    # red = makeMaterial('Red', (1,0,0), (1,1,1), 1)
    origin = (xV,yV,zV)
    bpy.ops.mesh.primitive_uv_sphere_add(location=origin)
bpy.ops.object.select_by_type(type='LAMP')

# bpy.ops.object.select_by_type(type='CAMERA')