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

	profileFileLine >> panStampNumber;

	char dummy = ' ';

	while(isspace(dummy) || dummy == ',')
		profileFileLine >> dummy;



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



