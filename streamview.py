import numpy as np
import serial
from galry import *

ser = serial.Serial('/dev/ttyUSB0', 9600)
nsamples = 100
nplots = 1

t = np.tile(np.linspace(-1., 1., nsamples), (nplots, 1))
x = np.zeros([nplots, nsamples]) + np.linspace(-.75, .75, nplots)[:,np.newaxis]

def get_new_data():
    a= ser.readline().split()
    print a
    return np.array([[int(a[0])]])/500. + np.linspace(-.75, .75, nplots)[:,np.newaxis]

def get_pulse_data():
   a=ser.readline().split()
   print a
   """
   t0=0
   t=int(a[i])
   for i in len(a):
      x[t0:t]=i%2
      t0=t
      t+=int(a[i+1])
   """

def anim(fig, _):
   global x
   if(ser.isOpen()): 
      x = np.hstack((x[:,1:], get_new_data()))
   # create the new 1000*nplots*2 position array with x, y values at each row  
   position = np.vstack((t.flatten(), x.flatten())).T
   # update the position of the plot                                           
   fig.set_data(position=position)

plot(t, x)
animate(anim, dt=.001)
show()
