#ifndef MAIN_DAQ_H
#define MAIN_DAQ_H

#include <Arduino.h>
#include "HallEffectSpeedSensor.h"
#include "BrakePressureTransducer.h"

#define THREEV3DIG_1 14
#define THREEV3DIG_2 5
#define THREEV3DIG_3 16
#define THREEV3DIG_4 6
#define THREEV3DIG_5 15
#define THREEV3DIG_6 2
#define THREEV3DIG_7 38
#define THREEV3DIG_8 3

#define RX_1 17
#define RX_2 39

#define THREEV3ANA_1 9
#define THREEV3ANA_2 22
#define THREEV3ANA_3 33
#define THREEV3ANA_4 23
#define THREEV3ANA_5 7
#define THREEV3ANA_6 4
#define THREEV3ANA_7 40
#define THREEV3ANA_8 32
#define THREEV3ANA_9 41
#define THREEV3ANA_10 8

#define SDA 18
#define SCL 19

#define MISO 12
#define MOSI 11
#define CS_1 13
#define CS_2 24
#define CS_3 25
#define CS_4 26
#define CS_5 27
#define CS_6 28
#define CS_7 29

#define FIVEVDIG_1 31
#define FIVEVDIG_2 30
#define FIVEVDIG_3 20
#define FIVEVDIG_4 21

////// FIND THE PIN NUMBERS FOR THESE OR SOMESTHINGS
// #define 5VANA_1 AIN0
// #define 5VANA_2 AIN2
// #define 5VANA_3 AIN1
// #define 5VANA_4 AIN3

// 3V3 Dig Pins (5, 2, 6, 3, 8, 4, 32, 33)

// TX (8) RX (7)

// 3V3 Analog Pins (14, 15, 16, 38, 17, 39, 22, 40, 23, 41)

// SDA (18) SDL (19)

// MISO (12), MOSI (11), CS (13, 24, 25, 26, 27, 28, 29)

// 5V Dig Pins (31, 20, 30, 21)

// 5V Analog Pins (AIN1-4) 

#endif