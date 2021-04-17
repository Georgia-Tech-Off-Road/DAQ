#ifndef TLC5947_H
#define TLC5947_H

#include <Arduino.h>

// 30us

class TLC5947 {
private:
    /**
     * Pin outputs.
     */
    uint8_t  o_sin;
    uint8_t  o_sclk;
    uint8_t  o_xlat;

    uint16_t led_pwm [24];
    uint8_t  led_counter;
    uint8_t  bit_counter;

    /**
     * Period sclk runs at (in us)
     * sin will be clocked at the same period, phase offset 90 degrees.
     */
    uint16_t T_sclk;

    /**
     * Time since the last rising edge on sclk.
     */
    uint32_t sclk_last_rising;

    uint8_t  sin_state;
    uint8_t  new_sin_state;
    uint8_t  sclk_state;
    uint8_t  xlat_state;

public:
    TLC5947(uint8_t _o_sin, uint8_t _o_sclk, uint8_t _o_xlat, uint8_t _T_sclk);
    void begin();
    void update();
    void writePWM(uint8_t led, uint16_t pwm);
    void writeAll(uint16_t pwm);
    void writeAllInd(uint16_t pwm[24]);

};


#endif