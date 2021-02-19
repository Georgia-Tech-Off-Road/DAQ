#ifndef DIFFERENTIAL_CONTROLLER_H
#define DIFFERENTIAL_CONTROLLER_H

#include <Arduino.h>
#include <Single_MAX14870_MotorShield.h>
#include <HallEffectSpeedSensor.h>

#define STATE_4WD  0
#define STATE_LS   1
#define STATE_LOCK 2

#define SWITCHPOSLEFT 1
#define SWITCHPOSMIDDLE 0
#define SWITCHPOSRIGHT 2

#define STARTPOS 3

#define STATE1 1
#define STATE2 2
#define STATE3 3
#define STATE4 4
#define STATE5 5
#define STATE6 6
#define STATE7 7
#define STATE8 8


#define PH_1 1
#define PH_2 2
#define PH_3 3

class Differential_Controller {
  public:
    Differential_Controller(uint8_t pin_diff1, uint8_t pin_diff2, uint8_t pin_diff3, uint8_t pin_diff4, uint8_t pin_diff5, uint8_t pin_diff6, uint8_t pin_switchLeft, uint8_t pin_switchRight, Single_MAX14870_MotorShield& motor_shield, HallEffectSpeedSensor& he_sensor1, HallEffectSpeedSensor& he_sensor2);
    void setup();
    void update();
    void change_state(uint8_t desired_state);
    uint8_t decode_desiredState();
    uint8_t decode_desiredDiffMode();
    uint8_t get_state();
    uint8_t get_switchPos();
    uint8_t get_desiredState();
    void rotate_stop();
    void rotate_F();
    void rotate_R();

  private:
    Single_MAX14870_MotorShield _motor_driver;
    HallEffectSpeedSensor _he_sensor1;
    HallEffectSpeedSensor _he_sensor2;

    const uint8_t _pin_diff1; 
    const uint8_t _pin_diff2; 
    const uint8_t _pin_diff3; 
    const uint8_t _pin_diff4; 
    const uint8_t _pin_diff5; 
    const uint8_t _pin_diff6; 
    
    const uint8_t _pin_switchLeft;    // pin assigned for left-side active-low switch on dashboard
    const uint8_t _pin_switchRight;   // pin assigned for right-side active-low switch on dashboard

    uint8_t _currState;
    uint8_t _desiredState;
    uint8_t _desiredDiffMode;
    uint8_t _diffWiperCombo;
        



};

#endif
