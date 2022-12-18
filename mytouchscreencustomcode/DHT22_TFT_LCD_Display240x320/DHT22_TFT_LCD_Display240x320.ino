// Using the ASAIR AM2302 Temp/Humidity sensor with a Velleman VMA412 2.8" 240x320 resistive touch screen
// Draws a thermometer bulb graphic with two buttons for you to choose to display the temperature in
// degree Celsius or Fahrenheit and shows humidity on an analog meter needle display.
//
// Based on Adafruit sensor library example sketches
// REQUIRES the following libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
//
// Inspired and forked from sketch/project:
// GadgetReboot YouTube video
// https://github.com/GadgetReboot/Arduino/tree/master/Uno/DS18B20_TFT_LCD
//
// On-screen button interface is based on the button_simple.ino demo sketch included with
// the MCUFRIEND_kbv library
//
// For the latest updates, more details and a schematic, refer to my GitHub page:
// https://github.com/soharddbwarez/Arduino/tree/master/Uno/DHT22
//
// Tested with Arduino IDE v1.8.12
//             Adafruit GFX Library v1.7.5
//             Adafruit TouchScreen v1.0.4
//             Adafruit Unified Sensor v1.1.2
//             DHT sensor library v1.3.8
//             MCUFRIEND_kbv v2.9.8
//
//
// Gadget Reboot customized by Ricardo Penders

#include "DHT.h"
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

#define DHTPIN 19       // Analog pin A5 used as digital pin 19 connected to the DHT sensor
#define DHTTYPE DHT22   // can be DHT11, DHT21, DHT22

// create a DHT sensor object
DHT dht(DHTPIN, DHTTYPE);

// run TouchScreen_Calibr_native.ino in MCUFRIEND_kbv examples to calibrate screen
// copy calibration data from serial monitor for XP, XM, YP, YM, TS_LEFT, TS_RT, TS_TOP, TS_BOT
// const int XP = 6, XM = 15, YP = 16, YM = 7; //480x320 ID=0x9341
// const int TS_LEFT = 920, TS_RT = 139, TS_TOP = 944, TS_BOT = 150;
const int XP = 8, XM = A2, YP = A3, YM = 9; //240x320 ID=0x9341
const int TS_LEFT = 910, TS_RT = 108, TS_TOP = 76, TS_BOT = 901;

MCUFRIEND_kbv tft;                                 // create display object
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); // create touch screen object

// assign human-readable names to some common 16-bit color values:
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF
#define GRAY     0xC618
#define DARKGRAY 0x6B0C
#define ORANGE   0xFA60
#define LIME     0x07FF
#define AQUA     0x04FF
#define PINK     0xF8FF

// touch pressure sensitivity thresholds to detect screen presses
#define MINPRESSURE 10
#define MAXPRESSURE 1000

// used to control/check if units should be displayed in C or F (makes code more readable)
#define  degC false
#define  degF true

// misc variables
uint16_t      tsID;                                     // touch screen driver type ID
int           pixel_x;                                  // touch screen pressed location
int           pixel_y;                                  // touch screen pressed location
uint8_t       Orientation      = 0;                     // portrait screen orientation
int           prevTempLevel    = 0;                     // store last thermometer reading level
bool          displayTempUnits = degC;                  // default to displaying in degrees C
float         humidity;                                 // humidity sensor reading
float         displayTemp;                              // temperature sensor reading
unsigned long timeInterval     = 2000;                  // time interval between DHT readings in mS
unsigned long timeLapsed;                               // time lapsed since last timer expiry

// humidity gauge
#define meterScaling 1.0000                        // resize the meter if desired. original = 1.3333
float    ltx = 0;                                  // saved x coord of bottom of needle
uint16_t osx = meterScaling * 100, osy = meterScaling * 100; // saved x & y coords analog meter
uint32_t updateTime = 0;                           // time for next update
int      old_analog =  -999;                       // value last displayed
int      value[6] = {0, 0, 0, 0, 0, 0};
int      old_value[6] = { -1, -1, -1, -1, -1, -1};

// thermometer graphic coordinates and size
#define thermXTop      180  // thermometer tube top center x coordinate      
#define thermYTop      (meterScaling * 100) + 90  // thermometer tube top center y coordinate    
#define thermHeight    100  // thermometer tube height                      
#define thermTubeWidth  20  // thermometer tube width                      
#define thermBulbRadius 20  // thermometer bulb radius                     

