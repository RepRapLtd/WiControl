/*
 * Simple thermostat. Also reports what it is doing as a web page.
 * Adrian Bowyer
 * RepRap Ltd
 * http://reprapltd.com
 * 
 * 12 April 2018
 * 
 * Select 
 * 
 *   Board: WeMos D1 R1 
 * 
 * to compile.
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//-----------------------------------------------------------------------------------------------------

// User configuration area


/*

The file local_wifi.h included below should contain two lines:

const char* ssid = "-------";      // The name of your WiFi network
const char* password = "--------"; // Your WiFi network's password

*/

#include "local_wifi.h"


#define WIFIBOARD-V2

#ifdef WIFIBOARD-V2
 #define LEDPIN 2                    // D4 on later PCBs?
 #define HEATPIN D3                  //GPIO5
 #define THERMISTOR_BETA 3528.0      // thermistor: RS 538-0806
 #define THERMISTOR_SERIES_R 10000   // Ohms in series with the thermistors
 #define THERMISTOR_25_R 1000.0      // Thermistor ohms at 25 C = 298.15 K
 #define TOP_VOLTAGE 3.303           // The voltage at the top of the series resistor
 #define MAX_AD_VOLTAGE 1.0          // The voltage that gives full-range (i.e. AD_RANGE) on the A->D converter
 #define T_CORRECTION 1.5            // Final fudge to get it just right/variation in beta from spec 
#endif

float switchTemperature = 25.0;      // The temperature at which to swich on or off the load
bool cooling = true;                 // true: switch load on for high temperatures; false: switch it off
bool debug = true;                   // Set false to turn of diagnostics
const unsigned long checkTime=10000; // Milliseconds - check the temperature this often

//-----------------------------------------------------------------------------------------------------

#define ABS_ZERO -273.15  // Celsius
#define TEMP_SENSE_PIN A0   // Analogue pin number
#define AD_RANGE 1023.0 // The A->D converter that measures temperatures gives an int this big as its max value


unsigned long lastTime;
int ledPin = LEDPIN;
int heatPin = HEATPIN;

ESP8266WebServer server(80);

float lastTemperature = ABS_ZERO;

#define MSG_LEN 2056
char message[MSG_LEN];

#define MAX_RING 20
float tempRing[MAX_RING];
int ringPointer = 0;

float Temperature()
{
  float r = (float)analogRead(TEMP_SENSE_PIN);
  //Serial.println(r);
  r = T_CORRECTION + ABS_ZERO + THERMISTOR_BETA/log( (r*THERMISTOR_SERIES_R/(AD_RANGE*TOP_VOLTAGE/MAX_AD_VOLTAGE - r))/
      ( THERMISTOR_25_R*exp(-THERMISTOR_BETA/(25.0 - ABS_ZERO)) ) );
  lastTemperature = r;
  return r;
}

void control(boolean hot)
{
  if(hot == cooling)
  {
    digitalWrite(ledPin, LOW);
    digitalWrite(heatPin, HIGH);
    if(debug) Serial.println("On");
  } else
  {
    digitalWrite(ledPin, HIGH);
    digitalWrite(heatPin, LOW);
    if(debug) Serial.println("Off");
  }
}
 

void handleRoot() 
{
  digitalWrite(ledPin, 0);

  int i = ringPointer;
  int count = 0;
  char *cur = message;
  const char *end = message + sizeof(message);
  if(cooling)
    cur += snprintf(cur, end-cur, "Cooling. Switch-off temperature is: %f\n", switchTemperature);
  else
    cur += snprintf(cur, end-cur, "Heating. Switch-on temperature is: %f\n", switchTemperature);
  cur += snprintf(cur, end-cur, "The last %d temperatures (taken every %f seconds) were:\n\n", MAX_RING, 0.001*(float)checkTime);
  while(count < MAX_RING && cur < end)
  {
    cur += snprintf(cur, end-cur, "%f C\n", tempRing[i]);
    i++;
    if(i >= MAX_RING)
      i = 0;
    count++;
  }
  server.send(200, "text/plain", message);
  digitalWrite(ledPin, 1);
}

void handleNotFound(){
  digitalWrite(ledPin, 0);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(ledPin, 1);
}

void setup(void)
{
  if(debug)
  {
    Serial.begin(9600);
     while (!Serial); // wait for serial port to connect. Needed for Leonardo only
  }

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(heatPin, OUTPUT);
  digitalWrite(heatPin, LOW);
  pinMode(TEMP_SENSE_PIN, INPUT);
  //wdt_enable(WDTO_8S);

  for(ringPointer = 0; ringPointer < MAX_RING; ringPointer++)
    tempRing[ringPointer] = ABS_ZERO;
  ringPointer = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println();

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) 
  {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

/*  server.on("/inline", []()
  {
    server.send(200, "text/plain", "this works as well");
  });*/

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  
  lastTime = millis();
}

void loop(void)
{

  if(millis() - lastTime >= checkTime)
  {
    //Serial.println(lastTime);
    float t = Temperature();
    tempRing[ringPointer] = t;
    ringPointer++;
    if(ringPointer >= MAX_RING)
      ringPointer = 0;
    
  
    if(debug)
    {
      Serial.print("Temp: ");
      Serial.println(t);
    }
    
    if (t > switchTemperature) 
    {
      control(true);
    } else
    {
      control(false);
    }
    lastTime = millis();
  }

  server.handleClient();
}





 

