#include <Adafruit_NeoPixel.h>

class Strip
{
public:
  uint8_t   effect;
  uint8_t   effects;
  uint16_t  effStep;
  unsigned long effStart;
  Adafruit_NeoPixel strip;
  Strip(uint16_t leds, uint8_t pin, uint8_t toteffects, uint16_t striptype) : strip(leds, pin, striptype) {
    effect = -1;
    effects = toteffects;
    Reset();
  }
  void Reset(){
    effStep = 0;
    effect = (effect + 1) % effects;
    effStart = millis();
  }
};

struct Loop
{
  uint8_t currentChild;
  uint8_t childs;
  bool timeBased;
  uint16_t cycles;
  uint16_t currentTime;
  Loop(uint8_t totchilds, bool timebased, uint16_t tottime) {currentTime=0;currentChild=0;childs=totchilds;timeBased=timebased;cycles=tottime;}
};

Strip strip_0(17, 6, 17, NEO_GRB + NEO_KHZ800);
struct Loop strip0loop0(3, true, 10);
struct Loop strip0loop00(1, true, 10);
struct Loop strip0loop01(4, false, 10);
struct Loop strip0loop010(1, false, 10);

//[GLOBAL_VARIABLES]

void setup() {

  //Your setup here:

  strip_0.strip.begin();
}

void loop() {

  //Your code here:

  strips_loop();
}

void strips_loop() {
  if(strip0_loop0() & 0x01)
    strip_0.strip.show();
}

uint8_t strip0_loop0() {
  uint8_t ret = 0x00;
  switch(strip0loop0.currentChild) {
    case 0: 
           ret = strip0_loop0_eff0();break;
    case 1: 
           ret = strip0_loop00();break;
    case 2: 
           ret = strip0_loop01();break;
  }
  if(ret & 0x02) {
    ret &= 0xfd;
    if(strip0loop0.currentChild + 1 >= strip0loop0.childs) {
      strip0loop0.currentChild = 0;
      if(++strip0loop0.currentTime >= strip0loop0.cycles) {strip0loop0.currentTime = 0; ret |= 0x02;}
    }
    else {
      strip0loop0.currentChild++;
    }
  };
  return ret;
}

uint8_t strip0_loop0_eff0() {
    // Strip ID: 0 - Effect: Rainbow - LEDS: 17
    // Steps: 84 - Delay: 25
    // Colors: 3 (255.0.0, 0.255.0, 0.0.255)
    // Options: rainbowlen=17, toLeft=false, 
  if(millis() - strip_0.effStart < 25 * (strip_0.effStep)) return 0x00;
  float factor1, factor2;
  uint16_t ind;
  for(uint16_t j=0;j<17;j++) {
    ind = 84 - (uint16_t)(strip_0.effStep - j * 4.9411764705882355) % 84;
    switch((int)((ind % 84) / 28)) {
      case 0: factor1 = 1.0 - ((float)(ind % 84 - 0 * 28) / 28);
              factor2 = (float)((int)(ind - 0) % 84) / 28;
              strip_0.strip.setPixelColor(j, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2);
              break;
      case 1: factor1 = 1.0 - ((float)(ind % 84 - 1 * 28) / 28);
              factor2 = (float)((int)(ind - 28) % 84) / 28;
              strip_0.strip.setPixelColor(j, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2);
              break;
      case 2: factor1 = 1.0 - ((float)(ind % 84 - 2 * 28) / 28);
              factor2 = (float)((int)(ind - 56) % 84) / 28;
              strip_0.strip.setPixelColor(j, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2);
              break;
    }
  }
  if(strip_0.effStep >= 84) {strip_0.Reset(); return 0x03; }
  else strip_0.effStep++;
  return 0x01;
}