// create two touch screen buttons
Adafruit_GFX_Button degC_btn;
Adafruit_GFX_Button degF_btn;

void setup(void) {

  Serial.begin(9600);

  // initialize the DHT sensor
  dht.begin();

  // initialize Touch Screen
  tft.reset();
  tsID = tft.readID();                                  // find what driver is on the TFT LCD
  tft.begin(tsID);
  tft.invertDisplay(false);                              // invert screen colors if this driver requires it
  tft.setRotation(Orientation);                         // set portrait or landscape mode
  tft.fillScreen(BLACK);                                // clear the screen

  // display box for numerical temperature and humidity
  // y-position is offset by meterScaling*126, which is the y-height of the humidity gauge
  // so re-sizing the gauge allows other graphics to be drawn in relation to analog humidity gauge at the top of the screen.
  //
  // This is now customized so it fits on a smaller screen which is the 2.8" 240x320, originally this sketch was written for
  // the 3.2" 480x320 display and it does work on a smaller screen but the whole graphics are out of whack, not readable and
  // much of it is out of screen so totally not usable, and I've tried many different values for the meterScaling*126 thing
  // and 100 looks like the right value for my screen.
  // At this point I have everything working and scaled down so it all fits nicely on the screen, there's only one thing left
  // which is the animated needle position, it needs to go further up.
  tft.drawRect(5, (meterScaling * 100) + 5, 230, 60, MAGENTA);
  tft.fillRect(10, (meterScaling * 100) + 10, 220, 50, DARKGRAY);
  tft.setTextColor(WHITE, DARKGRAY);        // printer header text
  tft.setTextSize(1);
  tft.setCursor(35, (meterScaling * 120) + 1);
  tft.print("Temperature        Humidity");

  // main thermometer graphic display box area
  tft.drawRect(5, (meterScaling * 100) + 70, 230, 150, WHITE);
  tft.fillRect(10, (meterScaling * 100) + 75, 220, 140, GRAY);

  // draw thermometer background graphics
  drawThermometer();

  // set up two on-screen buttons to switch between deg C and F
  degC_btn.initButton(&tft, 80, (meterScaling * 100) + 120, 100, 40, WHITE, CYAN, BLACK, "deg C", 2);
  degF_btn.initButton(&tft, 80, (meterScaling * 100) + 180, 100, 40, WHITE, CYAN, BLACK, "deg F", 2);
  degC_btn.drawButton(false);
  degF_btn.drawButton(false);

  // draw humidity gauge background graphics
  analogMeter();
  plotNeedle(0, 0);
}

