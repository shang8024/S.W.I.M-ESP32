#include "FastLED.h"
#include <WiFi.h>
#include "secrets.h"

FASTLED_USING_NAMESPACE


#ifndef SECRETS
//WPA2-Personal
const char *wifi_ssid = "SWIMtest";
const char *wifi_pass = "ece516BA3165";
#endif

WiFiServer server(80); /* Instance of WiFiServer with port number 80 */
String request;
WiFiClient client;

int timeout = 0;


#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

//#define FASTLED_FORCE_SOFTWARE_SPI
#define DEBUG_PIN 33
#define MOTOR_PIN 36
#define threshold 64

#define DATA_PIN 4
//#define CLK_PIN   13
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS 72
CRGB leds[NUM_LEDS];

#define BRIGHTNESS 31

int motor = 0;
int prevmotor = 0;

int state = 0;
int speed = 1;

//boolean debugon = 0;
boolean debugon = 1;

boolean drawgrat = 1;

int img_height = 12;
int img_width = 12;
const unsigned int img[] = {
  0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ed1c24, 0x00ed1c24, 0x00ed1c24, 0x00ed1c24, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
	0x00000000, 0x00000000, 0x00ed1c24, 0x00ed1c24, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ed1c24, 0x00ed1c24, 0x00000000, 0x00000000, 
	0x00000000, 0x00ed1c24, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ed1c24, 0x00000000, 
	0x00000000, 0x00ed1c24, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ed1c24, 0x00000000, 
	0x00ed1c24, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ed1c24, 
	0x00ed1c24, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ed1c24, 0x00ed1c24, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ed1c24, 
	0x00ed1c24, 0x00ed1c24, 0x00000000, 0x00000000, 0x00ed1c24, 0x00000000, 0x00000000, 0x00ed1c24, 0x00000000, 0x00000000, 0x00ed1c24, 0x00ed1c24, 
	0x00ffffff, 0x00000000, 0x00ed1c24, 0x00ed1c24, 0x00ed1c24, 0x00000000, 0x00000000, 0x00ed1c24, 0x00ed1c24, 0x00ed1c24, 0x00000000, 0x00ffffff, 
	0x00000000, 0x00ffffff, 0x00000000, 0x00000000, 0x00000000, 0x00ed1c24, 0x00ed1c24, 0x00000000, 0x00000000, 0x00000000, 0x00ffffff, 0x00000000, 
	0x00000000, 0x00ffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ffffff, 0x00000000, 
	0x00000000, 0x00000000, 0x00ffffff, 0x00ffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ffffff, 0x00ffffff, 0x00000000, 0x00000000, 
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};
int scale = 2;

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

  motor = analogRead(MOTOR_PIN);
  prevmotor = motor;

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  Serial.print("MAC >> ");
  Serial.println(WiFi.macAddress());
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED && timeout < 1000000)
  {
    Serial.print(".");
    timeout += 100;
    delay(100);
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connection Failed!\n");
    return;
  }
  Serial.print("\n");
  Serial.print("Connected to Wi-Fi ");
  Serial.println(WiFi.SSID());
  delay(1000);
  server.begin(); /* Start the HTTP web Server */
  Serial.print("Connect to IP Address: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.print("\n");
}

// List of patterns to cycle through.  Each is defined as a separate function below.
void stateUpdate(){
  // read motor value and change 
  motor = analogRead(MOTOR_PIN);
  int diff = motor - prevmotor;
  prevmotor = motor;
  if (motor < 1964 && motor >1836){
    return;
  }
  if (motor >= 1964) state ++;
  if (motor <= 1836) state --;
  // display image based on state, 
  // Your name or other text should appear forwards when ind increases
  // The text or image content should display backwards when ind decreases
  if (state > img_width*speed*scale) state = 0;
  if (state < 0) state = img_width * speed * scale;
}

void displayImage(){
  // for width=1 led stip, display the state-th column of the image
  int start = (NUM_LEDS - img_height * scale) / 2;
  int ind = state / (speed * scale);
  // ind is between 0 and img_width
  for (int i = 0; i < img_height; i++){
    // if the pixel is black, do nothing
    if (img[i * img_width + ind] == 0) continue;
    // if the pixel is not black, get the color
    // convert 0x00RRGGBB to RR in deximal
    int r = (img[i * img_width + ind] >> 16) & 0xFF;
    int g = (img[i * img_width + ind] >> 8) & 0xFF;
    int b = img[i * img_width + ind] & 0xFF;
    
    // set the color to the corresponding leds
    for (int j = 0; j < scale; j++){
      // print in reverse order
      leds[NUM_LEDS - 1 - (i * scale + j + start)] = CRGB(r, g, b);
    }
  }
}

void html(){
  
}

void loop() {
  FastLED.clearData();

  stateUpdate();
  displayImage();

  if (drawgrat) {
    for (int g = 0; g < 72; g = g + 11) {
      //    leds[g+3]|=CRGB(75,0,75);
      leds[g + 2] |= CRGB(75, 0, 75);  // center the grat
    }
  }

  //fill_rainbow( leds, NUM_LEDS, gHue, 5); gHue=gHue+1;
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(10);
}