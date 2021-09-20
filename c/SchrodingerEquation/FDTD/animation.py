"""
An animated image
"""
import pylab 
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

fig = plt.figure()
data=np.loadtxt("Schrodinger.txt")
#data=np.loadtxt("Potential.txt")

x_axis=600
y_axis=300
t_axis=101
#t_axis=50
data=data.reshape(t_axis, x_axis*y_axis)


im = plt.imshow(data[0].reshape(y_axis, x_axis),
                cmap=plt.get_cmap('jet'))

def updatefig(i):
    im.set_array(data[i].reshape(y_axis, x_axis))
    return im,

ani = animation.FuncAnimation(fig, updatefig, frames=t_axis, interval=15, blit=True)
ani.save('wave_propagation.mp4', fps=30, clear_temp=True)
plt.show()
