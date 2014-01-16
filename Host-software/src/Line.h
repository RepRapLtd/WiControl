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
	Line();
	bool ByteAvailable();
	char GetByte();
	void PutByte(char byte);
	void PutString(char* string);

private:
	SerialStream serial_port ;

};

#endif /* LINE_H_ */
