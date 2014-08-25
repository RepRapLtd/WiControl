/*
 * Heating.h
 *
 *  Created on: Jan 13, 2014
 *      Author: ensab
 */

#ifndef HEATING_H_
#define HEATING_H_

#define NAME_LENGTH 50
//#define PROFILE_FILE "/home/pi/Panstamp/WiControl/heat-profile.dat"
//#define SERIAL_PORT "/dev/ttyUSB0"
#define LINE_LENGTH 300
#define MIN_INPUT_LENGTH 11
#define DEFAULT_TEMPERATURE 15.0
#define TOP_PAN 100

class Line;
class HeatProfile;
class Wireless;
class Device;
class Heating;

#include <SerialStream.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <cstring>
#include <stdio.h>
#include <sstream>

#include "Line.h"
#include "HeatProfile.h"
#include "Wireless.h"
#include "Device.h"

void ReadQuotedString(std::stringstream& profileFileLine, char* s);
Device* FindDevice(char* name);

extern HeatProfile* heatProfileList;
extern Wireless* wireless;
extern Device* deviceList;
extern char scratchString[];
extern Device* scratchDevices[];


class Heating
{
public:
	Heating(char* profileFile, char* port);
	void Run(struct tm* timeinfo);
	void PrintHeating(std::ostream& os);

private:
};




#endif /* HEATING_H_ */
