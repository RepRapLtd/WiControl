/*
 * Line.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: ensab
 */

#include "Heating.h"

Line::Line(char* port)
{
    serial_port.Open(port) ;
    if ( ! serial_port.good() )
    {
    	Error("Cannot open port:");
    	Error(port);
        exit(1) ;
    }
    //
    // Set the baud rate of the serial port.
    //
    serial_port.SetBaudRate( SerialStreamBuf::BAUD_38400 ) ;
    if ( ! serial_port.good() )
    {
    	Error("Cannot set the baud rate.");
        exit(1) ;
    }
    //
    // Set the number of data bits.
    //
    serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
    if ( ! serial_port.good() )
    {
    	Error("Cannot set the character size.");
        exit(1) ;
    }
    //
    // Disable parity.
    //
    serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
    if ( ! serial_port.good() )
    {
    	Error("Cannot set parity.");
        exit(1) ;
    }
    //
    // Set the number of stop bits.
    //
    serial_port.SetNumOfStopBits( 1 ) ;
    if ( ! serial_port.good() )
    {
    	Error("Cannot set number of stop bits.");
        exit(1) ;
    }
    //
    // Turn off hardware flow control.
    //
    serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
    if ( ! serial_port.good() )
    {
    	Error("Cannot set hardware flow control.");
        exit(1) ;
    }

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



