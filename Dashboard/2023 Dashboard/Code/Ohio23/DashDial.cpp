#include "DashDial.h"

DashDial::DashDial(TLC5952 &driver, ServoControl &servo, const int16_t min, const int16_t max, const uint16_t min_brightness, const uint16_t max_brightness, const int16_t min_servo, const int16_t max_servo) :
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
//    _driver.set_control_data(127, 127, 127);
    _driver.set_control_data(1, 1, 1);
    _servo.begin();
}

void DashDial::update() {
    const uint16_t diff = _max - _min;
    const uint16_t setting_diff = _setting - _min;
    const uint8_t to_led = 23 * setting_diff / diff;
    
    _driver.set_on_off_data(16777216 - (pow(2, 23 - to_led)));

    _driver.write();

    const uint16_t servo_diff = _max_servo - _min_servo;
    _servo.writeDeg(_min_servo + servo_diff * setting_diff / diff);
}

void DashDial::set(const int16_t setting){
    _setting = setting;
    if(_setting > _max) _setting = _max;
    if(_setting < _min) _setting = _min;
}

void DashDial::calibrate_servo(uint8_t passes){
    do {
        Serial.println("Calibrating");
        _servo.writeDeg(_min_servo);
        delay(4000);
        _servo.writeDeg(_max_servo);
        delay(4000);
    } while (--passes > 0);
}
