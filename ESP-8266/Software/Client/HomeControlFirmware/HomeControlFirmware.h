/*
 * Program to run a node on the WiFi Home Control system.
 * 
 * This makes periodic requests to the server to report its state (on or off,
 * current temperature etc) and to find out what to do next.
 * 
 * The general form of a request is (N.B. "/" caracters should be in the strings):
 * 
 * GET http://server/pageRoot/page/?unit=1&load=0&temperature=20&debugOn=1
 * 
 * See local_wifi.h below for where server, pageRoot, page and unit are defined.
 * 
 * Select 
 * 
 *   Board: WeMos D1 R1 
 * 
 * to compile in the Arduion IDE.
 * 
 * Adrian Bwowyer
 * RepRap Ltd
 * http://reprapltd.com
 * 
 * 5 September 2018
 * 
 * Licence: GPL
 * 
 * 
 */

#ifndef HOME_CONTROL_FIRMWARE
#define HOME_CONTROL_FIRMWARE

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

//-----------------------------------------------------------------------------------------------------

// User configuration area

// The unit number must be unique across the whole system

const int unit = 36;

/*

The file local_wifi.h included below should contain five lines:

const char* ssid = "-------";                 // The name of your WiFi network
const char* password = "--------";            // Your WiFi network's password
const String server = "myURL.com";            // Server IP address/URL
const String pageRoot = "/WiFiHeating/";      // Where the .php script is on the server, usually [myURL.com]/WiFiHeating/
const String backupServer = "192.168.1.100";  // Backup server IP address/URL

*/

#include "local_wifi.h" // Separated to prevent passwords appearing on Github

const String page = "scontrollednode.php";        // The PHP script that interprets the heating tables and decides between ON or OFF
const int loadCount = 1;                          // The number of loads this device drives; usually 1; must be 5 or less

const int loadOn = 1;
const int loadOff = 0;

// End of user configuration area

//-----------------------------------------------------------------------------------------------------

/*
  * WeMos D1 R1 pins from .arduino15/packages/esp8266/hardware/esp8266/2.4.2/variants/d1/pins_arduino.h
  * The numbers are GPIO numbers.
  * 
  * #define LED_BUILTIN 2 // D9
  *
  * static const uint8_t D0   = 3;
  * static const uint8_t D1   = 1;
  * static const uint8_t D2   = 16;
  * static const uint8_t D3   = 5;  // SCL
  * static const uint8_t D4   = 4;  // SDA
  * static const uint8_t D5   = 14; // SCK
  * static const uint8_t D6   = 12; // MISO (not top pin 10 below; Arduino def)
  * static const uint8_t D7   = 13; // MOSI (not top pin 9 below; Arduino def)
  * static const uint8_t D8   = 0;
  * static const uint8_t D9   = 2;
  * static const uint8_t D10  = 15;
  * static const uint8_t D11  = 13; // = D7
  * static const uint8_t D12  = 12; // = D6
  * static const uint8_t D13  = 14; // = D5
  * static const uint8_t D14  = 4;  // = D4
  * static const uint8_t D15  = 5;  // = D3
  *
  *
  * Pins looking down on top, aerial at the bottom, outside numbers are GPIO numbers
  * 
  *                     M  M
  *         C  I        I  O
  *         L  N  1     S  S
  *         K  T  0  9  O  I
  *        
  *         14 13 12 11 10 9
  *  GND 15                   8  VCC
  *  15  16                   7  13
  *   2  17                   6  12
  *   0  18                   5  14
  *   4  19                   4  16
  *   5  20                   3  EN
  *   3  21                   2  ADC/A0
  *   1  22                   1  RST
  *                     |
  *          \/\/\/\/\/\/ 
  *          
  *   The MOSFET is driven by GPIO5/Pin-20/D3/OUTPUT_PIN_0.
  *   The 5-pin header is:
  *   
  *     1 Pin-7/GPIO13/D7/OUTPUT_PIN_2
  *     2 Pin-6/GPIO12/D6/USER_LED_PIN
  *     3 Pin-5/GPIO14/D5/debug
  *     4 Pin-4/GPIO16/D2/OUTPUT_PIN_1
  *     5 GND
  */

#define MAX_LOADS 3
#define ESP8266_LED_PIN LED_BUILTIN // GPIO2/D9 ESP8266 internal LED; D4 on Wemos D1 R2
#define USER_LED_PIN D6             // GPIO12 - Front panel LED
//#define USER_LED_PIN -1             // GPIO12 - Front panel LED
#define OUTPUT_PIN_0 D3             // GPIO5 This is the switched MOSFET/relay
#define OUTPUT_PIN_1 D2             // GPIO16
#define OUTPUT_PIN_2 D7             // GPIO13
#define OUTPUT_PIN_3 9              // GPIO9 - direct wire to top of ESP8266
#define OUTPUT_PIN_4 10             // GPIO10 - direct wire to top of ESP8266 
#define OUTPUT_PIN_5 D6             // GPIO12 - If you use this (i.e. loadCount = 6) you must set USER_LED_PIN to -1
#define THERMISTOR_BETA 3528.0      // thermistor: RS 538-0806
#define THERMISTOR_SERIES_R 10000   // Ohms in series with the thermistor
#define THERMISTOR_25_R 1000.0      // Thermistor ohms at 25 C = 298.15 K
#define TOP_VOLTAGE 3.303           // The voltage at the top of the series resistor
#define MAX_AD_VOLTAGE 1.0          // The voltage that gives full-range (i.e. AD_RANGE - see below) on the A->D converter
#define DEBUG_PIN D5                // D5 Ground this pin to turn debugging on

const int outputPins[MAX_LOADS] = {OUTPUT_PIN_0, OUTPUT_PIN_1, OUTPUT_PIN_2};//, OUTPUT_PIN_3, OUTPUT_PIN_4, OUTPUT_PIN_5};

const long debugSampleTime = 15000*loadCount;   // Milliseconds between server requests when debugging
const long debugRandomTime = 2000;    // +/- Milliseconds (must be < sampleTime) used to randomise requests to reduce clashes
const long sampleTime = 60000;        // Milliseconds between server requests
const long randomTime = 5000;         // +/- Milliseconds (must be < sampleTime) used to randomise requests to reduce clashes
const long rebootTime = 3600000;      // Milliseconds between resets.
const long initialTime = 5000;        // Milliseconds to first server request

//#define BAUD 115200                   // Serial comms speed
#define BAUD 9600

const int version = 7;

// Bits of HTML we need to know (both cases of these are tried in atempting matches)

const String bodyStart = "<body>";        // The instructions are in the body of the loaded page
const String bodyEnd = "</body>";
const String htmlBreak = "<br>";

// The thermistor for measuring temperature

#define ABS_ZERO -273.15           // Celsius
#define TEMP_SENSE_PIN 0           // Analogue pin number
#define AD_RANGE 1023.0            // The A->D converter that measures temperatures gives an int this big as its max value


// LED control and blinking 

#define OFF 1 // N.B. ESP8266 LED is inverted; explicit #define is clearer than enum here.
#define ON 0
#define DOT 2
#define DASH 3
#define FLASH 4
const int dotOn = 100;
const int dotOff = 500;
const int dashOn = 500;
const int dashOff = 100;
const int flashOn = 100;
const int flashOff = 100;

// Prototype needed by the Load class

long TillNextTime();

void PrintStatus();

#endif
