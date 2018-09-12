#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
//#include <FastLED.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
// NETWORK: Static IP details...
IPAddress ip(192, 168, 0, 206); //static ip of WeMos D1 Mini
IPAddress gateway(192, 168, 0, 100); // ip of router/access point
IPAddress subnet(255, 255, 255, 0); // subnet of router/access point
//#define LED_PIN     14 // 5th pin on Sonoff (furthest from button)
//#define LED_TYPE    WS2812B
//#define COLOR_ORDER GRB
//const uint8_t NUM_LEDS  = 150;     // number of leds in string
//const uint8_t BRIGHTNESS = 240;   // this is half brightness
//CRGB leds[NUM_LEDS];              // create array of leds (size of NUM_LEDS)
const int relayPin = 12;
const int gpio13Led = 13;

// prototypes
boolean connectWifi();

//on/off callbacks 
void turnOnRelay();
void turnOffRelay();

// Change this before you flash
const char* ssid = "Your ssid";
const char* password = "your password";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *tvlight = NULL;

void setup()
{
  //LEDS.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds,NUM_LEDS);
 // FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);

    // preparing GPIOs
  pinMode(relayPin, OUTPUT);
  pinMode(gpio13Led, OUTPUT);
  digitalWrite(gpio13Led, HIGH);
  digitalWrite(relayPin, LOW);

   
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
       tvlight = new Switch("TV light", 85, turnOnRelay, turnOffRelay);

    Serial.println("Adding switches upnp broadcast responder");
      upnpBroadcastResponder.addDevice(*tvlight);
  }
}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      
      tvlight->serverLoop();
       }
}


// Tv Light

void turnOnRelay() {
 digitalWrite(relayPin, HIGH); // turn on relay
 digitalWrite(gpio13Led, LOW);
}

void turnOffRelay() {
  digitalWrite(relayPin, LOW);  // turn off relay
  digitalWrite(gpio13Led, HIGH);
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
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
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
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
