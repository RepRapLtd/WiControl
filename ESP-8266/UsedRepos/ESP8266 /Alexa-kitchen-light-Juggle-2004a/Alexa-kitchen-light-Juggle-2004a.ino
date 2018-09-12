#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include "FastLED.h"
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);



// NETWORK: Static IP details...change these to your choice
//IPAddress ip(192, 168, 0, 204); //static ip of WeMos D1 Mini
//IPAddress gateway(192, 168, 0, 100); // ip of router/access point
//IPAddress subnet(255, 255, 255, 0); // subnet of router/access point

// Some Settings
#define LED_PIN     4 // D2 on Wemos D1 mini V1
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
const int relayPin = D1;
const uint8_t NUM_LEDS  = 119;     // NUMBER OF LEDS IN STRING ******************
int BRI = 210;   // initial brightness
int rainB = 0;
int gHue = 5;
int Juggle = 0;
CRGB leds[NUM_LEDS];
const long rainBinterval = 22;
const long Juggleinterval = 33;
unsigned long currentMillis = millis();
unsigned long previousMillis = millis();


//Juggle Specific variables
CRGBPalette16 currentPalette;                                 // Use palettes instead of direct CHSV or CRGB assignments
CRGBPalette16 targetPalette;                                  // Also support smooth palette transitioning
TBlendType    currentBlending;                                // NOBLEND or LINEARBLEND
uint8_t    numdots =   4;                                     // Number of dots in use.
uint8_t   thisfade =   2;                                     // How long should the trails be. Very low value = longer trails.
uint8_t   thisdiff =  16;                                     // Incremental change in hue between each dot.
uint8_t    thishue =   0;                                     // Starting hue.
uint8_t     curhue =   0;                                     // The current hue
uint8_t    thissat = 255;                                     // Saturation of the colour.
uint8_t thisbright = 255;                                     // How bright should the LED/display be.
uint8_t   thisbeat =   5;                                     // Higher = faster movement.



// prototypes
boolean connectWifi();

//on/off callbacks
void whiteLightsOn();
void LightsOff();
void JuggleLightsOn();
void rainbowLightsOn();
void kitchenlightsOn();
void kitchenlightsOff();

// Change this before you flash
const char* ssid = "your ssid";
const char* password = "your passowrd";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *whitekitchen = NULL;
Switch *Jugglekitchen = NULL;
Switch *rainbowkitchen = NULL;
Switch *kitchenlights = NULL;

void setup()
{
  Serial.begin(9600);
  LEDS.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  LEDS.setBrightness(BRI);
  //set_max_power_in_volts_and_milliamps(5, 6000);              // FastLED power management --If you need it :)
  pinMode(relayPin, OUTPUT);
  lcd.begin();
  lcd.backlight();

  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

    // Define your switches here... Max 4... ,Actually more than 4 can be used but alexa will only dicover 4 at a time reliably, so comment out all except 4 during discovery.
    // Basically only have 4 'switches' visable to alexa discovery process at any one time and you'll be fine.
    // Then uncomment them all after discovery dont worry if some show up as offline they'll still work just fine as long as youve given them all different port numbers.
    // only down side is if you refresh alexa's wifi settings you have to discover them all again in groups of 4.
    // Format: Alexa invocation name, local port no, on callback, off callback
    whitekitchen = new Switch("White kitchen", 86, whiteLightsOn, LightsOff);
    Jugglekitchen = new Switch("Juggle kitchen", 87, JuggleLightsOn, LightsOff);
    rainbowkitchen = new Switch("Rainbow kitchen", 88, rainbowLightsOn, LightsOff);
    kitchenlights = new Switch("Kitchen lights", 89, kitchenlightsOn, kitchenlightsOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*whitekitchen);
    upnpBroadcastResponder.addDevice(*Jugglekitchen);
    upnpBroadcastResponder.addDevice(*rainbowkitchen);
    upnpBroadcastResponder.addDevice(*kitchenlights);
  }
}

