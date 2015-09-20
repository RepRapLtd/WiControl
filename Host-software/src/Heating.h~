/*
 * Heating.h
 *
 *  Created on: Jan 13, 2014
 *      Author: ensab
 */

#ifndef HEATING_H_
#define HEATING_H_

#define NAME_LENGTH 50

#define LINE_LENGTH 300
#define MIN_INPUT_LENGTH 11
#define DEFAULT_TEMPERATURE 15.0
#define TOP_PAN 100
#define SD '*'
#define OFF_TEMPERATURE 10.0
#define BOOST_TEMPERATURE 24.0

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
	Heating(char* profileFile, char* port, char* tempFile);
	void Run(time_t *rawtime);
	void PrintHeating(std::ostream& os);

private:
	std::ifstream webProfileFile;
	std::ofstream temperatureLogFile;
};




#endif /* HEATING_H_ */
