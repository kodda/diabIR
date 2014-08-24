import serial
import numpy as np

serRCV=serial.Serial('/dev/ttyUSB0', 9600)
serSND=serial.Serial('/dev/ttyUSB1', 9600)

#send a single command and choose protocol 
def keystrokes():
   prot=["NEC", "SONY"]
   while 1:
      i = raw_input("--> ")
      serSND.write("%s,a8bca,20"%int(i))
      r=serRCV.readline() 
      print "intended", prot[int(i)] , "a8bca and got", r

#send a temporal signal s, default protocol is SONY (2)
def sin_sig(s, prot=2):
   r=np.zeros(len(s))
   for i in range(len(s)):
      serSND.write("%s,%s,20"%(prot,s[i]))
      r[i]=serRCV.readline()
      print i
   return r

N=2**(10)
s=np.round((np.sin(np.linspace(0,2*np.pi,30))+1)*N/2).astype(np.int)
r=sin_sig(s, prot=2)
