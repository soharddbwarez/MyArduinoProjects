/* This project is made by Ricardo Penders (soharddbwarez) and you can find it on GitHub
where I share all my projects. This particular project you can find here:
https://github.com/soharddbwarez/MyArduinoProjects/tree/master/WeatherStation

The code of the original WeatherStation project wasn't really working for me and it's
missing some important things that I've added, I also changed the better part of the
code and added comments to help you understand what the code is doing.

I also added the WiFiManager part with the DoubleResetDetection function that
someone asked the developer of the original project if it's possible to add,
so because he didn't get any answer I decided to look into it and I finally
got it working after several tries using different libraries.

It took me a while but the code is now verified and should be working on
most of the ESP32 boards, so for those who asked or wanted to have a way
to save the network credentials dynamically. Well, here you go...

Have fun with it.
*/
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <time.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>                           // https://github.com/bblanchon/ArduinoJson
#include "Orbitron_Medium_20.h"
#include "ani.h"

#if defined(ESP32)
  #define USE_SPIFFS              true
  #define ESP_DRD_USE_EEPROM      true
#else
  #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif
#include <ESP_WiFiManager.h>                      // https://github.com/khoih-prog/ESP_WiFiManager
#define DRD_TIMEOUT                   10
#define DRD_ADDRESS                   0
#include <ESP_DoubleResetDetector.h>               // https://github.com/khoih-prog/ESP_DoubleResetDetector
DoubleResetDetector* drd;
const int PIN_LED                   = 2;           
bool      initialConfig             = false;

// Define colors for the display
#define TFT_GREY 0x5AEB
#define TFT_LIGHTBLUE 0x01E9
#define TFT_DARKRED 0xA041

StaticJsonDocument<1000> doc;

// Display settings
TFT_eSPI tft = TFT_eSPI();
const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;
int backlight[5] = {10,30,60,120,220}; // Brightness level options
byte b=1; // Starting at Brightness level 1

// Time settings
const char* ntpServer = "time.nist.gov"; // specify the time server ie. "pool.ntp.org" or "time.nist.gov"
const int gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

// Store date objects
struct tm timeinfo;
char dateFormat[80];
char secondFormat[80];
char timeFormat[80];

// Open Weather API example code, use by replacing with your data and uncommenting to activate
const String key = "YOUR_API_KEY"; // replace YOUR_API_KEY with your OpenWeatherMap API key
const String town = "Brunssum"; // replace with your town
const String country = "NL"; // replace with your country code
const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q="+town+","+country+"&units=metric&appid="; // specify the API endpoint

// Data
String payload = ""; // whole Json
String tmp = "";     // temperature
String hum = "";     // humidity

void setup() {
  pinMode(0,INPUT_PULLUP);
  pinMode(35,INPUT);
  pinMode(PIN_LED, OUTPUT);

  // Start debug console
  Serial.begin(115200);
  while (!Serial);
  delay(200);
  Serial.print(F("\nStarting WeatherStation with ConfigOnDoubleReset on ")); Serial.println(ARDUINO_BOARD);
  Serial.println(ESP_WIFIMANAGER_VERSION);
  drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);
  if (drd->detectDoubleReset()) { Serial.println(F("DRD")); initialConfig = true; }
  ESP_WiFiManager ESP_wifiManager("ConfigOnDoubleReset");
  if (ESP_wifiManager.WiFi_SSID() == "") { Serial.println(F("No AP credentials")); initialConfig = true; }
  if (initialConfig) {
    Serial.println(F("Starting Config Portal")); digitalWrite(PIN_LED, HIGH);
    if (!ESP_wifiManager.startConfigPortal()) { Serial.println(F("Not connected to WiFi")); }
    else { Serial.println(F("Connected")); }
  }
  else { WiFi.mode(WIFI_STA); WiFi.begin(); }
  unsigned long startedAt = millis();
  digitalWrite(PIN_LED, LOW); Serial.print(F("After waiting "));
  int connRes = WiFi.waitForConnectResult();
  float waited = (millis() - startedAt);
  Serial.print(waited / 1000); Serial.print(F(" secs , Connection result is ")); Serial.println(connRes);
  if (WiFi.status() != WL_CONNECTED) { Serial.println(F("Failed to connect")); }
  else { Serial.print(F("Local IP: ")); Serial.println(WiFi.localIP()); }

  // Initialize screen
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextSize(1);

  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(TFT_BLUE, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, backlight[b]);
  
  tft.println("");
  tft.println("WiFi connected.");
  tft.println("IP address: ");
  tft.println(WiFi.localIP());
  delay(3000);
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  
  // Screen setup
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);

  // IP address
  tft.setCursor(2, 232, 1);
  tft.println(WiFi.localIP());
  
  // Bright label
  tft.setCursor(80, 204, 1);
  tft.println("BRIGHT:");

  // Seconds label
  tft.setCursor(80, 152, 2);
  tft.println("SEC:");

  // Temperature label
  tft.setTextColor(TFT_WHITE,TFT_LIGHTBLUE);
  tft.setCursor(4, 152, 2);
  tft.println("TEMP:");

  // Humidity label
  tft.setCursor(4, 192, 2);
  tft.println("HUM: ");

  // Town
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setFreeFont(&Orbitron_Medium_20);
  tft.setCursor(6, 82);
  tft.println(town);

  // Vertical line
  tft.fillRect(68,152,1,74,TFT_GREY);

  // Brightness status
  for(int i=0;i<b+1;i++) {
    tft.fillRect(78+(i*7),216,3,10,TFT_BLUE);
  }
}

