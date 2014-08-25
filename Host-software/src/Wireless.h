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
	bool GetTemperature(int panStampNumber, float set, char* name, float& result);
	void SetSwitchOn(int panStampNumber, int port, float delay);
	void SetSwitchOff(int panStampNumber, int port, float delay);
	bool Valid();

private:
	Line* line;
};

#endif /* WIRELESS_H_ */
