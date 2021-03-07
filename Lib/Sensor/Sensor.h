#ifndef SENSOR_H
#define SENSOR_H

template <class DataType>
class Sensor {
private:
    uint16_t _id;
    bool     _is_connected;

    DataType _data;

public:
    /**
     * Constructors
     */
    Sensor();

    /**
     * Getters and Setters
     */
    virtual DataType get_data() = 0;
    uint16_t get_id();
    uint16_t set_id();

    /**
     * Member functions
     */
    virtual void Pack  (byte *pack) = 0;
    virtual void Unpack(byte *pack) = 0;
};

class GenericSensor : public Sensor<std::vector<byte>> {

public:
    GenericSensor (uint8_t id, uint8_t num_bytes) {
        _data.resize(num_bytes)
    }
}

#endif