#ifndef __HALLEFFECT_H__
#define __HALLEFFECT_H__

/**
 * HallEffect.h
 * A wrapper to work with a hall effect sensor.
 */

#include "FreqMeasureMulti.h"

class HallEffect {
private:
    /**
     * 
     */
    int inputPin;
    float currSpeed;
    long prevTime;
    const uint8_t toneWheelTeeth;
    uint16_t count;
    uint32_t sum;
    FreqMeasureMulti freq;

public:
    /**
     * Sets up the digital input pin for the sensor.
     */
    void setup(int _inputPin, uint8_t _toneWheelTeeth);

    /**
     * Speed is in RPM
     */
    int getSpeed(long currTime);

};

#endif