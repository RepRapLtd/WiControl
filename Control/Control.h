// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef Control_H_
#define Control_H_
#include "Arduino.h"
//add your includes for the project Control here

#include "Communicator.h"
#include "Command.h"
#include "Control.h"

#define ABS_ZERO -273.15  // Celsius
#define TEMP_SENSE_PIN 0   // Analogue pin number
#define HEAT_ON_PIN 6
#define THERMISTOR_BETA 3480.0 // thermistor: RS 484-0149; EPCOS B57550G103J; Extruder thermistor: RS 198-961
#define THERMISTOR_SERIES_R 1000 // Ohms in series with the thermistors
#define THERMISTOR_25_R 10000.0 // Thermistor ohms at 25 C = 298.15 K
#define AD_RANGE 1023.0 // The A->D converter that measures temperatures gives an int this big as its max value
// The LED is wired to the Arduino Output 4 (physical panStamp pin 19)
#define LEDOUTPUT 4

void IncommingInterrupt();
void blink();

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project Control here




//Do not add code below this line
#endif /* Control_H_ */
