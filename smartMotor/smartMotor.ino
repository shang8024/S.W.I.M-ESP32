#include "FastLED.h"
#include "polarImage.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

//#defining pins
#define DEBUG_PIN 33

#define DATA_PIN 4
//#define CLK_PIN   13
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS 72
CRGB leds[NUM_LEDS];
#define INNER_R 3  //75*75 image

#define threshold 1023
#define MAX_STATES 484

#define BRIGHTNESS 10


// defining variables
int pal = 0;
int pbl = 0;
int pcl = 0;
int pa = 0;
int pb = 0;
int pc = 0;
int paraw = 0;
int pbraw = 0;
int pcraw = 0;

//boolean debugon = 0;
boolean debugon = 1;

boolean drawgrat = 1;

#ifndef POLAR_IMAGE // i.e, doesn't have polar
const int row = 72; 
const int col = 75;
const int total_frame = 0;
//defining the polar coordinate image array, 75*75 pixels, with inner R 3
unsigned int img[row][col] = {};
unsigned int *frames[1] = {};
#endif

int state = 0;
int frame = 0;

unsigned long delayStart = 0;

int mode = 0; // 0 for data, 1 for image, 2 for three phase






int QEM[16] = { 0, -1, 1, 2, 1, 0, 2, -1, -1, 2, 0, 1, 2, 1, -1, 0 };  // Quadrature Encoder Matrix
int dir_last = 0;
// +----------+----+----+----+----+
// | old\curr | 00 | 01 | 10 | 11 |
// +----------+----+----+----+----+
// | 00       | 0  | -1 | +1 | X  |
// | 01       | +1 | 0  | X  | -1 |
// | 10       | -1 | X  | 0  | +1 |
// | 11       | X  | +1 | -1 | 0  |
// +----------+----+----+----+----+
int plast = 0;
int pcur = 0;

void rawLEDs(int last, int cur, int r, int g, int b) {
  leds[cur] |= CRGB(r, g, b);
  if (last > cur) {
    for (int i = cur; i < last; i++) {
      leds[i] |= CRGB(r, g, b);
    }
  } else if (last < cur) {
    for (int i = last; i < cur; i++) {
      leds[i] |= CRGB(r, g, b);
    }
  }

}

void rawDataDisplay(){
  pal = pa;
  pbl = pb;
  pcl = pc;

  // Show it working as a scientific data visualizer to visualize the raw output of the complex-valued encoder:
  // real is red, green is imaginary, and blue is the motor output.
  paraw = analogRead(36);  //real
  pbraw = analogRead(39);  //imaginary
  pcraw = analogRead(34);//voltage

  pa = map(paraw, 0, 4095, 0, NUM_LEDS - 1);
  pb = map(pbraw, 0, 4095, 0, NUM_LEDS - 1);
  pc = map(pcraw, 0, 4095, 0, NUM_LEDS - 1);

  rawLEDs(pal, pa, 255, 0, 0);
  rawLEDs(pbl, pb, 0, 255, 0);
  rawLEDs(pcl, pc, 0, 0, 255);
}

void threePhaseDisplay(){
  pal = pa;
  pbl = pb;
  pcl = pc;

  paraw = analogRead(36);  //real
  pbraw = analogRead(39);  //imaginary
  pcraw = analogRead(34);//voltage
  // apply the inverse Edith Clarke transform to the complex-valued encoder with an assumed zero third component
  long x = sqrt(2/3) * paraw;
  long y = sqrt(2/3) * (sqrt(3) * pbraw - paraw) / 2;
  long z = sqrt(2/3) * (-sqrt(3) * pcraw - paraw) / 2;

  // map the three phase values to the LED strip
  pa = map((int)x, -2500, 2500, 0, NUM_LEDS - 1);
  pb = map((int)y, -2500, 2500, 0, NUM_LEDS - 1);
  pc = map((int)z, -2500, 2500, 0, NUM_LEDS - 1);

  rawLEDs(pal, pa, 255, 0, 0);
  rawLEDs(pbl, pb, 0, 255, 0);
  rawLEDs(pcl, pc, 0, 0, 255);

}

void stateUpdate() {
  //store the previous pulse
  plast = pcur;

  paraw = analogRead(36);  //real
  pbraw = analogRead(39);  //imaginary
  // pcraw=analogRead(34);//voltage

  pcur = (paraw > threshold) * 2 + (pbraw > threshold);
  int dir = QEM[plast * 4 + pcur];
  // if we missed a pulse, we need to guess the direction, by previous direction
  if (dir == 0) return;
  else if (dir == 2) state += 1.5 * dir_last;
  else {
    state += dir;
    dir_last = dir;
  }
  if (state < 0){
    state = MAX_STATES - 1;
    frame --;
    if (frame < 0) frame = total_frame - 1;
  }
  else if (state >= MAX_STATES){
    state = 0;
    frame ++;
    if (frame >= total_frame) frame = 0;
  }
}

void testDisplay(){
  int pos = state * 72 / MAX_STATES;
  leds[pos] |= CRGB(255, 255, 255);
}

void displayPolarImages(){
  int pos = state * col / MAX_STATES;
  unsigned int* cur_img = frames[frame];
  for (int i = 0; i < row; i++) {
    unsigned int color = *(cur_img + i * col + pos);
    int b = (color >> 16) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int r = color & 0xFF;
    leds[NUM_LEDS -1-i] = CRGB(r, g, b);
  }
}

void setup() {
  delay(1000);  // 3 second delay for recovery

  pinMode(DEBUG_PIN, OUTPUT);  //set debug pin as output

  Serial.begin(115200);
  FastLED.addLeds<WS2812, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  paraw = analogRead(36);  //real
  pbraw = analogRead(39);  //imaginary
  pcraw = analogRead(34);//voltage
  // delayStart = millis();

  pcur = (paraw > threshold) * 2 + (pbraw > threshold);
}

void loop() {
  FastLED.clearData();
  if (mode == 0){
    rawDataDisplay();
  }else if (mode == 2){
    threePhaseDisplay();
  }else{
    stateUpdate();

  // if (total_frame > 0 && (millis() - delayStart) > 10000) {
  //   frame++;
  //   if (frame >= total_frame) frame = 0;
  //   delayStart = millis();
  // }

    if(total_frame == 0){
      testDisplay();
    }else{
      displayPolarImages();
    }
  }



  if (drawgrat) {
    for (int g = 0; g < 72; g = g + 11) {
      leds[g + 2] |= CRGB(75, 0, 75);  // center the grat
    }
  }


  FastLED.show();

  //  FastLED.delay(100);
}
