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
  
  Load(const int ln, const int p, Load* nxt);
  Load(const int ln, const int p);
  //Load* Next();
  //void SetNext(Load* n);
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
  //Load* next;
};

//inline void Load::SetNext(Load* n)
//{
//  next = n;
//}


#endif
