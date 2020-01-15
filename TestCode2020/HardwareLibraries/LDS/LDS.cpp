#ifndef __LDS_CPP__
#define __LDS_CPP__

/**
 * LDS.cpp
 */

#include "LDS.h"
#include <Arduino.h>


LDS::LDS(uint8_t inputPin, uint8_t travelMM, bool isReversed = false){
  _inputPin = inputPin;
  _travelMM = travelMM;
  _travelIn = travelMM * 2.54;
  _isReversed = isReversed;
}

void LDS::begin(){
  pinMode(_inputPin, INPUT);
}


int LDS::getPositionMM(){
  if(_isReversed){
    return round(analogRead(_inputPin) * _travelMM / 1023) * -1 + _travelMM;
  }
  return round(analogRead(_inputPin) * _travelMM / 1023);
}


float LDS::getPositionInches(){
    if(_isReversed){
    return round(analogRead(_inputPin) * _travelIn * 1000 / 1023) / 1000 * (-1) + _travelIn;
  }
  return round(analogRead(_inputPin) * _travelIn * 1000 / 1023) / 1000;
}


int LDS::getRawAnalog(){
  return analogRead(_inputPin);
}


#endif
