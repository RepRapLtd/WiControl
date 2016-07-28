// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef Control_H_
#define Control_H_

#define WORKSHOP
//#define A_AND_C
//#define S_AND_B

#define MY_ADDRESS 8
#define HOST 1
#define PLACE "Test"
#define NAME "WiControl"
#define DATE "2016-07-23"
#define VERSION "0.2"

#define BAUD_RATE 9600

// Uncomment this to enable LED control
//#define USE_LEDS

#define DATA_LENGTH 60

// The LED is wired to the Arduino Output 4 (physical panStamp pin 19)
#define FLASHOUTPUT 4

#ifdef USE_LEDS
// RGB PWM outputs
#define RED_PWM 6
#define GREEN_PWM 5
#define BLUE_PWM 9
#endif

#define RESET_SECONDS 90

#define ABS_ZERO -273.15  // Celsius
#define TEMP_SENSE_PIN 0   // Analogue pin number

// Outputs for controlling various things.  D_OUTPUT_1 is the MOSFET
// D_OUTPUT_2 is the first output on the 7 pin header, which from the top goes:
// 12v, Gnd, D8, D9, A3 = D17, A1 = D15, A2 = D16
// D_OUTPUT_3 is not available if LEDS are being controlled

#ifndef USE_LEDS
 #define SWITCHES 7
 #define OUT_PINS {3, 8, 9, 17, 15, 16, FLASHOUTPUT}
 #define FLASH 6
 #define RED FLASH
 #define GREEN FLASH
 #define BLUE FLASH
 //#define WATCHDOG
#else
 #define SWITCHES 9
 #define OUT_PINS {3, 8, 17, 15, 16, RED_PWM, GREEN_PWM, BLUE_PWM, FLASHOUTPUT};
 #define FLASH 8
 #define RED 5
 #define GREEN 6
 #define BLUE 7
#endif

#define THERMISTOR_BETA 3528.0 // thermistor: RS 538-0806
#define THERMISTOR_SERIES_R 1000 // Ohms in series with the thermistors
#define THERMISTOR_25_R 1000.0 // Thermistor ohms at 25 C = 298.15 K
#define AD_RANGE 1023.0 // The A->D converter that measures temperatures gives an int this big as its max value


void IncommingInterrupt();
void blink();

class Switch;
class Message;

extern Switch* switches[SWITCHES];
extern Message* message;

#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

#endif /* Control_H_ */

