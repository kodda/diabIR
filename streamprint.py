import numpy as np
from time import sleep
import serial

ser = serial.Serial('/dev/ttyUSB5', 9600)
while True:
   print ser.readline()
   sleep(0.1)
