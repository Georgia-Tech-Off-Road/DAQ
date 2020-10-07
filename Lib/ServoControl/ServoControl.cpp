#ifndef SERVOCONTROL_CPP
#define SERVOCONTROL_CPP

#include "ServoControl.h"

ServoControl::ServoControl(uint8_t _pin, uint16_t _degrees){
    pin = _pin;
    degrees = _degrees;
    min_us = 1000;
    longest_period = 1000;
}

ServoControl::ServoControl(uint8_t _pin, uint16_t _degrees, uint16_t _min_us, uint16_t _max_us){
    pin = _pin;
    degrees = _degrees;
    min_us = _min_us;
    longest_period = _max_us - _min_us;
}

void ServoControl::begin(){
    servo.attach(pin);
}

void ServoControl::writeDeg(uint16_t deg){
    uint16_t us = min_us + ((longest_period * deg)/degrees);
    servo.writeMicroseconds(us);
}

void ServoControl::writeMicroseconds(uint16_t us){
    servo.writeMicroseconds(us);
}

#endif