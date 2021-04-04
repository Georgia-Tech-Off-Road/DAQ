#ifndef TLC5947_CPP
#define TLC5947_CPP

#include "TLC5947.h"

/**
 * Basic constructor.
 */
TLC5947::TLC5947(uint8_t _o_sin, uint8_t _o_sclk, uint8_t _o_xlat, uint8_t _T_sclk){
    o_sin  = _o_sin;
    o_sclk = _o_sclk;
    o_xlat = _o_xlat;
    T_sclk = _T_sclk;
}

/**
 * Sets up pins and variables used within its functionality.
 * To be run in setup() function.
 */
void TLC5947::begin(){
    pinMode(o_sin , OUTPUT);
    pinMode(o_sclk, OUTPUT);
    pinMode(o_xlat, OUTPUT);

    // Start all LEDs at 0.
    writeAll(0);

    sin_state  = 0;
    sclk_state = 0;
    xlat_state = 0;
    digitalWriteFast(o_sin, sin_state);
    digitalWriteFast(o_sclk, sclk_state);
    digitalWriteFast(o_xlat, xlat_state);

    led_counter = 23;
    bit_counter = 12; // First clock cycle will decrement without writing.
}

void TLC5947::update(){
    uint32_t time = micros();
    uint32_t dt = abs(time - sclk_last_rising);

    // Handle sclk signal.
    if(dt >= T_sclk && !xlat_state){
        // Rising edge.
        sclk_state = 1;
        digitalWriteFast(o_sclk, sclk_state);
        sclk_last_rising = time;

        led_counter = (bit_counter == 0 && led_counter > 0) ? led_counter - 1 : 23;
        bit_counter = (bit_counter > 0) ? bit_counter - 1 : 11;

        new_sin_state = led_pwm[led_counter] & (1 << bit_counter);
    } else if(sclk_state && (dt >= T_sclk/2)){
        // Falling edge.
        sclk_state = 0;
        digitalWriteFast(o_sclk, sclk_state);
    }

    // Handle sin signal.
    if(dt >= (T_sclk/4) && sin_state != new_sin_state){
        // Write new state when different than curr state.
        sin_state = new_sin_state;
        digitalWriteFast(o_sin, sin_state);
    }

    // Handle xlat signal. 
    // Rising on 23, 11 because bit 0, 0 needs to shift into register.
    if(led_counter == 23 && bit_counter == 11 && dt >= (T_sclk/3) && !xlat_state){
        // Rising edge.
        xlat_state = 1;
        digitalWriteFast(o_xlat, xlat_state);
    } else if(!(led_counter == 23 && bit_counter == 11) || (xlat_state && dt >= (2*T_sclk/3))){
        // Falling edge.
        xlat_state = 0;
        digitalWriteFast(o_xlat, xlat_state);
    }
}

void TLC5947::writePWM(uint8_t led, uint16_t pwm){
    led_pwm[led] = pwm;
}

void TLC5947::writeAll(uint16_t pwm){
    for(int i = 0; i < 24; ++i){
        writePWM(i, pwm);
    }
}

void TLC5947::writeAllInd(uint16_t pwm[24]){
    memcpy(led_pwm, pwm, 48);
}

#endif