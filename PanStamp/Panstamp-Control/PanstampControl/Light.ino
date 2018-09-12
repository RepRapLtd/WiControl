/*
 * Light.cpp
 *
 *  Created on: 13 Sep 2014
 *      Author: ensab
 */

Light::Light()
{
  for (uint8_t led = 0; led < 3; led++)
  colours[led] = 0;
  nextTime = millis();
  totalSteps = 0;
  stepCount = 0;
  timeStep = 0;
}

void Light::Spin(unsigned long time)
{
  if(stepCount >= totalSteps || time < nextTime)
    return;


  for(int8_t led = 0; led < 3; led++)
  {
    counters[led] += deltas[led];
    if(counters[led] > 0)
    {
      if(colourPluses[led])
        colours[led]++;
      else
        colours[led]--;
      if(colours[led] > 255)
        colours[led] = 255;
      if(colours[led] < 0)
        colours[led] = 0;
      counters[led] -= totalSteps;
    }
  }

  switches[RED]->On((float)(colours[L_RED]*255), 0.0);
  switches[GREEN]->On((float)(colours[L_GREEN]*255), 0.0);
  switches[BLUE]->On((float)(colours[L_BLUE]*255), 0.0);

  nextTime = time + timeStep;
  stepCount++;
}

void Light::Set(float r, float g, float b, float t)
{
  totalSteps = 0;
  stepCount = 0;
  int targets[3];
  targets[L_RED] = (int)(r*255.0);
  targets[L_GREEN] = (int)(g*255.0);
  targets[L_BLUE] = (int)(b*255.0);
  uint8_t led;

  for(led = 0; led < 3; led++)
  {
    deltas[led] = targets[led] - colours[led];
    if(deltas[led] < 0)
    {
      colourPluses[led] = false;
      deltas[led] = -deltas[led];
    } else
      colourPluses[led] = true;
    if(deltas[led] > totalSteps)
      totalSteps = deltas[led];
  }


  counters[0] = -totalSteps/2;
    for(led = 1; led < 3; led++)
      counters[led] = counters[0];

  timeStep = (unsigned long)(t*1000.0/(float)totalSteps);
  nextTime = millis();
}

