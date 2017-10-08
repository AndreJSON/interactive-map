int redPin = 11;
int greenPin = 10;
int bluePin = 9;
int buttonPin = 7;
int d = 50;
int buttonPressed = 0;
 
void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin (9600);
}
 
void loop() {
  buttonPressed = digitalRead(buttonPin);
  Serial.println(buttonPressed);
  if(buttonPressed == HIGH) {
    setColor(150, 0, 150);
  } else {
    setColor(0, 0, 0);
  }
  delay(d);
}
 
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
