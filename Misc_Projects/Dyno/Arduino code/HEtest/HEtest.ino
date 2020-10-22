#include <HallEffectSpeedSensor.h>



unsigned long previousTime = micros();
unsigned long timeInterval = 100000;

int he1pin = 12;
uint8_t ppm = 500;

int val; 


HallEffectSpeedSensor he1(he1pin, ppm);

void setup() {
  
  Serial.begin(9600);
  he1.begin(1);
  
}

void loop() {
  
  he1.updateSensor();
  unsigned long currentTime = micros();
  
  if (currentTime - previousTime > timeInterval) {
    val = he1.getSpeed();
    Serial.println("The speed is " + val);
    previousTime = currentTime;
  }
  
}
