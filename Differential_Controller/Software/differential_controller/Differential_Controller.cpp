#ifndef DIFFERENTIAL_CONTROLLER_CPP
#define DIFFERENTIAL_CONTROLLER_CPP

#include "Differential_Controller.h"

#define PLACEHOLDERGOUP 4
#define PLACEHOLDERGODOWN 5


Differential_Controller::Differential_Controller(uint8_t pin_diff1, uint8_t pin_diff2, uint8_t pin_diff3, uint8_t pin_diff4, uint8_t pin_diff5, uint8_t pin_diff6,
    uint8_t pin_motorPos, uint8_t pin_motorNeg, uint8_t pin_switchLeft, uint8_t pin_switchRight,
    HallEffectSpeedSensor& he_sensor1, HallEffectSpeedSensor& he_sensor2) :
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
  _he_sensor1(he_sensor1),
  _he_sensor2(he_sensor2),
  _desiredState(STARTPOS),
  _prevSwitchPos(-1)
{

}


void Differential_Controller::setup()
{
  pinMode(_pin_diff1, INPUT_PULLUP);
  pinMode(_pin_diff2, INPUT_PULLUP);
  pinMode(_pin_diff3, INPUT_PULLUP);
  pinMode(_pin_diff4, INPUT_PULLUP);
  pinMode(_pin_diff5, OUTPUT);
  pinMode(_pin_diff6, INPUT_PULLUP);

  pinMode(_pin_motorPos, OUTPUT);
  pinMode(_pin_motorNeg, OUTPUT);

  pinMode(_pin_switchLeft, INPUT_PULLUP);
  pinMode(_pin_switchRight, INPUT_PULLUP);

}


/*
   Controls the rotation of the motor inside the differential controller based on physical state of hardware
*/
void Differential_Controller::update()
{
  //  if ((he_sensor1.get_speed() < 0) && (he_sensor2.get_speed() < 0 ))
  {

    uint8_t currState = get_currState();
    uint8_t desiredState;

    int mode;

    if (Serial.available() > 0) {
      if (Serial.read() == "switch") {
        mode = 0;
      } else if (Serial.read() == "serial") {
        mode = 1;
      }
    }
    
    if (!_changingDiffType) {
      switch (mode) {
        case 1:
            if (Serial.available() == 1) {
              uint8_t inputState = Serial.read();
              if (inputState >= 1 && inputState <= 8) {
                desiredState = inputState;
                _changingDiffType = true;
                _switchInputMode = false;
              }
            }
        case 0:
           desiredState = get_desiredState();
            _changingDiffType = true;
            _switchInputMode = true;
        default:
            
        }
      }

    // if an invalid state has been returned, stop the differential controller
    if (currState != 0)
    {

      // if not at the desired state, rotate the motor in the direction of the desired state
      // else stop rotating the motor and change the boolean to show the differential controller has reached the desired state
      if (desiredState != currState)
      {
        if (currState < desiredState)
        {
          rotate_R();
        }
        else if (currState > desiredState)
        {
          rotate_F();
        }
        else
        {
          rotate_stop();
          _changingDiffType = false;
        }
      }
      else
      {
        rotate_stop();
         _changingDiffType = false;
      }
    } else {
      rotate_stop();
    }
  }

}



/*
   Gets the current state of the physical differential controller wipers and tracks

   @return - value of the macro for the current state
*/
uint8_t Differential_Controller::get_currState()
{
  // differential pin 5 pulls all differential pins (pulled high by defualt)
  // that are in contact through the wipers/tracks low
  digitalWrite(_pin_diff5, LOW);

  // differential pins are active low
  uint8_t diffWiperCombo = (!digitalRead(_pin_diff6) << 5) | (1 << 4) | (!digitalRead(_pin_diff4) << 3) |
                           (!digitalRead(_pin_diff3) << 2) | (!digitalRead(_pin_diff2) << 1) | (!digitalRead(_pin_diff1)); // 16 (010000) for _diff5

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
    case 0b010111:    // state 7
      _currState = STATE7;
      break;
    case 0b010110:    // state 8      // This state is the limit of going in the R direction
      _currState = STATE8;
      break;
    default:      // all other unused combinations
      return 0;
  }

  return _currState;

}



/*
    This function gets the desired state of the physical differential controller wipers and tracks
    based on the phsysical position of the switch

    @return - value of the macro for the desired state
*/
uint8_t Differential_Controller::get_desiredState()
{
  // decodes the desired differential type
  // based on the current physical position of switch
  uint8_t currSwitchPos = (!(digitalRead(_pin_switchLeft)) << 1) | !digitalRead(_pin_switchRight);
  switch (currSwitchPos)
  {
    case 100:         // switch is rotated to the left position
      _desiredDiffType = PH_1;
      break;
    case 000:         // switch is rotated to the middle position
      _desiredDiffType = PH_2;
      break;
    case 001:         // switch is rotated to the right position
      _desiredDiffType = PH_3;
      break;
    default:
      _desiredDiffType = 0;
  }

  // check if the switch position has changed since last call
  if (currSwitchPos != _prevSwitchPos)
  {
    _changingDiffType = true;
  } 
  _prevSwitchPos = currSwitchPos;     // update what the prevous switch position was

  // decodes the desired state of physical differential controller wiper and tracks
  // based on the desired differential type
  switch (_desiredDiffType)
  {
    case PH_1:
      _desiredState = STATE2;
      break;
    case PH_2:
      _desiredState = STATE5;
      break;
    case PH_3:
      _desiredState = STATE7;
      break;
    default:
      _desiredState = 0;
  }

  return _desiredState;

}


/*
    This function sets the P-channel MOSFETs to be open to prevent current from flowing into the motor
*/
void Differential_Controller::rotate_stop()
{
  digitalWrite(_pin_motorPos, HIGH);
  digitalWrite(_pin_motorNeg, HIGH);
}


/*
    This function rotates the motor in the direction of the F on the differential controller case
*/
void Differential_Controller::rotate_F()
{
  digitalWrite(_pin_motorPos, LOW);
  digitalWrite(_pin_motorNeg, HIGH);
}


/*
    This function rotates the motor in the direction of the R on the differential controller case
*/
void Differential_Controller::rotate_R()
{
  digitalWrite(_pin_motorPos, HIGH);
  digitalWrite(_pin_motorNeg, LOW);
}


#endif
