/* 
 * en-addsubstract.ino - Using LedRGB library
 * Uses "setColor", "add" and "substract" functions from the library:
 * - 200ms WHITE (ONLY AT THE BEGINNING)
 * 
 * - 700ms YELLOW
 * - 700ms GREEN
 * - 300ms OFF
 * - 700ms RED
 * - 700ms VIOLET
 * - 700ms WHITE
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
  myLED.setColor(255, 255, 255);    //  Set color to white
  delay(200);                       //  Wait 200ms before starting loop
}

void loop() {
  // Using functions "substract" and "add"
  myLED.substract(0, 0, 255); //  Substract 0 from R, 0 from G, and 255 from B
  delay(700);                 //  Wait 700ms 
  myLED.substract(255, 0, 0); //  Substract 255 from R, 0 from G, and 0 from B
  delay(700);                 //  Wait 700ms 
  myLED.substract(0, 255, 0); //  Substract 0 from R, 255 from G, and 0 from B
  delay(300);                 //  Wait 300ms 
  myLED.add(255, 0, 0);       //  Add 255 to R, 0 to G, and 0 to B
  delay(700);                 //  Wait 700ms 
  myLED.add(0, 100, 255);     //  Add 0 to R, 100 to G, and 255 to B
  delay(700);                 //  Wait 700ms 
  myLED.add(0, 155, 0);       //  Add 0 to R, 155 to G, and 0 to B
  delay(700);                 //  Wait 700ms 
}
