//Thermocouple presetup code
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include "Adafruit_MCP9600.h"

#define I2C_ADDRESS (0x67)

Adafruit_MCP9600 mcp;

// Load Cell Amplifier presetup code
#include <SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h>
NAU7802 load;

// HE speed sensor code
#include <HallEffectSpeedSensor.h>
unsigned long previousTime = 0;
unsigned long timeInterval = 100000;

int he1pin = 3;
int he2pin = 4;
int he3pin = 5;
uint16_t ppm = 100;

int val1, val2, val3; 



HallEffectSpeedSensor he1(he1pin, ppm);
HallEffectSpeedSensor he2(he2pin, ppm);
HallEffectSpeedSensor he3(he3pin, ppm);
 
void setup() {
    //Thermocouple setup code
    Serial.begin(115200);
    while (!Serial) {
      delay(10);
    }
    Serial.println("Starting Engine Dyno sensor");

    if (! mcp.begin(I2C_ADDRESS)) {
        Serial.println("Sensor not found. Check wiring!");
        while (1);
    }
    mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
    mcp.setThermocoupleType(MCP9600_TYPE_K);
    mcp.setFilterCoefficient(3);
    mcp.setAlertTemperature(1, 50);
    mcp.configureAlert(1, true, true);  // alert 1 enabled, rising temp
    mcp.enable(true);

    //HE sensor setup code
    he1.begin(1);
    he2.begin(1);  
    he3.begin(1);

    //Load Cell setup code
      Wire.begin();
  
      if (load.begin() == false)
      {
          Serial.println("Scale not detected. Please check wiring. Freezing...");
      }
      
      load.setSampleRate(NAU7802_SPS_320); //Sample rate can be set to 10, 20, 40, 80, or 320Hz
    
      load.calibrateAFE(); //Does an internal calibration. Recommended after power up, gain changes, sample rate changes, or channel changes.

}

void loop() {
    //Thermocouple Code loop
    Serial.print(mcp.readThermocouple());
    //HEtest
      he1.updateSensor();
      he2.updateSensor();
      he3.updateSensor();
    
      unsigned long currentTime = micros();
    
      if (currentTime - previousTime > timeInterval) {
    
          val1 = he1.getSpeed();
          val2 = he2.getSpeed();
          val3 = he3.getSpeed();
      
          Serial.print("Sensor 1 speed: ");
          Serial.print(val1, DEC);
          Serial.print("  Sensor 2 speed: ");
          Serial.print(val2, DEC);
          Serial.print("  Sensor 3 speed: ");
          Serial.print(val3, DEC);
          Serial.print("   The time is: ");
          Serial.println(micros(), DEC);
          previousTime = currentTime;
      }
      //Load Cell loop code
      if(load.available() == true)
      {
          long currentReading = load.getReading();
          Serial.print("Reading: ");
          Serial.println(currentReading);
      }
}
