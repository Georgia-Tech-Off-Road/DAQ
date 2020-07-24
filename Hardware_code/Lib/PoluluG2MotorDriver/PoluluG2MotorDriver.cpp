#ifndef HARDWARELIBRARIES_POLULUG2MOTORDRIVER_CPP
#define HARDWARELIBRARIES_POLULUG2MOTORDRIVER_CPP

#include "PoluluG2MotorDriver.h"

PoluluG2MotorDriver::PoluluG2MotorDriver(){}
PoluluG2MotorDriver::PoluluG2MotorDriver(float kP, float kI, float kD, int16_t min_power = -255, int16_t max_power = 255):
    pid_(kP, kI, kD, min_power, max_power){}

void PoluluG2MotorDriver::begin(uint8_t p_pwm, int8_t p_dir){
    p_pwm_ = p_pwm;
    p_dir_ = p_dir;
    if(p_dir_ < 0){
        s_ = true;
        p_dir_ = -p_dir_;
    }
    pinMode(p_pwm_, OUTPUT);
    pinMode(p_dir_, OUTPUT);
}

void PoluluG2MotorDriver::set_power(int16_t power){
    if(power > 0){
        analogWrite(p_pwm_, power);
        digitalWrite(p_dir_, s_);
    } else if (power < 0){
        analogWrite(p_pwm_, abs(power));
        digitalWrite(p_dir_, !s_);
    } else {
        analogWrite(p_pwm_, LOW);
    }
}

void PoluluG2MotorDriver::step_pid(float error){
    int p__ = pid_.step(error);
    set_power(p__);
}

PID * PoluluG2MotorDriver::get_pid(){
    return &pid_;
}

#endif