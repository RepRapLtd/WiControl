/*
 * Communicator.cpp
 *
 *  Created on: 26 Oct 2013
 *      Author: ensab
 */

#include "Control.h"

Communicator::Communicator(byte address)
{
	networkAddress[0] = NETWORK_ADDRESS_0;
	networkAddress[1] = NETWORK_ADDRESS_1;
	myAddress = address;
	lastReceivedFrom = 0;
	dataAvailable = false;
	debug = false;
	cc1101.init();
	cc1101.setSyncWord(networkAddress, false);
	cc1101.setDevAddress(myAddress, false);
	cc1101.enableAddressCheck();
	cc1101.setRxState();
	attachInterrupt(0, IncommingInterrupt, FALLING);
}

void Communicator::Send(byte address, char* data)
{
	byte i = 0;
	do
	{
		if(i >= CC1101_DATA_LEN - 2)
		{
			Serial.println("Transmit buffer overflow!");
			return;
		}
		transmitPacket.data[i+2] = data[i];
	} while(data[i++]);

	transmitPacket.data[0] = address;
	transmitPacket.data[1] = myAddress;
	transmitPacket.length = i + 2;
	cc1101.setTxState();
	if(debug)
	{
		Serial.print(transmitPacket.data[1]);
		Serial.print(" sending: ");
		Serial.println((char*)&transmitPacket.data[2]);
	}
	if(!cc1101.sendData(transmitPacket))
		Serial.println("Transmit failed!");
	else
		blink();
	cc1101.setRxState();
}

char* Communicator::Receive(byte& address)
{
	if(!dataAvailable)
		return 0;
	lastReceivedFrom = receivePacket.data[1];
	address = lastReceivedFrom;
	return (char*)&receivePacket.data[2];
}


void Communicator::Interrupt()
{
	detachInterrupt(0);
	blink();

	if(dataAvailable)
		Serial.println("Overwriting input packet!");

	if(cc1101.receiveData(&receivePacket) > 0)
	{
		if (receivePacket.crc_ok && receivePacket.length > 1)
			dataAvailable = true;
	}

	// Enable wireless reception interrupt
	attachInterrupt(0, IncommingInterrupt, FALLING);
}


