/*
 * Line.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: ensab
 */

#include "Heating.h"

Line::Line(char* port)
{
	valid = true;
    serial_port.Open(port);
    if ( ! serial_port.good() )
    {
    	cerr << "Cannot open port: " << port << endl;
        valid = false;
    }
    //
    // Set the baud rate of the serial port.
    //
    serial_port.SetBaudRate( SerialStreamBuf::BAUD_38400 ) ;
    if ( ! serial_port.good() )
    {
    	cerr << "Cannot set the baud rate." << endl;
    	valid = false;
    }
    //
    // Set the number of data bits.
    //
    serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
    if ( ! serial_port.good() )
    {
    	cerr << "Cannot set the character size." << endl;
    	valid = false;
    }
    //
    // Disable parity.
    //
    serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
    if ( ! serial_port.good() )
    {
    	cerr << "Cannot set parity." << endl;
    	valid = false;
    }
    //
    // Set the number of stop bits.
    //
    serial_port.SetNumOfStopBits( 1 ) ;
    if ( ! serial_port.good() )
    {
    	cerr << "Cannot set number of stop bits." << endl;
    	valid = false;
    }
    //
    // Turn off hardware flow control.
    //
    serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
    if ( ! serial_port.good() )
    {
    	cerr << "Cannot set hardware flow control." << endl;
    	valid = false;
    }

}

bool Line::Valid()
{
	return valid;
}

bool Line::ByteAvailable()
{
	return serial_port.rdbuf()->in_avail();
}

char Line::GetByte()
{
	char next_byte;
	serial_port.get(next_byte);
	return next_byte;
}

void Line::PutByte(char byte)
{
	serial_port << byte;
}

void Line::PutString(char* string)
{
	serial_port << string;
}



