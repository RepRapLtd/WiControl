
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


String currentServer = "";                    // The one in use

// Local WiFi

char* ssid = "";
char* password = "";

// The unit number must be unique across the whole system

int unit = 1;

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

Load load2(2, outputPins[2]);
Load load1(1, outputPins[1]);
Load load0(0, outputPins[0]);
Load* loads[MAX_LOADS] = {&load0, &load1, &load2};
int loadCount = 1;                          // The number of loads this device drives; usually 1; must be 3 or less

Flash* flash = new Flash();

long debugSampleTime = 15000*loadCount;   // Milliseconds between server requests when debugging
const long debugRandomTime = 2000;    // +/- Milliseconds (must be < sampleTime) used to randomise requests to reduce clashes
const long sampleTime = 60000;        // Milliseconds between server requests
const long randomTime = 5000;         // +/- Milliseconds (must be < sampleTime) used to randomise requests to reduce clashes
const long rebootTime = 3600000;      // Milliseconds between resets.
const long initialTime = 5000;        // Milliseconds to first server request

//*********************************************************************************************************

// Get the configuration from EEPROM

bool GetConfiguration()
{ 
  if(!flash->Get())
  {
    return false;
  }

  ssid = flash->GetNextTag();
  if(debug)
  {
    Serial.print("ssid from EEPROM: ");
    Serial.println(ssid);
  }
  
  password = flash->GetNextTag();
  if(debug)
  {
    Serial.print("password from EEPROM: ");
    Serial.println(password);
  }

  char* server = flash->GetNextTag();
  currentServer = String(server);
  delete[] server;
  if(debug)
  {
    Serial.print("server from EEPROM: ");
    Serial.println(currentServer);
  }

  flash->Reset();

  return true;
}


void setup() 
{

  //ESP.wdtDisable();
  
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

  yield();
    
  pinMode(TEMP_SENSE_PIN, INPUT);
  pinMode(DEBUG_PIN, INPUT_PULLUP);
  debug = !digitalRead(DEBUG_PIN);
  randomSeed(analogRead(TEMP_SENSE_PIN));

  Serial.begin(BAUD);
  Serial.setTimeout(10000);

  while(!Serial) yield();

  if(!GetConfiguration())
  {
    if(debug)
      Serial.println("\nUse the w command to set the configuration then reboot.");
    ssid = "";
    password = "";
    currentServer = "";    
  }

  debugSampleTime = 15000*loadCount;

  if(debug)
  {
    Serial.print("Unit ");
    Serial.print(unit);
    Serial.print(", WiFi: ");
    Serial.print(ssid);
    Serial.print(", server: ");
    Serial.print(currentServer);    
    Serial.println("\n? for status; w to setup.\n");   
  }

  // 4 Needed for WiFi stability

  yield();
  delay(1000);
  yield();
  delay(1000);
  yield();
  delay(1000);
  yield();
  delay(1000);

  yield();

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  yield();

  bool wiFiNotConnected = true;

  char wCount = 0;
  do
  {
    Leds(ON);
    delay(100);
    Leds(OFF);
    delay(1900);
    if(debug)
    {
      Serial.print(".");
    }
    wCount++;
    yield();
    wiFiNotConnected = (WiFiMulti.run() != WL_CONNECTED);
  } while ( (wCount < maxWiFiTries) && wiFiNotConnected);

  if(debug)
  {
    if(wiFiNotConnected)
    {
      Serial.println("\nWiFi not connected. Entering main loop anyway.");
    } else
    {
      Serial.println("yes");
    }
  }

  yield();

  // Set up the chain of loads (loadCount is only 1 usually).  

//  loads = (Load*)NULL;
//  loadsAdded = 0;

  PrintStatus();

  yield();


/*  int i = 1;
  Load* l = loads;
  Load* nextL;
  while(l)
  {
    nextL = l->Next();
    if(i >= loadCount)
      l->SetNext((Load*)NULL);
    i++;
    l = nextL;
    yield();          
  }
*/
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
    Serial.print("Next call: ");
    Serial.print(f);
    Serial.println(" ms.");
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
      
    case FLASHING:
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
        Serial.print(" not in ");
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

  Serial.println("\n--");
  Serial.println(message);
  Serial.println("--\n");  
}

// This gets called once a second and
// handles delayed switching.