void loop() {

  // if it is time to read from the DHT sensor, take a temp/humidity reading
  if (millis() - timeLapsed > timeInterval)  {
    // reading temperature or humidity takes about 250 milliseconds
    // read humidity
    humidity = dht.readHumidity();

    // read temperature: displayTempUnits true=degF false=degC
    displayTemp = dht.readTemperature(displayTempUnits);

    // check if any reads failed
    if (isnan(humidity) || isnan(displayTemp)) {
      Serial.println(F("Failed to read from DHT sensor!"));
    }
    else {
      Serial.print(F("Humidity: "));
      Serial.print(humidity);
      Serial.print(F("%  Temperature: "));
      Serial.print(displayTemp);
      if (displayTempUnits == degC) {
        Serial.print(F("°C "));
      }
      else {
        Serial.print(F("°F "));
      }
      Serial.println();
    }


    // display temperature and humidity numerically in header box
    tft.setTextColor(WHITE, DARKGRAY);
    tft.setTextSize(2);
    tft.setCursor(35, (meterScaling * 100) + 40);
    tft.print(displayTemp, 1);                    // 1 decimal place because the second is always zero
    tft.print((char)247);                         // degree symbol
    tft.setTextSize(2);
    if (displayTempUnits == degC)
      tft.print("C  ");
    else
      tft.print("F  ");

    tft.setCursor(150, (meterScaling * 100) + 40);
    tft.print(humidity, 1);                       // 1 decimal place because the second is always zero
    tft.print("%");

    // update thermometer level graphic if temperature is within graphical range
    if ((displayTemp <= 100 ) && (displayTemp >= 0 )) {
      int curTempLevel = map(displayTemp, 0, 100, 0, (thermHeight - (thermBulbRadius)));

      // update thermometer level if height has changed
      if ((prevTempLevel - curTempLevel) != 0) {
        // draw new red level
        tft.fillRect((thermXTop - (thermTubeWidth / 2) + 6),
                     thermYTop + ((thermHeight - (thermBulbRadius) - curTempLevel)),
                     (thermTubeWidth - 12),
                     curTempLevel,
                     RED);

        // draw new white background above red level height in tube
        tft.fillRect((thermXTop - (thermTubeWidth / 2) + 3),
                     thermYTop,
                     (thermTubeWidth - 6), (thermHeight - (thermBulbRadius) - curTempLevel),
                     WHITE);
      }
      prevTempLevel = curTempLevel; // store bar height for next iteration
    }

    timeLapsed = millis();
  }

  // update analog humidity meter needle
  plotNeedle(humidity, 10);

  // check if either of the on screen buttons are pressed and
  // change between deg C and F display as required
  bool buttonPressed = Touch_getXY();
  degC_btn.press(buttonPressed && degC_btn.contains(pixel_x, pixel_y));
  degF_btn.press(buttonPressed && degF_btn.contains(pixel_x, pixel_y));

  if (degC_btn.justReleased())    // switch to showing degrees C
    degC_btn.drawButton();
  if (degC_btn.justPressed()) {
    degC_btn.drawButton(true);
    displayTempUnits = degC;
  }

  if (degF_btn.justReleased())    // switch to showing degrees F
    degF_btn.drawButton();
  if (degF_btn.justPressed()) {
    degF_btn.drawButton(true);
    displayTempUnits = degF;
  }
}  // end void loop()

// get touch screen pixel location if screen is touched
bool Touch_getXY(void)
{
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);      // restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);   // because TFT control pins
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());  // map x location from analog pos. reading to screen width
    pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height()); // map y location from analog pos. reading to screen height
  }
  return pressed;
}

// draw the background graphics for the thermometer and markings
// this is only drawn once, then the temperature bar is adjusted as needed
void drawThermometer(void) {

  // draw thermometer tube outline
  tft.fillRoundRect((thermXTop - (thermTubeWidth / 2)), thermYTop - (thermTubeWidth / 2) + 1,
                    thermTubeWidth, thermHeight, thermTubeWidth / 2, BLACK);                             // tube
  tft.fillCircle(thermXTop, (thermYTop + thermHeight), (thermBulbRadius), BLACK);                        // bottom bulb

  // fill white thermometer tube inside
  tft.fillRoundRect((thermXTop - (thermTubeWidth / 2) + 3), thermYTop - (thermTubeWidth / 2) + 5,
                    (thermTubeWidth - 6), thermHeight, (thermTubeWidth / 2) - 0.5, WHITE);               // tube
  tft.fillCircle(thermXTop, (thermYTop + thermHeight), (thermBulbRadius - 3), WHITE);                    // bottom bulb

  // fill red thermometer bulb
  tft.fillCircle(thermXTop, (thermYTop + thermHeight), (thermBulbRadius - 6), RED);

  // fill red tube to connect between lowest reading level and the bulb reservoir
  tft.fillRect((thermXTop - (thermTubeWidth / 2) + 6), thermYTop + thermHeight - thermBulbRadius, (thermTubeWidth - 12), 7, RED);

  // draw line markings beside thermometer for visual reference levels
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, thermYTop, 8, 2, BLACK);                                             // top level
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.75), 5, 2, BLACK);  // 1/4 from top
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.50), 8, 2, BLACK);  // center
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.25), 5, 2, BLACK);  // 1/4 from bot
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, ((thermYTop + thermHeight) - thermBulbRadius), 8, 2, BLACK);         // bottom level

  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.95), 3, 2, BLACK);  //  "5" level
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.90), 3, 2, BLACK);  // "10" level
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.85), 3, 2, BLACK);  // "15" level
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.80), 3, 2, BLACK);  // "20" level

  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.70), 3, 2, BLACK);  // "30" level
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.65), 3, 2, BLACK);  // "35" level
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.60), 3, 2, BLACK);  // "40" level
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.55), 3, 2, BLACK);  // "45" level

  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.45), 3, 2, BLACK);  // "55" level
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.40), 3, 2, BLACK);  // "60" level
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.35), 3, 2, BLACK);  // "65" level
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.30), 3, 2, BLACK);  // "70" level

  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.20), 3, 2, BLACK);  // "80" level
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.15), 3, 2, BLACK);  // "85" level
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.10), 3, 2, BLACK);  // "90" level
  tft.fillRect(thermXTop + (thermTubeWidth / 2) + 1, (thermYTop + (thermHeight - thermBulbRadius) * 0.05), 3, 2, BLACK);  // "95" level

  // draw thermometer level indicator numbers
  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.setCursor(((thermXTop + (thermTubeWidth / 2)) + 10), (thermYTop - 5));
  tft.print("100");
  tft.setTextSize(1);
  tft.print((char)247);

  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.setCursor(((thermXTop + (thermTubeWidth / 2)) + 10), (thermYTop + (thermHeight - thermBulbRadius) * 0.25) - 5);
  tft.print("75");
  tft.setTextSize(1);
  tft.print((char)247);

  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.setCursor(((thermXTop + (thermTubeWidth / 2)) + 10), (thermYTop + (thermHeight - thermBulbRadius) * 0.5) - 5);
  tft.print("50");
  tft.setTextSize(1);
  tft.print((char)247);

  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.setCursor(((thermXTop + (thermTubeWidth / 2)) + 10), (thermYTop + (thermHeight - thermBulbRadius) * 0.75) - 5);
  tft.print("25");
  tft.setTextSize(1);
  tft.print((char)247);

  tft.setTextColor(BLACK);
  tft.setTextSize(1);
  tft.setCursor(((thermXTop + (thermTubeWidth / 2)) + 10), (thermYTop + thermHeight - thermBulbRadius - 5));
  tft.print("0");
  tft.setTextSize(1);
  tft.print((char)247);
}  // end void drawThermometer(void)

