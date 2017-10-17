const int FAULTY_DATA = -1;
const int START_MESSAGE = 1;
const int END_MESSAGE = 2;
const int ledx = 13;
const int led0 = 13;
const int led1 = 13;
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
    message[1] = (rotCounter + 0x30);
    messagePos = 2; //Move forward 2 steps since we just added a 2 char message.
  } 
  aLastState = aState;
}

void readSensors() {
  readRotary();
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
