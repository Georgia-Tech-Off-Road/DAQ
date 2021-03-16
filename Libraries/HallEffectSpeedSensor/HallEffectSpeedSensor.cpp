#ifndef __HALLEFFECTSPEEDSENSOR_CPP__
#define __HALLEFFECTSPEEDSENSOR_CPP__

#include "HallEffectSpeedSensor.h"
#include "ElapsedCycles.h"
#include <Arduino.h>


// /**
//  * HallEffectSpeedSensor.cpp
//  */


HallEffectSpeedSensor::HallEffectSpeedSensor(uint8_t pin_input, uint32_t ticks_per_revolution) :
	_pin_input(pin_input),
	_ticks_per_revolution(ticks_per_revolution),
	_current_tick(0)
{

} 

/**
 *
 */
void HallEffectSpeedSensor::setup()
{
	pinMode(_pin_input, INPUT_PULLUP);
	// attachInterrupt(digitalPinToInterrupt(_pin_input), interrupt, FALLING)
	
}

void HallEffectSpeedSensor::interrupt()
{
	_latest_cycle_count = _cycle_counter;
	_cycle_counter = 0;
	++_current_tick;

	// // prevent overflow of _current_tick by checking if near max value of data type
	// if (_current_tick>3000000000) 
	// {
	// 	_current_tick = _current_tick%_ticks_per_revolution;
	// }
}

int HallEffectSpeedSensor::get_rpm()
{

	// if (_cycle_counter >= ) 		// 1 second 
	double dt = (double) _latest_cycle_count/_mcu_clock_speed;
	double rpm = 1/(dt*_ticks_per_revolution)*60;
	
	if ((rpm == _prev_rpm)&&(_cycle_counter>_mcu_clock_speed*.1))
	{
		_prev_rpm = rpm;
		return 0;
	} 
	else 
	{
		_prev_rpm = rpm;
		return (int) rpm;
	}
}

uint32_t HallEffectSpeedSensor::get_pos()
{
	if (_current_tick >= _ticks_per_revolution)
	{
		_current_tick = _current_tick%_ticks_per_revolution;
	}
	return _current_tick;
}

uint8_t HallEffectSpeedSensor::get_input_pin()
{
	return _pin_input;
}


// /**
//  * Sets up the digital out pins and the PID if specified.
//  */
// HallEffectSpeedSensor::HallEffectSpeedSensor(int inputPin, uint8_t toneWheelTeeth, uint16_t intervalLength = 50,uint16_t averagingAmount = 200){
//   _inputPin = inputPin;
//   _toneWheelTeeth = toneWheelTeeth; //Multiply by 2 when using FREQMEASUREMULTI_ALTERNATE mode
//   _averagingAmount = averagingAmount;
//   _intervalTime = micros();
//   _intervalLength = intervalLength;
//   _currSpeed = 0;
//   _readings = new uint16_t[averagingAmount];
//   _count = 0;
//   _sum = 0;
//   _index = 0;
//   _timeCounter = 1;
// }

// void HallEffectSpeedSensor::begin(bool useAlternate = false){ //Set to 1 to use alternate mode
//   //Only use alternate mode when tone wheel teeth width and air gap are same size.
//   if(useAlternate){
//     _freq.begin(_inputPin, FREQMEASUREMULTI_ALTERNATE);
//     _toneWheelTeeth *= 2;
//   } else {
//     _freq.begin(_inputPin);
//   }
//   for (int i = 0; i < _averagingAmount; i++) {
//     _readings[i] = 0;
//   }
// }

// void HallEffectSpeedSensor::updateSensor(){
//   if (_freq.available()){
//     _sum += _freq.read();
//     _count++;
//   }

//   if (abs(micros() - _intervalTime) > _intervalLength){
//     _intervalTime = micros();
//     //If any readings calculate a new curr_speed
//     if (_count > 0) {
//     _currSpeed = _freq.countToFrequency(_sum / _count) * 60 / _toneWheelTeeth; // (1 tooth / s) * (60s / 1 min) * (1 rev / x teeth) = rpm
//     _timeCounter = 1;
//     } 
//     //If no readings set curr_speed to max possible speed without taking measurements assuming less than previous speed
//     else if(60000000 / (_toneWheelTeeth * (_intervalLength * _timeCounter)) < _currSpeed) {
//         _currSpeed = 60000000 / (_toneWheelTeeth * (_intervalLength * _timeCounter));
//         _timeCounter++;
//     }
//     else{_timeCounter++;}

//     _sum = 0;
//     _count = 0;
//     _readings[_index] = _currSpeed;

//     //Increment or reset index
//     if(_index == _averagingAmount - 1){
//       _index = 0;
//     }
//     else{
//      _index++;
//     }
//   }
// }

// int HallEffectSpeedSensor::getSpeed(){
//   int sum = 0;
//   for (int i = 0; i < _averagingAmount; i++) {
//     sum += _readings[i];
//   }
//   return sum / _averagingAmount;
// }






#endif
