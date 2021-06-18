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

const int loadOn = 1;
const int loadOff = 0;

class Load 
{
  public:
  
  Load(const int ln, const int p, Load* nxt);
  Load(const int ln, const int p);
  long NextTime();
  void SecondTick();
  int LoadNumber();
  void SwitchOnOrOff(const bool on, const long tim);
  void ActIfItsTime();
  void ChangeNumber(const int n);

  private:
  
  int loadNumber;
  int pin;
  long nextTime;
  bool iAmOn;
  long onSeconds;
  long offSeconds;
};


#endif
