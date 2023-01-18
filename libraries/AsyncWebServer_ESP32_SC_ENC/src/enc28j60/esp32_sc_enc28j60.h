/****************************************************************************************************************************
  esp32_sc_enc28j60.h

  For LwIP ENC28J60 Ethernet in ESP32_SC_ENC (ESP32_S2/S3/C3 + LwIP ENC28J60)

  AsyncWebServer_ESP32_SC_ENC is a library for the LwIP Ethernet ENC28J60 in ESP32_S2/S3/C3 to run AsyncWebServer

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_ENC
  Licensed under GPLv3 license

  Version: 1.8.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.6.3   K Hoang      15/12/2022 Initial porting for ENC28J60 + ESP32_S3. Sync with AsyncWebServer_ESP32_ENC v1.6.3
  1.7.0   K Hoang      19/12/2022 Add support to ESP32_S2_ENC (ESP32_S2 + LwIP ENC28J60)
  1.8.0   K Hoang      20/12/2022 Add support to ESP32_C3_ENC (ESP32_C3 + LwIP ENC28J60)
 *****************************************************************************************************************************/
/*
  esp32_enc28j60.h - ETH PHY support for ENC28J60
  Based on ETH.h from arduino-esp32 and esp-idf
  and Tobozo ESP32-ENC28J60 library
*/

#ifndef _ESP32_SC_ENC_H_
#define _ESP32_SC_ENC_H_

#include "WiFi.h"
#include "esp_system.h"
#include "esp_eth.h"

////////////////////////////////////////

#if ESP_IDF_VERSION_MAJOR < 4 || ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(4,4,0)
  #error "This version of Arduino is too old"
#endif

////////////////////////////////////////

static uint8_t ENC28J60_Default_Mac[] = { 0xFE, 0xED, 0xDE, 0xAD, 0xBE, 0xEF };

////////////////////////////////////////

class ESP32_ENC
{
  private:
    bool initialized;
    bool staticIP;

    //static uint8_t ENC28J60_Default_Mac[6] = { 0x02, 0x00, 0xDE, 0xAD, 0xBE, 0xEF };
    uint8_t mac_eth[6] = { 0xFE, 0xED, 0xDE, 0xAD, 0xBE, 0xEF };

#if ESP_IDF_VERSION_MAJOR > 3
    esp_eth_handle_t eth_handle;

  protected:
    bool started;
    eth_link_t eth_link;
    static void eth_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
#else
    bool started;
    eth_config_t eth_config;
#endif

  public:
    ESP32_ENC();
    ~ESP32_ENC();

    bool begin(int MISO, int MOSI, int SCLK, int CS, int INT, int SPICLOCK_MHZ, int SPIHOST,
               uint8_t *ENC28J60_Mac = ENC28J60_Default_Mac);

    bool config(IPAddress local_ip, IPAddress gateway, IPAddress subnet, IPAddress dns1 = (uint32_t)0x00000000,
                IPAddress dns2 = (uint32_t)0x00000000);

    const char * getHostname();
    bool setHostname(const char * hostname);

    bool fullDuplex();
    bool linkUp();
    uint8_t linkSpeed();

    bool enableIpV6();
    IPv6Address localIPv6();

    IPAddress localIP();
    IPAddress subnetMask();
    IPAddress gatewayIP();
    IPAddress dnsIP(uint8_t dns_no = 0);

    IPAddress broadcastIP();
    IPAddress networkID();
    uint8_t subnetCIDR();

    uint8_t * macAddress(uint8_t* mac);
    String macAddress();

    friend class WiFiClient;
    friend class WiFiServer;
};

////////////////////////////////////////

extern ESP32_ENC ETH;

////////////////////////////////////////

#endif /* _ESP32_SC_ENC_H_ */
