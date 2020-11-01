#include <HallEffectSpeedSensor.h>



unsigned long previousTime = 0;
unsigned long timeInterval = 100000000;

int he1pin = 3;
int he2pin = 4;
int he3pin = 5;
uint16_t ppm = 100;

int val1, val2, val3; 


HallEffectSpeedSensor he1(he1pin, ppm);
HallEffectSpeedSensor he2(he2pin, ppm);
HallEffectSpeedSensor he3(he3pin, ppm);

void setup() {
  
  Serial.begin(9600);
  Serial.println("Starting");
  he1.begin(1);
  he2.begin(1);  
  he3.begin(1);
}

void loop() {
  he1.updateSensor();
  he2.updateSensor();
  he3.updateSensor();

  unsigned long currentTime = micros();
  Serial.println("I got here 0");
    
  if (currentTime - previousTime > timeInterval) {
    Serial.println("I got here 1");

    val1 = he1.getSpeed();
    val2 = he2.getSpeed();
    val3 = he3.getSpeed();

    Serial.print("Sensor 1 speed: ");
    Serial.print(val1);
    Serial.print("  Sensor 2 speed: ");
    Serial.print(val2);
    Serial.print("  Sensor 3 speed: ");
    Serial.print(val2);
    Serial.println("The time is: "+ micros());
    previousTime = currentTime;
  }
  
  
}
