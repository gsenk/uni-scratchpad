"""
An animated image
"""
import pylab 
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

fig = plt.figure()
data=np.loadtxt("Maxwell_PMLB.txt")

x_axis=100
y_axis=100
t_axis=301
data=data.reshape(t_axis, x_axis*y_axis)


im = plt.imshow(data[0].reshape(y_axis, x_axis),
                cmap=plt.get_cmap('jet'))

def updatefig(i):
    im.set_array(data[i].reshape(y_axis, x_axis))
    return im,

ani = animation.FuncAnimation(fig, updatefig, frames=t_axis, interval=20, blit=True)
plt.show()
