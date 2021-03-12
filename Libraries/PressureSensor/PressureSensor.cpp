#ifndef __PRESSURESENSOR_CPP__
#define __PRESSURESENSOR_CPP__

/**
 * PressureSensor.cpp
 */

#include "PressureSensor.h"
#include <Arduino.h>


PressureSensor::PressureSensor(int inputPin, uint16_t maxPressure, uint16_t r1, uint16_t r2){
  _inputPin = inputPin;
  _maxPressure = maxPressure;
  _r1 = r1; //ohms; This is the resistor that goes from the signal to the middle of the voltage divide
  _r2 = r2; //ohms; This is the resistor that goes to ground

}

void PressureSensor::begin(){
  pinMode(_inputPin, INPUT);
}


uint16_t PressureSensor::getPressurePSI(){
  int pressure = map(analogRead(_inputPin), 1023*0.5*_r2/(3.3*(_r1 + _r2)), 1023*4.5*_r2/(3.3*(_r1 + _r2)), 0, _maxPressure);
  pressure > 0 ? return (uint16_t)pressure : return 0;
}


uint16_t PressureSensor::getRawAnalog(){
  return analogRead(_inputPin);
}

uint16_t PressureSensor::analogToPressure(uint16_t v_analog){
  int pressure = map(v_analog, 1023*0.5*_r2/(3.3*(_r1 + _r2)), 1023*4.5*_r2/(3.3*(_r1 + _r2)), 0, _maxPressure)
  pressure > 0 ? return (uint16_t)pressure : return 0;
}


#endif
