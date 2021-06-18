

#include <EEPROM.h>

void setup() 
{
  EEPROM.begin(512);
  for(int address = 0; address < 512; address++)
    EEPROM.write(address, 0);
  EEPROM.commit();
  Serial.begin(9600);
  Serial.println("\n\nEEPROM erased.");
}

void loop() {
  // put your main code here, to run repeatedly:

}
