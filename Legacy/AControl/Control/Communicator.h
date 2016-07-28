/*
 * Communicator.h
 *
 *  Created on: 26 Oct 2013
 *      Author: ensab
 */

#ifndef COMMUNICATOR_H_
#define COMMUNICATOR_H_

#include "cc1101.h"

#define NETWORK_ADDRESS_0 199
#define NETWORK_ADDRESS_1 0

class Communicator
{
public:
	Communicator(byte address);
	void Send(byte address, char* data);
	char* Receive(byte& address);
	void Interrupt();
	void FreeReadData();
	void SetDebug(bool d);

private:
	CC1101 cc1101;
	CCPACKET transmitPacket;
	CCPACKET receivePacket;
	volatile bool dataAvailable;
	byte networkAddress[2];
	byte myAddress;
	byte lastReceivedFrom;
	bool debug;
};

inline void Communicator::SetDebug(bool d)
{
	debug = d;
}

inline void Communicator::FreeReadData()
{
	dataAvailable = false;
}

#endif /* COMMUNICATOR_H_ */
