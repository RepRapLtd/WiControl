#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include "FastLED.h"

// NETWORK: Static IP details...change these to your choice
IPAddress ip(192, 168, 0, 203); //static ip of WeMos D1 Mini
IPAddress gateway(192, 168, 0, 100); // ip of router/access point
IPAddress subnet(255, 255, 255, 0); // subnet of router/access point
#define LED_PIN     4 // D2 on Wemos D1 mini V1
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

// Fire
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 110
bool gReverseDirection = false;
CRGBPalette16 gPal;



const uint8_t NUM_LEDS  = 30;     // number of leds in string
int BRI = 240;   // initial brightness
int rainB = 0;
int gHue = 5;
int fire = 0;

CRGB leds[NUM_LEDS];
const long rainBinterval = 22;
const long fireinterval = 33;
unsigned long currentMillis = millis();
unsigned long previousMillis = millis();

// prototypes
boolean connectWifi();

//on/off callbacks
void whiteLightsOn();
void LightsOff();
void fireLightsOn();
void rainbowLightsOn();
void dimLightsOn();
void dimLightsOff();

// Change this before you flash
const char* ssid = "your ssid";
const char* password = "your password";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *whitelights = NULL;
Switch *firelights = NULL;
Switch *rainbowlights = NULL;
Switch *dimlights = NULL;

void setup()
{
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  LEDS.setBrightness(BRI);
  gPal = HeatColors_p;

  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

    // Define your switches here... Max 4... ,Actually more than 4 can be used but alexa will only dicover 4 at a time reliably, so comment out all except 4 during discovery.
    // Basically only have 4 'switches' visable to alexa discovery process at any one time and you'll be fine.
    // Then uncomment them all after discovery dont worry if some show up as offline they'll normally still work just fine as long as youve given them all different port numbers.
    // only down side is if you refresh alexa's wifi settings you have to discover them all again in groups of 4.
    // Format: Alexa invocation name, local port no, on callback, off callback
    whitelights = new Switch("White lights", 80, whiteLightsOn, LightsOff);
    firelights = new Switch("Fire lights", 81, fireLightsOn, LightsOff);
    rainbowlights = new Switch("Rainbow lights", 82, rainbowLightsOn, LightsOff);
    dimlights = new Switch("Night mode", 83, dimLightsOn, dimLightsOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*whitelights);
    upnpBroadcastResponder.addDevice(*firelights);
    upnpBroadcastResponder.addDevice(*rainbowlights);
    upnpBroadcastResponder.addDevice(*dimlights);
  }
}

void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();

    rainbowlights->serverLoop();
    whitelights->serverLoop();
    firelights->serverLoop();
    dimlights->serverLoop();

    //rainbow
    if (rainB == 1) {
      currentMillis = millis();
      if (currentMillis - previousMillis >= rainBinterval) {
        fill_rainbow( leds, NUM_LEDS, gHue, 5);
        FastLED.setBrightness(BRI);
        FastLED.show();
        gHue++;
        previousMillis = currentMillis;
      }
    }
    // end of rainbow

    // FIRE
    if (fire == 1) {
      currentMillis = millis();
      if (currentMillis - previousMillis >= fireinterval) {
        // Array of temperature readings at each simulation cell
        static byte heat[NUM_LEDS];

        // Step 1.  Cool down every cell a little
        for ( int i = 0; i < NUM_LEDS; i++) {
          heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
        }

        // Step 2.  Heat from each cell drifts 'up' and diffuses a little
        for ( int k = NUM_LEDS - 1; k >= 2; k--) {
          heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
        }

        // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
        if ( random8() < SPARKING ) {
          int y = random8(7);
          heat[y] = qadd8( heat[y], random8(160, 255) );
        }

        // Step 4.  Map from heat cells to LED colors
        for ( int j = 0; j < NUM_LEDS; j++) {
          // Scale the heat value from 0-255 down to 0-240
          // for best results with color palettes.
          byte colorindex = scale8( heat[j], 240);
          CRGB color = ColorFromPalette( gPal, colorindex);
          int pixelnumber;
          if ( gReverseDirection ) {
            pixelnumber = (NUM_LEDS - 1) - j;
          } else {
            pixelnumber = j;
          }
          leds[pixelnumber] = color;
        }
        FastLED.setBrightness(BRI);
        FastLED.show();

        previousMillis = currentMillis;
      }
    }
    FastLED.setBrightness(BRI);
    FastLED.show();
  }
}


// White lights

void whiteLightsOn() {
  rainB = 0;
  fire = 0;
  Serial.print("White leds turn on ...");
  fill_solid( leds, NUM_LEDS, CRGB::White);
  FastLED.show();
}

void LightsOff() {
  rainB = 0;
  fire = 0;
  Serial.print("Turn lights turn off ...");
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}


// Dim Lights
void dimLightsOn() {
  Serial.print("dimmer on ...");
  BRI = 75;
}
void dimLightsOff() {
  Serial.print("dimmer off ...");
  BRI = 240;
}


// rainbow lights
void rainbowLightsOn() {
  Serial.print("Rainbow leds turn on ...");
  FastLED.setBrightness(BRI);
  fire = 0;
  rainB = 1;
}


// FIRE

void fireLightsOn()
{
  Serial.print("Fire leds turn on ...");
  FastLED.setBrightness(BRI);
  rainB = 0;
  fire = 1;
}


// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.config(ip, gateway, subnet);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }

  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}
