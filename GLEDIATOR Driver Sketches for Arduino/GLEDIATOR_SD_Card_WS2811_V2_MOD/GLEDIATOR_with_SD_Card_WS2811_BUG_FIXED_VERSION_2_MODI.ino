// Glediator Arduino UNO sketch by Jens Andrée
// Some changes and testing by Nils Gregersen, www.hamburgtech.de, info@hamburgtech.de
// sdcard bug fixed by Pascal also from Hamburg, nerdishbynatureHH@gmail.com


#include <FastLED.h>
#include <SPI.h>
#include <SD.h>

#define NUM_LEDS 121 // LED number
#define DATA_PIN 6    // your data arduino pin
#define CHIPSET WS2811  // your LED chip type
#define CMD_NEW_DATA 1
//#define BAUD_RATE 500000  //if using Glediator via serial
unsigned char x = 11; // matrix x size
unsigned char y = 11; // matrix y size

File fxdata;
CRGB leds[NUM_LEDS];

void setup()
{
  FastLED.addLeds<CHIPSET, DATA_PIN, GRB>(leds, NUM_LEDS); //see doc for different LED strips
//  Serial.begin(BAUD_RATE); // when using Glediator via usb
  Serial.begin(115200);

  for(int y = 0 ; y < NUM_LEDS ; y++)
  {
    leds[y] = CRGB::Black; // set all leds to black during setup
  }
  FastLED.show();

  pinMode(10, OUTPUT); // CS/SS pin as output for SD library to work.
  digitalWrite(10, HIGH); // workaround for sdcard failed error...

  if (!SD.begin(4))
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

// uncomment for Glediator
//while (fileGlediator () != CMD_NEW_DATA) {}
//Serial.readBytes((char*)leds, NUM_LEDS*3);

  fxdata = SD.open("myanim.dat");  // read only
  if (fxdata)
    {
      Serial.println("file open ok");
    }

  while (fxdata.available())
  {
    fxdata.readBytes((char*)leds, NUM_LEDS*3);
    ledSort(2);  //1-4 possible, set your first LED's position. Change the number: 1=TL(top left),2=TR(top right),3=BL(bottom left),4=BR(bottom right)
     FastLED.show();
    delay(50); // set the speed of the animation. 20 is appx ~ 500k bauds
  }

  // close the file in order to prevent hanging IO or similar throughout time
  fxdata.close();
}

int ledSort (int modus) { //1=TL,2=TR,3=BL,4=BR, this function will rearrange the led array ;-)
    CRGB tmp[x];
    if(modus == 3 || modus == 4) {

	for(int i = 0; i < (y / 2);i++) {
	    for(int j = 0; j < x;j++) {
		tmp[j] = leds[i * x + j];
		leds[i * x] = leds[(y - i - 1) * x];
		leds[(y - i - 1) * x] = tmp[j];
	     }
        }
     }

     if(modus == 1 || modus == 3) {
       for(int i = 1; i < y; i = i + 2) {
       for(int j = x; j > 0;j--) {
        tmp[(x - j)] = leds[i * x + j - 1];
          }
           for(int j = 0; j < x;j++) {
	    leds[i * x + j] = tmp[j];
	    }
       }

      }


     if(modus == 2 | modus == 4) {
      for(int i = 0; i < y; i = i + 2) {
       for(int j = x; j > 0;j--) {
        tmp[(x - j)] = leds[i * x + j - 1];
          }
           for(int j = 0; j < x;j++) {
	    leds[i * x + j] = tmp[j];
	    }
       }

      }
      return 1;
}
