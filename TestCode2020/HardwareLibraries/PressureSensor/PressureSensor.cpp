#ifndef __PRESSURESENSOR_CPP__
#define __PRESSURESENSOR_CPP__

/**
 * PressureSensor.cpp
 */

#include "PressureSensor.h"
#include <Arduino.h>


PressureSensor::PressureSensor(int inputPin, uint16_t scale, uint16_t offset = 0){
  _inputPin = inputPin;
  _scale = scale;
  _offset = offset;
}

void PressureSensor::begin(){
  pinMode(_inputPin, INPUT);
}


uint16_t PressureSensor::getPressurePSI(){
  return round(analogRead(_inputPin) * _scale / 1023) + _offset;
}


int PressureSensor::getRawAnalog(){
  return analogRead(_inputPin);
}


#endif
