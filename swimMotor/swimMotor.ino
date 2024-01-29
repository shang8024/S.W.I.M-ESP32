#include "FastLED.h"
FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

//#define FASTLED_FORCE_SOFTWARE_SPI
#define DEBUG_PIN 33

#define DATA_PIN 4
//#define CLK_PIN   13
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS 72
CRGB leds[NUM_LEDS];

#define BRIGHTNESS 31

int motor = 0;
int prevmotor = 0;

int pa = 0;
int pb = 0;
int pc = 0;

int pal = 0;
int pbl = 0;
int pcl = 0;

int paraw = 0;
int pbraw = 0;
int pcraw = 0;


//boolean debugon = 0;
boolean debugon = 1;

boolean drawgrat = 1;

uint8_t gHue = 0;  // rotating "base color" used by many of the patterns

void setup() {
  delay(1000);  // 3 second delay for recovery

  pinMode(DEBUG_PIN, OUTPUT);  //set debug pin as output


  Serial.begin(115200);
  // tell FastLED about the LED strip configuration
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<APA102,11,13,RGB,DATA_RATE_MHZ(16)>(leds,NUM_LEDS);  //10
  // set master brightness control
  FastLED.addLeds<WS2812, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.







void loop() {
  FastLED.clearData();

  //for (int i = 0; i < NUM_LEDS; i++)
  //{
  //  leds[i] = 0x0000;

  //}
  //digitalWrite(DEBUG_PIN, 1);

  pal = pa;
  pbl = pb;
  pcl = pc;

  motor = analogRead(36);

  prevmotor = motor;
  paraw = (((motor + 150) - 2047) * 16) + 2047;  ///not sure why i skippe A0, maybe noise, it's close to SCK
  //  pbraw = ((analogRead(39)+0));
  //  pcraw = ((analogRead(34)+0));
  pbraw = (((motor + 150) - 2047) * 16) + 2047;
  pcraw = (((motor + 150) - 2047) * 16) + 2047;

  pa = map(paraw + 0, 0, 4095, 0, NUM_LEDS);
  pb = map(pbraw + 0, 0, 4095, 0, NUM_LEDS);
  pc = map(pcraw + 0, 0, 4095, 0, NUM_LEDS);




  //pa = int(float(int(analogRead(0)) -512) * 1) + 72;
  //pb = int(float(int(analogRead(1)) -512) * 1) + 72;
  //pc = int(float(int(analogRead(2)) -512) * 1) + 72;

  if (pa > (NUM_LEDS - 1)) { pa = (NUM_LEDS - 1); }
  if (pa < 0) { pa = 0; }
  if (pb > (NUM_LEDS - 1)) { pb = (NUM_LEDS - 1); }
  if (pb < 0) { pb = 0; }
  if (pc > (NUM_LEDS - 1)) { pc = (NUM_LEDS - 1); }
  if (pc < 0) { pc = 0; }


  if (pa == pal) { leds[pa] |= CRGB(255, 0, 0); }
  if (pb == pbl) { leds[pb] |= CRGB(0, 255, 0); }
  if (pc == pcl) { leds[pc] |= CRGB(0, 0, 255); }

  if (pa > pal) {
    for (int f = pal; f < pa; f++) {
      leds[f] |= CRGB(255, 0, 0);
    }
  }
  if (pa < pal) {
    for (int f = pa; f < pal; f++) {
      leds[f] |= CRGB(255, 0, 0);
    }
  }


  if (pb > pbl) {
    for (int f = pbl; f < pb; f++) {
      leds[f] |= CRGB(0, 255, 0);
    }
  }
  if (pb < pbl) {
    for (int f = pb; f < pbl; f++) {
      leds[f] |= CRGB(0, 255, 0);
    }
  }

  if (pc > pcl) {
    for (int f = pcl; f < pc; f++) {
      leds[f] |= CRGB(0, 0, 255);
    }
  }
  if (pc < pcl) {
    for (int f = pc; f < pcl; f++) {
      leds[f] |= CRGB(0, 0, 255);
    }
  }


  if (drawgrat) {
    for (int g = 0; g < 72; g = g + 11) {
      //    leds[g+3]|=CRGB(75,0,75);
      leds[g + 2] |= CRGB(75, 0, 75);  // center the grat
    }
  }

  //digitalWrite(DEBUG_PIN, 0);


  //fill_rainbow( leds, NUM_LEDS, gHue, 5); gHue=gHue+1;
  FastLED.show();
  // insert a delay to keep the framerate modest
  //FastLED.delay(10);


  if (debugon) {
    Serial.print(paraw);  //1895
    Serial.print("\t");
    Serial.print(pbraw);
    Serial.print("\t");
    Serial.print(pcraw);
    Serial.print("\n");
  }
}