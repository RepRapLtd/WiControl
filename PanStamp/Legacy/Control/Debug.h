/*
 * Debug.h
 *
 *  Created on: 31 Jan 2016
 *      Author: ensab
 */

#ifndef DEBUG_H_
#define DEBUG_H_

class Message
{
public:
	Message();
	void DebugOn();
	void DebugOff();
	void Debug(char* string);
	void Debug(int i);
	void Debug(float f);
	void Debug(unsigned long l);
	void Say(char* string);
	void Say(int i);
	void Say(float f);
	void Say(unsigned long l);

private:
	bool debug;
};



#endif /* DEBUG_H_ */
