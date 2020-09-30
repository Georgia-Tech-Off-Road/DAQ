#ifndef SERVOCONTROL_H
#define SERVOCONTROL_H

#include <Servo.h>

class ServoControl {
private:
    Servo servo;
    uint8_t pin;
    uint16_t degrees;
    uint16_t min_us;
    uint16_t longest_period;

public:
    ServoControl(uint8_t _pin, uint16_t _degrees);
    void begin();
    void writeDeg(uint16_t deg);
    void writeMicroseconds(uint16_t us);
};


#endif