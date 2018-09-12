/*
 * Line.h
 *
 *  Created on: Jan 13, 2014
 *      Author: ensab
 */

#ifndef LINE_H_
#define LINE_H_

using namespace std;
using namespace LibSerial ;

class Line
{

public:
	Line(char* port);
	bool ByteAvailable();
	char GetByte();
	void PutByte(char byte);
	void PutString(char* string);
	bool Valid();

private:
	SerialStream serial_port ;
	bool valid;
	char xmitString[LINE_LENGTH];
	char rcvString[LINE_LENGTH];
	int xmitP;
	int rcvP;
};

#endif /* LINE_H_ */
