/*
 * Program to run a node on the WiFi Heating Control system.
 * 
 * This makes periodic requests to the server to report its state (on or off,
 * current temperature etc) and to find out what to do next.
 * 
 * The general form of a request is (N.B. "/" caracters should be in the strings):
 * 
 * GET http://currentServer/pageRoot/page/?messageString
 * 
 * where messageString is something like
 * 
 * location=Office&temperature=20&debugOn=1
 * 
 * Select 
 * 
 *   Board: WeMos D1 R1 
 * 
 * to compile.
 * 
 * Adrian Bwowyer
 * RepRap Ltd
 * http://reprapltd.com
 * 
 * Version 2
 * 5 September 2018
 * 
 * Licence: GPL
 * 
 * 
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

//-----------------------------------------------------------------------------------------------------

// User configuration area


/*

The file local_wifi.h included below should contain two lines:

const char* ssid = "-------";      // The name of your WiFi network
const char* password = "--------"; // Your WiFi network's password

*/

#include "local_wifi.h" // Separated to prevent passwords appearing on Github

#define WIFIBOARD-V2
//#define WEMOS1

#ifdef WIFIBOARD-V2
 #define ESP8266_LED_PIN 2           // ESP8266 internal LED; D4 on later PCBs?
 #define USER_LED_PIN D6             // GPIO 12 - Front panel LED
 #define OUTPUT_PIN D3               // GPIO5 This is the switched MOSFET/relay
 #define THERMISTOR_BETA 3528.0      // thermistor: RS 538-0806
 #define THERMISTOR_SERIES_R 10000   // Ohms in series with the thermistor
 #define THERMISTOR_25_R 1000.0      // Thermistor ohms at 25 C = 298.15 K
 #define TOP_VOLTAGE 3.303           // The voltage at the top of the series resistor
 #define MAX_AD_VOLTAGE 1.0          // The voltage that gives full-range (i.e. AD_RANGE - see below) on the A->D converter
 #define T_CORRECTION 1.5            // Final fudge to get it just right/variation in beta from spec
 #define DEBUG_PIN D5                // Ground this pin to turn debugging on
#endif

#ifdef WEMOS1
 #define ESP8266_LED_PIN 2            // D4 on later PCBs?
 #define USER_LED_PIN D6              // GPIO 12 - Front panel LED
 #define OUTPUT_PIN D2                // This is the switched MOSFET/relay                  
 #define THERMISTOR_BETA 3528.0       // thermistor: RS 538-0806
 #define THERMISTOR_SERIES_R 1000     // Ohms in series with the thermistor
 #define THERMISTOR_25_R 1000.0       // Thermistor ohms at 25 C = 298.15 K
 #define TOP_VOLTAGE 3.3              // The voltage at the top of the series resistor
 #define MAX_AD_VOLTAGE TOP_VOLTAGE   // The voltage that gives full-range (i.e. AD_RANGE - see below) on the A->D converter
 #define T_CORRECTION 6               // Final fudge to get it just right/variation in beta from spec 
 #define DEBUG_PIN D5                 // Ground this pin to turn debugging on
#endif

const long debugSampleTime = 15000;   // Milliseconds between server requests when debugging
const long debugRandomTime = 2000;    // +/- Milliseconds (must be < sampleTime) used to randomise requests to reduce clashes

const long sampleTime = 60000;        // Milliseconds between server requests
const long randomTime = 5000;         // +/- Milliseconds (must be < sampleTime) used to randomise requests to reduce clashes

const long rebootTime = 3600000;      // Milliseconds between resets.

#define BAUD 9600     // Serial comms speed

const int version = 2;
const String myName = "ChemistryLab";                   // What am I controlling?
const String pageRoot = "/WiFiHeating/Workshop/"; // Where the .php script is on the server
const String page = "controllednode.php";         // The script we need
String server = "adrianbowyer.com";               // Server IP address/URL
String backupServer = "192.168.1.171";            // Backup server IP address/URL
String currentServer = server;                    // The one in use

//-----------------------------------------------------------------------------------------------------------


// Bits of HTML we need to know (both cases of these are tried in atempting matches)

const String bodyStart = "<body>";        // The instructions are in the body of the loaded page
const String bodyEnd = "</body>";
const String htmlBreak = "<br>";

// The thermistor for measuring temperature

