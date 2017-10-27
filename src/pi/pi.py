import serial
import time
import requests
import re

DOMAIN = 'l'
FAULTY_DATA = '\xFF'
START_MESSAGE = '\x01'
END_MESSAGE = '\x02'
DAY_1 = '2017-11-01'
DAY_2 = '2017-11-02'
DAY_3 = '2017-11-03'
DAY_4 = '2017-11-04'

lastPrintTime = time.clock()
buttonPresses = [0] * 10
selectedDay = DAY_1
selectedEvents = []
events = []

ser = serial.Serial("/dev/ttyACM0", 9600)


def handleButton(message):
	global buttonPresses
	buttonPresses.pop(0)  # Remove oldest element.
	value = int(message)
	if 1000 <= value <= 1024:
		buttonPresses.append(1)
	elif 800 <= value <= 900:
		buttonPresses.append(2)
	elif 600 <= value <= 660:
		buttonPresses.append(3)
	elif 320 <= value <= 380:
		buttonPresses.append(4)
	elif 200 <= value <= 240:
		buttonPresses.append(5)
	elif 130 <= value <= 185:
		buttonPresses.append(6)
	elif 90 <= value <= 110:
		buttonPresses.append(7)
	elif value < 90:
		buttonPresses.append(0)
	else:  # Some unexpected value, problem in circuit, noise or intervals need to be changed.
		print value
		buttonPresses.append(0)
	if buttonPresses[-1] != 0 and len([num for num in buttonPresses if num == buttonPresses[-1]]) > 4 and selectedEvents[buttonPresses[-1]-1] is not None:
		global lastPrintTime
		now = time.clock()
		if 100 * (now - lastPrintTime) > 7:  # at least this seconds have passed since last print.
			lastPrintTime = now
			print "yo"
			orderPrint(selectedEvents[buttonPresses[-1]-1])

def orderPrint(event):
	message = "P"
	message += event["title"] + "\n"
	message += "Date: " + event["date"] + "\n"
	message += "Time: " + event["start"] + " - " + event["end"] + "\n"
	message += "At: " + event["name"] + ", " + event["address"] + "\n\n"
	message += event["description"] + "\n"
	sendMessage(message.encode("utf-8"))

def findEvent(venueId):
	tmp = [event for event in events if (event["date"] == selectedDay and event["venueId"] == venueId)]
	return tmp[0] if len(tmp) == 1 else None

def doNewDayStuff():
	global selectedEvents
	selectedEvents = []
	print selectedDay
	for i in range(1,8):
		time.sleep(0.01)  # Needed because the arduino closes and opens serial port when told to light a led.
		event = findEvent(i)
		selectedEvents.append(event)
		if event is None:
			setLed(str(i), "N")
		elif event["category"] == "music":
			setLed(str(i), "R")
		elif event["category"] == "knowledge":
			setLed(str(i), "B")
		elif event["category"] == "sports":
			setLed(str(i), "G")
		elif event["category"] == "entertainment":
			setLed(str(i), "Y")
		else:
			setLed(str(i), "N")

def handleRotary(message):
	global selectedDay
	tmp = selectedDay
	if message[0] in '01':
		selectedDay = DAY_1
	elif message[0] in '23':
		selectedDay = DAY_2
	elif message[0] in '45':
		selectedDay = DAY_3
	elif message[0] in '678':
		selectedDay = DAY_4
	if tmp != selectedDay:  # Only if new day was selected.
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
	#orderPrint(events[0])
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
	global DOMAIN
	req = requests.get('http://' + DOMAIN + ':8082/events')
	return req.json()["events"]

def loop():
	sendInitialMessages()
	while True:
		message = readMessage()
		if message is None:
			time.sleep(0.02)  # Wait 20ms before checking for messages again.
		else:
			doStuff(message)

def main():
	global events
	events = getEvents()
	loop()

main()
