/*
 * Communicator.h
 *
 *  Created on: 26 Oct 2013
 *      Author: ensab
 */

#ifndef COMMUNICATOR_H_
#define COMMUNICATOR_H_

class Communicator
{
public:
  Communicator(byte address);
  void Send(byte address, char* data);
  char* Receive(byte& address);
  void Interrupt(CCPACKET *packet);
  void FreeReadData();

private:
  CC1101* cc1101;
  CCPACKET transmitPacket;
  CCPACKET* receivePacket;
  volatile bool dataAvailable;
  byte networkAddress[2];
  byte myAddress;
  byte lastReceivedFrom;
};

inline void Communicator::FreeReadData()
{
  dataAvailable = false;
  panstamp.rxOn();
}

#endif /* COMMUNICATOR_H_ */

