import numpy as np
import serial
from galry import *

ser = serial.Serial('/dev/ttyUSB0', 9600)
nsamples = 100
nplots = 3

t = np.tile(np.linspace(-1., 1., nsamples), (nplots, 1))
x = np.zeros([nplots, nsamples]) + np.linspace(-.75, .75, nplots)[:,np.newaxis]

def get_new_data():
   a=ser.readline().split()
   #a0=int(a[0])
   #a1=int(a[1])
   #print a0, " ", a1
   return np.array([[int(a[0])], [int(a[1])], [int(a[2])]])/10000. + np.linspace(-.75, .75, nplots)[:,np.newaxis]

def anim(fig, _):
   global x
   if(ser.isOpen()): x = np.hstack((x[:,1:], get_new_data()))
   # create the new 1000*nplots*2 position array with x, y values at each row  
   position = np.vstack((t.flatten(), x.flatten())).T
   # update the position of the plot                                           
   fig.set_data(position=position)

plot(t, x)
animate(anim, dt=.001)
show()