uint8_t strip0_loop00() {
  uint8_t ret = 0x00;
  switch(strip0loop00.currentChild) {
    case 0: 
           ret = strip0_loop00_eff0();break;
  }
  if(ret & 0x02) {
    ret &= 0xfd;
    if(strip0loop00.currentChild + 1 >= strip0loop00.childs) {
      strip0loop00.currentChild = 0;
      if(++strip0loop00.currentTime >= strip0loop00.cycles) {strip0loop00.currentTime = 0; ret |= 0x02;}
    }
    else {
      strip0loop00.currentChild++;
    }
  };
  return ret;
}

uint8_t strip0_loop00_eff0() {
    // Strip ID: 0 - Effect: Move - LEDS: 17
    // Steps: 1 - Delay: 84
    // Colors: 0 ()
    // Options: toLeft=true, rotate=true, 
  if(millis() - strip_0.effStart < 84 * (strip_0.effStep)) return 0x00;
  uint32_t c = strip_0.strip.getPixelColor(0);
  for(uint16_t j=0;j<16;j++) 
    strip_0.strip.setPixelColor(j, strip_0.strip.getPixelColor(j+1)); 
  strip_0.strip.setPixelColor(16, c); 
  if(strip_0.effStep >= 1) {strip_0.Reset(); return 0x03; }
  else strip_0.effStep++;
  return 0x01;
}

uint8_t strip0_loop01() {
  uint8_t ret = 0x00;
  switch(strip0loop01.currentChild) {
    case 0: 
           ret = strip0_loop01_eff0();break;
    case 1: 
           ret = strip0_loop01_eff1();break;
    case 2: 
           ret = strip0_loop01_eff2();break;
    case 3: 
           ret = strip0_loop010();break;
  }
  if(ret & 0x02) {
    ret &= 0xfd;
    if(strip0loop01.currentChild + 1 >= strip0loop01.childs) {
      strip0loop01.currentChild = 0;
      if(++strip0loop01.currentTime >= strip0loop01.cycles) {strip0loop01.currentTime = 0; ret |= 0x02;}
    }
    else {
      strip0loop01.currentChild++;
    }
  };
  return ret;
}

uint8_t strip0_loop01_eff0() {
    // Strip ID: 0 - Effect: Fade - LEDS: 17
    // Steps: 99.6 - Delay: 10
    // Colors: 2 (255.0.0, 0.255.0)
    // Options: duration=996, every=1, 
  if(millis() - strip_0.effStart < 10 * (strip_0.effStep)) return 0x00;
  uint8_t r,g,b;
  double e;
  e = (strip_0.effStep * 10) / 996;
  r = ( e ) * 0 + 255 * ( 1.0 - e );
  g = ( e ) * 255 + 0 * ( 1.0 - e );
  b = ( e ) * 0 + 0 * ( 1.0 - e );
  for(uint16_t j=0;j<17;j++) {
    if((j % 1) == 0)
      strip_0.strip.setPixelColor(j, r, g, b);
    else
      strip_0.strip.setPixelColor(j, 0, 0, 0);
  }
  if(strip_0.effStep >= 99.6) {strip_0.Reset(); return 0x03; }
  else strip_0.effStep++;
  return 0x01;
}

uint8_t strip0_loop01_eff1() {
    // Strip ID: 0 - Effect: Fade - LEDS: 17
    // Steps: 99.6 - Delay: 10
    // Colors: 2 (0.255.0, 0.0.255)
    // Options: duration=996, every=1, 
  if(millis() - strip_0.effStart < 10 * (strip_0.effStep)) return 0x00;
  uint8_t r,g,b;
  double e;
  e = (strip_0.effStep * 10) / 996;
  r = ( e ) * 0 + 0 * ( 1.0 - e );
  g = ( e ) * 0 + 255 * ( 1.0 - e );
  b = ( e ) * 255 + 0 * ( 1.0 - e );
  for(uint16_t j=0;j<17;j++) {
    if((j % 1) == 0)
      strip_0.strip.setPixelColor(j, r, g, b);
    else
      strip_0.strip.setPixelColor(j, 0, 0, 0);
  }
  if(strip_0.effStep >= 99.6) {strip_0.Reset(); return 0x03; }
  else strip_0.effStep++;
  return 0x01;
}