void TimedOnOrOff()
{
  for(int load = 0; load < loadCount; load++)
  {
    loads[load]->SecondTick();
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
        Serial.print(" not in ");
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
        Serial.print("Dud msg: ");
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
  Serial.print("Ver: ");
  Serial.println(version);
  Serial.print("Unit ");
  Serial.println(unit);
  Serial.print("Load(s): ");
  for(int load = 0; load < loadCount; load++)
  {
    Serial.print(loads[load]->LoadNumber());
    Serial.print(' ');
  }
  Serial.println();

  Serial.print("Heap: ");
  Serial.println(ESP.getFreeHeap());

  Serial.print("WiFi: ");
  if(WiFiMulti.run() == WL_CONNECTED)
  {
    Serial.println(ssid);
  } else
  {
    Serial.println("no connection.");
  }

  unsigned char MAC[6];

  WiFi.macAddress(MAC);
  Serial.print("MAC and IP: ");
  for(char i = 0; i < 6; i++)
  {
    Serial.print(MAC[i], HEX);
    if(i < 5) Serial.print(":");
  }
  Serial.print(", ");
  Serial.println(WiFi.localIP());

  Serial.print("Temp: ");
  Serial.println(Temperature());
}

void SetConfiguration()
{
  char buf[MAX_EEPROM];
  
  while(Serial.available())
    Serial.read();
      
  Serial.println("\n\nSet the configuration\n");

  Serial.print("Unit number (must be unique across the whole system): ");
  while(!Serial.available());
  unit = Serial.parseInt();
  sprintf(buf, "%d", unit);
  Serial.println(unit);
  flash->PutNextTag(buf);

  Serial.print("Number of loads controlled between 1 and 3 (usually 1): ");
  while(!Serial.available());
  loadCount = Serial.parseInt();
  sprintf(buf, "%d", loadCount);
  Serial.println(loadCount);
  flash->PutNextTag(buf);
  
  Serial.print("ssid: ");
  while(!Serial.available());
  int len = Serial.readBytesUntil('\n', buf, MAX_EEPROM - 3);
  buf[len - 1] = 0;
  ssid = new char(len + 3);
  strncpy(ssid, buf, len);
  Serial.println(ssid);
  flash->PutNextTag(ssid);
  
  Serial.print("Password: ");
  while(!Serial.available());
  len = Serial.readBytesUntil('\n', buf, MAX_EEPROM - 3);
  buf[len - 1] = 0;
  password = new char(len + 3);
  strncpy(password, buf, len);
  Serial.println(password);
  flash->PutNextTag(password);

  Serial.print("Server in the form xyz.com: ");
  while(!Serial.available());
  len = Serial.readBytesUntil('\n', buf, MAX_EEPROM - 3);
  buf[len - 1] = 0;
  currentServer = String(buf);
  Serial.println(currentServer);
  flash->PutNextTag(currentServer);

  flash->Put();
}


// What it says...

void loop() 
{
  
  yield();
      
  // Check for watchdog reset time
  
  if((long)millis() - nextReset > 0)
  {
    if(debug)
      Serial.println("\n\n*** Reset.\n");
    delay(2000); // Allow the serial output buffer to flush
    ESP.restart();
  }


  // Time for a load to do something?

  for(int load = 0; load < loadCount; load++)
  {
    loads[load]->ActIfItsTime();
    yield();
  }

  debug = !digitalRead(DEBUG_PIN);

  // Entertain the user
  
  Blink();

  // Timing by seconds
  
  SecondCounter();

  // Is the user debugging and do they want to see what's going on or change the settings?
  
  if(Serial.available() > 0 && debug)
  {
    char c = (char)Serial.read();
    if(c == '?')
    {
      PrintStatus();
      c = (char)Serial.read(); // Absorb the newline
    }
    if(c == 'w')
    {
      c = (char)Serial.read(); // Absorb the newline
      SetConfiguration();
    }  
  }

}

//***************************************************************************************************

// The Load class

Load::Load(const int ln, const int p)
{
  loadNumber = ln;
  pin = p;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, loadOff);
  //next = nxt;
  nextTime = (long)millis() + initialTime;
  iAmOn = false;
  onSeconds = -1;
  offSeconds = -1;
}

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
       Serial.print("Load on:");
       Serial.println(loadNumber);
    }
    blinkPattern = ON;
    digitalWrite(pin, loadOn);
    iAmOn = true;
    onSeconds = -1;
  }

  if(offSeconds > 0)
    offSeconds--;

  if(offSeconds == 0)
  {
    if(debug)
    {
       Serial.print("Load off:");
       Serial.println(loadNumber);
    }
    blinkPattern = OFF;
    digitalWrite(pin, loadOff);
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
    Serial.print("Load ");
    Serial.print(loadNumber);
  }
    
  if(on)
  {
   if(onSeconds >= 0)
   {
    if(debug)
      Serial.println(" **ON timer");
    return;
   }
   onSeconds = tim;
   if(debug)
   {
    Serial.print(" ON in ");
    Serial.print(onSeconds);
    Serial.println(" s");
   }
   return;    
  }

  if(offSeconds >= 0)
  {
    if(debug)
      Serial.println(" **OFF timer");
    return;
  }
  offSeconds = tim;
  if(debug)
  {
    Serial.print(" OFF in ");
    Serial.print(offSeconds);
    Serial.println(" s");
  }   
}


