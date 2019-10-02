
/*
 * Program to run a node on the WiFi Home Control system.
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
 * unit=1&load=0&temperature=20&debugOn=1
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
 * 5 September 2018
 * 
 * Licence: GPL
 * 
 * 
 * 
 * *******************************************************************************************
 * 
 * Note that if you use the last device (which uses the user LED pin to control
 * something) you must also set USER_LED_PIN to -1 in HomeControlFirmware.h.
 */

#include "HomeControlFirmware.h"
#include "Load.h"

String currentServer = server;                    // The one in use

// This is turned off and on when running by freeing/grounding DEBUG_PIN

bool debug = true;

long nextBlink;
byte blinkPattern;
byte ledState;

// Text and messages

String message = "";

// Timing and resets

long nextReset;
long seconds;

ESP8266WiFiMulti WiFiMulti;
Load* loads;


void setup() 
{
   // I/O pins...

  if(ESP8266_LED_PIN >= 0)
  {
    pinMode(ESP8266_LED_PIN, OUTPUT);
    digitalWrite(ESP8266_LED_PIN, OFF);
  }

  if(USER_LED_PIN >= 0)
  {
    pinMode(USER_LED_PIN, OUTPUT);
    digitalWrite(USER_LED_PIN, !OFF);
  }
  
  ledState = OFF;
    
  pinMode(TEMP_SENSE_PIN, INPUT);
  pinMode(DEBUG_PIN, INPUT_PULLUP);
  debug = !digitalRead(DEBUG_PIN);
  randomSeed(analogRead(TEMP_SENSE_PIN));

  // Set up the chain of loads (loadCount is only 1 usually).  

  loads = (Load*)NULL;
  for(int l = 0; l < loadCount; l++)
  {
    loads = new Load(l, outputPins[l], loads);
  }

  Serial.begin(BAUD);

  while(!Serial);

  if(debug)
  {
    Serial.print("I am unit ");
    Serial.println(unit);
    Serial.println("? to print status.");
    Serial.print("Connecting to WiFi: ");
    Serial.print(ssid);   
  }

  // Needed for WiFi stability
  
  delay(4000);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  char wCount = 0;
  do
  {
    delay(2000);
    if(debug)
    {
      Serial.print(" .");
      wCount++;
      if(!wCount%50)
      {
        Serial.println();
        wCount = 0;  
      }
    }
  } while (WiFiMulti.run() != WL_CONNECTED);

  if(debug)
  {
    Serial.println(" connected.");
  }

  PrintStatus();

  // Set up timings and LED behaviour
  
  blinkPattern = OFF;
  nextBlink = (long)millis();
  seconds = nextBlink;
  nextReset = nextBlink + rebootTime;
}

// Decide the time delay before the next server request.  This is usually quick (~15s) for debugging
// (so you can see what's going on), or slow (~60s) for normal operation. 

long TillNextTime()
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



// Amuse the user with blinking lights

void Leds(int onOff)
{
  if(ESP8266_LED_PIN > 0)
    digitalWrite(ESP8266_LED_PIN, onOff);
  if(USER_LED_PIN > 0)
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
  r = ABS_ZERO + THERMISTOR_BETA/log( (r*THERMISTOR_SERIES_R/(AD_RANGE*TOP_VOLTAGE/MAX_AD_VOLTAGE - r))/
      ( THERMISTOR_25_R*exp(-THERMISTOR_BETA/(25.0 - ABS_ZERO)) ) );
  return String(r, 1);
}


// Assemble the HTTP request in the message String

void ComposeQuery(Load* load)
{
  message = "http://";
  message.concat(currentServer);
  message.concat(pageRoot);
  message.concat(page);
  message.concat("?unit=");
  message.concat(unit);
  message.concat("&load=");
  message.concat(load->LoadNumber());
  message.concat("&temperature=");
  message.concat(Temperature());
  if(debug)
  {
    message.concat("&debugOn=1");
  }
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

// This gets called once a second and
// handles delayed switching.

void TimedOnOrOff()
{
  Load* load = loads;
  while(load)
  {
    load->SecondTick();
    load = load->Next();
  }
}


// Go through the body of the HTML returned from the server and decide what
// action it's instructing us to take.

void ParseMessage(Load* load)
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
    load->SwitchOnOrOff(false, message.substring(toDo+4).toInt());
  } else
  {
    load->SwitchOnOrOff(true, message.substring(toDo+3).toInt());
  }

  if(debug)
    PrintWebPageReturned(bod);
}

// This handles counting seconds and doing things when a second ticks

void SecondCounter()
{
  long tim = millis();
  if(tim - seconds > 0)
  {
    seconds = tim + 1000;
    TimedOnOrOff();
  }
}

