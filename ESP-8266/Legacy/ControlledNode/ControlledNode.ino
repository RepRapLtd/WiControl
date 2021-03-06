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
 * 29 December 2017
 * 
 * Licence: GPL
 * 
 * 
 */

#include <ESP8266WiFi.h>

#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
ESP8266WiFiMulti WiFiMulti;

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
 #define ESP8266_LED_PIN 2                    // D4 on later PCBs?
 #define OUTPUT_PIN D3                  //GPIO5
 #define THERMISTOR_BETA 3528.0      // thermistor: RS 538-0806
 #define THERMISTOR_SERIES_R 10000   // Ohms in series with the thermistors
 #define THERMISTOR_25_R 1000.0      // Thermistor ohms at 25 C = 298.15 K
 #define TOP_VOLTAGE 3.303           // The voltage at the top of the series resistor
 #define MAX_AD_VOLTAGE 1.0          // The voltage that gives full-range (i.e. AD_RANGE) on the A->D converter
 #define T_CORRECTION 1.5            // Final fudge to get it just right/variation in beta from spec
 #define DEBUG_PIN D5
#endif

#ifdef WEMOS1
 #define ESP8266_LED_PIN 2                    // D4 on later PCBs?
 #define OUTPUT_PIN D2                  
 #define THERMISTOR_BETA 3528.0      // thermistor: RS 538-0806
 #define THERMISTOR_SERIES_R 1000   // Ohms in series with the thermistors
 #define THERMISTOR_25_R 1000.0      // Thermistor ohms at 25 C = 298.15 K
 #define TOP_VOLTAGE 3.3           // The voltage at the top of the series resistor
 #define MAX_AD_VOLTAGE TOP_VOLTAGE          // The voltage that gives full-range (i.e. AD_RANGE) on the A->D converter
 #define T_CORRECTION 6            // Final fudge to get it just right/variation in beta from spec 
 #define DEBUG_PIN D5                 // Ground this pin to turn debugging on
#endif

const long debugSampleTime = 15000;           // Milliseconds between server requests
const long debugRandomTime = 2000;            // +/- Milliseconds (must be < sampleTime) used to randomise requests to reduce clashes

const long sampleTime = 60000;           // Milliseconds between server requests
const long randomTime = 5000;            // +/- Milliseconds (must be < sampleTime) used to randomise requests to reduce clashes

const long rebootTime = 3600000;           // Milliseconds between resets.

//-----------------------------------------------------------------------------------------------------------

//WiFiClient client;

// Typical HTML response with debugging is about 350 bytes.

#define MAXC 1000     // Maximum bytes in a message string (need a bit of space for debugging info).
#define TEMPC 10      // Maximum bytes in a temperature string

#define BAUD 9600     // Serial comms speed

const char* myName = "Office";   // What am I controlling?
const char* pageRoot = "/WiFiHeating/Workshop/"; // Where the .php script is on the server
const char* page = "controllednode.php";    // The script we need
char* server = "adrianbowyer.com";    // Server IP address/URL
char* backupServer = "192.168.1.171"; // Server IP address/URL
char* currentServer;                        // The one in use

// Bits of HTML we need to know (not case sensitive)

const char* bodyStart = "<BODY>";        // The instructions are in the body of the loaded page
const char* bodyEnd = "</BODY>";
const char* htmlBreak = "<BR>";

#define ABS_ZERO -273.15           // Celsius
#define TEMP_SENSE_PIN 0           // Analogue pin number
#define AD_RANGE 1023.0            // The A->D converter that measures temperatures gives an int this big as its max value

bool debug = true;

// LED control and blinking 

#define OFF 1 // NB LED is inverted
#define ON 0
#define DOT 2
const int dotOn = 100;
const int dotOff = 500;
#define DASH 3
const int dashOn = 500;
const int dashOff = 100;
long nextBlink;
byte blinkPattern;
byte ledState;

// Text and messages

char messageString[MAXC];
char tempString[TEMPC];
int messageCount;
int tagCount;
bool inMessage;
long nextTime;

long nextReset;

void setup() 
{
  // I/O pins...
  
  pinMode(ESP8266_LED_PIN, OUTPUT);
  digitalWrite(ESP8266_LED_PIN, OFF);
  ledState = OFF;
  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, 0);
  pinMode(TEMP_SENSE_PIN, INPUT);
  pinMode(DEBUG_PIN, INPUT_PULLUP);
  debug = !digitalRead(DEBUG_PIN);
  
  // Open serial communications and wait for the port to open
  
  Serial.begin(9600);
  while (!Serial) 
  {
    ; // wait for serial port to connect.
  }
  
  // Identify then connect to WiFi network
  
  if(debug)
  {
    Serial.println();
    Serial.println();
    Serial.print("I am: ");
    Serial.println(myName);
    Serial.print("Connecting to WiFi: ");
    Serial.print(ssid);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);
