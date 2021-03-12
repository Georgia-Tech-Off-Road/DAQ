#ifndef __HALLEFFECTSPEEDSENSOR_H__
#define __HALLEFFECTSPEEDSENSOR_H__

#include "ElapsedCycles.h"

// /**
//  * HallEffectSpeedSensor.h
//  * A wrapper to work with a hall effect sensor.
//  */

class HallEffectSpeedSensor {
public:
    HallEffectSpeedSensor(uint8_t pin_input, uint32_t ticks_per_revolution);

    void setup();
    void interrupt();

    double get_rpm();
    uint32_t get_pos();
    uint8_t get_input_pin();

private:
    const static uint32_t _mcu_clock_speed = F_CPU;
    const uint16_t _ticks_per_revolution;    // 
    const uint8_t _pin_input;
    ElapsedCycles _cycle_counter;       // number of elapsed processor clock cycles
    uint32_t _latest_cycle_count;       // number of elapsed clock cycles between the last two interrupts
    uint32_t _current_tick;         // tracks the current rotational position

};





// class HallEffectSpeedSensor {
// private:
//     /**
//      * 
//      */
//     uint8_t _inputPin;
//     uint16_t _currSpeed;
//     uint16_t * _readings;
//     uint32_t _intervalTime;
//     uint8_t _toneWheelTeeth;
//     uint16_t _averagingAmount;
//     uint16_t _intervalLength; //How often _currSpeed is updated in microseconds (limited by processor speed)
//     uint16_t _index;
//     uint16_t _count;
//     uint32_t _sum;
//     uint16_t _timeCounter;
//     FreqMeasureMulti _freq;
    

// public:
//     /**
//      * Sets up the digital input pin for the sensor.
//      */
//     HallEffectSpeedSensor(int inputPin, uint8_t toneWheelTeeth, uint16_t intervalLength = 50,uint16_t averagingAmount = 200);

//     ~HallEffectSpeedSensor(){
//         delete _readings;
//     }


//     /**
//      * Configures hardware
//      */
//     void begin(bool useAlternate = false);

//     /**
//      * Speed is in RPM
//      */
//     int getCurrSpeed(){ return _currSpeed; }
//     int getSpeed();

//     void updateSensor();

// };

#endif