// draw analog meter background graphics
void analogMeter() {

  // meter outline
  tft.fillRect(0, 0, meterScaling * 239, meterScaling * 100, GRAY);
  tft.fillRect(5, 3, meterScaling * 230, meterScaling *  93, WHITE);

  // text color
  tft.setTextColor(BLACK);

  // draw ticks every 5 degrees from -50 to +50 degrees (100 deg. FSD swing)
  // long scale tick length
  for (int i = -50; i < 51; i += 5) {

    // coordinates of tick to draw
    int tl = 15;
    float sx = cos((i - 90) * 0.0174532925);
    float sy = sin((i - 90) * 0.0174532925);
    uint16_t x0 = sx * (meterScaling * 100 + tl) + meterScaling * 100;
    uint16_t y0 = sy * (meterScaling * 100 + tl) + meterScaling * 120;
    uint16_t x1 = sx * meterScaling * 100 + meterScaling * 100;
    uint16_t y1 = sy * meterScaling * 100 + meterScaling * 120;

    // coordinates of next tick for zone fill
    float sx2 = cos((i + 5 - 90) * 0.0174532925);
    float sy2 = sin((i + 5 - 90) * 0.0174532925);
    int x2 = sx2 * (meterScaling * 100 + tl) + meterScaling * 100;
    int y2 = sy2 * (meterScaling * 100 + tl) + meterScaling * 120;
    int x3 = sx2 * meterScaling * 100 + meterScaling * 100;
    int y3 = sy2 * meterScaling * 100 + meterScaling * 120;

    // green zone limits
    if (i >= 0 && i < 25) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, GREEN);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, GREEN);
    }

    // red zone limits
    if (i >= 25 && i < 50) {
      tft.fillTriangle(x0, y0, x1, y1, x2, y2, RED);
      tft.fillTriangle(x1, y1, x2, y2, x3, y3, RED);
    }

    // short scale tick length
    if (i % 25 != 0) tl = 8;

    // recalculate coords incase tick lenght changed
    x0 = sx * (meterScaling * 100 + tl) + meterScaling * 100;
    y0 = sy * (meterScaling * 100 + tl) + meterScaling * 120;
    x1 = sx * meterScaling * 100 + meterScaling * 100;
    y1 = sy * meterScaling * 100 + meterScaling * 120;

    // draw tick
    tft.drawLine(x0, y0, x1, y1, BLACK);

    // check if labels should be drawn, with position tweaks
    if (i % 25 == 0) {
      // Calculate label positions
      x0 = sx * (meterScaling * 100 + tl + 10) + meterScaling * 100;
      y0 = sy * (meterScaling * 100 + tl + 10) + meterScaling * 120;
      switch (i / 25) {
      }
    }

    // now draw the arc of the scale
    sx = cos((i + 5 - 90) * 0.0174532925);
    sy = sin((i + 5 - 90) * 0.0174532925);
    x0 = sx * meterScaling * 100 + meterScaling * 100;
    y0 = sy * meterScaling * 100 + meterScaling * 120;

    // draw scale arc, don't draw the last part
    if (i < 50) tft.drawLine(x0, y0, x1, y1, BLACK);

    // Finally - plot scale numbers on the scale
    // these numbers are drawn at a fixed location to match
    // the cusomized meterScaling=1.0000 size.  
    // This part doesn't auto adjust.
    tft.setTextSize(1);

    tft.setCursor(24, 60);
    tft.println("0");

    tft.setCursor(64, 25);
    tft.println("25");

    tft.setCursor(114, 15);
    tft.println("50");

    tft.setCursor(164, 25);
    tft.println("75");

    tft.setCursor(210, 60);
    tft.println("100");
  }
  // draw bezel line
  tft.drawRect(5, 3, meterScaling * 230, meterScaling * 93, BLACK);
}  // end void analogMeter()

