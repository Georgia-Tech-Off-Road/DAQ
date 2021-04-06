#ifndef __HALLEFFECTSPEEDSENSOR_CPP__
#define __HALLEFFECTSPEEDSENSOR_CPP__

#include <HallEffectSpeedSensor.h>



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
 *	@param useAlternate: Set to 1 to use alternate mode
 *		Only use alternate mode when tone wheel teeth width and air gap are same size.		
 */
void HallEffectSpeedSensor::setup(bool useAlternate = false)
{
	// if (useAlternate) {
	// 	_freq.begin(_pin_input, FREQMEASUREMULTI_ALTERNATE);
	// 	_ticks_per_revolution *= 2;
	// } else {
	// 	_freq.begin(_pin_input);
	// }

	
	_freq.begin(_pin_input);
	
	_recent_pulse_freqs.assign(INTERVALSIZE, 0);
}

// void HallEffectSpeedSensor::interrupt()
// {
// 	_latest_cycle_count = _cycle_counter;
// 	_cycle_counter = 0;
// 	++_current_tick;

// 	// // prevent overflow of _current_tick by checking if near max value of data type
// 	// if (_current_tick>3000000000) 
// 	// {
// 	// 	_current_tick = _current_tick%_ticks_per_revolution;
// 	// }
// }

void HallEffectSpeedSensor::update()
{
	// uint32_t elapsed_ticks = 0;
	
	// // clear the buffer except for the most recent reading
	// while (_freq.available() > 1) {
	// 	_freq.read();
	// 	elapsed_ticks++;
		
	// 	Serial.println(_latest_cycle_count);
	// }

	// if (_freq.available()>0) {
	// 	_latest_cycle_count = _freq.read();
	// 	Serial.println(_latest_cycle_count);
	// 	elapsed_ticks++;
	// }

	// _current_tick += elapsed_ticks;

	// // prevent overflow of _current_tick by checking if near max value of data type
	// if (_current_tick>3000000000) 
	// {
	// 	_current_tick = _current_tick%_ticks_per_revolution;
	// }
}

uint32_t HallEffectSpeedSensor::get_tick_frequency()
{
	update();
	return _freq.countToFrequency(_latest_cycle_count);
}

uint32_t HallEffectSpeedSensor::get_rpm()
{
	uint32_t elapsed_ticks = 0; 
	while(_freq.available()) {
		_recent_pulse_freqs.push_back(_freq.read());
		elapsed_ticks++;
		_recent_pulse_freqs.erase(_recent_pulse_freqs.begin());
	}
	Serial.println(_recent_pulse_freqs.size());

	uint32_t sum = 0;
	for (uint8_t i = 0; i < INTERVALSIZE; i++) {
		sum += _recent_pulse_freqs[i];
	}
	Serial.print("Sum: ");
	Serial.println(sum);
	double avg_elapsed_cycles = ((double) sum)/INTERVALSIZE; 
	
	Serial.print("avg elapsed cycles: ");
	Serial.println(avg_elapsed_cycles);
	
	Serial.print("Paul's Freq: ");
	Serial.println(_freq.countToFrequency(avg_elapsed_cycles));
	// uint32_t avg_elapsed_cycles = _freq.read(); 

	// Serial.println(_freq.countToFrequency(avg_elapsed_cycles));
	_current_tick += elapsed_ticks;
	// float dt = (float) _freq.countToNanoseconds(avg_elapsed_cycles)/1000000000.0;
	// Serial.println(dt);
	// uint32_t rpm = (uint32_t) 1/(dt*_ticks_per_revolution)*60;

		
	double dt = avg_elapsed_cycles/ ((double) _mcu_clock_speed);
	Serial.print("change in time: ");
	Serial.println(dt);
	double freq = 1.0/dt;

	
	Serial.print("Our freq: ");
	Serial.println(freq);
	uint32_t rpm = freq/_ticks_per_revolution*60;

	// Serial.print("Average Ticks: ");
	// Serial.println(avg_elapsed_cycles);
	// Serial.print("Average Freq: ");
	// // Serial.println(_freq.countToFrequency(sum2 / count2));
	// Serial.println(_freq.countToFrequency(avg_elapsed_cycles));
	// Serial.print("dt: ");
	// // Serial.println(_freq.countToNanoseconds(sum2 / count2));
	// Serial.println(dt, 18);
	// Serial.print("rpm: ");
	// // Serial.println(_freq.countToNanoseconds(sum2 / count2));
	// Serial.println(rpm);
	// Serial.print("counts: ");
	// Serial.println(get_pos());


	// if (count2 > 400) {
	// 	sum2 = 0;
	// 	count2 = 0;
	// }

	return rpm;




	// update();
	// // if (_cycle_counter >= ) 		// 1 second 
	// double dt = (double) _latest_cycle_count/_mcu_clock_speed;
	// uint32_t rpm = (uint32_t) 1/(dt*_ticks_per_revolution)*60;
	
	// // 
	// if (_latest_cycle_count > 0)	
	// {
	// 	_prev_rpm = rpm;
	// 	return (uint32_t) rpm;
	// } 
	// else 
	// {
	// 	_prev_rpm = rpm;
	// 	return 0;
	// }
}

uint32_t HallEffectSpeedSensor::get_pos()
{
	update();
	if (_current_tick >= _ticks_per_revolution)
	{
		// _current_tick = _current_tick%_ticks_per_revolution;
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
