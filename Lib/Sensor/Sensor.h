#ifndef SENSOR_H
#define SENSOR_H

#include <vector>
#include "./SensorId.h"

class BaseSensor {
protected:
    sensor_id_t _id;
    uint8_t  _pack_bytes;

public:
    /**
     * Getters and Setters
     */
    virtual sensor_id_t get_id() = 0;
    virtual void set_id(sensor_id_t id) = 0;
    virtual uint8_t get_pack_bytes() = 0;
    virtual void set_pack_bytes(uint8_t pack_bytes) = 0;

    /**
     * Member functions
     */
    virtual void pack   (byte *pack) = 0;
    virtual void unpack (const byte *pack) = 0;
};

template <class DataType>
class Sensor : public BaseSensor {
protected:
    DataType _data;

public:
    /**
     * Getters and Setters
     */
    virtual const DataType& get_data() = 0;
    virtual sensor_id_t get_id();
    virtual void set_id(sensor_id_t id);
    virtual uint8_t get_pack_bytes();
    virtual void set_pack_bytes(uint8_t pack_bytes);

    /**
     * Member functions
     */
    virtual void pack   (byte *pack);
    virtual void unpack (const byte *pack);
};

class GenericSensor : public Sensor<std::vector<byte>> {
public:
    GenericSensor (sensor_id_t id, uint8_t pack_bytes);
    void pack   (byte *pack);
    void unpack (byte *pack);
    const std::vector<byte>& get_data();
};

#endif