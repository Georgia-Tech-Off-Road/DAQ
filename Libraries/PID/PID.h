#ifndef HARDWARELIBRARIES_PID_H
#define HARDWARELIBRARIES_PID_H

/**
 * PID.h
 * PID's are used for converting error into usable power output.
 * P - Proportion --> adding power as a proportion of the error
 * I - Integral   --> adding power as a proportion of the integral of error, leveling off any error present over longer periods of time.
 * D - Derivative --> removing power as a proportion of the derivative of error, preventing possible overshoots and restricting too large changes in error.
 */

#include <Arduino.h>

class PID {
private:
    float kP_, kI_, kD_;
    uint32_t old_time_;
    float integral_, integral_cap_;
    float derivative_, prev_error_;
    float min_derivative_, max_derivative_;
    int16_t min_power_, max_power_;
    int16_t min_threshold_, max_threshold_;

public:
    /**
     * The constructor defines the PID constants and tells the PID how long the loop is that it is called in.
     */
    PID();
    PID(float kP, float kI, float kD, int16_t min_power, int16_t max_power);

    void set_constants(float kP, float kI, float kD);
    void set_bounds(int16_t min_power, int16_t max_power);
    void set_integral_cap(float integral_cap);
    void set_derivative_bounds(float min_derivative, float max_derivative);
    void set_power_bounds(int16_t min_power, int16_t max_power);
    void set_threshold_bounds(int16_t min_threshold, int16_t max_threshold);

    /**
     * The step function is an instantaneous step that calculates and returns a new power value.
     * This needs to be called within a closed loop where the parameter error is constantly updated.
     * PID output is in the range from min_power to max_power.
     */
    int16_t step(float error);
};

#endif
