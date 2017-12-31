/*
 * Program to run a node on the WiFi Heating Control system.
 * 
 * This makes periodic requests to the server to report its state (on or off,
 * current temperature etc) and to find out what to do next.
 * 
 * 
 * 
 * Adrian Bwowyer
 * RepRap Ltd
 * http://reprapltd.com
 * 
 * 29 December 2017
 */

#include <ESP8266WiFi.h>

#include "local_wifi.h" // Separated to prevent passwords appearing on Github

WiFiClient client;
bool debug = true;

const char* myName = "ElectronicsLab";   // What am I controlling?
const char* pageRoot = "/heating/WiFi/"; // Where the .php scripts are on the server
const char* page = "controllednode.php";  // The script we need
const char* server = "192.168.1.171";    // Server IP address
const char* bodyStart = "<BODY>";
const char* bodyEnd = "</BODY>";
const long loopTime = 10000;             // Milliseconds
const long randomTime = 2000;            // Milliseconds (must be < loopTime)

#define ABS_ZERO -273.15           // Celsius
#define TEMP_SENSE_PIN 0           // Analogue pin number

#define THERMISTOR_BETA 3528.0     // thermistor: RS 538-0806
#define THERMISTOR_SERIES_R 1000.0 // Ohms in series with the thermistors
#define THERMISTOR_25_R 1000.0     // Thermistor ohms at 25 C = 298.15 K
#define AD_RANGE 1023.0            // The A->D converter that measures temperatures gives an int this big as its max value

#define MAXC 80
#define TEMPC 10
char messageString[MAXC];
char tempString[TEMPC];
int messageCount;
int tagCount;
bool inMessage;
long nextTime;

void setup() 
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) 
  {
    ; // wait for serial port to connect.
  }
  
  // Connect to WiFi network
  if(debug)
  {
    Serial.println();
    Serial.println();
    Serial.print("Connecting to WiFi: ");
    Serial.print(ssid);
  }

  WiFi.hostname(myName);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
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
  
  randomSeed(analogRead(TEMP_SENSE_PIN));
  nextTime = (long)millis();
}

// Return the celsius temperature as a text string

char* Temperature()
{
  double r = (double)analogRead(TEMP_SENSE_PIN);
  r = ABS_ZERO + THERMISTOR_BETA/log( (r*THERMISTOR_SERIES_R/(AD_RANGE - r))/
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


// This takes bytes, c, of input and throws them away up to the
// point where the string bodyStart is encountered.  Then they are
// stored in messageString until bodyEnd is encountered.  Note that
// messageString needs to be long enought to hold all the message plus
// strlen(bodyEnd).
// TODO - check this is OK for a null message

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
    if(messageCount >= MAXC)
    {
      messageCount = MAXC - 1;
    }
  } 
}


// This listens to the host and saves the transmitted message in
// messageString.

void GetMessage()
{
  messageCount = 0;
  tagCount = 0;
  inMessage = false;
  while(client.connected())
  {
    char c;
    if(client.available())
    {
      c = client.read();
      NextByte(c);
    }
  }
}


// Send an HTTP request to get the required message
// Any data needing to be transmitted can be placed in
// messageString and will be preceeded by "?"
// If no data is to be transmitted ensure that 
// messageString[0] = 0.

void HTTPRequest()
{
  // Note use of \r\n to force adherence to the HTTP standard.
    
  client.print("GET ");
  client.print(pageRoot);
  client.print(page);
  if(strlen(messageString) > 0)
  {
    client.print("?");
    client.print(messageString);
  }
  client.print(" HTTP/1.0\r\n\r\n");
  client.print("Connection: close\r\n\r\n");  
}


// Connect to the server.  Return true if successful,
// false otherwise.

bool Connect()
{
  if (client.connect(server, 80)) 
  {
    if(debug)
    { 
      Serial.print("Connected to server: ");
      Serial.println(server);
    }
    return true;
  } else 
  {
    if(debug)
    {
      Serial.print("Connection to ");
      Serial.print(server);
      Serial.println(" failed.");
    }
  }
  return false;  
}


// Disconnect from the server

void Disconnect()
{
  if(debug)
  {
    Serial.println();
    Serial.println("disconnecting.\n");
  }
  client.stop();  
}


// Assemble the query part of the HTTP request in messageString

void ComposeQuery()
{
  strcpy(messageString, "location=");
  strcat(messageString, myName);
  strcat(messageString, "&");
  strcat(messageString, "temperature=");
  strcat(messageString, Temperature()); 
}


// Time for the next HTTP request (includes a random component to reduce clashes)

long NextTime()
{
  return (long)millis() + loopTime + random(2*randomTime) - randomTime;
}

// Act on whatever the server has told us to do

void ParseMessage()
{
  
}

void loop()
{
  if((long)millis() - nextTime > 0)
  {
    if(Connect())
    {
      messageString[0] = 0;
      ComposeQuery();
      HTTPRequest();
      GetMessage();
      Disconnect();

      if(debug)
      {
        Serial.print("Recieved: ");
        Serial.println(messageString);
      }
  
      ParseMessage();

      nextTime = NextTime();
    }
  }
}

