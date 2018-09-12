/*
 * Wireless.h
 *
 *  Created on: Jan 18, 2014
 *      Author: ensab
 */

#ifndef WIRELESS_H_
#define WIRELESS_H_

class Wireless
{
public:
	Wireless(char* port);
	bool GetTemperature(Device* device, float& temperature);
	void SetSwitchOn(Device* device);
	void SetSwitchOff(Device* device);
	void SendTime(struct tm* timeinfo);
	void PrintTime();
	unsigned long SimpleTime(struct tm* timeinfo);
	bool Valid();
	void SendString(Device* device, const char* string);
	bool GetResponse(Device* device);

private:
	Line* line;
};

#endif /* WIRELESS_H_ */
