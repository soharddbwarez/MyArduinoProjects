/* 
 * en-defaultcolors.ino - Using LedRGB library
 * Showcases "defaultColor" function
 * 
 * By Alejandro Bertinelli
 * 
 * Created June 22, 2019
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
  // Using all available colors for "defaultColor"
  myLED.defaultColor("white");
  delay(300);
  myLED.defaultColor("red");
  delay(300);
  myLED.defaultColor("green");
  delay(300);
  myLED.defaultColor("blue");
  delay(300);
  myLED.defaultColor("yellow");
  delay(300);
  myLED.defaultColor("orange");
  delay(300);
  myLED.defaultColor("violet");
  delay(300);
  myLED.defaultColor("turquoise");
  delay(300);
  myLED.defaultColor("grey");
  delay(300);
  myLED.defaultColor("maroon");
  delay(300);
  myLED.defaultColor("fuchsia");
  delay(300);
}
