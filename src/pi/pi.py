import serial
import time
import requests

START_MESSAGE = '\x01'
END_MESSAGE = '\x02'
DAY_0 = '2017-11-01'
DAY_1 = '2017-11-02'
DAY_2 = '2017-11-03'
DAY_3 = '2017-11-04'
DAY_4 = '2017-11-05'

ser = serial.Serial("/dev/ttyACM0", 9600)
ser.baudrate = 9600


def sendMessage(message):
	ser.write(START_MESSAGE)
	ser.write(message)
	ser.write(END_MESSAGE)

def setLed(led, state):
	sendMessage("L" + str(led) + str(state))

def getEvents():
	req = requests.get('http://l:8082/events')
	return req.json()["events"]

def loop(events):
	while True:
		time.sleep(0.5)
		setLed(0,1)
		while ser.in_waiting > 0:
			resp = ser.read()
			print resp

def main():
	events = getEvents()
	loop(events)

main()