#ifndef BRAKEPRESSURETRANSDUCER_H
#define BRAKEPRESSURETRANSDUCER_H  

class BrakePressureTransducer : public Sensor<uint16_t> {
    

public:
    uint8_t _inputPin;

    BrakePressureTransducer(uint8_t inputPin) {
        _pack_bytes = 2;
    }

    void begin(){
        pinMode(_inputPin, INPUT);
    }

    const uint16_t& get_data() {
        if(_type == ACTIVE) {
            _data = analogRead(_inputPin);
        }
        return _data;
    }

    void pack (byte* pack) {
        *((uint16_t*)pack) = get_data();
    }

    void unpack (const byte* pack) {
        _data = *((uint16_t*)pack);
    }

    int getBrakePressure() {
        return get_data();
    }
};

#endif