void PrintStatus()
{
  if(!debug)
    return;
    
  Serial.println();
  Serial.println();
  Serial.print("Firmware version: ");
  Serial.println(version);
  Serial.print("I am unit ");
  Serial.println(unit);
  Serial.print("I control load(s): ");
  Load* load = loads;
  while(load)
  {
    Serial.print(load->LoadNumber());
    Serial.print(' ');
      load = load->Next();
  }
  Serial.println();

  if(WiFiMulti.run() == WL_CONNECTED)
  {
    Serial.print("Connected to WiFi: ");
    Serial.println(ssid);
  } else
  {
    Serial.println("Not connected to WiFi.");
  }

  unsigned char MAC[6];

  WiFi.macAddress(MAC);
  Serial.print("MAC and IP addresses: ");
  for(char i = 0; i < 6; i++)
  {
    Serial.print(MAC[i], HEX);
    if(i < 5) Serial.print(":");
  }
  Serial.print(", ");
  Serial.println(WiFi.localIP());

  Serial.print("My uncorrected temperature is: ");
  Serial.println(Temperature());
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

  // Time for a load to do something?

  Load* load = loads;
  while(load)
  {
    load->ActIfItsTime();
    load = load->Next();
  }

  debug = !digitalRead(DEBUG_PIN);

  // Entertain the user
  
  Blink();

  // Timing by seconds
  
  SecondCounter();

  // Is the user debugging and do they want to see what's going on?
  
  if(Serial.available() > 0 && debug)
  {
    char c = (char)Serial.read();
    if(c == '?')
    {
      PrintStatus();
      c = (char)Serial.read(); // Absorb the newline
    } 
  }

}

//***************************************************************************************************

// The Load class

Load::Load(const int ln, const int p, Load* nxt)
{
  loadNumber = ln;
  pin = p;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, 0);
  next = nxt;
  nextTime = (long)millis() + initialTime;
  iAmOn = false;
  onSeconds = -1;
  offSeconds = -1;
}

Load* Load::Next() { return next; }

long Load::NextTime() { return nextTime; }

int Load::LoadNumber() { return loadNumber; }

void Load::ChangeNumber(const int ln) { loadNumber = ln; }

void Load::SecondTick()
{
  if(onSeconds > 0)
    onSeconds--;

  if(onSeconds == 0)
  {
    if(debug)
    {
       Serial.print("Switching on load ");
       Serial.println(loadNumber);
    }
    blinkPattern = ON;
    digitalWrite(pin, 1);
    iAmOn = true;
    onSeconds = -1;
  }

  if(offSeconds > 0)
    offSeconds--;

  if(offSeconds == 0)
  {
    if(debug)
    {
       Serial.print("Switching off load ");
       Serial.println(loadNumber);
    }
    blinkPattern = OFF;
    digitalWrite(pin, 0);
    iAmOn = false;
    offSeconds = -1;
  }      
}

// Turn the load (central heating valve, or whatever) on or off
// This doesn't actually change the state of the load, it just
// sets a seconds count (which can be 0) for when that change is to be made in the future.

void Load::SwitchOnOrOff(bool on, long tim)
{
  if(debug)
  {
    Serial.print("Switching load ");
    Serial.print(loadNumber);
  }
    
  if(on)
  {
   if(onSeconds >= 0)
   {
    if(debug)
      Serial.println(" **ON timer running");
    return;
   }
   onSeconds = tim;
   if(debug)
   {
    Serial.print(" ON in ");
    Serial.print(onSeconds);
    Serial.println(" seconds.");
   }
   return;    
  }

  if(offSeconds >= 0)
  {
    if(debug)
      Serial.println(" **OFF timer running");
    return;
  }
  offSeconds = tim;
  if(debug)
  {
    Serial.print(" OFF in ");
    Serial.print(offSeconds);
    Serial.println(" seconds.");
  }   
}


void Load::ActIfItsTime()
{
  
  long tim = (long)millis();
    
  // If the HTTP request will be in 1 second, flash the LEDs...
 
  if((1000 + tim) - nextTime > 0)
    blinkPattern=FLASH;

  // Is it time to send another request to the server?
  
  if(tim - nextTime < 0)
    return;

  if(iAmOn)
        blinkPattern=ON;
  else
        blinkPattern=OFF;
        
  // wait for WiFi connection
  
  if ((WiFiMulti.run() == WL_CONNECTED)) 
  {
     if(debug)
       Serial.println();
    
     HTTPClient http;
    
     ComposeQuery(this);
    
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
         ParseMessage(this);
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
      blinkPattern = DASH;
      if(debug)
        Serial.println("\nNot WL_CONNECTED.");    
   }
    
   nextTime = TillNextTime();
}



