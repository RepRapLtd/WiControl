/*
 * HeatProfile.cpp
 *
 *  Created on: Jan 14, 2014
 *      Author: ensab
 */

#include "Heating.h"

TimeAndTemperature::TimeAndTemperature(long ds, float t, TimeAndTemperature* previous)
{
	daySeconds = ds;
	temperature = t;
	next = 0;
	if(previous != 0)
		previous->next = this;
}

float TimeAndTemperature::Temperature()
{
	return temperature;
}

long TimeAndTemperature::Time()
{
	return daySeconds;
}

TimeAndTemperature* TimeAndTemperature::Next()
{
	return next;
}

//***************************************************************************************************


HeatProfile::HeatProfile(std::stringstream& profileFileLine, HeatProfile* previous)
{
	next = 0;
	if(previous != 0)
		previous->next = this;

	invert = false;

	ReadQuotedString(profileFileLine, roomName);

	profileFileLine >> temperatureSensorPanstamp;
	if(temperatureSensorPanstamp < 0)
	{
		invert = true;
		temperatureSensorPanstamp = -temperatureSensorPanstamp;
	}

	bool readingDevices = true;
	deviceCount = 0;
	while(readingDevices)
	{
		char c;
		profileFileLine >> std::skipws >> c;
		if(c == '"')
		{
			profileFileLine.putback(c);
			ReadQuotedString(profileFileLine, scratchString);
			scratchDevices[deviceCount] = FindDevice(scratchString);
			if(scratchDevices[deviceCount] == NULL)
			{
				cerr << "Device " << scratchString << " not found. \n" << endl;
				readingDevices = false;
			}
			deviceCount++;
		} else
			readingDevices = false;
	}

	devices = new Device*[deviceCount];
	for(int d = 0; d < deviceCount; d++)
		devices[d] = scratchDevices[d];

	timeAndTemperatureList = 0;
	TimeAndTemperature* tat = 0;

	long hours, minutes, seconds;
	char dummy;
	float temperature;

	while(!profileFileLine.eof())
	{
		profileFileLine >> hours;
		profileFileLine >> dummy;
		profileFileLine >> minutes;
		profileFileLine >> dummy;
		profileFileLine >> seconds;
		profileFileLine >> temperature;

		tat = new TimeAndTemperature(hours*3600 + minutes*60 + seconds, temperature, tat);
		if(timeAndTemperatureList == 0)
			timeAndTemperatureList = tat;
	}
}

void HeatProfile::On()
{
	for(int d = 0; d < deviceCount; d++)
		devices[d]->On();
}

int HeatProfile::SensorNumber()
{
	return temperatureSensorPanstamp;
}

int HeatProfile::SwitchNumber()
{
	return switchNumber;
}

bool HeatProfile::Invert()
{
	return invert;
}

HeatProfile* HeatProfile::Next()
{
	return next;
}

char* HeatProfile::Name()
{
	return roomName;
}

void HeatProfile::PrintProfile(std::ostream& os)
{
	os << roomName << ' ' <<  temperatureSensorPanstamp << ' ';

	long time;
	long hours, minutes, seconds;
	TimeAndTemperature* tat = timeAndTemperatureList;

	while(tat)
	{
		time = tat->Time();
		hours = time/3600;
		minutes = (time - hours*3600)/60;
		seconds = time - hours*3600 - minutes*60;
		os << hours << ':' << minutes << ':' << seconds << ' ';
		os << tat->Temperature();
		tat = tat->Next();
		if(tat)
			os << ' ';
	}
	os << '\n';
}

float HeatProfile::Temperature(struct tm* timeinfo)
{
	float result = 0.0;

	long t = timeinfo->tm_hour*3600 + timeinfo->tm_min*60 + timeinfo->tm_sec;

	TimeAndTemperature* timeTempEntry = timeAndTemperatureList;
	if(timeTempEntry == 0)
	{
		cerr << "HeatProfile contains an empty list.\n";
		return result;
	}

	result = timeTempEntry->Temperature();
	while(timeTempEntry)
	{
		if(timeTempEntry->Time() >= t)
			return result;
		result = timeTempEntry->Temperature();
		timeTempEntry = timeTempEntry->Next();
	}
	return result;
}



