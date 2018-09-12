
/*
 * Central Heating (and other) Control System
 * PanStamp Code
 * 
 * Panstamp AVR 1 with atmega328P
 * Adrian Bowyer
 * 
 */

#include <Arduino.h>
#include <stdio.h>
#include <TimeLib.h>
#include <Time.h>
#include <avr/wdt.h>
#include <HardwareSerial.h>

#include "Control.h"
#include "Debug.h"
#include "Communicator.h"
#include "CommandBuffer.h"
#include "Switch.h"
#include "Light.h"


char data[DATA_LENGTH];
int8_t dataPointer;

byte pins[SWITCHES] = OUT_PINS;

Switch* switches[SWITCHES];

Light* light;

unsigned long timeOfLastMessage;

Communicator* communicator = 0;
CommandBuffer* serialInput;
CommandBuffer* wirelessInput;

Message* message;

void blink()
{
  switches[SWITCHES-1]->On(0.0);
  switches[SWITCHES-1]->Off(1.0);
}


void IncommingInterrupt(CCPACKET *packet)
{
  blink();
  if(communicator)
    communicator->Interrupt(packet);
}

float GetTemperature()
{
  float r = (float)analogRead(TEMP_SENSE_PIN);
  return ABS_ZERO + THERMISTOR_BETA/log( (r*THERMISTOR_SERIES_R/(AD_RANGE - r))/
      ( THERMISTOR_25_R*exp(-THERMISTOR_BETA/(25.0 - ABS_ZERO)) ) );
}

float SetOutput(int pin, int val)
{
  digitalWrite(pin, val);
}

void TimeToString(char* s, int leng, time_t t)
{
  snprintf(s, leng, "%d-%d-%d %d:%d:%d", day(t), month(t), year(t), hour(t), minute(t), second(t));
}

void GetHostTime()
{
  strncpy(data, "C7\n", DATA_LENGTH);
  communicator->Send(HOST, data);
}


