#include "FastLED.h"

// ColorWavesWithPalettes
// Animated shifting color waves, with several cross-fading color palettes.
// by Mark Kriegsman, August 2015
//
// Color palettes courtesy of cpt-city and its contributors:
//   http://soliton.vm.bytemark.co.uk/pub/cpt-city/
//
// Color palettes converted for FastLED using "PaletteKnife" v1:
//   http://fastled.io/tools/paletteknife/
//

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN     2
#define LED_TYPE     WS2812B
#define COLOR_ORDER  GRB  // RBG for the LED string and GRB for the SMD5050 strip
#define NUM_LEDS     7
#define BRIGHTNESS   200

CRGB leds[NUM_LEDS];

// ten seconds per color palette makes a good demo
// 20-120 is better for deployment
#define SECONDS_PER_PALETTE 120


void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalSMD5050) // cpt-city palettes have different color balance. Comment out or change accordingly (TypicalLEDStrip) or (TypicalSMD5050)
    .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

// Forward declarations of an array of cpt-city gradient palettes, and 
// a count of how many there are.  The actual color palette definitions
// are at the bottom of this file.
extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
extern const uint8_t gGradientPaletteCount;

// Current palette number from the 'playlist' of color palettes
uint8_t gCurrentPaletteNumber = 0;

CRGBPalette16 gCurrentPalette( CRGB::Black);
CRGBPalette16 gTargetPalette( gGradientPalettes[0] );


void loop()
{
  EVERY_N_SECONDS( SECONDS_PER_PALETTE ) {
    gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, gGradientPaletteCount);
    gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];
  }

  EVERY_N_MILLISECONDS(40) {
    nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 16);
  }
  
  colorwaves( leds, NUM_LEDS, gCurrentPalette);

  FastLED.show();
  FastLED.delay(20);
}


// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
void colorwaves( CRGB* ledarray, uint16_t numleds, CRGBPalette16& palette) 
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 300, 1500);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < numleds; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    uint8_t index = hue8;
    index = triwave8( index);
    index = scale8( index, 240);

    CRGB newcolor = ColorFromPalette( palette, index, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (numleds-1) - pixelnumber;
    
    nblend( ledarray[pixelnumber], newcolor, 128);
  }
}

// Alternate rendering function just scrolls the current palette 
// across the defined LED strip.
void palettetest( CRGB* ledarray, uint16_t numleds, const CRGBPalette16& gCurrentPalette)
{
  static uint8_t startindex = 0;
  startindex--;
  fill_palette( ledarray, numleds, startindex, (256 / NUM_LEDS) + 1, gCurrentPalette, 255, LINEARBLEND);
}


DEFINE_GRADIENT_PALETTE( ib_jul01_gp ) {
    0, 194,  1,  1,
   94,   1, 29, 18,
  132,  57,131, 28,
  255, 113,  1,  1};

DEFINE_GRADIENT_PALETTE( es_vintage_57_gp ) {
    0,   2,  1,  1,
   53,  18,  1,  0,
  104,  69, 29,  1,
  153, 167,135, 10,
  255,  46, 56,  4};

DEFINE_GRADIENT_PALETTE( es_vintage_01_gp ) {
    0,   4,  1,  1,
   51,  16,  0,  1,
   76,  97,104,  3,
  101, 255,131, 19,
  127,  67,  9,  4,
  153,  16,  0,  1,
  229,   4,  1,  1,
  255,   4,  1,  1};

DEFINE_GRADIENT_PALETTE( es_rivendell_15_gp ) {
    0,   1, 14,  5,
  101,  16, 36, 14,
  165,  56, 68, 30,
  242, 150,156, 99,
  255, 150,156, 99};

DEFINE_GRADIENT_PALETTE( rgi_15_gp ) {
    0,   4,  1, 31,
   31,  55,  1, 16,
   63, 197,  3,  7,
   95,  59,  2, 17,
  127,   6,  2, 34,
  159,  39,  6, 33,
  191, 112, 13, 32,
  223,  56,  9, 35,
  255,  22,  6, 38};

