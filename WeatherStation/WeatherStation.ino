// At this point in time the code is not complete and doesn't compile but you can use it
// as an example and have at least the beginning of this project and have something to 
// work of, I'll get this code working eventually but it's a bigger challenge than I expected.
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <time.h>
#include <HTTPClient.h>
#include "Orbitron_Medium_20.h"
#include "ani.h"

// includes below are for making WiFiManager work, this code is generated with the help of ChatGPT
// that you can find at https://openai.com and I highly recommend you to check it out because it
// already works really good, do keep in mind that it's still being tested at the time of writing
// so the output window is limited to an x amount of lines.
// To make this code complete I had to resort to do it in parts and then put the parts together
// myself which was a lot of work because of all the errors that I got during compiling the code.
// So the answer to my test if it's at all possible to use ChatGPT generating the code for this
// project and have it working first time, it did look promising at first glance but to get it
// to compile without problems... yeah, that's going to take me some time to figure this out.
// At least ChatGPT helped me a little bit giving me the start of some code that still needed to 
// be worked on to make it complete so it will actually work on the dev board.
#include <WiFiManager.h>
#include <ESP_DoubleResetDetector.h>
#include <ESP_MultiResetDetector.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <time.h>

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

#define drd = 0;
#define mrd = 0;
ESP_DoubleResetDetector drd(10); // create an instance of the DoubleResetDetector with a 10-second time window
ESP_MultiResetDetector mrd(5, 10000); // create an instance of the MultiResetDetector with a maximum of 5 resets within 10 seconds
WiFiClientSecure client; // create a secure WiFi client object to communicate with the API server

// Time settings
const char* timeServer = "time.nist.gov"; // specify the time server ie. "pool.ntp.org" or "time.nist.gov"
const int timeZone = 1; // specify the time zone offset in hours
const int NTP_PACKET_SIZE = 48; // size of the NTP packet
byte packetBuffer[NTP_PACKET_SIZE]; // buffer to hold the NTP packet

// Store date objects
struct tm timeinfo;
char dateFormat[80];
char secondFormat[80];
char timeFormat[80];

// Open Weather API example code, use by replacing with your data and uncommenting to activate
const char* apiHost = "api.openweathermap.org"; // specify the OpenWeatherMap API host
const char* apiKey = "83723267bd3c2f8aa9eb7d200cebd997"; // replace YOUR_API_KEY with your OpenWeatherMap API key
const String town = "Brunssum"; // replace with your town
const String country = "NL"; // replace with your country code
const char* apiEndpoint = "/data/2.5/weather?q="+town+","+country+"&units=metric&appid="; // specify the API endpoint

// Data
String payload = ""; // whole Json
String tmp = "";     // temperature
String hum = "";     // humidity

void setup() {
  pinMode(0,INPUT_PULLUP);
  pinMode(35,INPUT);
  Serial.begin(115200); // Initialize the serial monitor

  // Initialize screen
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextSize(1);

  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(TFT_BLUE, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, backlight[b]);

  if (drd.detect()) {
    Serial.println("Double reset detected. Clearing WiFi credentials.");
    WiFiManager wifiManager;
    wifiManager.resetSettings();
  }

  int resetReason = mrd.detect();
  if (resetReason == 0) {
    Serial.println("No reset detected.");
  } else {
    Serial.print(resetReason);
    Serial.println(" resets detected within 10 seconds. Clearing WiFi credentials.");
    WiFiManager wifiManager;
    wifiManager.resetSettings();
  }

  WiFiManager wifiManager;
  if (!wifiManager.autoConnect("AutoConnectAP")) {
    Serial.println("Failed to connect and hit timeout.");
    delay(3000);
    ESP.restart();
  }

  Serial.println("Connected to WiFi!");
  Serial.println(WiFi.localIP());

  client.setInsecure();
  setSyncProvider(getNtpTime); // set the time sync function
  setSyncInterval(300); // set the time sync interval to 5 minutes
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
      tft.invertedDisplay(inverted);
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

  if (timeStatus() == timeSet) {
    // print the current time
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(" - ");

    // get the weather data
    String weatherData = getWeatherData(apiHost, apiEndpoint, apiKey);

    // parse the weather data
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, weatherData);
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

  delay(1000);
}

// helper function to print digits with leading zero
void printDigits(int digits) {
  Serial.print(":");
  if (digits < 10) {
    Serial.print('0');
  }
  Serial.print(digits);
}

// function to get the time from the NTP server
time_t getNtpTime() {
  while (udp.parsePacket() > 0); // discard any previously received packets
  Serial.println("Sending NTP request...");
  udp.beginPacket(timeServer, 123); // send an NTP request
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[2] = 6;
  packetBuffer[3] = 0xEC;
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
  if (udp.parsePacket() == 0) {
    Serial.println("No response from time server.");
    return 0;
  }
  udp.read(packetBuffer, NTP_PACKET_SIZE); // read the NTP response