#define ABS_ZERO -273.15           // Celsius
#define TEMP_SENSE_PIN 0           // Analogue pin number
#define AD_RANGE 1023.0            // The A->D converter that measures temperatures gives an int this big as its max value

// This is turned off and on when running by freeing/grounding DEBUG_PIN

bool debug = true;

// LED control and blinking 

#define OFF 1 // N.B. ESP8266 LED is inverted
#define ON 0
#define DOT 2
const int dotOn = 100;
const int dotOff = 500;
#define DASH 3
const int dashOn = 500;
const int dashOff = 100;
#define FLASH 4
const int flashOn = 100;
const int flashOff = 100;
long nextBlink;
byte blinkPattern;
byte ledState;

// Text and messages

String message = "";

// Timing and resets

long nextTime;
long nextReset;

ESP8266WiFiMulti WiFiMulti;

void setup() 
{
   // I/O pins...
  
  pinMode(ESP8266_LED_PIN, OUTPUT);
  pinMode(USER_LED_PIN, OUTPUT);
  digitalWrite(ESP8266_LED_PIN, OFF);
  digitalWrite(USER_LED_PIN, !OFF);
  ledState = OFF;
  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, 0);
  pinMode(TEMP_SENSE_PIN, INPUT);
  pinMode(DEBUG_PIN, INPUT_PULLUP);
  debug = !digitalRead(DEBUG_PIN);

  Serial.begin(BAUD);

  while(!Serial);

  if(debug)
  {
    Serial.println();
    Serial.println();
    Serial.print("I am: ");
    Serial.println(myName);
    Serial.print("Connecting to WiFi: ");
    Serial.print(ssid);
  }

  // Needed for WiFi stability
  
  delay(4000);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  // Set up timings and LED behaviour
  
  randomSeed(analogRead(TEMP_SENSE_PIN));
  nextTime = (long)millis();
  nextBlink = nextTime;
  nextReset = nextTime + rebootTime;
  blinkPattern = OFF;

}


// Amuse the user with blinking lights

void Leds(int onOff)
{
  digitalWrite(ESP8266_LED_PIN, onOff);
  digitalWrite(USER_LED_PIN, !onOff);
  ledState = onOff;
}

void Blink()
{
  long now = millis();
  
  if(blinkPattern == OFF || blinkPattern == ON)
  {
    Leds(blinkPattern);
    nextBlink = now;
    return; 
  }
  
  if(now - nextBlink < 0)
    return;

  switch(blinkPattern)
  {
    case DOT:
      if(ledState == ON)
      {
        Leds(OFF);
        nextBlink = now + dotOff;
      } else
      {
        Leds(ON);
        nextBlink = now + dotOn;
      }
      break;
      
    case FLASH:
      if(ledState == ON)
      {
        Leds(OFF);
        nextBlink = now + flashOff;
      } else
      {
        Leds(ON);
        nextBlink = now + flashOn;
      }
      break;
      
    case DASH:
    default:
      if(ledState == ON)
      {
        Leds(OFF);
        nextBlink = now + dashOff;
      } else
      {
        Leds(ON);
        nextBlink = now + dashOn;
      }    
  }
}


// Return the celsius temperature as a text string

String Temperature()
{
  double r = (double)analogRead(TEMP_SENSE_PIN);
  //Serial.println(r);
  r = T_CORRECTION + ABS_ZERO + THERMISTOR_BETA/log( (r*THERMISTOR_SERIES_R/(AD_RANGE*TOP_VOLTAGE/MAX_AD_VOLTAGE - r))/
      ( THERMISTOR_25_R*exp(-THERMISTOR_BETA/(25.0 - ABS_ZERO)) ) );
  return String(r, 1);
}


// Assemble the HTTP request in the message String

void ComposeQuery()
{
  message = "http://";
  message.concat(currentServer);
  message.concat(pageRoot);
  message.concat(page);
  message.concat("?location=");
  message.concat(myName);
  message.concat("&temperature=");
  message.concat(Temperature());
  if(debug)
  {
    message.concat("&debugOn=1");
  }
}


// Decide the time delay before the next server request.  This is usually quick (~15s) for debugging
// (so you can see what's going on), or slow (~60s) for normal operation.

long NextTime()
{
  long f;
  if(debug)
    f = debugSampleTime + random(2*debugRandomTime) - debugRandomTime;
  else
    f = sampleTime + random(2*randomTime) - randomTime;
    
  if(debug)
  {
    Serial.print("Next server request will be in ");
    Serial.print(f);
    Serial.println(" milliseconds.");
  }
  return (long)millis() + f;
}