/*  
  while (WiFiMulti.run() != WL_CONNECTED) 
  {
    delay(500);
    if(debug)
      Serial.print(".");
  }
  if(debug)
  {
    Serial.print("\nConnected to ");
    Serial.println(ssid);
  }
*/
  // Set up timings and LED behaviour
  
  randomSeed(analogRead(TEMP_SENSE_PIN));
  nextTime = (long)millis();
  nextBlink = nextTime;
  nextReset = nextTime + rebootTime;
  blinkPattern = OFF;
}


// Handles LED blink patterns

void Blink()
{
  long now = millis();
  
  if(blinkPattern == OFF || blinkPattern == ON)
  {
    digitalWrite(ESP8266_LED_PIN, blinkPattern);
    ledState = blinkPattern;
    nextBlink = now;
    return; 
  }
  
  if(now - nextBlink < 0)
    return;
  
  if(blinkPattern == DOT)
  {
    if(ledState == ON)
    {
      digitalWrite(ESP8266_LED_PIN, OFF);
      ledState = OFF;
      nextBlink = now + dotOff;
    } else
    {
      digitalWrite(ESP8266_LED_PIN, ON);
      ledState = ON;
      nextBlink = now + dotOn;
    }
    return;
  }

  // The default (DASH) is the error signal
  
  if(ledState == ON)
  {
    digitalWrite(ESP8266_LED_PIN, OFF);
    ledState = OFF;
    nextBlink = now + dashOff;
  } else
  {
    digitalWrite(ESP8266_LED_PIN, ON);
    ledState = ON;
    nextBlink = now + dashOn;
  }
}

// Return the celsius temperature as a text string

char* Temperature()
{
  double r = (double)analogRead(TEMP_SENSE_PIN);
  //Serial.println(r);
  r = T_CORRECTION + ABS_ZERO + THERMISTOR_BETA/log( (r*THERMISTOR_SERIES_R/(AD_RANGE*TOP_VOLTAGE/MAX_AD_VOLTAGE - r))/
      ( THERMISTOR_25_R*exp(-THERMISTOR_BETA/(25.0 - ABS_ZERO)) ) );
  if(r >= 10.0 || r < 0.0) // Assumes temp is always bigger than -9.9 C...
    dtostrf(r, 4, 1, tempString);
  else
    dtostrf(r, 3, 1, tempString);
  return tempString;
}


// This checks a string of characters fed one by one in c to see
// if they match the string tag.  If they do true is
// returned, false otherwise.  Case is ignored.

bool FindTag(char c, const char* tag)
{
  if(toupper(c) == toupper(tag[tagCount]))
  {
    tagCount++;
    if(!tag[tagCount])
    {
      tagCount = 0;
      return true;
    }
  } else
  {
    tagCount = 0;
  }
  return false;
}

// See if a string starts with a tag

bool TagStartsString(const char* tag, char* s)
{
  int i = 0;
  while(tag[i])
  {
    if(toupper(tag[i]) != toupper(s[i]))
      return false;
    i++;
  }
  
  return true;
}


// This takes bytes, c, of input and throws them away up to the
// point where the string bodyStart is encountered.  Then they are
// stored in messageString until bodyEnd is encountered.  Note that
// messageString needs to be long enought to hold all the message plus
// strlen(bodyEnd).

void NextByte(char c)
{
  if(messageCount == 0) // Yet to read any message?
  
  {
    if(FindTag(c, bodyStart)) // No.
    {
      inMessage = true;
      return;
    }
  } else 
  {
    if(FindTag(c, bodyEnd)) // Yes - have read some message
    {
      inMessage = false;
      messageString[messageCount - strlen(bodyEnd) + 1] = 0;
      return;
    }   
  }
   
  if(inMessage)
  {
    messageString[messageCount] = c;
    messageCount++;

    
    if(messageCount >= MAXC - 2)  // Stop buffer overflow
    {
      messageCount = MAXC - 2;
    }
  }
}


// Run through a string replacing HTML "<br>" with "   \n"

void BreakReplace(char* s)
{
  int p0 = 0;

  while(s[p0])
  {
    if(TagStartsString(htmlBreak, &s[p0]))
    {
      for(int i = 0; i < 3; i++)
        s[p0++] = ' ';
      s[p0] = '\n';
    }
    p0++;
  } 
}


// This listens to the host and saves the transmitted message in
// messageString.
/*
void GetMessage()
{
  messageCount = 0;
  messageString[messageCount] = 0;
  tagCount = 0;
  inMessage = false;



  
  
  while(client.connected())
  {
    char c;
    if(client.available())
    {
      c = client.read();
      messageString[messageCount]= c;
      messageCount++;
      if(messageCount >= MAXC - 2) // Prevent buffer overflow
        messageCount = MAXC - 2;
      //NextByte(c);
    }
  }

  // Parse the message; this reduces its length, so it should be OK
  // to have it overwrite itself.
  
  messageString[messageCount] = 0;
  messageCount = 0;
  int mc = 0;
  while(messageString[mc])
  {
    NextByte(messageString[mc]);
    mc++;
  }

  if(debug)
  {
    BreakReplace(messageString);
    Serial.print("\nReceived: \n");
    Serial.println(messageString);
    Serial.println();
  }
}

*/

// Send an HTTP request to get the required message
// Any data needing to be transmitted can be placed in
// messageString and will be preceeded by "?"
// If no data is to be transmitted ensure that 
// messageString[0] = 0.

