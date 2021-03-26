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
    _pack_bytes = sizeof(uint8_t);
}

void LDS::begin(){
    pinMode(_inputPin, INPUT);
}

uint8_t LDS::read_position(){
    if(_isReversed){
        return round(analogRead(_inputPin) * _travelMM / 1023) * -1 + _travelMM;
    }
    return round(analogRead(_inputPin) * _travelMM / 1023);
}

uint8_t LDS::getPositionMM() {
    get_data();
    return _data;
}

float LDS::getPositionInches(){
    return (getPositionMM() * 2.54);
}


int LDS::getRawAnalog(){
    return analogRead(_inputPin);
}

const uint8_t& LDS::get_data() {
    if(_type == ACTIVE){
        _data = read_position();
    }
    return _data;
}

void pack(byte* pack){
    *pack = get_data();
}

void unpack(const byte* pack){
    _data = *pack;
}

#endif
