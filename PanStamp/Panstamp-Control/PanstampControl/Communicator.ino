/*
 * Communicator.cpp
 *
 *  Created on: 26 Oct 2013
 *      Author: ensab
 */

//****************************************************************************************************************************


 

Communicator::Communicator(byte address)
{
  myAddress = address;
  lastReceivedFrom = 0;
  dataAvailable = false;
  panstamp.radio.setChannel(RFCHANNEL);
  panstamp.radio.setSyncWord(SYNCWORD1, SYNCWORD0);
  panstamp.radio.setDevAddress(myAddress);
  panstamp.radio.setCCregs();
  panstamp.radio.enableAddressCheck();
  panstamp.setPacketRxCallback(IncommingInterrupt);
  panstamp.rxOn();
}

void Communicator::Send(byte address, char* data)
{
  byte i = 0;
  do
  {
    if(i >= CCPACKET_DATA_LEN - 2)
    {
      message->Say("Transmit buffer overflow!");
      return;
    }
    transmitPacket.data[i+2] = data[i];
  } while(data[i++]);

  transmitPacket.data[0] = address;
  transmitPacket.data[1] = myAddress;
  transmitPacket.length = i + 2;
  panstamp.radio.sendData(transmitPacket);
  message->Debug("I sent ");
  message->Debug(address);
  message->Debug(": ");
  message->Debug((char*)&transmitPacket.data[2]);
  message->Debug("\n");
}

char* Communicator::Receive(byte& address)
{
  if(!dataAvailable)
    return 0;
  lastReceivedFrom = receivePacket->data[1];
  address = lastReceivedFrom;
  message->Debug(address);
  message->Debug(" sent me: ");
  message->Debug((char*)&receivePacket->data[2]);
  message->Debug("\n");
  return (char*)&receivePacket->data[2];
}


void Communicator::Interrupt(CCPACKET *packet)
{
  panstamp.rxOff();
  receivePacket = packet;
  dataAvailable = true;
}


