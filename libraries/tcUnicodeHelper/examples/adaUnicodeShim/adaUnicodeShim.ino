#include <Adafruit_GFX.h>
#include "tcUnicodeHelper.h"
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <Wire.h>
#include <Fonts/RobotoMedium24.h>
#include "Fonts/OpenSansCyrillicLatin18.h"

#define TFT_CS   22  // Chip select control pin
#define TFT_DC   17  // Data Command control pin
#define TFT_RST  16  // Reset pin (could connect to Arduino RESET pin)

//
// Here we create an adafruit display, but the font handler works equally well with U8G2, and TFT_eSPI displays.
// It also works with TcMenu drawable interface and the designer can generate suitable themes.
//
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

//
// Create an object that can draw fonts onto the display, this shows adafruit but
// if using U8G2 or TFT_eSPI you'd simply pass the pointer to your display object instead.
//
UnicodeFontHandler fontHandler(&tft, ENCMODE_UTF8);

int yAdaFontSize = 0;
int yOpenSansSize = 0;
int baselineAda = 0;
int baselineTcUni = 0;
const char helloText[] PROGMEM = "hello world";
const char helloUkraine[] PROGMEM = "Привіт Світ";

const GFXfont* adaFontToUse = &RobotoMedium24;
const UnicodeFont* unicodeFontTouse = OpenSansCyrillicLatin18;

void setup() {
    // on ESP32 you may need to adjust the SPI settings, un/comment below
    SPI.begin(18, 19, 23);

    // On Pico, you may need to set up the SPI pins, un/comment below
    //SPI.setSCK(2);
    //SPI.setTX(3);
    //SPI.setRX(4);

    // start up serial
    Serial.begin(115200);
    Serial.print(F("Graphics test"));

    // initialise the display
    tft.begin();
    tft.fillScreen(ILI9341_BLACK);

    //
    // Get the size of the Unicode open sans font that we are using, it is returned as a Coord object that has x and y.
    // the baseline, indicates the amount of space below the drawing point that's needed.
    //
    fontHandler.setFont(unicodeFontTouse);
    Coord openSansSize = fontHandler.textExtents_P(helloText, &baselineTcUni);
    yOpenSansSize = (int)openSansSize.y - baselineTcUni;

    //
    // Now get the size of the Adafruit_GFX font that we are using, same as above basically.
    //
    fontHandler.setFont(adaFontToUse);
    Coord textSize = fontHandler.textExtents_P(helloText, &baselineAda);
    yAdaFontSize = (int)textSize.y - baselineAda;

    //
    // Now we print some text on the first available line, just as with adafruit, the font is drawn with reference to
    // the baseline, so we need to add the Y size of the font without the baseline. This is using a regular adafruit
    // graphics font.
    //
    fontHandler.setCursor(0, yAdaFontSize);
    fontHandler.setFont(adaFontToUse);
    fontHandler.setDrawColor(ILI9341_WHITE);
    fontHandler.print_P(helloText);

    //
    // Now we print some text just below that that is in unicode, this text is in cyrillic.
    //
    fontHandler.setCursor(0, 30 + yOpenSansSize);
    fontHandler.setFont(unicodeFontTouse);
    fontHandler.print_P(helloUkraine);

    //
    // Now we print the X and Y sizes of the adafruit font
    //
    fontHandler.setCursor(0, yAdaFontSize + 60);
    fontHandler.setFont(adaFontToUse);
    fontHandler.print((int)textSize.x);
    fontHandler.setCursor(80, yAdaFontSize + 60);
    fontHandler.print((int)textSize.y);
}

void loop() {
    int whereY = 100;
    delay(250);
    tft.fillRect(0, whereY, tft.width(), 30, ILI9341_BLACK);
    tft.fillRect(0, whereY + yAdaFontSize, 70, baselineAda, ILI9341_RED);

    fontHandler.setCursor(0, yAdaFontSize + whereY);
    fontHandler.setFont(adaFontToUse);
    fontHandler.setDrawColor(ILI9341_CYAN);
    fontHandler.print(millis() / 1000);
    fontHandler.print("Agy");

    tft.fillRect(80, whereY + yOpenSansSize, 200, baselineTcUni, ILI9341_RED);
    fontHandler.setCursor(80, yOpenSansSize + whereY);
    fontHandler.setFont(unicodeFontTouse);
    fontHandler.setDrawColor(ILI9341_GREEN);
    fontHandler.print((float)millis() / 1000.0F);
    fontHandler.print("Agy");
}