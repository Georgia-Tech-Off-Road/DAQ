#ifndef DIFFERENTIAL_CONTROLLER_H
#define DIFFERENTIAL_CONTROLLER_H

#include <Arduino.h>
#include <Single_MAX14870_MotorShield.h>
#include <HallEffectSpeedSensor.h>

#define STATE_4WD  0
#define STATE_LS   1
#define STATE_LOCK 2

#define SWITCHPOS1 1
#define SWITCHPOS2 2
#define SWITCHPOS3 3

#define STARTPOS 1

#define PLACEHOLDER1 1
#define PLACEHOLDER2 2
#define PLACEHOLDER3 3

class Differential_Controller {
  public:
    Differential_Controller(uint8_t pin_diff1, uint8_t pin_diff2, uint8_t pin_diff3, uint8_t pin_diff4, uint8_t pin_diff5, uint8_t pin_diff6, uint8_t pin_switch1, uint8_t pin_switch2, uint8_t pin_switch3, Single_MAX14870_MotorShield& motor_shield, HallEffectSpeedSensor& he_sensor1, HallEffectSpeedSensor& he_sensor2);
    void setup();
    void update();
    void changeMode(uint8_t desired_state);
    uint8_t get_state();
    uint8_t get_switchPos();

  private:
    Single_MAX14870_MotorShield _motor_driver;
    HallEffectSpeedSensor _he_sensor1;
    HallEffectSpeedSensor _he_sensor1;

    const uint8_t _pin_diff1; 
    const uint8_t _pin_diff2; 
    const uint8_t _pin_diff3; 
    const uint8_t _pin_diff4; 
    const uint8_t _pin_diff5; 
    const uint8_t _pin_diff6; 
    
    const uint8_t _pin_switch1;
    const uint8_t _pin_switch2;
    const uint8_t _pin_switch3;

    uint8_t _desired_state;
    uint8_t _diff_wiper_combo;
        
    uint8_t _switch_position;



};

#endif
