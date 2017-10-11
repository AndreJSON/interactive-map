const int FAULTY_DATA = -1;
const int START_MESSAGE = 1;
const int END_MESSAGE = 2;
const int ledx = 13;
const int led0 = 13;
const int led1 = 13;
char message[1000];
int messagePos = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledx, OUTPUT);
  pinMode(led0, OUTPUT);
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
  digitalWrite(led0, HIGH); 
  digitalWrite(led1, HIGH);
}

void processMessage() {
  if (messagePos > 0) {
    if (message[0] == 'L') {
      processLedMessage();
    }
  }
}

void readSensors() { 
}

void sendMessage() {
  Serial.write(START_MESSAGE);
  for (char i = 0; i < messagePos; i++) {
    Serial.write(message[i]);
  }
  messagePos = 0;
  Serial.write(END_MESSAGE);
}

void loop() {
  if(Serial.read() == START_MESSAGE) {
    digitalWrite(ledx, HIGH); //Light up built in LED to show that board has gotten at least one message.
    readMessage();
    processMessage();
    readSensors();
    sendMessage();
  }
}
