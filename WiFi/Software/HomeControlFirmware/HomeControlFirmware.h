/*
 * Program to run a node on the WiFi Home Control system.
 * 
 * This makes periodic requests to the server to report its state (on or off,
 * current temperature etc) and to find out what to do next.
 * 
 * The general form of a request is (N.B. "/" caracters should be in the strings):
 * 
 * GET http://currentServer/pageRoot/page/?messageString
 * 
 * where messageString is something like
 * 
 * building=Workshop&location=Office&temperature=20&debugOn=1
 * 
 * Select 
 * 
 *   Board: WeMos D1 R1 
 * 
 * to compile.
 * 
 * Adrian Bwowyer
 * RepRap Ltd
 * http://reprapltd.com
 * 
 * Version 2
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


/*

The file local_wifi.h included below should contain two lines:

const char* ssid = "-------";      // The name of your WiFi network
const char* password = "--------"; // Your WiFi network's password

*/

#include "local_wifi.h" // Separated to prevent passwords appearing on Github

#define WIFIBOARD-V2
//#define WEMOS1

#ifdef WIFIBOARD-V2
 #define ESP8266_LED_PIN 2           // ESP8266 internal LED; D4 on later PCBs?
 #define USER_LED_PIN D6             // GPIO 12 - Front panel LED
 #define OUTPUT_PIN_0 D3             // GPIO5 This is the switched MOSFET/relay
 #define THERMISTOR_BETA 3528.0      // thermistor: RS 538-0806
 #define THERMISTOR_SERIES_R 10000   // Ohms in series with the thermistor
 #define THERMISTOR_25_R 1000.0      // Thermistor ohms at 25 C = 298.15 K
 #define TOP_VOLTAGE 3.303           // The voltage at the top of the series resistor
 #define MAX_AD_VOLTAGE 1.0          // The voltage that gives full-range (i.e. AD_RANGE - see below) on the A->D converter
 #define T_CORRECTION 1.5            // Final fudge to get it just right/variation in beta from spec
 #define DEBUG_PIN D5                // Ground this pin to turn debugging on
#endif

#ifdef WEMOS1
 #define ESP8266_LED_PIN 2            // D4 on later PCBs?
 #define USER_LED_PIN D6              // GPIO 12 - Front panel LED
 #define OUTPUT_PIN_0 D2              // This is the switched MOSFET/relay                  
 #define THERMISTOR_BETA 3528.0       // thermistor: RS 538-0806
 #define THERMISTOR_SERIES_R 1000     // Ohms in series with the thermistor
 #define THERMISTOR_25_R 1000.0       // Thermistor ohms at 25 C = 298.15 K
 #define TOP_VOLTAGE 3.3              // The voltage at the top of the series resistor
 #define MAX_AD_VOLTAGE TOP_VOLTAGE   // The voltage that gives full-range (i.e. AD_RANGE - see below) on the A->D converter
 #define T_CORRECTION 6               // Final fudge to get it just right/variation in beta from spec 
 #define DEBUG_PIN D5                 // Ground this pin to turn debugging on
#endif

const long debugSampleTime = 15000;   // Milliseconds between server requests when debugging
const long debugRandomTime = 2000;    // +/- Milliseconds (must be < sampleTime) used to randomise requests to reduce clashes
const long sampleTime = 60000;        // Milliseconds between server requests
const long randomTime = 5000;         // +/- Milliseconds (must be < sampleTime) used to randomise requests to reduce clashes
const long rebootTime = 3600000;      // Milliseconds between resets.

#define BAUD 9600     // Serial comms speed

const int version = 2;
const int numberOfLocations = 1;                  // How many things am I controlling? (Max 4 at the moment)
const String l0 = "ElectronicsLab";               // What room/device am I controlling?
const String building = "Workshop";               // Which building is the device in?
const String pageRoot = "/WiFiHeating/";          // Where the .php script is on the server
const String page = "controllednode.php";         // The script we need
const String server = "adrianbowyer.com";         // Server IP address/URL
const String backupServer = "192.168.1.171";      // Backup server IP address/URL

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

long NextTime();

#endif
