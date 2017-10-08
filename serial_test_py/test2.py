import serial
import time
 
ser=serial.Serial("/dev/ttyACM1",9600)  #change ACM number as found from ls /dev/tty/ACM*
ser.baudrate=9600

while True:
    #line = ser.readline()
    line = ser.read()
    print ord(line)
