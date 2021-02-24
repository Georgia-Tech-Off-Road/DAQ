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
  _motor_driver(motor_shield),
  _he_sensor1(he_sensor1),
  _he_sensor2(he_sensor2),
  _desiredState(STARTPOS)
{

}


void Differential_Controller::setup()
{
    pinMode(_pin_diff1, INPUT);
    pinMode(_pin_diff2, INPUT);
    pinMode(_pin_diff3, INPUT);
    pinMode(_pin_diff4, INPUT);
    pinMode(_pin_diff5, OUTPUT);
    pinMode(_pin_diff6, INPUT);
    
    _motor_driver.setup();

    pinMode(_pin_switchLeft, INPUT_PULLUP);
    pinMode(_pin_switchRight, INPUT_PULLUP);

}

void Differential_Controller::update()
{
//  if ((he_sensor1.get_speed() < 0) && (he_sensor2.get_speed() < 0 ))
  uint8_t currState = get_state();
  uint8_t desiredState = decode_desiredState();
  
  if (currState != 0)
  {
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
      }
    } 
    else
    {
      rotate_stop();
    }
  }


  
}


uint8_t Differential_Controller::get_state()
{
  digitalWrite(_pin_diff5, HIGH);
  
  _diffWiperCombo = (digitalRead(_pin_diff6) << 5) | (1 << 4) | (digitalRead(_pin_diff4) << 3) | 
                    (digitalRead(_pin_diff3) << 2) | (digitalRead(_pin_diff2) << 1) | (digitalRead(_pin_diff1)); // 16 (00010000) for _diff5

  // decode the current state using the currently energized tracks
  switch (_diffWiperCombo) 
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

uint8_t Differential_Controller::get_desiredState()
{
   return decode_desiredState();
}


uint8_t Differential_Controller::decode_desiredState() 
{
  switch(decode_desiredDiffMode())
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

uint8_t Differential_Controller::decode_desiredDiffMode()
{
  switch (get_switchPos())
  {
    case SWITCHPOSLEFT:
      _desiredDiffMode = PH_1;
      break;
    case SWITCHPOSMIDDLE:
      _desiredDiffMode = PH_2;
      break;
    case SWITCHPOSRIGHT:
      _desiredDiffMode = PH_3;
      break;
    default:
      _desiredDiffMode = 0;
  }

  return _desiredDiffMode;
}

/*  
 *  This function gets the position of the dashboard switch 
 *  
 *  @return - integer value of the SWITCHPOS
 */
uint8_t Differential_Controller::get_switchPos()
{
  return (!(digitalRead(_pin_switchRight))<<1) | !digitalRead(_pin_switchLeft);

}


/*  
 *  This function 
 *  
 *  @return - integer value of the SWITCHPOS
 */
void Differential_Controller::rotate_stop()
{
  _motor_driver.set_speed(0);
}

void Differential_Controller::rotate_F()
{
  digitalWrite(pin_motorPos, HIGH);
  digitalWrite(pin_motorNeg, LOW);
}

void Differential_Controller::rotate_R()
{
  digitalWrite(pin_motorPos, HIGH);
  digitalWrite(pin_motorNeg, LOW);  
}


#endif
