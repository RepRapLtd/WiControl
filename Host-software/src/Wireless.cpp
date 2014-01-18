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

float Wireless::GetTemperature(int panStampNumber)
{
	float result = DEFAULT_TEMPERATURE;
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
	} while (now - startTime < 5 && notDone);

	ss.clear();
	ss.str(std::string());

	if(notDone)
	{
		ss << "Timeout on temperature read from: " << panStampNumber << endl;
		ss.getline(scratchString, LINE_LENGTH);
		Error(scratchString);
		return result;
	}

	ss.clear();
	ss.str(std::string());

	while(scratchString[i] != ' ' && i > 0) i--;
	ss << &scratchString[i];
	ss >> result;

	cout << "Temperature of " << panStampNumber << " is " << result << endl;

	return result;
}

void Wireless::SetHeatOn(int panStampNumber)
{
	cout << "Setting " << panStampNumber << " on." << endl;
	std::stringstream ss;
	ss << "C3 A" << panStampNumber << " SC1 H1";
	ss.getline(scratchString, LINE_LENGTH);
	line->PutString(scratchString);
	line->PutString("\n");
}

void Wireless::SetHeatOff(int panStampNumber)
{
	cout << "Setting " << panStampNumber << " off." << endl;
	std::stringstream ss;
	ss << "C3 A" << panStampNumber << " SC1 H0";
	ss.getline(scratchString, LINE_LENGTH);
	line->PutString(scratchString);
	line->PutString("\n");
}

