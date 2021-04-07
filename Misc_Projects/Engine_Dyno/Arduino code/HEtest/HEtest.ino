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

#define ENGINE_SPEED 5
#define ENGINE_PPR 4
#define SECONDARY_SPEED 6
#define SECONDARY_PPR 600
#define LED 13


uint32_t sns_interval = 100000;
uint32_t led_interval = 50000;
uint32_t sns_prev_time = 0;
uint32_t led_prev_time = 0;

bool led_on = 0;



HallEffectSpeedSensor he1(ENGINE_SPEED, ENGINE_PPR);
HallEffectSpeedSensor he2(SECONDARY_SPEED, SECONDARY_PPR);

void setup() {
  
  Serial.begin(9600);
  Serial.println("Starting");
  he1.setup();
  he2.setup();  
}

void loop() {
  if (abs(micros() - sns_prev_time) > sns_interval) {
    sns_prev_time = micros();

    Serial.print("Engine speed:\t");
    Serial.print(he1.get_rpm(), DEC);
    Serial.print("\tSecondary speed:\t");
    Serial.print(he2.get_rpm(), DEC);
    Serial.print("\tTime:\t");
    Serial.println(micros());
  }
  if(abs(micros() - led_prev_time) > led_interval) {
    led_on = !led_on;
    digitalWrite(13, led_on);
    led_prev_time = micros();
  }
  
  
}
