#include <SoftwareSerial.h>
const int FAULTY_DATA = -1;
const int START_MESSAGE = 1;
const int END_MESSAGE = 2;
const int buttonPin = A0;
const int led0 = 13; //Special built in led
const int led1 = 1;
const int led2 = 2;
const int led3 = 3;
const int led4 = 4;
const int led5 = 5;
const int led6 = 6;
const int led7 = 7;
const int printerTX = 9;
const int printerRX = 10;
SoftwareSerial Thermal(printerTX, printerRX);
const int outputA = 11; //CLK of rot
const int outputB = 12; //DT of rot
char rotCounter = 0; 
int aState;
int aLastState;
char message[1000];
int messagePos = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  aLastState = digitalRead(outputA);
  Thermal.begin(19200);
  Thermal.write(27);
  Thermal.write(55);
  Thermal.write(7);
  Thermal.write(80); //Heat time, 80 default.
  Thermal.write(255); //Heat interval, 255 default?
  Thermal.write(18);
  Thermal.write(35);
  int printSetting = (15<<4) | 15;
  Thermal.write(printSetting);
}

void readMessage() {
  char data;
  while(true) {
    data = Serial.read();
    if(data == END_MESSAGE) {
      break;
    }
    if(data != FAULTY_DATA) {
      message[messagePos] = data;
      messagePos++;
    }
  }
}

void processLedMessage() {
  switch(message[1]) {
    case '0': digitalWrite(led0, message[2] == '1'? HIGH:LOW);break;
    case '1': digitalWrite(led1, message[2] == '1'? HIGH:LOW);break;
    case '2': digitalWrite(led2, message[2] == '1'? HIGH:LOW);break;
    case '3': digitalWrite(led3, message[2] == '1'? HIGH:LOW);break;
    case '4': digitalWrite(led4, message[2] == '1'? HIGH:LOW);break;
    case '5': digitalWrite(led5, message[2] == '1'? HIGH:LOW);break;
    case '6': digitalWrite(led6, message[2] == '1'? HIGH:LOW);break;
    case '7': digitalWrite(led7, message[2] == '1'? HIGH:LOW);break;
  }
}

void processPrinterMessage() {
  for (int i = 1; i < messagePos; i++) {
    Thermal.write(message[i]);
  }
  Thermal.write(10);
  Thermal.write(10);
  Thermal.write(10);
  Thermal.write(10);
  Thermal.write(10);
  Thermal.write(10);
  Thermal.write(10);
}

void processMessage() {
  if (messagePos > 0) {
    if (message[0] == 'L' && messagePos == 3) { //Led message.
      processLedMessage();
      messagePos = 0; //Reset messagePos after processing.
    } else if (message[0] == 'P') { //Printer message
      processPrinterMessage();
      messagePos = 0; //Reset messagePos after processing.
    } else if (message[0] == 'E') { //Echo message.
      //Do nothing, message will be echoed back.
    } else { //Couldn't figure out what kind of message it was, so just reset.
      messagePos = 0;
    }
  }
}

void readRotary() {
  aState = digitalRead(outputA);
  if (aState != aLastState){
    if (digitalRead(outputB) == aState) { // if true, anti-clockwise rotation has ocurred.
      rotCounter++;
    } else {
      rotCounter--;
    }
    message[0] = 'R';
    message[1] = (rotCounter + 0x30); //add 0x30 to get correct utf-8 value for digit.
    messagePos = 2; //Move forward 2 steps since we just added a 2 char message.
  } 
  aLastState = aState;
}

void readButtons() {
  int buttonValue = analogRead(buttonPin);
  message[0] = 'B';
  message[1] = (buttonValue / 1000) + 0x30;
  message[2] = ((buttonValue % 1000) / 100) + 0x30;
  message[3] = ((buttonValue % 100) / 10) + 0x30;
  message[4] = (buttonValue % 10) + 0x30;
  messagePos = 5;
}

void readSensors() {
  if (messagePos == 0) { //Only deal with this now if we don't already have a message pending.
    readRotary();
  }
  if (messagePos == 0) { //Only deal with this now if we don't already have a message pending.
    readButtons();
  }
}

void sendMessage() {
  if (messagePos > 0) { //Only write start and end if there is a message to send.
    Serial.write(START_MESSAGE);
    for (char i = 0; i < messagePos; i++) {
      Serial.write(message[i]);
    }
    messagePos = 0;
    Serial.write(END_MESSAGE);
  }
}

void loop() {
  if(Serial.read() == START_MESSAGE) {
    readMessage();
  }
  processMessage();
  readSensors();
  sendMessage();
}