// Print the HTML body returned from the server on Serial output replacing
// HTML controls with readable stuff.

void PrintWebPageReturned(int bod)
{
  int end = message.indexOf(bodyEnd);
  String temp;
  
  if(end < 0)
  {
    temp = bodyEnd;
    temp.toUpperCase();
    end = message.indexOf(temp);
    if(end < 0)
    {
      blinkPattern=DASH;
      if(debug)
      {
        Serial.print(bodyEnd);
        Serial.print(" not found in ");
        Serial.println(message);
      }
    }
  }

  // Pull out the body of the message (note this overwrites message)
  
  if(end < 0)
    message = message.substring(bod);
  else
    message = message.substring(bod, end);

  // Replace <br> with \n
  
  temp = htmlBreak;
  if(message.indexOf(temp) < 0)
    temp.toUpperCase();
  message.replace(temp, "\n");

  Serial.println("\n--------");
  Serial.println(message);
  Serial.println("--------\n");  
}


// Turn the load (central heating valve, or whatever) on or off

void SwitchOnOrOff(bool on)
{
  if(on)
  {
   if(debug)
       Serial.println("Switching on.");
    blinkPattern = ON;
    digitalWrite(OUTPUT_PIN, 1);
    return;    
  }

  if(debug)
    Serial.println("Switching off.");
  blinkPattern = OFF;
  digitalWrite(OUTPUT_PIN, 0);  
}


// Go through the body of the HTML returned from the server and decide what
// action it's instructing us to take.

void ParseMessage()
{
  int bod = message.indexOf(bodyStart);
  
  if(bod < 0)
  {
    String temp = bodyStart;
    temp.toUpperCase();
    bod = message.indexOf(temp);
    if(bod < 0)
    {
      blinkPattern=DASH;
      if(debug)
      {
        Serial.print(bodyStart);
        Serial.print(" not found in ");
        Serial.println(message);
      }
      return;
    }
  }

  bod += bodyStart.length();
  int toDo = message.indexOf("ON", bod);

  if(toDo < 0)
  {
    toDo = message.indexOf("OFF", bod);
    if(toDo < 0)
    {
      blinkPattern=DASH;
      if(debug)
      {
        Serial.print("Neither ON nor OFF found in ");
        Serial.println(message);
      }
      return;     
    }
    SwitchOnOrOff(false);
  } else
  {
    SwitchOnOrOff(true);
  }

  if(debug)
    PrintWebPageReturned(bod);
}


// What it says...

void loop() 
{
  // Check for watchdog reset time
  
    if((long)millis() - nextReset > 0)
    {
      if(debug)
        Serial.println("\n\n*** Watchdog reset.\n");
      delay(2000); // Allow the serial output buffer to flush
      ESP.restart();
    }

  // If the HTTP request will be in 1 second, flash the LEDs...
  
  if((1000 + (long)millis()) - nextTime > 0)
    blinkPattern=FLASH;

  // Time for another HTTP request?
  
  if((long)millis() - nextTime > 0)
  {
    // wait for WiFi connection
    if ((WiFiMulti.run() == WL_CONNECTED)) 
    {
      if(debug)
        Serial.println();
  
      HTTPClient http;
  
      ComposeQuery();
  
      if(debug)
      {
        Serial.print("\nSending: ");
        Serial.println(message);
      }
   
      http.begin(message);
      
      // start connection and send HTTP header
      
      int httpCode = http.GET();
  
      // httpCode will be negative on error
      
      if (httpCode > 0) 
      {
        // HTTP header has been send and Server response header has been handled
        
        if(debug)
          Serial.printf("HTTP GET returned code: %d\n", httpCode);
  
        // file found at server
        
        if (httpCode == HTTP_CODE_OK) 
        {
          message = http.getString();
          ParseMessage();
        } else
        {
          blinkPattern = DASH;
          if(debug)
            Serial.println("\nReturned code not HTTP_CODE_OK");
        }
      } else 
      {
        blinkPattern = DASH;
        if(debug)
          Serial.printf("\nHTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
  
      http.end();
      
    } else
    {
      if(debug)
        Serial.println("\nNot WL_CONNECTED.");    
    }
  
    nextTime = NextTime();
  }

  debug = !digitalRead(DEBUG_PIN);

  // Entertain the user
  
  Blink();

}

