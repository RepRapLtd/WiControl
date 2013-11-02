// Do not remove the include below
#include "Control.h"

#define MY_ADDRESS 4
#define THEIR_ADDRESS 5

char data[60];
int8_t dataPointer;

Communicator* communicator = 0;
CommandBuffer* serialInput = new CommandBuffer();
CommandBuffer* wirelessInput = new CommandBuffer();

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
	  Serial.begin(38400);
	  Serial.println("start");
	  pinMode(LEDOUTPUT, OUTPUT);
	  digitalWrite(LEDOUTPUT, LOW);
	  pinMode(TEMP_SENSE_PIN, INPUT);
	  pinMode(HEAT_ON_PIN, OUTPUT);
	  SetHeater(0);
	  communicator = new Communicator(MY_ADDRESS);
	  communicator->SetDebug(true);
	  dataPointer = 0;
	  blinkQueued = false;
}

void Message(char* m)
{
	Serial.print(m);
}

void Interpret(CommandBuffer* cb, int address)
{
	float t;

	if(cb->Seen('C'))
	{
		switch(cb->GetIValue())
		{
		case 0: // Some sort of sleep or reset

			break;

		case 1:
			if(cb->Seen('H'))
				SetHeater(cb->GetIValue());
			break;

		case 2:
			t = GetTemperature();
			sprintf(data, "C2 %f\n", t);
			communicator->Send(address, data);
			break;

		case 3:
			break;


		default:
			Message("Unknown command: ");
			Message(cb->Buffer());
			Message("\n");
		}
	}
}



void loop()
{
	blinkInLoop();
	byte address;

	char* input = communicator->Receive(address);
	if(input)
	{
		//Serial.print("From ");
		//Serial.print(address);
		//Serial.print(": ");
		//Serial.println(input);
		byte point = 0;
		do
		{
			if(wirelessInput->Put(input[point]))
				Interpret(wirelessInput, address);
			point++;
		} while (input[point]);
		communicator->FreeReadData();
	}

	if(!Serial.available())
		return;

	char b = (char)Serial.read();
	if(serialInput->Put(b))
		Interpret(serialInput, -1);

	//Serial.print(b);
}
