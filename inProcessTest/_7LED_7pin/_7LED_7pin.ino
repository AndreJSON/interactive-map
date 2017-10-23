// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define START_PIN      0

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
// auto pixels = calloc(7, sizeof(Adafruit_NeoPixel));
Adafruit_NeoPixel pixel1 = Adafruit_NeoPixel(NUMPIXELS, START_PIN, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel2 = Adafruit_NeoPixel(NUMPIXELS, START_PIN+1, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel3 = Adafruit_NeoPixel(NUMPIXELS, START_PIN+2, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel4 = Adafruit_NeoPixel(NUMPIXELS, START_PIN+3, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel5 = Adafruit_NeoPixel(NUMPIXELS, START_PIN+4, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel6 = Adafruit_NeoPixel(NUMPIXELS, START_PIN+5, NEO_RGB + NEO_KHZ400);
Adafruit_NeoPixel pixel7 = Adafruit_NeoPixel(NUMPIXELS, START_PIN+6, NEO_RGB + NEO_KHZ400);


int delayval = 2000; // delay for half a second

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixel1.begin(); // This initializes the NeoPixel library.
  pixel2.begin();
  pixel3.begin();
  pixel4.begin();
  pixel5.begin();
  pixel6.begin();
  pixel7.begin();
}

void loop() {
  pixel1.setPixelColor(0,pixel1.Color(0,0,0)); // red
  pixel1.show();
  pixel2.setPixelColor(0,pixel2.Color(0,0,0)); // red
  pixel2.show();
  pixel3.setPixelColor(0,pixel3.Color(0,0,0)); // red
  pixel3.show();
  pixel4.setPixelColor(0,pixel4.Color(0,0,0)); // red
  pixel4.show();
  pixel5.setPixelColor(0,pixel5.Color(150,0,150)); // red
  pixel5.show();
  pixel6.setPixelColor(0,pixel6.Color(150,150,0)); // red
  pixel6.show();
  pixel7.setPixelColor(0,pixel7.Color(150,150,150)); // red
  pixel7.show();
}
