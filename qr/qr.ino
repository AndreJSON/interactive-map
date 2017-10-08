#include "qrprint.h"
#include <SoftwareSerial.h>

int pin = 9;                      // the pin that will be sending signals to the thermalPrinterPrinter printer (connected to printer's rx)
int printHeat = 7;                // 7 is the printer default. Controls number of heating dots, higher = hotter, darker, and more current draw
int printSpeed = 255;             // 80 is the printer default. Controls speed of printing (and darkness) higher = slower
int heatInterval = 255;
char printDensity = 15; 
char printBreakTime = 15;
SoftwareSerial thermalPrinter(99, pin);  // set rx to a non-existant pin, because we don't need rx just tx

void setup() {
  
  thermalPrinter.begin(19200);
  
  //Modify the print speed and heat
  thermalPrinter.write(27);
  thermalPrinter.write(55);
  thermalPrinter.write(printHeat);
  thermalPrinter.write(printSpeed);
  thermalPrinter.write(heatInterval);
  thermalPrinter.write(18);
  thermalPrinter.write(35);
  int printSetting = (printDensity<<4) | printBreakTime;
  thermalPrinter.write(printSetting); //Combination of printDensity and printBreakTime
  
  thermalPrinter.println("    /\\_____/\\");
  thermalPrinter.println("   /  o   o  \\");
  thermalPrinter.println("  ( ==  ^  == )");
  thermalPrinter.println("   )         (");
  thermalPrinter.println("  (           )");
  thermalPrinter.println(" ( (  )   (  ) )");
  thermalPrinter.println("(__(__)___(__)__)");
  thermalPrinter.write(10); //Sends the LF to the printer, advances the paper
  thermalPrinter.write(10);
  thermalPrinter.write(10);
  thermalPrinter.write(10);
  thermalPrinter.write(10);
  thermalPrinter.write(10);
  thermalPrinter.write(10);
  thermalPrinter.write(10);
  thermalPrinter.write(10);
  thermalPrinter.write(10);
  thermalPrinter.write(10);
  
  
  thermalPrinter.begin(19200);
  
  //Modify the print speed and heat
  thermalPrinter.write(27);
  thermalPrinter.write(55);
  thermalPrinter.write(printHeat);
  thermalPrinter.write(printSpeed);
  thermalPrinter.write(heatInterval);
  thermalPrinter.write(18);
  thermalPrinter.write(35);
  printSetting = (printDensity<<4) | printBreakTime;
  thermalPrinter.write(printSetting); //Combination of printDensity and printBreakTime
  
  // print "Hello, world!" as text and then a QR code.
  thermalPrinter.print("Hello, world!");
  printQR("Hello, world!");
  
}

void loop() { }


