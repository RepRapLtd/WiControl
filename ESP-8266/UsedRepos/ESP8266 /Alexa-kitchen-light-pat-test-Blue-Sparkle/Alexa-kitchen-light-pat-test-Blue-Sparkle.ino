#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"

// NETWORK: Static IP details...change these to your choice
IPAddress ip(192, 168, 0, 215); //static ip of WeMos D1 Mini
IPAddress gateway(192, 168, 0, 100); // ip of router/access point
IPAddress subnet(255, 255, 255, 0); // subnet of router/access point

// Some Settings
#define LED_PIN     D4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
const int Wifiled = D7;
const uint8_t NUM_LEDS  = 64;     // NUMBER OF LEDS IN STRING ******************
int BRI = 210;   // initial brightness
int rainB = 0;
int gHue = 5;
int Beats = 0;
int Spark = 0;
CRGB leds[NUM_LEDS];
const long rainBinterval = 22;
unsigned long currentMillis = millis();
unsigned long previousMillis = millis();


//Juggle Specific variables
CRGBPalette16 currentPalette;                                 // Use palettes instead of direct CHSV or CRGB assignments
CRGBPalette16 targetPalette;                                  // Also support smooth palette transitioning
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND

// prototypes
boolean connectWifi();

//on/off callbacks
void BlueLightsOn();
void LightsOff();
void BeatsLightsOn();
void rainbowLightsOn();
void SparkleLightsOn();

// Change this before you flash
const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *Bluekitchen = NULL;
Switch *Funkykitchen = NULL;
Switch *rainbowkitchen = NULL;
Switch *SparkleLight = NULL;


void setup()
{
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  LEDS.setBrightness(BRI);
  //set_max_power_in_volts_and_milliamps(5, 6000);              // FastLED power management --If you need it :)
  pinMode(Wifiled, OUTPUT);
  digitalWrite(Wifiled, LOW);
  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

    // Define your switches here... Max 4... ,Actually more than 4 can be used but alexa will only dicover 4 at a time reliably, so comment out all except 4 during discovery.
    // Basically only have 4 'switches' visable to alexa discovery process at any one time and you'll be fine.
    // Then uncomment them all after discovery dont worry if some show up as offline they'll still work just fine as long as youve given them all different port numbers.
    // only down side is if you refresh alexa's wifi settings you have to discover them all again in groups of 4.
    // Format: Alexa invocation name, local port no, on callback, off callback
    Bluekitchen = new Switch("Blue Lights", 86, BlueLightsOn, LightsOff);
    Funkykitchen = new Switch("funky Lights ", 87, BeatsLightsOn, LightsOff);
    rainbowkitchen = new Switch("Rainbow Lights ", 88, rainbowLightsOn, LightsOff);
    SparkleLight = new Switch("Sparkles Lights", 89, SparkleLightsOn, LightsOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*Bluekitchen);
    upnpBroadcastResponder.addDevice(*Funkykitchen);
    upnpBroadcastResponder.addDevice(*rainbowkitchen);
    upnpBroadcastResponder.addDevice(*SparkleLight);

  }
}

void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();

    rainbowkitchen->serverLoop();
    Bluekitchen->serverLoop();
    Funkykitchen->serverLoop();
    SparkleLight->serverLoop();


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

    // Beats / pulse
    if (Beats == 1) {
      EVERY_N_MILLISECONDS( 20 ) {
        gHue++;  // slowly cycle the "base color" through the rainbow
      }
      Pulse();
      FastLED.setBrightness(BRI);
      FastLED.show();
    }
    if (Spark == 1) {
      Sparkles();
      FastLED.setBrightness(BRI);
      FastLED.show();
    }
  }
}


// Beats/ Pulse voids

void Pulse()
{
  rainB = 0;
  Beats = 1;
  Spark = 0;
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}



// Blue lights

void BlueLightsOn() {
  rainB = 0;
  Beats = 0;
  Spark = 0;
  Serial.print("White leds turn on ...");
  fill_solid( leds, NUM_LEDS, CRGB::DeepSkyBlue);
  FastLED.show();
  fill_solid( leds, NUM_LEDS, CRGB::DeepSkyBlue);
  FastLED.show();
}

void LightsOff() {
  rainB = 0;
  Beats = 0;
  Spark = 0;
  Serial.print("Turn lights turn off ...");
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}



// rainbow lights
void rainbowLightsOn() {
  Serial.print("Rainbow leds turn on ...");
  Beats = 0;
  rainB = 1;
  Spark = 0;
}

void Sparkles()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

// Beats

void BeatsLightsOn()
{
  currentPalette  = CRGBPalette16(CRGB::Black);
  targetPalette   = RainbowColors_p;
  currentBlending = LINEARBLEND;
  Serial.print("Beats leds turn on ...");
  rainB = 0;
  Beats = 1;
  Spark = 0;
}

// Sparkle lights on
void SparkleLightsOn() {
  Beats = 0;
  rainB = 0;
  Spark = 1;
  Serial.print("Sparkle leds turn on ...");
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
    digitalWrite(Wifiled, HIGH);
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
    digitalWrite(Wifiled, LOW);
  }

  return state;
}