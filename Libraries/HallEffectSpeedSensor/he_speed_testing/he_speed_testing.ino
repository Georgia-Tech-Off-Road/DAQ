#include <ElapsedCycles.h>
#include <HallEffectSpeedSensor.h>

HallEffectSpeedSensor he_sensor_1(5,20);
HallEffectSpeedSensor he_sensor_2(17,20);
HallEffectSpeedSensor he_sensor_3(18,20);
ElapsedCycles first;
ElapsedCycles second;
ElapsedCycles third;
uint32_t modul;
uint32_t temp = 0;
uint32_t temp2 = 0;
bool led_on = 0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(15, OUTPUT);
  
  he_sensor_1.setup();
  he_sensor_2.setup();
  he_sensor_3.setup();
}

void loop() {  
  uint32_t d_us = 10000;
  uint32_t t2 = micros();
  if (he_sensor_1.get_pos() < 1500000) {
    if((t2 - temp2) > d_us) {
      digitalWrite(2, HIGH);
      delayMicroseconds(1);
      digitalWrite(2, LOW);
//      Serial.println(t2 - temp2);
      temp2 = t2;
    }
  }


  uint32_t t = micros();
  if((t - temp) > 50000) {
    led_on = !led_on;
    temp = t;
    digitalWrite(13, led_on);
    Serial.println(he_sensor_1.get_rpm());

    Serial.println();
  }


  
}
