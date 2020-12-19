#include <Arduino.h>

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    5
#define LED_TYPE    WS2811
//#define LED_TYPE    WS2812B
#define COLOR_ORDER BRG
//#define COLOR_ORDER GRB
#define NUM_LEDS    300
//#define NUM_LEDS    61
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
//#define BRIGHTNESS          50
#define FRAMES_PER_SECOND  120

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Gradient palette "bhw2_xmas_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_xmas.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 48 bytes of program space.

DEFINE_GRADIENT_PALETTE( christmasPalette ) {
    0,   0, 12,  0,
   40,   0, 55,  0,
   66,   1,117,  2,
   77,   1, 84,  1,
   81,   0, 55,  0,
  119,   0, 12,  0,
  153,  42,  0,  0,
  181, 121,  0,  0,
  204, 255, 12,  8,
  224, 121,  0,  0,
  244,  42,  0,  0,
  255,  42,  0,  0};

// Gradient palette "white_christmas_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ncdc/tn/white-christmas.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 72 bytes of program space.

DEFINE_GRADIENT_PALETTE( christmasPalette2 ) {
    0, 117, 97, 49,
   12, 117, 97, 49,
   12,  26,173,106,
   25,  26,173,106,
   25,  10, 77,164,
   63,  10, 77,164,
   63,   0, 21, 85,
  102,   0, 21, 85,
  102,  53, 86,135,
  127,  53, 86,135,
  127,  26, 31, 65,
  153,  26, 31, 65,
  153,  61, 31, 96,
  191,  61, 31, 96,
  191,  82, 97, 85,
  229,  82, 97, 85,
  229, 255,255,255,
  255, 255,255,255};

// Gradient palette "garish7_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/jjg/dem/tn/garish7.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 56 bytes of program space.

DEFINE_GRADIENT_PALETTE( christmasPalette3 ) {
    0,   7,139,  6,
    4,   7,139,  6,
    4, 106,189, 12,
    8, 106,189, 12,
    8, 227,217,  1,
   17, 227,217,  1,
   17, 239,151,  1,
   42, 239,151,  1,
   42, 239, 61,  0,
   84, 239, 61,  0,
   84,  72,  9, 42,
  170,  72,  9, 42,
  170, 163, 93,164,
  255, 163, 93,164};



// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void addChristmasGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::Red;
    leds[ random16(NUM_LEDS) ] += CRGB::Green;
  }
}

void addPaletteGlitter(CRGBPalette16 palette, fract8 chanceOfGlitter)
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] = ColorFromPalette(palette, gHue*random16(ARRAY_SIZE(palette)));
  }
}


void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void xmas_12()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  CRGBPalette16 palette = christmasPalette2;
  int pos = beatsin16( 20, 0, NUM_LEDS-1 );
  leds[pos] += ColorFromPalette(palette, gHue+(pos*2));
}

void xmas_11() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  CRGBPalette16 palette = christmasPalette3;
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    //leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] = ColorFromPalette(palette, dothue+(i*2), dothue+(i*10));
    dothue += 32;
  }
}

void xmas_10()
{
  // colored speckles from a pelette that blink in and fade smoothly
  EVERY_N_MILLISECONDS(50) {
    fadeToBlackBy( leds, NUM_LEDS, 10);
  }
  addPaletteGlitter(christmasPalette, 50);
}

void xmas_9()
{
  // colored speckles from a pelette that blink in and fade smoothly
  EVERY_N_MILLISECONDS(50) {
    fadeToBlackBy( leds, NUM_LEDS, 10);
  }
  addPaletteGlitter(christmasPalette2, 80);
  addGlitter(10);
}

void xmas_8()
{
  // colored speckles from a pelette that blink in and fade smoothly
  EVERY_N_MILLISECONDS(100) {
    fadeToBlackBy( leds, NUM_LEDS, 20);
  }
  addPaletteGlitter(christmasPalette3, 50);
}

void xmas_7()
{
  // colored speckles from a pelette that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 20);
  addChristmasGlitter(10);    
}

void xmas_6()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  CRGBPalette16 palette = christmasPalette3;
  int pos = random16(NUM_LEDS);
  leds[pos] += ColorFromPalette(palette, gHue+(pos*2), gHue+(pos*10));
  addGlitter(40);
}

void xmas_5() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  CRGBPalette16 palette = christmasPalette2;
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    //leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] = ColorFromPalette(palette, dothue+(i*2), dothue+(i*10));
    dothue += 32;
  }
}

void xmas_4()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  CRGBPalette16 palette = christmasPalette;
  int pos = beatsin16( 20, 0, NUM_LEDS-1 );
  leds[pos] += ColorFromPalette(palette, gHue+(pos*2));
}

void xmas_3()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  CRGBPalette16 palette = christmasPalette;
  int pos = random16(NUM_LEDS);
  leds[pos] += ColorFromPalette(palette, gHue+(pos*2), gHue+(pos*10));
  addGlitter(10);
}

void xmas_2() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  CRGBPalette16 palette = christmasPalette;
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    //leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] = ColorFromPalette(palette, dothue+(i*2), dothue+(i*10));
    dothue += 32;
  }
}

void xmas_1() {
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = christmasPalette;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { 
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}
  
SimplePatternList gPatterns = { 
  xmas_12,
  xmas_11,
  xmas_10,
  xmas_9,
  xmas_8,
  xmas_7,
  xmas_6,
  xmas_5,
  xmas_4,
  xmas_3,
  xmas_2,
  xmas_1,
};

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void randomPattern()
{
  gCurrentPatternNumber = random8(ARRAY_SIZE(gPatterns));
}

void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_MINUTES( 2 ) { randomPattern(); } // change patterns periodically
  //EVERY_N_SECONDS( 10 ) { randomPattern(); } // change patterns periodically
}