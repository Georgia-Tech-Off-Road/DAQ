#include <Differential_Controller.h>
#include <Single_MAX14870_MotorShield.h>
#include <HallEffectSpeedSensor.h>


#define PIN_DIFF1 13
#define PIN_DIFF2 14
#define PIN_DIFF3 15
#define PIN_DIFF4 16
#define PIN_DIFF5 17
#define PIN_DIFF6 18

#define PIN_PWM 23
#define PIN_DIR 21
#define PIN_NEN 19
#define PIN_NFAULT 20

#define PIN_HE1 
#define PIN_HE2 

#define PIN_SWITCH1 999999
#define PIN_SWITCH2 999999
#define PIN_SWITCH3 999999

Single_MAX14870_MotorShield motor_shield(PIN_PWM, PIN_DIR, PIN_NEN, PIN_NFAULT);
HallEffectSpeedSensor he_sensor1(PIN_HE1, 500);
HallEffectSpeedSensor he_sensor2(PIN_HE2, 500);
Differential_Controller diff_controller(PIN_DIFF1, PIN_DIFF2, PIN_DIFF3, PIN_DIFF4, PIN_DIFF5, PIN_DIFF6, PIN_SWITCH1, PIN_SWITCH2, PIN_SWITCH3, motor_shield, he_sensor1, he_sensor2);


void setup() {
  // put your setup code here, to run once:
  diff_controller.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  diff_controller.update();
}


// _pin_diff1 
// _pin_diff2 
// _pin_diff3 
// _pin_diff4 
// _pin_diff5 
// _pin_diff6 
// _pin_PWM 
// _pin_DIR 
// _pin_nEN 
// _pin_nFAULT 
// _pin_RX2 
// _pin_TX2