void Interpret(CommandBuffer* cb, int address)
{
  float t;
  int sendAddress;
  char timeString[DATA_LENGTH];
  time_t tim;

  if(address == MY_ADDRESS)
  {
    message->Say("Talking to myself!\n");
    return;
  }

  if(cb->Seen('C')) // Command
  {
    switch(cb->GetIValue())
    {
    case 0: // Some sort of sleep or reset

      break;

      // Turn an output on or off

    case 1:
      if(cb->Seen('H'))
      {
        int value = cb->GetIValue();
        if(cb->Seen('P'))
        {
          int s = cb->GetIValue();
          float delay = 0.0;
          if(cb->Seen('D'))
            delay = cb->GetFValue();
          if(s >= 0 && s < SWITCHES)
          {
            if(value)
              switches[s]->On(delay);
            else
              switches[s]->Off(delay);
          } else
          {
            message->Say("Dud switch index: ");
            message->Say(s);
            message->Say(".\n");
          }
        }
      }
      break;

      // Print the temperature, and return it

    case 2:
      t = GetTemperature();
      message->Say("Temp: ");
      message->Say(t);
      message->Say("\n");
      if(address >= 0)
      {
        snprintf(data, DATA_LENGTH, "R2 T%d\n", round(10.0*t));
        communicator->Send(address, data);
      }
      break;

      // Send a command to another device

    case 3:
      if(cb->Seen('A'))
      {
        sendAddress = cb->GetIValue();
        if(cb->Seen('S'))
          communicator->Send(sendAddress, cb->GetString());
      }
      break;

      // Say who I am

    case 4:
      snprintf(data, DATA_LENGTH, "\"%s\" %d, %s, ver: %s, date: %s\n",
          PLACE, MY_ADDRESS, NAME, VERSION, DATE);
      message->Say("\n");
      message->Say(data);
      message->Say("\n");
      if(address >= 0)
      {
        snprintf(data, DATA_LENGTH, "R4 S\"%s\" %d, v: %s, %s\n",
          PLACE, MY_ADDRESS, VERSION, DATE);
        communicator->Send(address, data);
      }
      break;

      // Low level write to a pin for test/message

    case 5:
      if(cb->Seen('P'))
      {
        int pin = cb->GetIValue();
        if(cb->Seen('H'))
        {
          pinMode(pin, OUTPUT);
          digitalWrite(pin,cb->GetIValue());
        }
      }
      break;

      // Set LED rgb values

    case 6:
    {
      float r = light->Red();
      float g = light->Green();
      float b = light->Blue();
      float t = 0.0;
      if(cb->Seen('r'))
        r = cb->GetFValue();
      if(cb->Seen('g'))
        g = cb->GetFValue();
      if(cb->Seen('b'))
        b = cb->GetFValue();
      if(cb->Seen('t'))
        t = cb->GetFValue();
      light->Set(r, g, b, t);
    }
      break;

    // Return the time
    case 7:
      tim = now();
      message->Say("Time: ");
      TimeToString(timeString, DATA_LENGTH, tim);
      message->Say(timeString);
      message->Say(" (");
      message->Say((unsigned long)tim);
      message->Say(")");
      message->Say("\n");
      if(address >= 0)
      {
        snprintf(data, DATA_LENGTH, "R7 T%lu\n", (unsigned long)tim);
        communicator->Send(address, data);
      }
      break;

    // Set the time
    case 8:
      if(cb->Seen('T'))
        setTime((time_t)cb->GetLValue());
      break;

    // Turn debugging on or off
    case 9:
      if(cb->Seen('P'))
      {
        if(cb->GetIValue())
          message->DebugOn();
        else
          message->DebugOff();
      }
      break;

    default:
      message->Debug("Unknown command: ");
      message->Debug(cb->Buffer());
      message->Debug("\n");
    }
  }

  if(cb->Seen('R')) // Reply
  {
    switch(cb->GetIValue())
    {
    // Reply from a request for a temperature

    case 2:
      if(cb->Seen('T'))
      {
        message->Say("Temp returned from ");
        message->Say(address);
        message->Say(" was ");
        message->Say((float)(0.1*(float)cb->GetIValue()));
        message->Say("\n");
      }
      break;

    // Reply from a request to send the identifier
    case 4:
      if(cb->Seen('S'))
      {
        message->Say("Identifier from: ");
        message->Say(address);
        message->Say(" was ");
        message->Say(cb->GetString());
        message->Say("\n");
      }
      break;

    // Reply from a request to send the time; use it to set my time
    case 7:
      if(cb->Seen('T'))
      {
        setTime((time_t)cb->GetLValue());
        tim = now();
        message->Say("Time returned from ");
        message->Say(address);
        message->Say(" was ");
        TimeToString(timeString, DATA_LENGTH, tim);
        message->Say(timeString);
        message->Say(" (");
        message->Say((unsigned long)tim);
        message->Say(")");
        message->Say("\n");
      }
      break;

    default:
      message->Debug("Unknown reply: ");
      message->Debug(cb->Buffer());
      message->Debug("\n");
    }
  }
}

void softReset()
{
   wdt_enable(WDTO_15MS);
   for(;;);
}

void setup()
{
    wdt_disable();
    message = new Message();
    message->Say((int)MY_ADDRESS);
    message->Say(" starting\n");
    pinMode(TEMP_SENSE_PIN, INPUT);

    for(byte s = 0; s < SWITCHES; s++)
      switches[s] = new Switch(pins[s]);

    light = new Light();
    communicator = new Communicator(MY_ADDRESS);
    dataPointer = 0;
    serialInput = new CommandBuffer();
    wirelessInput = new CommandBuffer();
    if(MY_ADDRESS != HOST)
    {
      GetHostTime();
    }
    timeOfLastMessage = millis();
    wdt_enable(WDTO_8S);
}



void loop()
{

  byte address;
  unsigned long time = millis();

#ifdef WATCHDOG
  if( time - timeOfLastMessage > ((unsigned long)RESET_SECONDS*(unsigned long)1000 ) )
  {
     message->Say("Timeout reset.\n");
     delay(2000);
  } else
  {
    wdt_reset();
  }
#else
  wdt_reset();
#endif

  time_t tim = now();
  for(byte s = 0; s < SWITCHES; s++)
    switches[s]->Spin(tim);
  light->Spin(time);

  char* input = communicator->Receive(address);
  if(input)
  {
    wirelessInput->Fill(input);
    communicator->FreeReadData();
    Interpret(wirelessInput, address);
    // If something has talked to us, we are working OK; reset the reset timer
    timeOfLastMessage = time;
  }

  if(!Serial.available())
    return;
  char b = (char)Serial.read();
  if(serialInput->Put(b))
    Interpret(serialInput, -1);

  //Serial.print(b);
}



