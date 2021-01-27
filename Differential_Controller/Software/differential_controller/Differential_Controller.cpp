#ifndef DIFFERENTIAL_CONTROLLER_CPP
#define DIFFERENTIAL_CONTROLLER_CPP

#include "Differential_Controller.h"


Differential_Controller::Differential_Controller(uint8_t pin_diff1, uint8_t pin_diff2, uint8_t pin_diff3, uint8_t pin_diff4, uint8_t pin_diff5, uint8_t pin_diff6, Single_MAX14870_MotorShield& motor_shield, uint8_t pin_switch1, uint8_t pin_switch2, uint8_t pin_switch3, HallEffectSpeedSensor& HE1, HallEffectSpeedSensor& HE2) : 
  _pin_diff1(pin_diff1),
  _pin_diff2(pin_diff2),
  _pin_diff3(pin_diff3),
  _pin_diff4(pin_diff4),
  _pin_diff5(pin_diff5),
  _pin_diff6(pin_diff6),
  _motor_driver(motor_shield),
  _pin_switch1(pin_switch1),
  _pin_switch2(pin_switch2),
  _pin_switch3(pin_switch3),
  _HE1(HE1),
  _HE2(HE2),
  _desired_state(STARTPOS)
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

    pinMode(_pin_switch1, INPUT);
    pinMode(_pin_switch2, INPUT);
    pinMode(_pin_switch3, INPUT);

    pinMode(_pin_HE1, INPUT);
    pinMode(_pin_HE2, INPUT);
}

void Differential_Controller::update()
{
  uint8_t curr_state = get_state();
  //TODO: ADD CHECK TO MAKE SURE CAR IS NOT MOVING USING HE SPEED SENSORS BEFORE SWITCHING DIFFERENTIAL MODES
  //TODO: ADD CHECK TO MAKE SURE CAR IS NOT MOVING USING HE SPEED SENSORS BEFORE SWITCHING DIFFERENTIAL MODES
  //TODO: ADD CHECK TO MAKE SURE CAR IS NOT MOVING USING HE SPEED SENSORS BEFORE SWITCHING DIFFERENTIAL MODES
  //TODO: ADD CHECK TO MAKE SURE CAR IS NOT MOVING USING HE SPEED SENSORS BEFORE SWITCHING DIFFERENTIAL MODES
  if (_desired_state != curr_state)
  {
    if (_desired_state == [PLACEHOLDERPOS2])          // logic to change the state to pos 2
    {
      if (curr_state == [PLACEHOLDERPOS3])
      {
        // go forward
        _motor_driver.set_speed([PLACEHOLDER_GO_UP]);
      } 
      else if (curr_state) == [PLACEHOLDERPOS1])
      {
        _motor_driver.set_speed([PLACEHOLDER_GO_DOWN]);
      }
    }
    else if (_desired_state == [PLACEHOLDERPOS1])     // logic to change the state to pos 1
    {
      _motor_driver.set_speed([PLACEHOLDER_GO_UP]);
    }
    else if (_desired_state == [PLACEHOLDERPOS3])     // logic to change the state to pos 3
    {
      _motor_driver.set_speed([PLACEHOLDER_GO_DOWN]);
    }
  }
  else
  {
      _motor_driver.set_speed(0)
  }
  
}

void Differential_Controller::changeMode(uint8_t desired_state);
{
  _desired_state = desired_state;
}


uint8_t Differential_Controller::get_state()
{
  digitalWrite(_pin_diff5, HIGH);
  
  _diff_wiper_combo = (digitalRead(_pin_diff6) << 5) | (16) | (digitalRead(_pin_diff4) << 3) | (digitalRead(_pin_diff3) << 2) | (digitalRead(_pin_diff2) << 1) | (digitalRead(_pin_diff1)); // 16 (00010000) for _diff5
  switch (_diff_wiper_combo) 
  {
    case 0x30:    // state 1 (00110000)
      return [PLACEHOLDERPOS1];
      break;
    case 0x31:    // state 2 (00110001)
      return [PLACEHOLDERPOS1];
      break;
    case 0x39:    // state 3 (00111001)
      return [PLACEHOLDERPOS1];
      break;
    case 0x19:    // state 4 (00011001)
      return [PLACEHOLDERPOS2];
      break;
    case 0x1B:    // state 5 (00011011)
      return [PLACEHOLDERPOS2];
      break;
    case 0x13:    // state 6 (00010011)
      return [PLACEHOLDERPOS3];
      break;
    case 0x17:    // state 7 (00010111)
      return [PLACEHOLDERPOS3];
      break;
    case 0x16:    // state 8 (00010110)
      return [PLACEHOLDERPOS3];
      break;
    default:      // all other unused combinations 
      return 0;
  }
  
}


uint8_t Differential_Controller::get_switchPos();
{
  if (digitalRead(_pin_switch2))
    return SWITCHPOS2;
  if (digitalRead(_pin_switch1))
    return SWITCHPOS1;
  if (digitalRead(_pin_switch3))
    return SWITCHPOS3;
}

#endif
