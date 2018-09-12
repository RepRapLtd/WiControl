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
    serial_port.SetBaudRate( SerialStreamBuf::BAUD_RATE ) ;
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
    xmitString[0] = 0;
    rcvString[0] = 0;
    xmitP = 0;
    rcvP = 0;
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
	rcvString[rcvP] = next_byte;
	rcvP++;
	if(rcvP >= LINE_LENGTH - 1)
	{
		rcvString[rcvP] = 0;
		cerr << "Wireles receive overflow: " << rcvString << endl;
		rcvP = 0;
	}
	if(next_byte == '\n' || !next_byte)
	{
		rcvString[rcvP] = 0;
		if(debug > 1)
			cout << "Wireless received: " << rcvString << endl;
		rcvP = 0;
	}
	return next_byte;
}

void Line::PutByte(char byte)
{
	serial_port << byte;
	xmitString[xmitP] = byte;
	xmitP++;
	if(xmitP >= LINE_LENGTH - 1)
	{
		xmitString[xmitP] = 0;
		cerr << "Wireles transmit overflow: " << xmitString << endl;
		xmitP = 0;
	}
	if(byte == '\n' || !byte)
	{
		xmitString[xmitP] = 0;
		if(debug > 1)
			cout << "Wireless transmitted: " << xmitString << endl;
		xmitP = 0;
	}
}

void Line::PutString(char* string)
{
	int i = 0;
	while(string[i])
	{
		PutByte(string[i]);
		i++;
	}
}



