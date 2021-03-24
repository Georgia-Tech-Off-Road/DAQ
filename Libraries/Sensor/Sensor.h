#ifndef SENSOR_H
#define SENSOR_H

#include <vector>
#include "./SensorId.h"

template <class DataType>
class Sensor {
protected:
    sensor_id_t _id;
    uint8_t  _pack_bytes;

    DataType _data;

public:
    /**
     * Getters and Setters
     */
    virtual const DataType& get_data() = 0;
    sensor_id_t get_id();
    void set_id(sensor_id_t id);

    /**
     * Member functions
     */
    virtual void pack   (byte *pack);
    virtual void unpack (byte *pack);
};

class GenericSensor : public Sensor<std::vector<byte>> {
public:
    GenericSensor (sensor_id_t id, uint8_t pack_bytes);
    void pack   (byte *pack);
    void unpack (byte *pack);
    const std::vector<byte>& get_data();
};

#endif