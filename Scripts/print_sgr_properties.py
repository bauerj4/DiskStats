import numpy as np
import sys

MASS_CUT_LOW  = 4.e7
MASS_CUT_HIGH = 2.e8
R_CUT         = 0.075


a = float(sys.argv[3])

"""

Get halo position at a = sys.argv[3]

"""

f = open(sys.argv[1])

xhalo = 0.
yhalo = 0.
zhalo = 0.

diff = 9999999.
for line in f:
    line = line.split()
    t = float(line[0])
    x = float(line[1])
    y = float(line[2])
    z = float(line[3])
    
    if (np.fabs(t - a) < diff):
        diff  = np.fabs(t - a)
        xhalo = x
        yhalo = y
        zhalo = z

f.close()

print("Host halo is at", xhalo,yhalo,zhalo)

f = open(sys.argv[2])


for line in f:
    if (line[0] == "#"):
        pass
    else:
        line = line.split()
        m  = float(line[2])
        x  = float(line[8])  - xhalo/1000.
        y  = float(line[9])  - yhalo/1000.
        z  = float(line[10]) - zhalo/1000.
        vx = float(line[11])
        vy = float(line[12])
        vz = float(line[13])
        rv = float(line[5])
        rs = float(line[6])

        if (x*x + y*y + z*z < R_CUT*R_CUT and m > MASS_CUT_LOW and m < MASS_CUT_HIGH):
            print m,x + xhalo/1000.,y + yhalo/1000.,z + zhalo/1000.,vx,vy,vz,rv,rs
            print
