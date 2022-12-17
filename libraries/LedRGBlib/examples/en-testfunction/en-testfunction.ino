/* 
 * en-testfunction.ino - Using LedRGB library
 * Tests all the R, G, B diodes of the LED:
 * -300ms RED
 * -300ms GREEN
 * -300ms BLUE
 * -500ms WHITE (all RGB diodes)
 * 
 * By Alejandro Bertinelli
 * 
 * Created June 29, 2017
 * Last updated June 22, 2019
 */

/*
 * MIT LICENSE 3.0
 */


#include <LedRGB.h>
LedRGB myLED(9, 10, 11, CC);

void setup() {

}

void loop() {
  // Usamos la función "test", which tests whether red, green and blue diodes are working fine.
  myLED.test();
}