uint8_t strip0_loop01_eff2() {
    // Strip ID: 0 - Effect: Fade - LEDS: 17
    // Steps: 99.3 - Delay: 10
    // Colors: 2 (0.0.243, 255.0.0)
    // Options: duration=993, every=1, 
  if(millis() - strip_0.effStart < 10 * (strip_0.effStep)) return 0x00;
  uint8_t r,g,b;
  double e;
  e = (strip_0.effStep * 10) / 993;
  r = ( e ) * 255 + 0 * ( 1.0 - e );
  g = ( e ) * 0 + 0 * ( 1.0 - e );
  b = ( e ) * 0 + 243 * ( 1.0 - e );
  for(uint16_t j=0;j<17;j++) {
    if((j % 1) == 0)
      strip_0.strip.setPixelColor(j, r, g, b);
    else
      strip_0.strip.setPixelColor(j, 0, 0, 0);
  }
  if(strip_0.effStep >= 99.3) {strip_0.Reset(); return 0x03; }
  else strip_0.effStep++;
  return 0x01;
}

uint8_t strip0_loop010() {
  uint8_t ret = 0x00;
  switch(strip0loop010.currentChild) {
    case 0: 
           ret = strip0_loop010_eff0();break;
  }
  if(ret & 0x02) {
    ret &= 0xfd;
    if(strip0loop010.currentChild + 1 >= strip0loop010.childs) {
      strip0loop010.currentChild = 0;
      if(++strip0loop010.currentTime >= strip0loop010.cycles) {strip0loop010.currentTime = 0; ret |= 0x02;}
    }
    else {
      strip0loop010.currentChild++;
    }
  };
  return ret;
}

uint8_t strip0_loop010_eff0() {
    // Strip ID: 0 - Effect: Blink - LEDS: 17
    // Steps: 250 - Delay: 1
    // Colors: 2 (0.0.0, 255.255.255)
    // Options: timeBegin=100, timeToOn=20, timeOn=10, timeToOff=20, timeOver=100, every=3, 
  if(millis() - strip_0.effStart < 1 * (strip_0.effStep)) return 0x00;
  uint8_t e,r,g,b;
  if(strip_0.effStep < 100) {
    for(uint16_t j=0;j<17;j++) 
      strip_0.strip.setPixelColor(j, 0, 0, 0);
  }
  else if(strip_0.effStep  < 120) {
    e = (strip_0.effStep * 1) - 100;
    r = 255 * ( e / 20 ) + 0 * ( 1.0 - e / 20 );
    g = 255 * ( e / 20 ) + 0 * ( 1.0 - e / 20 );
    b = 255 * ( e / 20 ) + 0 * ( 1.0 - e / 20 );
    for(uint16_t j=0;j<17;j++) 
      if((j%3)==0) strip_0.strip.setPixelColor(j, r, g, b);
      else strip_0.strip.setPixelColor(j, 0, 0, 0);
  }
  else if(strip_0.effStep < 130) {
    for(uint16_t j=0;j<17;j++) 
      if((j%3)==0) strip_0.strip.setPixelColor(j, 255, 255, 255);
      else strip_0.strip.setPixelColor(j, 0, 0, 0);
  }
  else if(strip_0.effStep < 150) {
    e = (strip_0.effStep * 1) - 130;
    r = 0 * ( e / 20 ) + 255 * ( 1.0 - e / 20 );
    g = 0 * ( e / 20 ) + 255 * ( 1.0 - e / 20 );
    b = 0 * ( e / 20 ) + 255 * ( 1.0 - e / 20 );
    for(uint16_t j=0;j<17;j++) 
      if((j%3)==0) strip_0.strip.setPixelColor(j, r, g, b);
      else strip_0.strip.setPixelColor(j, 0, 0, 0);
  }
  else {
    for(uint16_t j=0;j<17;j++) 
      strip_0.strip.setPixelColor(j, 0, 0, 0);
  }
  if(strip_0.effStep >= 250) {strip_0.Reset(); return 0x03; }
  else strip_0.effStep++;
  return 0x01;
}
