#include <SpeedSensor.h>

//600 PPR, Rotary Encoder
SpeedSensor steering_angle(2400);

double angle = 0.0;
// int32_t max = 0;

void setup() {    
  Serial.begin(9600);
  pinMode(22, INPUT);
  pinMode(23, INPUT);

  // Pins on teensy
  steering_angle.begin(22, 23);
  
  delay(100); // Good to delay for a bit just to allow hardware to initialize
}
 
void loop() {
  steering_angle.update();
  angle = (0.15 * (steering_angle.get_data().position));

  Serial.print("\nSteering Extrapolated Position: ");
  Serial.println(angle);
}
