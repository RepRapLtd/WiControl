/*

SWA1 Smart Switch Web Server

This turns a SWA1 smart switch into a web server on your network. You can
then load the page it serves in a browser on your PC or phone and turn 
the switch on or off from that page.

RepRap Ltd
https://reprapltd.com


Written by GPT4 (code) and Adrian Bowyer (comments)

3 May 2024

Licence: GPL


**** UNPLUG THE SMART SWITCH FROM THE MAINS FOR ALL BUT THE VERY END OF THIS. ****


Select "Generic ESP8266 Module" from the Arduino IDE Boards menu.

Open the Arduino IDE serial monitor and set the speed to 74880 baud.

Edit the code below to put your network's WiFi SSID and password in.

Dismantle the smart switch (5 Phillips screws on mine). 

The SWA1 Smart Switch has an ESP8266 internally which you can access
through a 5-pin connector on its PCB. (You may have to solder that connector in - it's 
not fitted on all devices.)

The pins are (counting pin 1 as the pin nearest the edge of the PCB):

Pin   GPIO    Function

1     3       Rx (recieve)
2     1       Tx (transmit)
3     -       GND (0v)
4     0       hold 0 to program on power up
5     -       Vdd (+3v3)

Internally, the LED is on GPIO4, the mains relay is on GPIO5 and the push button
grounds GPIO13.   

To program it from the Arduino IDE you will need a TTL-232R-3v3 USB cable.

Connect that as follows

SWA1      TTL-232R-3v3

Rx        Tx (Orange)
Tx        Rx (Yellow)
GND       GND (Black)
GPIO0     RTS (Green)
Vdd       3v3 (Red)

Puting a normally-closed push button in the power (3v3) line will make things
easier. You have to cycle the power as the Arduino IDE starts to program the device.
The sequence is that the IDE compiles the code then pulls RTS low. The ESP8266 has
to be rebooted then to put it in programming mode, so you push the switch for a second
when the IDE says it's trying to connect. You won't get the timing right every go, but it 
usually works. If you don't catch it, the ESP8266 just retains whatever firmware was in
it before. Try again.

When the IDE has completely finished programming the device (don't do this too early) you can
can reboot the ESP8266 by cycling its power again. It should then give a series of dots as it connects 
to your network and then tell you the IP address it's been allocated.

Put that IP address into a web browser and you will get a page that allows you to
turn the switch on and off. Pushing the button on the front of the switch achieves
the same.

Tell your internet hub to always use that IP address for the device then put it
all back together. Plug it into a socket and you have a web-page-controlled mains switch.


*/


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "MY_SSID";
const char* password = "my_password";

ESP8266WebServer server(80);

// Pin definitions
const int relayPin = 5;
const int buttonPin = 13;
const int ledPin = 4;

// Variable to store the relay state
bool relayState = false;

void setup() {
  Serial.begin(74880);
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Set button input mode
  pinMode(ledPin, OUTPUT);

  // Initialize relay and LED state
  digitalWrite(relayPin, relayState);
  digitalWrite(ledPin, relayState);

  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Define server routes
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  checkButton();
}

void checkButton() {
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(buttonPin);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    // Button is pressed; toggle the relay
    relayState = !relayState;
    digitalWrite(relayPin, relayState);
    digitalWrite(ledPin, relayState);
  }
  lastButtonState = currentButtonState;
}

void handleRoot() {
  String page = "<html><head><title>ESP8266 Relay Control</title></head><body><h1>Relay Control</h1><p><a href=\"/toggle\"><button>Toggle Relay</button></a></p></body></html>";
  server.send(200, "text/html", page);
}

void handleToggle() {
  relayState = !relayState;
  digitalWrite(relayPin, relayState);
  digitalWrite(ledPin, relayState);
  // Redirect back to the root page
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}
