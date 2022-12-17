//##############################################################################
//##############################################################################
//                                                                             #
// Glediator to WS2812B pixel converter for the Arduino NANO                   #
//                                                                             #
//                                                                             #            
//                                                                             #
//                                                                             #
// Receives serial data in Glediator protocol format @ 1 MBit/s                #
// and distributes it to a connectect chain of WS2812B pixels                  #
//                                                                             #
// Adjust the correct DATA PIN and the correct NUMBER OF PIXELS you are using  # 
// in the definitions section below before uploading this sketch to your       #
// Arduino device.                                                             #
//                                                                             #
// Maxiumim number of supported pixeles is 512 !!!                             #
//                                                                             #
// In the Glediator software set output mode to "Glediator_Protocol",          #
// color order to "GRB" and baud rate to "1000000"                             #
//                                                                             #
//##############################################################################
//##############################################################################

#include "FastLED.h"
#define NUM_LEDS 128
const int dataline = 6;

CRGB leds[NUM_LEDS];
void setup() {
  Serial.begin(1000000);
  LEDS.addLeds<WS2812B, dataline>(leds, NUM_LEDS);
}
int serialGlediator() {
  while (!Serial.available()) {}
  return Serial.read();
}
void loop() {
   while (serialGlediator() != 1) {}

   for (int i=0; i < NUM_LEDS; i++) {
     leds[i].r = serialGlediator();
     leds[i].g = serialGlediator();
     leds[i].b = serialGlediator();
   }
     FastSPI_LED.show();
}
