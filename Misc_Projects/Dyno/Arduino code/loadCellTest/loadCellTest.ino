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
