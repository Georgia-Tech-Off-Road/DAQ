#ifndef DASHDIAL_H
#define DASHDIAL_H

#include <Adafruit_TLC5947.h>
#include <ServoControl.h>

class DashDial {
private:
    Adafruit_TLC5947 &_driver;
    ServoControl     &_servo;
    const  int16_t _min;
    const  int16_t _max;
    const uint16_t _min_brightness;
    const uint16_t _max_brightness;
    const uint16_t _min_servo;
    const uint16_t _max_servo;
    uint16_t _setting;

public:
    DashDial(Adafruit_TLC5947 &driver, ServoControl &servo, const int16_t min, const int16_t max, const uint16_t min_brightness, const uint16_t max_brightness, const int16_t min_servo, const int16_t max_servo);
    void begin();
    void update();
    void set(const int16_t setting);
};

#endif