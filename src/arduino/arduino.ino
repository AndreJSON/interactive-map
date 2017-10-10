const int FAULTY_DATA = -1;
const int START_MESSAGE = 1;
const int END_MESSAGE = 2;
const int ledx = 13; //The built in led.
int data = 0;

void setup(){
  Serial.begin(9600);
  pinMode(ledx, OUTPUT);
}

void readMessage() {
  while(true) {
    data = Serial.read();
    if(data == END_MESSAGE) {
      break;
    }
    if(data != FAULTY_DATA) {
      Serial.write(data);
    }
  }
}

void loop(){
  if(Serial.read() == START_MESSAGE) {
    readMessage();
  }
}
