int redPin = 11;
int greenPin = 10;
int bluePin = 9;
 
void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}
 
void loop() {
  setColor(0, 0, 255);
  delay(3000);
  setColor(0,0,0);
  delay(70);
  setColor(0,0,255);
  delay(70);
  setColor(0,0,0);
  delay(70);
}
 
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
