#ifndef DIFFERENTIAL_CONTROLLER_CPP
#define DIFFERENTIAL_CONTROLLER_CPP

#include "Differential_Controller.h"


/*
*   Constructor for Differential Controller
*   @param
*/
Differential_Controller::Differential_Controller(uint8_t pin_diff1, uint8_t pin_diff2, uint8_t pin_diff3, uint8_t pin_diff4, uint8_t pin_diff5, uint8_t pin_diff6,
                                            uint8_t pin_motorPos, uint8_t pin_motorNeg, uint8_t pin_switchLeft, uint8_t pin_switchRight) :
    _pin_diff1(pin_diff1),
    _pin_diff2(pin_diff2),
    _pin_diff3(pin_diff3),
    _pin_diff4(pin_diff4),
    _pin_diff5(pin_diff5),
    _pin_diff6(pin_diff6),
    _pin_motorPos(pin_motorPos),
    _pin_motorNeg(pin_motorNeg),
    _pin_switchLeft(pin_switchLeft),
    _pin_switchRight(pin_switchRight),
    _desiredState(STARTPOS),
    _prevSwitchPos(-1)
{}

void Differential_Controller::begin() {
    pinMode(_pin_diff1, INPUT_PULLUP);
    pinMode(_pin_diff2, INPUT_PULLUP);
    pinMode(_pin_diff3, INPUT_PULLUP);
    pinMode(_pin_diff4, INPUT_PULLUP);
    pinMode(_pin_diff6, INPUT_PULLUP);

    pinMode(_pin_motorPos, OUTPUT);
    pinMode(_pin_motorNeg, OUTPUT);

    pinMode(_pin_switchLeft, INPUT_PULLUP);
    pinMode(_pin_switchRight, INPUT_PULLUP);
}

/*
*  Controls the rotation of the motor inside the differential controller based on physical state of hardware
*/
void Differential_Controller::update() {
    // // Code for switching control input from switch to serial bus
    // String user_input_str;
    // long user_input;
    
    // if (Serial.available() > 0) {
    // user_input_str = Serial.readString();
    // user_input = user_input_str.toInt();
    // }

    
    // if (user_input_str.indexOf("switch") > -1) {
    // _controlMode = CONTROLMODE_SWITCH;
    // Serial.println("Changing to switch control mode");
    // } else if (user_input_str.indexOf("serial") > -1) {
    // _controlMode = CONTROLMODE_SERIAL;
    // Serial.println("Changing to serial control mode");
    // }

    // switch (_controlMode) {
    // case CONTROLMODE_SERIAL:
    //     if ((user_input >= 1) && (user_input <= 8)) {
    //         _desiredState = user_input;
    //         _changingDiffType = true;
    //     }
    //     if (user_input_str.indexOf("reverse") > -1) {
    //         rotate_R();
    //         _changingDiffType = true;
    //         _desiredState = 0;
    //         delay(500);
    //     }
    //     if (user_input_str.indexOf("forward") > -1) {
    //         rotate_F();
    //         _changingDiffType = true;
    //         _desiredState = 0;
    //         delay(500);
    //     }
    //     if (user_input_str.indexOf("stop") > -1) {
    //         rotate_stop();
    //         _changingDiffType = true;
    //         _desiredState = 0;
    //         delay(500);
    //     }
    //     break;
    // case CONTROLMODE_SWITCH:
    //     _desiredState = get_desiredState();
    //     break;
    // default:
    //     Serial.println("Invalid control mode");
    //     break;
    // }

    _diff_state.set_state(get_currState());
    _desiredState = get_desiredState();
    rotateToState(_desiredState);
    
}

/**
 * Differential will rotate to the desired state
 * 
 */ 

void Differential_Controller::rotateToState(uint8_t desired_state) {

    uint8_t currState = get_currState();


    // DEBUG: print current and desired states
    // if (_changingDiffType) {
    // Serial.print("Current state: ");
    // Serial.println(currState);
    // }
    // if (_changingDiffType) {
    // Serial.print("Desired state: ");
    // Serial.println(_desiredState);
    // }
    
    // if an invalid state has been returned, stop the differential controller
    if (currState != 0 && (1 <= _desiredState && _desiredState <= 7)) {
        // if not at the desired state, rotate the motor in the direction of the desired state
        // else stop rotating the motor and change the boolean to show the differential controller has reached the desired state
        if ((_desiredState != currState) & _changingDiffType) {
            if (currState < _desiredState) {
                rotate_R();
            } else if (currState > _desiredState) {
                rotate_F();
            } else {
                rotate_stop();
                _changingDiffType = false;
            }
        } else {
            rotate_stop();
            _changingDiffType = false;
        }
    } else {
        rotate_stop();
    }
}