DEFINE_GRADIENT_PALETTE( retro2_16_gp ) {
    0, 188,135,  1,
  255,  46,  7,  1};

DEFINE_GRADIENT_PALETTE( Analogous_1_gp ) {
    0,   3,  0,255,
   63,  23,  0,255,
  127,  67,  0,255,
  191, 142,  0, 45,
  255, 255,  0,  0};

DEFINE_GRADIENT_PALETTE( es_pinksplash_08_gp ) {
    0, 126, 11,255,
  127, 197,  1, 22,
  175, 210,157,172,
  221, 157,  3,112,
  255, 157,  3,112};

DEFINE_GRADIENT_PALETTE( es_pinksplash_07_gp ) {
    0, 229,  1,  1,
   61, 242,  4, 63,
  101, 255, 12,255,
  127, 249, 81,252,
  153, 255, 11,235,
  193, 244,  5, 68,
  255, 232,  1,  5};

DEFINE_GRADIENT_PALETTE( Coral_reef_gp ) {
    0,  40,199,197,
   50,  10,152,155,
   96,   1,111,120,
   96,  43,127,162,
  139,  10, 73,111,
  255,   1, 34, 71};

DEFINE_GRADIENT_PALETTE( es_ocean_breeze_068_gp ) {
    0, 100,156,153,
   51,   1, 99,137,
  101,   1, 68, 84,
  104,  35,142,168,
  178,   0, 63,117,
  255,   1, 10, 10};

DEFINE_GRADIENT_PALETTE( es_ocean_breeze_036_gp ) {
    0,   1,  6,  7,
   89,   1, 99,111,
  153, 144,209,255,
  255,   0, 73, 82};

DEFINE_GRADIENT_PALETTE( departure_gp ) {
    0,   8,  3,  0,
   42,  23,  7,  0,
   63,  75, 38,  6,
   84, 169, 99, 38,
  106, 213,169,119,
  116, 255,255,255,
  138, 135,255,138,
  148,  22,255, 24,
  170,   0,255,  0,
  191,   0,136,  0,
  212,   0, 55,  0,
  255,   0, 55,  0};

DEFINE_GRADIENT_PALETTE( es_landscape_64_gp ) {
    0,   0,  0,  0,
   37,   2, 25,  1,
   76,  15,115,  5,
  127,  79,213,  1,
  128, 126,211, 47,
  130, 188,209,247,
  153, 144,182,205,
  204,  59,117,250,
  255,   1, 37,192};

DEFINE_GRADIENT_PALETTE( es_landscape_33_gp ) {
    0,   1,  5,  0,
   19,  32, 23,  1,
   38, 161, 55,  1,
   63, 229,144,  1,
   66,  39,142, 74,
  255,   1,  4,  1};

DEFINE_GRADIENT_PALETTE( rainbowsherbet_gp ) {
    0, 255, 33,  4,
   43, 255, 68, 25,
   86, 255,  7, 25,
  127, 255, 82,103,
  170, 255,255,242,
  209,  42,255, 22,
  255,  87,255, 65};

DEFINE_GRADIENT_PALETTE( gr65_hult_gp ) {
    0, 247,176,247,
   48, 255,136,255,
   89, 220, 29,226,
  160,   7, 82,178,
  216,   1,124,109,
  255,   1,124,109};

DEFINE_GRADIENT_PALETTE( gr64_hult_gp ) {
    0,   1,124,109,
   66,   1, 93, 79,
  104,  52, 65,  1,
  130, 115,127,  1,
  150,  52, 65,  1,
  201,   1, 86, 72,
  239,   0, 55, 45,
  255,   0, 55, 45};

DEFINE_GRADIENT_PALETTE( GMT_drywet_gp ) {
    0,  47, 30,  2,
   42, 213,147, 24,
   84, 103,219, 52,
  127,   3,219,207,
  170,   1, 48,214,
  212,   1,  1,111,
  255,   1,  7, 33};

