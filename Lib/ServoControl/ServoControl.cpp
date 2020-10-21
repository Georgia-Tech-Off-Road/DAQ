#ifndef SERVOCONTROL_CPP
#define SERVOCONTROL_CPP

#include "ServoControl.h"

ServoControl::ServoControl(uint8_t _pin, uint16_t _degrees){
    pin = _pin;
    degrees = _degrees;
}

void ServoControl::begin(){
    servo.attach(pin);
}

void ServoControl::writeDeg(uint16_t deg){
    servo.write( (degrees - deg) * 180 / 270 );
}

#endif