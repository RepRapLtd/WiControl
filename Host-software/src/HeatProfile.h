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
	HeatProfile(std::stringstream profileFileLine, HeatProfile* previous);
	float Temperature(long time);
	int PanStampNumber();
	void PrintProfile(std::ostream);

private:
	TimeAndTemperature* list;
	int panStampNumber;
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

inline int HeatProfile::PanStampNumber()
{
	return panStampNumber;
}

#endif /* HEATPROFILE_H_ */
