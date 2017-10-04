#define outputA 6
#define outputB 7
#define led1 8
#define led2 9
#define led3 10
#define led4 11
int counter = 0; 
int aState;
int aLastState;

void performLogics() {
  Serial.print("Position: ");
  Serial.println(counter);
  digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW); digitalWrite(led4, LOW);
  switch(counter) {
    case 0: digitalWrite(led1, HIGH); break;
    case 1: digitalWrite(led2, HIGH); break;
    case 2: digitalWrite(led3, HIGH); break;
    case 3: digitalWrite(led4, HIGH); break;
    default: digitalWrite(led1, HIGH); digitalWrite(led2, HIGH); digitalWrite(led3, HIGH); digitalWrite(led4, HIGH);
  }
}

void setup() { 
  pinMode (outputA,INPUT);
  pinMode (outputB,INPUT);
  pinMode (led1,OUTPUT);
  pinMode (led2,OUTPUT);
  pinMode (led3,OUTPUT);
  pinMode (led4,OUTPUT);
 
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
    performLogics();
  } 
  aLastState = aState;
}
