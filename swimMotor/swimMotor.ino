#include "FastLED.h"
#include <WiFi.h>
#include "secrets.h"
#include "charMap.h"
#include <HTTPClient.h>
#include "esp_wpa2.h"

FASTLED_USING_NAMESPACE

#ifndef SECRETS
//WPA2-Personal
const char *ssid = "ssid";
const char *pass = "pass";
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

#define FONT_HEIGHT 8
#define FONT_WIDTH 5 // plus 2 for gap
#define FONT_GAP 2
#define MAX_STRING_LENGTH 25

#define BRIGHTNESS 31

int motor = 0;
int prevmotor = 0;

int state = 0;
int total_state = 0;
int speed = 2;
int space = 2; // gap between repeating image/text

//boolean debugon = 0;
boolean debugon = 1;

boolean drawgrat = 1;
boolean autodraw = 0;
int scale = 2;

//values for the image
int img_height = 12;
int img_width = 12;
unsigned int img[256] = {
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

int auto_delay = 20;

//values for text
int string_length = 8;
char string[MAX_STRING_LENGTH] = "ECE 516";
int font_color[3] = {243, 57, 166};
int grad_end[3] = {43, 225, 238};
//TBD: font
//TBD: gradient

uint8_t gHue = 0;  // rotating "base color" used by many of the patterns

boolean display_mode = 0; // 0 for image, 1 for text

void startAnime(){
  FastLED.clearData();
  // from center of leds, gradually turn on leds
  for (int i =0; i<NUM_LEDS/2;i++){
    FastLED.show();
    if (WiFi.status() != WL_CONNECTED){
      leds[NUM_LEDS/2-i] |= CRGB(255,0,0);
      leds[NUM_LEDS/2+1+i] |= CRGB(255,0,0);
    }
    else{
      leds[NUM_LEDS/2-i] |= CRGB(255,255,255);
      leds[NUM_LEDS/2+1+i] |= CRGB(255,255,255);
    }
    FastLED.show();
    delay(10);
  }
  // then from two ends of leds, gradually turn off leds
  for (int i =0; i<NUM_LEDS/2;i++){
    leds[i] = CRGB(0,0,0);
    leds[NUM_LEDS-1-i] = CRGB(0,0,0);
    FastLED.show();
    delay(10);
  }
}

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

  if (display_mode == 0) total_state = (img_width+space)*speed;
  else total_state = (string_length * (FONT_WIDTH + FONT_GAP) * scale + space)*speed;
  if (scale>=3 || autodraw) speed=1;
  else speed=2;

#ifndef SECRETS
  WiFi.mode(WIFI_STA);
#endif
  WiFi.disconnect(true);
  Serial.print("MAC >> ");
  Serial.println(WiFi.macAddress());
  Serial.print("Connecting to: ");
  Serial.println(ssid);
#ifndef SECRETS
  WiFi.begin(ssid,pass);
#else
  WiFi.begin(ssid, WPA2_AUTH_PEAP, identity, username, pass);
#endif
  int tmp = 0;
  while((WiFi.status() != WL_CONNECTED && timeout < 10000) || tmp<NUM_LEDS)
  {
    FastLED.clearData();
    Serial.print(".");
    timeout += 10;
    leds[tmp] |= CRGB(255,255,255);
    tmp++;
    if(tmp>=NUM_LEDS && WiFi.status() != WL_CONNECTED && timeout < 10000) tmp=0;
    FastLED.show();
    delay(10);
  }
  Serial.print("\n");
  
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connection Failed!\n");
    startAnime();
    return;
  }
  Serial.print("\n");
  Serial.print("Connected to Wi-Fi: ");
  Serial.println(WiFi.SSID());
  server.begin(); /* Start the HTTP web Server */
  startAnime();
  Serial.print("Connect to IP Address: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.print("\n");
}

// List of patterns to cycle through.  Each is defined as a separate function below.
void stateUpdate(){
  if (total_state <= 0) return;
  if (autodraw){
    state ++;
    delay(auto_delay);
  }else {
    // read motor value and change 
    motor = analogRead(MOTOR_PIN);
    int diff = motor - prevmotor;
    prevmotor = motor;
    if (motor < 1964 && motor >1836) return;
    if (motor >= 1964) state ++;
    if (motor <= 1836) state --;
  }
      // display image based on state, 
    // Your name or other text should appear forwards when ind increases
    // The text or image content should display backwards when ind decreases
    if (state >= total_state) state = 0;
    if (state < 0) state = total_state - 1;
}

