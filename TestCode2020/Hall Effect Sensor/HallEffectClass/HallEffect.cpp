#ifndef __HALLEFFECT_CPP__
#define __HALLEFFECT_CPP__

/**
 * HallEffect.cpp
 */

#include "HallEffect.h"
#include <Arduino.h>

/**
 * Sets up the digital out pins and the PID if specified.
 */
void HallEffect::setup(int _inputPin, uint8_t _toneWheelTeeth){
    this->inputPin = _inputPin;
    this->toneWheelTeeth = _toneWheelTeeth;
    this->prevTime = micros();
    this->currSpeed = 0;
    this->count = 0;
    this->sum = 0;
    freq.begin(_inputPin);
}

/**
 * Speed is in RPM
 */
int HallEffect::getSpeed(){
    //If any readings calculate a new curr_speed
    long currTime = micros();
    if (this->count > 0) {
    this->currSpeed = freq.countToFrequency(this->sum / this->count) / this->toneWheelTeeth * 60;
    this->prevTime = micros();
    }    

    //If no readings set curr_speed to max possible speed without taking measurements assuming less than previous speed
    else if(60000000 / (this->toneWheelTeeth * (currTime - this->prevTime)) < this->currSpeed) {
    this->currSpeed = 60000000 / (this->toneWheelTeeth * i * wait_time);
    }

    this->sum = 0;
    this->count = 0;    
    return this->currSpeed;
}

#endif