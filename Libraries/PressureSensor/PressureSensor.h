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
    uint16_t _maxPressure;
    uint16_t _r1;
    uint16_t _r2;

public:
    /**
     * Constructor for the PressureSensor sensor
     */
    PressureSensor(int inputPin, uint16_t maxPressure, uint16_t r1, uint16_t r2);

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
    uint16_t getRawAnalog();

    uint16_t analogToPressure(uint16_t v_analog);



};

#endif