void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();

    rainbowkitchen->serverLoop();
    whitekitchen->serverLoop();
    Jugglekitchen->serverLoop();
    kitchenlights->serverLoop();

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

    // Juggle
    if (Juggle == 1) {
      EVERY_N_MILLISECONDS(100) {                                 // AWESOME palette blending capability provided as required.
        uint8_t maxChanges = 24;
        nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
      }

      ChangeMe();
      juggle_pal();
      FastLED.setBrightness(BRI);
      FastLED.show();

    }
  }
}

// juggle voids

void juggle_pal() {                                           // Several colored dots, weaving in and out of sync with each other

  curhue = thishue;                                           // Reset the hue values.
  fadeToBlackBy(leds, NUM_LEDS, thisfade);

  for ( int i = 0; i < numdots; i++) {
    leds[beatsin16(thisbeat + i + numdots, 0, NUM_LEDS)] += ColorFromPalette(currentPalette, curhue , thisbright, currentBlending); // Munge the values and pick a colour from the palette
    curhue += thisdiff;
  }

}

void ChangeMe() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.

  uint8_t secondHand = (millis() / 1000) % 30;                // IMPORTANT!!! Change '30' to a different value to change duration of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.

  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch (secondHand) {
      case  0: numdots = 1; thisbeat = 20; thisdiff = 16; thisfade = 2; thishue = 0; break;                  // You can change values here, one at a time , or altogether.
      case 10: numdots = 4; thisbeat = 10; thisdiff = 16; thisfade = 8; thishue = 128; break;
      case 20: numdots = 8; thisbeat =  3; thisdiff =  0; thisfade = 8; thishue = random8(); break;         // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
      case 30: break;
    }
  }

}



// White lights

void whiteLightsOn() {
  rainB = 0;
  Juggle = 0;
  Serial.print("White leds turn on ...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  lcd.setCursor(0, 3);
  lcd.print("White LEDS on");
  fill_solid( leds, NUM_LEDS, CRGB::White);
  FastLED.show();
  fill_solid( leds, NUM_LEDS, CRGB::White);
  FastLED.show();
}

void LightsOff() {
  rainB = 0;
  Juggle = 0;
  Serial.print("Turn lights turn off ...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  lcd.setCursor(0, 3);
  lcd.print("All Leds off");
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}


// Relay for Kitchen lights
void kitchenlightsOn() {
  Serial.print("Relay for kitchen lights on ...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  lcd.setCursor(0, 3);
  lcd.print("Kitchen Lights on");
  digitalWrite(relayPin, HIGH);
}

void kitchenlightsOff() {
  Serial.print("Relay for kitchen lights off ...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  lcd.setCursor(0, 3);
  lcd.print("Kitchen lights off");
  digitalWrite(relayPin, LOW);
}


// rainbow lights
void rainbowLightsOn() {
  Serial.print("Rainbow leds turn on ...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  lcd.setCursor(0, 3);
  lcd.print("Rainbow LEDS");
  FastLED.setBrightness(BRI);
  Juggle = 0;
  rainB = 1;
}



// Juggle

void JuggleLightsOn()
{
  currentPalette  = CRGBPalette16(CRGB::Black);
  targetPalette   = RainbowColors_p;
  currentBlending = LINEARBLEND;
  Serial.print("Juggle leds turn on ...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  lcd.setCursor(0, 3);
  lcd.print("Juggle LEDS");
  FastLED.setBrightness(BRI);
  rainB = 0;
  Juggle = 1;
}


// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;

  //  WiFi.config(ip, gateway, subnet);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("connecting");

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
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("connected");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
    lcd.setCursor(0, 2);
    lcd.print("Awaiting command");
    lcd.setCursor(0, 3);
    lcd.print("From Alexa");
    
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connection failed.");
    lcd.setCursor(0, 1);
    lcd.print("Check SSID/Password");
  }

  return state;
}