void displayImage(){
  if (total_state <= 0) return;
  if (state >= total_state - space*speed) return;
  // for width=1 led stip, display the state-th column of the image
  int start = (NUM_LEDS - img_height) / 2;
  int ind = state / speed / scale;
  // ind is between 0 and img_width
  for (int i = 0; i < img_height; i++){
    // if the pixel is black, do nothing
    if (img[i * img_width + ind] == 0) continue;
    // if the pixel is not black, get the color
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

void displayGrat(){
  if (drawgrat) {
    for (int g = 0; g < 72; g = g + 11) {
      leds[g + 2] |= CRGB(75, 0, 75);  // center the grat
    }
  }
}

void displayChar(char x,int offset,int start){
  if (offset >= FONT_WIDTH || offset < 0) return;
  int ind = x - 0x20;
  // 0x7e - 0x20 = 0x5e -> 94 characters
  if (ind == 0) return;
  if (ind > char_map_size || ind < 0) ind = char_map_size;
  // calculate color based on gradient from font_color to grad_end
  // the percentage of the gradient is based on the state/total_state
  int r = font_color[0] + (grad_end[0] - font_color[0]) * state / total_state;
  int g = font_color[1] + (grad_end[1] - font_color[1]) * state / total_state;
  int b = font_color[2] + (grad_end[2] - font_color[2]) * state / total_state;
  for (int i = 0; i < FONT_HEIGHT; i++){
    // if the pixel is black, do nothing
    unsigned int y = charMap[ind-1][i];
    if (y == 0) continue;
    //y is a 2-byte number, each bit represents a pixel
    //extract the offset-th bit
    bool a = (y >> (FONT_WIDTH - offset - 1)) & 0x01;
    if (a == 0) continue;
    // set the color to the corresponding leds
    for (int j = 0; j < scale; j++){
      // print in reverse order
      leds[NUM_LEDS - 1 - (i * scale + j + start)] |= CRGB(r, g, b);
    }
  }  
}

void displayText(){
  if (total_state == 0) return;
  if (state >= total_state - space*speed) return;
  int start = (NUM_LEDS - FONT_HEIGHT * scale) / 2;
  int char_ind = state / speed / scale;
  int char_offset = char_ind % (FONT_WIDTH + FONT_GAP);
  char_ind = char_ind / (FONT_WIDTH + FONT_GAP);
  // char_ind is between 0 and string_length
  // char_offset is between 0 and FONT_WIDTH+FONT_GAP
  // display the char_ind-th character of the string
  displayChar(string[char_ind], char_offset, start);
}

void html(){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();

  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.println("<head>");
    client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<link rel=\"icon\" href=\"data:,\">");
    client.println("<style>");
      client.println("html { font-family: Roboto; display: inline-block; margin: 0px auto; text-align: center;}");
      client.println(".button {background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer;");
      client.println("text-decoration: none; font-size: 25px; margin: 2px; cursor: pointer;}");
      client.println(".button_ON {background-color: white; color: black; border: 2px solid #4CAF50;}");
      client.println(".button_OFF {background-color: white; color: black; border: 2px solid #f44336;}");
    client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h2>ECE516 - S.W.I.M</h2>");
  client.println("<p>Send a message to ESP32 to change the display content</p>");
  client.println("<hr/>");
  client.println("<form action=\"/Config\n\">");
    client.println("<h4>Change Configuration <a><button type=\"submit\">Send</button></a></h4>");
    client.print("<p><input type=\"checkbox\" id=\"grat\" name=\"grat\" value=\"On\"");
    if(drawgrat) client.print("checked");
    client.println(">");
      client.println("<label for=\"grat\">Display Grat</label><br></p>");
    client.println("<p><label for=\"gap\">Gap<small>(between repeating display)</small>:</label>");
      client.print("<input type=\"number\" id=\"gap\" name=\"gap\" value=\"");
      client.print(space);
      client.println("\" required min=\"0\" max=\"10\"></p>");
    client.println("<p><label for=\"delay\">Delay<small>(for auto display)</small>:</label>");
      client.print("<input type=\"number\" id=\"delay\" name=\"delay\" value=\"");
      client.print(auto_delay);
      client.println("\" required min=\"0\" max=\"100\"></p>");
  client.println("</form>");
  client.println("<hr/>");
  client.println("<form action=\"/Mes\n\">");
    client.println("<h4>Change Text Display <a><button type=\"submit\">Send</button></a></h4>");
    client.println("<p><label for=\"text\">Text<small>(optional)</small>:</label>");
      client.print("<input type=\"text\" id=\"text\" name=\"text\" placeholder=\"ECE 516\" maxlength=\"25\" value=\"");
      client.print(string);
      client.println("\"></p>");
    client.println("<p><label for=\"scale\">Scale:</label>");
      client.print("<input type=\"number\" id=\"scale\" name=\"scale\" value=\"");
      client.print(scale);
      client.println("\" required min=\"1\" max=\"6\"></p>");
    client.println("<p><label for=\"color\">Font Color:</label>");
      client.print("<input type=\"color\" id=\"color\" name=\"color\" value=\"#");
      for(int i = 0; i < 3; i++)
      {
        client.printf("%x", font_color[i]);
        if(font_color[i] == 0) client.print("0");
      }
      client.println("\"/>");
      client.print("<input type=\"color\" id=\"gradend\" name=\"gradend\" value=\"#");
      for(int i = 0; i < 3; i++)
      {
        client.printf("%x", grad_end[i]);
        if(grad_end[i] == 0) client.print("0");
      }
      client.println("\" /></p>");
  client.println("</form>");
  client.println("<hr/>");
  if(display_mode == 0)
  {
    client.print("<p><a href=\"/Toggle\n\"><button class=\"button button_ON\">Display Text</button></a></p>"); 
  } else
  {
    client.print("<p><a href=\"/Toggle\n\"><button class=\"button button_OFF\">Display Image</button></a></p>"); 
  } 
  if(autodraw)
  {
    client.print("<p><a href=\"/Auto\n\"><button class=\"button button_ON\">Auto Off</button></a></p>"); 
  } else
  {
    client.print("<p><a href=\"/Auto\n\"><button class=\"button button_OFF\">Auto On</button></a></p>"); 
  } 
  client.println("</body>");
 client.println("</html>");    
}

void toggleDisplay(){
  if (scale>=3 || autodraw) speed=1;
  else speed=2;
  if (display_mode == 0)
  {
    Serial.println("Printing Text\n");
    total_state = (string_length * (FONT_WIDTH + FONT_GAP) * scale + space)*speed;
  } else
  {
    Serial.println("Printing Image\n");
    total_state = (img_width+space)*speed;
  }
  display_mode = !display_mode;
  state = 0;
}

void display(){
  FastLED.clearData();

  stateUpdate();
  if (display_mode==0) displayImage();
  else displayText();

  displayGrat();
  //fill_rainbow( leds, NUM_LEDS, gHue, 5); gHue=gHue+1;
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(10);
}

String extractString(String req, char* str){
  int start = req.indexOf(str);
  if (start == -1) return "";
  start += strlen(str);
  int end = req.indexOf("&", start);
  if (end == -1) end = req.indexOf(" ", start);
  String tmp = req.substring(start, end);
  //replace + with space
  tmp.replace("+", " ");
  //for each %xy, replace with corresponding char
  int pos = 0;
  while((pos = tmp.indexOf("%", pos)) != -1)
  {
    char c = strtol(tmp.substring(pos+1, pos+3).c_str(), NULL, 16);
    tmp.replace(tmp.substring(pos, pos+3), String(c));
    pos++;
  }
  return tmp;
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    display();
    return;
  }
  client = server.available();
  if(!client)
  {
    display();
    return;
  }
  while (client.connected())
  {
    display();
    if (client.available())
    {
      char c = client.read();
      request += c;

      if (c == '\n')
      {
        Serial.println(request);
        String tmp;
        if (request.indexOf("GET /Config") != -1)
        {
          if(request.indexOf("grat") != -1) drawgrat = 1;
          else drawgrat = 0;
          tmp = extractString(request, "gap=");
          if (tmp.length() > 0) space = tmp.toInt();

          tmp = extractString(request, "delay=");
          if (tmp.length() > 0) auto_delay = tmp.toInt();

          state = 0;
        }
        if (request.indexOf("GET /Mes") != -1)
        {
          tmp = extractString(request, "scale=");
          if (tmp.length() > 0) scale = tmp.toInt();

          tmp = extractString(request, "color=%23");
          if (tmp.length() > 0)
          {
            for(int i = 0; i < 3; i++)
            {
              font_color[i] = strtol(tmp.substring(i*2, i*2+2).c_str(), NULL, 16);
            }
          }
          tmp = extractString(request, "gradend=%23");
          if (tmp.length() > 0)
          {
            for(int i = 0; i < 3; i++)
            {
              grad_end[i] = strtol(tmp.substring(i*2, i*2+2).c_str(), NULL, 16);
            }
          }
          tmp = extractString(request, "text=");
          if (tmp.length() > 0 && tmp.length() < MAX_STRING_LENGTH)
          {
            tmp.toCharArray(string, MAX_STRING_LENGTH);
            string_length = tmp.length();
            Serial.println(string);
            if(display_mode == 0) toggleDisplay();
            else total_state = (string_length * (FONT_WIDTH + FONT_GAP) * scale + space)*speed;
          }
          if (scale>=3 || autodraw) speed=1;
          else speed=2;
          state = 0;
        }

        if (request.indexOf("GET /Toggle") != -1)
        {
          toggleDisplay();
        }
        if (request.indexOf("GET /Auto") != -1)
        {
          autodraw = !autodraw;
          state = 0;
          if (scale>=3 || autodraw) speed=1;
          else speed=2;
        }
        html();
        break;
    }
    }
  }
  request="";
  client.stop();
}