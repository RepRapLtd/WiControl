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

const char* myName = "ElectronicsLab";  // What am I controlling?
const char* pageRoot = "/WiFiHeating/"; // Where the .php scripts are on the server
const char* server = "192.168.1.171";   // Server IP address
const char* bodyStart = "<BODY>";
const char* bodyEnd = "</BODY>";
const long loopTime = 10000;            // Milliseconds

#define MAXC 80
char messageString[MAXC];
int messageCount;
int tagCount;
bool inMessage;

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
}


// This checks a string of characters fed in c to see
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

void NextByte(char c)
{
  if(messageCount == 0)
  {
    if(FindTag(c, bodyStart))
    {
      inMessage = true;
      return;
    }
  } else
  {
    if(FindTag(c, bodyEnd))
    {
      inMessage = false;
      messageString[messageCount - strlen(bodyEnd)] = 0;
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
  client.print(myName);
  client.print(".php");
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
    Serial.println("disconnecting.");
  }
  client.stop();  
}


void loop()
{
  if(Connect())
  {
    messageString[0] = 0;
    HTTPRequest();
    GetMessage();

    Serial.println(messageString);
  
    Disconnect();

    delay(loopTime);
  }
}

