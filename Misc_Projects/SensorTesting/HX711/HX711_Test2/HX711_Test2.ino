#include "HX711.h"
//#include <SD.h>


// HX711 circuit wiring
const int LOADCELL_SDA_PIN = 18;
const int LOADCELL_SCK_PIN = 19;

//File dataFile;
//const int chipSelect = BUILTIN_SDCARD;
//bool hasSD = false; //Keep false, will auto-set to true if it detects an SD card present
//uint16_t sdInterval     = 50  * 1000; //us  (50  ms)
//uint64_t sdTime = micros();

HX711 scale;

void setup() {
  Serial.begin(115200);
  // Initialize library with data output pin, clock input pin and gain factor.
  scale.begin(LOADCELL_SDA_PIN, LOADCELL_SCK_PIN);

//  if (!SD.begin(chipSelect)) {
//    Serial.println("SD card initialization failed");
//  } else {
//    Serial.println("SD card initialization successful");
//    hasSD = true;
//  }
  delay(1000);
  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0

}

void loop() {
  //Serial.print("one reading:\t");
  Serial.println(scale.get_units(), 1);
  
//  if (abs(micros() - sdTime) > sdInterval && hasSD){
//    dataFile = SD.open("COFTEST1.txt", FILE_WRITE); //---------CHANGE FILE NAME HERE---------//
//    if (dataFile) {
//      Serial.println(scale.get_units(), 1);
//      dataFile.print(scale.get_units(), 1);
//      dataFile.println();
//    } else {
//      Serial.println("Error opening test.txt");
//    }
//    dataFile.close();
//    sdTime = micros();
//  }  

  //scale.power_down();			        // put the ADC in sleep mode
  //delay(5);
  //scale.power_up();
}