/*

void HTTPRequest(char* request)
{

  client.print(request);
  
  // Note use of \r\n to force adherence to the HTTP standard.
    
  client.print("GET http://");
  client.print(currentServer);
  client.print(pageRoot);
  client.print(page);
  if(messageString[0] != 0)
  {
    client.print("?");
    client.print(messageString);
  }
  client.print(" HTTP/1.0\r\n\r\n");
  client.print("Connection: close\r\n\r\n"); 
   
}
*/

// Connect to the server.  Return true if successful,
// false otherwise.

/*
bool Connect()
{
  int k;
  if (k = client.connect(server, 80)) 
  {
    currentServer = server;
    if(debug)
    { 
      Serial.print("\n>>>Connected (");
      Serial.print(k); 
      Serial.print(") to server: ");
      Serial.println(currentServer);
    }
    return true;
  } else 
  {
    if(debug)
    {
      Serial.print("\nConnection to ");
      Serial.print(server);
      Serial.println(" failed.");
    }
    if (client.connect(backupServer, 80)) 
    {
      currentServer = backupServer;
      if(debug)
      { 
        Serial.print("\n>>>Connected to server: ");
        Serial.println(currentServer);
      }
      return true;
    } else 
    {
      if(debug)
      {
        Serial.print("\nConnection to ");
        Serial.print(backupServer);
        Serial.println(" failed.");
      }
    }
  }
  return false;  
}

*/

/*
// Disconnect from the server

void Disconnect()
{
  if(debug)
  {
    Serial.println("<<<Disconnecting.");
  }
  client.stop();  
}

*/
// Assemble the query part of the HTTP request in messageString

void ComposeQuery()
{
  // Note use of \r\n to force adherence to the HTTP standard.

  //strcpy(messageString, "GET http://");
  strcpy(messageString, "http://");
  strcat(messageString, currentServer);
  strcat(messageString, pageRoot);
  strcat(messageString, page);
  strcat(messageString, "?");
  strcat(messageString, "location=");
  strcat(messageString, myName);
  strcat(messageString, "&");
  strcat(messageString, "temperature=");
  strcat(messageString, Temperature());
  if(debug)
  {
    strcat(messageString, "&");
    strcat(messageString, "debugOn=1");
  }
  //strcat(messageString, " HTTP/1.0\r\n\r\n");
  //strcat(messageString, "Connection: close\r\n\r\n"); 
}


// Time for the next HTTP request (includes a random component to reduce clashes)

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

// Act on whatever the server has told us to do

void ParseMessage()
{     
  if(TagStartsString("OFF", messageString))
  {
    if(debug)
       Serial.println("Switching off");
    blinkPattern = OFF;
    digitalWrite(OUTPUT_PIN, 0);
    return;
  }

  if(TagStartsString("ON", messageString))
  {
    if(debug)
      Serial.println("Switching on");
    blinkPattern = ON;
    digitalWrite(OUTPUT_PIN, 1);
    return;
  }

  if(debug)
      Serial.println("Error! Message wasn't ON or OFF!");
      
  blinkPattern = DASH;
}

void loop()
{
  if((long)millis() - nextReset > 0)
    ESP.restart();
  
  if((long)millis() - nextTime > 0)
  {
    if(debug)
      Serial.println("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

    while (WiFiMulti.run() != WL_CONNECTED) 
    {
      delay(500);
      if(debug)
        Serial.print(".");
    }
    if(debug)
    {
      Serial.print("\nConnected to ");
      Serial.println(ssid);
    }

    HTTPClient http;
    blinkPattern = OFF;
    messageString[0] = 0;
    currentServer = server;
    ComposeQuery();
    if(debug)
    {
      Serial.println("Sending:");
      Serial.println(messageString);
      Serial.println();
    }
    http.begin(messageString);

    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) 
    {
      // HTTP header has been send and Server response header has been handled
      if(debug) Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) 
      {
        String payload = http.getString();
        if(debug) Serial.println(payload);
        strcpy(messageString, payload.c_str());
        int mc = 0;
        while(messageString[mc])
        {
          NextByte(messageString[mc]);
          mc++;
        }
        if(debug)
        {
          BreakReplace(messageString);
          Serial.print("\nReceived: \n");
          Serial.println(messageString);
          Serial.println();
        }
        ParseMessage();
      }
    } else 
    {
      if(debug) Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      blinkPattern = DASH;
    }

    http.end();

/*
    
    if(Connect())
    {
      blinkPattern = OFF;
      messageString[0] = 0;
      ComposeQuery();
      if(debug)
      {
        Serial.println("Sending:");
        Serial.println(messageString);
        Serial.println();
      }
      HTTPRequest(messageString);
      
      GetMessage();
      Disconnect();
  
      ParseMessage();

      nextTime = NextTime();
    } else
    {
      blinkPattern = DASH;
    }  
*/        
    if(debug)
      Serial.println("------------------------------------------------------------");
      
    nextTime = NextTime();     
  }

  Blink();

  debug = !digitalRead(DEBUG_PIN);
}

