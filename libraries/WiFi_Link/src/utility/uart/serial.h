/*
  serial.h - Library for Arduino Wifi shield.
  Copyright (c) 2011-2014 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
//*********************************************/
//
//  File:   serial.h
//
//  Author: bing@arduino.org (arduino srl)
//  edit: andrea@arduino.org (arduino srl)
//
//********************************************/

#if defined(ESP_CH_UART)

#ifndef SERIAL_H
#define SERIAL_H

#include "Arduino.h"
#if defined(__AVR_ATmega328P__)
#include "SC16IS750.h"    //Arduino Uno WiFi developer edition
#endif

#define Serial SerialWiFi

class WfSerial {
private:
    int timedRead();

public:
    void begin();
    int read();
    void write(unsigned char c);
    int available();
    String readStringUntil(char terminator);
};

extern WfSerial wfSerial;

#endif
#endif
