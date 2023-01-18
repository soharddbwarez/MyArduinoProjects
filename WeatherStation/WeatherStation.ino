#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <time.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Orbitron_Medium_20.h"
#include "ani.h"
/****************************************************************************************************************************
  ESP_WiFi.ino
  For ESP8266 / ESP32 boards
  ESP_WiFiManager_Lite (https://github.com/khoih-prog/ESP_WiFiManager_Lite) is a library 
  for the ESP32/ESP8266 boards to enable store Credentials in EEPROM/SPIFFS/LittleFS for easy 
  configuration/reconfiguration and autoconnect/autoreconnect of WiFi and other services without Hardcoding.
  Built by Khoi Hoang https://github.com/khoih-prog/ESP_WiFiManager_Lite
  Licensed under MIT license
*****************************************************************************************************************************/

#include <ESP_WiFiManager_Lite.h>  
#include "defines.h"
#include "Credentials.h"
#include "dynamicParams.h"

ESP_WiFiManager_Lite* ESP_WiFiManager;

void heartBeatPrint()
{
  static int num = 1;

  if (WiFi.status() == WL_CONNECTED)
    Serial.print("H");        // H means connected to WiFi
  else
  {
    if (ESP_WiFiManager->isConfigMode())
      Serial.print("C");        // C means in Config Mode
    else
      Serial.print("F");        // F means not connected to WiFi  
  }

  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(F(" "));
  }
}

void check_status()
{
  static unsigned long checkstatus_timeout = 0;

  //KH
#define HEARTBEAT_INTERVAL    20000L
  // Print hearbeat every HEARTBEAT_INTERVAL (20) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = millis() + HEARTBEAT_INTERVAL;
  }
}

#if USING_CUSTOMS_STYLE
const char NewCustomsStyle[] /*PROGMEM*/ = "<style>div,input{padding:5px;font-size:1em;}input{width:95%;}body{text-align: center;}\
button{background-color:blue;color:white;line-height:2.4rem;font-size:1.2rem;width:100%;}fieldset{border-radius:0.3rem;margin:0px;}</style>";
#endif

// Define colors for display
#define TFT_GREY 0x5AEB
#define TFT_LIGHTBLUE 0x01E9
#define TFT_DARKRED 0xA041

TFT_eSPI tft = TFT_eSPI();
StaticJsonDocument<1000> doc;

// Display settings
const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;

// Time settings
const char* ntpServer = "pool.ntp.org";
const int gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
struct tm timeinfo;
// Store date objects
char dateFormat[80];
char secondFormat[80];
char timeFormat[80];

// Open Weather API
const String key = "27634cd6d75ba60a8d4850be223a9c43"; // Enter your API key from openweathermap.org
const String town = "Brunssum";
const String country = "NL";
const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q="+town+","+country+"&units=metric&APPID=";

// Data
String payload = "";  // whole json 
String tmp = "";      // temperature
String hum = "";      // humidity

int backlight[5] = {10,30,60,120,220};  // Brightness options
byte b=1;                               // Brighthess level

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

void setup(void) {
  pinMode(0,INPUT_PULLUP);
  pinMode(35,INPUT);

  // Initialize screen
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setTextSize(1);

  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(TFT_BL, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, backlight[b]);
  
  /* This is the old part of the code
  Serial.begin(115200);

  // Initialize WiFi
  tft.print("Connecting to ");
  tft.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_pw);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    tft.print(".");
  }
  
  tft.println("");
  tft.println("WiFi connected.");
  tft.println("IP address: ");
  tft.println(WiFi.localIP());
  delay(3000);
  */
  
  // Debug console
  Serial.begin(115200);
  while (!Serial);

  delay(200);

  Serial.print(F("\nStarting ESP_WiFi using ")); Serial.print(FS_Name);
  Serial.print(F(" on ")); Serial.println(ARDUINO_BOARD);
  Serial.println(ESP_WIFI_MANAGER_LITE_VERSION);

  #if USING_MRD  
    Serial.println(ESP_MULTI_RESET_DETECTOR_VERSION);
  #else
    Serial.println(ESP_DOUBLE_RESET_DETECTOR_VERSION);
  #endif

  ESP_WiFiManager = new ESP_WiFiManager_Lite();

  String AP_SSID = "your_customized_ssid";
  String AP_PWD  = "your_customized_pwd";
  
  // Set customized AP SSID and PWD
  ESP_WiFiManager->setConfigPortal(AP_SSID, AP_PWD);

  // Optional to change default AP IP(192.168.4.1) and channel(10)
  //ESP_WiFiManager->setConfigPortalIP(IPAddress(192, 168, 120, 1));
  ESP_WiFiManager->setConfigPortalChannel(0);

  #if USING_CUSTOMS_STYLE
    ESP_WiFiManager->setCustomsStyle(NewCustomsStyle);
  #endif

  #if USING_CUSTOMS_HEAD_ELEMENT
    ESP_WiFiManager->setCustomsHeadElement("<style>html{filter: invert(10%);}</style>");
  #endif

  #if USING_CORS_FEATURE  
    ESP_WiFiManager->setCORSHeader("Your Access-Control-Allow-Origin");
  #endif

  // Set customized DHCP HostName
  ESP_WiFiManager->begin(HOST_NAME);
  //Or use default Hostname "ESP32-WIFI-XXXXXX"
  //ESP_WiFiManager->begin();

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

#if USE_DYNAMIC_PARAMETERS
void displayCredentials() {
  Serial.println(F("\nYour stored Credentials :"));

  for (uint16_t i = 0; i < NUM_MENU_ITEMS; i++)
  {
    Serial.print(myMenuItems[i].displayName);
    Serial.print(F(" = "));
    Serial.println(myMenuItems[i].pdata);
  }
}

void displayCredentialsInLoop() {
  static bool displayedCredentials = false;

  if (!displayedCredentials)
  {
    for (int i = 0; i < NUM_MENU_ITEMS; i++)
    {
      if (!strlen(myMenuItems[i].pdata))
      {
        break;
      }

      if ( i == (NUM_MENU_ITEMS - 1) )
      {
        displayedCredentials = true;
        displayCredentials();
      }
    }
  }
}

#endif

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
  ESP_WiFiManager->run();
  check_status();

  #if USE_DYNAMIC_PARAMETERS
    displayCredentialsInLoop();
  #endif

  // Animation
  tft.pushImage(0, 88,  135, 65, ani[frame]);
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

  // Data request every period
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
  }
  
  delay(80);
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