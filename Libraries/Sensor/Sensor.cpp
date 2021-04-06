#ifndef SENSOR_CPP
#define SENSOR_CPP

#include "Sensor.h"

/**
 * Sensor Implementation
 */

template <class DataType>
sensor_id_t Sensor<DataType>::get_id() {
    return _id;
}

template <class DataType>
void Sensor<DataType>::set_id(sensor_id_t id) {
    _id = id;
}

// Defaulted to no implementation. Child classes need to define this to work with UARTComms.
// TODO: add defaults for common types
template <class DataType>
void Sensor<DataType>::pack(byte *pack) { }

template <class DataType>
void Sensor<DataType>::unpack(byte *pack) { }


/**
 * GenericSensor Implementation
 */

GenericSensor::GenericSensor(sensor_id_t, uint8_t pack_bytes){
    _id(id),
    _pack_bytes(pack_bytes) {
    _data.resize(num_bytes);
}

void GenericSensor::pack(byte *pack){
    memcpy(pack, _data.data(), _pack_bytes);
}

void GenericSensor::unpack(byte *pack){
    memcpy(_data.data(), pack, _pack_bytes);
}

const std::vector<byte>& GenericSensor::get_data(){
    return _data;
};

#endif