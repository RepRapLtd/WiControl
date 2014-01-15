/*
 * Heating.h
 *
 *  Created on: Jan 13, 2014
 *      Author: ensab
 */

#ifndef HEATING_H_
#define HEATING_H_

#define NAME_LENGTH 50

#define PROFILE_FILE "/home/ensab/Panstamp/WiControl/heat-profile.dat"

#include <SerialStream.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <sstream>
#include "Line.h"
#include "HeatProfile.h"

void Error(char* message);




#endif /* HEATING_H_ */
