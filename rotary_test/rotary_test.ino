#define outputA 6
#define outputB 7
int counter = 0; 
int aState;
int aLastState;

void setup() { 
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
 
  Serial.begin (9600);
  aLastState = digitalRead(outputA);   
}

void loop() { 
  aState = digitalRead(outputA);
  if (aState != aLastState){
    if (digitalRead(outputB) == aState) { // if true, anti-clockwise rotation has ocurred.
      counter++;
    } else {
      counter--;
    }
    Serial.print("Position: ");
    Serial.println(counter);
  } 
  aLastState = aState;
}
