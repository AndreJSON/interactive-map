import serial
import time
import requests

FAULTY_DATA = '\xFF'
START_MESSAGE = '\x01'
END_MESSAGE = '\x02'
DAY_0 = '2017-11-01'
DAY_1 = '2017-11-02'
DAY_2 = '2017-11-03'
DAY_3 = '2017-11-04'
DAY_4 = '2017-11-05'

ser = serial.Serial("/dev/ttyACM0", 9600)
ser.baudrate = 9600


def doStuff(message):
	print message

def sendInitialMessages():
	setLed(0, 1)

def setLed(led, state):
	sendMessage("L" + str(led) + str(state))

def readMessage():
	if ser.in_waiting > 0:
		message = ""
		while ser.read() != START_MESSAGE:
			pass
		while True:
			data = ser.read()
			if data != END_MESSAGE:
				message += data
			else:
				return message
	else:
		return None

def sendMessage(message):
	ser.write(START_MESSAGE)
	ser.write(message)
	ser.write(END_MESSAGE)

def getEvents():
	req = requests.get('http://l:8082/events')
	return req.json()["events"]

def loop(events):
	sendInitialMessages()
	while True:
		message = readMessage()
		if message is None:
			time.sleep(0.03)  # Wait 30ms before checking for messages again.
		else:
			doStuff(message)

def main():
	events = getEvents()
	loop(events)

main()