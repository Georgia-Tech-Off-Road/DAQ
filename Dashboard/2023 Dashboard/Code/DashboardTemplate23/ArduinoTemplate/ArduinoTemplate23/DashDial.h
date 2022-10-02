#ifndef DASHDIAL_H
#define DASHDIAL_H

#include <TLC5952.h>
#include <ServoControl.h>

class DashDial {
private:
    TLC5952 &_driver;
    ServoControl     &_servo;
    const  int16_t _min;
    const  int16_t _max;
    const uint16_t _min_brightness;
    const uint16_t _max_brightness;
    const uint16_t _min_servo;
    const uint16_t _max_servo;
    uint16_t _setting;

public:
    DashDial(TLC5952 &driver, ServoControl &servo, const int16_t min, const int16_t max, const uint16_t min_brightness, const uint16_t max_brightness, const int16_t min_servo, const int16_t max_servo);
    void begin();
    void update();
    void set(const int16_t setting);
    void calibrate_servo(uint8_t passes);
};

#endif