DEFINE_GRADIENT_PALETTE( ib15_gp ) {
    0, 113, 91,147,
   72, 157, 88, 78,
   89, 208, 85, 33,
  107, 255, 29, 11,
  141, 137, 31, 39,
  255,  59, 33, 89};

DEFINE_GRADIENT_PALETTE( Fuschia_7_gp ) {
    0,  43,  3,153,
   63, 100,  4,103,
  127, 188,  5, 66,
  191, 161, 11,115,
  255, 135, 20,182};

DEFINE_GRADIENT_PALETTE( es_emerald_dragon_08_gp ) {
    0,  97,255,  1,
  101,  47,133,  1,
  178,  13, 43,  1,
  255,   2, 10,  1};

DEFINE_GRADIENT_PALETTE( lava_gp ) {
    0,   0,  0,  0,
   46,  18,  0,  0,
   96, 113,  0,  0,
  108, 142,  3,  1,
  119, 175, 17,  1,
  146, 213, 44,  2,
  174, 255, 82,  4,
  188, 255,115,  4,
  202, 255,156,  4,
  218, 255,203,  4,
  234, 255,255,  4,
  244, 255,255, 71,
  255, 255,255,255};

DEFINE_GRADIENT_PALETTE( fire_gp ) {
    0,   1,  1,  0,
   76,  32,  5,  0,
  146, 192, 24,  0,
  197, 220,105,  5,
  240, 252,255, 31,
  250, 252,255,111,
  255, 255,255,255};

DEFINE_GRADIENT_PALETTE( Colorfull_gp ) {
    0,  10, 85,  5,
   25,  29,109, 18,
   60,  59,138, 42,
   93,  83, 99, 52,
  106, 110, 66, 64,
  109, 123, 49, 65,
  113, 139, 35, 66,
  116, 192,117, 98,
  124, 255,255,137,
  168, 100,180,155,
  255,  22,121,174};

DEFINE_GRADIENT_PALETTE( Magenta_Evening_gp ) {
    0,  71, 27, 39,
   31, 130, 11, 51,
   63, 213,  2, 64,
   70, 232,  1, 66,
   76, 252,  1, 69,
  108, 123,  2, 51,
  255,  46,  9, 35};

DEFINE_GRADIENT_PALETTE( Pink_Purple_gp ) {
    0,  19,  2, 39,
   25,  26,  4, 45,
   51,  33,  6, 52,
   76,  68, 62,125,
  102, 118,187,240,
  109, 163,215,247,
  114, 217,244,255,
  122, 159,149,221,
  149, 113, 78,188,
  183, 128, 57,155,
  255, 146, 40,123};

DEFINE_GRADIENT_PALETTE( Sunset_Real_gp ) {
    0, 120,  0,  0,
   22, 179, 22,  0,
   51, 255,104,  0,
   85, 167, 22, 18,
  135, 100,  0,103,
  198,  16,  0,130,
  255,   0,  0,160};

DEFINE_GRADIENT_PALETTE( es_autumn_19_gp ) {
    0,  26,  1,  1,
   51,  67,  4,  1,
   84, 118, 14,  1,
  104, 137,152, 52,
  112, 113, 65,  1,
  122, 133,149, 59,
  124, 137,152, 52,
  135, 113, 65,  1,
  142, 139,154, 46,
  163, 113, 13,  1,
  204,  55,  3,  1,
  249,  17,  1,  1,
  255,  17,  1,  1};

DEFINE_GRADIENT_PALETTE( BlacK_Blue_Magenta_White_gp ) {
    0,   0,  0,  0,
   42,   0,  0, 45,
   84,   0,  0,255,
  127,  42,  0,255,
  170, 255,  0,255,
  212, 255, 55,255,
  255, 255,255,255};

DEFINE_GRADIENT_PALETTE( BlacK_Magenta_Red_gp ) {
    0,   0,  0,  0,
   63,  42,  0, 45,
  127, 255,  0,255,
  191, 255,  0, 45,
  255, 255,  0,  0};

