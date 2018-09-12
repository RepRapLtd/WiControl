

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
//#include <functional>
#include "Switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

const char* myName = "testswitch";

const int relayPin = D1;
const int led = 2;

const char* ssid = "reprapltd";
const char* password = "1sgdttsa";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *whitekitchen = NULL;


void switchOn() {
  Serial.println("turn on ...");
  digitalWrite(led, 0);
}

void switchOff() {
  Serial.println("turn off ...");
  digitalWrite(led, 1);
}


// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.hostname(myName);
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


void setup()
{
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);
  pinMode(led, OUTPUT);
  digitalWrite(led, 1);
  Serial.println("Starting...");

  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

    // Define your switches here... Max 4... ,Actually more than 4 can be used but alexa will only dicover 4 at a time reliably, so comment out all except 4 during discovery.
    // Basically only have 4 'switches' visable to alexa discovery process at any one time and you'll be fine.
    // Then uncomment them all after discovery dont worry if some show up as offline they'll normally still work just fine as long as youve given them all different port numbers.
    // only down side is if you refresh alexa's wifi settings you have to discover them all again in groups of 4.
    // Format: Alexa invocation name, local port no, on callback, off callback
    whitekitchen = new Switch("White kitchen", 86, switchOn, switchOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(whitekitchen);
  } else
  {
    Serial.println("WiFi not connected.");
  }
}

void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();

    whitekitchen->serverLoop();
  }
}



