#ifndef RTCSENSOR_H
#define RTCSENSOR_H

// RTCSensor derived class based off of RTClib. Add from Arduinol built in library manager
#include <RTClib.h>

// RTC connects to I2C. Make sure it's set before usage which assumes that it is set

class RTCSensor : public Sensor<DateTime> {
private:
    RTC_DS1307 _rtc;
public:
    RTCSensor(){
        _pack_bytes = 4; // Sending unixtime as 32 bit int
    }
    void begin(){
        _rtc.begin();
    }
    const DateTime& get_data() { 
        if(_type == ACTIVE){
            _data = _rtc.now(); 
        }
        return _data; 
    }
    void pack(byte* pack){ *((uint32_t*)pack) = get_data().unixtime(); }
    void unpack(const byte* pack){ _data = DateTime(*((uint32_t*)pack)); }
};

#endif