/*
 * Debug.cpp
 *
 *  Created on: 31 Jan 2016
 *      Author: ensab
 */

#include "Control.h"

Message::Message()
{
	debug = false;
	Serial.begin(BAUD_RATE);
}

void Message::DebugOn()
{
	debug = true;
	Serial.println("Debugging on");
}

void Message::DebugOff()
{
	debug = false;
	Serial.println("Debugging off");
}

void Message::Debug(char* string)
{
	if(!debug)
		return;
	Serial.print(string);
}

void Message::Debug(int i)
{
	if(!debug)
		return;
	Serial.print(i);
}

void Message::Debug(float f)
{
	if(!debug)
		return;
	Serial.print(f);
}

void Message::Debug(unsigned long l)
{
	if(!debug)
		return;
	Serial.print(l);
}

void Message::Say(char* string)
{
	Serial.print(string);
}

void Message::Say(int i)
{
	Serial.print(i);
}


void Message::Say(float f)
{
	Serial.print(f);
}

void Message::Say(unsigned long l)
{
	Serial.print(l);
}

