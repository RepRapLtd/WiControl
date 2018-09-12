#include <ESP8266WiFi.h>

#define ABS_ZERO -273.15  // Celsius
#define TEMP_SENSE_PIN 0   // Analogue pin number

#define THERMISTOR_BETA 3528.0 // thermistor: RS 538-0806
#define THERMISTOR_SERIES_R 1000 // Ohms in series with the thermistors
#define THERMISTOR_25_R 1000.0 // Thermistor ohms at 25 C = 298.15 K
#define AD_RANGE 1023.0 // The A->D converter that measures temperatures gives an int this big as its max value
 

 
int ledPin = 2; // D4 on later PCBs?
int heatPin = D2;
//WiFiServer server(80);
bool debug = true;

long checkTime=10000; // Milliseconds
float switchTemperature = 28.0;


float Temperature()
{
  float r = (float)analogRead(TEMP_SENSE_PIN);
  return ABS_ZERO + THERMISTOR_BETA/log( (r*THERMISTOR_SERIES_R/(AD_RANGE - r))/
      ( THERMISTOR_25_R*exp(-THERMISTOR_BETA/(25.0 - ABS_ZERO)) ) );
}

void control(boolean on)
{
  if(on)
  {
    digitalWrite(ledPin, LOW);
    digitalWrite(heatPin, HIGH);
    if(debug) Serial.println("On");
  } else
  {
    digitalWrite(ledPin, HIGH);
    digitalWrite(heatPin, LOW);
    if(debug) Serial.println("Off");
  }
}
 
void setup() 
{
  if(debug)
  {
    Serial.begin(9600);
     while (!Serial); // wait for serial port to connect. Needed for Leonardo only
  }
 
  pinMode(ledPin, FUNCTION_3);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  pinMode(heatPin, OUTPUT);
  digitalWrite(heatPin, LOW);
  //wdt_enable(WDTO_8S);


}


 
void loop() 
{

  float t = Temperature();

  if(debug)
  {
    Serial.print("Temp: ");
    Serial.println(t);
  }
  
  if (t > switchTemperature) 
  {
    control(true);
  } else
  {
    control(false);
  }

  delay(checkTime);
 
}
