/*
 * Switch.h
 *
 *  Created on: 24 Aug 2014
 *      Author: ensab
 */

#ifndef SWITCH_H_
#define SWITCH_H_

class Switch
{
public:
	Switch(int p);
	void On(float delay);
	void On(float value, float delay);
	void Off(float delay);
	bool Spin(unsigned long time);
	int Pin();

private:
	int pin;
	uint8_t value;
	bool onWaiting;
	bool analogue;
	bool offWaiting;
	bool iAmOn;
	time_t onTime;
	time_t offTime;
};

inline int Switch::Pin()
{
	return pin;
}


#endif /* SWITCH_H_ */
