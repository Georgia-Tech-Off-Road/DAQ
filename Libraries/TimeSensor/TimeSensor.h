#ifndef TIMESENSOR_H
#define TIMESENSOR_H

#include <Sensor.h>

enum time_type_t {
    MILLIS, MICROS
};

class TimeSensor : public Sensor<uint32_t> {
private:
    time_type_t _time_type;
public:
    TimeSensor(time_type_t time_type) : 
        _time_type(time_type) {
        _pack_bytes = 4;
    }

    const uint32_t& get_data() {
        if(_type == ACTIVE){
            if(_time_type == MICROS) _data = micros();
            else if (_time_type == MILLIS) _data = millis();
        }
        return _data;
    }

    void pack (byte* pack){
        *((uint32_t*)pack) = get_data();
    }
    void unpack (const byte* pack){
        _data = *((uint32_t*)pack);
    }
};

#endif