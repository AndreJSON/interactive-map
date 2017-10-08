int led = 13;
byte count = 0;

void setup(){
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop(){
  digitalWrite(led, HIGH);
  Serial.write(count);
  count = (count + 1) % 256;
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
}
