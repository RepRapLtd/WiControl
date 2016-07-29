//============================================================================
// Name        : Heating.cpp
// Author      : Adrian
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//#include <iostream>
//using namespace std;

#include "Heating.h"

char scratchString[LINE_LENGTH];
Device* scratchDevices[LINE_LENGTH];
HeatProfile* heatProfileList;
Wireless* wireless;
Device* deviceList;

void ReadQuotedString(std::stringstream& profileFileLine, char* s)
{
	char c = ' ';
	while(c != SD  && !profileFileLine.eof()) c = profileFileLine.get();
	if(profileFileLine.eof())
	{
		// No start quote found.
		s[0] = 0;
		return;
	}

	int i = -1;
	do
	{
		i++;
		if(i >= NAME_LENGTH - 1)
		{
			s[i] = 0;
			cerr << "Name too long: " << s << endl;
			return;
		}
		s[i] = profileFileLine.get();
	} while(s[i] != SD && !profileFileLine.eof());
	s[i] = 0;
}

Device* FindDevice(char* name)
{
	Device* dp = deviceList;
	while(dp)
	{
		if(strcmp(name, dp->Name()) == 0)
			return dp;
		dp = dp->Next();
	}
	return NULL;
}

Heating::Heating(char* profileFile, char* port, char* tempFile)
{
	deviceList = 0;
	Device* tempD = 0;
	heatProfileList = 0;
	HeatProfile* tempH = 0;
	std::stringstream strs;
	char string[LINE_LENGTH];

	webProfileFile.open(profileFile, std::ifstream::in);
	bool deviceEnd = false;
	deviceList = NULL;
	while(!deviceEnd)
	{
		webProfileFile.getline(string, LINE_LENGTH);
		//cout << string << endl;
		std::string s = string;
		if(s.length() >= MIN_INPUT_LENGTH)
		{
			strs << string;
			tempD = new Device(strs, tempD);
			if(deviceList == 0)
				deviceList = tempD;
		} else
			deviceEnd = true;
		strs.clear();
		strs.str(std::string());
	}

	oldTemperatureLogFile.open(tempFile, std::ifstream::in);
	while(!oldTemperatureLogFile.eof())
	{
		oldTemperatureLogFile.getline(string, LINE_LENGTH);
		std::string s = string;
		strs << string;
		ReadQuotedString(strs, scratchString);
		float t;
		strs >> std::skipws >> t;
		Device* d = FindDevice(scratchString);
		if(d)
			d->SetOldTemperature(t);
		strs.clear();
		strs.str(std::string());
	}
	oldTemperatureLogFile.close();

	temperatureLogFile.open(tempFile, std::ofstream::out);

	while(!webProfileFile.eof())
	{
		webProfileFile.getline(string, LINE_LENGTH);
		//cout << string << endl;
		std::string s = string;
		if(s.length() >= MIN_INPUT_LENGTH)
		{
			strs << string;
			tempH = new HeatProfile(strs, tempH);
			strs.clear();
			strs.str(std::string());
			if(heatProfileList == 0)
				heatProfileList = tempH;
		}
	}

	wireless = new Wireless(port);

	if(!wireless->Valid())
		cerr << "No wireless communications available." << endl;

}

void Heating::PrintHeating(std::ostream& os)
{
	os << "Heating settings:\n Devices:\n";

	Device* d = deviceList;
	while(d)
	{
		os << "  ";
		d->PrintDevice(os);
		d = d->Next();
	}

	os << " Profiles:\n";
	HeatProfile* hp = heatProfileList;
	while(hp)
	{
		os << "  ";
		hp->PrintProfile(os);
		hp = hp->Next();
	}
}

void Heating::Run(time_t *rawtime)
{
	struct tm *timeinfo;
	timeinfo = localtime (rawtime);

	// Tell the Arduino the time

	wireless->SendTime(timeinfo);
	usleep(2000);

	// Flag all devices as not on

	Device* dp = deviceList;
	while(dp)
	{
		dp->FlagOff();
		dp = dp->Next();
	}

	// Print the time

	wireless->PrintTime();
	usleep(2000);

	// Check each profile and see if its devices need to be on

	HeatProfile* hp = heatProfileList;
	float setTemperature;
	bool switchOn;
	while(hp)
	{
		setTemperature = hp->Temperature(timeinfo, wireless);
		float locationTemperature;
		int retries = 0;

		while(!hp->TemperatureSensor()->GetTemperature(setTemperature, locationTemperature) && retries < 3)
			retries++;

		switchOn = ((locationTemperature < setTemperature) != hp->Invert());
		if(switchOn)
			hp->On();

		temperatureLogFile << SD << hp->Name() << SD << ' ' << locationTemperature << ' ' << setTemperature;

		if(switchOn)
			temperatureLogFile << " 1";
		else
			temperatureLogFile << " 0";

		temperatureLogFile << endl;

		hp = hp->Next();
		usleep(2000);
	}

	// Any device still flagged as not on may need to be turned off
	// NB the calls to On() above and the calls to Off() below mean
	// that every device should be talked to, and so all the watchdogs
	// should be reset.

	dp = deviceList;
	while(dp)
	{
		if(!dp->IAmOn())
			dp->Off();
		dp = dp->Next();
		usleep(2000);
	}

	temperatureLogFile.close();
}

int main(int argc, char** argv)
{
	time_t rawtime;
	int serialPortArg = -1;
	int profileArg = -1;
	int tempFileArg = -1;

	if(argc != 7)
	{
		cerr << "Usage: Heating -p USB-port -i heat-profile-file -o temperature-file\n";
		return EXIT_FAILURE;
	}

	for(int i = 0; i < argc; i++)
	{
		if(strcmp("-p", argv[i]) == 0)
			serialPortArg = i + 1;
		if(strcmp("-i", argv[i]) == 0)
			profileArg = i + 1;
		if(strcmp("-o", argv[i]) == 0)
			tempFileArg = i + 1;
	}

	if(serialPortArg < 0)
	{
		cerr << "Heating: no serial port defined (no -i /dev/ttyUSB0)\n";
		return EXIT_FAILURE;
	}

	if(profileArg < 0)
	{
		cerr << "Heating: no profile file defined (no -p heat-profile-file)\n";
		return EXIT_FAILURE;
	}

	if(tempFileArg < 0)
	{
		cerr << "Heating: no temperature file defined (no -o temperature-file)\n";
		return EXIT_FAILURE;
	}

	time ( &rawtime );

	//cout << asctime (timeinfo) ;

	Heating* heating = new Heating(argv[profileArg], argv[serialPortArg], argv[tempFileArg]);

	// heating->PrintHeating(cout); cout << "\n";

	heating->Run(&rawtime);

	cout << "\n";

    return EXIT_SUCCESS;
}

