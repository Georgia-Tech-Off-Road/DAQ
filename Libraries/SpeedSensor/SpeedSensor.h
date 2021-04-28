/* SpeedSensor Library, for measuring both the speed and position of
 * single input or quadrature speed sensors.
 * Based on the Encoder Library written by Paul Stoffregen.
 */

/* Encoder Library, for measuring quadrature encoded signals
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 * Copyright (c) 2011,2013 PJRC.COM, LLC - Paul Stoffregen <paul@pjrc.com>
 *
 * Version 1.2 - fix -2 bug in C-only code
 * Version 1.1 - expand to support boards with up to 60 interrupts
 * Version 1.0 - initial release
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef SpeedSensor_h_
#define SpeedSensor_h_


#include "Arduino.h"
#include "utility/direct_pin_read.h"
#include <Sensor.h>

#if defined(ENCODER_USE_INTERRUPTS) || !defined(ENCODER_DO_NOT_USE_INTERRUPTS)
#define ENCODER_USE_INTERRUPTS
#define ENCODER_ARGLIST_SIZE CORE_NUM_INTERRUPT
#include "utility/interrupt_pins.h"
#else
#define ENCODER_ARGLIST_SIZE 0
#endif

// Will average data points over a a certain amount of time (or all the data
// point, whichever is lesser). Will average over a minimum of two data points
#define NUM_MEASUREMENTS 100
#define TIME_AVERAGING 100000 //microseconds



// All the data needed by interrupts is consolidated into this ugly struct
// to facilitate assembly language optimizing of the speed critical update.
// The assembly code uses auto-incrementing addressing modes, so the struct
// must remain in exactly this order.
typedef struct {
	volatile IO_REG_TYPE * pin1_register;
	volatile IO_REG_TYPE * pin2_register;
	IO_REG_TYPE            pin1_bitmask;
	IO_REG_TYPE            pin2_bitmask;
	uint8_t                state;
	int32_t                position;
    uint32_t             * times;
    uint8_t                time_pos;
} Encoder_internal_state_t;

typedef struct {
    uint32_t position;
    uint16_t speed;
} speed_sensor_data_t;

class SpeedSensor : public Sensor<speed_sensor_data_t>
{
public:
    // Set pin2 to 255 if it is not used
	SpeedSensor(uint16_t ppr, uint8_t pin1, uint8_t pin2 = 255) {
        _ppr = ppr;
        _pack_bytes = 6;

        pinMode(pin1, INPUT_PULLUP);
        _encoder.pin1_register = PIN_TO_BASEREG(pin1);
		_encoder.pin1_bitmask = PIN_TO_BITMASK(pin1);
        _encoder.position = 0;
        _encoder.times = new uint32_t [NUM_MEASUREMENTS];
        _encoder.time_pos = 0;
        // allow time for a passive R-C filter to charge
		// through the pullup resistors, before reading
		// the initial state
		delayMicroseconds(2000);
        uint8_t s = 0;
        if (DIRECT_PIN_READ(_encoder.pin1_register, _encoder.pin1_bitmask)) s |= 1;

        if (pin2 != 255){
            pinMode(pin2, INPUT_PULLUP);
            _encoder.pin2_register = PIN_TO_BASEREG(pin2);
		    _encoder.pin2_bitmask = PIN_TO_BITMASK(pin2);
            if (DIRECT_PIN_READ(_encoder.pin2_register, _encoder.pin2_bitmask)) s |= 2;
        }
        else{
            _encoder.pin2_bitmask = 255;
        }
		_encoder.state = s;
        attach_interrupt(pin1, &_encoder, CHANGE);
        if (pin2 != 255) attach_interrupt(pin2, &_encoder, CHANGE);
	}

    inline int32_t get_position() {
        noInterrupts();
        int32_t ret = _encoder.position;
        interrupts();
        return ret;
    }
    inline int32_t reset_position() {
        noInterrupts();
        int32_t ret = _encoder.position;
        _encoder.position = 0;
        interrupts();
        return ret;
    }
    inline void write_position(int32_t p) {
        noInterrupts();
        _encoder.position = p;
        interrupts();
    }    
    inline uint16_t get_speed(){
        noInterrupts();
        uint16_t rpm = 0;
        
        uint32_t time_deltas[NUM_MEASUREMENTS - 1]; // With 10 measurements there are 9 deltas to be calculated
        uint32_t curr_time_delta;

        // find all of the time deltas
        // TODO: handle the case where the time has wrapped
        uint8_t delta_pos = 0;
        for (uint8_t i = _encoder.time_pos; i < _encoder.time_pos + NUM_MEASUREMENTS - 1; i++) {
            time_deltas[delta_pos] = _encoder.times[(i+2)%NUM_MEASUREMENTS] - _encoder.times[(i+1)%NUM_MEASUREMENTS];
            delta_pos++;
        }
        curr_time_delta = micros() - _encoder.times[_encoder.time_pos];

        uint32_t sum = 0;
        uint8_t i = 0;
        // average the numbers summed
        while ((i < NUM_MEASUREMENTS && sum < TIME_AVERAGING) || i < 2){
            sum += time_deltas[i];
            i++;
        }
        // If measurement hasn't been taken in a while, assume it is slowing down
        if (curr_time_delta > time_deltas[0] && curr_time_delta > time_deltas[1]){
            rpm = 60*1000*1000/(curr_time_delta)/_ppr;
        } else {
            rpm = 60*1000*1000/(sum/i)/_ppr;
        }

        interrupts();
        return rpm;
    }
    // TODO: Add if statements to make these be able to only send speed or position
    const speed_sensor_data_t& get_data(){
        if(_type == ACTIVE){
            _data.position = get_position();
            _data.speed = get_speed();
        }
        return _data;
    }
    void pack(byte* pack){
        get_data();
        *((uint32_t*) pack) = _data.position;
        *((uint16_t*)(pack + 4)) = _data.speed;        
    }
    void unpack(const byte* pack){
        _data.position = *((uint32_t*) pack);
        _data.speed = *((uint16_t*)(pack + 4));
    }

private:
	Encoder_internal_state_t _encoder;
    uint16_t _ppr;

public:
	static Encoder_internal_state_t * interruptArgs[ENCODER_ARGLIST_SIZE];

/*
Quadrature mode           _______         _______       
              Pin1 ______|       |_______|       |______ Pin1
negative <---         _______         _______         __      --> positive
              Pin2 __|       |_______|       |_______|   Pin2

        new	new	old	old
        p2  p1  p2  p1	Result
        --- --- --- ---	------
        0	0	0	0	no movement
        0	0	0	1	+1
        0	0	1	0	-1
        0	0	1	1	+2  (assume pin1 edges only)
        0	1	0	0	-1
        0	1	0	1	no movement
        0	1	1	0	-2  (assume pin1 edges only)
        0	1	1	1	+1
        1	0	0	0	+1
        1	0	0	1	-2  (assume pin1 edges only)
        1	0	1	0	no movement
        1	0	1	1	-1
        1	1	0	0	+2  (assume pin1 edges only)
        1	1	0	1	-1
        1	1	1	0	+1
        1	1	1	1	no movement

*/

