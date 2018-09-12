#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
float Ptemp;
float Stemp;
int PumpPin = 12;
int ValvePin = 11;
long interval = 700; // cycle delay for main loop
long previousMillis = 0;

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
// Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!


// Temp sensor Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
// arrays to hold device addresses
DeviceAddress InThermometer = { 0x28, 0xFF, 0xD2, 0xC3, 0xA4, 0x16, 0x04, 0x6B }; // define address for Pool sensor
DeviceAddress OutThermometer = { 0x28, 0xFF, 0x8D, 0x49, 0xA4, 0x16, 0x05, 0x0D }; // define address for solar sensor

void setup(void)
{
  pinMode(PumpPin, OUTPUT);
  pinMode(ValvePin, OUTPUT);
  // start serial port
  Serial.begin(9600);
  //Serial.println("Dallas Temperature IC Control Library Demo");
  display.begin();
  display.setContrast(50);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("FiddleTech");
  display.setCursor(0, 10);
  display.println("Solar Control");
  delay(200);
  display.setCursor(0, 25);
  display.println("Starting Pump");
  display.display();
  digitalWrite(PumpPin, HIGH); 
  digitalWrite(ValvePin, HIGH); // start pump / open valve at system start to help system prime
  display.setCursor(0, 35);
  display.println("Wait 5s for prime");
  delay(8000); //wait 5 seconds for pump to hopefully prime.

  // Start up the DallasTemperature library
  sensors.begin();
}


void loop(void)
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;

    sensors.requestTemperaturesByAddress(InThermometer); // get pool temps
    sensors.requestTemperaturesByAddress(OutThermometer);// get Solar temps
    Ptemp = sensors.getTempC(InThermometer); // pool temperature
    Stemp = sensors.getTempC(OutThermometer); // solar water temperature

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Temperatures");
    display.setCursor(0, 10);
    display.println("In Pool");
    display.setCursor(45, 10);
    display.println(Ptemp);
    display.setCursor(0, 20);
    display.println("Solar");
    display.setCursor(45, 20);
    display.println(Stemp);
    display.display();

    if ((Stemp - 1) > Ptemp) // if solar is 1c or more above pool temp turn on pump/valve
    {
      display.setCursor(0, 30);
      digitalWrite(PumpPin, HIGH);
      digitalWrite(ValvePin, HIGH);
      display.println("Pump/Valve ON");

    }
    else
    {
      display.setCursor(0, 30);
      digitalWrite(PumpPin, LOW);
      digitalWrite(ValvePin, LOW);
      display.println("Pump/Valve OFF");
    }
    display.display();
  }
}