DEFINE_GRADIENT_PALETTE( BlacK_Red_Magenta_Yellow_gp ) {
    0,   0,  0,  0,
   42,  42,  0,  0,
   84, 255,  0,  0,
  127, 255,  0, 45,
  170, 255,  0,255,
  212, 255, 55, 45,
  255, 255,255,  0};

DEFINE_GRADIENT_PALETTE( Blue_Cyan_Yellow_gp ) {
    0,   0,  0,255,
   63,   0, 55,255,
  127,   0,255,255,
  191,  42,255, 45,
  255, 255,255,  0};

DEFINE_GRADIENT_PALETTE( purplefly_gp ) {
    0,   0,  0,  0,
   63, 239,  0,122,
  191, 252,255, 78,
  255,   0,  0,  0};

DEFINE_GRADIENT_PALETTE( quagga_gp ) {
    0,   1,  9, 84,
   40,  42, 24, 72,
   84,   6, 58,  2,
  168,  88,169, 24,
  211,  42, 24, 72,
  255,   1,  9, 84};

DEFINE_GRADIENT_PALETTE( rainbow_gp ) {
    0, 126,  1,142,
   25, 171,  1, 26,
   48, 224,  9,  1,
   71, 237,138,  1,
   94,  52,173,  1,
  117,   1,201,  1,
  140,   1,211, 54,
  163,   1,124,168,
  186,   1,  8,149,
  209,  12,  1,151,
  232,  12,  1,151,
  255, 171,  1,190};

DEFINE_GRADIENT_PALETTE( scarletdragon_gp ) {
    0,   1,  1,  1,
   51,  16,  2,  1,
   91,  58,  5,  1,
  122, 100, 16,  2,
  150, 120, 33,  9,
  178, 132, 43, 13,
  216, 157, 79, 28,
  255, 227, 92, 29};

DEFINE_GRADIENT_PALETTE( scoutie_gp ) {
    0, 255,156,  0,
  127,   0,195, 18,
  216,   1,  0, 39,
  255,   1,  0, 39};

DEFINE_GRADIENT_PALETTE( shyviolet_gp ) {
    0,  10,  9, 32,
   38,  41, 36, 79,
   76, 103,107,188,
  127, 142,154,194,
  178,  58, 92,176,
  216,  19, 51,158,
  255,   5, 24,103};

DEFINE_GRADIENT_PALETTE( spellbound_gp ) {
    0, 232,235, 40,
   12, 157,248, 46,
   25, 100,246, 51,
   45,  53,250, 33,
   63,  18,237, 53,
   81,  11,211,162,
   94,  18,147,214,
  101,  43,124,237,
  112,  49, 75,247,
  127,  49, 75,247,
  140,  92,107,247,
  150, 120,127,250,
  163, 130,138,252,
  173, 144,131,252,
  186, 148,112,252,
  196, 144, 37,176,
  211, 113, 18, 87,
  221, 163, 33, 53,
  234, 255,101, 78,
  247, 229,235, 46,
  255, 229,235, 46};

DEFINE_GRADIENT_PALETTE( springbird_gp ) {
    0,  10, 45,147,
   38,  10, 45,147,
  216, 192,121,162,
  255, 192,121,162};

DEFINE_GRADIENT_PALETTE( sunlitwave_gp ) {
    0,   5,  9, 84,
   45,  37, 24,111,
   81,  16,  5, 59,
  112,  24,  1, 20,
  150,  34,  1,  2,
  198, 175, 36,  7,
  237, 208,104, 16,
  255, 239,211,158};

DEFINE_GRADIENT_PALETTE( tashangel_gp ) {
    0, 133, 68,197,
   51,   2,  1, 33,
  101,  50, 35,130,
  153, 199,225,237,
  204,  41,187,228,
  255, 133, 68,197};

