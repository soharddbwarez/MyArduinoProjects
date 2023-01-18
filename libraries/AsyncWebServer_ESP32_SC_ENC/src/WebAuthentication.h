/****************************************************************************************************************************
  WebAuthentication.h - Dead simple Ethernet AsyncWebServer.

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

#ifndef WEB_AUTHENTICATION_H_
#define WEB_AUTHENTICATION_H_

#include "Arduino.h"

#include "AsyncWebServer_ESP32_SC_ENC_Debug.h"

/////////////////////////////////////////////////

bool checkBasicAuthentication(const char * header, const char * username, const char * password);
String requestDigestAuthentication(const char * realm);

bool checkDigestAuthentication(const char * header, const char * method, const char * username, const char * password,
                               const char * realm,
                               bool passwordIsHash, const char * nonce, const char * opaque, const char * uri);

//for storing hashed versions on the device that can be authenticated against
String generateDigestHash(const char * username, const char * password, const char * realm);

/////////////////////////////////////////////////

#endif    // WEB_AUTHENTICATION_H_
