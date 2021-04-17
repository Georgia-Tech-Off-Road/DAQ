#ifndef SCALESENSOR_H
#define SCALESENSOR_H

#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library"

class ScaleSensor : public Sensor<int32_t> {
private:
    NAU7802 nau;

public:
    bool begin(TwoWire &wirePort = Wire, bool reset = true) {
        nau.begin(wirePort, reset);
    }

    const int32_t& get_data() {
        if(_type == ACTIVE){
            _data = nau.getReading();
        }
        return _data;
    }
}

#endif