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

template <class DataType>
uint8_t Sensor<DataType>::get_pack_bytes() {
    return _pack_bytes;
}

template <class DataType>
void Sensor<DataType>::set_pack_bytes(uint8_t pack_bytes) {
    _pack_bytes = pack_bytes;
}

// Defaulted to no implementation. Child classes need to define this to work with UARTComms.
// TODO: add defaults for common types
template <class DataType>
void Sensor<DataType>::pack(byte *pack) { }

template <class DataType>
void Sensor<DataType>::unpack(const byte *pack) { }


/**
 * GenericSensor Implementation
 */

GenericSensor::GenericSensor(sensor_id_t id, uint8_t pack_bytes) {
    _id = id;
    _pack_bytes = pack_bytes;
    _data.resize(pack_bytes);
}

void GenericSensor::pack(byte *pack){
    memcpy(pack, _data.data(), _pack_bytes);
}

void GenericSensor::unpack(byte *pack){
    memcpy(_data.data(), pack, _pack_bytes);
}

const std::vector<byte>& GenericSensor::get_data(){
    return _data;
}

#endif