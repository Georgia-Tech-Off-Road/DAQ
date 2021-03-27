#ifndef __HALLEFFECTSPEEDSENSOR_H__
#define __HALLEFFECTSPEEDSENSOR_H__

#include <ElapsedCycles.h>
#include <FreqMeasureMulti.h>
#include <vector>
#include <Sensor.h>

#define INTERVALSIZE 10.0

// /**
//  * HallEffectSpeedSensor.h
//  * A wrapper to work with a hall effect sensor.
//  */

class HallEffectSpeedSensor : public Sensor<uint32_t> {
public:
    HallEffectSpeedSensor(uint8_t pin_input, uint32_t ticks_per_revolution) :
        _pin_input(pin_input),
        _ticks_per_revolution(ticks_per_revolution),
        _current_tick(0)
    {
        _pack_bytes = 8;
    } 


    void setup(bool useAlternate = false){
        _freq.begin(_pin_input);
        
        _recent_pulse_freqs.assign(INTERVALSIZE, 0);
    }

    const uint32_t& get_rpm(){
        if(_type == ACTIVE){
            uint32_t elapsed_ticks = 0; 
            while(_freq.available()) {
                _recent_pulse_freqs.push_back(_freq.read());
                elapsed_ticks++;
                _recent_pulse_freqs.erase(_recent_pulse_freqs.begin());
            }
            uint32_t sum = 0;
            for (uint8_t i = 0; i < INTERVALSIZE; i++) {
                sum += _recent_pulse_freqs[i];
            }
            double avg_elapsed_cycles = ((double) sum)/INTERVALSIZE; 
            
            double dt = avg_elapsed_cycles/ ((double) _mcu_clock_speed);
            double freq = 1.0/dt;
            
            uint32_t rpm = freq/_ticks_per_revolution*60;

            _data = rpm;
        }
        return _data;
    }
    const uint32_t& get_pos() { return _current_tick; }
    uint32_t get_tick_frequency(){ return _freq.countToFrequency(_latest_cycle_count); }
    uint8_t get_input_pin(){ return _pin_input; }

    const uint32_t& get_data(){ return get_rpm(); }
    void pack (byte *pack){
        *((uint32_t*)pack) = get_rpm();
        *((uint32_t*)(pack + 3)) = get_pos();
    }
    void unpack (const byte *pack){
        _data = *((uint32_t*)pack);
        _current_tick = *((uint32_t*)(pack+3));
    }


private:
    std::vector<uint32_t> _recent_pulse_freqs;
    FreqMeasureMulti _freq;
    const static uint32_t _mcu_clock_speed = F_CPU;
    uint16_t _ticks_per_revolution;    // 
    const uint8_t _pin_input;
    ElapsedCycles _cycle_counter;       // number of elapsed processor clock cycles
    uint32_t _latest_cycle_count;       // number of elapsed clock cycles between the last two interrupts
    uint32_t _current_tick;         // tracks the current rotational position
    uint32_t _prev_rpm;
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
