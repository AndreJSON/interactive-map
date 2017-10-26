void setup() {
}

void loop() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Hello");
  delay(3000);
  Serial.end();
  delay(3000);
}
