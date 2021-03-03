#include "DashDial.h"

DashDial::DashDial(Adafruit_TLC5947 &driver, ServoControl &servo, const int16_t min, const int16_t max, const uint16_t min_brightness, const uint16_t max_brightness, const int16_t min_servo, const int16_t max_servo) :
    _driver(driver),
    _servo(servo),
    _min(min),
    _max(max),
    _min_brightness(min_brightness),
    _max_brightness(max_brightness),
    _min_servo(min_servo),
    _max_servo(max_servo),
    _setting(0) { }

void DashDial::begin() {
    _driver.begin();
    _servo.begin();
}

void DashDial::update() {
    const uint8_t diff = _max - _min;
    const uint8_t setting_diff = _setting - _min;
    const uint16_t brightness_diff = _max_brightness - _min_brightness;
    const uint8_t to_led = 23 * setting_diff / diff;

    for(uint8_t i = 0; i < to_led; ++i)
        _driver.setPWM(i, _max_brightness);

    _driver.setPWM(to_led, _min_brightness + brightness_diff * setting_diff / diff);

    for(uint8_t i = to_led + 1; i < 24; ++i)
        _driver.setPWM(i, _min_brightness);

    _driver.write();

    const uint16_t servo_diff = _max_servo - _min_servo;
    _servo.writeDeg(_min_servo + servo_diff * setting_diff / diff);
}

void DashDial::set(const int16_t setting){
    _setting = setting;
    if(_setting > _max) _setting = _max;
    if(_setting < _min) _setting = _min;
}

