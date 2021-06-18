// Putting and getting to/from EEPROM

#ifndef FLASH
#define FLASH

#define MAX_EEPROM 512           // The number of bytes that can be stored in the EEPROM

// Check string for valid pattern in flash memory

const char* eCheckString = "uu0123456789uu";

class Flash
{
public:
  Flash();

  bool Get();
  void Put();
  void Reset();
  char* GetNextTag();
  void PutNextTag(const char* tag);
  void PutNextTag(const String tag);

private:
  bool writing;
  bool reading;
  int address;
  char buf[MAX_EEPROM];
};

#endif
