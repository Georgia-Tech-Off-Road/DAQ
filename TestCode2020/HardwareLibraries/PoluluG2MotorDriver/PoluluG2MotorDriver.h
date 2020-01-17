#ifndef HARDWARELIBRARIES_POLULUG2MOTORDRIVER_H
#define HARDWARELIBRARIES_POLULUG2MOTORDRIVER_H

#include <Arduino.h>
#include <PID.h>

class PoluluG2MotorDriver {
private:
    uint8_t p_pwm_, p_dir_;
    PID pid_;

public:
    PoluluG2MotorDriver();
    PoluluG2MotorDriver(float kP, float kI, float kD, int16_t min_power, int16_t max_power);

    void begin(uint8_t p_pwm, uint8_t p_dir);
    void set_power(int16_t power);
    void step_pid(float error);
};


#endif