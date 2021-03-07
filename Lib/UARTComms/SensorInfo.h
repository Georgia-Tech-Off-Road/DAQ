#ifndef SENSORID_H
#define SENSORID_H

#include <Arduino.h>

struct SensorInfo {
    const uint8_t id;
    const uint8_t num_bytes;

    SensorInfo(const uint8_t in_id, const uint8_t in_num_bytes) : id(in_id), num_bytes(in_num_bytes) {};
};

const SensorInfo NOTHING_SENSOR (0,0);
const SensorInfo ENGINE_SPEED_RPM  ( 1,  2);

#endif