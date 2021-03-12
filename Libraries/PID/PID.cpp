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
        integral_cap_ = 0;
    }

void PID::set_constants(float kP, float kI, float kD){
    kP_ = kP;
    kI_ = kI;
    kD_ = kD;
}

void PID::set_integral_cap(float integral_cap){
    integral_cap_ = integral_cap;
}

void PID::set_derivative_bounds(float min_derivative, float max_derivative){
    min_derivative_ = min_derivative;
    max_derivative_ = max_derivative;
}

void PID::set_power_bounds(int16_t min_power, int16_t max_power){
    min_power_ = min_power;
    max_power_ = max_power;
}

void PID::set_threshold_bounds(int16_t min_threshold, int16_t max_threshold){
    min_threshold_ = min_threshold;
    max_threshold_ = max_threshold;
}

int16_t PID::step(float error){
    uint32_t new_time = micros();
    float dt = (new_time - old_time_) / 1000.0; // Microseconds --> Milliseconds to make constants simpler.
    old_time_ = new_time;

    // Riemann Sum
    integral_ += error * dt;
    integral_ = (integral_cap_ != 0 && integral_ >= integral_cap_) ? integral_cap_ : integral_;
    integral_ = (integral_cap_ != 0 && integral_ <= -integral_cap_) ? -integral_cap_ : integral_;
    integral_ = (error == 0) ? 0 : integral_;

    // Slope Formula
    derivative_ = (error - prev_error_) / dt;
    prev_error_ = error;
    derivative_ = (derivative_ > max_derivative_) ? max_derivative_ : derivative_;
    derivative_ = (derivative_ < min_derivative_) ? min_derivative_ : derivative_;

    // Adding the three terms
    int16_t power = error * kP_ + integral_ * kI_ + derivative_ * kD_;

    // Setting power to zero when within the threshold bounds
    power = ((power < max_threshold_ && power > 0) || (power > min_threshold_ && power < 0)) ? 0 : power;

    // Capping the power at max_power
    power = (power >  max_power_) ?  max_power_ : power;
    power = (power < -max_power_) ? -max_power_ : power;
    return power;
} 

#endif