/*
*  Gets the current state of the physical differential controller wipers and tracks
* 
*  @return - value of the macro for the current state
*/
uint8_t Differential_Controller::get_currState() { 
    /**
     * differential pin 5 pulls all differential pins (pulled high by defualt)
     * that are in contact through the wipers/tracks low
     */

    // differential pins are active low
    uint8_t diffWiperCombo = (!digitalRead(_pin_diff6) << 5) | (1 << 4) | (!digitalRead(_pin_diff4) << 3) |
                            (!digitalRead(_pin_diff3) << 2) | (!digitalRead(_pin_diff2) << 1) | (!digitalRead(_pin_diff1)); // 16 (010000) for _diff5

    //  // print out which tracks are connected to pin 5
    //  Serial.print(!digitalRead(_pin_diff6));
    //  Serial.print(1);
    //  Serial.print(!digitalRead(_pin_diff4));
    //  Serial.print(!digitalRead(_pin_diff3));
    //  Serial.print(!digitalRead(_pin_diff2));
    //  Serial.print(!digitalRead(_pin_diff1));
    //  Serial.println();
    

    // decode the current state using the current tracks that are pulled low
    switch (diffWiperCombo)
    {
        case 0b110000:    // state 1 // This state is the limit of going in the F direction
            _currState = STATE1;
            break;
        case 0b110001:    // state 2
            _currState = STATE2;
            break;
        case 0b111001:    // state 3
            _currState = STATE3;
            break;
        case 0b011001:    // state 4
            _currState = STATE4;
            break;
        case 0b011011:    // state 5
            _currState = STATE5;
            break;
        case 0b010011:    // state 6
            _currState = STATE6;
            break;
        case 0b010111:    // state 7 // This state is the limit of going in the R direction
            _currState = STATE7;
            break;
        case 0b010110:    // state 8      
            _currState = STATE8;
            break;
        default:      // all invalid combinations
            return 0;   
    }

    return _currState;

}

/*
*  This function gets the desired state of the physical differential controller wipers and tracks
*  based on the phsysical position of the switch
* 
*  @return - value of the macro for the desired state
*/
uint8_t Differential_Controller::get_desiredState() {
    // decodes the desired state
    // based on the current physical position of switch
    uint8_t currSwitchPos = (!(digitalRead(_pin_switchLeft)) << 2) | !digitalRead(_pin_switchRight);
    switch (currSwitchPos)
    {
        case 0b100:         // switch is rotated to the left position
            _desiredState = STATE_RWD;
            break;
        case 0b000:         // switch is rotated to the middle position
            _desiredState = STATE_LIMITEDSLIP;
            break;
        case 0b001:         // switch is rotated to the right position
            _desiredState = STATE_LOCKED;
            break;
        default:
            _desiredState = 0;
    }

    // check if the switch position has changed since last call
    if (currSwitchPos != _prevSwitchPos)
    {
        _changingDiffType = true;
    }
    _prevSwitchPos = currSwitchPos;     // update what the prevous switch position was

    
    return _desiredState;
}

/*
*  This function sets the P-channel MOSFETs to be open to prevent current from flowing into the motor
*/
void Differential_Controller::rotate_stop() {
    digitalWrite(_pin_motorPos, HIGH);
    digitalWrite(_pin_motorNeg, HIGH);
}

/*
* This function rotates the motor in the direction of the F on the differential controller case
*/
void Differential_Controller::rotate_F() {
    digitalWrite(_pin_motorPos, HIGH);
    digitalWrite(_pin_motorNeg, LOW);
}

/*
* This function rotates the motor in the direction of the R on the differential controller case
*/
void Differential_Controller::rotate_R() {
    digitalWrite(_pin_motorPos, LOW);
    digitalWrite(_pin_motorNeg, HIGH);
}

BaseSensor& Differential_Controller::get_diff_state_sensor() {
    return _diff_state;
}

#endif
