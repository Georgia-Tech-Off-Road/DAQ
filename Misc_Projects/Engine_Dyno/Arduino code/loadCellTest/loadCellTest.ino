/*
 * Load Cell for Dyno important links
 * 
 * Load Cell
 * https://www.digikey.com/en/products/detail/te-connectivity-measurement-specialties/FX1901-0001-0200-L/5277314
 * Load Cell Amplifier
 * https://www.amazon.com/SparkFun-Qwiic-Scale-chip-temperature/dp/B082MPLRCS/ref=sr_1_7?dchild=1&keywords=SparkFun+Qwiic&qid=1602027887&sr=8-7
 * Documentation
 * https://learn.sparkfun.com/tutorials/qwiic-scale-hookup-guide?_ga=2.89089179.1660775929.1612854803-1613491761.1610674487
 */
#include <Wire.h>
#include <SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h>


NAU7802 load;
void setup() {
  Serial.begin(9600);
  Serial.println("Qwiic Scale Example");
  
  Wire.begin();
  
  if (load.begin() == false)
  {
    Serial.println("Scale not detected. Please check wiring. Freezing...");
  }
  Serial.println("Scale detected!");
  
  load.setSampleRate(NAU7802_SPS_320); //Sample rate can be set to 10, 20, 40, 80, or 320Hz

  load.calibrateAFE(); //Does an internal calibration. Recommended after power up, gain changes, sample rate changes, or channel changes.

}

void loop() {
  
  if(load.available() == true)
  {
    long currentReading = load.getReading();
    Serial.print("Reading: ");
    Serial.println(currentReading);
  }
}