DEFINE_GRADIENT_PALETTE( teabearrose_gp ) {
    0, 107,  1,  5,
   25, 165, 25, 45,
   38, 184, 82, 88,
   63, 229,133,130,
   89, 229,133,130,
  109, 186, 40,  4,
  117, 215,139, 96,
  122, 148,  8,  1,
  127, 215,139, 96,
  132, 148,  8,  1,
  137, 215,139, 96,
  145, 186, 40,  4,
  165, 229,133,130,
  191, 229,133,130,
  216, 184, 82, 88,
  229, 165, 25, 45,
  255, 107,  1,  5};

DEFINE_GRADIENT_PALETTE( titannightfall_gp ) {
    0, 173, 72,109,
   28, 112, 33, 68,
   51,  72, 19, 67,
  127,   2,  1, 33,
  204,  72, 19, 67,
  229, 112, 33, 68,
  255, 173, 72,109};

DEFINE_GRADIENT_PALETTE( trove_gp ) {
    0,  12, 23, 11,
   12,   8, 52, 27,
   25,  32,142, 64,
   38,  55, 68, 30,
   51, 190,135, 45,
   63, 201,175, 59,
   76, 186, 80, 20,
   89, 220, 79, 32,
  101, 184, 33, 14,
  114, 137, 16, 15,
  127, 118, 20, 27,
  140,  79, 16, 35,
  153,  67,  8, 26,
  165,  22,  9, 42,
  178,  11,  3, 34,
  191,  58, 31,109,
  204, 186, 49, 83,
  216, 182, 25, 55,
  229,  39, 90,100,
  242,  15, 81,132,
  255,  68,135, 52};
  
DEFINE_GRADIENT_PALETTE( tubepreview_gp ) {
    0, 113, 99, 23,
   51, 255,255,255,
   76, 255,255,255,
  114,   0,  0,  0,
  140,   0,  0,  0,
  178, 255,255,255,
  204, 255,255,255,
  255, 113, 99, 23};

DEFINE_GRADIENT_PALETTE( angelcompassion_gp ) {
    0, 121,108, 63,
   81,  48, 36, 42,
  170,   1,  4, 17,
  255,  19, 31, 50};

DEFINE_GRADIENT_PALETTE( aquamarinemermaid_gp ) {
    0,  23,  4, 32,
   63,  98, 31, 52,
  127, 224,138, 24,
  191,   7, 55,164,
  255,  23,  4, 32};

DEFINE_GRADIENT_PALETTE( autumnrose_gp ) {
    0,  71,  3,  1,
   45, 128,  5,  2,
   84, 186, 11,  3,
  127, 215, 27,  8,
  153, 224, 69, 13,
  188, 229, 84,  6,
  226, 242,135, 17,
  255, 247,161, 79};

DEFINE_GRADIENT_PALETTE( blackhorse_gp ) {
    0,   8, 30,  1,
   53,  29, 99,237,
  112,   1,  1, 14,
  168,  21,  5, 21,
  219,   1,  5,  2,
  255,   8, 30,  1};

DEFINE_GRADIENT_PALETTE( bluefly_gp ) {
    0,   0,  0,  0,
   63,   0, 39, 64,
  191, 175,215,235,
  255,   0,  0,  0};

DEFINE_GRADIENT_PALETTE( butterflyfairy_gp ) {
    0,  84, 18,  2,
   63, 163,100, 27,
  127,  46,154,149,
  191,   1, 24, 54,
  255,   1,  1,  1};

DEFINE_GRADIENT_PALETTE( butterflytalker_gp ) {
    0,   1,  1,  6,
   51,   6, 11, 52,
   89, 107,107,192,
  127, 101,161,192,
  165, 107,107,192,
  204,   6, 11, 52,
  255,   0,  0,  0};

DEFINE_GRADIENT_PALETTE( carousel_gp ) {
    0,   2,  6, 37,
  101,   2,  6, 37,
  122, 177,121,  9,
  127, 217,149,  2,
  132, 177,121,  9,
  153,  84, 13, 36,
  255,  84, 13, 36};

