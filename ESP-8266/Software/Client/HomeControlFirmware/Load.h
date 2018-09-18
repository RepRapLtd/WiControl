/*
 * Class that controls and administers a load, such as a radiator valve.
 * 
 * Adrian Bwowyer
 * RepRap Ltd
 * http://reprapltd.com
 * 
 * Version 1
 * 10 September 2018
 * 
 * Licence: GPL
 * 
 * 
 */

#ifndef LOAD
#define LOAD

class Load 
{
  public:
  
  Load(const String l, int p, Load* n);
  Load* Next();
  long NextTime();
  void SecondTick();
  String Location();
  void SwitchOnOrOff(bool on, long tim);
  void ActIfItsTime();
  void ChangeLocation(String l);

  private:
  
  String location;
  int pin;
  long nextTime;
  bool iAmOn;
  long onSeconds;
  long offSeconds;
  Load* next;
};


#endif
