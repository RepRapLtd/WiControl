/*
 * HeatProfile.cpp
 *
 *  Created on: Jan 14, 2014
 *      Author: ensab
 */

#include "Heating.h"

HeatProfile::HeatProfile(std::stringstream profileFileLine, HeatProfile* previous)
{
	next = 0;
	if(previous != 0)
		previous->next = this;

	profileFileLine >> roomName;

	profileFileLine >> panStampNumber;

	list = 0;
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
		if(list == 0)
			list = tat;
	}
}

float HeatProfile::Temperature(long time)
{
	float result = 0.0;

	TimeAndTemperature* timeTempEntry = list;
	if(timeTempEntry == 0)
	{
		Error("HeatProfile contains an empty list.\n");
		return result;
	}

	result = timeTempEntry->Temperature();
	while(timeTempEntry)
	{
		if(timeTempEntry->Time() >= time)
			return result;
		result = timeTempEntry->Temperature();
		timeTempEntry = timeTempEntry->Next();
	}
	return result;
}



