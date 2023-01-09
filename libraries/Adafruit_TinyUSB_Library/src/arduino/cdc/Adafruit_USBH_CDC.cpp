/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Ha Thach (tinyusb.org) for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "tusb_option.h"

#if CFG_TUH_ENABLED && CFG_TUH_MSC

#include "tusb.h"

#include "Adafruit_USBH_CDC.h"

Adafruit_USBH_CDC::Adafruit_USBH_CDC(void) { _idx = TUSB_INDEX_INVALID; }

void Adafruit_USBH_CDC::begin(uint8_t idx) { _idx = idx; }

void Adafruit_USBH_CDC::end(void) { _idx = TUSB_INDEX_INVALID; }

bool Adafruit_USBH_CDC::connected(void) {
  return (_idx != TUSB_INDEX_INVALID) && tuh_cdc_connected(_idx);
}

bool Adafruit_USBH_CDC::mounted(void) {
  return (_idx != TUSB_INDEX_INVALID) && tuh_cdc_mounted(_idx);
}

int Adafruit_USBH_CDC::available(void) {
  return (int)tuh_cdc_read_available(_idx);
}

int Adafruit_USBH_CDC::peek(void) {
  uint8_t ch;
  return tuh_cdc_peek(_idx, &ch) ? (int)ch : -1;
}

int Adafruit_USBH_CDC::read(void) {
  uint8_t ch;
  return read(&ch, 1) ? (int)ch : -1;
}

size_t Adafruit_USBH_CDC::read(uint8_t *buffer, size_t size) {
  return tuh_cdc_read(_idx, buffer, size);
}

void Adafruit_USBH_CDC::flush(void) { (void)tuh_cdc_write_flush(_idx); }

size_t Adafruit_USBH_CDC::write(uint8_t ch) { return write(&ch, 1); }

size_t Adafruit_USBH_CDC::write(const uint8_t *buffer, size_t size) {
  size_t remain = size;
  while (remain && tuh_cdc_mounted(_idx)) {
    size_t wrcount = tuh_cdc_write(_idx, buffer, remain);
    remain -= wrcount;
    buffer += wrcount;

    // Write FIFO is full, run host task while wait for space become available
    if (remain) {
      tuh_task();
    }
  }

  return size - remain;

  return tuh_cdc_write(_idx, buffer, size);
}

int Adafruit_USBH_CDC::availableForWrite(void) {
  return (int)tuh_cdc_write_available(_idx);
}

#endif
