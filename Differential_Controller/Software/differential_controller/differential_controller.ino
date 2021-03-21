#include "Differential_Controller.h"
#include "HallEffectSpeedSensor.h"


#define PIN_DIFF1 14
#define PIN_DIFF2 15
#define PIN_DIFF3 16
#define PIN_DIFF4 17
#define PIN_DIFF5 18
#define PIN_DIFF6 19



#define PIN_SWITCHLEFT 4
#define PIN_SWITCHRIGHT 5


#define PIN_HE1 2
#define PIN_HE2 3

#define PIN_MOTORPOS 20
#define PIN_MOTORNEG 21



HallEffectSpeedSensor he_sensor1(PIN_HE1, 50);
HallEffectSpeedSensor he_sensor2(PIN_HE2, 50);
Differential_Controller diff_controller(PIN_DIFF1, PIN_DIFF2, PIN_DIFF3, PIN_DIFF4, PIN_DIFF5, PIN_DIFF6, PIN_MOTORPOS, PIN_MOTORNEG, PIN_SWITCHLEFT, PIN_SWITCHRIGHT, he_sensor1, he_sensor2);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  diff_controller.setup();
  pinMode(13, OUTPUT);
  
}

uint32_t temp = 0;
bool led_on = 0;

void loop() {
  // put your main code here, to run repeatedly:
  
//  if (Serial.available() > 0)
//  {
//    uint8_t incomingData = Serial.read();
//    Serial.println(incomingData);
//    if ((incomingData<=8) && (incomingData >=1))
//    {
//      diff_controller.change_state(incomingData);
//    } 
//  }
//  
  uint32_t t = micros();
  if((t - temp) > 500000) {
    led_on = !led_on;
    temp = t;
    digitalWrite(13, led_on);
  }
//  diff_controller.update();
  diff_controller.rotate_R();
}
