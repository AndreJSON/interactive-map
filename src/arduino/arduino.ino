const int FAULTY_DATA = -1;
const int START_MESSAGE = 1;
const int END_MESSAGE = 2;
const int BUTTON_TOLERANCE = 7;
const int BUTTON1 = 91;
const int BUTTON2 = 144;
const int BUTTON3 = 203;
const int BUTTON4 = 340;
const int BUTTON5 = 639;
const int BUTTON6 = 852;
const int ledx = 13;
const int led0 = 13;
const int led1 = 13;
const int buttonPin = A1;
const int outputA = 11; //CLK of rot
const int outputB = 12; //DT of rot
char rotCounter = 0; 
int aState;
int aLastState;
char message[1000];
int messagePos = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledx, OUTPUT);
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  aLastState = digitalRead(outputA);
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
}

void processMessage() {
  if (messagePos > 0) {
    if (message[0] == 'L') {
      processLedMessage();
    }
  }
  messagePos = 0; //Reset messagePos after processing.
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
  if (buttonValue == 0) {
    message[1] = '0';
  } else if (buttonValue > BUTTON1 - BUTTON_TOLERANCE && buttonValue < BUTTON1 + BUTTON_TOLERANCE) {
    message[1] = '1';
  } else if (buttonValue > BUTTON2 - BUTTON_TOLERANCE && buttonValue < BUTTON2 + BUTTON_TOLERANCE) {
    message[1] = '2';
  } else if (buttonValue > BUTTON3 - BUTTON_TOLERANCE && buttonValue < BUTTON3 + BUTTON_TOLERANCE) {
    message[1] = '3';
  } else if (buttonValue > BUTTON4 - BUTTON_TOLERANCE && buttonValue < BUTTON4 + BUTTON_TOLERANCE) {
    message[1] = '4';
  } else if (buttonValue > BUTTON5 - BUTTON_TOLERANCE && buttonValue < BUTTON5 + BUTTON_TOLERANCE) {
    message[1] = '5';
  } else if (buttonValue > BUTTON6 - BUTTON_TOLERANCE && buttonValue < BUTTON6 + BUTTON_TOLERANCE) {
    message[1] = '6';
  } else { //If none of them match, we just pretend that no button was pressed.
    message[1] = '0';
  }
  messagePos = 2;
}

void readSensors() {
  //readRotary();
  readButtons();
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
