#ifndef DIFFERENTIAL_CONTROLLER_H
#define DIFFERENTIAL_CONTROLLER_H

#include <Arduino.h>
#include "HallEffectSpeedSensor.h"

#define STATE_4WD  0
#define STATE_LS   1
#define STATE_LOCK 2



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

#define CONTROLMODE_SWITCH 0
#define CONTROLMODE_SERIAL 1


class Differential_Controller {
  public:   
    Differential_Controller(uint8_t pin_diff1, uint8_t pin_diff2, uint8_t pin_diff3, uint8_t pin_diff4, uint8_t pin_diff5, uint8_t pin_diff6,
                                                 uint8_t pin_motorPos, uint8_t pin_motorNeg, uint8_t pin_switchLeft, uint8_t pin_switchRight, 
                                                 HallEffectSpeedSensor& he_sensor1, HallEffectSpeedSensor& he_sensor2);
    void setup();
    void update();
    void change_state(uint8_t desired_state);
    uint8_t get_currState();
    uint8_t get_desiredState();
    void rotate_stop();
    void rotate_F();
    void rotate_R();

  private:
    HallEffectSpeedSensor _he_sensor1;
    HallEffectSpeedSensor _he_sensor2;

    const uint8_t _pin_diff1; 
    const uint8_t _pin_diff2; 
    const uint8_t _pin_diff3; 
    const uint8_t _pin_diff4; 
    const uint8_t _pin_diff5; 
    const uint8_t _pin_diff6; 

    const uint8_t _pin_motorPos;
    const uint8_t _pin_motorNeg;
    
    const uint8_t _pin_switchLeft;    // pin assigned for left-side active-low switch on dashboard
    const uint8_t _pin_switchRight;   // pin assigned for right-side active-low switch on dashboard

    uint8_t _currState;
    uint8_t _desiredState;
    uint8_t _desiredDiffType;
    uint8_t _prevSwitchPos;   // NOT TO BE USED CARELESSLY. Does not indicate previous switch position accurately. Only used to change _changingDiffTypeType variable.
    bool _changingDiffType;
    bool _switchInputMode;

    uint8_t _controlMode = CONTROLMODE_SWITCH;
        



};

#endif