public:
	// update() is not meant to be called from outside Encoder,
	// but it is public to allow static interrupt routines.
	// DO NOT call update() directly from sketches.
	static void update(Encoder_internal_state_t *arg) {
        if(arg->pin2_bitmask==255){
            uint8_t p1val = DIRECT_PIN_READ(arg->pin1_register, arg->pin1_bitmask);
            if (p1val && !arg->state){
                arg->position++;
                arg->time_pos = arg->time_pos == NUM_MEASUREMENTS - 1 ? 0 : arg->time_pos + 1;
                arg->times[arg->time_pos] = micros();            
            }
            arg->state = p1val;
        }
        else{
            uint8_t p1val = DIRECT_PIN_READ(arg->pin1_register, arg->pin1_bitmask);
            uint8_t p2val = DIRECT_PIN_READ(arg->pin2_register, arg->pin2_bitmask);
            arg->time_pos = arg->time_pos == NUM_MEASUREMENTS - 1 ? 0 : arg->time_pos + 1;
            arg->times[arg->time_pos] = micros();            
            uint8_t state = arg->state & 3;
            if (p1val) state |= 4;
            if (p2val) state |= 8;
            arg->state = (state >> 2);
            switch (state) {
                case 1: case 7: case 8: case 14:
                    arg->position++;
                    return;
                case 2: case 4: case 11: case 13:
                    arg->position--;
                    return;
                case 3: case 12:
                    arg->position += 2;
                    return;
                case 6: case 9:
                    arg->position -= 2;
                    return;
            }
        }
	}