DEFINE_GRADIENT_PALETTE( cloud_gp ) {
    0, 247,149, 91,
  127, 208, 32, 71,
  255,  42, 79,188};

DEFINE_GRADIENT_PALETTE( daisyfae_gp ) {
    0,   4,  8,  0,
   86,  74, 91,  1,
  178, 161,175, 41,
  255, 237,139,  0};

DEFINE_GRADIENT_PALETTE( faecat_gp ) {
    0,  30, 18, 31,
   33,  78, 46, 82,
   63,  35, 43,  6,
   96,   5, 12,  2,
  130,  35,  5,  1,
  163, 190,122,  8,
  191,  84,146,240,
  221, 107, 56, 64,
  255,  30, 18, 31};

DEFINE_GRADIENT_PALETTE( fireandice_gp ) {
    0,  80,  2,  1,
   51, 206, 15,  1,
  101, 242, 34,  1,
  153,  16, 67,128,
  204,   2, 21, 69,
  255,   1,  2,  4};

DEFINE_GRADIENT_PALETTE( flame_gp ) {
    0, 252, 42,  1,
   43, 217,  6,  1,
   89, 213, 66,  1,
  127,   3, 74,  1,
  165, 213, 66,  1,
  211, 217,  6,  1,
  255, 252, 42,  1};

DEFINE_GRADIENT_PALETTE( lailah1_gp ) {
    0,  13,  1,  4,
   28,  34,  5, 19,
   56,   1,  1,  3,
   79,   1,  1, 75,
  101,  31, 15,  4,
  127,  80, 43,  7,
  150,   1, 45, 27,
  173,  64,252,205,
  196,  20, 23,  2,
  216, 234,252, 89,
  237,   8,  2,  6,
  255, 175, 70, 88};

DEFINE_GRADIENT_PALETTE( otis_gp ) {
    0,  26,  1, 89,
  127,  17,193,  0,
  216,   0, 34, 98,
  255,   0, 34, 98};

DEFINE_GRADIENT_PALETTE( parrot_gp ) {
    0, 126,  0,  1,
  114, 197,168, 16,
  140, 197,168, 16,
  216,   0,  2, 32,
  255,   0,  2, 32};

DEFINE_GRADIENT_PALETTE( Justified_Reasons_gp ) {
    0, 255,180, 29,
   36, 255,180, 29,
   73, 169,124, 14,
   87, 104, 78,  5,
  102,  64, 19,  5,
  107,  35,  1,  6,
  112,  26,  1,  6,
  133,  18,  1,  5,
  155,  10,  1,  4,
  205,   5,  1,  2,
  255,   5,  1,  2};

DEFINE_GRADIENT_PALETTE( iCreate_gp ) {
    0, 210, 80, 22,
   10, 210, 80, 22,
   20, 232, 47, 19,
   39, 255, 23, 17,
   58, 208,  9, 19,
   82, 167,  1, 21,
  107,  95,  1, 14,
  135,  46,  1,  8,
  163,  26,  1,  6,
  209,  12,  1,  4,
  255,  12,  1,  4};

DEFINE_GRADIENT_PALETTE( Action_Comics_No1_gp ) {
    0, 217,133,  1,
   25, 217,133,  1,
   51, 232,103,  1,
   76, 249, 77,  2,
  102, 186, 22,  1,
  127, 133,  1,  1,
  153,  73,  1,  2,
  178,  33,  1,  6,
  204,  14,  9, 11,
  229,   3, 23, 19,
  255,   3, 23, 19};

DEFINE_GRADIENT_PALETTE( Aleutian_Clouds_gp ) {
    0,  37,184,133,
   25,  37,184,133,
   51,  46,168, 98,
   76,  55,152, 67,
  102,  22,104, 77,
  127,   5, 66, 88,
  153,   1, 31, 49,
  178,   1,  9, 22,
  204,   1,  2, 11,
  229,   0,  1,  5,
  255,   0,  1,  5};

