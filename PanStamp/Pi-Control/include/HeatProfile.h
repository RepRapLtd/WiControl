/*
 * HeatProfile.h
 *
 *  Created on: Jan 14, 2014
 *      Author: ensab
 */

#ifndef HEATPROFILE_H_
#define HEATPROFILE_H_



class TimeAndTemperature
{
public:
	TimeAndTemperature(long ds, float t, TimeAndTemperature* previous);
	TimeAndTemperature* Next();
	float Temperature();
	long Time();

private:
	long daySeconds;
	float temperature;
	TimeAndTemperature* next;
};

//************************************************************************************************

class HeatProfile
{
public:
	HeatProfile(std::stringstream& profileFileLine, HeatProfile* previous);
	float Temperature(struct tm* timeinfo, Wireless *wireless);
//	int SensorNumber();
//	int SwitchNumber();
	bool Invert();
	Device* TemperatureSensor();
	void PrintProfile(std::ostream& os);
	void On();
	char* Name();
	HeatProfile* Next();


private:
	TimeAndTemperature* timeAndTemperatureList;
	int switchNumber;
	bool invert;
	HeatProfile* next;
	char roomName[NAME_LENGTH];
	Device* tempDevice;
	Device** devices;
	int deviceCount;
	int switchedOn;
	unsigned long boostSetting;
};


#endif /* HEATPROFILE_H_ */
