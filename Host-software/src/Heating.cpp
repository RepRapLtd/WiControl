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

void loop(Line* usb)
{
	char b;

	if(usb->ByteAvailable())
		std::cout << usb->GetByte();

//	struct pollfd fds;
//	        int ret;
//	        fds.fd = 0; /* this is STDIN */
//	        fds.events = POLLIN;
//	        ret = poll(&fds, 1, 0);
//
//	if(_kbhit())
//	{
//		std::cin.get(b);
//		if(b == '\r')
//			b = '\n';
//		std::cout << b;
//		usb->PutByte(b);
//	}
}

int
main(int argc, char** argv)
{
	Line* usb = new Line();

	for(;;) loop(usb);

    std::cerr << std::endl ;
    return EXIT_SUCCESS ;
}


