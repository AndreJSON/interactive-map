import serial
import time
import requests

START_MESSAGE = '\x01'
END_MESSAGE = '\x02'

#req = requests.get('http://localhost:8082/events')
#events = req.json()["events"]

ser = serial.Serial("/dev/ttyACM0", 9600)
ser.baudrate = 9600

while True:
	time.sleep(0.5)
	ser.write(START_MESSAGE)
	ser.write("L23")
	ser.write(END_MESSAGE)
	while ser.in_waiting > 0:
		resp = ser.read()
		print resp