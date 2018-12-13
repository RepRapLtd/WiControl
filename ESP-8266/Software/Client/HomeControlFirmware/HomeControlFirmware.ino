
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
 * 5 September 2018
 * 
 * Licence: GPL
 * 
 * 
 * 
 * *******************************************************************************************
 * 
 */

#include "HomeControlFirmware.h"

// This is turned off and on when running by freeing/grounding DEBUG_PIN

bool debug = true;

// Interactively set locations etc when true

int setting = 0;

// Flashing lights

long nextBlink;
byte blinkPattern;
byte ledState;

// Text and messages

/*
 * Typical format for these:
 * 
 * ssid          "MyHubName"
 * password      "HubWiFiPassword"
 * pageRoot      "/FolderUnder/  -  /var/www/html"
 * page          "controllednode.php"
 * server        "myDomain.com"
 * backupServer  "192.168.local.server"
 * 
 */

// Useful to have just one instance of this to prevent
// constructors being called all over the place.

const String blank = "";

String message = blank;
String ssid = blank;         // WiFi Hub
String password = blank;     // WiFi password
String pageRoot = blank;     // Where the .php script is on the server
String page = blank;         // The script we need
String server = blank;       // Server IP address/URL
String backupServer = blank; // Backup server IP address/URL
String building = blank;     // Which building is the device in?
String loadName = blank;     // Temporary storage for load names
int loadCount = 0;           // Used for saving and loading from flash

String currentServer;                    // The one in use

// Thermistor temperature correction

double tCorrection = DEFAULT_T_CORRECTION;

// Saving and loading from flash memory

Flash* flash;

// Timing and resets

long nextReset;
long seconds;

ESP8266WiFiMulti WiFiMulti;
Load* loads = (Load*)0;
int outputs[] = {OUTPUT_PIN_0, OUTPUT_PIN_1, OUTPUT_PIN_2, OUTPUT_PIN_3, OUTPUT_PIN_4, OUTPUT_PIN_5};

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

  Serial.begin(BAUD);
  while(!Serial);

  // Absorb power-up/reset noise from the serial input.
  
  while(Serial.available() > 0)
    Serial.read();

  if(debug)
    Serial.println("Starting.");

  
  randomSeed(analogRead(TEMP_SENSE_PIN));

  setting = 0;
  loadCount = 0;

  flash = new Flash();

  loads = (Load*)0;

  // Needed for WiFi stability
  
  delay(4000);


  if(debug)
  {
    Serial.println("Type L to load settings from flash or S to set new ones."); 
  } else
  {
    LoadSettings();
  }
  
  // Set up timings and LED behaviour
  
  blinkPattern = OFF;
  nextBlink = (long)millis();
  seconds = nextBlink;
  nextReset = nextBlink + rebootTime;
}


void Connect()
{
  if(debug)
  {
    Serial.print("Connecting to ");
    Serial.print(ssid);
  }

  unsigned char sBuf[BUF_LEN] = {0x00};
  unsigned char pBuf[BUF_LEN] = {0x00};
  if(ssid.length() < BUF_LEN)
  {
    ssid.getBytes(sBuf, ssid.length());
  } else
  {
    if(debug)
      Serial.println("ssid is too long for the buffer.");
  }
  if(password.length() < BUF_LEN)
  {
    password.getBytes(pBuf, password.length());
  } else
  {
    if(debug)
      Serial.println("password is too long for the buffer.");
  }   
  
  WiFi.mode(WIFI_STA);

  // These casts are a bit nasty...
  
  WiFiMulti.addAP((char*)sBuf, (char*)pBuf);

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
}

// Load the settings from flash memory

void LoadSettings()
{
  flash->Load();
  
  ssid = flash->NextString();
  password = flash->NextString();
  server = flash->NextString();
  currentServer = server;                    // The one in use
  backupServer = flash->NextString();
  pageRoot = flash->NextString();
  page = flash->NextString();
  building = flash->NextString();
  tCorrection = (double)flash->NextString().toFloat();
  
  String l;
  loadCount = 0;
  while(!(l = flash->NextString()).equals(blank))
  {
    if(loadCount < MAX_LOADS)
    {
      loads = new Load(l, outputs[loadCount], loads);
      loadCount++;
    } else
    {
      if(debug)
        Serial.println("LoadSettings() - maximum number of loads exceeded.");
    }
  }
  
  flash->Clear();
  Connect();
  PrintStatus();
}

