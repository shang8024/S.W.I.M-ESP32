#include "FastLED.h"

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
#define INNER_R 4  //76*76 image

#define threshold 1023
#define MAX_STATES 484

#define BRIGHTNESS 31


// defining variables
int pa = 0;
int pb = 0;
int pc = 0;
int pal = 0;
int pbl = 0;
int pcl = 0;

//boolean debugon = 0;
boolean debugon = 1;
long va = 0;
long vb = 0;
long vc = 0;
long theta = 0;

boolean drawgrat = 1;

uint8_t gHue = 0;  // rotating "base color" used by many of the patterns


const int row = 72;
const int col = 69;
const int total_frame = 1;

int state = 0;
int frame = 0;




int pos = 0;
unsigned long delayStart = 0;



//defining the polar coordinate image array
int img[total_frame][row][col][3] = {};


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

void stateUpdate() {
  //store the previous pulse
  plast = pcur;

  pa = analogRead(36);  //real
  pb = analogRead(39);  //imaginary
  // pc=analogRead(34);//voltage

  pcur = (pa > threshold) * 2 + (pb > threshold);
  int dir = QEM[plast * 4 + pcur];
  // if we missed a pulse, we need to guess the direction, by previous direction
  if (dir == 0) return;
  else if (dir == 2) state += 1.5 * dir_last;
  else {
    state += dir;
    dir_last = dir;
  }
  if (state < 0) state = MAX_STATES - 1;
  else if (state >= MAX_STATES) state = 0;
}

void setup() {
  delay(1000);  // 3 second delay for recovery

  pinMode(DEBUG_PIN, OUTPUT);  //set debug pin as output

  Serial.begin(115200);
  FastLED.addLeds<WS2812, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  pa = analogRead(36);  //real
  pb = analogRead(39);  //imaginary
  // pc=analogRead(34);//voltage
  delayStart = millis();

  pcur = (pa > threshold) * 2 + (pb > threshold);
}

void loop() {
  FastLED.clearData();

  stateUpdate();

  if (total_frame > 0 && (millis() - delayStart) > 10000) {
    frame++;
    if (frame >= total_frame) frame = 0;
    delayStart = millis();
  }

  pos = state * 72 / MAX_STATES;
  leds[pos] = CRGB(255, 255, 255);




  if (drawgrat) {
    for (int g = 0; g < 72; g = g + 11) {
      leds[g + 2] |= CRGB(75, 0, 75);  // center the grat
    }
  }


  FastLED.show();

  //  FastLED.delay(100);
}
