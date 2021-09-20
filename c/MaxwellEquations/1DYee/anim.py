# -*- coding: utf-8 -*-
# Gasper Senk
# try 1 pride delanju animacije

# berem z fila, tega predelam v matriko in to matriko v vrsticah izrisujem 
# na sliko in tako izdelam animacijo

import pylab 
import numpy 
import matplotlib.pyplot as plt
import matplotlib.animation as animation
data=numpy.loadtxt("PMaxwell_yeeE.txt")

# data shranim v 101 vrstic po 100 elementov, 1 je dodatna zaradi nicelnega casa
data=data.reshape(100,400)

# create figure windows
fig = plt.figure()
# create single axis in the figure
ax = plt.axes(xlim=(0, 100), ylim=(-10, 10))
# create line object which is modified for animation
line,=ax.plot([],[], lw=1)



# creates base upon frame so animation can take place
# important to return LINE data so animator knows what to update
def init():
    line.set_data([], [])
    return line,

# animation function.  This is called sequentially
def animate(i):
    x = numpy.linspace(1, 100, 400)
    y = data[i]
    line.set_data(x, y)
    return line,


anim = animation.FuncAnimation(fig, animate, init_func=init,
    frames=100, interval=25, blit=True)

anim.save('wave_propagation.mp4', fps=30, clear_temp=True)

plt.show()