DEFINE_GRADIENT_PALETTE( uerracreativacom_gp ) {
    0,  19,  1,  2,
   25,  19,  1,  2,
   51,  25,  1,  2,
   76,  32,  0,  2,
  102,  77,  0,  2,
  127, 146,  0,  2,
  153, 192, 11,  4,
  178, 247, 52,  7,
  204, 224, 78, 10,
  229, 203,109, 13,
  255, 203,109, 13};

DEFINE_GRADIENT_PALETTE( Baffin_Bay_gp ) {
    0,   1,  1,  5,
   25,   1,  1,  5,
   51,   4, 13, 24,
   76,  16, 42, 61,
  102,  26, 81,114,
  127,  40,136,188,
  153,  91,159,192,
  178, 167,186,197,
  204, 206,217,223,
  229, 247,250,255,
  255, 247,250,255};

DEFINE_GRADIENT_PALETTE( r_a_i_n_y_gp ) {
    0,   8,  1, 54,
   25,   8,  1, 54,
   51,   2,  9, 67,
   76,   1, 33, 83,
  102,   2, 55, 82,
  127,   5, 84, 80,
  153,  27, 99, 78,
  178,  72,117, 75,
  204,  92,124, 69,
  229, 117,130, 64,
  255, 117,130, 64};

DEFINE_GRADIENT_PALETTE( Alive_And_Kicking_gp ) {
    0, 192, 50,207,
   25, 192, 50,207,
   51, 133, 50,207,
   76,  87, 50,207,
  102,  59, 61,207,
  127,  37, 73,207,
  153,  37, 97,207,
  178,  37,127,207,
  204,  37,166,172,
  229,  37,213,140,
  255,  37,213,140};

DEFINE_GRADIENT_PALETTE( Optimus_Prime_gp ) {
    0,   5, 16, 18,
   25,   5, 16, 18,
   51,   7, 25, 39,
   76,   8, 38, 71,
  102,  64, 99,106,
  127, 194,189,151,
  153, 182, 63, 42,
  178, 167,  6,  2,
  204, 100,  3,  1,
  229,  53,  1,  1,
  255,  53,  1,  1};

DEFINE_GRADIENT_PALETTE( visspec_gp ) {
    0,  22,  0, 25,
    0,  24,  0, 28,
    1,  26,  0, 31,
    1,  27,  0, 34,
    2,  29,  0, 37,
    3,  30,  0, 41,
    3,  32,  0, 44,
    4,  33,  0, 49,
    5,  35,  0, 52,
    5,  36,  0, 56,
    6,  38,  0, 61,
    7,  39,  0, 65,
    7,  39,  0, 69,
    8,  40,  0, 74,
    8,  41,  0, 79,
    9,  42,  0, 84,
   10,  43,  0, 89,
   10,  43,  0, 95,
   11,  44,  0,100,
   12,  44,  0,106,
   12,  44,  0,112,
   13,  44,  0,119,
   14,  44,  0,125,
   14,  44,  0,132,
   15,  43,  0,137,
   15,  43,  0,144,
   16,  42,  0,151,
   17,  41,  0,158,
   17,  40,  0,164,
   18,  39,  0,172,
   19,  39,  0,180,
   19,  37,  0,188,
   20,  36,  0,194,
   21,  35,  0,203,
   21,  33,  0,210,
   22,  32,  0,219,
   23,  30,  0,222,
  224, 128,  0,  0,
  224, 125,  0,  0,
  225, 121,  0,  0,
  226, 118,  0,  0,
  226, 115,  0,  0,
  227, 112,  0,  0,
  228, 109,  0,  0,
  228, 106,  0,  0,
  229, 103,  0,  0,
  230, 100,  0,  0,
  230,  97,  0,  0,
  231,  94,  0,  0,
  231,  91,  0,  0,
  232,  88,  0,  0,
  233,  86,  0,  0,
  233,  83,  0,  0,
  234,  80,  0,  0,
  235,  78,  0,  0,
  235,  75,  0,  0,
  236,  73,  0,  0,
  237,  71,  0,  0,
  237,  68,  0,  0,
  238,  66,  0,  0,
  239,  64,  0,  0,
  239,  61,  0,  0,
  240,  59,  0,  0,
  240,  57,  0,  0,
  241,  55,  0,  0,
  242,  53,  0,  0,
  242,  51,  0,  0,
  243,  49,  0,  0,
  244,  47,  0,  0,
  244,  46,  0,  0,
  245,  44,  0,  0,
  246,  41,  0,  0,
  246,  39,  0,  0,
  247,  38,  0,  0,
  247,  36,  0,  0,
  248,  35,  0,  0,
  249,  33,  0,  0,
  249,  32,  0,  0,
  250,  30,  0,  0,
  251,  28,  0,  0,
  251,  27,  0,  0,
  252,  26,  0,  0,
  253,  24,  0,  0,
  253,  22,  0,  0,
  254,  21,  0,  0,
  255,  20,  0,  0};

