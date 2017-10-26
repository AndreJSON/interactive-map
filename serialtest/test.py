import serial
import time

ser = serial.Serial("/dev/ttyACM0", 9600)

while(True):
	print ser.readline()
	time.sleep(0.1)
