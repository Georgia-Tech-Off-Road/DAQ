#include "Differential_Controller.h"
#include <Single_MAX14870_MotorShield.h>
#include <HallEffectSpeedSensor.h>

//#define PIN_BUILTINLED 13
//#define PIN_DIFF1 14
//#define PIN_DIFF2 15
//#define PIN_DIFF3 16
//#define PIN_DIFF4 17
//#define PIN_DIFF5 18
//#define PIN_DIFF6 19
//
//#define PIN_PWM 23
//#define PIN_DIR 22
//#define PIN_NEN 20
//#define PIN_NFAULT 21
//
//#define PIN_HE1 53
//#define PIN_HE2 54
//
//#define pin_switchLeft 5
//#define pin_switchRight 4

#define PIN_DIFF1 12
#define PIN_DIFF2 11
#define PIN_DIFF3 10
#define PIN_DIFF4 9
#define PIN_DIFF5 8
#define PIN_DIFF6 7

#define PIN_PWM 2
#define PIN_DIR 3
#define PIN_NEN 5
#define PIN_NFAULT 4

#define PIN_HE1 53
#define PIN_HE2 54

#define pin_switchLeft 14
#define pin_switchRight 15



Single_MAX14870_MotorShield motor_shield(PIN_PWM, PIN_DIR, PIN_NEN, PIN_NFAULT);
HallEffectSpeedSensor he_sensor1(PIN_HE1, 50);
HallEffectSpeedSensor he_sensor2(PIN_HE2, 50);
Differential_Controller diff_controller(PIN_DIFF1, PIN_DIFF2, PIN_DIFF3, PIN_DIFF4, PIN_DIFF5, PIN_DIFF6, pin_switchLeft, pin_switchRight, motor_shield, he_sensor1, he_sensor2);


void setup() {
  // put your setup code here, to run once:
  diff_controller.setup();

  
}


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
  
  diff_controller.update();
  Serial.print("Current state: ");
  Serial.println(diff_controller.get_state());
  Serial.print("Desired state: ");
  Serial.println(diff_controller.decode_desiredState());
  Serial.println();
  delay(10);
}
