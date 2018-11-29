/*
 * Class that saves and loads things from permanent memory.
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
 * 
 * See: http://www.aroundincircles.com/2016/08/17/a-quick-guide-to-using-the-flash-on-the-esp8266/
 * 
 */

#ifndef FLASH
#define FLASH

#include "spi_flash.h"

// Flash read and write (a sector is 4096 bytes long).
// We must read and write in 4-byte (uint32) chunks.

#define FLASH_SECTOR 0x8c
#define FLASH_RW 0x8c000
#define FLASH_TOP 1024      // = 4096/4, i.e. 1 sector

class Flash
{
public:

  Flash();

  // Write a string to flash at the current flashPointer adding a trailing 0 so it's like a char array.
  void Cat(String s);

  // Get the next string from the flash array
  String NextString();

  // Actually save flash into the flash sector
  void Save();

  // Load flash from the flash sector
  void Load();

  // Clear it to start recording again.
  void Clear();

private:

  // Add a single character to the flash array
  // NB does not check for buffer overflow; assumed to have been done.
  void CatByte(char c);

  // Get the next single character from the flash array
  char GetByte();

  uint32 flash[FLASH_TOP];
  unsigned int flashPointer = 0;   // NB, this goes from 0 to 4*FLASH_TOP and we can write and read in the middle of 32 bit words in flash
};


#endif
