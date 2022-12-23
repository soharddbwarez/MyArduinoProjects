#include "FastLED.h"
#define NUM_LEDS 150
const int dataline = 4;
CRGB leds[NUM_LEDS];

void setup() {
Serial.begin(500000); 
LEDS.addLeds<WS2812B, dataline>(leds, NUM_LEDS);
   		
for (int p=0;p< NUM_LEDS;p++){
leds[p] = CRGB::Black;  
FastLED.show();}
}

int serialReadBlocking() {
  while (!Serial.available()) {}
  return Serial.read(); 
}

void loop() {
while (serialReadBlocking() != 1) {} 
   
   for (long i=0; i < NUM_LEDS; i++) {
     leds[i].r = serialReadBlocking();
     leds[i].g = serialReadBlocking();
     leds[i].b = serialReadBlocking();
   }
FastLED.show();
}


