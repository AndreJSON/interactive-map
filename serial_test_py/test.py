import serial
import time

ser = serial.Serial("/dev/ttyACM0", 9600)
ser.baudrate = 9600

while True:
	time.sleep(0.1)
	ser.write("Hello\n")
	while ser.in_waiting > 0:
		resp = ser.read()
		if ord(resp) != 255:
			print resp