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
const uint8_t NUM_LEDS  = 30;     // number of leds in string
int BRI = 210;   // initial brightness
int rainB = 0;
int gHue = 5;

CRGB leds[NUM_LEDS];
unsigned long previousMillis = 0;
const long interval = 350;


// prototypes
boolean connectWifi();

//on/off callbacks
void whiteLightsOn();
void whiteLightsOff();
void redLightsOn();
void redLightsOff();
void rainbowLightsOn();
void rainbowLightsOff();
void dimLightsOn();
void dimLightsOff();

// Change this before you flash
const char* ssid = "Your ssid";
const char* password = "your password";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *whitelights = NULL;
Switch *redlights = NULL;
Switch *rainbowlights = NULL;
Switch *dimlights = NULL;

void setup()
{
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  LEDS.setBrightness(120);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
    whitelights = new Switch("White lights", 80, whiteLightsOn, whiteLightsOff);
    redlights = new Switch("Red lights", 81, redLightsOn, redLightsOff);
    rainbowlights = new Switch("Rainbow lights", 82, rainbowLightsOn, rainbowLightsOff);
    dimlights = new Switch("Night mode", 83, dimLightsOn, dimLightsOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*whitelights);
    upnpBroadcastResponder.addDevice(*redlights);
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
    redlights->serverLoop();
    dimlights->serverLoop();

    //rainbow
    if (rainB == 1) {
      unsigned long currentMillis = millis();
      fill_rainbow( leds, NUM_LEDS, gHue, 7);
      FastLED.setBrightness(BRI);
      FastLED.show();
      gHue++;
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
      }
    }
    // end of rainbow
    FastLED.setBrightness(BRI);
    FastLED.show();
   }
}

// White lights

void whiteLightsOn() {
  rainB = 0;
  unsigned long currentMillis = millis();
  Serial.print("White leds turn on ...");
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
     FastLED.show();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
    }
  }
}

void whiteLightsOff() {
  unsigned long currentMillis = millis();
  Serial.print("White leds turn on ...");
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
    FastLED.show();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
    }
  }
}

// red lights

void redLightsOn() {
  rainB = 0;
  unsigned long currentMillis = millis();
  Serial.print("Red leds turn on ...");
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
    }
  }
}

void redLightsOff() {
  unsigned long currentMillis = millis();
  Serial.print("Red leds turn on ...");
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
    FastLED.show();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
    }
  }
}

// Dim Lights
void dimLightsOn() {
   Serial.print("dimmer on ...");
    BRI = 65;
}
void dimLightsOff() {
   Serial.print("dimmer off ...");
    BRI = 210;
}



// rainbow lights
void rainbowLightsOn() {
  unsigned long currentMillis = millis();
  Serial.print("Rainbow leds turn on ...");
  FastLED.setBrightness(BRI);
  rainB = 1;
}


void rainbowLightsOff() {
  rainB = 0;
  unsigned long currentMillis = millis();
  Serial.print("Rainbow leds turn off ...");
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
    FastLED.show();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
    }
  }
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
