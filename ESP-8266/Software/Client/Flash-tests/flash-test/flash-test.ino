
#include "spi_flash.h"

static void ICACHE_FLASH_ATTR test() 
{
    char s[1000];
    uint32 temp[4]={122323, 13, 14, 15};
    uint32 temp1[4]={0};

/*    int i = spi_flash_erase_sector(0x8c);
    sprintf(s, "spi_flash_erase_sector: %d\n", i);
    Serial.println(s);
    
    int o = spi_flash_write(0x8c000, temp, sizeof(temp));
    sprintf(s,"spi_flash_write: %d\n", o);
    Serial.println(s);
 */   
    int p = spi_flash_read(0x8c000, temp1, sizeof(temp1));
    sprintf(s, "spi_flash_read: %d\n", p);
    Serial.println(s);
    
    sprintf(s, "read :%d, %d, %d, %d\n", temp1[0], temp1[1], temp1[2], temp1[3]);
    Serial.println(s);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  test();
}

void loop() {
  // put your main code here, to run repeatedly:

}
