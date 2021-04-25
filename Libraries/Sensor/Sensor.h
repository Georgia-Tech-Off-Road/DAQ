#ifndef SENSOR_H
#define SENSOR_H

#include <inttypes.h>


class ExampleClass {
private:
    uint8_t mem1;
    uint16_t mem2;

public:
    virtual uint8_t get_mem1() = 0;
}


class Extended : public ExampleClass {
public:
    uint8_t get_mem1() {
        return mem1;
    }
    void bruh() {

    }
};

int t = 0;
0x0014 -> [________7]
int y = 5;
0x0018 -> [________5]

int* p = &t;
0x001c -> [_______14]

*p = 7;

Extended ext;
0x00d4 -->
Extended* point = &ext;
0x00f0 --> [0x00d4]
ExampleClass* ex = &ext;

#include <vector>
#include "./SensorId.h"

enum sensor_type_t : uint8_t {
    PASSIVE, ACTIVE
};

class BaseSensor {
protected:
    sensor_id_t   _id;
    sensor_type_t _type;
    uint8_t  _pack_bytes;

public:
    /**
     * Getters and Setters
     */
    virtual sensor_id_t get_id() = 0;
    virtual void set_id(sensor_id_t id) = 0;
    virtual sensor_type_t get_type() = 0;
    virtual void set_type(sensor_type_t type) = 0;
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
    Sensor(){
        _id = DEFAULT_NO_SENSOR;
        _type = ACTIVE;
    }

    /**
     * Getters and Setters
     */
    virtual const DataType& get_data() = 0;
    virtual sensor_id_t get_id(){
        return _id;
    }
    virtual void set_id(sensor_id_t id){
        _id = id;
    }
    virtual sensor_type_t get_type(){
        return _type;
    }
    virtual void set_type(sensor_type_t type){
        _type = type;
    }
    virtual uint8_t get_pack_bytes(){
        return _pack_bytes;
    }
    virtual void set_pack_bytes(uint8_t pack_bytes){
        _pack_bytes = pack_bytes;
    }

    /**
     * Member functions
     */
    virtual void pack   (byte *pack) { }
    virtual void unpack (const byte *pack) { }
};

class GenericSensor : public Sensor<std::vector<byte>> {
public:
    GenericSensor (sensor_id_t id, uint8_t pack_bytes){
        _id = id;
        _pack_bytes = pack_bytes;
        _data.resize(pack_bytes);
    }
    void pack   (byte *pack){
        memcpy(pack, _data.data(), _pack_bytes);
    }
    void unpack (const byte *pack){
        memcpy(_data.data(), pack, _pack_bytes);
    }
    const std::vector<byte>& get_data(){
        return _data;
    }
};

#include "../LDS/LDS.h"
#include "../TimeSensor/TimeSensor.h"
#include "../HallEffectSpeedSensor/HallEffectSpeedSensor.h"
#include "../BrakePressureTransducer/BrakePressureTransducer.h"
#include "./DerivedSensors/RTCSensor.h"
#include "./DerivedSensors/IMUSensor.h"

#endif