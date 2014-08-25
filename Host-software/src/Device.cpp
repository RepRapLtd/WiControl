/*
 * Device.cpp
 *
 *  Created on: Aug 25, 2014
 *      Author: ensab
 */

#include "Heating.h"


Device::Device(std::stringstream& profileFileLine, Device* previous)
{
	ReadQuotedString(profileFileLine, name);

	profileFileLine >> std::skipws >> panStampNumber;
	profileFileLine >> std::skipws >> port;
	profileFileLine >> std::skipws >> onDelay;
	profileFileLine >> std::skipws >> offDelay;

	on = false;
	if(previous)
		previous->next = this;
	next = NULL;
}

void Device::PrintDevice(std::ostream& os)
{
	os << '"' << name << '"' << " " << panStampNumber << " " << port << " " << onDelay << " " << offDelay << endl;
}

void Device::On()
{
	if(on)
		return;
	wireless->SetSwitchOn(panStampNumber, port, onDelay);
	on = true;
}

void Device::Off()
{
	wireless->SetSwitchOff(panStampNumber, port, offDelay);
	on = false;
}

void Device::FlagOff()
{
	on = false;
}

bool Device::IAmOn()
{
	return on;
}

char* Device::Name()
{
	return name;
}

Device* Device::Next()
{
	return next;
}




