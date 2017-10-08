#include <SoftwareSerial.h>
SoftwareSerial Thermal(2, 3);
int heatTime = 140;
int heatInterval = 255;
char printDensity = 15; 
char printBreakTime = 15;
void setup() {
 Serial.begin(57600); // for debug info to serial monitor
 Thermal.begin(19200); // to write to our new printer
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
 int printSetting = (printDensity<<4) | printBreakTime;
 Thermal.write(printSetting); //Combination of printDensity and printBreakTime
 Serial.println();
 Serial.println("Printer ready"); 
}
void loop() {
Thermal.write("STORIES AND SONGS\n");
Thermal.write("Stories (Ramsor) and songs for\n");
Thermal.write("the youngest children 0-7 months\n");
Thermal.write("and their adults.\n");
Thermal.write(10);
Thermal.write("Date: 2017-10-11\n");
Thermal.write("Time: 11:00 - 11:30\n");
Thermal.write(10);
Thermal.write("The event is held at \n");
Thermal.write("Stadsbiblioteket\n");
Thermal.write("in the neighbourhood \n");
Thermal.write("VASASTADEN \n");
Thermal.write("on SVEAVÄGEN 73\n");
Thermal.write(10);
Thermal.write("A ticket is needed, get it here:\n");
Thermal.write("http://bit.ly/2yYpVPf \n");
Thermal.write(10);
Thermal.write(10);
Thermal.write("       -___________-      \n");
Thermal.write("       -           -      \n");
Thermal.write("       -           -      \n");
Thermal.write("       -           -      \n");
Thermal.write("   ____-___________-____ \n");
Thermal.write(" -_______________________-\n");
Thermal.write(" -                       -\n");
Thermal.write(" -          ___          -\n");
Thermal.write(" -         -   -         -\n");
Thermal.write(" -         -   -         -\n");
Thermal.write(" ._________-___-_________.\n");
Thermal.write(10);
Thermal.write(10);
Thermal.write("Stadsbiblioteket is a nice place\n");
Thermal.write("to visit and has a restaurant\n");
Thermal.write("inside. The opening hours are:\n");
Thermal.write(10);
Thermal.write("Monday: 10:00-21:00,\n");
Thermal.write("Tuesday: 10:00-21:00,\n");
Thermal.write("Wednesday: 10:00-21:00,\n");
Thermal.write("Thursday: 10:00-21:00,\n");
Thermal.write("Friday: 10:00-19:00,\n");
Thermal.write("Saturday: 11:00-17:00\n");
Thermal.write("Sunday: 11:00-17:00,\n");
Thermal.write(10);
Thermal.write("Have a nice day\n");
Thermal.write(10);
Thermal.write(10);
Thermal.write(10);
Thermal.write(10);
Thermal.write(10);
Thermal.write(10);
 do { } while (1>0);
}
