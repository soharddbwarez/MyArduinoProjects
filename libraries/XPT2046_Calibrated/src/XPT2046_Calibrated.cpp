/* Touchscreen library for XPT2046 Touch Controller Chip
 * Copyright (c) 2015, Paul Stoffregen, paul@pjrc.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "XPT2046_Calibrated.h"

#define Z_THRESHOLD     400
#define Z_THRESHOLD_INT	75
#define MSEC_THRESHOLD  3
#define SPI_SETTING     SPISettings(2000000, MSBFIRST, SPI_MODE0)

static XPT2046_Calibrated 	*isrPinptr;
void isrPin(void);

bool XPT2046_Calibrated::begin()
{
	SPI.begin();
	pinMode(csPin, OUTPUT);
	digitalWrite(csPin, HIGH);
	if (255 != tirqPin) {
		pinMode( tirqPin, INPUT );
		attachInterrupt(digitalPinToInterrupt(tirqPin), isrPin, FALLING);
		isrPinptr = this;
	}
	return true;
}

ISR_PREFIX
void isrPin( void )
{
	XPT2046_Calibrated *o = isrPinptr;
	o->isrWake = true;
}

TS_Point XPT2046_Calibrated::getPoint()
{
	update();
	return TS_Point(xraw, yraw, zraw);
}

bool XPT2046_Calibrated::tirqTouched()
{
	return (isrWake);
}

bool XPT2046_Calibrated::touched()
{
	update();
	return (zraw >= Z_THRESHOLD);
}

void XPT2046_Calibrated::readData(uint16_t *x, uint16_t *y, uint8_t *z)
{
	update();
	*x = xraw;
	*y = yraw;
	*z = zraw;
}

bool XPT2046_Calibrated::bufferEmpty()
{
	return ((millis() - msraw) < MSEC_THRESHOLD);
}

static int16_t besttwoavg( int16_t x , int16_t y , int16_t z ) {
  int16_t da, db, dc;
  int16_t reta = 0;
  if ( x > y ) da = x - y; else da = y - x;
  if ( x > z ) db = x - z; else db = z - x;
  if ( z > y ) dc = z - y; else dc = y - z;

  if ( da <= db && da <= dc ) reta = (x + y) >> 1;
  else if ( db <= da && db <= dc ) reta = (x + z) >> 1;
  else reta = (y + z) >> 1;   //    else if ( dc <= da && dc <= db ) reta = (x + y) >> 1;

  return (reta);
}

// TODO: perhaps a future version should offer an option for more oversampling,
//       with the RANSAC algorithm https://en.wikipedia.org/wiki/RANSAC

void XPT2046_Calibrated::update()
{
	int16_t data[6];

	if (!isrWake) return;
	uint32_t now = millis();
	if (now - msraw < MSEC_THRESHOLD) return;

	SPI.beginTransaction(SPI_SETTING);
	digitalWrite(csPin, LOW);
	SPI.transfer(0xB1 /* Z1 */);
	int16_t z1 = SPI.transfer16(0xC1 /* Z2 */) >> 3;
	int z = z1 + 4095;
	int16_t z2 = SPI.transfer16(0x91 /* X */) >> 3;
	z -= z2;
	if (z >= Z_THRESHOLD) {
		SPI.transfer16(0x91 /* X */);  // dummy X measure, 1st is always noisy
		data[0] = SPI.transfer16(0xD1 /* Y */) >> 3;
		data[1] = SPI.transfer16(0x91 /* X */) >> 3; // make 3 x-y measurements
		data[2] = SPI.transfer16(0xD1 /* Y */) >> 3;
		data[3] = SPI.transfer16(0x91 /* X */) >> 3;
	}
	else data[0] = data[1] = data[2] = data[3] = 0;	// Compiler warns these values may be used unset on early exit.
	data[4] = SPI.transfer16(0xD0 /* Y */) >> 3;	// Last Y touch power down
	data[5] = SPI.transfer16(0) >> 3;
	digitalWrite(csPin, HIGH);
	SPI.endTransaction();
	//Serial.printf("z=%d  ::  z1=%d,  z2=%d  ", z, z1, z2);
	if (z < 0) z = 0;
	if (z < Z_THRESHOLD) { //	if ( !touched ) {
		// Serial.println();
		zraw = 0;
		if (z < Z_THRESHOLD_INT) { //	if ( !touched ) {
			if (255 != tirqPin) isrWake = false;
		}
		return;
	}
	zraw = z;

	// Average pair with least distance between each measured x then y
	//Serial.printf("    z1=%d,z2=%d  ", z1, z2);
	//Serial.printf("p=%d,  %d,%d  %d,%d  %d,%d", zraw,
		//data[0], data[1], data[2], data[3], data[4], data[5]);
	int16_t x = besttwoavg( data[0], data[2], data[4] );
	int16_t y = besttwoavg( data[1], data[3], data[5] );

	//Serial.printf("    %d,%d", x, y);
	//Serial.println();
	if (z >= Z_THRESHOLD) {
		msraw = now;	// good read completed, set wait
		if (cal.defined) {

			xraw = cal.alphaX * x + cal.betaX * y + cal.deltaX;
			yraw = cal.alphaY * x + cal.betaY * y + cal.deltaY;

			int32_t rot_x;
			int32_t rot_y;

			switch (rotation % 4) {
			case 0:
				rot_x = cal.screenWidth - yraw;
				rot_y = xraw;
				if (rot_x < 0) { rot_x = 0; }
				break;
			case 1:
				rot_x = xraw;
				rot_y = yraw;
				break;
			case 2:
				rot_x = yraw;
				rot_y = cal.screenHeight - xraw;
				if (rot_y < 0) { rot_y = 0; }
				break;
			case 3:
				rot_x = cal.screenWidth - xraw;
				rot_y = cal.screenHeight - yraw;
				if (rot_x < 0) { rot_x = 0; }
				if (rot_y < 0) { rot_y = 0; }
				break;
			}
			xraw = rot_x;
			yraw = rot_y;
		}
		else {
			switch (rotation) {
			case 0:
				xraw = 4095 - y;
				yraw = x;
				break;
			case 1:
				xraw = x;
				yraw = y;
				break;
			case 2:
				xraw = y;
				yraw = 4095 - x;
				break;
			default: // 3
				xraw = 4095 - x;
				yraw = 4095 - y;
			}
		}
	}
}