// needle psoition updater
// this function is blocking while needle moves, time depends on ms_delay
// 10ms minimises needle flicker if text is drawn within needle sweep area
// Smaller values OK if text not in sweep area, zero for instant movement but
// does not look realistic... (note: 100 increments for full scale deflection)

void plotNeedle(int value, byte ms_delay) {

  tft.setTextColor(BLACK, WHITE);
  char buf[8]; dtostrf(value, 4, 0, buf);
  if (value < -10) value = -10;               // Limit value to emulate needle end stops
  if (value > 110) value = 110;

  // move the needle until new value reached
  while (!(value == old_analog)) {
    if (old_analog < value) old_analog++;
    else old_analog--;

    if (ms_delay == 0) old_analog = value;     // Update immediately if delay is 0

    float sdeg = map(old_analog, -10, 110, -150, -30); // Map value to angle

    // calculate tip of needle coords
    float sx = cos(sdeg * 0.0174532925);
    float sy = sin(sdeg * 0.0174532925);

    // calculate x delta of needle start (does not start at pivot point)
    float tx = tan((sdeg + 90) * 0.0174532925);

    // erase old needle image
    tft.drawLine(meterScaling * (100 + 20 * ltx - 1), meterScaling * (120 - 20), osx - 1, osy, WHITE);
    tft.drawLine(meterScaling * (100 + 20 * ltx), meterScaling * (120 - 20), osx, osy, WHITE);
    tft.drawLine(meterScaling * (100 + 20 * ltx + 1), meterScaling * (120 - 20), osx + 1, osy, WHITE);

    // re-plot text under needle
    tft.setTextColor(BLACK);

    // store new needle end coords for next erase
    ltx = tx;
    osx = meterScaling * (sx * 98 + 100);
    osy = meterScaling * (sy * 98 + 120);

    // draw the needle in the new position, magenta makes needle a bit bolder
    // draws 3 lines to thicken needle
    tft.drawLine(meterScaling * (100 + 20 * ltx - 1), meterScaling * (120 - 20), osx - 1, osy, RED);
    tft.drawLine(meterScaling * (100 + 20 * ltx), meterScaling * (120 - 20), osx, osy, MAGENTA);

    // try to erase old line
    tft.drawLine(meterScaling * (100 + 20 * ltx), meterScaling * (120 - 20), osx, osy, MAGENTA);
    tft.drawLine(meterScaling * (100 + 20 * ltx + 1), meterScaling * (120 - 20), osx + 1, osy, RED);

    // slow needle down slightly as it approaches new postion
    if (abs(old_analog - value) < 10) ms_delay += ms_delay / 5;

    // wait before next update
    delay(ms_delay);
  }
}  // end plotNeedle
