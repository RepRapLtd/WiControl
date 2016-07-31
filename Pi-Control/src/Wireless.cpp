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

// Returns true for a valid temperature however obtained, false for timeout

bool Wireless::GetTemperature(Device* device, float set, float& result)
{
	bool responseOK = true;
	bool notDone = true;
	int i = 0;
	std::stringstream ss;

	if(!Valid())
	{
		result = device->GetOldTemperature();
		cerr << "Wireless not live! Using old temperature for " << device->Name() << " (" << result << ")" << endl;
	} else
	{
		result = defaultTemperature;
		ss << "C3 A" << device->PanStampNumber() << " SC2";
		ss.getline(scratchString, LINE_LENGTH);
		line->PutString(scratchString);
		line->PutString("\n");
		time_t startTime;
		time_t now;
		time ( &startTime );

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
			} else
				usleep(2000); // Don't hog the processor
			time ( &now );
		} while (now - startTime < 4 && notDone && i < LINE_LENGTH);

	
		if(notDone)
		{
			result = device->GetOldTemperature();;
			responseOK = false;
		} else
		{
			ss.clear();
			ss.str(std::string());

			while(scratchString[i] != ' ' && i > 0) i--;
			ss << &scratchString[i];
			ss >> result;
		}
	}

	return responseOK;
}

bool Wireless::Valid()
{
	if(!line)
		return false;
	return line->Valid();
}

unsigned long Wireless::SimpleTime(struct tm* timeinfo)
{
	unsigned long summerTime = 0;
	if(timeinfo->tm_isdst > 0)
		summerTime = 3600;
	return (unsigned long)mktime(timeinfo) + summerTime;
}

void Wireless::SendTime(struct tm* timeinfo)
{
	if(!Valid())
		return;

	std::stringstream ss;
	ss << "C8 T" << SimpleTime(timeinfo);
	ss.getline(scratchString, LINE_LENGTH);
	line->PutString(scratchString);
	line->PutString("\n");
}

void Wireless::PrintTime()
{
	if(!Valid())
	{
		cerr << "Print time - no serial connection.\n";
		return;
	}

	std::stringstream ss;
	ss << "C7";
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
		} else
			usleep(2000); // Don't hog the processor
		time ( &now );
	} while (now - startTime < 4 && notDone && i < LINE_LENGTH);

	if(notDone)
	{
		cerr << "Timeout on time read." << endl;
		return;
	}


	cout << scratchString << endl;
}

void Wireless::SetSwitchOn(Device* device)
{
	if(!Valid())
		return;

	std::stringstream ss;
	ss << "C3 A" << device->PanStampNumber() << " SC1" << " P" << device->PanStampPort() << " H1" << " D" << device->OnDelay();
	ss.getline(scratchString, LINE_LENGTH);
	line->PutString(scratchString);
	line->PutString("\n");
	cout << "Turned " << device->Name() << " on." << endl;
}

void Wireless::SetSwitchOff(Device* device)
{
	if(!Valid())
		return;

	std::stringstream ss;
	ss << "C3 A" << device->PanStampNumber() << " SC1" << " P" << device->PanStampPort() << " H0" << " D" << device->OffDelay();
	ss.getline(scratchString, LINE_LENGTH);
	line->PutString(scratchString);
	line->PutString("\n");
	cout << "Turned " << device->Name() << " off." << endl;
}

