/****************************************************************************************************************************
  ESP32_ENC_SPIFFSEditor.h - Dead simple Ethernet AsyncWebServer.

  For LwIP ENC28J60 Ethernet in ESP32_SC_ENC (ESP32_S2/S3/C3 + LwIP ENC28J60)

  AsyncWebServer_ESP32_SC_ENC is a library for the LwIP Ethernet ENC28J60 in ESP32_S2/S3/C3 to run AsyncWebServer

  Based on and modified from ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
  Built by Khoi Hoang https://github.com/khoih-prog/AsyncWebServer_ESP32_SC_ENC
  Licensed under GPLv3 license

  Original author: Hristo Gochkov

  Copyright (c) 2016 Hristo Gochkov. All rights reserved.

  This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along with this library;
  if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Version: 1.8.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.6.3   K Hoang      15/12/2022 Initial porting for ENC28J60 + ESP32_S3. Sync with AsyncWebServer_ESP32_ENC v1.6.3
  1.7.0   K Hoang      19/12/2022 Add support to ESP32_S2_ENC (ESP32_S2 + LwIP ENC28J60)
  1.8.0   K Hoang      20/12/2022 Add support to ESP32_C3_ENC (ESP32_C3 + LwIP ENC28J60)
 *****************************************************************************************************************************/

#ifndef ESP32_ENC_SPIFFSEditor_H_
#define ESP32_ENC_SPIFFSEditor_H_

#include "AsyncWebServer_ESP32_SC_ENC.h"

/////////////////////////////////////////////////

class SPIFFSEditor: public AsyncWebHandler
{
  private:
    fs::FS _fs;
    String _username;
    String _password;
    bool _authenticated;
    uint32_t _startTime;

  public:
    SPIFFSEditor(const fs::FS& fs, const String& username = String(), const String& password = String());
    virtual bool canHandle(AsyncWebServerRequest *request) override final;
    virtual void handleRequest(AsyncWebServerRequest *request) override final;
    virtual void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data,
                              size_t len, bool final) override final;

    virtual bool isRequestHandlerTrivial() override final
    {
      return false;
    }
};

/////////////////////////////////////////////////

#endif    // SPIFFSEditor_H_
