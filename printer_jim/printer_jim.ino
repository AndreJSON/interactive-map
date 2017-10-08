#include <SoftwareSerial.h>
SoftwareSerial Thermal(8, 9);
int heatTime = 80;
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
  Thermal.println("    /\\_____/\\");
  Thermal.println("   /  o   o  \\");
  Thermal.println("  ( ==  ^  == )");
  Thermal.println("   )         (");
  Thermal.println("  (           )");
  Thermal.println(" ( (  )   (  ) )");
  Thermal.println("(__(__)___(__)__)");
  Thermal.write(10); //Sends the LF to the printer, advances the paper
  Thermal.write(10);
  Thermal.write(10);
  Thermal.write(10);
  Thermal.write(10);
  Thermal.write(10);
  Thermal.write(10);
  Thermal.write(10);
  Thermal.write(10);
  Thermal.write(10);
  Thermal.write(10);
 do { } while (1>0);
}
