import serial
import time
import requests

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
	print message

def doNewDayStuff():
	print selected_day

def handleRotary(message):
	if message[0] in '012':
		selected_day = DAY_1
	elif message[0] in '34':
		selected_day = DAY_2
	elif message[0] in '56':
		selected_day = DAY_3
	elif message[0] in '78':
		selected_day = DAY_4
	doNewDayStuff()

def doStuff(message):
	if message[0] == "E":  # Echo
		print message[1:]
	elif message[0] == "B":  # Button
		handleButton(message[1:])
	elif message[0] == "R":  # Rotary
		handleRotary(message[1:])
	else:  # Something faulty probably, just print it.
		print message

def sendInitialMessages():
	doNewDayStuff()
	sendMessage("E" + "Yooo!")

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