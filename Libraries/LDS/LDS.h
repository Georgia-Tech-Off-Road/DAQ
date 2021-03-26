#ifndef __LDS_H__
#define __LDS_H__

/**
 * LDS.h
 * A wrapper to work with any analog LDS.
 */
#include <Sensor.h>
#include <Arduino.h>

class LDS : public Sensor<uint8_t> {
private:
    /**
     * 
     */
    uint8_t _inputPin;
    uint8_t _travelMM;
    float _travelIn;
    bool _isReversed;

    uint8_t read_position();

public:
    /**
     * Constructor for the LDS sensor
     */
    LDS(uint8_t inputPin, uint8_t travelMM, bool isReversed = false);

    /**
     * Configures hardware; required in setup
     */
    void begin();

    /**
     * Returns position to nearest milimeter
     */
    uint8_t getPositionMM();

    /**
     * Returns position in inches to 3 decimal places
     */
    float getPositionInches();

    /**
     * Returns the raw analog reading of the LDS sensor
     */
    uint8_t getRawAnalog();

    const uint8_t& get_data();

    void pack(byte* pack); 
    void unpack(const byte* pack);
};

#endif