void Load::ActIfItsTime()
{
  
  long tim = (long)millis();
    
  // If the HTTP request will be in 1 second, flash the LEDs...
 
  if((1000 + tim) - nextTime > 0)
    blinkPattern = FLASHING;

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
           Serial.println("\nNo HTTP_CODE_OK");
       }
     } else 
     {
       blinkPattern = DASH;
       if(debug)
         Serial.printf("\nGET error: %s\n", http.errorToString(httpCode).c_str());
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

//*********************************************************************************************************

// The EEPROM Flash class

Flash::Flash()
{
  EEPROM.begin(MAX_EEPROM);
  Reset();    
}

void Flash::Reset()
{
  writing = false;
  reading = false;
  address = 0;   
}

bool Flash::Get()
{
  if(reading || writing)
  {
    if(debug)
    {
      Serial.println("Flash error - attempt to load while reading or writing tags.");
    }
    return false;
  }

  for(address = 0; address < MAX_EEPROM; address++)
    buf[address] = char(EEPROM.read(address));
  
  Reset();

  char* checkString = GetNextTag();

  if(strncmp(checkString, eCheckString, MAX_EEPROM))
  {
    if(debug)
      Serial.println("Load from flash - checkstring invalid.");
    Reset();
    return false;
  }
  
  return true;
}

void Flash::Put()
{
  if(reading)
  {
    if(debug)
    {
      Serial.println("Flash error - attempt to save while reading tags.");
    }
    return;
  }
  
  for(address = 0; address < MAX_EEPROM; address++)
    EEPROM.write(address, buf[address]);
  EEPROM.commit();
  
  Reset();
}

char* Flash::GetNextTag()
{
  if(writing)
  {
    if(debug)
    {
      Serial.println("Flash error - attempt to read while writing tags.");
    }
    return "";
  }
  
  reading = true;
  int a = address;
  while(buf[address] && address < MAX_EEPROM)
    address++;
  if(address >= MAX_EEPROM - 1)
  {
    if(debug)
    {
      Serial.println("Flash error - attempt to read past end of buffer.");
    }
  } else
  {
    address++;
  }
  
  return &buf[a];    
}

void Flash::PutNextTag(const char* tag)
{
  if(reading)
  {
    if(debug)
    {
      Serial.println("Flash error - attempt to write while reading tags.");
    }
    return;
  }

  // First one? Yes - first write the checkstring.
  
  if(!writing)
  {
    Reset();
    writing = true;
    PutNextTag(eCheckString);
  }
  
  writing = true;
  int tp = 0;
  while(tag[tp] && address < MAX_EEPROM)
  {
    buf[address] = tag[tp];
    address++;
    tp++;  
  }
  if(address >= MAX_EEPROM - 1)
  {
    if(debug)
    {
      Serial.println("Flash error - attempt to write past end of buffer.");
    }
    buf[MAX_EEPROM - 1] = 0;
  } else
  {
    buf[address] = 0;
    address++;
  }
}

void Flash::PutNextTag(const String tag)
{
  int len = tag.length();
  char* temp = new char[len+1];
  for(int tp = 0; tp < len; tp++)
    temp[tp] = tag.charAt(tp);
  temp[len] = 0;
  PutNextTag(temp);
  delete[] temp;  
}
