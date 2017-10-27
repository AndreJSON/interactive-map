//char ctrlKey = KEY_LEFT_GUI;
#include <SoftwareSerial.h>
int incomingByte = 0;   // for incoming serial data
SoftwareSerial Thermal(10, 9);
int heatTime = 80;
int heatInterval = 255;
char printDensity = 15; 
char printBreakTime = 15;
void setup() {
 Serial.begin(57600); // for debug info to serial monitor
 Thermal.begin(19200); // to write to our new printer
 initPrinter();
 loop();
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
 delay(1000);
}
void loop() {
          // send data only when you receive data:
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();

                // say what you got:
                Serial.print("I received: ");
                Serial.println(incomingByte, DEC);
                if(incomingByte > 100)
                {
                  Serial.println("hoger");
                  Thermal.print("hoger");
                  Thermal.write(10); //Sends the LF to the printer, advances the paper
                  delay(1000);
                }
                else
                {
                  Serial.println("lager");
                  Thermal.print("lager");  
                  Thermal.write(10); //Sends the LF to the printer, advances the paper
                  delay(1000);
                }
        }

}
