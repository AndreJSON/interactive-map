import serial
import time
import requests
import re

FAULTY_DATA = '\xFF'
START_MESSAGE = '\x01'
END_MESSAGE = '\x02'
DAY_1 = '2017-11-01'
DAY_2 = '2017-11-02'
DAY_3 = '2017-11-03'
DAY_4 = '2017-11-04'

selected_day = DAY_1

ser = serial.Serial("/dev/ttyACM0", 9600)
ser.baudrate = 9600


def handleButton(message):
	value = int(message)
	if 1024 <= value <= 1000:
		print "B1"
	elif 800 <= value <= 900:
		print "B2"
	else:
		print value

def doNewDayStuff():
	print selected_day

def handleRotary(message):
	global selected_day
	tmp = selected_day
	if message[0] in '01':
		selected_day = DAY_1
	elif message[0] in '23':
		selected_day = DAY_2
	elif message[0] in '45':
		selected_day = DAY_3
	elif message[0] in '678':
		selected_day = DAY_4
	if tmp != selected_day:  # Only if new day was selected.
		doNewDayStuff()

def getMessageType(message):
	if len(message) == 0:
		return "invalid"
	if message[0] == "E":
		return "echo"
	elif message[0] == "B" and len(message) == 5 and re.match("[0-9]{4}",message[1:]):
		return "button"
	elif message[0] == "R" and len(message) == 2 and message[1] in "012345678":
		return "rotary"
	else:
		return "invalid"


def doStuff(message):
	messageType = getMessageType(message)
	if messageType == "echo":
		print message[1:]
	elif messageType == "button":
		handleButton(message[1:])
	elif messageType == "rotary":
		handleRotary(message[1:])
	else:  # Something is wrong with the message, so we dump it.
		print "INVALID MESSAGE: " + message

def sendInitialMessages():
	doNewDayStuff()

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
			time.sleep(0.02)  # Wait 20ms before checking for messages again.
		else:
			doStuff(message)

def main():
	events = getEvents()
	loop(events)

main()