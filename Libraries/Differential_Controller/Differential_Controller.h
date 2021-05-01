#ifndef DIFFERENTIAL_CONTROLLER_H
#define DIFFERENTIAL_CONTROLLER_H

#include <Arduino.h>
#include <Sensor.h>

#define STARTPOS 3

// These states each represent a specific combination of which tracks are in contact with track 5
#define STATE1 1
#define STATE2 2
#define STATE3 3
#define STATE4 4
#define STATE5 5
#define STATE6 6
#define STATE7 7
#define STATE8 8

// The differential modes and their corresponding state 
#define STATE_RWD STATE1
#define STATE_LIMITEDSLIP STATE5
#define STATE_LOCKED STATE7
#define STATE_NAN 0

// Decides which input (physical switch or serial bus) controls the differential
#define CONTROLMODE_SWITCH 0
#define CONTROLMODE_SERIAL 1


class Differential_Controller {
    public:   
        Differential_Controller(uint8_t pin_diff1, uint8_t pin_diff2, uint8_t pin_diff3, uint8_t pin_diff4, uint8_t pin_diff5, uint8_t pin_diff6, uint8_t pin_motorPos, uint8_t pin_motorNeg, uint8_t pin_switchLeft, uint8_t pin_switchRight);
        void begin();
        void update();
        void rotateToState(uint8_t desired_state);
        uint8_t get_currState();
        uint8_t get_desiredState();
        void rotate_stop();
        void rotate_F();
        void rotate_R();
        BaseSensor& get_diff_state_sensor();


    private:
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
        uint8_t _prevSwitchPos;   // NOT TO BE USED CARELESSLY. Does not indicate previous switch position accurately. Only used to change _changingDiffTypeType variable.
        bool _changingDiffType;

        uint8_t _controlMode = CONTROLMODE_SWITCH;

        StateSensor _diff_state;
            
};

#endif
