const int FAULTY_DATA = -1;
const int START_MESSAGE = 1;
const int END_MESSAGE = 2;
const int led0 = 13;
char data = 0;
char message[1000];
int messagePos = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led0, OUTPUT);
  digitalWrite(led0, LOW);
}

void readMessage() {
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
    readMessage();
    sendMessage();
  }
}
