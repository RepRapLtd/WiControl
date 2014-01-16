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

void HeatProfile::PrintProfile(std::ostream os)
{
	os << roomName << ' ' <<  panStampNumber << ' ';

	long time;
	long hours, minutes, seconds;
	TimeAndTemperature* tat = list;

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



