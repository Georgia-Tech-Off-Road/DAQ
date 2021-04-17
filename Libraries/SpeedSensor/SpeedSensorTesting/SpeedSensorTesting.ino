#include <SpeedSensor.h>

SpeedSensor he_sensor_1(600, 5, 255, RISING);




uint32_t temp = 0;
uint32_t temp2 = 0;
bool led_on = 0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13, OUTPUT);

  delay(100);

  uint8_t hi = -1;
  Serial.println(hi);
  Serial.println(hi==255);
  Serial.println(CHANGE);
  Serial.println(RISING);
}

void loop() {  



  uint32_t t = micros();
  if((t - temp) > 50000) {
    led_on = !led_on;
    temp = t;
    digitalWrite(13, led_on);
    Serial.println(he_sensor_1.get_position());
  }


  
}