TS_Calibration::TS_Calibration(
	const TS_Point aS, const TS_Point aT,
	const TS_Point bS, const TS_Point bT,
	const TS_Point cS, const TS_Point cT,
	const uint16_t sW, const uint16_t sH) {

	defined      = true;
	screenWidth  = sW;
	screenHeight = sH;

	int32_t delta =
	    ( (aT.x - cT.x) * (bT.y - cT.y) )
	    -
	    ( (bT.x - cT.x) * (aT.y - cT.y) );

	alphaX =
	    (float)
	      ( ( (aS.x - cS.x) * (bT.y - cT.y) )
	        -
	        ( (bS.x - cS.x) * (aT.y - cT.y) ) )
	    / delta;

	betaX =
	    (float)
	      ( ( (aT.x - cT.x) * (bS.x - cS.x) )
	        -
	        ( (bT.x - cT.x) * (aS.x - cS.x) ) )
	    / delta;

	deltaX =
	    ( ( (uint64_t)aS.x
	          * ( (bT.x * cT.y) - (cT.x * bT.y) ) )
	      -
	      ( (uint64_t)bS.x
	          * ( (aT.x * cT.y) - (cT.x * aT.y) ) )
	      +
	      ( (uint64_t)cS.x
	          * ( (aT.x * bT.y) - (bT.x * aT.y) ) ) )
	    / delta;

	alphaY =
	    (float)
	      ( ( (aS.y - cS.y) * (bT.y - cT.y) )
	        -
	        ( (bS.y - cS.y) * (aT.y - cT.y) ) )
	    / delta;

	betaY =
	    (float)
	      ( ( (aT.x - cT.x) * (bS.y - cS.y) )
	        -
	        ( (bT.x - cT.x) * (aS.y - cS.y) ) )
	    / delta;

	deltaY =
	    ( ( (uint64_t)aS.y
	          * (bT.x * cT.y - cT.x * bT.y) )
	      -
	      ( (uint64_t)bS.y
	          * (aT.x * cT.y - cT.x * aT.y) )
	      +
	      ( (uint64_t)cS.y
	          * (aT.x * bT.y - bT.x * aT.y) ) )
	    / delta;
}
