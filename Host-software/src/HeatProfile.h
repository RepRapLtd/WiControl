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

class HeatProfile
{
public:
	HeatProfile(std::stringstream& profileFileLine, HeatProfile* previous);
	float Temperature(struct tm* timeinfo);
	int SensorNumber();
	int SwitchNumber();
	bool Invert();
	void PrintProfile(std::ostream& os);
	char* Name();
	HeatProfile* Next();

private:
	TimeAndTemperature* list;
	int sensorNumber;
	int switchNumber;
	bool invert;
	HeatProfile* next;
	char roomName[NAME_LENGTH];
};

inline TimeAndTemperature::TimeAndTemperature(long ds, float t, TimeAndTemperature* previous)
{
	daySeconds = ds;
	temperature = t;
	next = 0;
	if(previous != 0)
		previous->next = this;
}

inline float TimeAndTemperature::Temperature()
{
	return temperature;
}

inline long TimeAndTemperature::Time()
{
	return daySeconds;
}

inline TimeAndTemperature* TimeAndTemperature::Next()
{
	return next;
}

inline int HeatProfile::SensorNumber()
{
	return sensorNumber;
}

inline int HeatProfile::SwitchNumber()
{
	return switchNumber;
}

inline bool HeatProfile::Invert()
{
	return invert;
}

inline HeatProfile* HeatProfile::Next()
{
	return next;
}

inline char* HeatProfile::Name()
{
	return roomName;
}

#endif /* HEATPROFILE_H_ */
