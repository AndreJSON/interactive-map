import serial
import time

ser = serial.Serial("/dev/ttyACM0", 9600)
ser.baudrate = 9600

while True:
	#line = ser.readline()
	line = ser.read()
	print(ord(line))
