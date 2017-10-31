#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
const int FAULTY_DATA = -1;
const int START_MESSAGE = 1;
const int END_MESSAGE = 2;
const int buttonPin = A0;
const int led0 = 13; //Special built in led
const int led1 = 0;
const int led2 = 1;
const int led3 = 2;
const int led4 = 3;
const int led5 = 4;
const int led6 = 5;
const int led7 = 6;
const int themeleds = 11;
Adafruit_NeoPixel pixel1 = Adafruit_NeoPixel(1, led1, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel2 = Adafruit_NeoPixel(1, led2, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel3 = Adafruit_NeoPixel(1, led3, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel4 = Adafruit_NeoPixel(1, led4, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel5 = Adafruit_NeoPixel(1, led5, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel6 = Adafruit_NeoPixel(1, led6, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel7 = Adafruit_NeoPixel(1, led7, NEO_RGB + NEO_KHZ400);
const int distanceSensor = 2;
const int printerTX = 10;
const int printerRX = 9;
SoftwareSerial Thermal(printerTX, printerRX);
const int outputA = 7; //CLK of rot
const int outputB = 8; //DT of rot
char rotCounter = 0; 
int aState;
int aLastState;
char message[1000];
int messagePos = 0;
int loopCounter = 0;
int distanceCounter = 0;
int distanceLoopCounter = 0;
long loopsSinceLastPrint = 496000;

void setup() {
  pinMode(led0, OUTPUT);
  pinMode(themeleds, OUTPUT);
  analogWrite(themeleds, 40);
  pixel1.begin();
  pixel2.begin();
  pixel3.begin();
  pixel4.begin();
  pixel5.begin();
  pixel6.begin();
  pixel7.begin();
  pixel1.setPixelColor(0,pixel1.Color(0,0,0));
  pixel1.show();
  pixel2.setPixelColor(0,pixel2.Color(0,0,0));
  pixel2.show();
  pixel3.setPixelColor(0,pixel3.Color(0,0,0));
  pixel3.show();
  pixel4.setPixelColor(0,pixel4.Color(0,0,0));
  pixel4.show();
  pixel5.setPixelColor(0,pixel5.Color(0,0,0));
  pixel5.show();
  pixel6.setPixelColor(0,pixel6.Color(0,0,0));
  pixel6.show();
  pixel7.setPixelColor(0,pixel7.Color(0,0,0));
  pixel7.show();
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
  Serial.begin(9600);
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

void setLedColor(Adafruit_NeoPixel *p, char c) {
  if (c == 'R') {
    (*p).setPixelColor(0,(*p).Color(50,0,0));
  } else if (c == 'G') {
    (*p).setPixelColor(0,(*p).Color(0,50,0));
  } else if (c == 'B') {
    (*p).setPixelColor(0,(*p).Color(0,0,50));
  } else if (c == 'Y') {
    (*p).setPixelColor(0,(*p).Color(50,50,0));
  } else {
    (*p).setPixelColor(0,(*p).Color(0,0,0));
  }
  (*p).show();
}

void processLedMessage() {
  Serial.end();
  switch(message[1]) {
    case '0': digitalWrite(led0, message[2] == '1'? HIGH:LOW);break;
    case '1': setLedColor(&pixel1, message[2]); break;
    case '2': setLedColor(&pixel2, message[2]); break;
    case '3': setLedColor(&pixel3, message[2]); break;
    case '4': setLedColor(&pixel4, message[2]); break;
    case '5': setLedColor(&pixel5, message[2]); break;
    case '6': setLedColor(&pixel6, message[2]); break;
    case '7': setLedColor(&pixel7, message[2]); break;
  }
  Serial.begin(9600);
}

void processPrinterMessage() {
  for (int i = 1; i < messagePos; i++) {
    Thermal.write(message[i]);
  }
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
      rotCounter--;
    } else {
      rotCounter++;
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

void readDistanceSensor() {
  float volts = analogRead(distanceSensor) * 0.0048828125; // value from sensor * (5/1024)
  int distance = 13 * pow(volts, -1); // worked out from datasheet graph
  if (distance > 18 && distance < 300) {
    distanceCounter++;
  } else {
    distanceCounter = 0;
  }
  if (distanceCounter > 4) {
    loopsSinceLastPrint = 0;
    Thermal.println("Welcome to this EventMap of");
    Thermal.println("Stockholm! Press any building");
    Thermal.println("for event information. Feel");
    Thermal.println("free to take the print-out");
    Thermal.println("with you on-the-go");
    Thermal.write(10);
    Thermal.write(10);
    Thermal.write(10);
    Thermal.write(10);
  }
}

void readSensors() {
  if (messagePos == 0) { //Only deal with this now if we don't already have a message pending.
    readRotary();
  }
  if (messagePos == 0 && loopCounter > 20) { //Only deal with this now if we don't already have a message pending.
    loopCounter = 0;
    readButtons();
  }
  if(distanceLoopCounter > 50 && loopsSinceLastPrint > 500000) {
    distanceLoopCounter = 0;
    readDistanceSensor();
  } else {
    loopsSinceLastPrint++;
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
  loopCounter++;
  distanceLoopCounter++;
  delay(1);
}
