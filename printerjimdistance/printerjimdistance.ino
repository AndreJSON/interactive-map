#include <SoftwareSerial.h>
#define sensor 2 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)

SoftwareSerial Thermal(10, 9); // select the right pins
int heatTime = 80; // The printer can't start receiving data immediately upon power up it needs a moment to cold boot and initialize.
int heatInterval = 255;
char printDensity = 15;
char printBreakTime = 15;
int distanceCounter = 0;
void setup() {
  Serial.begin(9600); // open serial communication
  Thermal.begin(19200); // to write to the printer
  initPrinter();
}

void initPrinter() {
  //Modify the print speed and heat
  Thermal.write(27);
  Thermal.write(55);
  Thermal.write(7); //Default 64 dots = 8*('7'+1)
  Thermal.write(heatTime); //Default 80 or 800us
  Thermal.write(heatInterval); //Default 2 or 20us
  //Modify the print density and timeout
  Thermal.write(18);
  Thermal.write(35);
  int printSetting = (printDensity << 4) | printBreakTime;
  Thermal.write(printSetting); //Combination of printDensity and printBreakTime
  Serial.println();
  Serial.println("Printer ready");
  delay(1000);
}

void loop() {
  // 5v
  float volts = analogRead(sensor) * 0.0048828125; // value from sensor * (5/1024)
  int distance = 13 * pow(volts, -1); // worked out from datasheet graph
  delay(100); // slow down serial port
  Serial.println(distance);
  if (distance > 18 && distance < 300) {
    distanceCounter++;
  } else {
    distanceCounter = 0;
  }
  if (distanceCounter > 4) {
    Serial.println("WHOOOOOOAAAAAAAAAAAAAAA");
  }
  /*if (distance > 20 && distance < 60) {
    Thermal.println("Welcome to this EventMap of");
    Thermal.println("Stockholm! Press any building");
    Thermal.println("for event information. Feel");
    Thermal.println("free to take the print-out");
    Thermal.println("with you on-the-go");
    Thermal.write(10);
    Thermal.write(10);
    Thermal.write(10);
    delay(1000);
    while (1) {}
  }*/
}
