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
	float GetTemperature(int panStampNumber, float set, char* name);
	void SetHeatOn(int panStampNumber);
	void SetHeatOff(int panStampNumber);

private:
	Line* line;
	char scratchString[LINE_LENGTH];
};

#endif /* WIRELESS_H_ */
