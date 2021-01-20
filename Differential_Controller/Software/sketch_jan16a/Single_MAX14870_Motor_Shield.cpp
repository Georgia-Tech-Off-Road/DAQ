#include "Single_MAX14870_Motor_Shield.h"


Single_MAX14870_Motor_Shield::Single_MAX14870_Motor_Shield(uint8_t pin_PWM, uint8_t pin_DIR, uint8_t pin_nEN, uint8_t pin_nFAULT) : 
  _pin_PWM(pin_PWM),
  _pin_DIR(pin_DIR),
  _pin_nEN(pin_nEN),
  _pin_nFAULT(pin_nFAULT)
{


}

void Single_MAX14870_Motor_Shield::setup() 
{
  pinMode(pin_PWM, OUTPUT);
  pinMode(pin_DIR, OUTPUT);
  pinMode(pin_nEN, OUTPUT);
  pinMode(pin_nFAULT, INPUT_PULLUP);
}

void Single_MAX14870_Motor_Shield::set_speed(int16_t speed)
{
  bool reverse = (speed < 0); // indicates if the motor is rotating reverse
  speed = abs(speed);
  if (speed > 256)
    speed = 256;
  digitalWrite(_pin_DIR, reverse);  // set the motor direction to clockwise/forward
  analogWrite(_pin_PWM, speed);   // send a PWM signal to the motor shield
  

}
