#ifndef __LDS_H__
#define __LDS_H__

/**
 * LDS.h
 * A wrapper to work with any analog LDS.
 */
#include <Sensor.h>

class LDS : public Sensor<uint8_t> {
private:
    uint8_t _inputPin;
    uint8_t _travelMM;
    float _travelIn;
    bool _isReversed;

    uint8_t read_position(){
        if(_isReversed){
            return round(analogRead(_inputPin) * _travelMM / 1023) * -1 + _travelMM;
        }
        return round(analogRead(_inputPin) * _travelMM / 1023);
    }

public:
    /**
     * Constructor for the LDS sensor
     */
    LDS(uint8_t inputPin, uint8_t travelMM, bool isReversed = false) {
        _inputPin = inputPin;
        _travelMM = travelMM;
        _travelIn = travelMM * 2.54;
        _isReversed = isReversed;
        _pack_bytes = 1;
    }

    LDS() { 
        _pack_bytes = 1;
    }

    /**
     * Configures hardware; required in setup
     */
    void begin(){
        pinMode(_inputPin, INPUT);
    }

    /**
     * Returns position to nearest milimeter
     */
    uint8_t getPositionMM(){
        get_data();
        return _data;
    }

    /**
     * Returns the raw analog reading of the LDS sensor
     */
    uint8_t getRawAnalog(){
        if(_type == ACTIVE) return analogRead(_inputPin);
        else return 255;
    }

    const uint8_t& get_data(){
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
};

#endif
