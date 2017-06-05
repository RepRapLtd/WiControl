#include <ESP8266WiFi.h>

#define ABS_ZERO -273.15  // Celsius
#define TEMP_SENSE_PIN 0   // Analogue pin number

#define THERMISTOR_BETA 3528.0 // thermistor: RS 538-0806
#define THERMISTOR_SERIES_R 1000 // Ohms in series with the thermistors
#define THERMISTOR_25_R 1000.0 // Thermistor ohms at 25 C = 298.15 K
#define AD_RANGE 1023.0 // The A->D converter that measures temperatures gives an int this big as its max value
 
const char* ssid = "reprapltd";
const char* password = "1sgdttsa";
 
int ledPin = D4;
int heatPin = D0;
WiFiServer server(80);
bool debug = true;



float Temperature()
{
  float r = (float)analogRead(TEMP_SENSE_PIN);
  return ABS_ZERO + THERMISTOR_BETA/log( (r*THERMISTOR_SERIES_R/(AD_RANGE - r))/
      ( THERMISTOR_25_R*exp(-THERMISTOR_BETA/(25.0 - ABS_ZERO)) ) );
}
 
void setup() {
  if(debug)
  {
    Serial.begin(115200);
    delay(10);
  }
 
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(heatPin, OUTPUT);
  digitalWrite(heatPin, LOW);
 
  // Connect to WiFi network
  if(debug)
  {
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
  }

  //WiFi.hostname("thermometer");
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if(debug)
      Serial.print(".");
  }

  if(debug)
  {
    Serial.println("");
    Serial.println("WiFi connected");
  }
 
  // Start the server
  server.begin();

  if(debug)
  {
    Serial.println("Server started");
 
    // Print the IP address
    Serial.print("Use this URL : ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
  }
 
}

void control(boolean on)
{
  if(on)
  {
    digitalWrite(ledPin, LOW);
    digitalWrite(heatPin, HIGH);
  } else
  {
    digitalWrite(ledPin, HIGH);
    digitalWrite(heatPin, LOW);
  }
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  //Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  if(debug)
    Serial.println(request);
  client.flush();
 
  // Match the request
 
  if (request.indexOf("/HEAT=ON") != -1) 
  {
    control(true);
  } 
  if (request.indexOf("/HEAT=OFF") != -1)
  {
    control(false);
  }
 
 
 
  // Return the response
/*  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led pin is now: ");
 
  if(value == HIGH) {
    client.print("On");  
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 5 ON<br>");
  client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 5 OFF<br>");*/
  float temp = Temperature();
  client.print(temp);
  client.println();
  if(debug)
  {
    Serial.print("Temperature is: ");
    Serial.println(temp);
  }
  //client.println("<sup>o</sup>C<br>");
//  client.println("</html>");
 
  delay(1);
//  Serial.println("Client disconnected");
//  Serial.println("");
 
}
