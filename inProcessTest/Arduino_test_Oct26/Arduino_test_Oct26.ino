#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

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
const int printerTX = 10;
const int printerRX = 9;
SoftwareSerial Thermal(printerTX, printerRX);
const int outputA = 11; //CLK of rot
const int outputB = 12; //DT of rot
char rotCounter = 0; 
int aState;
int aLastState;
char message[1000];
int messagePos = 0;
int intFlag = 0;
Adafruit_NeoPixel pixel1 = Adafruit_NeoPixel(1, led1, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel2 = Adafruit_NeoPixel(1, led2, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel3 = Adafruit_NeoPixel(1, led3, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel4 = Adafruit_NeoPixel(1, led4, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel5 = Adafruit_NeoPixel(1, led5, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel6 = Adafruit_NeoPixel(1, led6, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel7 = Adafruit_NeoPixel(1, led7, NEO_RGB + NEO_KHZ400);

void setup() {
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  Serial.begin(9600);
  pinMode(led0, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  aLastState = digitalRead(outputA);
  pixel1.begin(); // This initializes the NeoPixel library.
  pixel2.begin();
  pixel3.begin();
  pixel4.begin();
  pixel5.begin();
  pixel6.begin();
  pixel7.begin();
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
    case '1': pixel1.setPixelColor(0,pixel1.Color(20,0,0));pixel1.show();break;
    case '2': pixel2.setPixelColor(0,pixel2.Color(0,20,0));pixel2.show();break;
    case '3': pixel3.setPixelColor(0,pixel3.Color(0,0,20));pixel3.show();break;
    case '4': pixel4.setPixelColor(0,pixel4.Color(0,20,20));pixel4.show();break;
    case '5': pixel5.setPixelColor(0,pixel5.Color(20,0,20));pixel5.show();break;
    case '6': pixel6.setPixelColor(0,pixel6.Color(20,20,0));pixel6.show();break;
    case '7': pixel7.setPixelColor(0,pixel7.Color(20,20,20));pixel7.show();break;
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

void ledTest(int flag) {
  if (flag == 0){
    pixel1.setPixelColor(0,pixel1.Color(20,0,0)); // red
    pixel1.show();
    pixel2.setPixelColor(0,pixel2.Color(0,20,0)); // red
    pixel2.show();
    pixel3.setPixelColor(0,pixel3.Color(0,0,20)); // red
    pixel3.show();
    pixel4.setPixelColor(0,pixel4.Color(0,20,20)); // red
    pixel4.show();
    pixel5.setPixelColor(0,pixel5.Color(20,0,20)); // red
    pixel5.show();
    pixel6.setPixelColor(0,pixel6.Color(20,20,0)); // red
    pixel6.show();
    pixel7.setPixelColor(0,pixel7.Color(20,20,20)); // red
    pixel7.show();
  }
  else{
    pixel1.setPixelColor(0,pixel1.Color(0,0,0)); // red
    pixel1.show();
    pixel2.setPixelColor(0,pixel2.Color(0,0,0)); // red
    pixel2.show();
    pixel3.setPixelColor(0,pixel3.Color(0,0,0)); // red
    pixel3.show();
    pixel4.setPixelColor(0,pixel4.Color(0,0,0)); // red
    pixel4.show();
    pixel5.setPixelColor(0,pixel5.Color(0,0,0)); // red
    pixel5.show();
    pixel6.setPixelColor(0,pixel6.Color(0,0,0)); // red
    pixel6.show();
    pixel7.setPixelColor(0,pixel7.Color(0,0,0)); // red
    pixel7.show();
  }
}

// void loop() {
//   if(Serial.read() == START_MESSAGE) {
//     readMessage();
//   }
//   processMessage();
//   readSensors();
//   sendMessage();
// }

void loop() {
  ledTest(intFlag);
  delay(2000);
  intFlag = 1 - intFlag;
}