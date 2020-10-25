#include <HallEffectSpeedSensor.h>



unsigned long previousTime = micros();
unsigned long timeInterval = 100000;

int he1pin = 3;
int he2pin = 4;
int he3pin = 5;
uint16_t ppm = 500;

int val1, val2, val3; 


HallEffectSpeedSensor he1(he1pin, ppm);
HallEffectSpeedSensor he2(he2pin, ppm);
HallEffectSpeedSensor he3(he3pin, ppm);

void setup() {
  
  Serial.begin(9600);
  he1.begin(1);
  he2.begin(1);  
  he3.begin(1);
}

void loop() {
  
  he1.updateSensor();
  he2.updateSensor();
  he3.updateSensor();

  unsigned long currentTime = micros();
  
  if (currentTime - previousTime > timeInterval) {
    val1 = he1.getSpeed();
    val2 = he2.getSpeed();
    val3 = he3.getSpeed();

    Serial.print("Sensor 1 speed: " + val1);
    Serial.print("  Sensor 2 speed: " + val2);
    Serial.println("  Sensor 3 speed: " + val3);
    previousTime = currentTime;
  }
  
}
