#ifndef SENSOR_H
#define SENSOR_H

template <class DataType>
class Sensor {
private:
    bool     _is_connected;
    DataType _data;

public:
    SensorInfo *info;
    virtual void packetize(byte *packet);
    virtual  get_data() = 0;
}

Sensor(_,_,_);


class GenericSensor : public Sensor<std::vector<byte>> {

public:
    GenericSensor (uint8_t id, uint8_t num_bytes) {
        _data.resize(num_bytes)
    }
}

#endif