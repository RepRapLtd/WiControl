#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "yourSSID";
const char* password = "yourPassword";

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
