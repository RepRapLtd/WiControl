/*
 * Communicator.h
 *
 *  Created on: 26 Oct 2013
 *      Author: ensab
 */

#ifndef COMMUNICATOR_H_
#define COMMUNICATOR_H_

#ifdef WORKSHOP
 #define SYNCWORD1 199
 #define SYNCWORD0 0
 #define RFCHANNEL 0
#endif

#ifdef A_AND_C
 #define SYNCWORD1 198
 #define SYNCWORD0 0
 #define RFCHANNEL 1
#endif

#ifdef S_AND_B
 #define SYNCWORD1 197
 #define SYNCWORD0 0
 #define RFCHANNEL 2
#endif

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

