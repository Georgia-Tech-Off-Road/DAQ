#ifndef HARDWARELIBRARIES_PID_CPP
#define HARDWARELIBRARIES_PID_CPP

/**
 * PID.cpp
 * PID's are used for converting error into usable power output.
 * P - Proportion --> adding power as a proportion of the error
 * I - Integral   --> adding power as a proportion of the integral of error, leveling off any error present over longer periods of time.
 * D - Derivative --> removing power as a proportion of the derivative of error, preventing possible overshoots and restricting too large changes in error.
 */

#include "PID.h"

// Dummy constructor in case PID is included in a class and not used
PID::PID(){}

PID::PID(float kP, float kI, float kD, int16_t min_power, int16_t max_power):
    kP_(kP),
    kI_(kI),
    kD_(kD),
    min_power_(min_power),
    max_power_(max_power) {
        old_time_ = micros();
    }

void PID::set_constants(float kP, float kI, float kD){
    kP_ = kP;
    kI_ = kI;
    kD_ = kD;
}

int16_t PID::step(float error){
    uint32_t new_time = micros();
    uint8_t dt = new_time - old_time_;
    old_time_ = new_time;

    // Riemann Sum
    integral_ += error * dt;

    // Slope Formula
    derivative_ = (error - prev_error_) / dt;
    prev_error_ = error;

    // Adding the three terms
    int16_t power = error * kP_ + integral_ * kI_ + derivative_ * kD_;

    // Capping the power at max_power
    power = (power >  max_power_) ?  max_power_ : power;
    power = (power < -max_power_) ? -max_power_ : power;
    return power;
} 

#endif
