import numpy as np
import matplotlib.pylab as plt
import sys
from matplotlib.colors import LogNorm


"""
x = []
y = []
z = []
t = -1
f = open(sys.argv[1])

i = 0
for line in f:
    if (i == 0):
      t = float(line)
      print "Time is ", t
    else:
        line = line.split()
        x.append(float(line[1]))
        y.append(float(line[2]))
        z.append(float(line[3]))
    i+=1

f.close()
x = np.array(x)
y = np.array(y)
z = np.array(z)
"""

j = 0
plt.figure(figsize=(20,6))

colors = ['red', 'teal']
shapes = ['^', 's']
xbins = np.linspace(-200, 200, 100)

for f in sys.argv[1::2]:
    x = []
    y = []
    z = []
    t = -1

    f = open(f)

    i = 0
    for line in f:
        if (i == 0):
            t = float(line)
            print "Time is ", t
        else:
            line = line.split()
            x.append(float(line[1]))
            y.append(float(line[2]))
            z.append(float(line[3]))
        i+=1
            
    f.close()

    x = np.array(x)
    y = np.array(y)
    z = np.array(z) 
    f = open(sys.argv[2 * j + 2])
    
    diff = 99999.
    xh   = 0.
    yh   = 0.
    zh   = 0.
    
    for line in f:
        line = line.split()
        ht = float(line[0])
        if (abs(t - ht) < diff):
            diff = abs(t - ht)
            xh = float(line[1])
            yh = float(line[2])
            zh = float(line[3])
            
    f.close()

    x = x - xh
    y = y - yh
    z = z - zh

    #plt.figure(figsize=(20,6))

    plt.subplot(131)
    #plt.scatter(x,y,c=colors[j], marker=shapes[j])
    plt.hist2d(x,y,bins=(xbins,xbins),norm=LogNorm())
    plt.xlabel(r'$x$ (kpc/$h$)',fontsize=18)
    plt.ylabel(r'$y$ (kpc/$h$)',fontsize=18)
    plt.xlim(-200,200)
    plt.ylim(-200,200)
    
    plt.subplot(132)
    #plt.scatter(x,z,c=colors[j], marker=shapes[j])
    plt.hist2d(x,z,bins=(xbins,xbins),norm=LogNorm())
    plt.xlabel(r'$x$ (kpc/$h$)',fontsize=18)
    plt.ylabel(r'$z$ (kpc/$h$)',fontsize=18)
    plt.xlim(-200,200)
    plt.ylim(-200,200)
    
    
    plt.subplot(133)
    #plt.scatter(y,z,c=colors[j], marker=shapes[j])
    plt.hist2d(y,z,bins=(xbins,xbins),norm=LogNorm())
    plt.xlabel(r'$y$ (kpc/$h$)',fontsize=18)
    plt.ylabel(r'$z$ (kpc/$h$)',fontsize=18)
    plt.xlim(-200,200)
    plt.ylim(-200,200)
   
    j += 1
plt.show()
