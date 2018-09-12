/*
 * Switch.cpp
 *
 *  Created on: 24 Aug 2014
 *      Author: ensab
 */

Switch::Switch(int p)
{
  pin = p;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  onWaiting = false;
  offWaiting = false;
  iAmOn = false;
  analogue = false;
  value = 0;
}

bool Switch::Spin(time_t time)
{
  if(onWaiting)
  {
    if(time >= onTime)
    {
      onWaiting = false;
      message->Debug("Switching pin ");
      message->Debug(pin);
      message->Debug(" on at ");
      TimeToString(data, DATA_LENGTH, time);
      message->Debug(data);
      message->Debug(".\n");
      iAmOn = true;
    }
  }

  if(offWaiting)
  {
    if(time >= offTime)
    {
      offWaiting = false;
      message->Debug("Switching pin ");
      message->Debug(pin);
      message->Debug(" off at ");
      TimeToString(data, DATA_LENGTH, time);
      message->Debug(data);
      message->Debug(".\n");
      iAmOn = false;
    }
  }

  if(iAmOn)
  {
    if(analogue)
      analogWrite(pin, value);
    else
      digitalWrite(pin, HIGH);
  } else
  {
    digitalWrite(pin, LOW);
  }
}

void Switch::On(float delay)
{
  if(!onWaiting)
  {
    onWaiting = true;
    analogue = false;
    onTime = now() + (time_t)delay;
    message->Debug("Setting pin ");
    message->Debug(pin);
    message->Debug(" to come on at ");
    TimeToString(data, DATA_LENGTH, onTime);
    message->Debug(data);
    message->Debug(".\n");
  }
}

void Switch::On(float val, float delay)
{
  if(!onWaiting)
  {
    onWaiting = true;
    analogue = true;
    value = (uint8_t)(255.0*val);
    onTime = now() + (time_t)delay;
    message->Debug("Setting pin ");
    message->Debug(pin);
    message->Debug(" to become value ");
    message->Debug(value);
    message->Debug(" at ");
    TimeToString(data, DATA_LENGTH, onTime);
    message->Debug(data);
    message->Debug(".\n");
  }
}

void Switch::Off(float delay)
{
  if(!offWaiting)
  {
    offWaiting = true;
    offTime = now() + (time_t)delay;
    message->Debug("Setting pin ");
    message->Debug(pin);
    message->Debug(" to go off at ");
    TimeToString(data, DATA_LENGTH, offTime);
    message->Debug(data);
    message->Debug(".\n");
  }
}



