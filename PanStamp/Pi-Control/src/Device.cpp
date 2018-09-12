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
	oldTemperature = defaultTemperature;
	listenedTo = false;

	on = false;
	if(previous)
		previous->next = this;
	next = NULL;
}

void Device::PrintDevice(std::ostream& os)
{
	os << SD << name << SD << ", panStamp: " << panStampNumber << ", port:" << port << ", on-delay:" << onDelay << ", off-delay:" << offDelay << endl;
}

int Device::PanStampNumber()
{
	return panStampNumber;
}

int Device::PanStampPort()
{
	return port;
}

float Device::OnDelay()
{
	return onDelay;
}

float Device::OffDelay()
{
	return offDelay;
}

bool Device::GetTemperature(float& temperature)
{
	return wireless->GetTemperature(this, temperature);
}

float Device::GetOldTemperature()
{
	return oldTemperature;
}

void Device::SetOldTemperature(float t)
{
	oldTemperature = t;
}

void Device::On()
{
	wireless->SetSwitchOn(this);
	on = true;
}

void Device::Off()
{
	wireless->SetSwitchOff(this);
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

bool Device::CheckLive()
{
	wireless->SendString(this, "SC4");
	bool gotResponse = wireless->GetResponse(this);
	if(debug)
	{
		if(gotResponse)
			cout << "Checking response from " << Name() << ". It returned: " << scratchString << endl;
		else
			cerr << "Checking response from " << Name() << ". No response." << endl;
	}
	return gotResponse;
}

void Device::SetListenedTo(bool l)
{
	listenedTo = l;
}

bool Device::ListenedTo()
{
	return listenedTo;
}