// Single array of defined cpt-city color palettes.
// This will let us programmatically choose one based on
// a number, rather than having to activate each explicitly 
// by name every time.
// Since it is const, this array could also be moved 
// into PROGMEM to save SRAM, but for simplicity of illustration
// we'll keep it in a regular SRAM array.
//
// This list of color palettes acts as a "playlist"; you can
// add or delete, or re-arrange as you wish.
const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
  visspec_gp,
  Optimus_Prime_gp,
  Alive_And_Kicking_gp,
  r_a_i_n_y_gp,
  uerracreativacom_gp,
  Action_Comics_No1_gp,
  Aleutian_Clouds_gp,
  Baffin_Bay_gp,
  Justified_Reasons_gp,
  iCreate_gp,
  rainbow_gp,
  quagga_gp,
  purplefly_gp,
  scarletdragon_gp,
  scoutie_gp,
  shyviolet_gp,
  spellbound_gp,
  springbird_gp,
  sunlitwave_gp,
  tashangel_gp,
  teabearrose_gp,
  titannightfall_gp,
  trove_gp,
  tubepreview_gp,
  angelcompassion_gp,
  aquamarinemermaid_gp,
  autumnrose_gp,
  blackhorse_gp,
  bluefly_gp,
  butterflyfairy_gp,
  butterflytalker_gp,
  carousel_gp,
  cloud_gp,
  daisyfae_gp,
  faecat_gp,
  fireandice_gp,
  flame_gp,
  lailah1_gp,
  otis_gp,
  parrot_gp,
  Sunset_Real_gp,
  es_rivendell_15_gp,
  es_ocean_breeze_036_gp,
  rgi_15_gp,
  retro2_16_gp,
  Analogous_1_gp,
  es_pinksplash_08_gp,
  Coral_reef_gp,
  es_ocean_breeze_068_gp,
  es_pinksplash_07_gp,
  es_vintage_01_gp,
  departure_gp,
  es_landscape_64_gp,
  es_landscape_33_gp,
  rainbowsherbet_gp,
  gr65_hult_gp,
  gr64_hult_gp,
  GMT_drywet_gp,
  ib_jul01_gp,
  es_vintage_57_gp,
  ib15_gp,
  Fuschia_7_gp,
  es_emerald_dragon_08_gp,
  lava_gp,
  fire_gp,
  Colorfull_gp,
  Magenta_Evening_gp,
  Pink_Purple_gp,
  es_autumn_19_gp,
  BlacK_Blue_Magenta_White_gp,
  BlacK_Magenta_Red_gp,
  BlacK_Red_Magenta_Yellow_gp,
  Blue_Cyan_Yellow_gp };


// Count of how many cpt-city gradients are defined:
const uint8_t gGradientPaletteCount = 
  sizeof( gGradientPalettes) / sizeof( TProgmemRGBGradientPalettePtr );