int counter = 0;        // Count time after last data update
int period = 2000;      // Period for data request
int btnLeft = 0;        // Invert display colors
int btnRight = 0;       // Brightness controll
int frame = 0;          // Frame for animation
String currentDate;
String currentSec;
String currentTime;
bool inverted = 1;      // Display invert state

void loop() {
  drd->loop();
  
  // Animation
  tft.pushImage(0, 88, 135, 65, ani[frame]);
  frame++;
  if(frame >= 10)
  frame = 0;

  // Brightness control
  if(digitalRead(35)==0) {
    if(btnRight==0) {
      btnRight=1;
      tft.fillRect(78,216,44,12,TFT_BLACK);
      b++;

      if(b>=5) {
        b=0;
      }

      for(int i=0;i<b+1;i++) {
        tft.fillRect(78+(i*7),216,3,10,TFT_BLUE);
        ledcWrite(pwmLedChannelTFT, backlight[b]);
      }
    }
  } else {
    btnRight=0;
  }
  
  // Invert screen color
  if(digitalRead(0)==0) {
    if(btnLeft==0) {
      btnLeft=1;
      inverted=!inverted;
      tft.invertDisplay(inverted);
    }
  } else {
    btnLeft=0;
  }

  //Data request every period
  if(counter==0) {
    getData();
    counter++;
  }

  if(counter>period) {
    counter=0;
  }

  // Temperature value
  tft.setFreeFont(&Orbitron_Medium_20);
  tft.setCursor(2, 187);
  tft.println(tmp.substring(0,2));
  
  // Humidity value
  tft.setCursor(2, 227);
  tft.println(hum+"%");
  printLocalTime();
  
  // Display date
  if (currentDate != dateFormat) {
    currentDate = dateFormat;
    tft.setTextColor(TFT_ORANGE,TFT_BLACK);
    tft.setTextFont(2);
    tft.setCursor(6, 44);
    tft.println(dateFormat);
  }

  // Display seconds
  if (currentSec != secondFormat) {
    currentSec = secondFormat;
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.fillRect(78,170,48,28,TFT_DARKRED);
    tft.setFreeFont(&Orbitron_Light_24);
    tft.setCursor(81, 192);
    tft.println(secondFormat);
  }

  // Display time
  if (currentTime != timeFormat) {
    currentTime = timeFormat;
    tft.setFreeFont(&Orbitron_Light_32);
    tft.fillRect(3,8,120,30,TFT_BLACK);
    tft.setCursor(5, 34);
    tft.println(timeFormat);
  
    // parse the weather data
    float temperature = doc["main"]["temp"].as<float>();
    float humidity = doc["main"]["humidity"].as<float>();
    float pressure = doc["main"]["pressure"].as<float>();
    float windSpeed = doc["wind"]["speed"].as<float>();

    // print the weather data
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("C, Humidity: ");
    Serial.print(humidity);
    Serial.print("%, Pressure: ");
    Serial.print(pressure);
    Serial.print("hPa, Wind Speed: ");
    Serial.print(windSpeed);
    Serial.println("m/s");
  }
  delay(80);
}

void printLocalTime() {
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  strftime(dateFormat,80,"%d %B %Y", &timeinfo);
  strftime(timeFormat,80,"%H:%M", &timeinfo);
  strftime(secondFormat,80,"%S", &timeinfo);
  
  Serial.println(&timeinfo, "%A, %d %B %Y %H:%M:%S");
}

void getData() {
  tft.fillRect(1,170,64,20,TFT_BLACK);
  tft.fillRect(1,210,64,20,TFT_BLACK);
    if (WiFi.status() == WL_CONNECTED) { //Check the current connection status
 
    HTTPClient http;
 
    http.begin(endpoint + key); //Specify the URL
    int httpCode = http.GET();  //Make the request
 
    if (httpCode > 0) { //Check for the returning code
      payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
  char inp[1000];
  payload.toCharArray(inp,1000);
  deserializeJson(doc,inp);
  
  String tmp2 = doc["main"]["temp"];
  String hum2 = doc["main"]["humidity"];
  tmp=tmp2;
  hum=hum2;
}
