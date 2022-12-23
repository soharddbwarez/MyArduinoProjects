// Glediator Arduino UNO sketch by Jens Andrée / https://politisktinkorrektpappa.wordpress.com/
// 500k bauds with 80 pixels no problem
// sdcard stream for stand-alone operation.
// changed to WS2811 chip and a few small changes by Nils Gregersen / http://hamburgtech.de/


#include <FastLED.h>
#include <SPI.h>
#include <SD.h>

#define NUM_LEDS 16
#define DATA_PIN 6
#define CHIPSET WS2811
#define CMD_NEW_DATA 1
#define BAUD_RATE 115200

File fxdata;
CRGB leds[NUM_LEDS];

void setup()
{
  FastLED.addLeds<CHIPSET, DATA_PIN>(leds, NUM_LEDS); //see doc for different LED strips
  Serial.begin(BAUD_RATE); 

  
  for(int y = 0 ; y < NUM_LEDS ; y++) 
  {
    leds[y] = CRGB::Black; // set all leds to black during setup
  }
  FastLED.show();

  pinMode(10, OUTPUT); // CS/SS pin as output for SD library to work.
  digitalWrite(10, HIGH); // workaround for sdcard failed error...

  if (!SD.begin(10))
  {
    Serial.println("sdcard initialization failed!");
    return;
  }
  Serial.println("sdcard initialization done.");
  
  // test file open
  fxdata = SD.open("myanim.dat");  // read only
  if (fxdata)
  {
    Serial.println("file open ok");      
    fxdata.close();
  }
}

int serialGlediator ()
{
  while (!Serial.available()) {}
  return Serial.read();
}

void loop()
{

  fxdata = SD.open("myanim.dat");  // read only
  if (fxdata)
    {
      Serial.println("file open ok");      
    }

  while (fxdata.available()) 
  {
    fxdata.readBytes((char*)leds, NUM_LEDS*3);
    FastLED.show();
    delay(50); // set the speed of the animation. 20 is appx ~ 500k bauds
  }
  
  // close the file in order to prevent hanging IO or similar throughout time
  fxdata.close();
}
