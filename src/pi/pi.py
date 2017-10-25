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
	if 1000 <= value <= 1024:
		print "B1"
	elif 800 <= value <= 900:
		print "B2"
	elif 600 <= value <= 660:
		print "B3"
	elif 320 <= value <= 380:
		print "B4"
	elif 200 <= value <= 240:
		print "B5"
	elif 150 <= value <= 185:
		print "B6"
	elif 90 <= value <= 110:
		print "B7"
	elif value < 90:
		print "B0"
	else:
		print value

def doNewDayStuff():
	print selected_day
	setLed("6", "Y")

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


def doStuff(message, events):
	messageType = getMessageType(message)
	if messageType == "echo":
		print message[1:]
	elif messageType == "button":
		handleButton(message[1:])
	elif messageType == "rotary":
		handleRotary(message[1:])
	else:  # Something is wrong with the message, so we dump it.
		print "INVALID MESSAGE: " + message

def sendInitialMessages(events):
	#orderPrint(events[0])
	doNewDayStuff()

def orderPrint(event):
	message = "P"
	message += "Hi,\n"
	message += "this is a message\n"
	message += "BR\n"
	message += "Name Lastname\n"
	message += "\n\n\n\n\n\n"
	sendMessage(message)

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
	sendInitialMessages(events)
	while True:
		message = readMessage()
		if message is None:
			time.sleep(0.02)  # Wait 20ms before checking for messages again.
		else:
			doStuff(message, events)

def main():
	events = getEvents()
	loop(events)

main()