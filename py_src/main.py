import serial
import sys
import time

ser = serial.Serial("/dev/ttyACM1", 9600)
ser.baudrate=9600

while True:
	ser.write('3')
	time.sleep(0.5)
	line = ser.readline()
	print '>' + line + '<'
	time.sleep(0.5)
