/*
 * Device.h
 *
 *  Created on: Aug 25, 2014
 *      Author: ensab
 */

#ifndef DEVICE_H_
#define DEVICE_H_


class Device
{
public:
	Device(std::stringstream& profileFileLine, Device* previous);
	void On();
	void Off();
	void FlagOff();
	bool IAmOn();
	char* Name();
	void PrintDevice(std::ostream& os);
	int PanStampNumber();
	int PanStampPort();
	float OnDelay();
	float OffDelay();
	bool GetTemperature(float setTemperature, float& temperature);
        float GetOldTemperature();
        void SetOldTemperature(float t);
	Device* Next();

private:
	char name[NAME_LENGTH];
	int panStampNumber;
	int port;
	float onDelay;
	float offDelay;
	float oldTemperature;
	bool on;
	Device* next;
};


#endif /* DEVICE_H_ */