// Build a single entry in the save-to-flash table.

bool BuildSetting(const char c, String& current, String& next, const char* prompt1, const String& prompt2, const char* prompt3)
{
   if(c != '\n')
   {
     current.concat(c);
     return false;        
   } else
   {
     flash->Cat(current);
     Serial.print(prompt1);
     Serial.print(prompt2);
     Serial.println(prompt3);
     next = blank;
   }
   return true;
}

// Save the settings to flash memory.  This takes
// user input one byte at a time (the char parameter)
// in order to keep the main loop() function running
// as this prevents the esp-8266 resetting itself.

void SaveSettings(char c)
{  
  switch(setting)
  {
    case 1:
      Serial.println("Type the name of the WiFi ssid.");
      setting = 2;
      ssid = blank;
      flash->Clear();
      break;
      
    case 2:
      if(BuildSetting(c, ssid, password, "Type the WiFi password.", blank, ""))
      {
        setting = 3;
      }
      break;

    case 3:
      if(BuildSetting(c, password, server, "Type the server name in the form abc.com or as an IP like 123.56.1.39 .", blank, ""))
      {
        setting = 4;
      }
      break; 

    case 4:
      if(BuildSetting(c, server, backupServer, "Type the backup server name in the form def.net or as an IP like 78.242.0.27 .", blank, ""))
      {
        setting = 5;
      }
      break; 

    case 5:
      if(BuildSetting(c, backupServer, pageRoot, "Type the pageroot in the form /WiFiHeating/", blank, ""))
      {
        setting = 6;
      }
      break; 

     case 6:
      if(BuildSetting(c, pageRoot, page, "Type the page in the form controllednode.php .", blank, ""))
      {
        setting = 7;
      }
      break; 

    case 7:
      if(BuildSetting(c, page, building, "Type the name of the building.", blank, ""))
      {
        setting = 8;
        loadName = blank;
      }
      break;

    case 8:  // Note we use loadName as temporary storage for the temp offset
      if(BuildSetting(c, building, loadName, "Type the temperature correction in the form -2.3 .", blank, ""))
      {
        setting = 9;
      }
      break;

    case 9:
      if(BuildSetting(c, loadName, loadName, "Type the name of load ", String(0), ""))
      {
        tCorrection = (double)loadName.toFloat();
        loadCount = 1;
        loadName = blank;
        setting = 10;
      }
      break;

    case 10:
      if (c == '#')
      {
        flash->Save();
        LoadSettings();
        setting = 0;
      } else if(BuildSetting(c, loadName, loadName, "Type the name of load ", String(loadCount), " or # to save them all."))
      {
        loadCount++;
      }
      break;

    default:
      Serial.print("Dud flash setting value: ");
      Serial.println(setting); 
  }
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
  r = tCorrection + ABS_ZERO + THERMISTOR_BETA/log( (r*THERMISTOR_SERIES_R/(AD_RANGE*TOP_VOLTAGE/MAX_AD_VOLTAGE - r))/
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
  message.concat("?building=");
  message.concat(building);
  message.concat("&location=");
  message.concat(load->Location());
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
    
  Serial.print("\n\nFirmware version: ");
  Serial.println(version);
  Serial.print("I control: ");
  Load* load = loads;
  while(load)
  {
    Serial.print(load->Location());
    Serial.print(' ');
      load = load->Next();
  }
  Serial.print("in ");
  Serial.println(building);

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

  Serial.print("My temperature is: ");
  Serial.print(Temperature());
  Serial.print(" (using a temperature correction of ");
  Serial.print(tCorrection);
  Serial.println(").");
  Serial.println("Type ? to get the status again at any time.");
}

// Wait for \n with timeout

void WaitForNewLine()
{
  long now = millis();
  char c = ' ';
  while(c != '\n' && millis() - (now + 500) < 0)
  {
    if(Serial.available() > 0)
        c = (char)Serial.read();
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

  // Is the user debugging and do they want to Load, Save, or get status?
  
  if(Serial.available() > 0 && debug)
  {
    char c = (char)Serial.read();
    if(setting)
    {
      SaveSettings(c);
    } else
    {
      c = (char)toupper(c);

      // Absorb the newline plus any other rubbish
       
      WaitForNewLine();
      
      switch(c)
      {
        case 'S':
          setting = 1;
          break;
        
        case 'L':
          LoadSettings();
          break;
          
        case '?':
          PrintStatus();
          break;
          
        default:
          Serial.print("Dud character received: ");
          Serial.println(c);
          Serial.println("Type L to load location(s) from flash, S to set location(s) in flash or ? to print status.");
      }
      
    }
  }

}

//***************************************************************************************************

// The Load class

Load::Load(const String l, int p, Load* n)
{
  location = l;
  pin = p;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, 0);
  next = n;
  nextTime = (long)millis() + initialTime;
  iAmOn = false;
  onSeconds = -1;
  offSeconds = -1;
}

Load* Load::Next() { return next; }

long Load::NextTime() { return nextTime; }

String Load::Location() { return location; }

void Load::ChangeLocation(String l) { location = l; }

void Load::SecondTick()
{
  if(onSeconds > 0)
    onSeconds--;

  if(onSeconds == 0)
  {
    if(debug)
    {
       Serial.print("Switching on ");
       Serial.println(location);
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
       Serial.print("Switching off ");
       Serial.println(location);
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
    Serial.print("Switching ");
    Serial.print(location);
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

//******************************************************************************************************

// The Flash class that saves and loads things from permanent memory.

Flash::Flash()
{
  Clear();
}

// Clear the flash array

void Flash::Clear()
{
  for(int i = 0; i < FLASH_TOP; i++)
    flash[i] = 0;
  flashPointer = 0;
}

// Add a single character to the flash array

void Flash::CatByte(char c)
{
  if(1 + flashPointer >= 4*FLASH_TOP)
  {
    if(debug)
      Serial.println("Error: byte exceeds maximum flash space; not written.");
    return;
  }
  
  unsigned int i = flashPointer >> 2;
  unsigned int b = (flashPointer%4)*8;
  uint32 bits = c;
  bits = bits << b;
  uint32 mask = 0xff << b;
  mask = ~mask;
  flash[i] = (flash[i] & mask) | bits;
  flashPointer++;
}

// Get the next single character from the flash array

char Flash::GetByte()
{
  unsigned int i = flashPointer >> 2;
  unsigned int b = (flashPointer%4)*8;
  uint32 bits = flash[i];
  bits = bits >> b;
  char c = (char)(bits & 0x000000ff);
  flashPointer++;
  return c;
}

// Write a string to flash at the current flashPointer adding a trailing 0 so it's like a char array.

void Flash::Cat(String s)
{
  int l = s.length();
  
  if(2 + flashPointer + l >= 4*FLASH_TOP) // 2 is defensive...
  {
    if(debug)
      Serial.println("Error: string length exceeds maximum flash space; not written.");
    return;
  }
  
  for(int i = 0; i < l; i++)
    CatByte(s.charAt(i));
  CatByte(0);
}



// Get the next string from the flash array
// The convention is that if this is "" that's the end of the lot.

String Flash::NextString()
{
  String r = blank;
  char c;
  while(c = GetByte()) // NB = not ==
    r.concat(c);
  return r;
}

// Actually save flash into the flash sector

void Flash::Save()
{
  // Flag end of data.
  CatByte(0);
  
  int e = spi_flash_erase_sector(FLASH_SECTOR);
  if(e && debug)
  {
    Serial.print("Error erasing flash: ");
    Serial.println(e);
  }

  e = spi_flash_write(FLASH_RW, flash, sizeof(flash));
  if(e && debug)
  {
    Serial.print("Error writing flash: ");
    Serial.println(e);
  }  
}

// Load flash from the flash sector

void Flash::Load()
{
  int e = spi_flash_read(FLASH_RW, flash, sizeof(flash));
  if(e && debug)
  {
    Serial.print("Error reading flash: ");
    Serial.println(e);
  }
  flashPointer = 0;    
}



