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
}

void loop() {
  
  if(load.available() == true)
  {
    long currentReading = load.getReading();
    Serial.print("Reading: ");
    Serial.println(currentReading);
  }
}
