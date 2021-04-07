/*
 * Hall effect Sensor for Dyno important links
 * 
 * Sensor Chip
 * https://www.digikey.com.mx/en/products/detail/ams/AS5047D-ATST/7793473
 * Magnets
 * https://www.digikey.com.mx/en/products/detail/radial-magnets-inc/9049/6030786?s=N4IgTCBcDaICwDYCcBaAjABgOwFYUDkAREAXQF8g
 * Chip datasheet
 * https://ams.com/documents/20143/36005/AS5047D_DS000394_2-00.pdf
 * Board files
 * https://github.com/GTOR-BajaSAE/DAQ/tree/he_speed_sensor/Misc_Projects/HE_Speed_Sensor
 * 
 */
#include <HallEffectSpeedSensor.h>


unsigned long previousTime = 0;
unsigned long timeInterval = 100000;

int he1pin = 2;

uint16_t teeth = 5;

int val1;

HallEffectSpeedSensor he1(he1pin, teeth);


void setup() {
  
  Serial.begin(9600);
  Serial.println("Starting");
  he1.begin(1);

}

void loop() {
  he1.updateSensor();


  unsigned long currentTime = micros();

  if (currentTime - previousTime > timeInterval) {

    val1 = he1.getSpeed();

    Serial.print("Sensor 1 speed: ");
    Serial.print(val1, DEC);
    Serial.print("   The time is: ");
    Serial.println(micros(), DEC);
    previousTime = currentTime;
  }
  
  
}
