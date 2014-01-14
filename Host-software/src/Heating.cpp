//============================================================================
// Name        : Heating.cpp
// Author      : Adrian
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//#include <iostream>
//using namespace std;

#include "Heating.h"



void Error(char* message)
{
	cerr << message;
}

int
main(int argc, char** argv)
{
	Line* usb = new Line();
	time_t rawtime;

	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	cout << "Current local time and date: " << asctime (timeinfo) ;

    return EXIT_SUCCESS ;
}


