#ifndef __PRESSURESENSOR_H__
#define __PRESSURESENSOR_H__

/**
 * PressureSensor.h
 * A wrapper to work with an analog pressure sensor (MLH02KPSB06A Honeywell sensor).
 */
#include <Arduino.h>

class PressureSensor {
private:
    /**
     * 
     */
    uint8_t _inputPin;
    uint16_t _scale;
    uint16_t _offset;
    bool _isReversed;

public:
    /**
     * Constructor for the PressureSensor sensor
     */
    PressureSensor(int inputPin, uint16_t scale, uint16_t offset = 0);

    /**
     * Configures hardware; required in setup
     */
    void begin();

    /**
     * Returns pressure in PSI
     */
    uint16_t getPressurePSI();

    /**
     * Returns the raw analog reading of the PressureSensor sensor
     */
    int getRawAnalog();



};

#endif
