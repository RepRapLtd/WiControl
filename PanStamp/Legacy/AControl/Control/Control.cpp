
#include "Control.h"

char data[DATA_LENGTH];
int8_t dataPointer;

long resetTime;

Communicator* communicator = 0;
CommandBuffer* serialInput;
CommandBuffer* wirelessInput;

volatile bool blinkQueued = false;

void blink()
{
	blinkQueued = true;
}

void blinkInLoop()
{
	if(!blinkQueued)
		return;
	digitalWrite(LEDOUTPUT, !digitalRead(LEDOUTPUT));
	delay(100);
	digitalWrite(LEDOUTPUT, !digitalRead(LEDOUTPUT));
	delay(100);
	blinkQueued = false;
}

void IncommingInterrupt()
{
	blink();
	if(communicator)
		communicator->Interrupt();
}

float GetTemperature()
{
  float r = (float)analogRead(TEMP_SENSE_PIN);
  return ABS_ZERO + THERMISTOR_BETA/log( (r*THERMISTOR_SERIES_R/(AD_RANGE - r))/
		  ( THERMISTOR_25_R*exp(-THERMISTOR_BETA/(25.0 - ABS_ZERO)) ) );
}

float SetHeater(int val)
{
	digitalWrite(HEAT_ON_PIN, val);
}


void setup()
{
	  wdt_disable();
	  resetTime = millis();
	  Serial.begin(38400);
	  Serial.print((int)MY_ADDRESS);
	  Serial.println(" starting");
	  pinMode(LEDOUTPUT, OUTPUT);
	  digitalWrite(LEDOUTPUT, LOW);
	  pinMode(TEMP_SENSE_PIN, INPUT);
	  pinMode(HEAT_ON_PIN, OUTPUT);
	  SetHeater(0);
	  communicator = new Communicator(MY_ADDRESS);
	  communicator->SetDebug(false);
	  dataPointer = 0;
	  blinkQueued = false;
	  serialInput = new CommandBuffer();
	  wirelessInput = new CommandBuffer();
	  wdt_enable(WDTO_8S);
	  wdt_reset();
}

void Message(char* m)
{
	Serial.print(m);
}

void Message(int i)
{
	Serial.print(i);
}

void Interpret(CommandBuffer* cb, int address)
{
	float t;
	int sendAddress;

	if(address == MY_ADDRESS)
	{
		Message("Talking to myself!\n");
		return;
	}

	if(cb->Seen('C')) // Command
	{
		switch(cb->GetIValue())
		{
		case 0: // Some sort of sleep or reset

			break;

		// Turn the heater on

		case 1:
			if(cb->Seen('H'))
				SetHeater(cb->GetIValue());
			break;

		// Print the temperature, and return it

		case 2:
			t = GetTemperature();
			Message("Temp: ");
			Message(t);
			Message("\n");
			if(address >= 0)
			{
				snprintf(data, DATA_LENGTH, "R2 T%d\n", 10*(int)t);
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

		case 4:
			snprintf(data, DATA_LENGTH, "%s, ver: %s, date: %s, addr: %d\n",
					NAME, VERSION, DATE, MY_ADDRESS);
			Message("\n");
			Message(data);
			Message("\n");
			if(address >= 0)
			{
				snprintf(data, DATA_LENGTH, "R4 S%s, ver: %s, date: %s, addr: %d\n",
									NAME, VERSION, DATE, MY_ADDRESS);
				communicator->Send(address, data);
			}
			break;


		default:
			Message("Unknown command: ");
			Message(cb->Buffer());
			Message("\n");
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
				Message("Temp returned from ");
				Message(address);
				Message(" was ");
				Message(0.1*(float)cb->GetIValue());
				Message("\n");
			}
			break;

		case 4:
			if(cb->Seen('S'))
			{
				Message("Identifier from: ");
				Message(address);
				Message(" was ");
				Message(cb->GetString());
				Message("\n");
			}
			break;

		default:
			Message("Unknown reply: ");
			Message(cb->Buffer());
			Message("\n");
		}
	}
}



void loop()
{

	byte address;

	if(millis() - resetTime < ((long)RESET_SECONDS - (long)8)*(long)1000)
		wdt_reset();

	blinkInLoop();

	char* input = communicator->Receive(address);
	if(input)
	{
//		Serial.print("From ");
//		Serial.print(address);
//		Serial.print(": ");
//		Serial.println(input);
		wirelessInput->Fill(input);
		communicator->FreeReadData();
		Interpret(wirelessInput, address);
		// If something has talked to us, we are working OK; reset the reset timer
		resetTime = millis();
	}

	if(!Serial.available())
		return;

	char b = (char)Serial.read();
	if(serialInput->Put(b))
		Interpret(serialInput, -1);

	//Serial.print(b);
}
