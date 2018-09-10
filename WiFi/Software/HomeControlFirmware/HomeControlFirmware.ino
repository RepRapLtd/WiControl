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
 * building=Workshop&location=Office&temperature=20&debugOn=1
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
long onSecondCount;
long offSecondCount;
long seconds;

bool loadIsOn;

ESP8266WiFiMulti WiFiMulti;


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

  // Set up each load 'by hand'.
  // Subsequently they will be refered to by their array index.
  // TODO make a load a class.
  


  Serial.begin(BAUD);

  while(!Serial);

  if(debug)
  {
    Serial.println();
    Serial.println();
    Serial.print("I control: ");
// PRINT LOADS HERE
    Serial.print("in ");
    Serial.println(building);    
    Serial.print("Connecting to WiFi: ");
    Serial.print(ssid);
  }

  // Needed for WiFi stability
  
  delay(4000);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  // Set up timings and LED behaviour
  
  blinkPattern = OFF;
  onSecondCount = -1;
  offSecondCount = -1;
  nextBlink = (long)millis();
  seconds = nextTime;
  nextReset = nextTime + rebootTime;
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
  //Serial.println(r);
  r = T_CORRECTION + ABS_ZERO + THERMISTOR_BETA/log( (r*THERMISTOR_SERIES_R/(AD_RANGE*TOP_VOLTAGE/MAX_AD_VOLTAGE - r))/
      ( THERMISTOR_25_R*exp(-THERMISTOR_BETA/(25.0 - ABS_ZERO)) ) );
  return String(r, 1);
}


// Assemble the HTTP request in the message String

void ComposeQuery(String* load)
{
  message = "http://";
  message.concat(currentServer);
  message.concat(pageRoot);
  message.concat(page);
  message.concat("?building=");
  message.concat(building);
  message.concat("&location=");
  message.concat(&load);
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
  if(onSecondCount > 0)
    onSecondCount--;

  if(onSecondCount == 0)
  {
    if(debug)
       Serial.println("Switching on.");
    blinkPattern = ON;
    digitalWrite(OUTPUT_PIN, 1);
    loadIsOn = true;
    onSecondCount = -1;
  }

  if(offSecondCount > 0)
    offSecondCount--;

  if(offSecondCount == 0)
  {
    if(debug)
       Serial.println("Switching off.");
    blinkPattern = OFF;
    digitalWrite(OUTPUT_PIN, 0);
    loadIsOn = false;
    offSecondCount = -1;
  }    
}


// Turn the load (central heating valve, or whatever) on or off
// This doesn't actually change the state of the load, it just
// sets a seconds count (which can be 0) for when that change is to be made in the future.

void SwitchOnOrOff(bool on, long tim)
{
  if(debug)
    Serial.print("Switching ");
    
  if(on)
  {
   if(onSecondCount >= 0)
   {
    if(debug)
      Serial.println("**ON timer running");
    return;
   }
   onSecondCount = tim;
   if(debug)
   {
    Serial.print("ON in ");
    Serial.print(onSecondCount);
    Serial.println(" seconds.");
   }
   return;    
  }

  if(offSecondCount >= 0)
  {
    if(debug)
      Serial.println("**OFF timer running");
    return;
  }
  offSecondCount = tim;
  if(debug)
  {
    Serial.print("OFF in ");
    Serial.print(offSecondCount);
    Serial.println(" seconds.");
  } 
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
    SwitchOnOrOff(false, message.substring(toDo+4).toInt());
  } else
  {
    SwitchOnOrOff(true, message.substring(toDo+3).toInt());
  }

  if(debug)
    PrintWebPageReturned(bod);
}

// This handles counting seconds and doing things when a second ticks

void SecondCounter()
{
  long t = millis();
  if(t - seconds > 0)
  {
    seconds = t + 1000;
    TimedOnOrOff();
  }
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
    if(loadIsOn)
      blinkPattern=ON;
    else
      blinkPattern=OFF;
      
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

  // Timing by seconds
  
  SecondCounter();

}

//***************************************************************************************************

// The Load class

Load::Load(String* l, int p, Load* n)
{
  location = l;
  pin = p;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, 0);
  next = n;
  nextTime = NextTime();
  iAmOn = false;
  onSeconds = -1;
  offSeconds = -1;

  
}

Load* Load::Next() { return next; }

long Load::NextTime() { return nextTime; }

void Load::SecondTick()
{
  
}



