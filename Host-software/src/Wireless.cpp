/*
 * Wireless.cpp
 *
 *  Created on: Jan 18, 2014
 *      Author: ensab
 */

#include "Heating.h"

Wireless::Wireless(char* port)
{
	line = new Line(port);
}

bool Wireless::GetTemperature(int panStampNumber, float set, char* name, float& result)
{
	result = DEFAULT_TEMPERATURE;
	std::stringstream ss;
	ss << "C3 A" << panStampNumber << " SC2";
	ss.getline(scratchString, LINE_LENGTH);
	line->PutString(scratchString);
	line->PutString("\n");
	time_t startTime;
	time_t now;
	time ( &startTime );
	int i = 0;
	bool notDone = true;
	do
	{
		if(line->ByteAvailable())
		{
			scratchString[i] = line->GetByte();
			if(scratchString[i] == '\n')
			{
				scratchString[i] = 0;
				notDone = false;
			} else
				i++;
		}
		time ( &now );
	} while (now - startTime < 4 && notDone);

	ss.clear();
	ss.str(std::string());

	if(notDone)
	{
		ss << "Timeout on temperature read from: " << panStampNumber << endl;
		ss.getline(scratchString, LINE_LENGTH);
		Error(scratchString);
		result = 1000.0;
		return false;
	}

	ss.clear();
	ss.str(std::string());

	while(scratchString[i] != ' ' && i > 0) i--;
	ss << &scratchString[i];
	ss >> result;

	cout << "Temperature of " << name << " (" << panStampNumber << ") is " << result << " (set temperature: " << set << ").  ";

	return result;
}

void Wireless::SetHeatOn(int panStampNumber)
{
	cout << "Turning " << panStampNumber << " on." << endl;
	std::stringstream ss;
	ss << "C3 A" << panStampNumber << " SC1 H1";
	ss.getline(scratchString, LINE_LENGTH);
	line->PutString(scratchString);
	line->PutString("\n");
}

void Wireless::SetHeatOff(int panStampNumber)
{
	cout << "Turning " << panStampNumber << " off." << endl;
	std::stringstream ss;
	ss << "C3 A" << panStampNumber << " SC1 H0";
	ss.getline(scratchString, LINE_LENGTH);
	line->PutString(scratchString);
	line->PutString("\n");
}

