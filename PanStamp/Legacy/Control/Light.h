/*
 * Light.h
 *
 *  Created on: 13 Sep 2014
 *      Author: ensab
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#define L_RED 0
#define L_GREEN 1
#define L_BLUE 2

class Light
{
public:
	Light();
	void Spin(unsigned long time);
	void Set(float r, float g, float b, float t);
	float Red();
	float Green();
	float Blue();

private:
	int colours[3];
	int deltas[3];
	int totalSteps;
	int stepCount;
	bool colourPluses[3];
	int counters[3];
	unsigned long nextTime;
	unsigned long timeStep;
};

inline float Light::Red()
{
	return (float)colours[L_RED]/255.0;
}
inline float Light::Green()
{
	return (float)colours[L_GREEN]/255.0;
}
inline float Light::Blue()
{
	return (float)colours[L_BLUE]/255.0;
}

#endif /* LIGHT_H_ */