private:

// this giant function is an unfortunate consequence of Arduino's
// attachInterrupt function not supporting any way to pass a pointer
// or other context to the attached function.
static uint8_t attach_interrupt(uint8_t pin, Encoder_internal_state_t *state, uint8_t mode) {
    switch (pin) {
    #ifdef CORE_INT0_PIN
        case CORE_INT0_PIN:
            interruptArgs[0] = state;
            attachInterrupt(0, isr0, mode);
            break;
    #endif
    #ifdef CORE_INT1_PIN
        case CORE_INT1_PIN:
            interruptArgs[1] = state;
            attachInterrupt(1, isr1, mode);
            break;
    #endif
    #ifdef CORE_INT2_PIN
        case CORE_INT2_PIN:
            interruptArgs[2] = state;
            attachInterrupt(2, isr2, mode);
            break;
    #endif
    #ifdef CORE_INT3_PIN
        case CORE_INT3_PIN:
            interruptArgs[3] = state;
            attachInterrupt(3, isr3, mode);
            break;
    #endif
    #ifdef CORE_INT4_PIN
        case CORE_INT4_PIN:
            interruptArgs[4] = state;
            attachInterrupt(4, isr4, mode);
            break;
    #endif
    #ifdef CORE_INT5_PIN
        case CORE_INT5_PIN:
            interruptArgs[5] = state;
            attachInterrupt(5, isr5, mode);
            break;
    #endif
    #ifdef CORE_INT6_PIN
        case CORE_INT6_PIN:
            interruptArgs[6] = state;
            attachInterrupt(6, isr6, mode);
            break;
    #endif
    #ifdef CORE_INT7_PIN
        case CORE_INT7_PIN:
            interruptArgs[7] = state;
            attachInterrupt(7, isr7, mode);
            break;
    #endif
    #ifdef CORE_INT8_PIN
        case CORE_INT8_PIN:
            interruptArgs[8] = state;
            attachInterrupt(8, isr8, mode);
            break;
    #endif
    #ifdef CORE_INT9_PIN
        case CORE_INT9_PIN:
            interruptArgs[9] = state;
            attachInterrupt(9, isr9, mode);
            break;
    #endif
    #ifdef CORE_INT10_PIN
        case CORE_INT10_PIN:
            interruptArgs[10] = state;
            attachInterrupt(10, isr10, mode);
            break;
    #endif
    #ifdef CORE_INT11_PIN
        case CORE_INT11_PIN:
            interruptArgs[11] = state;
            attachInterrupt(11, isr11, mode);
            break;
    #endif
    #ifdef CORE_INT12_PIN
        case CORE_INT12_PIN:
            interruptArgs[12] = state;
            attachInterrupt(12, isr12, mode);
            break;
    #endif
    #ifdef CORE_INT13_PIN
        case CORE_INT13_PIN:
            interruptArgs[13] = state;
            attachInterrupt(13, isr13, mode);
            break;
    #endif
    #ifdef CORE_INT14_PIN
        case CORE_INT14_PIN:
            interruptArgs[14] = state;
            attachInterrupt(14, isr14, mode);
            break;
    #endif
    #ifdef CORE_INT15_PIN
        case CORE_INT15_PIN:
            interruptArgs[15] = state;
            attachInterrupt(15, isr15, mode);
            break;
    #endif
    #ifdef CORE_INT16_PIN
        case CORE_INT16_PIN:
            interruptArgs[16] = state;
            attachInterrupt(16, isr16, mode);
            break;
    #endif
    #ifdef CORE_INT17_PIN
        case CORE_INT17_PIN:
            interruptArgs[17] = state;
            attachInterrupt(17, isr17, mode);
            break;
    #endif
    #ifdef CORE_INT18_PIN
        case CORE_INT18_PIN:
            interruptArgs[18] = state;
            attachInterrupt(18, isr18, mode);
            break;
    #endif
    #ifdef CORE_INT19_PIN
        case CORE_INT19_PIN:
            interruptArgs[19] = state;
            attachInterrupt(19, isr19, mode);
            break;
    #endif
    #ifdef CORE_INT20_PIN
        case CORE_INT20_PIN:
            interruptArgs[20] = state;
            attachInterrupt(20, isr20, mode);
            break;
    #endif
    #ifdef CORE_INT21_PIN
        case CORE_INT21_PIN:
            interruptArgs[21] = state;
            attachInterrupt(21, isr21, mode);
            break;
    #endif
    #ifdef CORE_INT22_PIN
        case CORE_INT22_PIN:
            interruptArgs[22] = state;
            attachInterrupt(22, isr22, mode);
            break;
    #endif
    #ifdef CORE_INT23_PIN
        case CORE_INT23_PIN:
            interruptArgs[23] = state;
            attachInterrupt(23, isr23, mode);
            break;
    #endif
    #ifdef CORE_INT24_PIN
        case CORE_INT24_PIN:
            interruptArgs[24] = state;
            attachInterrupt(24, isr24, mode);
            break;
    #endif
    #ifdef CORE_INT25_PIN
        case CORE_INT25_PIN:
            interruptArgs[25] = state;
            attachInterrupt(25, isr25, mode);
            break;
    #endif
    #ifdef CORE_INT26_PIN
        case CORE_INT26_PIN:
            interruptArgs[26] = state;
            attachInterrupt(26, isr26, mode);
            break;
    #endif
    #ifdef CORE_INT27_PIN
        case CORE_INT27_PIN:
            interruptArgs[27] = state;
            attachInterrupt(27, isr27, mode);
            break;
    #endif
    #ifdef CORE_INT28_PIN
        case CORE_INT28_PIN:
            interruptArgs[28] = state;
            attachInterrupt(28, isr28, mode);
            break;
    #endif
    #ifdef CORE_INT29_PIN
        case CORE_INT29_PIN:
            interruptArgs[29] = state;
            attachInterrupt(29, isr29, mode);
            break;
    #endif

    #ifdef CORE_INT30_PIN
        case CORE_INT30_PIN:
            interruptArgs[30] = state;
            attachInterrupt(30, isr30, mode);
            break;
    #endif
    #ifdef CORE_INT31_PIN
        case CORE_INT31_PIN:
            interruptArgs[31] = state;
            attachInterrupt(31, isr31, mode);
            break;
    #endif
    #ifdef CORE_INT32_PIN
        case CORE_INT32_PIN:
            interruptArgs[32] = state;
            attachInterrupt(32, isr32, mode);
            break;
    #endif
    #ifdef CORE_INT33_PIN
        case CORE_INT33_PIN:
            interruptArgs[33] = state;
            attachInterrupt(33, isr33, mode);
            break;
    #endif
    #ifdef CORE_INT34_PIN
        case CORE_INT34_PIN:
            interruptArgs[34] = state;
            attachInterrupt(34, isr34, mode);
            break;
    #endif
    #ifdef CORE_INT35_PIN
        case CORE_INT35_PIN:
            interruptArgs[35] = state;
            attachInterrupt(35, isr35, mode);
            break;
    #endif
    #ifdef CORE_INT36_PIN
        case CORE_INT36_PIN:
            interruptArgs[36] = state;
            attachInterrupt(36, isr36, mode);
            break;
    #endif
    #ifdef CORE_INT37_PIN
        case CORE_INT37_PIN:
            interruptArgs[37] = state;
            attachInterrupt(37, isr37, mode);
            break;
    #endif
    #ifdef CORE_INT38_PIN
        case CORE_INT38_PIN:
            interruptArgs[38] = state;
            attachInterrupt(38, isr38, mode);
            break;
    #endif
    #ifdef CORE_INT39_PIN
        case CORE_INT39_PIN:
            interruptArgs[39] = state;
            attachInterrupt(39, isr39, mode);
            break;
    #endif
    #ifdef CORE_INT40_PIN
        case CORE_INT40_PIN:
            interruptArgs[40] = state;
            attachInterrupt(40, isr40, mode);
            break;
    #endif
    #ifdef CORE_INT41_PIN
        case CORE_INT41_PIN:
            interruptArgs[41] = state;
            attachInterrupt(41, isr41, mode);
            break;
    #endif
    #ifdef CORE_INT42_PIN
        case CORE_INT42_PIN:
            interruptArgs[42] = state;
            attachInterrupt(42, isr42, mode);
            break;
    #endif
    #ifdef CORE_INT43_PIN
        case CORE_INT43_PIN:
            interruptArgs[43] = state;
            attachInterrupt(43, isr43, mode);
            break;
    #endif
    #ifdef CORE_INT44_PIN
        case CORE_INT44_PIN:
            interruptArgs[44] = state;
            attachInterrupt(44, isr44, mode);
            break;
    #endif
    #ifdef CORE_INT45_PIN
        case CORE_INT45_PIN:
            interruptArgs[45] = state;
            attachInterrupt(45, isr45, mode);
            break;
    #endif
    #ifdef CORE_INT46_PIN
        case CORE_INT46_PIN:
            interruptArgs[46] = state;
            attachInterrupt(46, isr46, mode);
            break;
    #endif
    #ifdef CORE_INT47_PIN
        case CORE_INT47_PIN:
            interruptArgs[47] = state;
            attachInterrupt(47, isr47, mode);
            break;
    #endif
    #ifdef CORE_INT48_PIN
        case CORE_INT48_PIN:
            interruptArgs[48] = state;
            attachInterrupt(48, isr48, mode);
            break;
    #endif
    #ifdef CORE_INT49_PIN
        case CORE_INT49_PIN:
            interruptArgs[49] = state;
            attachInterrupt(49, isr49, mode);
            break;
    #endif
    #ifdef CORE_INT50_PIN
        case CORE_INT50_PIN:
            interruptArgs[50] = state;
            attachInterrupt(50, isr50, mode);
            break;
    #endif
    #ifdef CORE_INT51_PIN
        case CORE_INT51_PIN:
            interruptArgs[51] = state;
            attachInterrupt(51, isr51, mode);
            break;
    #endif
    #ifdef CORE_INT52_PIN
        case CORE_INT52_PIN:
            interruptArgs[52] = state;
            attachInterrupt(52, isr52, mode);
            break;
    #endif
    #ifdef CORE_INT53_PIN
        case CORE_INT53_PIN:
            interruptArgs[53] = state;
            attachInterrupt(53, isr53, mode);
            break;
    #endif
    #ifdef CORE_INT54_PIN
        case CORE_INT54_PIN:
            interruptArgs[54] = state;
            attachInterrupt(54, isr54, mode);
            break;
    #endif
    #ifdef CORE_INT55_PIN
        case CORE_INT55_PIN:
            interruptArgs[55] = state;
            attachInterrupt(55, isr55, mode);
            break;
    #endif
    #ifdef CORE_INT56_PIN
        case CORE_INT56_PIN:
            interruptArgs[56] = state;
            attachInterrupt(56, isr56, mode);
            break;
    #endif
    #ifdef CORE_INT57_PIN
        case CORE_INT57_PIN:
            interruptArgs[57] = state;
            attachInterrupt(57, isr57, mode);
            break;
    #endif
    #ifdef CORE_INT58_PIN
        case CORE_INT58_PIN:
            interruptArgs[58] = state;
            attachInterrupt(58, isr58, mode);
            break;
    #endif
    #ifdef CORE_INT59_PIN
        case CORE_INT59_PIN:
            interruptArgs[59] = state;
            attachInterrupt(59, isr59, mode);
            break;
    #endif
        default:
            return 0;
    }
    return 1;
}


