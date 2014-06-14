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



void Error(char* message)
{
	cout << message << endl;
}

Heating::Heating(char* profileFile, char* port)
{
	heatProfile = 0;
	HeatProfile* temp = 0;
	std::stringstream strs;
	char string[LINE_LENGTH];
	std::ifstream ifs;

	ifs.open(profileFile, std::ifstream::in);

	ifs.getline(string, LINE_LENGTH);
	//cout << string << endl;
	strs << string;
	strs >> boilerNumber;

	strs.clear();
	strs.str(std::string());

	while(!ifs.eof())
	{
		ifs.getline(string, LINE_LENGTH);
		//cout << string << endl;
		std::string s = string;
		if(s.length() >= MIN_INPUT_LENGTH)
		{
			strs << string;
			temp = new HeatProfile(strs, temp);
			strs.clear();
			strs.str(std::string());
			if(heatProfile == 0)
				heatProfile = temp;
		}
	}

	wireless = new Wireless(port);

}

void Heating::PrintHeating(std::ostream& os)
{
	os << "Heating settings:\n";
	os << " Boiler address: " << boilerNumber << endl;

	HeatProfile* hp = heatProfile;
	while(hp)
	{
		os << "  ";
		hp->PrintProfile(os);
		hp = hp->Next();
	}
}

void Heating::Run(struct tm* timeinfo)
{
	HeatProfile* hp = heatProfile;
	float setTemperature;
	bool boilerOn = false;
	while(hp)
	{
		setTemperature = hp->Temperature(timeinfo);
		float temp;
		int retries = 0;
		while(!wireless->GetTemperature(hp->SensorNumber(), setTemperature, hp->Name(), temp) && retries < 3)
			retries++;

		if((temp < setTemperature) != hp->Invert())
		{
			wireless->SetSwitchOn(hp->SwitchNumber());
			boilerOn = true;
		} else
			wireless->SetSwitchOff(hp->SwitchNumber());
		hp = hp->Next();
	}

	if(boilerOn)
		wireless->SetSwitchOn(boilerNumber);
	else
		wireless->SetSwitchOff(boilerNumber);
}

int main(int argc, char** argv)
{
	time_t rawtime;

	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	cout << asctime (timeinfo) ;

	Heating* heating = new Heating(PROFILE_FILE, SERIAL_PORT);

	//heating->PrintHeating(cout);

	heating->Run(timeinfo);

	cout << "\n";

    return EXIT_SUCCESS ;
}


