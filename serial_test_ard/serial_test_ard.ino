int led = 13;
int data = 0;
int light = LOW;

void setup(){
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop(){
  data = Serial.read();
  if(data == 'H') {
    Serial.write(data);
  }
}