#ifdef CORE_INT0_PIN
static void isr0(void) { update(interruptArgs[0]); }
#endif
#ifdef CORE_INT1_PIN
static void isr1(void) { update(interruptArgs[1]); }
#endif
#ifdef CORE_INT2_PIN
static void isr2(void) { update(interruptArgs[2]); }
#endif
#ifdef CORE_INT3_PIN
static void isr3(void) { update(interruptArgs[3]); }
#endif
#ifdef CORE_INT4_PIN
static void isr4(void) { update(interruptArgs[4]); }
#endif
#ifdef CORE_INT5_PIN
static void isr5(void) { update(interruptArgs[5]); }
#endif
#ifdef CORE_INT6_PIN
static void isr6(void) { update(interruptArgs[6]); }
#endif
#ifdef CORE_INT7_PIN
static void isr7(void) { update(interruptArgs[7]); }
#endif
#ifdef CORE_INT8_PIN
static void isr8(void) { update(interruptArgs[8]); }
#endif
#ifdef CORE_INT9_PIN
static void isr9(void) { update(interruptArgs[9]); }
#endif
#ifdef CORE_INT10_PIN
static void isr10(void) { update(interruptArgs[10]); }
#endif
#ifdef CORE_INT11_PIN
static void isr11(void) { update(interruptArgs[11]); }
#endif
#ifdef CORE_INT12_PIN
static void isr12(void) { update(interruptArgs[12]); }
#endif
#ifdef CORE_INT13_PIN
static void isr13(void) { update(interruptArgs[13]); }
#endif
#ifdef CORE_INT14_PIN
static void isr14(void) { update(interruptArgs[14]); }
#endif
#ifdef CORE_INT15_PIN
static void isr15(void) { update(interruptArgs[15]); }
#endif
#ifdef CORE_INT16_PIN
static void isr16(void) { update(interruptArgs[16]); }
#endif
#ifdef CORE_INT17_PIN
static void isr17(void) { update(interruptArgs[17]); }
#endif
#ifdef CORE_INT18_PIN
static void isr18(void) { update(interruptArgs[18]); }
#endif
#ifdef CORE_INT19_PIN
static void isr19(void) { update(interruptArgs[19]); }
#endif
#ifdef CORE_INT20_PIN
static void isr20(void) { update(interruptArgs[20]); }
#endif
#ifdef CORE_INT21_PIN
static void isr21(void) { update(interruptArgs[21]); }
#endif
#ifdef CORE_INT22_PIN
static void isr22(void) { update(interruptArgs[22]); }
#endif
#ifdef CORE_INT23_PIN
static void isr23(void) { update(interruptArgs[23]); }
#endif
#ifdef CORE_INT24_PIN
static void isr24(void) { update(interruptArgs[24]); }
#endif
#ifdef CORE_INT25_PIN
static void isr25(void) { update(interruptArgs[25]); }
#endif
#ifdef CORE_INT26_PIN
static void isr26(void) { update(interruptArgs[26]); }
#endif
#ifdef CORE_INT27_PIN
static void isr27(void) { update(interruptArgs[27]); }
#endif
#ifdef CORE_INT28_PIN
static void isr28(void) { update(interruptArgs[28]); }
#endif
#ifdef CORE_INT29_PIN
static void isr29(void) { update(interruptArgs[29]); }
#endif
#ifdef CORE_INT30_PIN
static void isr30(void) { update(interruptArgs[30]); }
#endif
#ifdef CORE_INT31_PIN
static void isr31(void) { update(interruptArgs[31]); }
#endif
#ifdef CORE_INT32_PIN
static void isr32(void) { update(interruptArgs[32]); }
#endif
#ifdef CORE_INT33_PIN
static void isr33(void) { update(interruptArgs[33]); }
#endif
#ifdef CORE_INT34_PIN
static void isr34(void) { update(interruptArgs[34]); }
#endif
#ifdef CORE_INT35_PIN
static void isr35(void) { update(interruptArgs[35]); }
#endif
#ifdef CORE_INT36_PIN
static void isr36(void) { update(interruptArgs[36]); }
#endif
#ifdef CORE_INT37_PIN
static void isr37(void) { update(interruptArgs[37]); }
#endif
#ifdef CORE_INT38_PIN
static void isr38(void) { update(interruptArgs[38]); }
#endif
#ifdef CORE_INT39_PIN
static void isr39(void) { update(interruptArgs[39]); }
#endif
#ifdef CORE_INT40_PIN
static void isr40(void) { update(interruptArgs[40]); }
#endif
#ifdef CORE_INT41_PIN
static void isr41(void) { update(interruptArgs[41]); }
#endif
#ifdef CORE_INT42_PIN
static void isr42(void) { update(interruptArgs[42]); }
#endif
#ifdef CORE_INT43_PIN
static void isr43(void) { update(interruptArgs[43]); }
#endif
#ifdef CORE_INT44_PIN
static void isr44(void) { update(interruptArgs[44]); }
#endif
#ifdef CORE_INT45_PIN
static void isr45(void) { update(interruptArgs[45]); }
#endif
#ifdef CORE_INT46_PIN
static void isr46(void) { update(interruptArgs[46]); }
#endif
#ifdef CORE_INT47_PIN
static void isr47(void) { update(interruptArgs[47]); }
#endif
#ifdef CORE_INT48_PIN
static void isr48(void) { update(interruptArgs[48]); }
#endif
#ifdef CORE_INT49_PIN
static void isr49(void) { update(interruptArgs[49]); }
#endif
#ifdef CORE_INT50_PIN
static void isr50(void) { update(interruptArgs[50]); }
#endif
#ifdef CORE_INT51_PIN
static void isr51(void) { update(interruptArgs[51]); }
#endif
#ifdef CORE_INT52_PIN
static void isr52(void) { update(interruptArgs[52]); }
#endif
#ifdef CORE_INT53_PIN
static void isr53(void) { update(interruptArgs[53]); }
#endif
#ifdef CORE_INT54_PIN
static void isr54(void) { update(interruptArgs[54]); }
#endif
#ifdef CORE_INT55_PIN
static void isr55(void) { update(interruptArgs[55]); }
#endif
#ifdef CORE_INT56_PIN
static void isr56(void) { update(interruptArgs[56]); }
#endif
#ifdef CORE_INT57_PIN
static void isr57(void) { update(interruptArgs[57]); }
#endif
#ifdef CORE_INT58_PIN
static void isr58(void) { update(interruptArgs[58]); }
#endif
#ifdef CORE_INT59_PIN
static void isr59(void) { update(interruptArgs[59]); }
#endif

};





#endif